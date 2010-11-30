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

  Copyright (c) 2002-2005, 2008-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#include "internal.h"


void *
ucl_btree_find_value (void * root, ucl_value_t value, ucl_comparison_t compar)
{
  ucl_node_t	N  = root;
  ucl_value_t	D;
  int		v;
  while (NULL != root) {
    D.pointer = N;
    v = compar.func(compar.data, value, D);
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

void
ucl_btree_swap_out (void * A_, void * B_)
/* Given two  pointers to links, interpret A  as pointer to a  node in a
   tree and B as pointer to a  node out of any tree; store the links and
   meta value of A into B, then resets the links and meta value of A. */
{
  ucl_node_t		A = A_, B = B_;
  B->dad	= A->dad;
  B->son	= A->son;
  B->bro	= A->bro;
  B->meta	= A->meta;
  A->dad	= NULL;
  A->son	= NULL;
  A->bro	= NULL;
  A->meta	= ucl_value_null;
}
void
ucl_btree_swap (void * A_, void * B_)
/* Given two pointers to links, swap the links and the meta value in the
   node structures. */
{
  ucl_node_t		A = A_, B = B_;
  ucl_node_t		A_dad, A_son, A_bro;
  ucl_node_t		B_dad, B_son, B_bro;
  ucl_value_t		A_meta;

  A_dad		= A->dad;
  A_son		= A->son;
  A_bro		= A->bro;
  A_meta	= A->meta;

  B_dad		= B->dad;
  B_son		= B->son;
  B_bro		= B->bro;

  A->dad	= (B_dad == A)? B : B_dad;
  A->son	= (B_son == A)? B : B_son;
  A->bro	= (B_bro == A)? B : B_bro;
  A->meta = B->meta;
  if (A_dad && A_dad != B) {
    if (A_dad->son == A)
      A_dad->son = B;
    else
      A_dad->bro = B;
  }
  if (A_son && A_son != B) A_son->dad = B;
  if (A_bro && A_bro != B) A_bro->dad = B;

  B->dad	= (A_dad == B)? A : A_dad;
  B->son	= (A_son == B)? A : A_son;
  B->bro	= (A_bro == B)? A : A_bro;
  B->meta	= A_meta;
  if (B_dad && B_dad != A) {
    if (B_dad->son == B)
      B_dad->son = A;
    else
      B_dad->bro = A;
  }
  if (B_son && B_son != A) B_son->dad = A;
  if (B_bro && B_bro != A) B_bro->dad = A;
}

void *
ucl_btree_avl_rot_left (void * old_cur_)
/* Perform a  left rotation  which balances a  left-left-higher subtree.
 * Example:
 *
 *               (top)                      (top)
 *                 |                          |
 *                11 (old_cur)                9 (new_cur)
 *                / \                    -----+-----
 *     (new_cur) 9   12                 7           11 (old_cur)
 *              / \             =>     / \         /  \
 *             7   10 (bro)           6   8      10   12
 *            / \                              (bro)
 *           6   8
 *
 */
{
  ucl_node_t	old_cur = old_cur_, new_cur, bro;
  new_cur	= old_cur->son;
  bro		= new_cur->bro;
  assert(UCL_SON_DEEPER == old_cur->meta.avl_status);
  assert(UCL_SON_DEEPER == new_cur->meta.avl_status);
  { /* relink "old_cur" and "new_cur" */
    new_cur->bro	= old_cur;
    new_cur->dad	= old_cur->dad;
    old_cur->dad	= new_cur;
  }
  { /* relink "old_cur" and "bro" */
    old_cur->son	= bro;
    if (bro) bro->dad	= old_cur;
  }
  { /* relink "top" */
    ucl_node_t	top = new_cur->dad;
    if (top) {
      if (old_cur == top->bro)
	top->bro = new_cur;
      else {
	assert(old_cur == top->son);
	top->son = new_cur;
      }
    }
  }
  old_cur->meta.avl_status = UCL_EQUAL_DEPTH;
  new_cur->meta.avl_status = UCL_EQUAL_DEPTH;
  return new_cur;
}

void *
ucl_btree_avl_rot_right (void * old_cur_)
/* Perform a right rotation which balances a right-right-higher subtree.
 * Example:
 *
 *         (top)                              (top)
 *           |                                  |
 *           7 (old_cur)                        9 (new_cur)
 *          / \                            -----+-----
 *         6   9 (new_cur)   => (old_cur) 7           11
 *            / \                        / \         /  \
 *     (son) 8   11                     6   8      10    12
 *              /  \                      (son)
 *            10    12
 */
{
  ucl_node_t	old_cur = old_cur_, new_cur, son;
  new_cur	= old_cur->bro;
  son		= new_cur->son;
  assert(UCL_BRO_DEEPER == old_cur->meta.avl_status);
  assert(UCL_BRO_DEEPER == new_cur->meta.avl_status);
  { /* relink "old_cur" and "new_cur" */
    new_cur->son	= old_cur;
    new_cur->dad	= old_cur->dad;
    old_cur->dad	= new_cur;
  }
  { /* relink "old_cur" and "son" */
    old_cur->bro	= son;
    if (son) son->dad	= old_cur;
  }
  { /* relink "top" */
    ucl_node_t	top = new_cur->dad;
    if (top) {
      if (old_cur == top->bro)
	top->bro = new_cur;
      else {
	assert(old_cur == top->son);
	top->son = new_cur;
      }
    }
  }
  old_cur->meta.avl_status = UCL_EQUAL_DEPTH;
  new_cur->meta.avl_status = UCL_EQUAL_DEPTH;
  return new_cur;
}

void *
ucl_btree_avl_rot_left_right (void * old_cur_)
/* Perform  a  left/right rotation  which  balances a  left-right-higher
 * subtree.  Example:
 *
 *             (top)                             (top)
 *               |                                 |
 *              10 (old_cur)                       8 (new_cur)
 *             /  \                      ----------+----------
 *      (son) 5    13             (son) 5                    10 (old_cur)
 *           / \               =>      / \                  /  \
 *          3   8 (new_cur)           3   7                9    13
 *             / \                      (deep_son)  (deep_bro)
 * (deep_son) 7   9 (deep_bro)
 *
 *We  perform this  rotation to  balance a  tree in  which  "old_cur" is
 *son-deeper and "son" is bro-deeper.
 */
{
  ucl_node_t	old_cur = old_cur_, new_cur, son, deep_bro, deep_son;
  son		= old_cur->son;
  new_cur	= son->bro;
  assert(UCL_SON_DEEPER == old_cur->meta.avl_status);
  assert(UCL_BRO_DEEPER == son->meta.avl_status || UCL_EQUAL_DEPTH == son->meta.avl_status);
  { /* relink "son" and "deep_son" */
    deep_son = new_cur->son;
    son->bro	= deep_son;
    if (deep_son) deep_son->dad = son;
  }
  { /* relink "new_cur" and "son" */
    new_cur->son= son;
    son->dad	= new_cur;
  }
  { /* relink "old_cur" and "deep_bro" */
    deep_bro = new_cur->bro;
    old_cur->son = deep_bro;
    if (deep_bro) deep_bro->dad = old_cur;
  }
  { /* finish relinking "old_cur" and "new_cur" */
    new_cur->bro	= old_cur;
    new_cur->dad	= old_cur->dad;
    old_cur->dad	= new_cur;
  }
  { /* relink "top" */
    ucl_node_t	top = new_cur->dad;
    if (top) {
      if (old_cur == top->bro)
	top->bro = new_cur;
      else {
	assert(old_cur == top->son);
	top->son = new_cur;
      }
    }
  }
  /* Adjust the statuses. */
  if (deep_son) {
    son->meta.avl_status = son->son? UCL_EQUAL_DEPTH : UCL_BRO_DEEPER;
  } else {
    son->meta.avl_status = son->son? UCL_SON_DEEPER  : UCL_EQUAL_DEPTH;
  }
  if (deep_bro) {
    old_cur->meta.avl_status = old_cur->bro? UCL_EQUAL_DEPTH : UCL_SON_DEEPER;
  } else {
    old_cur->meta.avl_status = old_cur->bro? UCL_BRO_DEEPER  : UCL_EQUAL_DEPTH;
  }
  new_cur->meta.avl_status = UCL_EQUAL_DEPTH;
  return new_cur;
}

void *
ucl_btree_avl_rot_right_left (void * old_cur_)
/* Perform a  bro/son rotation which balances  a bro-son-higher subtree.
 * Example:
 *
 *             (top)                                (top)
 *               |                                    |
 *     (old_cur) 9                                   11 (new_cur)
 *              / \                         ----------+---------
 *             8   13 (bro)      (old_cur) 9                    13 (bro)
 *                /  \         =>         / \                  /  \
 *    (new_cur) 11    14                 8   10              12    14
 *             /  \                        (deep_son)  (deep_bro)
 *           10    12
 *   (deep_son)    (deep_bro)
 *
 *We  perform this  rotation to  balance a  tree in  which  "old_cur" is
 *bro-deeper and  "bro" is son-deeper.
 */
{
  ucl_node_t	old_cur = old_cur_, new_cur, bro, deep_bro, deep_son;
  bro		= old_cur->bro;
  new_cur	= bro->son;
  assert(UCL_BRO_DEEPER == old_cur->meta.avl_status);
  assert(UCL_SON_DEEPER == bro->meta.avl_status);
  { /* relink "bro" and "deep_bro" */
    deep_bro = new_cur->bro;
    bro->son	= deep_bro;
    if (deep_bro) deep_bro->dad = bro;
  }
  { /* relink "new_cur" and "bro" */
    new_cur->bro= bro;
    bro->dad	= new_cur;
  }
  { /* relink "old_cur" and "deep_son" */
    deep_son = new_cur->son;
    old_cur->bro = deep_son;
    if (deep_son) deep_son->dad = old_cur;
  }
  { /* finish relinking "old_cur" and "new_cur" */
    new_cur->son	= old_cur;
    new_cur->dad	= old_cur->dad;
    old_cur->dad	= new_cur;
  }
  { /* relink "top" */
    ucl_node_t	top = new_cur->dad;
    if (top) {
      if (old_cur == top->bro)
	top->bro = new_cur;
      else {
	assert(old_cur == top->son);
	top->son = new_cur;
      }
    }
  }
  /* Adjust the statuses. */
  if (deep_son) {
    old_cur->meta.avl_status = old_cur->son? UCL_EQUAL_DEPTH : UCL_BRO_DEEPER;
  } else {
    old_cur->meta.avl_status = old_cur->son? UCL_SON_DEEPER  : UCL_EQUAL_DEPTH;
  }
  if (deep_bro) {
    bro->meta.avl_status	= bro->bro? UCL_EQUAL_DEPTH : UCL_SON_DEEPER;
  } else {
    bro->meta.avl_status	= bro->bro? UCL_BRO_DEEPER  : UCL_EQUAL_DEPTH;
  }
  new_cur->meta.avl_status = UCL_EQUAL_DEPTH;
  return new_cur;
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
