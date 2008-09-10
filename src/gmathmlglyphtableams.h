/* gmathmlglyphtableams.h
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#ifndef GMATHML_GLYPH_TABLE_AMS_H
#define GMATHML_GLYPH_TABLE_AMS_H

#include <gmathml.h>
#include <gmathmlview.h>

G_BEGIN_DECLS

#define GMATHML_RADICAL_UTF8 "\xe2\x88\x9a"

extern const GMathmlOperatorGlyph AMS_table[];

const GMathmlOperatorGlyph *gmathml_glyph_table_find_operator_glyph (const char *text);

G_END_DECLS

#endif