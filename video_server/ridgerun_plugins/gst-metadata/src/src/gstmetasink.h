/* GStreamer
 * Copyright (C) 2016 RidgeRun Engineering  Michael Gruner <michael.gruner@ridgerun.com>
 *                                          Fabricio Quiros <fabricio.quiros@ridgerun.com
 *
 * gstmetasink.h:
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

#ifndef __GST_META_SINK_H__
#define __GST_META_SINK_H__

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

G_BEGIN_DECLS
#define GST_TYPE_META_SINK \
  (gst_meta_sink_get_type())
#define GST_META_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_META_SINK,GstMetaSink))
#define GST_META_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_META_SINK,GstMetaSinkClass))
#define GST_IS_META_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_META_SINK))
#define GST_IS_META_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_META_SINK))
#define GST_META_SINK_CAST(obj) ((GstMetaSink *)obj)
typedef struct _GstMetaSink GstMetaSink;
typedef struct _GstMetaSinkClass GstMetaSinkClass;

struct _GstMetaSink
{
  GstAppSink parent;

  /* property */
  gboolean dump;
  gboolean signal_new_metadata;
#ifdef EVAL
  GstClockTime start_time;
#endif
};

struct _GstMetaSinkClass
{
  GstAppSinkClass parent_class;

  /* signals */
  void (*new_metadata) (GstMetaSink *sink, guint size, const guint8 * data);
};

GType gst_meta_sink_get_type (void);

G_END_DECLS
#endif /* __GST_META_SINK_H__ */
