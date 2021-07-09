/* GStreamer 
 * Copyright (C) 2019 RidgeRun Engineering  Edison Fernandez <edison.fernandez@ridgerun.com> 
 * 
 * gstmisbparser.c:
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
/**
 * SECTION:element-gstmisbparser
 *
 * The misbparser element checks and fixes (if necessary) 
 * the content of an incomming KLV packet. It can also dump 
 * its content. It currently support ST0601.14 and ST0604.5
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v filesrc location=video.ts ! tsdemux ! misbparser ! fakesink
 * ]|
 * In this example, a transport stream file containing KLV metadata is 
 * read, the KLV packets are extracted by tsdemux and then parsed by
 * misbparser.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstmisbparser.h"

#include <gst/gst.h>
#include <gst/base/gstbaseparse.h>
#include <string.h>
#include <stdio.h>

#include "misbparser.h"
#include "klvutils.h"
#include "st0601dictionary.h"
#include "st0604dictionary.h"

GST_DEBUG_CATEGORY_STATIC (gst_misb_parser_debug_category);
#define GST_CAT_DEFAULT gst_misb_parser_debug_category

#define DEFAULT_PROP_DUMP TRUE

/* prototypes */


static void gst_misb_parser_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_misb_parser_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static GstFlowReturn gst_misb_parser_pre_push_frame (GstBaseParse * parse,
    GstBaseParseFrame * frame);
static GstFlowReturn gst_misb_parser_handle_frame (GstBaseParse * parse,
    GstBaseParseFrame * frame, gint * skipsize);
static void gst_misb_parser_finalize (GObject * object);

enum
{
  PROP_0,
  PROP_DUMP
};

struct _GstMisbParser
{
  GstBaseParse base_misbparser;

  MisbParser *parser;

  gboolean dump;
};

struct _GstMisbParserClass
{
  GstBaseParseClass base_misbparser_class;
};

/* pad templates */

static GstStaticPadTemplate gst_misb_parser_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("meta/x-klv")
    );

static GstStaticPadTemplate gst_misb_parser_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("meta/x-klv")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMisbParser, gst_misb_parser,
    GST_TYPE_BASE_PARSE,
    GST_DEBUG_CATEGORY_INIT (gst_misb_parser_debug_category, "misbparser", 0,
        "debug category for misbparser element"));

