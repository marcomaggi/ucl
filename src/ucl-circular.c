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

#define next		son
#define prev		dad


void
ucl_circular_insert (ucl_circular_t self, ucl_node_t new)
{
  assert(self);
  if (! self->current) {
    self->current	= new;
    new->next		= new;
    new->prev		= new;
  } else if (1 == self->size) {
    new->next		= self->current;
    new->prev		= self->current;
    self->current->next	= new;
    self->current->prev	= new;
    self->current	= new;
  } else {
    new->next			= self->current->next;
    self->current->next->prev	= new;
    new->prev			= self->current;
    self->current->next		= new;
    self->current		= new;
  }
  ++(self->size);
}

ucl_node_t
ucl_circular_extract (ucl_circular_t self)
{
  ucl_node_t 	cur;
  assert(self);
  if (self->size) {
    if (1 == self->size) {
      cur = self->current;
      self->current = NULL;
    } else {
      cur		= self->current;
      cur->next->prev	= cur->prev;
      cur->prev->next	= cur->next;
      self->current	= cur->next;
    }
    --(self->size);
    return cur;
  } else return NULL;
}

void
ucl_circular_forward (ucl_circular_t self, int times)
{
  assert(self);
  if (self->current && times) {
    int		i;
    if (times > 0) {
      for (i=0; i<times; ++i) {
	assert(self->current->next);
	self->current = self->current->next;
      }
    } else {
      for (i=0; i<-times; ++i) {
	assert(self->current->prev);
	self->current = self->current->prev;
      }
    }
  }
}

ucl_node_t
ucl_circular_find (ucl_circular_t self, ucl_value_t val)
{
  ucl_node_t		cur;
  ucl_comparison_t	compar;
  ucl_value_t		inner;
  assert(self);
  compar = self->compar;
  assert(compar.func);
  if (self->size == 0)
    return NULL;
  cur = self->current;
  do {
    inner.pointer = cur;
    if (0 == compar.func(compar.data, val, inner)) {
      self->current = cur;
      return cur;
    }
    cur = cur->next;
  } while (cur != self->current);
  return NULL;
}

/* end of file */
