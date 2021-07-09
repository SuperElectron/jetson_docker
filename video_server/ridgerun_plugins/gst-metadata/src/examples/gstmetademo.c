/* Copyright (C) <year> RidgeRun, LLC (http://www.ridgerun.com)
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

#include <gst/gst.h>
#include <glib-unix.h>
#include <stdlib.h>
#include <string.h>

#define PUSH_META_METHOD 0 //SIGNAL
//#define PUSH_META_METHOD 1 //BINARY PROPERTY
//#define PUSH_META_METHOD 2 //STRING PROPERTY

typedef struct _GstMetaDemo GstMetaDemo;
struct _GstMetaDemo
{
  GstElement *pipeline;
  GstElement *metasrc;
  GstElement *overlay;
  GMainLoop *main_loop;
  guint timeout_id;
};

GstMetaDemo *gst_meta_demo_new (void);
void gst_meta_demo_free (GstMetaDemo * metademo);
void gst_meta_demo_create_pipeline (GstMetaDemo * metademo,
    const gchar * output);
void gst_meta_demo_start (GstMetaDemo * metademo);
void gst_meta_demo_stop (GstMetaDemo * metademo);

static gboolean gst_meta_demo_handle_message (GstBus * bus,
    GstMessage * message, gpointer data);
static gboolean gst_meta_demo_handle_timeout (gpointer priv);
static void gst_meta_demo_handle_paused_to_playing (GstMetaDemo * metademo);
static void gst_meta_demo_parse_args (int *argc, char **argv[]);
static void gst_meta_demo_install_probe (GstMetaDemo * metademo,
    GstElement * pipeline);
static GstPadProbeReturn gst_meta_demo_handle_buffer (GstPad * pad,
    GstPadProbeInfo * info, gpointer user_data);
static gboolean gst_meta_demo_handle_signal (gpointer data);

gint length;
gchar *output;

static GOptionEntry entries[] = {
  {"length", 'l', 0, G_OPTION_ARG_INT, &length,
      "The length of the video in seconds (defaults to 10s)", "S"},
  {"output", 'o', 0, G_OPTION_ARG_FILENAME, &output,
      "The location of the output file (defaults to ./meta.ts)", "F"},
  {NULL}
};

int
main (int argc, char *argv[])
{
  GstMetaDemo *metademo;
  GMainLoop *main_loop;

  //Defaults
  length = 10;
  output = g_strdup ("./meta.ts");

  gst_meta_demo_parse_args (&argc, &argv);

  metademo = gst_meta_demo_new ();

  main_loop = g_main_loop_new (NULL, TRUE);
  metademo->main_loop = main_loop;

  g_unix_signal_add (SIGINT, (GSourceFunc) gst_meta_demo_handle_signal,
      metademo);

  gst_meta_demo_create_pipeline (metademo, output);

  g_print ("Recording...\n");
  gst_meta_demo_start (metademo);

  g_main_loop_run (main_loop);

  g_print ("Stopping...\n");
  gst_meta_demo_stop (metademo);

  gst_meta_demo_free (metademo);
  g_free (output);

  exit (EXIT_SUCCESS);
}

static void
gst_meta_demo_parse_args (int *argc, char **argv[])
{
  GOptionContext *context;
  GError *error = NULL;

  context = g_option_context_new ("- GstMetaDemo");
  g_option_context_add_main_entries (context, entries, "meta");
  g_option_context_add_group (context, gst_init_get_option_group ());
  if (!g_option_context_parse (context, argc, argv, &error)) {
    g_printerr ("option parsing failed: %s\n", error->message);
    g_clear_error (&error);
    g_option_context_free (context);
    exit (EXIT_FAILURE);
  }
  g_option_context_free (context);
}

GstMetaDemo *
gst_meta_demo_new (void)
{
  GstMetaDemo *metademo;

  metademo = g_new0 (GstMetaDemo, 1);

  metademo->pipeline = NULL;
  metademo->metasrc = NULL;
  metademo->overlay = NULL;
  metademo->main_loop = NULL;
  metademo->timeout_id = 0;

  return metademo;
}

void
gst_meta_demo_free (GstMetaDemo * metademo)
{
  if (metademo->pipeline) {
    gst_element_set_state (metademo->pipeline, GST_STATE_NULL);
    gst_object_unref (metademo->pipeline);
    metademo->pipeline = NULL;
  }

  if (metademo->metasrc) {
    gst_object_unref (metademo->metasrc);
    metademo->metasrc = NULL;
  }

  if (metademo->overlay) {
    gst_object_unref (metademo->overlay);
    metademo->overlay = NULL;
  }

  if (metademo->main_loop) {
    g_main_loop_unref (metademo->main_loop);
    metademo->main_loop = NULL;
  }

  if (metademo->timeout_id) {
    g_source_remove (metademo->timeout_id);
    metademo->timeout_id = 0;
  }

  g_free (metademo);
}

void
gst_meta_demo_create_pipeline (GstMetaDemo * metademo, const char *output)
{
  GstElement *pipeline;
  GError *error = NULL;
  GstBus *bus;
  const gchar *desc_templ = "metasrc name=src ! meta/x-klv ! "
      "mpegtsmux name=mux ! filesink location=%s videotestsrc "
      "is-live=true name=vts ! video/x-raw,width=1024,height=768,framerate=60/1 ! "
      "textoverlay name=overlay shaded-background=true ! x264enc ! mux. "
      "audiotestsrc is-live=true ! lamemp3enc ! audio/mpeg,mpegversion=1,parsed=true ! mux. ";

  gchar *desc = g_strdup_printf (desc_templ, output);

  pipeline = gst_parse_launch (desc, &error);
  if (error) {
    g_printerr ("pipeline parsing error: %s\n", error->message);
    g_error_free (error);
    return;
  }

  gst_meta_demo_install_probe (metademo, pipeline);

  metademo->metasrc = gst_bin_get_by_name (GST_BIN (pipeline), "src");
  metademo->overlay = gst_bin_get_by_name (GST_BIN (pipeline), "overlay");
  metademo->pipeline = pipeline;

  gst_pipeline_set_auto_flush_bus (GST_PIPELINE (pipeline), FALSE);
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_bus_add_watch (bus, gst_meta_demo_handle_message, metademo);
  gst_object_unref (bus);
}

static void
gst_meta_demo_install_probe (GstMetaDemo * metademo, GstElement * pipeline)
{
  GstElement *videotestsrc;
  GstPad *pad;

  videotestsrc = gst_bin_get_by_name (GST_BIN (pipeline), "vts");
  pad = gst_element_get_static_pad (videotestsrc, "src");

  gst_pad_add_probe (pad, GST_PAD_PROBE_TYPE_BUFFER,
      gst_meta_demo_handle_buffer, metademo, NULL);

  gst_object_unref (pad);
  gst_object_unref (videotestsrc);
}

static GstPadProbeReturn
gst_meta_demo_handle_buffer (GstPad * pad, GstPadProbeInfo * info,
    gpointer user_data)
{
  GstMetaDemo *metademo = (GstMetaDemo *) user_data;
  GstClockTime ts;
  guint64 offset;
  GstBuffer *buffer;
  gchar *meta;
  gsize metalen;
  
  buffer = GST_BUFFER (info->data);
  ts = GST_BUFFER_DTS_OR_PTS (buffer);
  offset = GST_BUFFER_OFFSET (buffer);

  meta = g_strdup_printf ("Buffer %04lu - %" GST_TIME_FORMAT "\n", offset,
      GST_TIME_ARGS (ts));
  metalen = strlen (meta);

  /* Metadata can be set via the action signal or via
     the "metadata-binary" property. The action receives a plain
     uchar pointer and a size, whilst the property needs a boxed
     GBytesArray
  */
