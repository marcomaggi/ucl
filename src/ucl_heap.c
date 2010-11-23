/*
  Part of: Useless Containers Library
  Contents: heap implementation
  Date: Fri Feb 28, 2003

  Abstract



  Copyright (c) 2003-2005, 2008-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this  program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** ------------------------------------------------------------
 ** Header files and type declarations.
 ** ----------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#include "internal.h"

#if (DEBUGGING == 1)
#  include <stdio.h>

static void	printnode	(ucl_node_t node);
static void	printlevel	(ucl_node_t root);
static void	assertnode	(ucl_node_t node);
#endif

#define SWAP(aptr, bptr)	tmp=(aptr);(aptr)=(bptr);(bptr)=tmp


void
ucl_heap_initialise (ucl_heap_t H, ucl_comparison_t compar)
{
  ucl_struct_clean(H, ucl_heap_tag_t);
  H->compar = compar;
}

void
ucl_heap_insert (ucl_heap_t H, void * _node)
{
  ucl_node_t	node = _node;
  assert(H);
  if (! H->root) {
    H->root	= H->next = node;
    H->size	= 1;
    H->state	= 0;
    return;
  } else {
    ucl_node_t	root = H->root, next, dad, tmp;
    ucl_value_t	a, b;
    int		first;
    next = H->next;
    node->son = node->bro = NULL;
    if (H->state)
      next->bro = node;
    else
      next->son = node;
    dad = node->dad = next;
    a.pointer = dad;
    b.pointer = node;
    for (first = 2; H->compar.func(H->compar.data, a, b) > 0; --first) {
      switch (first) {
      case 2:
	next = node;
	break;
      case 1:
	next = dad;
	break;
      }

      node->dad	= dad->dad;
      dad->dad	= node;
      if (node->dad) {
	if (node->dad->son == dad)
	  node->dad->son = node;
	else
	  node->dad->bro = node;
      }

      if (dad->son == node) {
	/*
	 *
	 *      |                      |
	 *     ----                  ----
	 *    |dad |                |link|
	 *     ----                  ----
	 *    /    \     ->         /    \
	 *  ----                 ----
	 * |link|               |dad |
	 *  ----                 ----
	 */

	dad->son = node->son;
	node->son = dad;

	SWAP(dad->bro, node->bro);
	if (node->bro)
	  node->bro->dad = node;
      } else { /* dad->bro == node */
	/*
	 *      |                       |
	 *     ----                    ----
	 *    |dad |                  |link|
	 *     ----                    ----
	 *    /    \       ->         /    \
	 *  ----                   ----
	 * |link|                 |dad |
	 *  ----                   ----
	 */
	dad->bro  = node->bro;
	node->bro = dad;
	SWAP(dad->son, node->son);
	if (node->son)
	  node->son->dad = node;
      }
      if (dad->son)
	dad->son->dad = dad;
      if (dad->dad)
	dad->bro->dad = dad;
      if (dad == root) {
	root = node;
	assert(root->dad == NULL);
	break;
      }
      dad = node->dad;
      /* a.pointer = dad; */
      /* b.pointer = node; */
    }

    if (H->state || next->bro)
      next = (void *) ucl_btree_step_levelorder((void *) next);

    assert(! root->dad);
    assert(next);

    H->state	= !(H->state);
    H->root	= root;
    H->next	= next;
    ++(H->size);

#if (DEBUGGING == 1)
    printnode(root);
    printnode(next);
    printlevel(root);
#endif
  }
}

