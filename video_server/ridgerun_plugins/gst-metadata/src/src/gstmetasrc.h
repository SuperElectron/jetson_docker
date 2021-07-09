/* GStreamer
 * Copyright (C) 2016 RidgeRun Engineering  Michael Gruner <michael.gruner@ridgerun.com> 
 *                                          Fabricio Quiros <fabricio.quiros@ridgerun.com
 * 
 * gstmetasrc.h:
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

#ifndef __GST_META_SRC_H__
#define __GST_META_SRC_H__

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#include <string.h>
G_BEGIN_DECLS
#define GST_TYPE_META_SRC \
    (gst_meta_src_get_type ())
#define GST_META_SRC(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_META_SRC, GstMetaSrc))
#define GST_META_SRC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_META_SRC, GstMetaSrcClass))
#define GST_IS_META_SRC(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_META_SRC))
#define GST_IS_META_SRC_CLASS(klass) \
(G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_META_SRC))

typedef struct _GstMetaSrc GstMetaSrc;
typedef struct _GstMetaSrcClass GstMetaSrcClass;

struct _GstMetaSrc
{
  GstAppSrc parent;

  /* properties */
  guint period;
  gchar *metadata;
  GByteArray *binary;

  /* state */
  gboolean started;
  gboolean has_pattern;

  /* global */
  gulong probe_id;
  guint src_id;
};

struct _GstMetaSrcClass
{
  GstAppSrcClass parent_class;

  /* Actions */
  GstFlowReturn (*push_metadata) (GstMetaSrc *metasrc, guint8 *metadata, gsize size);
};

/* Standard function returning type information */
GType gst_meta_src_get_type (void);

G_END_DECLS
#endif /* __GST_META_SRC_H__ */