#if PUSH_META_METHOD == 0
  GstFlowReturn ret;
  g_signal_emit_by_name (metademo->metasrc, "push-metadata",
      meta, metalen, &ret);

#elif PUSH_META_METHOD == 1

  GByteArray *barray;
  barray = g_byte_array_sized_new (metalen);
  barray->len = metalen;
  memcpy (barray->data, meta, metalen);
  g_object_set (metademo->metasrc, "metadata-binary", barray, NULL);
  g_boxed_free (G_TYPE_BYTE_ARRAY, barray);

#else

  g_object_set (metademo->metasrc, "metadata", meta, NULL);

#endif

  /* Render the same text on the video buffer */
  g_object_set (metademo->overlay, "text", meta, NULL);

  g_free (meta);


  return GST_PAD_PROBE_OK;
}

void
gst_meta_demo_start (GstMetaDemo * metademo)
{
  gst_element_set_state (metademo->pipeline, GST_STATE_PLAYING);
}

void
gst_meta_demo_stop (GstMetaDemo * metademo)
{
  gst_element_set_state (metademo->pipeline, GST_STATE_NULL);
}

static void
gst_meta_demo_handle_eos (GstMetaDemo * metademo)
{
  g_main_loop_quit (metademo->main_loop);
}

static void
gst_meta_demo_handle_error (GstMetaDemo * metademo, GError * error,
    const char *debug)
{
  g_print ("error: %s\n", error->message);
  gst_meta_demo_stop (metademo);
}

