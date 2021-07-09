/* 
 * GStreamer
 * Copyright (C) 2016 Sebastian Dröge <sebastian@centricular.com>
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
 
#ifndef __GST_AUDIO_BUFFER_SPLIT_H__
#define __GST_AUDIO_BUFFER_SPLIT_H__

#include <gst/gst.h>
#include <gst/base/base.h>
#include <gst/audio/audio.h>

G_BEGIN_DECLS

#define GST_TYPE_AUDIO_BUFFER_SPLIT            (gst_audio_buffer_split_get_type())
#define GST_AUDIO_BUFFER_SPLIT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AUDIO_BUFFER_SPLIT,GstAudioBufferSplit))
#define GST_IS_AUDIO_BUFFER_SPLIT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AUDIO_BUFFER_SPLIT))
#define GST_AUDIO_BUFFER_SPLIT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass) ,GST_TYPE_AUDIO_BUFFER_SPLIT,GstAudioBufferSplitClass))
#define GST_IS_AUDIO_BUFFER_SPLIT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass) ,GST_TYPE_AUDIO_BUFFER_SPLIT))
#define GST_AUDIO_BUFFER_SPLIT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj) ,GST_TYPE_AUDIO_BUFFER_SPLIT,GstAudioBufferSplitClass))

typedef struct _GstAudioBufferSplit      GstAudioBufferSplit;
typedef struct _GstAudioBufferSplitClass GstAudioBufferSplitClass;

struct _GstAudioBufferSplit {
  GstElement parent;

  GstPad *srcpad, *sinkpad;

  /* Properties */
  gint output_buffer_duration_n;
  gint output_buffer_duration_d;

  /* State */
  GstSegment segment;
  GstAudioInfo info;

  GstAdapter *adapter;

  GstAudioStreamAlign *stream_align;
  GstClockTime resync_time;
  guint64 current_offset; /* offset from start time in samples */

  guint samples_per_buffer;
  guint error_per_buffer;
  guint accumulated_error;

  gboolean strict_buffer_size;
};

struct _GstAudioBufferSplitClass {
  GstElementClass parent_class;
};

GType gst_audio_buffer_split_get_type (void);

G_END_DECLS

#endif /* __GST_AUDIO_BUFFER_SPLIT_H__ */
