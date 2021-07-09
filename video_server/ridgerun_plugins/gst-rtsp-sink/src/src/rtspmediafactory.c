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

#include "rtspmediafactory.h"

GST_DEBUG_CATEGORY_STATIC (rr_rtsp_media_factory_debug);
#define GST_CAT_DEFAULT rr_rtsp_media_factory_debug

/* This will create rr_rtsp_factory_get_type and 
 * set rr_rtsp_factory_parent_class */
G_DEFINE_TYPE (RrRtspMediaFactory, rr_rtsp_media_factory,
    GST_TYPE_RTSP_MEDIA_FACTORY);
#define parent_class rr_rtsp_media_factory_parent_class

/* VTable */
static GstElement *rr_rtsp_media_factory_create_pipeline (GstRTSPMediaFactory *
    base, GstRTSPMedia * media);

static void rr_rtsp_media_factory_unprepared (GstRTSPMedia * media,
    gpointer data);

static void rr_rtsp_media_factory_prepared (GstRTSPMedia * media,
    gpointer data);

static void rr_rtsp_media_factory_finalize (GObject * object);

RrRtspMediaFactory *
rr_rtsp_media_factory_new (void)
{
  return g_object_new (TYPE_RR_RTSP_MEDIA_FACTORY, NULL);
}

static void
rr_rtsp_media_factory_class_init (RrRtspMediaFactoryClass * klass)
{
  GstRTSPMediaFactoryClass *rtsp_class;
  GObjectClass *gobject_class;

  rtsp_class = GST_RTSP_MEDIA_FACTORY_CLASS (klass);
  gobject_class = G_OBJECT_CLASS (klass);

  /* Override the function */
  rtsp_class->create_pipeline = rr_rtsp_media_factory_create_pipeline;

  /* Need a custom finalize function to free mapping */
  gobject_class->finalize = rr_rtsp_media_factory_finalize;

  GST_DEBUG_CATEGORY_INIT (rr_rtsp_media_factory_debug, "rrrtspmediafactory", 0,
      "RR Rtsp Media Factory");
}

/* Constructor */
static void
rr_rtsp_media_factory_init (RrRtspMediaFactory * this)
{
  this->pipeline = NULL;
  this->mapping = NULL;
  this->is_prepared = FALSE;
  this->index = 0;

  /* If many clients connect, make them use our same pipeline instead of
   * creating a new one for each */
  gst_rtsp_media_factory_set_shared (GST_RTSP_MEDIA_FACTORY (this), TRUE);
}

/* Override function */
static GstElement *
rr_rtsp_media_factory_create_pipeline (GstRTSPMediaFactory * base,
    GstRTSPMedia * media)
{
  RrRtspMediaFactory *this = (RrRtspMediaFactory *) base;

  /* If the elements in the configured pipe are null then
     we cant do anything yet */
  GstElement *media_element = gst_rtsp_media_get_element (media);
  if (media_element == NULL) {
    return NULL;
  }
  gst_object_unref (media_element);
  /* Create the pipeline with the elements and grab the source */
  this->pipeline = gst_pipeline_new ("media-pipeline");
  gst_rtsp_media_take_pipeline (media, GST_PIPELINE (this->pipeline));

  /* We also need a reference to the media to 
     connect to the signals */
  /* Connect to the media signals */
  g_signal_connect (media, "unprepared",
      G_CALLBACK (rr_rtsp_media_factory_unprepared), (gpointer) this);
  g_signal_connect (media, "prepared",
      G_CALLBACK (rr_rtsp_media_factory_prepared), (gpointer) this);

  GST_INFO_OBJECT (this, "Successfully created pipeline");
  this->is_prepared = TRUE;

  return this->pipeline;
}

/* Callbacks to rtsp media */

/* No more clients */
static void
rr_rtsp_media_factory_unprepared (GstRTSPMedia * media, gpointer data)
{
  RrRtspMediaFactory *this = RR_RTSP_MEDIA_FACTORY (data);
  GST_INFO_OBJECT (this, "Connections closed");
  this->is_prepared = FALSE;
  this->pipeline = NULL;
}

/* Prepared to stream */
static void
rr_rtsp_media_factory_prepared (GstRTSPMedia * media, gpointer data)
{
  RrRtspMediaFactory *this = RR_RTSP_MEDIA_FACTORY (data);
  GST_INFO_OBJECT (this, "Prepared to stream");
  this->is_prepared = TRUE;
}

/* Object destructor
 */
static void
rr_rtsp_media_factory_finalize (GObject * object)
{
  RrRtspMediaFactory *this = RR_RTSP_MEDIA_FACTORY (object);

  GST_DEBUG_OBJECT (this, "Freeing media factory");

  if (this->mapping)
    g_free (this->mapping);
  /* Chain up to the parent class */
  G_OBJECT_CLASS (parent_class)->finalize (object);
}
