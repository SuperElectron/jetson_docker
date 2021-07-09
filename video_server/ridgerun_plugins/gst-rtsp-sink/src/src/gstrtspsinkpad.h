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

#ifndef __RTSP_SINK_PAD_H__
#define __RTSP_SINK_PAD_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include "rtspmediafactory.h"
#include <gst/app/gstappsink.h>

G_BEGIN_DECLS
#define TYPE_GST_RTSP_SINK_PAD (gst_rtsp_sink_pad_get_type ())
#define GST_RTSP_SINK_PAD(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_GST_RTSP_SINK_PAD, GstRtspSinkPad))
#define GST_RTSP_SINK_PAD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_GST_RTSP_SINK_PAD, GstRtspSinkPadClass))
#define IS_GST_RTSP_SINK_PAD(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_GST_RTSP_SINK_PAD))
#define IS_GST_RTSP_SINK_PAD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_GST_RTSP_SINK_PAD))
#define GST_RTSP_SINK_PAD_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_GST_RTSP_SINK_PAD, GstRtspSinkPadClass))
typedef struct _GstRtspSinkPad GstRtspSinkPad;
typedef struct _GstRtspSinkPadClass GstRtspSinkPadClass;

struct _GstRtspSinkPad
{
  GstGhostPad parent;
  RrRtspMediaFactory *factory;
  GstAppSrc *appsrc;
  gchar *mapping;
  GstAppSink *appsink;

#ifdef EVAL
  GstClockTime start_time;
#endif
};

struct _GstRtspSinkPadClass
{
  GstGhostPadClass parent_class;
};

GstRtspSinkPad *gst_rtsp_sink_pad_new_from_template (GstPadTemplate * templ,
    const gchar * name);
GType gst_rtsp_sink_pad_get_type (void);

G_END_DECLS
#endif // __RTSP_SINK_PAD_H__
