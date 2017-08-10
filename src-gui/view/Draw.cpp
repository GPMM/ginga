/* Copyright (C) 2006-2017 PUC-Rio/Laboratorio TeleMidia

This file is part of Ginga (Ginga-NCL).

Ginga is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Ginga is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Ginga.  If not, see <http://www.gnu.org/licenses/>.  */

#include "ginga_gtk.h"
#include <cairo.h>

#if GTK_CHECK_VERSION(3,8,0)
gboolean
update_draw_callback (GtkWidget *widget, GdkFrameClock *frame_clock,
               G_GNUC_UNUSED gpointer data)
#else
gboolean
update_draw_callback (GtkWidget *widget)
#endif
{
  guint64 time;
  static guint64 frame = (guint64) -1;
  static guint64 last;
  static guint64 first;

#if GTK_CHECK_VERSION(3,8,0)
  time = (guint64)(gdk_frame_clock_get_frame_time (frame_clock) * 1000);
  frame = (guint64) gdk_frame_clock_get_frame_counter (frame_clock);
#else
  time = ginga_gettime ();
  frame++;
#endif

  if (frame == 0)
    {
      first = time;
      last = time;
    }
  GINGA->send_tick (time - first, time - last, frame);
  last = time;
  gtk_widget_queue_draw (widget);
  return G_SOURCE_CONTINUE;
}

void
draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{

  int w, h;
  w = gtk_widget_get_allocated_width (widget);
  h = gtk_widget_get_allocated_height (widget);

  cairo_set_source_rgb (cr, 1., 0., 1.);
  cairo_rectangle (cr, 0, 0, w, h);
  cairo_fill (cr);

  GINGA->redraw (cr);
}