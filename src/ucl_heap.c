/*
   Part of: Useless Containers Library
   Contents: heap implementation
   Date: Fri Feb 28, 2003

   Abstract



   Copyright (c) 2003-2005, 2008-2010 Marco Maggi <marcomaggi@gna.org>

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
 ** Header files and type declarations.
 ** ----------------------------------------------------------*/

#include "internal.h"

typedef ucl_node_tag_t		node_t;
typedef ucl_value_t		value_t;
typedef ucl_heap_t		heap_t;

#if (DEBUGGING == 1)
#  include <stdio.h>

static void	printnode	(node_t *node);
static void	printlevel	(node_t *root);
static void	assertnode	(node_t *node);
#endif

#define SWAP(aptr, bptr)	tmp=(aptr);(aptr)=(bptr);(bptr)=tmp


void
ucl_heap_insert (ucl_heap_t this, void * _node)
{
    if (! this->root) {
	this->root	= this->next = (ucl_node_t)_node;
	this->size	= 1;
	this->state	= 0;
	return;
    } else {
	ucl_node_t	root = this->root;
	ucl_node_t	node = _node;
	ucl_node_t	next;
	ucl_node_t	dad;
	ucl_node_t	tmp;
	ucl_value_t	a, b;
	int		first;
	next = this->next;
	node->son = node->bro = NULL;
	if (this->state)
	    next->bro = node;
	else
	    next->son = node;
	dad = node->dad = next;
	first = 2;
	while (this->compar.func(this->compar.data, dad, node) > 0) {
	    switch (first) {
	    case 2:
		next = node;
		--first;
		break;
	    case 1:
		next = dad;
		--first;
		break;
	    }

	    node->dad = dad->dad;
	    dad->dad    = node;
	    if (node->dad) {
		if (node->dad->son == dad)
		    node->dad->son = node;
		else
		    node->dad->bro = node;
	    }

	    if (dad->son == node) {
		/*
		  |                        |
		  ----                    ----
		  |dad |                  |link|
		  ----                    ----
		  /    \     ->           /    \
		  ----                     ----
		  |link|                   |dad |
		  ----                     ----
		*/

		dad->son = node->son;
		node->son = dad;

		SWAP(dad->bro, node->bro);
		if (node->bro)
		    node->bro->dad = node;
	    } else { /* dad->bro == node */
		/*
		  |                         |
		  ----                      ----
		  |dad |                    |link|
		  ----                      ----
		  /    \         ->         /    \
		  ----                     ----
		  |link|                   |dad |
		  ----                     ----
		*/

		dad->bro    = node->bro;
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
	    a.pointer = dad;
	    b.pointer = node;
	}

	if (this->state || next->bro)
	    next = (void *) ucl_btree_step_levelorder((void *) next);

	assert(! root->dad);
	assert(next);

	this->state	= !(this->state);
	this->root	= root;
	this->next	= next;
	++(this->size);

#if (DEBUGGING == 1)
	printnode(root);
	printnode(next);
	printlevel(root);
#endif
    }
}


ucl_node_t
ucl_heap_extract (ucl_heap_t this)
{
    assert(this != NULL);
    if (this->root == NULL) {
	return NULL;
    } else if (this->size == 1) {
	ucl_node_t	tmp = this->root;
	this->root	= this->next = NULL;
	this->size	= 0;
	return tmp;
    } else {
	ucl_node_t		link = this->root;
	ucl_node_t		tmp = NULL;
	ucl_node_t		dad;
	int			first = 1, v, fromleft=-1;
	ucl_node_tag_t	links, links1;
	links = *link;
	while (link) {
	    if (links.son && links.bro) {
		v = (this->compar.func(this->compar.data, links.son, links.bro) <= 0)? 1 : 0;
	    } else if ((!links.son) && (!links.bro)) {
		/*
		  if (son(link) == link)
		  {
		  son(link) = NULL;
		  }
		  else
		  {
		  bro(link) = NULL;
		  }
		*/
#if (DEBUGGING == 1)
		printf("both NULLs\n");fflush(0);
		printlevel(this->root);
#endif

		if (this->root) {
		    this->next	= (void *) \
			ucl_btree_find_leftmost((void *) this->root);
		    this->state	= 0;
		} else {
		    this->next = NULL;
		}

#if (DEBUGGING == 1)
		printf("next %p ", this->next);
		printnode(this->next);fflush(0);
#endif

		--(this->size);
		goto end;
	    } else if (links.son) {
		v = 1;
	    } else {
		v = 0;
	    }

	    dad = link;
	    if (first) {
		tmp	= dad;
		link->dad = dad = NULL;
	    }

	    if (v) {
		link = links.son;
		if (first) {
		    this->root	= link;
		    first	= 0;
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
		    this->root = link;
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
  ucl_node_t 	node;

  while ((node = ucl_heap_extract(other)))
    ucl_heap_insert(this, node);
}


/** ------------------------------------------------------------
 ** Debug functions.
 ** ----------------------------------------------------------*/

#if (DEBUGGING == 1)
void
printlevel (node_t *root)
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
printnode (node_t *node)
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
assertnode (node_t *node)
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
