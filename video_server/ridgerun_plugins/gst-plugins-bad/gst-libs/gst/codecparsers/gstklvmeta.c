/*
 * GStreamer
 * Copyright (C) 2017 Michael Fien <mfien@harris.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstklvmeta.h"

GST_DEBUG_CATEGORY (klv_meta_debug);
#define GST_CAT_DEFAULT klv_meta_debug

static gboolean
gst_klv_meta_init (GstKlvMeta * klv_meta, gpointer params, GstBuffer * buffer)
{
  klv_meta->metadata_service_id = 0;
  klv_meta->sequence_number = 0;
  klv_meta->flags = 0;
  klv_meta->au_cell_data_length = 0;

  return TRUE;
}

static void
gst_klv_meta_free (GstKlvMeta * klv_meta, GstBuffer * buffer)
{
}

static gboolean
gst_klv_meta_transform (GstBuffer * dest, GstMeta * meta,
    GstBuffer * buffer, GQuark type, gpointer data)
{
  GstKlvMeta *smeta, *dmeta;

  smeta = (GstKlvMeta *) meta;

  if (GST_META_TRANSFORM_IS_COPY (type)) {
    GstMetaTransformCopy *copy = data;

    if (!copy->region) {
      /* only copy if the complete data is copied as well */
      dmeta =
          gst_buffer_add_klv_meta (dest, smeta->metadata_service_id,
          smeta->sequence_number, smeta->flags, smeta->au_cell_data_length);

      if (!dmeta)
        return FALSE;
    }
  } else {
    /* return FALSE, if transform type is not supported */
    return FALSE;
  }

  return TRUE;
}

GType
gst_klv_meta_api_get_type (void)
{
  static volatile GType type;
  static const gchar *tags[] = { "au_header", NULL };   /* don't know what to set here */

  if (g_once_init_enter (&type)) {
    GType _type = gst_meta_api_type_register ("GstKlvMetaAPI", tags);
    GST_DEBUG_CATEGORY_INIT (klv_meta_debug, "klvmeta", 0, "KLV GstMeta");

    g_once_init_leave (&type, _type);
  }
  return type;
}

const GstMetaInfo *
gst_klv_meta_get_info (void)
{
  static const GstMetaInfo *klv_meta_info = NULL;

  if (g_once_init_enter ((GstMetaInfo **) & klv_meta_info)) {
    const GstMetaInfo *meta = gst_meta_register (GST_KLV_META_API_TYPE,
        "GstKlvMeta", sizeof (GstKlvMeta),
        (GstMetaInitFunction) gst_klv_meta_init,
        (GstMetaFreeFunction) gst_klv_meta_free,
        (GstMetaTransformFunction) gst_klv_meta_transform);
    g_once_init_leave ((GstMetaInfo **) & klv_meta_info, (GstMetaInfo *) meta);
  }

  return klv_meta_info;
}

/**
 * gst_buffer_add_klv_meta:
 * @buffer: a #GstBuffer
 *
 * Creates and adds a #GstKlvMeta to a @buffer.
 *
 * Returns: (transfer full): a newly created #GstKlvMeta
 *
 * Since: 1.2
 */
GstKlvMeta *
gst_buffer_add_klv_meta (GstBuffer * buffer,
    const guint8 metadata_service_id,
    const guint8 sequence_number,
    const guint8 flags, const guint16 au_cell_data_length)
{
  GstKlvMeta *klv_meta;

  klv_meta =
      (GstKlvMeta *) gst_buffer_add_meta (buffer, GST_KLV_META_INFO, NULL);

  GST_DEBUG
      ("metadata_service_id:%i, sequence_number:%i, flags:%i, au_cell_data_length:%i",
      metadata_service_id, sequence_number, flags, au_cell_data_length);

  klv_meta->metadata_service_id = metadata_service_id;
  klv_meta->sequence_number = sequence_number;
  klv_meta->flags = flags;
  klv_meta->au_cell_data_length = au_cell_data_length;

  return klv_meta;
}
