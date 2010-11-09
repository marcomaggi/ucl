/*
  Part of: Useless Container Library
  Contents: code for the circular container.

  Abstract

	The circular container provides a circular double linked list.

  Copyright (c) 2001-2005, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>

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


void
ucl_circular_insert (ucl_circular_t self, ucl_circular_link_t *new)
{
  assert(self);
  if (! self->current_link) {
    self->current_link		= new;
    new->next			= new;
    new->prev			= new;
  } else if (1 == self->size) {
    new->next			= self->current_link;
    new->prev			= self->current_link;
    self->current_link->next	= new;
    self->current_link->prev	= new;
    self->current_link		= new;
  } else {
    new->next			= self->current_link->next;
    self->current_link->next->prev = new;
    new->prev			= self->current_link;
    self->current_link->next	= new;
    self->current_link		= new;
  }
  ++(self->size);
}

ucl_circular_link_t *
ucl_circular_extract (ucl_circular_t self)
{
  ucl_circular_link_t *	cur;
  assert(self);
  if (self->size) {
    if (1 == self->size) {
      cur = self->current_link;
      self->current_link = NULL;
    } else {
      cur		= self->current_link;
      cur->next->prev	= cur->prev;
      cur->prev->next	= cur->next;
      self->current_link= cur->next;
    }
    --(self->size);
    return cur;
  } else return NULL;
}

void
ucl_circular_forward (ucl_circular_t self, int times)
{
  assert(self);
  if (self->current_link && times) {
    int		i;
    if (times > 0) {
      for (i=0; i<times; ++i) {
	assert(self->current_link->next);
	self->current_link = self->current_link->next;
      }
    } else {
      for (i=0; i<-times; ++i) {
	assert(self->current_link->prev);
	self->current_link = self->current_link->prev;
      }
    }
  }
}

ucl_circular_link_t *
ucl_circular_find (ucl_circular_t self, ucl_value_t val)
{
  ucl_circular_link_t *		cur_p;
  ucl_value_comparison_t	compar;
  assert(self);
  compar = self->compar;
  assert(compar.func);
  if (self->size == 0)
    return NULL;
  cur_p = self->current_link;
  do {
    if (0 == compar.func(compar.data, val, cur_p->val)) {
      self->current_link = cur_p;
      return cur_p;
    }
    cur_p = cur_p->next;
  }
  while (cur_p != self->current_link);
  return NULL;
}

/* end of file */
