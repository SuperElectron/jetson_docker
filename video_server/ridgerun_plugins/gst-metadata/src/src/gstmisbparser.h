/* GStreamer 
 * Copyright (C) 2019 RidgeRun Engineering  Edison Fernandez <edison.fernandez@ridgerun.com> 
 * 
 * gstmisbparser.h:
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

#ifndef _GST_MISB_PARSER_H_
#define _GST_MISB_PARSER_H_

#include <gst/gst.h>
#include <gst/base/gstbaseparse.h>

G_BEGIN_DECLS
#define GST_TYPE_MISB_PARSER   (gst_misb_parser_get_type())
#define GST_MISB_PARSER(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MISB_PARSER,GstMisbParser))
#define GST_MISB_PARSER_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MISB_PARSER,GstMisbParserClass))
#define GST_IS_MISB_PARSER(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MISB_PARSER))
#define GST_IS_MISB_PARSER_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MISB_PARSER))
typedef struct _GstMisbParser GstMisbParser;
typedef struct _GstMisbParserClass GstMisbParserClass;

GType gst_misb_parser_get_type (void);

G_END_DECLS
#endif
