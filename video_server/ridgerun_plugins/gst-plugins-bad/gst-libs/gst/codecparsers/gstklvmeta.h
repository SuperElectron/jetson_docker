/* Gstreamer
 * Copyright (C) <2017> Michael Fien <mfien@harris.com>
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

#ifndef __GST_KLV_META_H__
#define __GST_KLV_META_H__

#include <gst/gst.h>
// #include <gst/codecparsers/codecparsers-prelude.h>

G_BEGIN_DECLS

typedef struct _GstKlvMeta GstKlvMeta;

GST_EXPORT
GType gst_klv_meta_api_get_type (void);
#define GST_KLV_META_API_TYPE  (gst_klv_meta_api_get_type())
#define GST_KLV_META_INFO  (gst_klv_meta_get_info())
const GstMetaInfo * gst_klv_meta_get_info (void);

/**
 * GstKlvMeta:
 * @meta: parent #GstMeta
 * @metadata_service_id: the Metadata service id
 * @sequence_number: the sequence number
 * @flags: the flags
 * @au_cell_data_length: the AU cell data length
 *
 * Extra buffer metadata describing Metadata AU header.
 *
 * Can be used by mpegtsmuxer add metadata au header back to
 * synchronous KLV metadata per MISB 0604.
 *
 * The various fields are only valid during the lifetime of the #GstKlvMeta.
 * If elements wish to use those for longer, they are required to make a copy.
 *
 * Since: 1.2
 */
struct _GstKlvMeta {
  GstMeta            meta;

  guint8             metadata_service_id;
  guint8             sequence_number;
  guint8             flags;
  guint16            au_cell_data_length;
};


#define gst_buffer_get_klv_meta(b) ((GstKlvMeta*)gst_buffer_get_meta((b),GST_KLV_META_API_TYPE))

GST_EXPORT
GstKlvMeta * gst_buffer_add_klv_meta (GstBuffer * buffer,
                                      const guint8 metadata_service_id,
                                      const guint8 sequence_number,
                                      const guint8 flags,
                                      const guint16 au_cell_data_length);

G_END_DECLS

#endif
