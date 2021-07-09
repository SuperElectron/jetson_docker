/* GStreamer Wayland video sink
 *
 * Copyright (C) 2014 Collabora Ltd.
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

/* GstWlBuffer wraps wl_buffer and provides a mechanism for preventing
 * buffers from being re-used while the compositor is using them. This
 * is achieved by adding a reference to the GstBuffer as soon as its
 * associated wl_buffer is sent to the compositor and by removing this
 * reference as soon as the compositor sends a wl_buffer::release message.
 *
 * This mechanism is a bit complicated, though, because it adds cyclic
 * references that can be dangerous. The reference cycles looks like:
 *
 *   ----------------
 *   | GstWlDisplay | ---------------------------->
 *   ----------------                              |
 *                                                 |
 *                                                 V
 *   -----------------     -------------     ---------------
 *   | GstBufferPool | --> | GstBuffer | ==> | GstWlBuffer |
 *   |               | <-- |           | <-- |             |
 *   -----------------     -------------     ---------------
 *
 * A GstBufferPool normally holds references to its GstBuffers and each buffer
 * holds a reference to a GstWlBuffer (saved in the GstMiniObject qdata).
 * When a GstBuffer is in use, it holds a reference back to the pool and the
 * pool doesn't hold a reference to the GstBuffer. When the GstBuffer is unrefed
 * externally, it returns back to the pool and the pool holds again a reference
 * to the buffer.
 *
 * Now when the compositor is using a buffer, the GstWlBuffer also holds a ref
 * to the GstBuffer, which prevents it from returning to the pool. When the
 * last GstWlBuffer receives a release event and unrefs the last GstBuffer,
 * the GstBufferPool will be able to stop and if no-one is holding a strong
 * ref to it, it will be destroyed. This will destroy the pool's GstBuffers and
 * also the GstWlBuffers. This will all happen in the same context of the last
 * gst_buffer_unref, which will be called from the buffer_release() callback.
 *
 * The problem here lies in the fact that buffer_release() will be called
 * from the event loop thread of GstWlDisplay, so it's as if the display
 * holds a reference to the GstWlBuffer, but without having an actual reference.
 * When we kill the display, there is no way for the GstWlBuffer, the associated
 * GstBuffer and the GstBufferPool to get destroyed, so we are going to leak a
 * fair ammount of memory.
 *
 * Normally, this rarely happens, because the compositor releases buffers
 * almost immediately and when waylandsink stops, they are already released.
 *
 * However, we want to be absolutely certain, so a solution is introduced
 * by registering all the GstWlBuffers with the display and explicitly
 * releasing all the buffer references as soon as the display is destroyed.
 *
 * When the GstWlDisplay is finalized, it takes a reference to all the
 * registered GstWlBuffers and then calls gst_wl_buffer_force_release_and_unref,
 * which releases the potential reference to the GstBuffer, destroys the
 * underlying wl_buffer and removes the reference that GstWlDisplay is holding.
 * At that point, either the GstBuffer is alive somewhere and still holds a ref
 * to the GstWlBuffer, which it will release when it gets destroyed, or the
 * GstBuffer was destroyed in the meantime and the GstWlBuffer gets destroyed
 * as soon as we remove the reference that GstWlDisplay holds.
 */

#include "wlbuffer.h"

GST_DEBUG_CATEGORY_EXTERN (gstwayland_debug);
#define GST_CAT_DEFAULT gstwayland_debug

G_DEFINE_TYPE (GstWlBuffer, gst_wl_buffer, G_TYPE_OBJECT);

static G_DEFINE_QUARK (GstWlBufferQDataQuark, gst_wl_buffer_qdata);

static void
gst_wl_buffer_dispose (GObject * gobject)
{
  GstWlBuffer *self = GST_WL_BUFFER (gobject);

  GST_TRACE_OBJECT (self, "dispose");

  /* if the display is shutting down and we are trying to dipose
   * the GstWlBuffer from another thread, unregister_buffer() will
   * block and in the end the display will increase the refcount
   * of this GstWlBuffer, so it will not be finalized */
  if (self->display)
    gst_wl_display_unregister_buffer (self->display, self);

  G_OBJECT_CLASS (gst_wl_buffer_parent_class)->dispose (gobject);
}

