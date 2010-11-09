/*
   Part of: Useless Containers Library
   Contents: binary tree functions
   Date: Fri Oct 11, 2002

   Abstract

	The btree container is built as a chain of structures; each link
	has a brother, a son and a parent.

                 -----    bro     -----
	        | no1 |--------->| no2 |
	         ----- <--------- -----
	         |  ^     dad
             son |  | dad
                 v  |
                 -----
                | no3 |
                 -----


   Copyright (c) 2002-2005, 2008-2010 Marco Maggi <marcomaggi@gna.org>

   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.

   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.

   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#define DEBUGGING		0
#include "internal.h"


void *
ucl_btree_find_value (void * node, void * value,
		      ucl_comparison_t compar)
{
  ucl_node_t	N = node;
  int		v;
  while (NULL != node) {
    v = compar.func(compar.data, value, N);
    if (v > 0)
      N = N->bro;
    else if (v < 0)
      N = N->son;
    else
      return N;
  }
  return NULL;
}


void *
ucl_btree_find_leftmost (void * node)
{
  ucl_node_t	N = node;
  while (N->son)
    N = N->son;
  return N;
}
void *
ucl_btree_find_rightmost (void * node)
{
  ucl_node_t	N = node;
  while (N->bro)
    N = N->bro;
  return N;
}
void *
ucl_btree_find_deepest_son (void * node)
{
  ucl_node_t	N = node;
  do {
    while (N->son) N = N->son;
    if    (N->bro) N = N->bro;
  } while (N->son || N->bro);
  return N;
}
void *
ucl_btree_find_deepest_bro (void * node)
{
  ucl_node_t	N = node;
  do {
    while (N->bro) N = N->bro;
    if    (N->son) N = N->son;
  } while (N->bro || N->son);
  return N;
}
void *
ucl_btree_find_root (void * node)
{
  ucl_node_t	N = node;
  while (N->dad)
    N = N->dad;
  return N;
}


void *
ucl_btree_step_inorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->bro) {
    N = N->bro;
    return ucl_btree_find_leftmost(N);
  } else {
    dad = N->dad;
    while (dad && (dad->bro == N)) {
      N		= dad;
      dad	= N->dad;
    }
    return dad;
  }
}
void *
ucl_btree_step_inorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->son) {
    N = N->son;
    return ucl_btree_find_rightmost(N);
  } else {
    dad = N->dad;
    while (dad && (dad->son == N)) {
      N		= dad;
      dad	= N->dad;
    }
    return dad;
  }
}


void *
ucl_btree_step_preorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->son)
    return N->son;
  else if (N->bro)
    return N->bro;
  else {
    for (;;) {
      dad = N->dad;
      if (! dad)
	break;
      else if ((dad->son == N) && (dad->bro))
	return dad->bro;
      else
	N = dad;
    }
  }
  return dad;
}
void *
ucl_btree_step_preorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->bro)
    return N->bro;
  else if (N->son)
    return N->son;
  else {
    for (;;) {
      dad = N->dad;
      if (! dad)
	break;
      else if ((dad->bro == N) && (dad->son))
	return dad->son;
      else
	N = dad;
    }
  }
  return dad;
}


void *
ucl_btree_step_postorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  dad = N->dad;
  if (! dad)
    return dad;
  else if ((dad->bro) && (dad->bro != N))
    return ucl_btree_find_deepest_son(dad->bro);
  else
    return dad;
}
void *
ucl_btree_step_postorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  dad = N->dad;
  if (! dad)
    return dad;
  else if ((dad->son) && (dad->son != N))
    return ucl_btree_find_deepest_bro(dad->son);
  else
    return dad;
}


void *
ucl_btree_step_levelorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	org_p;
  ucl_node_t	last_p;
  int		i=0;
  if ((! N->dad) && (! N->son) && (! N->bro))
    return NULL;
  org_p = N;
  for (;;) {
    if (N->dad) {
      last_p = N;
      N = N->dad;
      ++i;
      if (N->bro && N->bro != last_p) {
	last_p = N;
	N = N->bro;
	--i;
	if (i == 0)
	  return N;
	if (N == org_p && !N->son && !N->bro)
	  return NULL;
	while (N->son || N->bro) {
	  if (N->son) {
	    last_p = N;
	    N = N->son;
	  } else if (N->bro) {
	    last_p = N;
	    N = N->bro;
	  }
	  --i;
	  if (i == 0)
	    return N;
	  if (N == org_p && !N->son && !N->bro)
	    return NULL;
	}
      }
    } else {
      ++i;
      while (N->son || N->bro) {
	if (N->son) {
	  last_p = N;
	  N = N->son;
	} else if (N->bro) {
	  last_p = N;
	  N = N->bro;
	}
	--i;
	if (i == 0)
	  return N;
	if ((N == org_p) && (!N->son) && (!N->bro))
	  return NULL;
      }
    }
  }
  return N;
}


void *
ucl_btree_step_levelorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	org_p;
  ucl_node_t	last_p;
  int		i=0;
  if ((! N->dad) && (! N->bro) && (! N->son))
    return NULL;
  org_p = N;
  for (;;) {
    if (N->dad) {
      last_p = N;
      N = N->dad;
      ++i;
      if (N->son && N->son != last_p) {
	last_p = N;
	N = N->son;
	--i;
	if (i == 0)
	  return N;
	if (N == org_p && !N->bro && !N->son)
	  return NULL;
	while (N->bro || N->son) {
	  if (N->bro) {
	    last_p = N;
	    N = N->bro;
	  } else if (N->son) {
	    last_p = N;
	    N = N->son;
	  }
	  --i;
	  if (i == 0)
	    return N;
	  if (N == org_p && !N->bro && !N->son)
	    return NULL;
	}
      }
    } else {
      ++i;
      while (N->bro || N->son) {
	if (N->bro) {
	  last_p = N;
	  N = N->bro;
	} else if (N->son) {
	  last_p = N;
	  N = N->son;
	}
	--i;
	if (i == 0)
	  return N;
	if ((N == org_p) && (!N->bro) && (!N->son))
	  return NULL;
      }
    }
  }
  return N;
}


static void __attribute__((__nonnull__))
ucl_btree_generic_iteration_next (ucl_iterator_t iterator)
{
  iterator->iterator = iterator->internal2.pointer_map_function(iterator->iterator);
}

/* ------------------------------------------------------------ */

