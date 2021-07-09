/* GStreamer 
 * Copyright (C) 2016 RidgeRun Engineering  Michael Gruner <michael.gruner@ridgerun.com> 
 *                                          Fabricio Quiros <fabricio.quiros@ridgerun.com
 * 
 * gstmetasrc.c:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
/**
 * SECTION:element-metasrc
 * @see_also: #GstMetaSrc
 *
 * Send different kinds of metadata buffers
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -e metasrc metadata="hello!" ! 'meta/x-klv' ! filesink sync=true async=true location=test.txt	
 * ]| Store metadata in a text file
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include <string.h>

#include "gstmetasrc.h"

GST_DEBUG_CATEGORY_STATIC (gst_meta_src_debug);
#define GST_CAT_DEFAULT gst_meta_src_debug

#define DEFAULT_PROP_METADATA NULL
#define DEFAULT_PROP_PERIOD   0
#define LAST_CHARACTER 1

#ifdef EVAL
#  define EVAL_TIME 5*60
#  define EVAL_START 5
#endif

enum
{
  SIGNAL_PUSH_METADATA,
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_METADATA,
  PROP_BINARY,
  PROP_PERIOD
};

static guint gst_meta_src_signals[LAST_SIGNAL] = { 0 };

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("meta/x-klv,parsed=true")
    );

#define gst_meta_src_parent_class parent_class
G_DEFINE_TYPE (GstMetaSrc, gst_meta_src, GST_TYPE_APP_SRC);

static void gst_meta_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);

static void gst_meta_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static void gst_meta_src_finalize (GObject * object);

static GstFlowReturn gst_meta_src_new_buffer (GstMetaSrc * src);
static GstFlowReturn gst_meta_src_new_buffer_meta (GstMetaSrc * src,
    gchar * metadata);
static GstFlowReturn gst_meta_src_new_buffer_binary (GstMetaSrc * src,
    GByteArray *binary);

static gboolean gst_meta_src_start (GstBaseSrc * base);

static gboolean gst_meta_src_stop (GstBaseSrc * base);

static gboolean gst_meta_src_timeout (gpointer user_data);

static gchar *gst_meta_src_time_format (const gchar * format);

static GstFlowReturn gst_meta_src_save_metadata (GstMetaSrc * src,
    gchar * metadata);
static GstFlowReturn gst_meta_src_save_binary (GstMetaSrc * src,
    GByteArray * binary);
static void gst_meta_src_disable_metadata (GstMetaSrc * src);
static void gst_meta_src_save_period (GstMetaSrc * src, guint period);

static GstFlowReturn gst_meta_src_push_metadata (GstMetaSrc *metasrc, guint8 *metadata, gsize size);

#ifdef EVAL
static GstStateChangeReturn gst_meta_src_change_state (GstElement * element,
    GstStateChange transition);
#endif

static void
gst_meta_src_class_init (GstMetaSrcClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSrcClass *gstbasesrc_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gstelement_class = GST_ELEMENT_CLASS (klass);
  gstbasesrc_class = GST_BASE_SRC_CLASS (klass);

  GST_DEBUG_CATEGORY_INIT (gst_meta_src_debug, "metasrc", 0, "metasrc element");

  gstbasesrc_class->start = GST_DEBUG_FUNCPTR (gst_meta_src_start);
  gstbasesrc_class->stop = GST_DEBUG_FUNCPTR (gst_meta_src_stop);
  gobject_class->set_property = gst_meta_src_set_property;
  gobject_class->get_property = gst_meta_src_get_property;
  gobject_class->finalize = gst_meta_src_finalize;

  g_object_class_install_property (gobject_class, PROP_METADATA,
      g_param_spec_string ("metadata", "The string metadata to push",
          "A string to push as metadata. This string "
          "may contain time formats as specified in C99, specifically "
          "the ones supported by g_date_time_format(). An update in this "
	  "property will affect the \"metadata-binary\" value.",
          DEFAULT_PROP_METADATA, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_BINARY,
      g_param_spec_boxed ("metadata-binary", "The binary metadata to push",
          "A binary blob to push as metadata. An update in this property "
	  "will clear up the \"metadata\" value",
          G_TYPE_BYTE_ARRAY, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_PERIOD,
      g_param_spec_uint ("period", "The metadata periodic sending",
          "Time interval in seconds between every metadata sending",
          DEFAULT_PROP_PERIOD, G_MAXUINT, DEFAULT_PROP_PERIOD,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

#ifdef EVAL
  gstelement_class->change_state =
    GST_DEBUG_FUNCPTR (gst_meta_src_change_state);
#endif

  klass->push_metadata = GST_DEBUG_FUNCPTR (gst_meta_src_push_metadata);
  /**
   * GstMetaSrc::push-metadata:
   * @metadata: the binary blob to push
   * @size: the size in bytes of the metadata to push
   *
   * Push a binary blob of metadata
   *
   * Since: 1.2
   */
  gst_meta_src_signals[SIGNAL_PUSH_METADATA] =
      g_signal_new ("push-metadata", G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION, G_STRUCT_OFFSET (GstMetaSrcClass,
          push_metadata), NULL, NULL, g_cclosure_marshal_generic,
          G_TYPE_INT, 2, G_TYPE_POINTER, G_TYPE_ULONG);

  gst_element_class_set_details_simple (gstelement_class,
      "Metadata Source",
      "Source/Metadata",
      "Send metadata buffers",
      "Fabricio Quiros <fabricio.quiros@ridgerun.com>");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_factory));
}

