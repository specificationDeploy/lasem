/* lsmdomnode.c
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmdomnode.h>
#include <lsmdomdocument.h>
#include <lsmdebug.h>
#include <glib/gprintf.h>
#include <stdio.h>

static GObjectClass *parent_class = NULL;

/* LsmDomNode implementation */

const char*
lsm_dom_node_get_node_name (LsmDomNode* self)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_val_if_fail (node_class != NULL, NULL);

	if (node_class->get_node_name)
		return node_class->get_node_name (self);

	return NULL;
}

const char*
lsm_dom_node_get_node_value (LsmDomNode* self)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_val_if_fail (node_class != NULL, NULL);

	if (node_class->get_node_value)
		return node_class->get_node_value (self);

	return NULL;
}

void
lsm_dom_node_set_node_value (LsmDomNode* self, const char* new_value)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_if_fail (node_class != NULL);
	g_return_if_fail (new_value != NULL);

	if (node_class->set_node_value)
		node_class->set_node_value (self, new_value);
}

LsmDomNodeType lsm_dom_node_get_node_type (LsmDomNode* self)
{
	LsmDomNodeClass *node_class = LSM_DOM_NODE_GET_CLASS (self);

	g_return_val_if_fail (node_class != NULL, 0);

	if (node_class->get_node_type)
		return node_class->get_node_type (self);

	return 0;
}

LsmDomNode*
lsm_dom_node_get_parent_node (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->parent_node;
}

/*LsmDomNodeList**/
/*lsm_dom_node_get_child_nodes (LsmDomNode* self)*/
/*{*/
/*        return LSM_DOM_NODE_GET_CLASS (self)->get_child_nodes (self);*/
/*}*/

LsmDomNode*
lsm_dom_node_get_first_child (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->first_child;
}

LsmDomNode*
lsm_dom_node_get_last_child (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->last_child;
}

LsmDomNode*
lsm_dom_node_get_previous_sibling (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->previous_sibling;
}

LsmDomNode*
lsm_dom_node_get_next_sibling (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	return self->next_sibling;
}

/*LsmDomNamedNodeMap**/
/*lsm_dom_node_get_attributes (LsmDomNode* self)*/
/*{*/
/*        return LSM_DOM_NODE_GET_CLASS (self)->get_attributes (self);*/
/*}*/

LsmDomDocument*
lsm_dom_node_get_owner_document (LsmDomNode* self)
{
	LsmDomNode *parent;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);

	for (parent = self;
	     parent != NULL &&
	     !LSM_IS_DOM_DOCUMENT (parent);
	     parent = parent->parent_node);

	return LSM_DOM_DOCUMENT (parent);
}

LsmDomNode*
lsm_dom_node_insert_before (LsmDomNode* self, LsmDomNode* new_child, LsmDomNode* ref_child)
{
	g_warning ("Not yet implemented");

	return NULL;
}

LsmDomNode*
lsm_dom_node_replace_child (LsmDomNode* self, LsmDomNode* new_child, LsmDomNode* old_child)
{
	g_warning ("Not yet implemented");

	return NULL;
}

LsmDomNode*
lsm_dom_node_remove_child (LsmDomNode* self, LsmDomNode* old_child)
{
	LsmDomNode *node;
	LsmDomNodeClass *node_class;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);
	g_return_val_if_fail (LSM_IS_DOM_NODE (old_child), NULL);

	for (node = self->first_child;
	     node != NULL && node != old_child;
	     node = node->next_sibling);

	g_return_val_if_fail (node != NULL, NULL);

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->pre_remove_child)
		node_class->pre_remove_child (self, old_child);

	if (self->first_child == old_child)
		self->first_child = old_child->next_sibling;
	if (self->last_child == old_child)
		self->last_child = old_child->previous_sibling;

	if (old_child->next_sibling != NULL)
		old_child->next_sibling->previous_sibling = old_child->previous_sibling;
	if (old_child->previous_sibling != NULL)
		old_child->previous_sibling->next_sibling = old_child->next_sibling;

	old_child->parent_node = NULL;
	old_child->next_sibling = NULL;
	old_child->previous_sibling = NULL;

	lsm_dom_node_changed (self);

	return old_child;
}

