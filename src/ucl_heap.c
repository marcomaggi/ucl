/*    
   Part of: Useless Container Library
   Contents: heap implementation
   Date: Fri Feb 28, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
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

#define stubmodule		heap

typedef ucl_node_tag_t		node_t;
typedef ucl_value_t		value_t;
typedef ucl_heap_t		heap_t;

#if (DEBUGGING == 1)
#  include <stdio.h>

static void	printnode	(node_t *node_p);
static void	printlevel	(node_t *root_p);
static void	assertnode	(node_t *node_p);
#endif

#define SWAP(aptr, bptr)	tmp_p=(aptr);(aptr)=(bptr);(bptr)=tmp_p

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Insertion.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-11-04) void
ucl_heap_insert (ucl_heap_t this, void * _node_p)
{
  ucl_node_t	node_p = _node_p;
  ucl_node_t	root_p;
  ucl_node_t	next_p;
  ucl_node_t	dad;
  ucl_node_t	tmp_p;
  ucl_value_t	a, b;
  int		first;


  root_p = this->root_p;
  if (! root_p)
    {
      this->root_p	= this->next_p = node_p;
      this->size	= 1;
      this->state	= 0;
      return;
    }

  next_p = this->next_p;
  node_p->son = node_p->bro = NULL;
  if (this->state)
    next_p->bro = node_p;
  else 
    next_p->son = node_p;
  dad = node_p->dad = next_p;

  a.pointer = dad;
  b.pointer = node_p;
  first = 2;
  while (this->compar.func(this->compar.data, a, b) > 0)
    {
      switch (first)
	{
	case 2:
	  next_p = node_p;
	  --first;
	  break;
	case 1:
	  next_p = dad;
	  --first;
	  break;
	}

      node_p->dad = dad->dad;
      dad->dad    = node_p;
      if (node_p->dad)
	{
	  if (node_p->dad->son == dad)
	    node_p->dad->son = node_p;
	  else
	    node_p->dad->bro = node_p;
	}

      if (dad->son == node_p)
	{
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

	  dad->son = node_p->son;
	  node_p->son = dad;
	  
	  SWAP(dad->bro, node_p->bro);
	  if (node_p->bro)
	    {
	      node_p->bro->dad = node_p;
	    }
	}
      else /* dad->bro == node_p */
	{
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
	  
	  dad->bro    = node_p->bro;
	  node_p->bro = dad;

	  SWAP(dad->son, node_p->son);
	  if (node_p->son)
	    {
	      node_p->son->dad = node_p;
	    }
	}

      if (dad->son)
	{
	  dad->son->dad = dad;
	}
      if (dad->dad)
	{
	  dad->bro->dad = dad;
	}
      
      if (dad == root_p)
	{
	  root_p = node_p;
	  assert(root_p->dad == NULL);
	  break;
	}

      dad = node_p->dad;
      a.pointer = dad;
      b.pointer = node_p;
    }

  if (this->state || next_p->bro)
    {
      next_p = (void *) ucl_btree_step_levelorder((void *) next_p);
    }

  assert(! root_p->dad);
  assert(next_p);

  this->state	= !(this->state);
  this->root_p	= root_p;
  this->next_p = next_p;
  ++(this->size);

#if (DEBUGGING == 1)
  printnode(root_p);
  printnode(next_p);
  printlevel(root_p);
