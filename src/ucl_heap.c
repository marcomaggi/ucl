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

typedef ucl_value_t		value_t;
typedef ucl_heap_t		heap_t;
typedef ucl_heap_node_t		node_t;

#if (DEBUGGING == 1)
#  include <stdio.h>

static void	printnode	(node_t *node_p);
static void	printlevel	(node_t *root_p);
static void	assertnode	(node_t *node_p);
#endif

typedef struct Links {
  node_t *	dad_p;
  node_t *	bro_p;
  node_t *	son_p;
} Links;


#define dad(link)		((link)->dad_p)
#define bro(link)		((link)->bro_p)
#define son(link)		((link)->son_p)

#define SWAP(aptr, bptr)	tmp_p=(aptr);(aptr)=(bptr);(bptr)=tmp_p
#define LINKS(link)		*((Links *) (link))

/* ------------------------------------------------------------ */



stub(2005-09-23-18-11-02) void
ucl_heap_constructor (ucl_heap_t this, ucl_comparison_t compar)
{
  assert(this);
  ucl_struct_clean(this, ucl_heap_struct_t);
  this->compar	= compar;
}


stub(2005-09-23-18-11-04) void
ucl_heap_insert (ucl_heap_t this, ucl_heap_node_t *node_p)
{
  node_t *	root_p;
  node_t *	next_p;
  node_t *	dad_p;
  node_t *	tmp_p;
  value_t	a, b;
  int		first;


  root_p = this->root_p;
  if (! root_p)
    {
      this->root_p	= this->next_p = node_p;
      this->size	= 1;
      this->state	= 0;
      return;
    }


  next_p	= this->next_p;

  son(node_p) = bro(node_p) = NULL;
  if (this->state)
    {
      bro(next_p) = node_p;
    }
  else 
    {
      son(next_p) = node_p;
    }
  dad_p = dad(node_p) = next_p;

  a = ucl_heap_getval(dad_p);
  b = ucl_heap_getval(node_p);
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
	  next_p = dad_p;
	  --first;
	  break;
	}

      dad(node_p) = dad(dad_p);
      dad(dad_p)  = node_p;
      if (dad(node_p))
	{
	  if (son(dad(node_p)) == dad_p)
	    {
	      son(dad(node_p)) = node_p;
	    }
	  else
	    {
	      bro(dad(node_p)) = node_p;
	    }
	}

      if (son(dad_p) == node_p)
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

	  son(dad_p) = son(node_p);
	  son(node_p) = dad_p;
	  
	  SWAP(bro(dad_p), bro(node_p));
	  if (bro(node_p))
	    {
	      dad(bro(node_p)) = node_p;
	    }
	}
      else /* bro(dad_p) == node_p */
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
	  
	  bro(dad_p) = bro(node_p);
	  bro(node_p) = dad_p;

	  SWAP(son(dad_p), son(node_p));
	  if (son(node_p))
	    {
	      dad(son(node_p)) = node_p;
	    }
	}

      if (son(dad_p))
	{
	  dad(son(dad_p)) = dad_p;
	}
      if (bro(dad_p))
	{
	  dad(bro(dad_p)) = dad_p;
	}
      
      if (dad_p == root_p)
	{
	  root_p = node_p;
	  assert(dad(root_p) == NULL);
	  break;
	}

      dad_p = dad(node_p);
      a = ucl_heap_getval(dad_p);
      b = ucl_heap_getval(node_p);
    }

  if (this->state || bro(next_p))
    {
      next_p = (void *) ucl_btree_step_levelorder((void *) next_p);
    }

  assert(! dad(root_p));
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


