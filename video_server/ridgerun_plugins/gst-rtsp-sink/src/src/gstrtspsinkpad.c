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

#include "gstrtspsinkpad.h"

GST_DEBUG_CATEGORY_STATIC (gst_rtsp_sink_pad_debug);
#define GST_CAT_DEFAULT gst_rtsp_sink_pad_debug

/* This will create rr_rtsp_factory_get_type and 
 * set rr_rtsp_factory_parent_class */
G_DEFINE_TYPE (GstRtspSinkPad, gst_rtsp_sink_pad, GST_TYPE_GHOST_PAD);
#define parent_class gst_rtsp_sink_pad_parent_class

/* VTable */
static void gst_rtsp_sink_pad_finalize (GObject * object);
static GObject *gst_rtsp_sink_pad_constructor (GType gtype, guint n_properties,
    GObjectConstructParam * properties);

GstRtspSinkPad *
gst_rtsp_sink_pad_new_from_template (GstPadTemplate * templ, const gchar * name)
{
  GstRtspSinkPad *pad;
  pad = g_object_new (TYPE_GST_RTSP_SINK_PAD,
      "name", name, "template", templ, "direction", templ->direction, NULL);

  /* Finish the ghost pad initialization */
  gst_ghost_pad_construct (GST_GHOST_PAD (pad));

  return pad;
}

static void
gst_rtsp_sink_pad_class_init (GstRtspSinkPadClass * klass)
{
  GObjectClass *gobject_class;
  gobject_class = G_OBJECT_CLASS (klass);

  /* Need a custom finalize function to free mapping */
  gobject_class->finalize = gst_rtsp_sink_pad_finalize;
  gobject_class->constructor = gst_rtsp_sink_pad_constructor;

  GST_DEBUG_CATEGORY_INIT (gst_rtsp_sink_pad_debug, "rtspsinkpad", 0,
      "GstRtspSinkPad");
}


/* We just create a dummy constructor in order to chain up with the
   parent one so the creation time properties are properly set */
static GObject *
gst_rtsp_sink_pad_constructor (GType gtype, guint n_properties,
    GObjectConstructParam * properties)
{
  /* Pass the construction time properties to the parent */
  return G_OBJECT_CLASS (parent_class)->constructor (gtype, n_properties,
      properties);
}

static void
gst_rtsp_sink_pad_init (GstRtspSinkPad * this)
{
  this->factory = NULL;
  this->appsrc = NULL;
  this->appsink = NULL;

#ifdef EVAL
  this->start_time = GST_CLOCK_TIME_NONE;
#endif
}

/* Object destructor
 */
static void
gst_rtsp_sink_pad_finalize (GObject * object)
{
  GstRtspSinkPad *this = GST_RTSP_SINK_PAD (object);

  GST_INFO_OBJECT (this, "Freeing pad %s", GST_OBJECT_NAME (this));

  if (this->factory) {
    gst_object_unref (this->factory);
  }
  this->factory = NULL;

  if (this->appsrc) {
    gst_object_unref (this->appsrc);
  }
  this->appsrc = NULL;

  /* Chain up to the parent class */
  G_OBJECT_CLASS (parent_class)->finalize (object);
}