#endif
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Removal.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-11-06) ucl_node_t 
ucl_heap_extract (ucl_heap_t this)
{
  ucl_node_t	link_p;
  ucl_node_t	tmp_p;
  ucl_node_t	dad;
  value_t	a, b;
  int		first, v, fromleft=-1;
  ucl_node_tag_t	links, links1;


  assert(this != NULL);

  if (this->root_p == NULL)
    {
      return NULL;
    }
  if (this->size == 1)
    {
      tmp_p		= this->root_p;
      this->root_p	= this->next_p = NULL;
      this->size	= 0;
      return tmp_p;
    }

  tmp_p	= NULL;

  link_p	= this->root_p;
  first		= 1;

#if (DEBUGGING == 1)
  printf("---------------------\n");
  printlevel(this->root_p);
  printf("root ");
  printnode(this->root_p);
#endif

  links = *link_p;
  while (link_p)
    {
      if (links.son && links.bro)
	{
	  a.pointer = links.son;
	  b.pointer = links.bro;
	  v = (this->compar.func(this->compar.data, a, b) <= 0)? 1 : 0;
	}
      else if ((!links.son) && (!links.bro))
	{
	  /*
	  if (son(link_p) == link_p)
	    {
	      son(link_p) = NULL;
	    }
	  else
	    {
	      bro(link_p) = NULL;
	    }
	  */
#if (DEBUGGING == 1)
	  printf("both NULLs\n");fflush(0);
  	  printlevel(this->root_p);
#endif

	  if (this->root_p)
	    {
	      this->next_p	= (void *) \
		ucl_btree_find_leftmost((void *) this->root_p);
	      this->state	= 0;
	    }
	  else
	    {
	      this->next_p = NULL;
	    }

#if (DEBUGGING == 1)
	  printf("next %p ", this->next_p);
  	  printnode(this->next_p);fflush(0);
#endif

	  --(this->size);
	  goto End;
	}
      else if (links.son)
	{
	  v = 1;
	}
      else
	{
	  v = 0;
	}

      dad = link_p;
      if (first)
	{
	  tmp_p	= dad;
#if (DEBUGGING == 1)
	  printf("extracting %d\n", tmp_p->val.integer);fflush(0);
#endif
	  link_p->dad = dad = NULL;
	}
      
      if (v)
	{
#if (DEBUGGING == 1)
	  printf("going left\n");fflush(0);
#endif
	  link_p = links.son;
	  if (first)
	    {
	      this->root_p = link_p;
	      first	    = 0;
	    }
	  
	  if (link_p)
	    {
	      links1  = *link_p;
	      *link_p = links;
	      link_p->son   = NULL;
	      if (links.bro)
		{
		  links.bro->dad = link_p;
		}
	      if (dad)
		{
		  if (fromleft == 1)
		    dad->son = link_p;
		  else
		    dad->bro = link_p;
		}
	      link_p->dad = dad;
	      fromleft = 1;

#if (DEBUGGING == 1)
	      assertnode(dad);

	      printf("new link\ndad is ");
	      if (dad) { printnode(dad); } else { printf("NULL\n"); }
	      printnode(link_p);
#endif
	    }
	}
      else
	{
#if (DEBUGGING == 1)
	  printf("going right\n");fflush(0);
#endif
	  link_p = links.bro;
	  if (first)
	    {
	      this->root_p = link_p;
	      first	    = 0;
	    }
	  
	  if (link_p)
	    {
	      links1  = *link_p;
	      *link_p = links;
	      link_p->bro = NULL;
	      if (links.son)
		links.son->dad = link_p;
	      if (dad)
		{
		  if (fromleft == 1)
		    dad->son = link_p;
		  else
		    dad->bro = link_p;
		}
	      link_p->dad = dad;
	      fromleft = 0;
	      
#if (DEBUGGING == 1)
	      assertnode(dad);

	      printf("new link\ndad is ");
	      if (dad) { printnode(dad); } else { printf("NULL\n"); }
	      printnode(link_p);
#endif
	    }
	}
      
      links = links1;
    }

 End:
  return tmp_p;
}


/** ------------------------------------------------------------
 ** Merging.
 ** ----------------------------------------------------------*/

stub(2007-09-09-09-35-41) void
ucl_heap_merge (ucl_heap_t this, ucl_heap_t other)
{
  ucl_node_t 	node_p;

  while ((node_p = ucl_heap_extract(other)))
    ucl_heap_insert(this, node_p);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Debug functions.
 ** ----------------------------------------------------------*/

#if (DEBUGGING == 1)
void
printlevel (node_t *root_p)
{
  printf("level:");
  while (root_p)
    {
      printf(" %d", root_p->val.integer);
      fflush(0);
      root_p = (void *) ucl_btree_step_levelorder((void *) root_p);
    }
  printf("\n");
  fflush(0);
}

void
printnode (node_t *node_p)
{
  assert(node_p);

  printf("node %d:", node_p->val.integer);
  printf(" dad=%d", (node_p->dad)? node_p->dad->val.integer : -1);
  printf(" son=%d", (node_p->son)? node_p->son->val.integer : -1);
  printf(" bro=%d", (node_p->bro)? node_p->bro->val.integer : -1);
  printf("\n");fflush(0);

  if (node_p->dad)
    {
      assert((son(node_p->dad) == node_p)
	     || (bro(node_p->dad) == node_p));
    }
  if (node_p->son)
    {
      assert(dad(node_p->son) == node_p);
    }
  if (node_p->bro)
    {
      assert(dad(node_p->bro) == node_p);
    }
}

void
assertnode (node_t *node_p)
{
  if (!node_p) return;

  if (node_p->dad)
    {
      assert((son(node_p->dad) == node_p)
	     || (bro(node_p->dad) == node_p));
    }
  if (node_p->son)
    {
      assert(dad(node_p->son) == node_p);
    }
  if (node_p->bro)
    {
      assert(dad(node_p->bro) == node_p);
    }
}
#endif

/* end of file */
