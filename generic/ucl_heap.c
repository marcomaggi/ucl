/* ucl_heap.c --
   
   Part of: Useless Container Library
   Contents: heap implementation
   Date: Fri Feb 28, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: ucl_heap.c,v 1.1.1.9 2003/12/09 12:52:33 marco Exp $
*/


/** ------------------------------------------------------------
 ** Header files and type declarations.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"

typedef ucl_value_t		value_t;
typedef ucl_valcmp_t		valcmp_t;
typedef ucl_heap_t		heap_t;
typedef ucl_heap_node_t		node_t;

#ifdef DEBUGGING
#  include <stdio.h>

void	printnode	(node_t *nodePtr);
void	printlevel	(node_t *rootPtr);
void	assertnode	(node_t *nodePtr);
#endif

typedef struct Links {
  node_t *	dadPtr;
  node_t *	broPtr;
  node_t *	sonPtr;
} Links;


#define dad(link)		((link)->dadPtr)
#define bro(link)		((link)->broPtr)
#define son(link)		((link)->sonPtr)

#define SWAP(aptr, bptr)	tmpPtr=(aptr);(aptr)=(bptr);(bptr)=tmpPtr
#define LINKS(link)		*((Links *) (link))



/* ucl_heap_constructor --

	Initialises an already allocated heap structure.

   Arguments:

	this -		pointer to the base structure
	valcmp -	pointer to a strcmp()-like function used
			to compare values

   Results:

        Returns nothing.

   Side effects:

        None.

*/

void
ucl_heap_constructor (heap_t *this, ucl_valcmp_t *valcmp)
{
  assert(this != NULL);

  this->size	= 0;
  this->valcmp	= valcmp;
  this->state	= 0;
  this->rootPtr	= this->nextPtr = NULL;
}



/* ucl_heap_destructor --

	Resets all the fields of the base structure.

   Arguments:

	this -		pointer to the base structure

   Results:

        Returns nothing.

   Side effects:

        Before  invoking  this  function,   all  the  elements  must  be
        extracted from the heap. Sample code:

	heap_t *        this;
	node_t *   nodePtr;

	while (ucl_heap_size(this))
	  {
	     nodePtr = ucl_heap_extract(this);
	     ... frees the node structure ...
	  }
	ucl_heap_destructor(this);

*/

void
ucl_heap_destructor (heap_t *this)
{
  memset(this, '\0', sizeof(heap_t));
}


/* ucl_heap_insert --

	Inserts a node in the heap.

   Arguments:

	this -		pointer to the base structure
	nodePtr -	pointer to the node to insert

   Results:

        Returns nothing.

   Side effects:

        None.

*/