LsmDomNode *
lsm_dom_node_append_child (LsmDomNode* self, LsmDomNode* new_child)
{
	LsmDomNodeClass *node_class;

	g_return_val_if_fail (LSM_IS_DOM_NODE (self), NULL);
	g_return_val_if_fail (new_child->parent_node == NULL, NULL);

	if (!LSM_DOM_NODE_GET_CLASS (self)->can_append_child (self, new_child)) {
		lsm_debug ("dom", "[LsmDomNode::append_child] Can't append '%s' to '%s'",
			   lsm_dom_node_get_node_name (new_child),
			   lsm_dom_node_get_node_name (self));
		return NULL;
	}

	if (self->first_child == NULL)
		self->first_child = new_child;
	if (self->last_child != NULL)
		self->last_child->next_sibling = new_child;

	new_child->parent_node = self;
	new_child->next_sibling = NULL;
	new_child->previous_sibling = self->last_child;
	self->last_child = new_child;

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->post_new_child)
		node_class->post_new_child (self, new_child);

	lsm_dom_node_changed (self);

	return new_child;
}

static gboolean
lsm_dom_node_can_append_child (LsmDomNode *self, LsmDomNode* new_child)
{
	return FALSE;
}

void
lsm_dom_node_changed (LsmDomNode *self)
{
	LsmDomNode *parent_node;
	LsmDomNode *child_node;
	LsmDomNodeClass *node_class;

	g_return_if_fail (LSM_IS_DOM_NODE (self));

	node_class = LSM_DOM_NODE_GET_CLASS (self);

	if (node_class->changed)
		node_class->changed (self);

	child_node = self;
	for (parent_node = self->parent_node;
	       parent_node != NULL;
	       parent_node = parent_node->parent_node) {
		node_class = LSM_DOM_NODE_GET_CLASS (parent_node);
		if (node_class->child_changed == NULL ||
		    !node_class->child_changed (parent_node, child_node))
			break;
		child_node = parent_node;
	}
}

gboolean
lsm_dom_node_has_child_nodes (LsmDomNode* self)
{
	g_return_val_if_fail (LSM_IS_DOM_NODE (self), FALSE);

	return self->first_child != NULL;
}

void
lsm_dom_node_dump (LsmDomNode *self)
{
	LsmDomNode *node;
	LsmDomNodeType type;
	const char *text;

	g_return_if_fail (LSM_IS_DOM_NODE (self));

	type = lsm_dom_node_get_node_type (self);

	switch (type) {
		case LSM_DOM_NODE_TYPE_ELEMENT_NODE:
			g_printf ("<%s>", lsm_dom_node_get_node_name (self));
			for (node = self->first_child;
			     node != NULL;
			     node = node->next_sibling)
				lsm_dom_node_dump (node);
			g_printf ("</%s>", lsm_dom_node_get_node_name (self));
			break;
		case LSM_DOM_NODE_TYPE_TEXT_NODE:
			text = lsm_dom_node_get_node_value (self);
			g_printf ("%s", text != NULL ? text : "null");
			break;
		case LSM_DOM_NODE_TYPE_DOCUMENT_NODE:
			g_printf ("Mathml Document\n");
			if (self->first_child != NULL) {
				lsm_dom_node_dump (self->first_child);
				g_printf ("\n");
			}
			break;
		default:
			g_printf ("Not supported\n");
			break;
	}
}

static void
lsm_dom_node_init (LsmDomNode *node)
{
}

static void
lsm_dom_node_finalize (GObject *object)
{
	LsmDomNode *node = LSM_DOM_NODE (object);
	LsmDomNode *child, *next_child;

	child = node->first_child;
	while (child != NULL) {
		next_child = child->next_sibling;
		g_object_unref (child);
		child = next_child;
	}

	parent_class->finalize (object);
}

/* LsmDomNode class */

static void
lsm_dom_node_class_init (LsmDomNodeClass *node_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (node_class);

	parent_class = g_type_class_peek_parent (node_class);

	object_class->finalize = lsm_dom_node_finalize;

	node_class->can_append_child = lsm_dom_node_can_append_child;
}

G_DEFINE_ABSTRACT_TYPE (LsmDomNode, lsm_dom_node, G_TYPE_OBJECT)