void *
ucl_heap_extract (ucl_heap_t H)
{
  assert(H);
  if (H->root == NULL) {
    return NULL;
  } else if (H->size == 1) {
    ucl_node_t	tmp = H->root;
    H->root	= H->next = NULL;
    H->size	= 0;
    return tmp;
  } else {
    ucl_node_tag_t	links, links1;
    ucl_node_t		link = H->root, tmp = NULL, dad;
    int			first = 1, v, fromleft=-1;
    links = *link;
    while (link) {
      /* This "if ... else ..."  block establishes a value for "v": true
	 if the son is lesser than the bro. */
      {
	if (links.son && links.bro) {
	  ucl_value_t a = { .pointer = links.son };
	  ucl_value_t b = { .pointer = links.bro };
	  v = (H->compar.func(H->compar.data, a, b) <= 0)? 1 : 0;
	} else if (!links.son && !links.bro) {
#if (DEBUGGING == 1)
	  printf("both NULLs\n");fflush(0);
	  printlevel(H->root);
#endif
	  if (H->root) {
	    H->next	= (void *) ucl_btree_find_leftmost(H->root);
	    H->state	= 0;
	  } else {
	    H->next	= NULL;
	  }
#if (DEBUGGING == 1)
	  printf("next %p ", (void *)H->next);
	  printnode(H->next);fflush(0);
#endif
	  --(H->size);
	  goto end;
	} else if (links.son) { /* NULL == links.bro */
	  v = 1;
	} else { /* NULL == links.son && NULL != links.bro */
	  v = 0;
	}
      }

      dad = link;
      if (first) {
	tmp       = dad;
	link->dad = dad = NULL;
      }

      if (v) {
	link = links.son;
	if (first) {
	  H->root = link;
	  first	  = 0;
	}
	if (link) {
	  links1  = *link;
	  *link   = links;
	  link->son   = NULL;
	  if (links.bro)
	    links.bro->dad = link;
	  if (dad) {
	    if (fromleft == 1)
	      dad->son = link;
	    else
	      dad->bro = link;
	  }
	  link->dad = dad;
	  fromleft = 1;
	}
      } else {
	link = links.bro;
	if (first) {
	  H->root = link;
	  first = 0;
	}
	if (link) {
	  links1  = *link;
	  *link = links;
	  link->bro = NULL;
	  if (links.son)
	    links.son->dad = link;
	  if (dad) {
	    if (fromleft == 1)
	      dad->son = link;
	    else
	      dad->bro = link;
	  }
	  link->dad = dad;
	  fromleft = 0;
	}
      }
      links = links1;
    }
  end:
    return tmp;
  }
}


void
ucl_heap_merge (ucl_heap_t this, ucl_heap_t other)
{
  ucl_node_t 	N;
  while ((N = ucl_heap_extract(other)))
    ucl_heap_insert(this, N);
}


/** ------------------------------------------------------------
 ** Debug functions.
 ** ----------------------------------------------------------*/

#if (DEBUGGING == 1)
void
printlevel (ucl_node_t root)
{
  printf("level:");
  while (root)
    {
      printf(" %d", root->val.integer);
      fflush(0);
      root = (void *) ucl_btree_step_levelorder((void *) root);
    }
  printf("\n");
  fflush(0);
}

void
printnode (ucl_node_t node)
{
  assert(node);

  printf("node %d:", node->val.integer);
  printf(" dad=%d", (node->dad)? node->dad->val.integer : -1);
  printf(" son=%d", (node->son)? node->son->val.integer : -1);
  printf(" bro=%d", (node->bro)? node->bro->val.integer : -1);
  printf("\n");fflush(0);

  if (node->dad)
    {
      assert((son(node->dad) == node)
	     || (bro(node->dad) == node));
    }
  if (node->son)
    {
      assert(dad(node->son) == node);
    }
  if (node->bro)
    {
      assert(dad(node->bro) == node);
    }
}

void
assertnode (ucl_node_t node)
{
  if (!node) return;

  if (node->dad)
    {
      assert((son(node->dad) == node)
	     || (bro(node->dad) == node));
    }
  if (node->son)
    {
      assert(dad(node->son) == node);
    }
  if (node->bro)
    {
      assert(dad(node->bro) == node);
    }
}
#endif

/* end of file */