static void
gst_wl_buffer_finalize (GObject * gobject)
{
  GstWlBuffer *self = GST_WL_BUFFER (gobject);

  GST_TRACE_OBJECT (self, "finalize");

  if (self->wlbuffer)
    wl_buffer_destroy (self->wlbuffer);

  G_OBJECT_CLASS (gst_wl_buffer_parent_class)->finalize (gobject);
}

static void
gst_wl_buffer_class_init (GstWlBufferClass * klass)
{
  GObjectClass *object_class = (GObjectClass *) klass;

  object_class->dispose = gst_wl_buffer_dispose;
  object_class->finalize = gst_wl_buffer_finalize;
}

static void
gst_wl_buffer_init (GstWlBuffer * self)
{
}

static void
buffer_release (void *data, struct wl_buffer *wl_buffer)
{
  GstWlBuffer *self = data;

  GST_LOG_OBJECT (self, "wl_buffer::release (GstBuffer: %p)", self->gstbuffer);

  self->used_by_compositor = FALSE;

  /* unref should be last, because it may end up destroying the GstWlBuffer */
  gst_buffer_unref (self->gstbuffer);
}

static const struct wl_buffer_listener buffer_listener = {
  buffer_release
};

static void
gstbuffer_disposed (GstWlBuffer * self)
{
  g_assert (!self->used_by_compositor);
  self->gstbuffer = NULL;

  GST_TRACE_OBJECT (self, "owning GstBuffer was finalized");

  /* this will normally destroy the GstWlBuffer, unless the display is
   * finalizing and it has taken an additional reference to it */
  g_object_unref (self);
}

GstWlBuffer *
gst_buffer_add_wl_buffer (GstBuffer * gstbuffer, struct wl_buffer *wlbuffer,
    GstWlDisplay * display)
{
  GstWlBuffer *self;

  self = g_object_new (GST_TYPE_WL_BUFFER, NULL);
  self->gstbuffer = gstbuffer;
  self->wlbuffer = wlbuffer;
  self->display = display;

  gst_wl_display_register_buffer (self->display, self);

  wl_buffer_add_listener (self->wlbuffer, &buffer_listener, self);

  gst_mini_object_set_qdata ((GstMiniObject *) gstbuffer,
      gst_wl_buffer_qdata_quark (), self, (GDestroyNotify) gstbuffer_disposed);

  return self;
}

GstWlBuffer *
gst_buffer_get_wl_buffer (GstBuffer * gstbuffer)
{
  return gst_mini_object_get_qdata ((GstMiniObject *) gstbuffer,
      gst_wl_buffer_qdata_quark ());
}

void
gst_wl_buffer_force_release_and_unref (GstWlBuffer * self)
{
  /* Force a buffer release.
   * At this point, the GstWlDisplay has killed its event loop,
   * so we don't need to worry about buffer_release() being called
   * at the same time from the event loop thread */
  if (self->used_by_compositor) {
    GST_DEBUG_OBJECT (self, "forcing wl_buffer::release (GstBuffer: %p)",
        self->gstbuffer);
    self->used_by_compositor = FALSE;
    gst_buffer_unref (self->gstbuffer);
  }

  /* Finalize this GstWlBuffer early.
   * This method has been called as a result of the display shutting down,
   * so we need to stop using any wayland resources and disconnect from
   * the display. The GstWlBuffer stays alive, though, to avoid race
   * conditions with the GstBuffer being destroyed from another thread.
   * The last reference is either owned by the GstBuffer or by us and
   * it will be released at the end of this function. */
  GST_TRACE_OBJECT (self, "finalizing early");
  wl_buffer_destroy (self->wlbuffer);
  self->wlbuffer = NULL;
  self->display = NULL;

  /* remove the reference that the caller (GstWlDisplay) owns */
  g_object_unref (self);
}

void
gst_wl_buffer_attach (GstWlBuffer * self, struct wl_surface *surface)
{
  g_return_if_fail (self->used_by_compositor == FALSE);

  wl_surface_attach (surface, self->wlbuffer, 0, 0);

  /* Add a reference to the buffer. This represents the fact that
   * the compositor is using the buffer and it should not return
   * back to the pool and be re-used until the compositor releases it. */
  gst_buffer_ref (self->gstbuffer);
  self->used_by_compositor = TRUE;
}
