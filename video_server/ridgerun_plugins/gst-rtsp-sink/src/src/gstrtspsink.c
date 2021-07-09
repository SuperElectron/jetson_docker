/* 
 * Copyright (C) 2016-2017 RidgeRun, LLC (http://www.ridgerun.com)
 * All Rights Reserved.
 *
 * The contents of this software are proprietary and confidential to RidgeRun,
 * LLC.  No part of this program may be photocopied, reproduced or translated
 * into another programming language without prior written consent of 
 * RidgeRun, LLC.  The user is free to modify the source code after obtaining
 * a software license from RidgeRun.  All source code changes must be provided
 * back to RidgeRun without any encumbrance.
 */

/**
 * SECTION:element-gstrtspsink
 *
 * This element provides a RidgeRun implementation of a rtsp sink.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch v4l2src ! video/x-raw, format=NV12, width=640, height=480\
 *  ! x264enc ! video/x-h264, mapping=/test ! rtspsink
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include "gstrtspsink.h"
#include <gst/rtsp-server/rtsp-server.h>




typedef gboolean (*GstRtspSinkIteratorFunc) (GstRtspSink *, GstRtspSinkPad *,
    gpointer);

typedef struct _GstRtspSinkPayloader GstRtspSinkPayloader;
struct _GstRtspSinkPayloader
{
  const gchar *mimetype;
  const gchar *payloader;
};

GST_DEBUG_CATEGORY_STATIC (gst_rtsp_sink_debug);
#define GST_CAT_DEFAULT gst_rtsp_sink_debug

enum
{
  PROP_0,
  PROP_SERVICE,
  PROP_IP_MIN,
  PROP_IP_MAX,
  PROP_PORT_MIN,
  PROP_PORT_MAX,
  PROP_MULTICAST,
  PROP_TTL,
  PROP_AUTH,
  PROP_TIMEOUT,
};

/* The name of the mapping field on the caps*/
#define MAPPING "mapping"
#define DEFAULT_SERVICE "554"
#define DEFAULT_MAPPING "/test"
#define DEFAULT_IP_MIN "224.2.0.1"
#define DEFAULT_IP_MAX "224.2.0.10"
#define DEFAULT_PORT_MIN 5000
#define DEFAULT_PORT_MAX 5010
#define DEFAULT_MULTICAST FALSE
#define DEFAULT_TTL 16
#define DEFAULT_AUTH NULL
#define DEFAULT_TIMEOUT 60
#define DEFAULT_TIMEOUT_MIN 1
#define DEFAULT_TIMEOUT_MAX G_MAXUINT

#define GST_RTSP_SINK_VIDEO_CAPS_MAKE(mimetype, ...)				\
  mimetype ", "								\
  "width = " GST_VIDEO_SIZE_RANGE ", "					\
  "height = " GST_VIDEO_SIZE_RANGE ", "					\
  "framerate = " GST_VIDEO_FPS_RANGE					\
  __VA_ARGS__ "; "

#define GST_RTSP_SINK_EMPTY_CAPS_MAKE(mimetype, ...)				\
  mimetype 								\
  __VA_ARGS__ "; "


/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink_%d",
    GST_PAD_SINK,
    GST_PAD_REQUEST,
    GST_STATIC_CAPS (GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/x-h264")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/x-h265")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/x-vp8")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/x-vp9")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/mpeg",
            ", mpegversion=4")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/x-jpeg")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("video/mpegts")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("video/x-divx")
        GST_RTSP_SINK_VIDEO_CAPS_MAKE ("image/jpeg")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("audio/mpeg",
            ", mpegversion=4")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("audio/x-ac3")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("audio/ac3")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("audio/x-mulaw" ", rate=8000",
            ", channels=1")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("audio/x-alaw" ", rate=8000",
            ", channels=1")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("audio/x-opus" ", channels=[ 1, 2 ]",
            ", channel-mapping-family=0")
        GST_RTSP_SINK_EMPTY_CAPS_MAKE ("meta/x-klv", ", parsed=true")
    ));

static GstRtspSinkPayloader payloaders[] = {
  {"video/x-h264", "rtph264pay"},
  {"video/x-h265", "rtph265pay"},
  {"video/x-vp8", "rtpvp8pay"},
  {"video/x-vp9", "rtpvp9pay"},
  {"video/mpeg", "rtpmp4vpay"},
  {"video/x-divx", "rtpmp4vpay"},
  {"image/jpeg", "rtpjpegpay"},
  {"video/x-jpeg", "rtpjpegpay"},
  {"audio/mpeg", "rtpmp4apay"},
  {"audio/x-ac3", "rtpac3pay"},
  {"audio/ac3", "rtpac3pay"},
  {"video/mpegts", "rtpmp2tpay"},
  {"audio/x-mulaw", "rtppcmupay"},
  {"audio/x-alaw", "rtppcmapay"},
  {"audio/x-opus", "rtpopuspay"},
  {"meta/x-klv", "rtpklvpay"},
  {NULL}
};

static void gst_rtsp_sink_child_proxy_init (gpointer g_iface,
    gpointer iface_data);

#define gst_rtsp_sink_parent_class parent_class
G_DEFINE_TYPE_WITH_CODE (GstRtspSink, gst_rtsp_sink, GST_TYPE_BIN,
    G_IMPLEMENT_INTERFACE (GST_TYPE_CHILD_PROXY,
        gst_rtsp_sink_child_proxy_init));

/* Object declarations */
static void gst_rtsp_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_rtsp_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);
static void gst_rtsp_sink_finalize (GObject * object);
static GstFlowReturn gst_rtsp_sink_push_buffer (GstPad * pad, GstBuffer * buf);
static gboolean gst_rtsp_sink_event_function (GstPad * pad,
    GstObject * parent, GstEvent * event);