void
ucl_heap_insert (heap_t *this, node_t *nodePtr)
{
  node_t *	rootPtr;
  node_t *	nextPtr;
  node_t *	dadPtr;
  node_t *	tmpPtr;
  value_t		a, b;
  valcmp_t *		compar;
  int			first;


  rootPtr	= this->rootPtr;
  if (rootPtr == NULL)
    {
      this->rootPtr	= this->nextPtr = nodePtr;
      this->size	= 1;
      this->state	= 0;
      return;
    }


  nextPtr	= this->nextPtr;
  compar	= this->valcmp;

  son(nodePtr) = bro(nodePtr) = NULL;
  if (this->state)
    {
      bro(nextPtr) = nodePtr;
    }
  else 
    {
      son(nextPtr) = nodePtr;
    }
  dadPtr = dad(nodePtr) = nextPtr;

  a = ucl_heap_getval(dadPtr);
  b = ucl_heap_getval(nodePtr);
  first = 2;

  while (compar(a, b) > 0)
    {
      switch (first)
	{
	case 2:
	  nextPtr = nodePtr;
	  --first;
	  break;
	case 1:
	  nextPtr = dadPtr;
	  --first;
	  break;
	}

      dad(nodePtr) = dad(dadPtr);
      dad(dadPtr)  = nodePtr;
      if (dad(nodePtr))
	{
	  if (son(dad(nodePtr)) == dadPtr)
	    {
	      son(dad(nodePtr)) = nodePtr;
	    }
	  else
	    {
	      bro(dad(nodePtr)) = nodePtr;
	    }
	}

      if (son(dadPtr) == nodePtr)
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

	  son(dadPtr) = son(nodePtr);
	  son(nodePtr) = dadPtr;
	  
	  SWAP(bro(dadPtr), bro(nodePtr));
	  if (bro(nodePtr))
	    {
	      dad(bro(nodePtr)) = nodePtr;
	    }
	}
      else /* bro(dadPtr) == nodePtr */
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
	  
	  bro(dadPtr) = bro(nodePtr);
	  bro(nodePtr) = dadPtr;

	  SWAP(son(dadPtr), son(nodePtr));
	  if (son(nodePtr))
	    {
	      dad(son(nodePtr)) = nodePtr;
	    }
	}

      if (son(dadPtr))
	{
	  dad(son(dadPtr)) = dadPtr;
	}
      if (bro(dadPtr))
	{
	  dad(bro(dadPtr)) = dadPtr;
	}
      
      if (dadPtr == rootPtr)
	{
	  rootPtr = nodePtr;
	  assert(dad(rootPtr) == NULL);
	  break;
	}

      dadPtr = dad(nodePtr);
      a = ucl_heap_getval(dadPtr);
      b = ucl_heap_getval(nodePtr);
    }

  if (this->state || bro(nextPtr))
    {
      nextPtr = (void *) ucl_btree_step_levelorder((void *) nextPtr);
    }

  assert(dad(rootPtr) == NULL);
  assert(nextPtr);

  this->state	= !(this->state);
  this->rootPtr	= rootPtr;
  this->nextPtr = nextPtr;
  ++(this->size);

#ifdef DEBUGGING
  printnode(rootPtr);
  printnode(nextPtr);
  printlevel(rootPtr);
#endif
}


/* ucl_heap_extract --

	Extracts the next node from the heap.

   Arguments:

	this -		pointer to the base structure

   Results:

        Returns a pointer to the extracted node.

   Side effects:

        None.

*/

