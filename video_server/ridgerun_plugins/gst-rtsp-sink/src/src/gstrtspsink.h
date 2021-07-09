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

#ifndef __GST_RTSP_SINK_H__
#define __GST_RTSP_SINK_H__

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/rtsp-server/rtsp-server.h>
#include "rtspmediafactory.h"
#include "gstrtspsinkpad.h"

G_BEGIN_DECLS
#define GST_TYPE_RTSP_SINK			\
  (gst_rtsp_sink_get_type())
#define GST_RTSP_SINK(obj)				                \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_RTSP_SINK,GstRtspSink))
#define GST_RTSP_SINK_CLASS(klass)					\
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_RTSP_SINK,GstRtspSinkClass))
#define GST_IS_RTSP_SINK(obj)					\
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_RTSP_SINK))
#define GST_IS_RTSP_SINK_CLASS(klass)			\
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_RTSP_SINK))
typedef struct _GstRtspSink GstRtspSink;
typedef struct _GstRtspSinkClass GstRtspSinkClass;

struct _GstRtspSink
{
  /* Gstreamer infrastructure */
  GstBin parent;

  /* Rtsp server interface */
  GstRTSPServer *server;

  /* Control variables */
  gboolean attached;

  /* Main context source */
  gint source;

  /* Expired sessions */
  gint timeout_source;
  guint timeout;

  /* Port to attach the server to */
  gchar *service;

  gchar *ip_min;

  gchar *ip_max;

  guint port_min;

  guint port_max;

  gboolean multicast;

  guint ttl;

  guint padcount;

  gchar *auth;

  GstRTSPAddressPool *pool;
  
  GMutex padlock;
};

struct _GstRtspSinkClass
{
  GstBinClass parent_class;
};


GType gst_rtsp_sink_get_type (void);

G_END_DECLS
#endif /* __GST_RTSP_SINK_H__ */
