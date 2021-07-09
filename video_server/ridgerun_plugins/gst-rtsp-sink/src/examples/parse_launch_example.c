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
#include <gst/gst.h>
#include <glib-unix.h>

#define MAPPING "/ridgerun"
#define SERVICE "12345"

gboolean
signal_handler (gpointer user_data)
{
  GMainLoop * loop = (GMainLoop *)user_data;

  g_print ("Interrupt received, closing...\n");
  g_main_loop_quit (loop);

  return TRUE;
}

int main (gint argc, gchar *argv[])
{
  GstElement * pipeline;
  GMainLoop * loop;
  gint ret = -1;
  const gchar * desc;
  GError *error = NULL;
  
  gst_init (&argc, &argv);

  desc = "videotestsrc is-live=true ! x264enc speed-preset=ultrafast key-int-max=10 ! "
    "video/x-h264,mapping=" MAPPING " ! rtspsink service=" SERVICE;
  pipeline = gst_parse_launch_full (desc, NULL, GST_PARSE_FLAG_FATAL_ERRORS, &error);
  if (!pipeline || error) {
    g_printerr ("Unable to build pipeline: %s", error->message ? error->message : "(no debug)");
    goto out;
  }

  /* Playing the pipeline */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  g_print ("New RTSP stream started at rtsp://127.0.0.1:" SERVICE MAPPING "\n");
  
  /* Block until CTRL+C is pressed */
  loop = g_main_loop_new (NULL, TRUE);
  g_unix_signal_add (SIGINT, signal_handler, loop);
  g_main_loop_run (loop);
  g_main_loop_unref (loop);
  
  /* NULL the pipeline */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  
  /* Successful run */
  g_print ("Successfully closed\n");
  ret = 0;

 out:
  gst_deinit ();
  
  return ret;
}
