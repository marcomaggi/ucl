/* 
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

   Copyright (c) 2001, 2002, 2003, 2004, 2005, 2008 Marco Maggi
   
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

#include "internal.h"

#define stubmodule		list

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_list_link_t		link_t;
typedef ucl_list_t		list_t;


stub(2005-09-23-18-11-16) ucl_list_link_t *
ucl_list_index (ucl_list_t this, size_t idx)
{
  size_t	m;
  size_t	i;
  link_t *	link_p;


  if (idx >= this->size)
    {
      return NULL;
    }
  else if (idx == 0)
    {
      return this->front;
    }
  else if (idx == this->size-1)
    {
      return this->back;
    }
  else if (this->li == idx)
    {
      return this->ll;
    }
  else if (this->li+1 == idx)
    {
      ++(this->li);
      return (this->ll = this->ll->next);
    }
  else if (this->li-1 == idx)
    {
      --(this->li);
      return (this->ll = this->ll->prev);
    }

  m = this->li - idx;

  /* m = abs(m) */
  m = (m>0)? m : -m;

  if (idx <= m)
    {
      /*
	"idx"  is near the  beginning more  than it's  near "this->li"
	start from the beginning.
      */

      link_p = this->front;
      for (i=0; i<idx; ++i)
	{
	  link_p = link_p->next;
	}
    }
  else if (this->size-idx-1 < m)
    {
      /*
	"idx" is near the end more than it's near "this->li"
	start from the end.
      */

      link_p = this->back;
      for (i=this->size-1; i>idx; --i)
	{
	  link_p = link_p->prev;
	}
    }
  else
    {
      /*
	"idx" is near "this->li"
      */

      link_p = this->ll;
      if (this->li > idx)
	{
	  /* from "this->li" backward */
	  for (i=this->li; i>idx; --i)
	    {
	      link_p = link_p->prev;
	    }
	}
      else
	{
	  /* fromt "this->li" forward */
	  for (i=this->li; i<idx; ++i)
	  {
	    link_p = link_p->next;
	  }
	}
    }

  this->li = idx;
  this->ll = link_p;
  return link_p;
}

/* ucl_list_insertbefore --

	Inserts a new link before a selected link.

   Arguments:

	this -		pointer to the base structure
	link_p -	pointer to a link in the list, it must be the
			return value of a previous invocation of
			ucl_list_index()
	new_p -	pointer to a new list link

   Results:

        The  new  link is  inserted  before  the  selected one;  if  the
        selected link  is the first in  the chain, the  new link becomes
        the first in the chain.

   Side effects:

	This function  can't be  used to insert  an element in  an empty
	list.

*/

stub(2005-09-23-18-11-20) void
ucl_list_insertbefore (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p)
{
  link_t *	tmp_p;


  assert(this); assert(link_p); assert(new_p);
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

  if (link_p == this->front)
    {
      this->front	= new_p;
      new_p->prev	= NULL;
    }
  else
    {
      new_p->prev = tmp_p = link_p->prev;
      tmp_p->next = new_p;
    }

  link_p->prev	= new_p;
  new_p->next	= link_p;

  this->ll	= this->front;
  this->li	= 0;
  
  ++(this->size);
}

/* ucl_list_insertafter --

	Inserts a new link after a selected link.

   Arguments:

	this -		pointer to the base structure
	link_p -	pointer to a link in the list, it must be the
			return value of a previous invocation of
			ucl_list_index()
	new_p -	pointer to a new list link

   Results:

        The new link is inserted after the selected one; if the selected
        link is the last in the  chain, the new link becomes the last in
        the chain.

   Side effects:

	This function  can't be  used to insert  an element in  an empty
	list.

*/

stub(2005-09-23-18-11-24) void
ucl_list_insertafter (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p)
{
  link_t *	tmp_p;


  assert(this); assert(link_p); assert(new_p);
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

  if (link_p == this->back)
    {
      this->back	= new_p;
      new_p->next	= NULL;
    }
  else
    {
      new_p->next = tmp_p = link_p->next;
      tmp_p->prev = new_p;
    }

  link_p->next	= new_p;
  new_p->prev	= link_p;
  
  this->ll	= this->front;
  this->li	= 0;

  ++(this->size);
}

stub(2005-09-23-18-11-26) void
ucl_list_pushfront (ucl_list_t this, ucl_list_link_t *link_p)
{
  assert(this); assert(link_p);

  if (! this->front)
    {
      link_p->next = NULL;
      this->ll = this->back = link_p;
      this->li = 0;
    }
  else
    {
      link_p->next = this->front;
      this->front->prev = link_p;
      ++(this->li);
    }
  link_p->prev = NULL;
  this->front = link_p;
  ++(this->size);
}
stub(2005-09-23-18-11-28) void
ucl_list_pushback (ucl_list_t this, ucl_list_link_t *link_p)
{
  assert(this); assert(link_p);

  if (! this->back)
    {
      link_p->prev = NULL;
      this->ll = this->front = link_p;
      this->li = 0;
    }
  else
    {
      link_p->prev = this->back;
      this->back->next = link_p;
    }
  link_p->next = NULL;
  this->back = link_p;
  ++(this->size);
}

/* ucl_list_extract --

	Removes an element from the list.

   Arguments:

	this -		pointer to the base structure
	link_p -	pointer to a list link, it must be the return
			value of a previous invocation of
			ucl_list_index()

   Results:

        The selected link is extracted from the list.

   Side effects:

        none

*/

stub(2005-09-23-18-11-32) void
ucl_list_extract (ucl_list_t this, ucl_list_link_t *link_p)
{
  link_t	*tmp_p;


  assert(this);
  assert(link_p);

  if (1 == this->size)
    {
      assert(link_p == this->back);
      assert(link_p == this->front);
      
      this->front = this->back = this->ll = NULL;
      this->li    = 0;
    }
  else if (link_p == this->front)
    {
      this->front	  = link_p->next;
      link_p->next->prev = NULL;

      if (this->li == 0)
	{
	  this->ll = this->front;
	}
      else 
	{
	  --(this->li);
	}
    }
  else if (link_p == this->back)
    {
      this->back	  = link_p->prev;
      link_p->prev->next = NULL;

      if (this->li == ((this->size) - 1))
	{
	  --(this->li); /* this is because we decrement this->size */
	  this->ll = this->back;
	}
    }
  else
    {
      tmp_p		= link_p->prev;
      tmp_p->next	= link_p->next;
      link_p->next->prev = tmp_p;

      this->li	= 0;
      this->ll	= NULL;
    }

  --(this->size);
}

/* end of file */
