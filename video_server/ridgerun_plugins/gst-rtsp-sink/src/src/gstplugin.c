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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gstrtspsink.h>

static gboolean
plugin_init (GstPlugin * plugin)
{
  gst_element_register (plugin, "rtspsink", GST_RANK_NONE, GST_TYPE_RTSP_SINK);

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR, rtspsink,
    "RTSP sink plugin", plugin_init, VERSION,
    "Proprietary", "Ridgerun elements", "http://www.ridgerun.com")