static gboolean gst_rtsp_sink_set_caps (GstPad * pad, GstCaps * caps);
static GstPad *gst_rtsp_sink_request_new_pad (GstElement * element,
    GstPadTemplate * templ, const gchar * name, const GstCaps * caps);
static void gst_rtsp_sink_release_pad (GstElement * element, GstPad * pad);

static GstStateChangeReturn gst_rtsp_sink_change_state (GstElement * element,
    GstStateChange transition);
static gboolean gst_rtsp_sink_start (GstRtspSink * sink);
static void gst_rtsp_sink_stop (GstRtspSink * sink);
static const gchar *rtsp_sink_get_payloader_from_mime (const gchar * mime);
static gchar *rtsp_sink_get_pipeline (const gchar * mimetype, gchar * padname,
    guint index);
static gboolean rtsp_sink_configure_mapping (GstRtspSink * sink, GstPad * pad,
    gchar * mapping, const gchar * mimetype);
static gboolean rtsp_sink_attach_if_needed (GstRtspSink * sink);
static gboolean rtsp_sink_get_factory_by_mapping (GstRtspSink * sink,
    GstRtspSinkPad * pad, gpointer data);
static gboolean rtsp_sink_remove_mappings (GstRtspSink * sink,
    GstRtspSinkPad * pad, gpointer data);
static void rtsp_sink_iterate_pads (GstRtspSink * sink,
    GstRtspSinkIteratorFunc func, gpointer data);
static GstRTSPFilterResult filter_session_poll_callback (GstRTSPSessionPool *
    pool, GstRTSPSession * session, gpointer user_data);
static gboolean rtsp_sink_needs_attach (GstRtspSink * sink,
    GstRtspSinkPad * pad, gpointer data);
static gboolean rtsp_sink_timeout (GstRtspSink * sink);
static void rtsp_client_connected_callback (GstRTSPServer * server,
    GstRTSPClient * client, gpointer data);
static void configure_timeout (GstRTSPClient * client, GstRTSPSession * session,
    gpointer user_data);
static gboolean rtsp_sink_reset_caps (GstRtspSink * sink,
    GstRtspSinkPad * pad, gpointer data);

#ifdef EVAL
static void print_eval ();
#endif

static gboolean
rtsp_sink_get_sink_by_name (GstRtspSink * sink, GstRtspSinkPad * pad,
    gpointer data)
{
  gpointer *namedata = (gpointer *) data;
  GstAppSink **appsink = (GstAppSink **) namedata[1];
  const gchar *name = (gchar *) namedata[0];
  gchar *sinkname;
  g_object_get (pad->appsink, "name", &sinkname, NULL);
  if (!strcmp (sinkname, name)) {
    *appsink = gst_object_ref (pad->appsink);
    return FALSE;
  }
  g_free (sinkname);
  return TRUE;
}

static GObject *
gst_rtsp_sink_child_proxy_get_child_by_name (GstChildProxy * child_proxy,
    const gchar * name)
{
  GstAppSink *appsink = NULL;
  GstRtspSink *sink = GST_RTSP_SINK (child_proxy);
  gpointer namedata[2] = { (gpointer) name, &appsink };
  GST_INFO_OBJECT (sink, "using child proxy by name: %s", name);
  rtsp_sink_iterate_pads (sink, rtsp_sink_get_sink_by_name, namedata);
  return G_OBJECT (appsink);
}

static guint
gst_rtsp_sink_child_proxy_get_children_count (GstChildProxy * child_proxy)
{
  GstRtspSink *sink = GST_RTSP_SINK (child_proxy);
  GST_INFO_OBJECT (sink, "children count %u", sink->padcount);
  return sink->padcount;
}



static void
gst_rtsp_sink_child_proxy_init (gpointer g_iface, gpointer iface_data)
{
  GstChildProxyInterface *iface = g_iface;
  GST_INFO ("intializing child proxy interface");
  iface->get_child_by_name = gst_rtsp_sink_child_proxy_get_child_by_name;
  iface->get_children_count = gst_rtsp_sink_child_proxy_get_children_count;
}

/* GObject vmethod implementations */

