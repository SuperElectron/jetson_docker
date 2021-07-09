/* GStreamer
 * Copyright (C) 2016 RidgeRun Engineering  Michael Gruner <michael.gruner@ridgerun.com>
 *                                          Fabricio Quiros <fabricio.quiros@ridgerun.com
 *
 * gstmetasink.c:
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
 * SECTION:element-metasink
 * @see_also: #GstMetaSink
 *
 * Receive different kinds of metadata buffers
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -e filesrc location=test.txt ! 'meta/x-klv' ! metasink
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include "gstmetasink.h"

GST_DEBUG_CATEGORY_STATIC (gst_meta_sink_debug);
#define GST_CAT_DEFAULT gst_meta_sink_debug

#define DEFAULT_PROP_DUMP TRUE
#define DEFAULT_PROP_SIGNAL_NEW_METADATA FALSE

#ifdef EVAL
#  define EVAL_TIME 5*60*GST_SECOND
#endif

enum
{
  SIGNAL_NEW_METADATA,
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SIGNAL_NEW_METADATA,
  PROP_DUMP
};

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("meta/x-klv,parsed=true")
    );

#define gst_meta_sink_parent_class parent_class
G_DEFINE_TYPE (GstMetaSink, gst_meta_sink, GST_TYPE_APP_SINK);

static void gst_meta_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);

static void gst_meta_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static GstFlowReturn gst_meta_sink_render (GstBaseSink * bsink,
    GstBuffer * buf);

static guint gst_meta_sink_signals[LAST_SIGNAL] = { 0 };

static void
gst_meta_sink_class_init (GstMetaSinkClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSinkClass *gstbase_sink_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gstelement_class = GST_ELEMENT_CLASS (klass);
  gstbase_sink_class = GST_BASE_SINK_CLASS (klass);

  GST_DEBUG_CATEGORY_INIT (gst_meta_sink_debug, "metasink", 0,
      "metasink element");

  gobject_class->set_property = gst_meta_sink_set_property;
  gobject_class->get_property = gst_meta_sink_get_property;

  g_object_class_install_property (gobject_class, PROP_SIGNAL_NEW_METADATA,
      g_param_spec_boolean ("signal-new-metadata", "Signal new metadata",
          "Send a signal on new metadata", DEFAULT_PROP_SIGNAL_NEW_METADATA,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_DUMP,
      g_param_spec_boolean ("dump", "Dump",
          "Dump metadata to the standard output", DEFAULT_PROP_DUMP,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  gst_meta_sink_signals[SIGNAL_NEW_METADATA] =
      g_signal_new ("new-metadata", G_TYPE_FROM_CLASS (klass),
      G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET (GstMetaSinkClass, new_metadata), NULL,
      NULL, g_cclosure_marshal_VOID__UINT_POINTER, G_TYPE_NONE, 2, G_TYPE_UINT,
      G_TYPE_POINTER);

  gst_element_class_set_details_simple (gstelement_class,
      "Metadata Sink",
      "Sink/Metadata",
      "Receive metadata buffers",
      "Fabricio Quiros <fabricio.quiros@ridgerun.com");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));

  gstbase_sink_class->render = GST_DEBUG_FUNCPTR (gst_meta_sink_render);
}

static void
gst_meta_sink_init (GstMetaSink * metasink)
{
  metasink->dump = DEFAULT_PROP_DUMP;
  metasink->signal_new_metadata = DEFAULT_PROP_SIGNAL_NEW_METADATA;

#ifdef EVAL
  g_printerr ("                                         \n"
	      "*****************************************\n"
	      "***** THIS IS AN EVALUATION VERSION *****\n"
	      "*****************************************\n"
	      "                                         \n"
	      "  Thanks for evaluating GstMetaSink!     \n"
	      "                                         \n"
	      "  The pipeline will lag for 5 seconds    \n"
	      "  before starting. Similarly, after      \n"
	      "  approximately 5 minutes the stream     \n"
	      "  will stop. Please contact              \n"
	      "  <support@ridgerun.com> to purchase     \n"
	      "  the professional version of the        \n"
	      "  plug-in.                               \n"
	      "                                         \n"
	      "*****************************************\n"
	      "***** THIS IS AN EVALUATION VERSION *****\n"
	      "*****************************************\n"
	      "                                         \n");

  g_usleep (5*1000000);

  metasink->start_time = gst_util_get_timestamp ();
#endif
}

static void
gst_meta_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMetaSink *sink = NULL;
  sink = GST_META_SINK (object);

  switch (prop_id) {
    case PROP_DUMP:
      sink->dump = g_value_get_boolean (value);
      break;
    case PROP_SIGNAL_NEW_METADATA:
      sink->signal_new_metadata = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_meta_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstMetaSink *sink = NULL;
  sink = GST_META_SINK (object);

  switch (prop_id) {
    case PROP_DUMP:
      g_value_set_boolean (value, sink->dump);
      break;
    case PROP_SIGNAL_NEW_METADATA:
      g_value_set_boolean (value, sink->signal_new_metadata);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static GstFlowReturn
gst_meta_sink_render (GstBaseSink * bsink, GstBuffer * buf)
{
  GstMetaSink *sink = NULL;
  gboolean ret = GST_FLOW_OK;

  g_return_val_if_fail (GST_IS_META_SINK (bsink), GST_FLOW_ERROR);
  g_return_val_if_fail (buf, GST_FLOW_ERROR);

  sink = GST_META_SINK_CAST (bsink);

  if (sink->dump || sink->signal_new_metadata) {
    GstMapInfo info;

    if (gst_buffer_map (buf, &info, GST_MAP_READ)) {

      if (sink->dump)
        gst_util_dump_mem (info.data, info.size);
      if (sink->signal_new_metadata)
        g_signal_emit (sink, gst_meta_sink_signals[SIGNAL_NEW_METADATA], 0,
            info.size, info.data);

      gst_buffer_unmap (buf, &info);
    }
  }

  GST_INFO_OBJECT (sink, "Successfully showed metadata buffer");

#ifdef EVAL
  GstClockTime curr_time = gst_util_get_timestamp ();

  if (curr_time - sink->start_time > EVAL_TIME) {
    g_printerr ("                                         \n"
		"*****************************************\n"
		"***** THIS IS AN EVALUATION VERSION *****\n"
		"*****************************************\n"
		"                                         \n"
		"  Thanks for evaluating GstMetaSink!     \n"
		"                                         \n"
		"  The pipeline will lag for 5 seconds    \n"
		"  before starting. Similarly, after      \n"
		"  approximately 5 minutes the stream     \n"
		"  will stop. Please contact              \n"
		"  <support@ridgerun.com> to purchase     \n"
		"  the professional version of the        \n"
		"  plug-in.                               \n"
		"                                         \n"
		"*****************************************\n"
		"***** THIS IS AN EVALUATION VERSION *****\n"
		"*****************************************\n"
		"                                         \n");
    ret = GST_FLOW_EOS;
  }
#endif

  return ret;
}
