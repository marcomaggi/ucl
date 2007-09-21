/* ucl_circular.c --

   Part of: Useless Container Library (UCL).
   Contents: code for the circular container.

	The circular container provides a circular double linked list.

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

   $Id: ucl_circular.c,v 2.0.1.13 2003/12/09 10:20:35 marco Exp $

*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_circular_t		circular_t;
typedef ucl_circular_link_t	link_t;
typedef ucl_value_t		value_t;


/* ucl_circular_constructor --

	Initialises an already allocated UCL_Circlar struct.

   Arguments:

	this -		pointer to the base structure

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_circular_constructor (circular_t *this)
{
  assert(this != 0);
  
  this->current_link	= 0;
  this->size		= 0;
}

/* ucl_circular_destructor --

	Destroys  a circular_t struct.  All the  members are  reset to
	zero.

   Arguments:

	this -		pointer to the base structure

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_circular_destructor (circular_t *this)
{
  assert(this != 0);
  memset(this, '\0', sizeof(circular_t));
}

/* ucl_circular_insert --

	Inserts  an  element after  the  current  one  and make  it  the
	current.

   Arguments:

	this -		pointer to the base structure

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_circular_insert (circular_t *this, link_t *new)
{
  assert(this != 0);

  if (this->current_link == 0)
    {
      this->current_link	= new;
      new->next		= new;
      new->prev		= new;
    }
  else if (this->size == 1)
    {
      new->next			= this->current_link;
      new->prev			= this->current_link;
      this->current_link->next	= new;
      this->current_link->prev	= new;
      this->current_link	= new;
    }
  else
    {
      new->next			= this->current_link->next;
      this->current_link->next->prev = new;
      new->prev			= this->current_link;
      this->current_link->next	= new;
      this->current_link	= new;
    }

  ++(this->size);
}

/* ucl_circular_extract --

	Removes an element, make the next element the current one.

   Arguments:

	this -		pointer to the base structure

   Results:

        Returns a pointer  to the extracted link or NULL  if the list is
        empty.

   Side effects:

        None.

*/


link_t *
ucl_circular_extract (circular_t *this)
{
  link_t *	cur;


  assert(this != 0);

  if (this->size == 0)
    {
      return NULL;
    }
  else
    {
      if (this->size == 1)
	{
	  cur = this->current_link;
	  this->current_link = 0;
	}
      else
	{
	  cur			= this->current_link;
	  cur->next->prev	= cur->prev;
	  cur->prev->next	= cur->next;
	  this->current_link	= cur->next;
	}
      
      --(this->size);
      return cur;
    }
}

/* ucl_circular_forward --

	Make  current  an  element  in  the forward  direction.  If  the
	container is empty or the offset is zero nothing happens.

   Arguments:

	this -		pointer to the base structure
	times -		move forward this number of positions

   Results:

	None.

   Side effects:

        None.

*/

void
ucl_circular_forward (circular_t *this, size_t times)
{
  size_t	i;


  assert(this != 0);

  if ((this->current_link != 0) && (times != 0))
    {
      if (times > 0)
	{
	  for (i=0; i<times; ++i)
	    {
	      assert(this->current_link->next != 0);
	      this->current_link = this->current_link->next;
	    }
	}
      else
	{
	  ucl_circular_backward(this, -times);
	}
    }
}

/* ucl_circular_backward --

	Make  current  an element  in  the  backward  direction. If  the
	container is empty or the offset is zero nothing happens.

   Arguments:

	this -		pointer to the base structure
	times -		move forward this number of positions

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_circular_backward (circular_t *this, size_t times)
{
  size_t	i;


  assert(this != 0);

  if ((this->current_link != 0) && (times != 0))
    {
      if (times > 0)
	{
	  for (i=0; i<times; ++i)
	    {
	      assert(this->current_link->prev != 0);
	      this->current_link = this->current_link->prev;
	    }
	}
      else
	{
	  ucl_circular_forward(this, -times);
	}
    }
}

/* ucl_circular_find --

	Find an element and make it the current one.

   Arguments:

	this -		pointer to the base structure
	val -		the element to find
	valcmp -	the compare function, must work like strcmp()

   Results:

        Returns a pointer  to the link or NULL if  a matching object was
        not found.

   Side effects:

        None.

*/

link_t *
ucl_circular_find (this, val, valcmp)
     circular_t *	this;
     value_t		val;
     ucl_valcmp_t *	valcmp;
{
  link_t *	curPtr;


  assert(this != 0);

  if (this->size == 0)
    {
      return NULL;
    }

  curPtr = this->current_link;
  do
    {
      if (valcmp(val, curPtr->val) == 0)
	{
	  this->current_link = curPtr;
	  return curPtr;
	}
      curPtr = curPtr->next;
    }
  while (curPtr != this->current_link);

  return NULL;
}


/* end of file */
