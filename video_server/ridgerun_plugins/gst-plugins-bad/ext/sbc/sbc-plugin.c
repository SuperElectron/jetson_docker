/* GStreamer SBC audio plugin
 * Copyright (C) 2013 Tim-Philipp Müller <tim@centricular.net>
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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gstsbcdec.h"
#include "gstsbcenc.h"

#include <string.h>

static gboolean
plugin_init (GstPlugin * plugin)
{
  gst_element_register (plugin, "sbcdec", GST_RANK_PRIMARY, GST_TYPE_SBC_DEC);
  gst_element_register (plugin, "sbcenc", GST_RANK_NONE, GST_TYPE_SBC_ENC);
  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    sbc,
    "SBC bluetooth audio support",
    plugin_init, VERSION, "LGPL", GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN);
