/*
 * gstrtponviftimestamp-parse.c
 *
 * Copyright (C) 2014 Axis Communications AB
 *  Author: Guillaume Desmottes <guillaume.desmottes@collabora.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gst/rtp/gstrtpbuffer.h>

#include "gstrtponvifparse.h"

static GstFlowReturn gst_rtp_onvif_parse_chain (GstPad * pad,
    GstObject * parent, GstBuffer * buf);

static GstStaticPadTemplate sink_template_factory =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/x-rtp")
    );

static GstStaticPadTemplate src_template_factory =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("application/x-rtp")
    );

G_DEFINE_TYPE (GstRtpOnvifParse, gst_rtp_onvif_parse, GST_TYPE_ELEMENT);

static void
gst_rtp_onvif_parse_class_init (GstRtpOnvifParseClass * klass)
{
  GstElementClass *gstelement_class;

  gstelement_class = GST_ELEMENT_CLASS (klass);

  /* register pads */
  gst_element_class_add_static_pad_template (gstelement_class,
      &sink_template_factory);
  gst_element_class_add_static_pad_template (gstelement_class,
      &src_template_factory);

  gst_element_class_set_static_metadata (gstelement_class,
      "ONVIF NTP timestamps RTP extension", "Effect/RTP",
      "Add absolute timestamps and flags of recorded data in a playback "
      "session", "Guillaume Desmottes <guillaume.desmottes@collabora.com>");
}

static void
gst_rtp_onvif_parse_init (GstRtpOnvifParse * self)
{
  self->sinkpad =
      gst_pad_new_from_static_template (&sink_template_factory, "sink");
  gst_pad_set_chain_function (self->sinkpad, gst_rtp_onvif_parse_chain);
  gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);
  GST_PAD_SET_PROXY_CAPS (self->sinkpad);

  self->srcpad =
      gst_pad_new_from_static_template (&src_template_factory, "src");
  gst_element_add_pad (GST_ELEMENT (self), self->srcpad);
}

#define EXTENSION_ID 0xABAC
#define EXTENSION_SIZE 3

static gboolean
handle_buffer (GstRtpOnvifParse * self, GstBuffer * buf)
{
  GstRTPBuffer rtp = GST_RTP_BUFFER_INIT;
  guint8 *data;
  guint16 bits;
  guint wordlen;
  guint8 flags;
  /*
     guint64 timestamp;
     guint8 cseq;
   */

  if (!gst_rtp_buffer_map (buf, GST_MAP_READ, &rtp)) {
    GST_ELEMENT_ERROR (self, STREAM, FAILED,
        ("Failed to map RTP buffer"), (NULL));
    return FALSE;
  }

  /* Check if the ONVIF RTP extension is present in the packet */
  if (!gst_rtp_buffer_get_extension_data (&rtp, &bits, (gpointer) & data,
          &wordlen))
    goto out;

  if (bits != EXTENSION_ID || wordlen != EXTENSION_SIZE)
    goto out;

  /* timestamp = GST_READ_UINT64_BE (data);  TODO */
  flags = GST_READ_UINT8 (data + 8);
  /* cseq = GST_READ_UINT8 (data + 9);  TODO */

  /* C */
  if (flags & (1 << 7))
    GST_BUFFER_FLAG_UNSET (buf, GST_BUFFER_FLAG_DELTA_UNIT);
  else
    GST_BUFFER_FLAG_SET (buf, GST_BUFFER_FLAG_DELTA_UNIT);

  /* E */
  /* if (flags & (1 << 6));  TODO */

  /* D */
  if (flags & (1 << 5))
    GST_BUFFER_FLAG_SET (buf, GST_BUFFER_FLAG_DISCONT);
  else
    GST_BUFFER_FLAG_UNSET (buf, GST_BUFFER_FLAG_DISCONT);

out:
  gst_rtp_buffer_unmap (&rtp);
  return TRUE;
}

static GstFlowReturn
gst_rtp_onvif_parse_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  GstRtpOnvifParse *self = GST_RTP_ONVIF_PARSE (parent);

  if (!handle_buffer (self, buf)) {
    gst_buffer_unref (buf);
    return GST_FLOW_ERROR;
  }

  return gst_pad_push (self->srcpad, buf);
}