stub(2005-09-23-18-11-06) ucl_heap_node_t *
ucl_heap_extract (ucl_heap_t this)
{
  node_t *	link_p;
  node_t *	tmp_p;
  node_t *	dad_p;
  value_t	a, b;
  int		first, v, fromleft=-1;
  Links		links, links1;


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

  links = LINKS(link_p);
  while (link_p)
    {
      if (links.son_p && links.bro_p)
	{
	  a = ucl_heap_getval(links.son_p);
	  b = ucl_heap_getval(links.bro_p);
	  v = (this->compar.func(this->compar.data, a, b) <= 0)? 1 : 0;
	}
      else if ((!links.son_p) && (!links.bro_p))
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
      else if (links.son_p)
	{
	  v = 1;
	}
      else
	{
	  v = 0;
	}

      dad_p = link_p;
      if (first)
	{
	  tmp_p	= dad_p;
#if (DEBUGGING == 1)
	  printf("extracting %d\n", tmp_p->val.integer);fflush(0);
#endif
	  dad(link_p) = dad_p = NULL;
	}
      
      if (v)
	{
#if (DEBUGGING == 1)
	  printf("going left\n");fflush(0);
#endif
	  link_p = links.son_p;
	  if (first)
	    {
	      this->root_p = link_p;
	      first	    = 0;
	    }
	  
	  if (link_p)
	    {
	      links1  = LINKS(link_p);
	      LINKS(link_p) = links;
	      son(link_p)   = NULL;
	      if (links.bro_p)
		{
		  dad(links.bro_p) = link_p;
		}
	      if (dad_p)
		{
		  if (fromleft == 1)
		    {
		      son(dad_p) = link_p;
		    }
		  else
		    {
		      bro(dad_p) = link_p;
		    }
		}
	      dad(link_p) = dad_p;
	      fromleft = 1;

#if (DEBUGGING == 1)
	      assertnode(dad_p);

	      printf("new link\ndad is ");
	      if (dad_p) { printnode(dad_p); } else { printf("NULL\n"); }
	      printnode(link_p);
#endif
	    }
	}
      else
	{
#if (DEBUGGING == 1)
	  printf("going right\n");fflush(0);
#endif
	  link_p = links.bro_p;
	  if (first)
	    {
	      this->root_p = link_p;
	      first	    = 0;
	    }
	  
	  if (link_p)
	    {
	      links1 = LINKS(link_p);
	      LINKS(link_p) = links;
	      bro(link_p)   = NULL;
	      if (links.son_p)
		{
		  dad(links.son_p) = link_p;
		}
	      if (dad_p)
		{
		  if (fromleft == 1)
		    {
		      son(dad_p) = link_p;
		    }
		  else
		    {
		      bro(dad_p) = link_p;
		    }
		}
	      dad(link_p) = dad_p;
	      fromleft = 0;
	      
#if (DEBUGGING == 1)
	      assertnode(dad_p);

	      printf("new link\ndad is ");
	      if (dad_p) { printnode(dad_p); } else { printf("NULL\n"); }
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
  ucl_heap_node_t *	node_p;


  for (node_p = ucl_heap_extract(other);
       node_p != NULL;
       node_p = ucl_heap_extract(other))
    {
      ucl_heap_insert(this, node_p);
    }
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
  printf(" dad=%d", (dad(node_p))? dad(node_p)->val.integer : -1);
  printf(" son=%d", (son(node_p))? son(node_p)->val.integer : -1);
  printf(" bro=%d", (bro(node_p))? bro(node_p)->val.integer : -1);
  printf("\n");fflush(0);

  if (dad(node_p))
    {
      assert((son(dad(node_p)) == node_p)
	     || (bro(dad(node_p)) == node_p));
    }
  if (son(node_p))
    {
      assert(dad(son(node_p)) == node_p);
    }
  if (bro(node_p))
    {
      assert(dad(bro(node_p)) == node_p);
    }
}

void
assertnode (node_t *node_p)
{
  if (!node_p) return;

  if (dad(node_p))
    {
      assert((son(dad(node_p)) == node_p)
	     || (bro(dad(node_p)) == node_p));
    }
  if (son(node_p))
    {
      assert(dad(son(node_p)) == node_p);
    }
  if (bro(node_p))
    {
      assert(dad(bro(node_p)) == node_p);
    }
}
#endif

/* end of file */
