/* ucl_unrolled_list.c  --

   Part of: Useless Container Library (UCL).
   Contents: source code for the linked list container.
   Date: Sat Oct  7, 2006

   Abstract:

	The unrolled  list container  is a doubly  linked list  in which
	each node has an array of values.

		 --------
		| node   |
		| next_p |
		| prev_p |
		|        |
		| value1 |
		| value2 |
		| value3 |
		| value4 |
		 --------

	The length  of the  array is configurable.   The indexes  of the
	nodes and the indexes of the values are different.

	The unrolled list cannot be sorted with the heap sort.

   Copyright (c) 2006 Marco Maggi

   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.

*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

typedef struct ucl_unrolled_list_link_t {
  struct ucl_unrolled_list_link_t *	next_p;
  struct ucl_unrolled_list_link_t *	prev_p;

  size_t	number_of_used_elements;
  ucl_value_t *	val_p;
  ucl_value_t 	val[0];
} ucl_unrolled_list_link_t;

typedef struct ucl_unrolled_list_struct_t {
  size_t		number_of_allocated_nodes;
  size_t		number_of_elements;
  size_t		number_of_elements_per_node;

  ucl_unrolled_list_link_t *	front;	/* pointer to the first node */
  ucl_unrolled_list_link_t *	back;	/* pointer to the last node */
} ucl_unrolled_list_struct_t;

typedef ucl_unrolled_list_struct_t	ucl_unrolled_list_t[1];

typedef struct ucl_unrolled_list_index_t {
  size_t			element_index;
  ucl_unrolled_list_link_t *	link_p;
} ucl_unrolled_list_index_t;



#include "ucl_defines.h"

/* stub module name list */


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_unrolled_list_link_t	link_t;
typedef ucl_unrolled_list_t		list_t;



/** ------------------------------------------------------------
 ** Link allocation.
 ** ----------------------------------------------------------*/

stub(2006-10-07-09-25-04) size_t
ucl_unrolled_list_link_size (ucl_unrolled_list_t self)
{
  return sizeof(link_t) +
    (sizeof(ucl_value_t) - 1) * self->number_of_elements_per_node;
}
stub(2006-10-07-09-26-28) void
ucl_unrolled_list_initialise_link (ucl_unrolled_list_t self, ucl_unrolled_list_link_t * link_p)
{
  link_p->prev_p = link_p->next_p = NULL;
  link_p->val_p = &(link_p->val[0]);
  memset(link_p->val_p, '\0', sizeof(ucl_value_t) * self->number_of_elements_per_node);
}


/** ------------------------------------------------------------
 ** Link iteration.
 ** ----------------------------------------------------------*/

stub(2006-10-07-09-28-53) void
ucl_unrolled_list_next (ucl_unrolled_list_index_t * index_p)
{
  if (++(index_p->element_index) >= link_p->number_of_used_elements)
    {
      index_p->link_p = index_p->link_p->next_p;
      index_p->element_index = 0;
    }
}
stub(2006-10-07-09-32-37) void
ucl_unrolled_list_prev (ucl_unrolled_list_index_t * index_p)
{
  if (0 == index_p->element_index)
    {
      index_p->link_p = index_p->link_p->prev_p;
      index_p->element_index = link_p->number_of_used_elements - 1;
    }
  else
    {
      --(index_p->element_index);
    }
}


/** ------------------------------------------------------------
 ** Element index to list complex index.
 ** ----------------------------------------------------------*/

stub(2006-10-07-09-15-38) ucl_unrolled_list_index_t
ucl_unrolled_list_index (ucl_unrolled_list_t self, size_t element_index)
{
  ucl_unrolled_list_index_t	index = { 0, NULL };
  size_t	i;
  link_t *	link_p;


  if (element_index >= self->number_of_elements)
    {
      goto exit;
    }
  else if ((0 <= element_index) && (element_index < self->front->number_of_used_elements))
    {
      index.link_p        = self->front;
      index.element_index = element_index;
    }
  else if (((self->number_of_elements - self->back->number_of_used_elements)
	    <= element_index) &&
	   (element_index < self->number_of_elements))
    {
      index.link_p        = self->back;
      index.element_index = self->back->number_of_used_elements -
	(self->number_of_elements - element_index);
    }
  else if (element_index < (self->number_of_elements - element_index))
    { /* start scan at the beginning */
      link_p = self->front;
      index_of_first_element = 0;

      while (link_p)
	{
	  if ((index_of_first_element <= element_index) &&
	      (element_index < (index_of_first_element + link_p->number_of_used_elements)))
	    {
	      index.link_p        = link_p;
	      index.element_index = element_index - index_of_first_element;
	      break;
	    }
	  index_of_first_element += link_p->number_of_used_elements;
	  link_p = link_p->next_p;
	}
    }
  else
    { /* start scan at the end */      
      link_p = self->back;
      index_of_first_element = self->number_of_elements - link_p->number_of_used_elements;

      while (link_p)
	{
	  if ((index_of_first_element <= element_index) &&
	      (element_index < (index_of_first_element + link_p->number_of_used_elements)))
	    {
	      index.link_p        = link_p;
	      index.element_index = element_index - index_of_first_element;
	      break;
	    }
	  link_p = link_p->prev_p;
	  index_of_first_element -= link_p->number_of_used_elements;
	}
    }

  return index;
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

/** ------------------------------------------------------------
 ** Element extraction.
 ** ----------------------------------------------------------*/

stub(2006-10-07-09-51-14) ucl_unrolled_list_link_t *
ucl_unrolled_list_extract (ucl_unrolled_list_t self, ucl_unrolled_list_index_t index)
{
  link_t *	link_p = NULL;
  ucl_value_t *	val_p;


  if (index.element_index == index.link_p->number_of_used_elements)
    {
      --(index.link_p->number_of_used_elements);
    }
  else
    {
      val_p = &(index.link_p->val[index.element_index]);
      memmove(val_p, val_p+1, sizeof(ucl_value_t) *
	      (index.link_p->number_of_used_elements - index.element_index));
    }

  if (index.link_p->number_of_used_elements <
      (self->number_of_elements_per_node -
       index.link_p->prev_p->number_of_used_elements))
    { /* move elements into the prev link */
      memmove(&(index.link_p->prev_p->val[index.link_p->prev_p->number_of_used_elements]),
	      index.link_p->val_p, index.link_p->number_of_used_elements);
      index.link_p->number_of_used_elements = 0;
    }
  else if (index.link_p->number_of_used_elements <
	   (self->number_of_elements_per_node -
	    index.link_p->next_p->number_of_used_elements))
    { /* move elements into the next link */
      memmove(&(index.link_p->next_p->val[index.link_p->number_of_used_elements]),
	      index.link_p->next_p->val_p, index.link_p->next_p->number_of_used_elements);
      memmove(index.link_p->next_p->val_p,
	      index.link_p->val_p, index.link_p->number_of_used_elements);
      index.link_p->number_of_used_elements = 0;
    }
  
  if (0 == index.link_p->number_of_used_elements)
    {
      /* remove the link from the chain */
      link_p = index.link_p;
      link_p->next_p->prev_p = link_p->prev_p;
      link_p->prev_p->next_p = link_p->next_p;
    }

  return link_p;
}

/* end of file */