#ifdef EVAL
static gboolean
gst_meta_src_eval_disconnect (gpointer user_data)
{
  GstMetaSrc *metasrc = GST_META_SRC (user_data);

  g_printerr ("                                       \n"
      "***************************************\n"
      "**** THIS IS AN EVALUATION VERSION ****\n"
      "***************************************\n"
      "                                       \n"
      "  Thanks for evaluating GstMetaSrc!    \n"
      "                                       \n"
      "  The pipeline will lag for 5 seconds  \n"
      "  before starting. Similarly, after    \n"
      "  approximately 5 minutes the steam    \n"
      "  will stop. Please contact            \n"
      "  <support@ridgerun.com> to purchase   \n"
      "  the professional version of the      \n"
      "  plug-in.                             \n"
      "                                       \n"
      "***************************************\n"
      "**** THIS IS AN EVALUATION VERSION ****\n"
      "***************************************\n"
      "                                       \n");
  gst_element_send_event (GST_ELEMENT (metasrc), gst_event_new_eos ());

  return TRUE;
}

static GstStateChangeReturn
gst_meta_src_change_state (GstElement * element, GstStateChange transition)
{
  if (GST_STATE_CHANGE_READY_TO_PAUSED == transition) {
    g_printerr ("                                       \n"
        "***************************************\n"
        "**** THIS IS AN EVALUATION VERSION ****\n"
        "***************************************\n"
        "                                       \n"
        "  Thanks for evaluating GstMetaSrc!    \n"
        "                                       \n"
        "  The pipeline will lag for 5 seconds  \n"
        "  before starting. Similarly, after    \n"
        "  approximately 5 minutes the steam    \n"
        "  will stop. Please contact            \n"
        "  <support@ridgerun.com> to purchase   \n"
        "  the professional version of the      \n"
        "  plug-in.                             \n"
        "                                       \n"
        "***************************************\n"
        "**** THIS IS AN EVALUATION VERSION ****\n"
        "***************************************\n"
        "                                       \n");

    /* Initial evaluation lag. EVAL_START is in nanoseconds, we are sleeping in useconds */
    g_usleep (EVAL_START * GST_SECOND / 1000);

    g_timeout_add_seconds (EVAL_TIME, gst_meta_src_eval_disconnect, element);
  }

  return GST_ELEMENT_CLASS (gst_meta_src_parent_class)->change_state (element,
      transition);
}
#endif

static void
gst_meta_src_finalize (GObject * object)
{
  GstMetaSrc *metasrc = NULL;
  metasrc = GST_META_SRC (object);

  if (metasrc->src_id > 0) {
    g_source_remove (metasrc->src_id);
    metasrc->src_id = 0;
  }
  if (metasrc->metadata != NULL) {
    g_free (metasrc->metadata);
    metasrc->metadata = NULL;
  }
}

