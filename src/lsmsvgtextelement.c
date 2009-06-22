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

#include <lsmsvgtextelement.h>
#include <lsmsvgview.h>
#include <lsmdomtext.h>

static GObjectClass *parent_class;

/* GdomNode implementation */

static const char *
lsm_svg_text_element_get_node_name (LsmDomNode *node)
{
	return "text";
}

static gboolean
lsm_svg_text_element_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return (LSM_IS_DOM_TEXT (child));
}

/* LsmSvgElement implementation */

static void
lsm_svg_text_element_update (LsmSvgElement *self, LsmSvgStyle *parent_style)
{
	LsmSvgTextElement *text = LSM_SVG_TEXT_ELEMENT (self);
	LsmSvgLength length;

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&text->x, &length);

	length.value_unit = 0.0;
	length.type = LSM_SVG_LENGTH_TYPE_PX;
	lsm_svg_animated_length_attribute_parse (&text->y, &length);

	LSM_SVG_ELEMENT_CLASS (parent_class)->update (self, parent_style);
}

/* LsmSvgGraphic implementation */

static void
lsm_svg_text_element_graphic_render (LsmSvgElement *self, LsmSvgView *view)
{
	LsmSvgTextElement *text = LSM_SVG_TEXT_ELEMENT (self);
	LsmDomNode *node = LSM_DOM_NODE (self);
	LsmDomNode *iter;
	GString *string = g_string_new ("");
	double x, y;

	if (node->first_child == NULL)
		return;

	for (iter = LSM_DOM_NODE (self)->first_child; iter != NULL; iter = iter->next_sibling) {
		if (LSM_IS_DOM_TEXT (iter)) {
			g_string_append (string, lsm_dom_node_get_node_value (iter));
		}
	}

	x = lsm_svg_view_normalize_length (view, &text->x.length.base, LSM_SVG_LENGTH_DIRECTION_HORIZONTAL);
	y = lsm_svg_view_normalize_length (view, &text->y.length.base, LSM_SVG_LENGTH_DIRECTION_VERTICAL);

	lsm_svg_view_show_text (view, g_strstrip (string->str), x, y);

	g_string_free (string, TRUE);
}

/* LsmSvgTextElement implementation */

LsmDomNode *
lsm_svg_text_element_new (void)
{
	return g_object_new (LSM_TYPE_SVG_TEXT_ELEMENT, NULL);
}

static void
lsm_svg_text_element_init (LsmSvgTextElement *self)
{
}

static void
lsm_svg_text_element_finalize (GObject *object)
{
}

/* LsmSvgTextElement class */

static void
lsm_svg_text_element_class_init (LsmSvgTextElementClass *s_rect_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (s_rect_class);
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (s_rect_class);
	LsmSvgElementClass *s_element_class = LSM_SVG_ELEMENT_CLASS (s_rect_class);
	LsmSvgGraphicClass *s_graphic_class = LSM_SVG_GRAPHIC_CLASS (s_rect_class);

	parent_class = g_type_class_peek_parent (s_rect_class);

	object_class->finalize = lsm_svg_text_element_finalize;

	d_node_class->get_node_name = lsm_svg_text_element_get_node_name;
	d_node_class->can_append_child = lsm_svg_text_element_can_append_child;

	s_element_class->update = lsm_svg_text_element_update;

	s_graphic_class->graphic_render = lsm_svg_text_element_graphic_render;
	s_graphic_class->late_opacity_handling = TRUE;

	s_element_class->attributes = lsm_dom_attribute_map_duplicate (s_element_class->attributes);

	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "x",
					  offsetof (LsmSvgTextElement, x));
	lsm_dom_attribute_map_add_attribute (s_element_class->attributes, "y",
					  offsetof (LsmSvgTextElement, y));
}

G_DEFINE_TYPE (LsmSvgTextElement, lsm_svg_text_element, LSM_TYPE_SVG_GRAPHIC)