node_t *
ucl_heap_extract (heap_t *this)
{
  node_t *		linkPtr;
  node_t *		tmpPtr;
  node_t *		dadPtr;
  value_t		a, b;
  int			first, v, fromleft=-1;
  valcmp_t *		compar;
  Links			links, links1;


  assert(this != NULL);

  if (this->rootPtr == NULL)
    {
      return NULL;
    }
  if (this->size == 1)
    {
      tmpPtr		= this->rootPtr;
      this->rootPtr	= this->nextPtr = NULL;
      this->size	= 0;
      return tmpPtr;
    }

  tmpPtr	= NULL;

  compar	= this->valcmp;
  linkPtr	= this->rootPtr;
  first		= 1;

#ifdef DEBUGGING
  printf("---------------------\n");
  printlevel(this->rootPtr);
  printf("root ");
  printnode(this->rootPtr);
#endif

  links = LINKS(linkPtr);
  while (linkPtr)
    {
      if (links.sonPtr && links.broPtr)
	{
	  a = ucl_heap_getval(links.sonPtr);
	  b = ucl_heap_getval(links.broPtr);
	  v = (compar(a, b) <= 0)? 1 : 0;
	}
      else if ((!links.sonPtr) && (!links.broPtr))
	{
	  /*
	  if (son(linkPtr) == linkPtr)
	    {
	      son(linkPtr) = NULL;
	    }
	  else
	    {
	      bro(linkPtr) = NULL;
	    }
	  */
#ifdef DEBUGGING
	  printf("both NULLs\n");fflush(0);
  	  printlevel(this->rootPtr);
#endif

	  if (this->rootPtr)
	    {
	      this->nextPtr	= (void *) \
		ucl_btree_find_leftmost((void *) this->rootPtr);
	      this->state	= 0;
	    }
	  else
	    {
	      this->nextPtr = NULL;
	    }

#ifdef DEBUGGING
	  printf("next %p ", this->nextPtr);
  	  printnode(this->nextPtr);fflush(0);
#endif

	  --(this->size);
	  goto End;
	}
      else if (links.sonPtr)
	{
	  v = 1;
	}
      else
	{
	  v = 0;
	}

      dadPtr = linkPtr;
      if (first)
	{
	  tmpPtr	= dadPtr;
#ifdef DEBUGGING
	  printf("extracting %d\n", tmpPtr->val.integer);fflush(0);
#endif
	  dad(linkPtr) = dadPtr = NULL;
	}
      
      if (v)
	{
#ifdef DEBUGGING
	  printf("going left\n");fflush(0);
#endif
	  linkPtr = links.sonPtr;
	  if (first)
	    {
	      this->rootPtr = linkPtr;
	      first	    = 0;
	    }
	  
	  if (linkPtr)
	    {
	      links1  = LINKS(linkPtr);
	      LINKS(linkPtr) = links;
	      son(linkPtr)   = NULL;
	      if (links.broPtr)
		{
		  dad(links.broPtr) = linkPtr;
		}
	      if (dadPtr)
		{
		  if (fromleft == 1)
		    {
		      son(dadPtr) = linkPtr;
		    }
		  else
		    {
		      bro(dadPtr) = linkPtr;
		    }
		}
	      dad(linkPtr) = dadPtr;
	      fromleft = 1;

#ifdef DEBUGGING
	      assertnode(dadPtr);

	      printf("new link\ndad is ");
	      if (dadPtr) { printnode(dadPtr); } else { printf("NULL\n"); }
	      printnode(linkPtr);
#endif
	    }
	}
      else
	{
#ifdef DEBUGGING
	  printf("going right\n");fflush(0);
#endif
	  linkPtr = links.broPtr;
	  if (first)
	    {
	      this->rootPtr = linkPtr;
	      first	    = 0;
	    }
	  
	  if (linkPtr)
	    {
	      links1 = LINKS(linkPtr);
	      LINKS(linkPtr) = links;
	      bro(linkPtr)   = NULL;
	      if (links.sonPtr)
		{
		  dad(links.sonPtr) = linkPtr;
		}
	      if (dadPtr)
		{
		  if (fromleft == 1)
		    {
		      son(dadPtr) = linkPtr;
		    }
		  else
		    {
		      bro(dadPtr) = linkPtr;
		    }
		}
	      dad(linkPtr) = dadPtr;
	      fromleft = 0;
	      
#ifdef DEBUGGING
	      assertnode(dadPtr);

	      printf("new link\ndad is ");
	      if (dadPtr) { printnode(dadPtr); } else { printf("NULL\n"); }
	      printnode(linkPtr);
#endif
	    }
	}
      
      links = links1;
    }

 End:
  return tmpPtr;
}


#ifdef DEBUGGING
void
printlevel (node_t *rootPtr)
{
  printf("level:");
  while (rootPtr)
    {
      printf(" %d", rootPtr->val.integer);
      fflush(0);
      rootPtr = (void *) ucl_btree_step_levelorder((void *) rootPtr);
    }
  printf("\n");
  fflush(0);
}

void
printnode (node_t *nodePtr)
{
  assert(nodePtr);

  printf("node %d:", nodePtr->val.integer);
  printf(" dad=%d", (dad(nodePtr))? dad(nodePtr)->val.integer : -1);
  printf(" son=%d", (son(nodePtr))? son(nodePtr)->val.integer : -1);
  printf(" bro=%d", (bro(nodePtr))? bro(nodePtr)->val.integer : -1);
  printf("\n");fflush(0);

  if (dad(nodePtr))
    {
      assert((son(dad(nodePtr)) == nodePtr)
	     || (bro(dad(nodePtr)) == nodePtr));
    }
  if (son(nodePtr))
    {
      assert(dad(son(nodePtr)) == nodePtr);
    }
  if (bro(nodePtr))
    {
      assert(dad(bro(nodePtr)) == nodePtr);
    }
}

void
assertnode (node_t *nodePtr)
{
  if (!nodePtr) return;

  if (dad(nodePtr))
    {
      assert((son(dad(nodePtr)) == nodePtr)
	     || (bro(dad(nodePtr)) == nodePtr));
    }
  if (son(nodePtr))
    {
      assert(dad(son(nodePtr)) == nodePtr);
    }
  if (bro(nodePtr))
    {
      assert(dad(bro(nodePtr)) == nodePtr);
    }
}
#endif

/* end of file */
