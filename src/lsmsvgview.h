/*
 * Copyright © 2009 Emmanuel Pacaud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_SVG_VIEW_H
#define LSM_SVG_VIEW_H

#include <lsmdomview.h>
#include <lsmsvgelement.h>
#include <lsmsvg.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define LSM_TYPE_SVG_VIEW             (lsm_svg_view_get_type ())
#define LSM_SVG_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LSM_TYPE_SVG_VIEW, LsmSvgView))
#define LSM_SVG_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LSM_TYPE_SVG_VIEW, LsmSvgViewClass))
#define LSM_IS_SVG_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LSM_TYPE_SVG_VIEW))
#define LSM_IS_SVG_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LSM_TYPE_SVG_VIEW))
#define LSM_SVG_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), LSM_TYPE_SVG_VIEW, LsmSvgViewClass))

typedef struct _LsmSvgViewClass LsmSvgViewClass;
typedef struct _LsmSvgViewPrivate LsmSvgViewPrivate;

typedef struct _LsmSvgViewPatternData LsmSvgViewPatternData;

struct _LsmSvgView {
	LsmDomView dom_view;

	double resolution_ppi;

	GSList *viewbox_stack;
	GSList *fill_stack;
	GSList *stroke_stack;
	GSList *opacity_stack;
	GSList *text_stack;
	GSList *matrix_stack;

	LsmSvgViewPatternData *pattern_data;

	GSList *pattern_stack;

	gboolean is_clipping;
	LsmBox clip_extents;
};

struct _LsmSvgViewClass {
	LsmDomViewClass parent_class;
};

GType lsm_svg_view_get_type (void);

LsmSvgView *	lsm_svg_view_new 			(LsmSvgDocument *document);

const LsmBox *	lsm_svg_view_get_pattern_extents	(LsmSvgView *view);
const LsmBox *	lsm_svg_view_get_clip_extents		(LsmSvgView *view);

void 		lsm_svg_view_create_radial_gradient 	(LsmSvgView *view, double cx, double cy,
							                   double r, double fx, double fy);
void 		lsm_svg_view_create_linear_gradient 	(LsmSvgView *view, double x1, double y1,
							                   double x2, double y2);
void 		lsm_svg_view_add_gradient_color_stop	(LsmSvgView *view, double offset,
							 const LsmSvgColor *color, double opacity);
void 		lsm_svg_view_set_gradient_properties	(LsmSvgView *view,
							 LsmSvgSpreadMethod method,
							 LsmSvgPatternUnits units,
							 const LsmSvgMatrix *matrix);

void		lsm_svg_view_create_surface_pattern	(LsmSvgView *view, const LsmBox *viewport,
							 LsmSvgPatternUnits units,
							 LsmSvgPatternUnits content_units,
							 const LsmSvgMatrix *matrix);

double 		lsm_svg_view_normalize_length 		(LsmSvgView *view, const LsmSvgLength *length,
							 LsmSvgLengthDirection direction);
void 		lsm_svg_view_push_viewbox 		(LsmSvgView *view, const LsmBox *viewbox);
void 		lsm_svg_view_pop_viewbox 		(LsmSvgView *view);
void 		lsm_svg_view_push_viewport 		(LsmSvgView *view, const LsmBox *viewport,
							 const LsmBox *viewbox,
							 const LsmSvgPreserveAspectRatio *aspect_ratio);
void 		lsm_svg_view_pop_viewport 		(LsmSvgView *view);
void 		lsm_svg_view_push_matrix		(LsmSvgView *view, LsmSvgMatrix *matrix);
void 		lsm_svg_view_pop_matrix			(LsmSvgView *view);
void 		lsm_svg_view_push_clip 			(LsmSvgView *view, char *clip_path,
							 LsmSvgFillRule clip_rule, const LsmExtents *extents);
void 		lsm_svg_view_pop_clip 			(LsmSvgView *view);
gboolean 	lsm_svg_view_is_clipping 		(LsmSvgView *view);
void 		lsm_svg_view_push_fill_attributes 	(LsmSvgView *view, LsmSvgFillAttributeBag *fill);
void 		lsm_svg_view_pop_fill_attributes 	(LsmSvgView *view);
void 		lsm_svg_view_push_stroke_attributes 	(LsmSvgView *view, LsmSvgStrokeAttributeBag *stroke);
void 		lsm_svg_view_pop_stroke_attributes 	(LsmSvgView *view);
void 		lsm_svg_view_push_text_attributes 	(LsmSvgView *view, LsmSvgTextAttributeBag *text);
void 		lsm_svg_view_pop_text_attributes 	(LsmSvgView *view);

void 		lsm_svg_view_show_rectangle 	(LsmSvgView *view, double x, double y,
						                   double width, double height,
								   double rx, double ry);
void 		lsm_svg_view_show_circle	(LsmSvgView *view, double cx, double cy, double r);
void 		lsm_svg_view_show_ellipse	(LsmSvgView *view, double cx, double cy, double rx, double ry);
void		lsm_svg_view_show_path		(LsmSvgView *view, const char *d);
void 		lsm_svg_view_show_line 		(LsmSvgView *view, double x1, double y1, double x2, double y2);
void 		lsm_svg_view_show_polyline	(LsmSvgView *view, const char *points);
void 		lsm_svg_view_show_polygon	(LsmSvgView *view, const char *points);
void 		lsm_svg_view_show_text 		(LsmSvgView *view, char const *text, double x, double y);
void		lsm_svg_view_show_pixbuf	(LsmSvgView *view, GdkPixbuf *pixbuf);

void 		lsm_svg_view_push_opacity 	(LsmSvgView *view, double opacity, gboolean late_opacity_handling);
void 		lsm_svg_view_pop_opacity 	(LsmSvgView *view);

G_END_DECLS

#endif
