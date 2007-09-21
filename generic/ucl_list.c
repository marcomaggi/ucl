/* ucl_list.c  --

   Part of: Useless Container Library (UCL).
   Contents: source code for the linked list container.

   Abstract:

        This file must  be included in all the modules  that make use of
        the list container.

	  The  UCL list container  is an  implementation of  the classic
	double  linked list:  elements are  stored in  little structures
	chained  together with  pointers  that allow  the  user code  to
	iterate forward and backward.

	  The handling  of list  links is derived  from the  handling of
	elements in the  TCL (Tool Command Language) hash  table by John
	Ousterhout and others (<http://www.tcl.tk> for more about TCL).

   Copyright (c) 2001, 2002, 2003 Marco Maggi

   This library is free software;  you can redistribute it and/or modify
   it  under the  terms  of the  GNU  Lesser General  Public License  as
   published by the Free Software  Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.

   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA

   $Id: ucl_list.c,v 2.0.1.22 2003/12/09 12:54:11 marco Exp $

*/



/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_list_link_t		link_t;
typedef ucl_list_t		list_t;


/* ucl_list_constructor --

	Initialises an already allocated list structure.

   Arguments:

	this -		pointer to the base structure

   Results:

        None.

   Side effects:

        None

*/

void
ucl_list_constructor (ucl_list_t *this)
{
  assert(this != NULL);

  this->size		= 0;
  this->front		= NULL;
  this->back		= NULL;
  this->li		= 0;
  this->ll		= NULL;
}


/* ucl_list_destructor --

	Destroys the  container. Before the invocation  of this function
	all the elements must be extracted from the list.

   Arguments:

	this -		pointer to the base structure

   Results:

	None.

   Side effects:

        none

*/

void
ucl_list_destructor (ucl_list_t *this)
{
  assert(this != NULL);
  memset(this, '\0', sizeof(ucl_list_t));
}



/* ucl_list_index --

	Converts a position in a pointer to the corresponding list link.

   Arguments:

	this -		pointer to the base structure
	index -		the index in the list

   Results:

        Returns a pointer to the selected  link, or NULL if the index is
        out of range.

   Side effects:

        None.

*/

link_t *
ucl_list_index (ucl_list_t *this, size_t index)
{
  ptrdiff_t		m;
  size_t		i;
  link_t *	linkPtr;


  if (index >= this->size)
    {
      return NULL;
    }
  else if (index == 0)
    {
      return this->front;
    }
  else if (index == this->size-1)
    {
      return this->back;
    }
  else if (this->li == index)
    {
      return this->ll;
    }
  else if (this->li+1 == index)
    {
      ++(this->li);
      return (this->ll = this->ll->next);
    }
  else if (this->li-1 == index)
    {
      --(this->li);
      return (this->ll = this->ll->prev);
    }

  m = this->li - index;

  /* m = abs(m) */
  m = (m>0)? m : -m;

  if (index <= m)
    {
      /*
	"index"  is near the  beginning more  than it's  near "this->li"
	start from the beginning.
      */

      linkPtr = this->front;
      for (i=0; i<index; ++i)
	{
	  linkPtr = linkPtr->next;
	}
    }
  else if (this->size-index-1 < m)
    {
      /*
	"index" is near the end more than it's near "this->li"
	start from the end.
      */

      linkPtr = this->back;
      for (i=this->size-1; i>index; --i)
	{
	  linkPtr = linkPtr->prev;
	}
    }
  else
    {
      /*
	"index" is near "this->li"
      */

      linkPtr = this->ll;
      if (this->li > index)
	{
	  /* from "this->li" backward */
	  for (i=this->li; i>index; --i)
	    {
	      linkPtr = linkPtr->prev;
	    }
	}
      else
	{
	  /* fromt "this->li" forward */
	  for (i=this->li; i<index; ++i)
	  {
	    linkPtr = linkPtr->next;
	  }
	}
    }

  this->li = index;
  this->ll = linkPtr;
  return linkPtr;
}


/* ucl_list_insertbefore --

	Inserts a new link before a selected link.

   Arguments:

	this -		pointer to the base structure
	linkPtr -	pointer to a link in the list, it must be the
			return value of a previous invocation of
			ucl_list_index()
	newPtr -	pointer to a new list link

   Results:

        The  new  link is  inserted  before  the  selected one;  if  the
        selected link  is the first in  the chain, the  new link becomes
        the first in the chain.

   Side effects:

	This function  can't be  used to insert  an element in  an empty
	list.

*/