static GstPadProbeReturn
event_probe (GstPad * pad, GstPadProbeInfo * info, gpointer user_data)
{
  GstMetaSrc *metasrc = NULL;
  GstStreamFlags flags;
  GstEvent *event = NULL;
  GstEvent *new_event = NULL;

  g_return_val_if_fail (GST_IS_META_SRC (user_data), GST_PAD_PROBE_REMOVE);

  metasrc = GST_META_SRC (user_data);
  event = GST_PAD_PROBE_INFO_EVENT (info);

  new_event = gst_event_copy (event);

  GST_LOG_OBJECT (pad, "Received %s event: %" GST_PTR_FORMAT,
      GST_EVENT_TYPE_NAME (new_event), new_event);
  if (GST_PAD_PROBE_INFO_TYPE (info) & GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM) {
    if (GST_EVENT_TYPE (new_event) == GST_EVENT_STREAM_START) {
      /* Setting up GST_STREAM_FLAG_SPARSE */
      gst_event_parse_stream_flags (new_event, &flags);
      flags |= GST_STREAM_FLAG_SPARSE;

      gst_event_set_stream_flags (new_event, flags);
      gst_pad_remove_probe (pad, metasrc->probe_id);
      gst_pad_push_event (pad, new_event);
      return GST_PAD_PROBE_DROP;
    }
  }
  gst_event_unref (new_event);
  return GST_PAD_PROBE_OK;
}

static gulong
setup_probe (GstElement * src)
{
  gulong id;
  GstPad *pad = NULL;
  GstMetaSrc *metasrc = NULL;

  g_return_val_if_fail (GST_IS_META_SRC (src), 0);

  metasrc = GST_META_SRC (src);

  pad = gst_element_get_static_pad (src, "src");
  id = gst_pad_add_probe (pad,
      GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM, event_probe, metasrc, NULL);

  gst_object_unref (pad);
  return id;
}

static void
gst_meta_src_init (GstMetaSrc * metasrc)
{
  g_object_set (GST_APP_SRC (metasrc), "format", GST_FORMAT_TIME, NULL);
  gst_app_src_set_stream_type (GST_APP_SRC (metasrc),
      GST_APP_STREAM_TYPE_STREAM);

  metasrc->metadata = DEFAULT_PROP_METADATA;
  metasrc->binary = NULL;
  metasrc->probe_id = setup_probe (GST_ELEMENT (metasrc));
  metasrc->period = DEFAULT_PROP_PERIOD;
  metasrc->started = FALSE;

  metasrc->src_id = 0;
  metasrc->has_pattern = FALSE;
}