static void
gst_misb_parser_class_init (GstMisbParserClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseParseClass *base_parse_class = GST_BASE_PARSE_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS (klass),
      gst_static_pad_template_get (&gst_misb_parser_src_template));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS (klass),
      gst_static_pad_template_get (&gst_misb_parser_sink_template));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MISB Parser",
      "Parser/Converter/Meta",
      "Parses data from MISB ST0601 ",
      "Edison Fernandez <edison.fernandez@ridgerun.com>");

  gobject_class->set_property = gst_misb_parser_set_property;
  gobject_class->get_property = gst_misb_parser_get_property;
  gobject_class->finalize = gst_misb_parser_finalize;
  base_parse_class->pre_push_frame =
      GST_DEBUG_FUNCPTR (gst_misb_parser_pre_push_frame);
  base_parse_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_misb_parser_handle_frame);

  g_object_class_install_property (gobject_class, PROP_DUMP,
      g_param_spec_boolean ("dump", "Dump",
          "Dump metadata to the standard output", DEFAULT_PROP_DUMP,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

}

static void
gst_misb_parser_init (GstMisbParser * misbparser)
{
  misbparser->dump = DEFAULT_PROP_DUMP;
  misbparser->parser = misb_parser_alloc ();

  /*Add standards */
  misb_parser_add_std (misbparser->parser, MISB_STD_0601);
  misb_parser_add_std (misbparser->parser, MISB_STD_0604);
}

static void
gst_misb_parser_finalize (GObject * object)
{
  GstMisbParser *misbparser = NULL;
  misbparser = GST_MISB_PARSER (object);

  misb_parser_release (misbparser->parser);
  misbparser->parser = NULL;
}

void
gst_misb_parser_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMisbParser *misbparser = GST_MISB_PARSER (object);

  GST_DEBUG_OBJECT (misbparser, "set_property");

  switch (property_id) {
    case PROP_DUMP:
      misbparser->dump = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_misb_parser_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMisbParser *misbparser = GST_MISB_PARSER (object);

  GST_DEBUG_OBJECT (misbparser, "get_property");

  switch (property_id) {
    case PROP_DUMP:
      g_value_set_boolean (value, misbparser->dump);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static GstFlowReturn
gst_misb_parser_pre_push_frame (GstBaseParse * parse, GstBaseParseFrame * frame)
{
  GstMisbParser *misbparser = GST_MISB_PARSER (parse);
  GstBuffer *inbuf = frame->buffer;
  GstBuffer *outbuf;
  GstMapInfo inmap;
  GstMapInfo outmap;
  gsize insize;
  Klv *klv, *new_klv;
  Lds *lds;
  GstFlowReturn ret;
  MisbReturn parse_ret;

  GST_LOG_OBJECT (misbparser, "pre_push_frame");

  ret = GST_FLOW_OK;
  lds = NULL;
  klv = NULL;
  new_klv = NULL;
  insize = gst_buffer_get_size (inbuf);
  GST_LOG_OBJECT (misbparser,
      "Size of input buffer is %" G_GSIZE_FORMAT " bytes", insize);

  gst_buffer_map (inbuf, &inmap, GST_MAP_READ);

  if (!klv_get (inmap.data, &klv)) {
    GST_WARNING_OBJECT (misbparser, "Could not get KLV data, dropping buffer");
    ret = GST_BASE_PARSE_FLOW_DROPPED;
    goto release_resources;
  }

  parse_ret =
      misb_parser_validate_packet (misbparser->parser, klv, &new_klv,
      misbparser->dump);

  if (parse_ret == MISB_RET_OK) {
    /*Packet is correct, so lets just pass the data */
    ret = GST_FLOW_OK;
    goto release_resources;
  } else if (parse_ret == MISB_RET_FIXED) {
    /*The packet was somehow fixed so we need to allocate a buffer
       for it */
    GST_LOG_OBJECT (misbparser, "Packet was fixed, Allocating new buffer");

    outbuf = gst_buffer_new_allocate (NULL, klv_packet_length (new_klv), NULL);

    gst_buffer_map (outbuf, &outmap, GST_MAP_WRITE);
    memcpy (outmap.data, klv_raw_data (new_klv), klv_packet_length (new_klv));
    gst_buffer_unmap (outbuf, &outmap);

    frame->out_buffer = outbuf;

    klv_release (new_klv);
    ret = GST_FLOW_OK;
    goto release_resources;
  }

  /* The package was wrong and couldn't be fixed 
     so lets drop the buffer */
  ret = GST_BASE_PARSE_FLOW_DROPPED;

release_resources:

  gst_buffer_unmap (inbuf, &inmap);
  if (klv)
    klv_release (klv);
  if (lds)
    lds_release (lds);

  return ret;
}

static GstFlowReturn
gst_misb_parser_handle_frame (GstBaseParse * parse,
    GstBaseParseFrame * frame, gint * skipsize)
{
  GstMisbParser *misbparser = GST_MISB_PARSER (parse);
  gsize insize = gst_buffer_get_size (frame->buffer);

  GST_LOG_OBJECT (misbparser, "handle_frame");

  if (!gst_pad_has_current_caps (GST_BASE_PARSE_SRC_PAD (parse))) {
    GstCaps *caps;

    caps = gst_caps_new_empty_simple ("meta/x-klv");
    gst_pad_set_caps (GST_BASE_PARSE_SRC_PAD (parse), caps);
    gst_caps_unref (caps);
  }

  return gst_base_parse_finish_frame (parse, frame, insize);
}