void
ucl_list_insertbefore (this, linkPtr, newPtr)
     ucl_list_t *		this;
     link_t *	linkPtr;
     link_t *	newPtr;
{
  link_t *	tmpPtr;


  assert(this    != NULL);
  assert(linkPtr != NULL);
  assert(newPtr  != NULL);


  /*
    Before:

                 ----       -----
                |prev|<--->|link |
                 ----       -----

    after:

                 ----       -----       -----
                |prev|<--->| new |<--->|link |
                 ----       -----       -----
  */

  if (linkPtr == this->front)
    {
      this->front	= newPtr;
      newPtr->prev	= NULL;
    }
  else
    {
      newPtr->prev = tmpPtr = linkPtr->prev;
      tmpPtr->next = newPtr;
    }

  linkPtr->prev	= newPtr;
  newPtr->next	= linkPtr;

  this->ll	= this->front;
  this->li	= 0;
  
  ++(this->size);
}


/* ucl_list_insertafter --

	Inserts a new link after a selected link.

   Arguments:

	this -		pointer to the base structure
	linkPtr -	pointer to a link in the list, it must be the
			return value of a previous invocation of
			ucl_list_index()
	newPtr -	pointer to a new list link

   Results:

        The new link is inserted after the selected one; if the selected
        link is the last in the  chain, the new link becomes the last in
        the chain.

   Side effects:

	This function  can't be  used to insert  an element in  an empty
	list.

*/

void
ucl_list_insertafter (this, linkPtr, newPtr)
     ucl_list_t *		this;
     link_t *	linkPtr;
     link_t *	newPtr;
{
  link_t *	tmpPtr;


  assert(this    != NULL);
  assert(linkPtr != NULL);
  assert(newPtr  != NULL);


  /*
    Before:

                 ----       ----
                |link|<--->|next|
                 ----       ----

    after:

                 ----       -----       ----
                |link|<--->| new |<--->|next|
                 ----       -----       ----
  */

  if (linkPtr == this->back)
    {
      this->back	= newPtr;
      newPtr->next	= NULL;
    }
  else
    {
      newPtr->next = tmpPtr = linkPtr->next;
      tmpPtr->prev = newPtr;
    }

  linkPtr->next	= newPtr;
  newPtr->prev	= linkPtr;
  
  this->ll	= this->front;
  this->li	= 0;

  ++(this->size);
}



/* ucl_list_pushfront --

	Inserts an element at the beginning of the list.

   Arguments:

	this -		pointer to the base structure
	linkPtr -	pointer to the new link

   Results:

        The element is inserted and becomes the first in the chain.

   Side effects:

        None.

*/

void
ucl_list_pushfront (ucl_list_t *this, link_t *linkPtr)
{
  assert(this != NULL);
  assert(linkPtr != NULL);

  if (this->front == NULL)
    {
      linkPtr->next = NULL;
      this->ll = this->back = linkPtr;
      this->li = 0;
    }
  else
    {
      linkPtr->next = this->front;
      this->front->prev = linkPtr;

      ++(this->li);
    }

  linkPtr->prev = NULL;
  this->front = linkPtr;
  ++(this->size);
}



/* ucl_list_pushback --

	Inserts an element at the end of the list.

   Arguments:

	this -		pointer to the base structure
	linkPtr -	pointer to the new link

   Results:

        The element is inserted and becomes the last in the chain.

   Side effects:

        None.

*/

void
ucl_list_pushback (ucl_list_t *this, link_t *linkPtr)
{
  assert(this != NULL);
  assert(linkPtr != NULL);

  if (this->back == NULL)
    {
      linkPtr->prev = NULL;
      this->ll = this->front = linkPtr;
      this->li = 0;
    }
  else
    {
      linkPtr->prev = this->back;
      this->back->next = linkPtr;
    }

  linkPtr->next = NULL;
  this->back = linkPtr;
  ++(this->size);
}



/* ucl_list_extract --

	Removes an element from the list.

   Arguments:

	this -		pointer to the base structure
	linkPtr -	pointer to a list link, it must be the return
			value of a previous invocation of
			ucl_list_index()

   Results:

        The selected link is extracted from the list.

   Side effects:

        none

*/

void
ucl_list_extract (ucl_list_t *this, link_t *linkPtr)
{
  link_t		*tmpPtr;


  assert(this != NULL);
  assert(linkPtr != NULL);

  if (this->size == 1)
    {
      assert(linkPtr == this->back);
      assert(linkPtr == this->front);
      
      this->front = this->back = this->ll = NULL;
      this->li    = 0;
    }
  else if (linkPtr == this->front)
    {
      this->front	  = linkPtr->next;
      linkPtr->next->prev = NULL;

      if (this->li == 0)
	{
	  this->ll = this->front;
	}
      else 
	{
	  --(this->li);
	}
    }
  else if (linkPtr == this->back)
    {
      this->back	  = linkPtr->prev;
      linkPtr->prev->next = NULL;

      if (this->li == ((this->size) - 1))
	{
	  --(this->li); /* this is because we decrement this->size */
	  this->ll = this->back;
	}
    }
  else
    {
      tmpPtr		= linkPtr->prev;
      tmpPtr->next	= linkPtr->next;
      linkPtr->next->prev = tmpPtr;

      this->li	= 0;
      this->ll	= NULL;
    }

  --(this->size);
}


/* end of file */
