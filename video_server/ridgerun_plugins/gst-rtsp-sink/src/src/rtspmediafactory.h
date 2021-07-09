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

#ifndef __RTSP_MEDIA_FACTORY_H__
#define __RTSP_MEDIA_FACTORY_H__

#include <gst/rtsp-server/rtsp-media-factory.h>
#include <gst/app/gstappsrc.h>
#include <gst/rtsp-server/rtsp-media.h>
#include <gst/gst.h>

G_BEGIN_DECLS
#define TYPE_RR_RTSP_MEDIA_FACTORY (rr_rtsp_media_factory_get_type ())
#define RR_RTSP_MEDIA_FACTORY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_RR_RTSP_MEDIA_FACTORY, RrRtspMediaFactory))
#define RR_RTSP_MEDIA_FACTORY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_RR_RTSP_MEDIA_FACTORY, RrRtspMediaFactoryClass))
#define IS_RR_RTSP_MEDIA_FACTORY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_RR_RTSP_MEDIA_FACTORY))
#define IS_RR_RTSP_MEDIA_FACTORY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_RR_RTSP_MEDIA_FACTORY))
#define RR_RTSP_MEDIA_FACTORY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_RR_RTSP_MEDIA_FACTORY, RrRtspMediaFactoryClass))
typedef struct _RrRtspMediaFactory RrRtspMediaFactory;
typedef struct _RrRtspMediaFactoryClass RrRtspMediaFactoryClass;

struct _RrRtspMediaFactory
{
  GstRTSPMediaFactory parent;
  GstElement *pipeline;
  gchar *mapping;
  gboolean is_prepared;
  guint index;
};

struct _RrRtspMediaFactoryClass
{
  GstRTSPMediaFactoryClass parent_class;
};

RrRtspMediaFactory *rr_rtsp_media_factory_new (void);
GType rr_rtsp_media_factory_get_type (void);
G_END_DECLS
#endif // __RTSP_MEDIA_FACTORY_H__