static void
gst_meta_demo_handle_warning (GstMetaDemo * metademo, GError * error,
    const char *debug)
{
  g_print ("warning: %s\n", error->message);
}

static void
gst_meta_demo_handle_info (GstMetaDemo * metademo, GError * error,
    const char *debug)
{
  g_print ("info: %s\n", error->message);
}

static gboolean
gst_meta_demo_handle_message (GstBus * bus, GstMessage * message, gpointer data)
{
  GstMetaDemo *metademo = (GstMetaDemo *) data;

  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS:
      gst_meta_demo_handle_eos (metademo);
      break;
    case GST_MESSAGE_ERROR:
    {
      GError *error = NULL;
      gchar *debug;

      gst_message_parse_error (message, &error, &debug);
      gst_meta_demo_handle_error (metademo, error, debug);
      g_clear_error (&error);
    }
      break;
    case GST_MESSAGE_WARNING:
    {
      GError *error = NULL;
      gchar *debug;

      gst_message_parse_warning (message, &error, &debug);
      gst_meta_demo_handle_warning (metademo, error, debug);
      g_clear_error (&error);
    }
      break;
    case GST_MESSAGE_INFO:
    {
      GError *error = NULL;
      gchar *debug;

      gst_message_parse_info (message, &error, &debug);
      gst_meta_demo_handle_info (metademo, error, debug);
      g_clear_error (&error);
    }
      break;
    case GST_MESSAGE_STATE_CHANGED:
    {
      GstState oldstate, newstate, pending;
      GstStateChange transition;

      gst_message_parse_state_changed (message, &oldstate, &newstate, &pending);
      if (GST_ELEMENT (message->src) == metademo->pipeline) {
        transition = GST_STATE_TRANSITION (oldstate, newstate);
        if (GST_STATE_CHANGE_PAUSED_TO_PLAYING == transition) {
          gst_meta_demo_handle_paused_to_playing (metademo);
        }
      }
    }
      break;
    default:
      break;
  }

  return TRUE;
}

static void
gst_meta_demo_handle_paused_to_playing (GstMetaDemo * metademo)
{
  metademo->timeout_id =
      g_timeout_add_seconds (length, gst_meta_demo_handle_timeout, metademo);
}

static gboolean
gst_meta_demo_handle_timeout (gpointer priv)
{
  GstMetaDemo *metademo = (GstMetaDemo *) priv;

  g_main_loop_quit (metademo->main_loop);
  metademo->timeout_id = 0;

  /* We dont want this handler to be called again */
  return FALSE;
}

static gboolean
gst_meta_demo_handle_signal (gpointer data)
{
  GstMetaDemo *metademo = (GstMetaDemo *) data;

  g_main_loop_quit (metademo->main_loop);

  return TRUE;
}