static void
gst_meta_src_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMetaSrc *src = NULL;
  src = GST_META_SRC (object);

  switch (prop_id) {
    case PROP_METADATA:
      gst_meta_src_save_metadata (src, g_value_dup_string (value));
      break;
    case PROP_BINARY:
      gst_meta_src_save_binary (src, g_value_dup_boxed (value));
      break;
    case PROP_PERIOD:
      gst_meta_src_save_period (src, g_value_get_uint (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_meta_src_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstMetaSrc *src = NULL;
  src = GST_META_SRC (object);

  switch (prop_id) {
    case PROP_METADATA:
      g_value_set_string (value, src->metadata);
      break;
    case PROP_BINARY:
      g_value_set_boxed (value, src->binary);
      break;
    case PROP_PERIOD:
      g_value_set_uint (value, src->period);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean
gst_meta_src_timeout (gpointer user_data)
{
  GstMetaSrc *src = NULL;

  g_return_val_if_fail (GST_IS_META_SRC (user_data), FALSE);

  src = GST_META_SRC (user_data);

  gst_meta_src_new_buffer (src);

  return TRUE;
}

static GstFlowReturn
gst_meta_src_new_buffer (GstMetaSrc * src)
{
  g_return_val_if_fail (src, GST_FLOW_ERROR);

  /* Nothing to push */
  if (!src->metadata && !src->binary) {
    return GST_FLOW_OK;
  }

  /* String metadata takes precedence over binary metadata */
  if (src->metadata) {
    return gst_meta_src_new_buffer_meta (src, src->metadata);
  } else {
    return gst_meta_src_new_buffer_binary (src, src->binary);
  }
}

static GstFlowReturn
gst_meta_src_new_buffer_meta (GstMetaSrc * src, gchar * metadata)
{
  GByteArray *barray;
  gchar *tosend = NULL;
  gsize size;

  g_return_val_if_fail (GST_IS_META_SRC (src), GST_FLOW_ERROR);
  g_return_val_if_fail (metadata, GST_FLOW_ERROR);

  if (src->started == FALSE) {
    return GST_FLOW_OK;
  }

  /* If this buffer needs formatting, grab the current time and do so */
  if (src->has_pattern) {
    tosend = gst_meta_src_time_format (metadata);
  } else {
    tosend = g_strdup (metadata);
  }

  /* We need the size of the formatted string */
  size = strlen (tosend) + LAST_CHARACTER;

  /* Convert to a bytearray and recycle binary method */
  barray = g_byte_array_new_take ((guint8 *)tosend, size);

  return gst_meta_src_new_buffer_binary(src, barray);
}

static GstFlowReturn
gst_meta_src_new_buffer_binary (GstMetaSrc * src,
    GByteArray *binary)
{
  GstBaseSrc *bsrc;
  GstAppSrc *appsrc;
  GstBuffer *buffer;
  GstAllocator *allocator;
  GstAllocationParams params;
  GstMapInfo map;
  GstClock *clock = NULL;
  GstClockTime base_time, now;

  g_return_val_if_fail (src, GST_FLOW_ERROR);
  g_return_val_if_fail (binary, GST_FLOW_ERROR);

  if (src->started == FALSE) {
    return GST_FLOW_OK;
  }

  bsrc = GST_BASE_SRC (src);
  appsrc = GST_APP_SRC (src);

  /* Ask for the allocator negotioated by our parent */
  gst_base_src_get_allocator (bsrc, &allocator, &params);

  buffer = gst_buffer_new_allocate (allocator, binary->len, &params);

  gst_buffer_map (buffer, &map, GST_MAP_WRITE);
  memcpy (map.data, binary->data, binary->len);
  gst_buffer_unmap (buffer, &map);

  /* Handle buffer timestamps */
  clock = GST_ELEMENT_CLOCK (src);
  if (clock) {
    /* The clock is valid, compute the running time and assign as ts */
    base_time = gst_element_get_base_time (GST_ELEMENT (src));
    now = gst_clock_get_time (clock);
    GST_BUFFER_PTS (buffer) = GST_CLOCK_DIFF (base_time, now);
    GST_BUFFER_DTS (buffer) = GST_CLOCK_TIME_NONE;
  } else {
    /* No clock, likely the pipeline is starting */
    GST_BUFFER_PTS (buffer) = 0;
    GST_BUFFER_DTS (buffer) = 0;
  }

  /* Finally, push the buffer downstream */
  if (GST_FLOW_OK != gst_app_src_push_buffer (appsrc, buffer)) {
    GST_ERROR_OBJECT (src, "Unable to push metadata buffer downstream");
    gst_buffer_unref (buffer);
    return GST_FLOW_ERROR;
  }
  GST_INFO_OBJECT (src, "Successfully pushed metadata buffer of length %u",
      binary->len);

  return GST_FLOW_OK;
}


static gboolean
gst_meta_src_start (GstBaseSrc * base)
{
  GstAppSrc *appsrc = NULL;
  GstCaps *caps = NULL;
  GstMetaSrc *metasrc = NULL;

  g_return_val_if_fail (GST_IS_META_SRC (base), FALSE);

  appsrc = GST_APP_SRC (base);
  metasrc = GST_META_SRC (base);

  if (metasrc->started == TRUE)
    return TRUE;

  GST_INFO_OBJECT (metasrc, "Metasrc successfully started");
  caps = gst_caps_from_string ("meta/x-klv,parsed=true");

  gst_app_src_set_caps (appsrc, caps);
  metasrc->started = TRUE;
  gst_caps_unref (caps);

  /* Send metadata if we have one set but no period is configured */
  if (!metasrc->src_id) {
    if (metasrc->binary || metasrc->metadata) {
      gst_meta_src_new_buffer (metasrc);
    }
  }

  return GST_BASE_SRC_CLASS (gst_meta_src_parent_class)->start (base);
}

static gboolean
gst_meta_src_stop (GstBaseSrc * base)
{
  GstMetaSrc *metasrc = NULL;

  metasrc = GST_META_SRC (base);
  metasrc->started = FALSE;

  return GST_BASE_SRC_CLASS (gst_meta_src_parent_class)->stop (base);
}

static gchar *
gst_meta_src_time_format (const gchar * format)
{
  gchar *output = NULL;
  GDateTime *date = NULL;

  g_return_val_if_fail (format, NULL);

  date = g_date_time_new_now_local ();
  output = g_date_time_format (date, format);

  g_date_time_unref (date);

  return output;
}

static GstFlowReturn
gst_meta_src_save_metadata (GstMetaSrc * src, gchar * metadata)
{
  g_return_val_if_fail (GST_IS_META_SRC (src), GST_FLOW_ERROR);

  GST_DEBUG_OBJECT (src, "Received metadata: %s", metadata);

  /* Wipe off everything */
  gst_meta_src_disable_metadata (src);

  /* Check for NULL or empty metadata */
  if (NULL == metadata || 0 == strlen (metadata)) {
    return GST_FLOW_OK;
  }

  /* Does this metadata requires time formatting? */
  if (g_strrstr (metadata, "%")) {
    GST_DEBUG_OBJECT (src, "The string \"%s\" will be time formatted",
        metadata);
    src->has_pattern = TRUE;
  } else {
    src->has_pattern = FALSE;
  }

  /* Finally save this metadata */
  src->metadata = metadata;

  /* Only send the metadata if there is no period configured */
  if (src->src_id == 0) {
    return gst_meta_src_new_buffer_meta (src, src->metadata);
  } else {
    return GST_FLOW_OK;
  }
}

static GstFlowReturn
gst_meta_src_save_binary (GstMetaSrc * src, GByteArray * binary)
{
  g_return_val_if_fail (GST_IS_META_SRC (src), GST_FLOW_ERROR);

  GST_DEBUG_OBJECT (src, "Received metadata: %p of length %u", binary,
      binary->len);

  /* Wipe off everything */
  gst_meta_src_disable_metadata (src);

  /* Check for NULL or empty metadata */
  if (NULL == binary) {
    return GST_FLOW_OK;
  }

  /* Finally save this metadata */
  src->binary = binary;

  /* Only send the metadata if there is no period configured */
  if (src->src_id == 0) {
    return gst_meta_src_new_buffer_binary (src, src->binary);
  } else {
    return GST_FLOW_OK;
  }
}

static void
gst_meta_src_disable_metadata (GstMetaSrc * src)
{
  g_return_if_fail (src);

  GST_INFO_OBJECT (src, "Disabling metadata");
  if (src->binary) {
    g_boxed_free (G_TYPE_BYTE_ARRAY, src->binary);
  }
  src->binary = NULL;

  if (src->metadata) {
    g_free (src->metadata);
  }
  src->metadata = NULL;
}

static void
gst_meta_src_save_period (GstMetaSrc * src, guint period)
{
  src->period = period;

  if (src->src_id > 0) {
    g_source_remove (src->src_id);
    src->src_id = 0;
  }

  if (src->period) {
    GST_DEBUG_OBJECT (src,
        "Received metadata will be sent every %d second(s)", src->period);
    src->src_id =
        g_timeout_add_seconds (src->period, gst_meta_src_timeout, src);
  }
}

static GstFlowReturn
gst_meta_src_push_metadata (GstMetaSrc *metasrc, guint8 *metadata, gsize size)
{
  GByteArray *barray;

  g_return_val_if_fail (metasrc, GST_FLOW_ERROR);
  g_return_val_if_fail (metadata, GST_FLOW_ERROR);
  g_return_val_if_fail (size > 0, GST_FLOW_ERROR);

  barray = g_byte_array_sized_new (size+1);
  barray->len = size;
  memcpy (barray->data, metadata, size);

  return gst_meta_src_save_binary(metasrc, barray);
}
