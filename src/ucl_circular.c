/*
  Part of: Useless Container Library (UCL).
  Contents: code for the circular container.

	The circular container provides a circular double linked list.

  Copyright (c) 2001, 2002, 2003, 2004, 2005, 2008 Marco Maggi

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.
  
  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.
  
  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include "internal.h"

#define stubmodule		circular

/* ------------------------------------------------------------ */


stub(2005-09-23-17-52-51) void
ucl_circular_insert (ucl_circular_t this, ucl_circular_link_t *new)
{
  assert(this);

  if (! this->current_link)
    {
      this->current_link	= new;
      new->next			= new;
      new->prev			= new;
    }
  else if (1 == this->size)
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

/* ------------------------------------------------------------ */


stub(2005-09-23-18-08-40) __attribute__((__noinline__)) ucl_circular_link_t *
ucl_circular_extract (ucl_circular_t this)
{
  ucl_circular_link_t *	cur;


  assert(this);
  if (this->size)
    {
      if (1 == this->size)
	{
	  cur = this->current_link;
	  this->current_link = NULL;
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
  else
    {
      return NULL;
    }
}

stub(2005-09-23-18-08-45) void
ucl_circular_forward (ucl_circular_t this, int times)
{
  assert(this);
  if (this->current_link && times)
    {
      int	i;

      if (times > 0)
	{
	  for (i=0; i<times; ++i)
	    {
	      assert(this->current_link->next);
	      this->current_link = this->current_link->next;
	    }
	}
      else
	{
	  for (i=0; i<-times; ++i)
	    {
	      assert(this->current_link->prev);
	      this->current_link = this->current_link->prev;
	    }
	}
    }
}

stub(2005-09-23-18-08-50) ucl_circular_link_t *
ucl_circular_find (ucl_circular_t this, ucl_value_t val)
{
  ucl_circular_link_t *	cur_p;
  ucl_valcmp_t		compar;


  assert(this);
  compar = this->compar;
  assert(compar.func);

  if (this->size == 0)
    {
      return NULL;
    }

  cur_p = this->current_link;
  do
    {
      if (0 == compar.func(compar.data, val, cur_p->val))
	{
	  this->current_link = cur_p;
	  return cur_p;
	}
      cur_p = cur_p->next;
    }
  while (cur_p != this->current_link);

  return NULL;
}

/* end of file */
