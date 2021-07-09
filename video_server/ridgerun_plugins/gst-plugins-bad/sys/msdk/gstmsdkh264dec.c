/* GStreamer Intel MSDK plugin
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGDECE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "gstmsdkh264dec.h"

GST_DEBUG_CATEGORY_EXTERN (gst_msdkh264dec_debug);
#define GST_CAT_DEFAULT gst_msdkh264dec_debug

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-h264, "
        "width = (int) [ 1, MAX ], height = (int) [ 1, MAX ], "
        "stream-format = (string) byte-stream , alignment = (string) au , "
        "profile = (string) { high, main, baseline, constrained-baseline }")
    );

#define gst_msdkh264dec_parent_class parent_class
G_DEFINE_TYPE (GstMsdkH264Dec, gst_msdkh264dec, GST_TYPE_MSDKDEC);

static gboolean
gst_msdkh264dec_configure (GstMsdkDec * decoder)
{
  decoder->param.mfx.CodecId = MFX_CODEC_AVC;
  return TRUE;
}

static void
gst_msdkh264dec_class_init (GstMsdkH264DecClass * klass)
{
  GstElementClass *element_class;
  GstMsdkDecClass *decoder_class;

  element_class = GST_ELEMENT_CLASS (klass);
  decoder_class = GST_MSDKDEC_CLASS (klass);

  decoder_class->configure = GST_DEBUG_FUNCPTR (gst_msdkh264dec_configure);

  gst_element_class_set_static_metadata (element_class,
      "Intel MSDK H264 decoder",
      "Codec/Decoder/Video",
      "H264 video decoder based on Intel Media SDK",
      "Scott D Phillips <scott.d.phillips@intel.com>");

  gst_element_class_add_static_pad_template (element_class, &sink_factory);
}

static void
gst_msdkh264dec_init (GstMsdkH264Dec * thiz)
{
}