void
ucl_btree_iterator_inorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_inorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder;
}
void
ucl_btree_iterator_inorder_backward (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_inorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_iterator_preorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_preorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder;
}
void
ucl_btree_iterator_preorder_backward (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_preorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder_backward;
}

/* ------------------------------------------------------------ */

__attribute__((__nonnull__)) void
ucl_btree_iterator_postorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_postorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder;
}
__attribute__((__nonnull__)) void
ucl_btree_iterator_postorder_backward (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_postorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_iterator_levelorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_levelorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_levelorder;
}
__attribute__((__nonnull__)) void
ucl_btree_iterator_levelorder_backward (ucl_iterator_t iterator,
					void * root_node)
{
  iterator->iterator	= ucl_btree_first_levelorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_levelorder_backward;
}


static void __attribute__((__nonnull__,__hot__))
ucl_btree_subtree_generic_next (ucl_iterator_t iterator)
{
  iterator->iterator = (iterator->iterator == iterator->internal1.pointer)?
    NULL : iterator->internal2.pointer_map_function(iterator->iterator);
}

/* ------------------------------------------------------------ */

void
ucl_btree_subtree_iterator_inorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_inorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_rightmost(node);	/* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder;
}
void
ucl_btree_subtree_iterator_inorder_backward (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_inorder_backward(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_leftmost(node);	/* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_subtree_iterator_preorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_preorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_deepest_bro(node); /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder;
}
void
ucl_btree_subtree_iterator_preorder_backward (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_preorder_backward(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_deepest_son(node); /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_subtree_iterator_postorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_postorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = node; /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder;
}
void
ucl_btree_subtree_iterator_postorder_backward (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_postorder_backward(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = node; /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder_backward;
}

/* ------------------------------------------------------------ */

#if 0
void
ucl_btree_subtree_iterator_levelorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_levelorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_rightmost(node);	/* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_levelorder;
}
void
ucl_btree_subtree_iterator_levelorder_backward (ucl_iterator_t iterator, void * node)
{
}
#endif

/* end of file */