static void
gst_rtsp_sink_class_init (GstRtspSinkClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_rtsp_sink_set_property;
  gobject_class->get_property = gst_rtsp_sink_get_property;
  gobject_class->finalize = gst_rtsp_sink_finalize;

  gstelement_class->request_new_pad =
      GST_DEBUG_FUNCPTR (gst_rtsp_sink_request_new_pad);
  gstelement_class->release_pad = GST_DEBUG_FUNCPTR (gst_rtsp_sink_release_pad);
  gstelement_class->change_state =
      GST_DEBUG_FUNCPTR (gst_rtsp_sink_change_state);

  GST_DEBUG_CATEGORY_INIT (gst_rtsp_sink_debug, "rtspsink",
      0, "RR Rtsp sink plugin");

  g_object_class_install_property (gobject_class, PROP_SERVICE,
      g_param_spec_string ("service", "Port",
          "Service or port of the connection",
          DEFAULT_SERVICE, G_PARAM_READWRITE));

  gst_element_class_set_static_metadata (gstelement_class,
      "RR RTSP Sink Element",
      "Sink/Network",
      "RidgeRun's RTSP server based sink element",
      "Michael Gruner <michael.gruner@ridgerun.com>\n\t\t\t   Adrian Cervantes <adrian.cervantes@ridgerun.com> ");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));

  g_object_class_install_property (gobject_class, PROP_IP_MIN,
      g_param_spec_string ("multicast-ip-min",
          "Minimum address",
          "The multicast minimum address to send media to",
          DEFAULT_IP_MIN, G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_IP_MAX,
      g_param_spec_string ("multicast-ip-max",
          "Maximum address",
          "The multicast maximum address to send media to",
          DEFAULT_IP_MAX, G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_PORT_MIN,
      g_param_spec_uint ("multicast-port-min",
          "Minimum port",
          "The multicast minimum port to send media to",
          0, G_MAXINT,
          DEFAULT_PORT_MIN, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_PORT_MAX,
      g_param_spec_uint ("multicast-port-max",
          "Maximum port",
          "The multicast maximum port to send media to",
          0, G_MAXINT,
          DEFAULT_PORT_MAX, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_MULTICAST,
      g_param_spec_boolean ("multicast",
          "Multicast Support",
          "Enables the multicast support",
          DEFAULT_MULTICAST, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_TTL,
      g_param_spec_uint ("multicast-ttl",
          "Multicast time-to-live",
          "Time-to-live", 0, 255,
          DEFAULT_TTL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class, PROP_AUTH,
      g_param_spec_string ("auth",
          "Authorization",
          "Authentication and authorization \n \t\t\tformat: user1:password1,user2:password2,....userN:passwordN",
          DEFAULT_AUTH, G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_TIMEOUT,
      g_param_spec_uint ("timeout",
          "Timeout",
          "Amount of seconds without a client heartbeat before considering it expired",
          DEFAULT_TIMEOUT_MIN, DEFAULT_TIMEOUT_MAX,
          DEFAULT_TIMEOUT, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

}

/* initialize the new element
 */
static void
gst_rtsp_sink_init (GstRtspSink * sink)
{
  sink->attached = FALSE;
  sink->source = 0;
  sink->service = g_strdup (DEFAULT_SERVICE);
  sink->ip_min = g_strdup (DEFAULT_IP_MIN);
  sink->ip_max = g_strdup (DEFAULT_IP_MAX);
  sink->port_min = DEFAULT_PORT_MIN;
  sink->port_max = DEFAULT_PORT_MAX;
  sink->multicast = DEFAULT_MULTICAST;
  sink->ttl = DEFAULT_TTL;
  sink->padcount = 0;
  sink->auth = g_strdup (DEFAULT_AUTH);
  sink->pool = gst_rtsp_address_pool_new ();
  sink->timeout_source = 0;
  sink->timeout = DEFAULT_TIMEOUT;
  g_mutex_init (&sink->padlock);
}

static GstFlowReturn
gst_rtsp_sink_new_sample (GstAppSink * appsink, gpointer data)
{
  GstPad *pad = GST_PAD (data);
  GstRtspSink *sink = GST_RTSP_SINK (GST_OBJECT_PARENT (appsink));
  GstSample *sample;
  GstBuffer *buf;

  GST_LOG_OBJECT (sink, "New buffer at %s:%s", GST_DEBUG_PAD_NAME (pad));

  sample = gst_app_sink_pull_sample (appsink);
  buf = gst_sample_get_buffer (sample);
  gst_buffer_ref (buf);

  gst_sample_unref (sample);

  return gst_rtsp_sink_push_buffer (pad, buf);
}

static void
gst_rtsp_sink_attach_appsink (GstRtspSink * sink, GstGhostPad * pad)
{
  GstAppSink *appsink;
  GstAppSinkCallbacks callbacks;
  GstPad *target;
  GstRtspSinkPad *spad;

  g_return_if_fail (NULL != sink);
  g_return_if_fail (NULL != pad);

  spad = GST_RTSP_SINK_PAD (pad);

  appsink =
      GST_APP_SINK (gst_element_factory_make ("appsink",
          GST_OBJECT_NAME (pad)));
  g_object_set (appsink, "max-buffers", 1, "sync", TRUE, "drop", TRUE, NULL);

  GST_INFO_OBJECT (sink, "Creating associated %s for %s:%s",
      GST_OBJECT_NAME (appsink), GST_DEBUG_PAD_NAME (pad));

  spad->appsink = appsink;

  callbacks.eos = NULL;
  callbacks.new_preroll = NULL;
  callbacks.new_sample = gst_rtsp_sink_new_sample;

  gst_app_sink_set_callbacks (appsink, &callbacks, (gpointer) pad, NULL);
  gst_bin_add (GST_BIN (sink), GST_ELEMENT (appsink));

  target = gst_element_get_static_pad (GST_ELEMENT (appsink), "sink");
  gst_ghost_pad_set_target (pad, target);
  gst_object_unref (target);
}

static GstPad *
gst_rtsp_sink_request_new_pad (GstElement * element,
    GstPadTemplate * templ, const gchar * name, const GstCaps * caps)
{
  GstRtspSink *sink = GST_RTSP_SINK (element);
  GstPad *pad;

  if (sink->attached)
    goto attached;

  pad = GST_PAD (gst_rtsp_sink_pad_new_from_template (templ, name));
  if (!pad)
    goto nopad;

  GST_INFO_OBJECT (sink, "Adding pad %s", GST_OBJECT_NAME (pad));
  gst_element_add_pad (element, pad);

  sink->padcount++;
  gst_pad_set_event_function (pad, gst_rtsp_sink_event_function);
  gst_rtsp_sink_attach_appsink (sink, GST_GHOST_PAD (pad));

  return pad;
attached:
  {
    GST_ERROR_OBJECT (sink, "Already attached, can't create new pads");
    return NULL;
  }
nopad:
  {
    GST_ERROR_OBJECT (sink, "Unable to create pad %s", name);
    return NULL;
  }
}

static void
gst_rtsp_sink_release_pad (GstElement * element, GstPad * pad)
{
  GstRtspSink *sink = GST_RTSP_SINK (element);
  sink->padcount--;
  GST_INFO_OBJECT (sink, "Releasing pad %s", GST_OBJECT_NAME (pad));
}

static void
gst_rtsp_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstRtspSink *sink = GST_RTSP_SINK (object);

  switch (prop_id) {
    case PROP_SERVICE:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set service because server is already attached, leaving old %s",
            sink->service);
        break;
      }

      g_free (sink->service);
      sink->service = g_value_dup_string (value);
      GST_INFO_OBJECT (sink, "Setting rtsp service to %s", sink->service);

      break;

    case PROP_IP_MIN:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set multicast ip-min because server is already attached, leaving old %s",
            sink->ip_min);
        break;
      }

      g_free (sink->ip_min);
      sink->ip_min = g_value_dup_string (value);
      GST_INFO_OBJECT (sink, "Setting multicast rtsp minimun address to %s",
          sink->ip_min);
      break;

    case PROP_IP_MAX:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set multicast ip-max because server is already attached, leaving old %s",
            sink->ip_max);
        break;
      }

      g_free (sink->ip_max);
      sink->ip_max = g_value_dup_string (value);
      GST_INFO_OBJECT (sink, "Setting rtsp multicast maximun address to %s",
          sink->ip_max);
      break;

    case PROP_PORT_MIN:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set multicast port-min because port is already attached, leaving old %d",
            sink->port_min);
        break;
      }

      sink->port_min = g_value_get_uint (value);
      GST_INFO_OBJECT (sink, "Setting rtsp multicast minimun port to %d",
          sink->port_min);
      break;

    case PROP_PORT_MAX:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set multicast port-max because port is already attached, leaving old %d",
            sink->port_max);
        break;
      }

      sink->port_max = g_value_get_uint (value);
      GST_INFO_OBJECT (sink, "Setting rtsp multicast maximun port to %d",
          sink->port_max);
      break;

    case PROP_MULTICAST:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set multicast because server is already attached, leaving old %s",
            sink->multicast ? "true" : "false");
        break;
      }

      sink->multicast = g_value_get_boolean (value);
      GST_INFO_OBJECT (sink, "Setting rtsp multicast support to %s",
          sink->multicast ? "true" : "false");
      break;

    case PROP_TTL:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set multicast ttl because is already attached, leaving old %d",
            sink->ttl);
        break;
      }

      sink->ttl = g_value_get_uint (value);
      GST_INFO_OBJECT (sink, "Setting rtsp multicast ttl to %d", sink->ttl);
      break;

    case PROP_AUTH:
      if (sink->attached) {
        GST_WARNING_OBJECT
            (sink,
            "Can't set auth because server is already attached, leaving old %s",
            sink->auth);
        break;
      }

      g_free (sink->auth);
      sink->auth = g_value_dup_string (value);
      GST_INFO_OBJECT (sink, "Setting rtsp authorization to %s", sink->auth);
      break;

    case PROP_TIMEOUT:
      GST_OBJECT_LOCK (sink);
      sink->timeout = g_value_get_uint (value);
      GST_OBJECT_UNLOCK (sink);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_rtsp_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstRtspSink *sink = GST_RTSP_SINK (object);
  switch (prop_id) {
    case PROP_SERVICE:
      g_value_set_string (value, sink->service);
      break;
    case PROP_IP_MIN:
      g_value_set_string (value, sink->ip_min);
      break;
    case PROP_IP_MAX:
      g_value_set_string (value, sink->ip_max);
      break;
    case PROP_PORT_MIN:
      g_value_set_uint (value, sink->port_min);
      break;
    case PROP_PORT_MAX:
      g_value_set_uint (value, sink->port_max);
      break;
    case PROP_MULTICAST:
      g_value_set_boolean (value, sink->multicast);
      break;
    case PROP_TTL:
      g_value_set_uint (value, sink->ttl);
      break;
    case PROP_AUTH:
      g_value_set_string (value, sink->auth);
      break;
    case PROP_TIMEOUT:
      g_value_set_uint (value, sink->timeout);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean
gst_rtsp_sink_set_caps (GstPad * pad, GstCaps * caps)
{
  GstRtspSink *sink = GST_RTSP_SINK (GST_PAD_PARENT (pad));
  GstPad *appsink_pad;
  GstAppSink *appsink;
  gchar *name = GST_OBJECT_NAME (pad);
  GstStructure *st = NULL;
  gchar *mapping = NULL;
  gchar *tmpmapping = NULL;
  gboolean mapret = FALSE;
  const gchar *mimetype = NULL;

  /* This doesn't need to be an error, TS may need to 
     negotiate multiple times, we can configure ourselves 
     the first time
   */
  if (sink->attached)
    goto attached;

  if (!name)
    goto noname;

  GST_DEBUG_OBJECT (sink, "Setting caps for pad %s", name);

  st = gst_caps_get_structure (caps, 0);
  if (!st)
    goto parsefail;

  mapping = g_strdup (gst_structure_get_string (st, "mapping"));
  if (!mapping) {
    GST_WARNING_OBJECT (sink,
        "The sink caps of %s must have a mapping. Consider adding "
        MAPPING
        "=/<unique name> to these caps. Using default "
        DEFAULT_MAPPING " mapping", name);

    mapping = g_strdup (DEFAULT_MAPPING);
  }
  /* Safe check to make sure mapping starts with a backslash */
  if ('/' != mapping[0]) {
    tmpmapping = mapping;
    mapping = g_malloc (strlen (tmpmapping) + 2);
    sprintf (mapping, "/%s", tmpmapping);
    g_free (tmpmapping);
  }

  mimetype = gst_structure_get_name (st);

  g_mutex_lock (&sink->padlock);
  mapret = rtsp_sink_configure_mapping (sink, pad, mapping, mimetype);
  g_mutex_unlock (&sink->padlock);
  if (!mapret)
    goto configfail;

  g_free (mapping);

  if (!rtsp_sink_attach_if_needed (sink))
    goto attachfail;

  appsink_pad = gst_ghost_pad_get_target (GST_GHOST_PAD (pad));
  appsink = GST_APP_SINK (gst_pad_get_parent (appsink_pad));
  gst_app_sink_set_caps (appsink, caps);

  gst_object_unref (appsink_pad);
  gst_object_unref (appsink);

  return TRUE;

attached:
  {
    GST_WARNING_OBJECT (sink,
        "Server already attached, can't change pad capabilities");
    return FALSE;
  }
noname:
  {
    GST_ERROR_OBJECT (sink, "Unable to get pad name, can't assign factory");
    return FALSE;
  }
parsefail:
  {
    GST_ERROR_OBJECT (sink, "Unable to parse caps, can't get mapping");
    return FALSE;
  }
configfail:
  {
    GST_ERROR_OBJECT (sink,
        "A problem occured configuring mapping for pad %s", name);
    g_free (mapping);
    return FALSE;
  }
attachfail:
  {
    GST_ERROR_OBJECT (sink, "Failed to attach to server");
    return FALSE;
  }
}

static GstFlowReturn
gst_rtsp_sink_push_buffer (GstPad * pad, GstBuffer * buf)
{
  GstRtspSink *sink = GST_RTSP_SINK (GST_PAD_PARENT (pad));
  GstRtspSinkPad *rtsppad = GST_RTSP_SINK_PAD (pad);
  RrRtspMediaFactory *factory;
  GstBuffer *compensation_buffer;
  GstFlowReturn ret = GST_FLOW_OK;

  if (!sink->attached)
    goto noattached;

  factory = rtsppad->factory;
  if (!factory || !factory->is_prepared)
    /* We are not ready to stream yet */
    goto noprepared;

  if (!rtsppad->appsrc) {
    GstCaps *caps;

    rtsppad->appsrc =
        GST_APP_SRC (gst_bin_get_by_name (GST_BIN (factory->pipeline),
            GST_OBJECT_NAME (rtsppad)));
    if (!rtsppad->appsrc)
      goto noappsrc;

    caps = gst_pad_get_current_caps (pad);
    if (!caps) {
      goto nocaps;
    }
    gst_app_src_set_caps (rtsppad->appsrc, caps);
    gst_caps_unref (caps);

#ifdef EVAL
    rtsppad->start_time = GST_BUFFER_PTS (buf);
    GST_LOG_OBJECT (sink, "Setting new eval start time to %" GST_TIME_FORMAT,
        GST_TIME_ARGS (rtsppad->start_time));
#endif
  }

  compensation_buffer = gst_buffer_make_writable (buf);
  /* By invalidating the buffer timestamp and setting
     do-timestamp=TRUE in the appsrc we ensure a monotonic timestamp on
     the rtsp server */
  GST_BUFFER_PTS (compensation_buffer) = GST_CLOCK_TIME_NONE;
  GST_BUFFER_DTS (compensation_buffer) = GST_CLOCK_TIME_NONE;

  GST_LOG_OBJECT (sink, "Pushing buffer to mapping %s",
      rtsppad->factory->mapping);

  ret = gst_app_src_push_buffer (rtsppad->appsrc, compensation_buffer);
  if (GST_FLOW_OK != ret)
    goto pushfail;

  GST_LOG_OBJECT (sink, "Pushed buffer into %s",
      GST_OBJECT_NAME (rtsppad->appsrc));

#ifdef EVAL
  if (GST_CLOCK_TIME_IS_VALID (rtsppad->start_time)) {
    GstClockTime end_time = 5 * 60 * GST_SECOND;
    GstClockTime so_far = GST_CLOCK_DIFF (rtsppad->start_time,
        GST_BUFFER_PTS (buf));

    GST_LOG_OBJECT (sink, "Eval time: start %" GST_TIME_FORMAT
        " so far %" GST_TIME_FORMAT, GST_TIME_ARGS (rtsppad->start_time),
        GST_TIME_ARGS (so_far));

    if (so_far >= end_time) {
      gst_element_send_event (GST_ELEMENT (factory->pipeline),
          gst_event_new_eos ());
      rtsppad->start_time = GST_CLOCK_TIME_NONE;
      print_eval ();
    }
  }
#endif

  return ret;

noattached:
  {
    GST_LOG_OBJECT (sink, "Server not attached yet");
    goto no_push;
  }
noprepared:
  {
    GST_LOG_OBJECT (sink,
        "Dropping buffer for mapping %s, no clients connected yet",
        rtsppad->factory->mapping);
    if (rtsppad->appsrc) {
      gst_object_unref (rtsppad->appsrc);
      rtsppad->appsrc = NULL;
    }
    goto no_push;
  }
noappsrc:
  {
    GST_ERROR_OBJECT (sink, "Unable to get appsrc for %s, aborting",
        GST_OBJECT_NAME (rtsppad));
    ret = GST_FLOW_ERROR;
    goto no_push;
  }
nocaps:
  {
    GST_ERROR_OBJECT (sink, "Unable to get negotiated caps");
    ret = GST_FLOW_ERROR;
    goto no_push;
  }
no_push:
  {
    gst_buffer_unref (buf);
    return ret;
  }
pushfail:
  {
    GST_INFO_OBJECT (sink,
        "Unable to push buffer into server, probably stream is closing");
    return GST_FLOW_OK;
  }

}

static const gchar *
rtsp_sink_get_payloader_from_mime (const gchar * mime)
{
  GstRtspSinkPayloader *payloader;

  for (payloader = payloaders; payloader; ++payloader) {
    if (!strcmp (mime, payloader->mimetype))
      return payloader->payloader;
  }

  return NULL;
}

#define PIPEDESC "appsrc is-live=true do-timestamp=true format=3 max-bytes=1 block=true name=%s ! queue ! %s name=pay%u"

static gchar *
rtsp_sink_get_pipeline (const gchar * mimetype, gchar * padname, guint index)
{
  gchar *pipeline = NULL;
  const gchar *payloader = NULL;

  payloader = rtsp_sink_get_payloader_from_mime (mimetype);
  if (!payloader)
    goto exit;

  /* Allocate mem for worst case scenario */
  pipeline = g_malloc (strlen (PIPEDESC) + strlen (padname) +
      strlen (payloader) + 1);
  sprintf (pipeline, PIPEDESC, padname, payloader, index);

exit:
  {
    return pipeline;
  }
}

static gboolean
rtsp_sink_get_factory_by_mapping (GstRtspSink * sink, GstRtspSinkPad * pad,
    gpointer data)
{
  gpointer *factorydata = (gpointer *) data;
  RrRtspMediaFactory **factory = (RrRtspMediaFactory **) factorydata[1];
  const gchar *mapping = (gchar *) factorydata[0];

  if (pad->factory && !strcmp (mapping, pad->factory->mapping)) {
    GST_DEBUG_OBJECT (sink, "Recycling mapping %s for %s", mapping,
        GST_OBJECT_NAME (pad));
    *factory = gst_object_ref (pad->factory);
    return FALSE;
  }
  return TRUE;
}

static gboolean
rtsp_sink_configure_mapping (GstRtspSink * sink, GstPad * pad,
    gchar * mapping, const gchar * mimetype)
{
  GstRtspSinkPad *mypad = GST_RTSP_SINK_PAD (pad);
  RrRtspMediaFactory *factory = NULL;
  GstRTSPMountPoints *rtsp_mount_points;
  gchar *new_pipeline = NULL;
  gchar *full_pipeline = NULL;
  gchar *current_pipeline = NULL;
  gboolean has_factory = FALSE;
  gpointer factorydata[2] = { mapping, &factory };
  /* Auth */
  GstRTSPAuth *auth;
  GstRTSPToken *token;
  gchar *basic;
  gchar *authorize = NULL;
  gchar *p, *comma, *col;
  gchar *user = NULL, *passwd = NULL;
  gboolean usernumber = TRUE;
  gboolean AUTHO = TRUE;

  rtsp_sink_iterate_pads (sink, rtsp_sink_get_factory_by_mapping, factorydata);
  has_factory = factory ? TRUE : FALSE;
  if (has_factory) {
    factory->index++;
  } else {
    /* Create the factory that will remain associated with this mapping */
    factory = rr_rtsp_media_factory_new ();
    if (!factory)
      goto no_factory;
  }


  /* Setting the multicast group */
  if (sink->multicast) {
    GST_INFO_OBJECT (sink, "multicast support set on %s",
        sink->multicast ? "true" : "false");
    /* make a new address pool */
    gst_rtsp_media_factory_set_address_pool (GST_RTSP_MEDIA_FACTORY
        (factory), sink->pool);
    /* only allow multicast */
    gst_rtsp_media_factory_set_protocols (GST_RTSP_MEDIA_FACTORY (factory),
        GST_RTSP_LOWER_TRANS_UDP_MCAST);
  }

  new_pipeline =
      rtsp_sink_get_pipeline (mimetype, GST_OBJECT_NAME (pad), factory->index);
  if (!new_pipeline)
    goto no_payloader;

  GST_INFO_OBJECT (sink, "Using \"%s\" as pipeline for %s", new_pipeline,
      GST_OBJECT_NAME (pad));

  if (has_factory) {
    current_pipeline =
        gst_rtsp_media_factory_get_launch (GST_RTSP_MEDIA_FACTORY (factory));
    full_pipeline =
        g_malloc (strlen (new_pipeline) + strlen (current_pipeline) + 5);
    sprintf (full_pipeline, "%s %s", new_pipeline, current_pipeline);
    GST_INFO_OBJECT (sink, "Using extended pipeline %s", full_pipeline);
    g_free (current_pipeline);
  } else {
    full_pipeline = g_strdup (new_pipeline);
  }

  /* get the mapping for this server, every server has a default mapper object
   * that be used to map uri mount points to media factories */
  rtsp_mount_points = gst_rtsp_server_get_mount_points (sink->server);
  if (!rtsp_mount_points)
    goto no_mapping;

  gst_rtsp_media_factory_set_launch (GST_RTSP_MEDIA_FACTORY (factory),
      full_pipeline);

  /* attach the test factory to the /test url */
  gst_rtsp_mount_points_add_factory (rtsp_mount_points, mapping,
      gst_object_ref (GST_RTSP_MEDIA_FACTORY (factory)));

  /* Store the name of the mapping */
  factory->mapping = g_strdup (mapping);
  mypad->factory = factory;

  /* don't need the ref to the mapper anymore */
  g_object_unref (rtsp_mount_points);
  g_free (full_pipeline);
  g_free (new_pipeline);

  GST_DEBUG_OBJECT (sink, "Configured %s properly on mapping: %s",
      GST_OBJECT_NAME (pad), mapping);

  /* Create a Authorization */
  if (sink->auth != NULL) {
    p = (gchar *) sink->auth;
    authorize = g_strdup (sink->auth);

    /* make a new authentication manager */
    auth = gst_rtsp_auth_new ();

    while (usernumber) {
      comma = strchr (p, ',');
      col = strchr (sink->auth, ':');
      if (comma && col && comma > col) {
        /* look for user:passwd */
        user = g_strndup (p, col - p);
        col++;
        passwd = g_strndup (col, comma - col);
        if (!strcmp (user, "")) {
          GST_ERROR_OBJECT (sink, "User not set for %s", passwd);
          AUTHO = FALSE;
        } else {
          AUTHO = TRUE;
        }
        p = g_strndup (comma + 1, authorize[strlen (sink->auth) - 1]);
        sink->auth = p;
      } else if (col && !comma) {
        /* look for user:passwd */
        user = g_strndup (p, col - p);
        col++;
        passwd = g_strndup (col, authorize[strlen (sink->auth) - 1]);
        if (!strcmp (user, "")) {
          goto no_user;
        }
        usernumber = FALSE;
        AUTHO = TRUE;
      } else if (!col || col > comma) {

        if (!col) {
          goto no_password;
        }
        user = g_strndup (p, comma - p);
        p = g_strndup (comma + 1, authorize[strlen (sink->auth) - 1]);
        GST_ERROR_OBJECT (sink, "Password not set for %s", user);
        sink->auth = p;
        AUTHO = FALSE;
      }

      if (AUTHO) {
        /* Add Auth */
        gst_rtsp_media_factory_add_role (GST_RTSP_MEDIA_FACTORY (factory), user,
            GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, TRUE,
            GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, TRUE, NULL);

        /* make user token */
        token =
            gst_rtsp_token_new (GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE,
            G_TYPE_STRING, user, NULL);

        basic = gst_rtsp_auth_make_basic (user, passwd);
        gst_rtsp_auth_add_basic (auth, basic, token);
        g_free (basic);
        g_free (user);
        g_free (passwd);
        gst_rtsp_token_unref (token);

        /* set as the server authentication manager */
        gst_rtsp_server_set_auth (sink->server, auth);
      }
    }
    g_object_unref (auth);
    g_free (authorize);
  }
  return TRUE;

no_factory:
  {
    GST_ERROR_OBJECT (sink, "Unable to create a new factory");
    return FALSE;
  }

no_payloader:
  {
    GST_ERROR_OBJECT (sink, "Unable to select payloader. Automatic detection "
        "works for aac, h264, mpeg4 and mjpeg.");
    factory->index--;
    gst_object_unref (factory);
    return FALSE;
  }

no_mapping:
  {
    GST_ERROR_OBJECT (sink, "Unable to get the RTSP server mapping");
    factory->index--;
    gst_object_unref (factory);
    g_free (full_pipeline);
    g_free (new_pipeline);
    return FALSE;
  }

no_password:
  {
    GST_ERROR_OBJECT (sink, "Password not set for %s", p);
    g_object_unref (auth);
    return TRUE;
  }

no_user:
  {
    GST_ERROR_OBJECT (sink, "User not set for %s", passwd);
    g_object_unref (auth);
    return TRUE;
  }
}

static gboolean
rtsp_sink_attach_if_needed (GstRtspSink * sink)
{
  gboolean needs_attach = TRUE;

  rtsp_sink_iterate_pads (sink, rtsp_sink_needs_attach, &needs_attach);
  if (needs_attach) {
    /* This is the same as set port */
    gst_rtsp_server_set_service (sink->server, sink->service);

    gst_rtsp_address_pool_clear (sink->pool);
    gst_rtsp_address_pool_add_range (sink->pool, sink->ip_min, sink->ip_max,
        sink->port_min, sink->port_max, sink->ttl);


    /* attach the server to the default maincontext */
    sink->source = gst_rtsp_server_attach (sink->server, NULL);
    if (sink->source == 0) {
      return FALSE;
    }

    sink->attached = TRUE;
    GST_INFO_OBJECT (sink, "Attached to server on port %s", sink->service);
  }

  return TRUE;
}

static gboolean
rtsp_sink_needs_attach (GstRtspSink * sink, GstRtspSinkPad * pad, gpointer data)
{
  gboolean *needs_attach = (gboolean *) data;

  if (!pad->factory) {
    GST_DEBUG_OBJECT (sink, "Still not configured mapping for pad %s",
        GST_OBJECT_NAME (pad));
    *needs_attach = FALSE;
    return FALSE;
  }

  return TRUE;
}

static gboolean
rtsp_sink_timeout (GstRtspSink * sink)
{
  GstRTSPServer *server = NULL;
  GstRTSPSessionPool *pool = NULL;

  g_return_val_if_fail (sink, FALSE);

  GST_LOG_OBJECT (sink, "Checking for expired sources");

  server = sink->server;

  pool = gst_rtsp_server_get_session_pool (server);
  gst_rtsp_session_pool_cleanup (pool);
  g_object_unref (pool);

  return TRUE;
}

static void
configure_timeout (GstRTSPClient * client, GstRTSPSession * session,
    gpointer user_data)
{
  GstRtspSink *sink = GST_RTSP_SINK (user_data);
  guint timeout = 0;

  GST_OBJECT_LOCK (sink);
  timeout = sink->timeout;
  GST_OBJECT_UNLOCK (sink);

  GST_INFO_OBJECT (sink, "Configuring session timeout to %u", timeout);
  gst_rtsp_session_set_timeout (session, timeout);
}

static void
rtsp_client_connected_callback (GstRTSPServer * server,
    GstRTSPClient * client, gpointer data)
{
  GstRtspSink *sink = GST_RTSP_SINK (data);

  GST_INFO_OBJECT (sink, "New client connected");
  g_signal_connect (client, "new-session", G_CALLBACK (configure_timeout),
      sink);
}

static gboolean
gst_rtsp_sink_start (GstRtspSink * sink)
{
  static const gint period = 2;

  GST_DEBUG_OBJECT (sink, "Initializing RtspSink");

  sink->server = gst_rtsp_server_new ();
  if (!sink->server) {
    GST_ERROR_OBJECT (sink, "Unable to create server");
    return FALSE;
  }

  sink->timeout_source =
      g_timeout_add_seconds (period, (GSourceFunc) rtsp_sink_timeout, sink);

  g_signal_connect (sink->server, "client-connected",
      G_CALLBACK (rtsp_client_connected_callback), sink);

#ifdef EVAL
  print_eval ();
#endif

  return TRUE;
}

static gboolean
rtsp_sink_remove_mappings (GstRtspSink * sink, GstRtspSinkPad * pad,
    gpointer data)
{
  GstRTSPMountPoints *mount_points;

  mount_points = gst_rtsp_server_get_mount_points (sink->server);
  gst_rtsp_mount_points_remove_factory (mount_points, pad->factory->mapping);

  /* Also remove factory references from pads that they are created
     again and avoid duplicate element errors in bins */
  gst_object_unref (pad->factory);
  pad->factory = NULL;

  gst_object_unref (mount_points);

  return TRUE;
}

static gboolean
rtsp_sink_reset_caps (GstRtspSink * sink, GstRtspSinkPad * pad, gpointer data)
{
  GstAppSink *appsink;
  gboolean ret = FALSE;

  g_return_val_if_fail (NULL != sink, FALSE);
  g_return_val_if_fail (NULL != pad, FALSE);

  appsink = GST_APP_SINK (gst_bin_get_by_name (GST_BIN (sink),
          GST_OBJECT_NAME (pad)));

  if (NULL != appsink) {
    gst_app_sink_set_caps (appsink, NULL);
    gst_object_unref (appsink);
    appsink = NULL;
    ret = TRUE;
  } else {
    GST_ERROR_OBJECT (sink, "Failed to reset sink pad %s:%s caps",
        GST_DEBUG_PAD_NAME (pad));
  }

  return ret;
}

static void
rtsp_sink_iterate_pads (GstRtspSink * sink, GstRtspSinkIteratorFunc func,
    gpointer data)
{
  GstIterator *iter = gst_element_iterate_sink_pads (GST_ELEMENT (sink));
  GstPad *pad = NULL;
  GValue value = G_VALUE_INIT;
  gboolean cont = TRUE;

  while (cont) {
    switch (gst_iterator_next (iter, &value)) {
      case GST_ITERATOR_OK:
        pad = GST_PAD (g_value_get_object (&value));
        cont = func (sink, GST_RTSP_SINK_PAD (pad), data);
        g_value_reset (&value);
        break;

      case GST_ITERATOR_RESYNC:
        gst_iterator_resync (iter);
        break;

      case GST_ITERATOR_ERROR:
      case GST_ITERATOR_DONE:
        cont = FALSE;
        break;
    }
  }
  gst_iterator_free (iter);
}

static void
gst_rtsp_sink_stop (GstRtspSink * sink)
{
  GstRTSPSessionPool *session_pool = NULL;

  GST_DEBUG_OBJECT (sink, "Closing current RTSP session");

  if (!sink->attached) {
    GST_INFO_OBJECT (sink, "Not attached");
    return;
  }

  /* Check for active sessions */
  session_pool = gst_rtsp_server_get_session_pool (sink->server);

  /* We get a list of the active sessions */
  gst_rtsp_session_pool_filter (session_pool,
      filter_session_poll_callback, NULL);

  g_object_unref (session_pool);

  /* remove the test factories url */
  rtsp_sink_iterate_pads (sink, rtsp_sink_remove_mappings, NULL);

  /* Remove the source from the main context to release the socket */
  if (sink->source)
    g_source_remove (sink->source);
  sink->source = 0;

  if (sink->timeout_source)
    g_source_remove (sink->timeout_source);
  sink->timeout_source = 0;

  if (sink->server)
    gst_object_unref (sink->server);
  sink->server = NULL;

  GST_INFO_OBJECT (sink, "Stopped service at port %s", sink->service);
  sink->attached = FALSE;

  /* Reset pads caps to allow re-negotiation */
  rtsp_sink_iterate_pads (sink, rtsp_sink_reset_caps, NULL);
}

static GstStateChangeReturn
gst_rtsp_sink_change_state (GstElement * element, GstStateChange transition)
{
  GstRtspSink *sink = GST_RTSP_SINK (element);
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;

  switch (transition) {
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      gst_rtsp_sink_start (sink);
      break;
    default:
      break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);
  if (ret == GST_STATE_CHANGE_FAILURE)
    return ret;

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      gst_rtsp_sink_stop (sink);
      break;
    default:
      break;
  }
  return ret;
}


/*
 * filter_session_poll_callback
 * 
 * This is a callback function which always returns GST_RTSP_FILTER_REF
 * in order to add all the existent sessions to the GList
 * 
 * \param pool  The session poll.
 * \param session Each existent session.
 * \param user_data Any user data sent by the callback.
 */
static GstRTSPFilterResult
filter_session_poll_callback (GstRTSPSessionPool * pool,
    GstRTSPSession * session, gpointer user_data)
{
  return GST_RTSP_FILTER_REMOVE;
}

static void
gst_rtsp_sink_finalize (GObject * object)
{
  GstRtspSink *sink = GST_RTSP_SINK (object);

  GST_DEBUG_OBJECT (sink, "Finalizing rtspsink");
  g_free (sink->service);
  g_free (sink->ip_min);
  g_free (sink->ip_max);
  g_free (sink->auth);

  g_object_unref (sink->pool);

  /* Chain up to the parent class */
  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gboolean
gst_rtsp_sink_event_function (GstPad * pad, GstObject * parent,
    GstEvent * event)
{
  gboolean ret = TRUE;

  GST_LOG_OBJECT (pad, "Got %s event", GST_EVENT_TYPE_NAME (event));

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps *caps;

      gst_event_parse_caps (event, &caps);
      ret = gst_rtsp_sink_set_caps (pad, caps);
      break;
    }
      /* fall through */
    default:
      break;
  }

  ret |= gst_pad_event_default (pad, parent, event);

  return ret;
}

#ifdef EVAL
static void
print_eval ()
{
  g_print ("                                     \n"
      "*************************************\n"
      "*** THIS IS AN EVALUATION VERSION ***\n"
      "*************************************\n"
      "                                     \n"
      "  Thanks for evaluating GstRtspSink! \n"
      "                                     \n"
      "  A total of 5minutes will be        \n"
      "  tranferred per pad. You will not be\n"
      "  able to reconnect. Please contact  \n"
      "  <support@ridgerun.com> to purchase \n"
      "  the professional version of the    \n"
      "  plug-in.                           \n"
      "                                     \n"
      "*************************************\n"
      "*** THIS IS AN EVALUATION VERSION ***\n"
      "*************************************\n"
      "                                     \n");
}
#endif
