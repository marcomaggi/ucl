/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Wed Oct 24, 2007
   
   Abstract
   
   
   
   Copyright (c) 2007, 2008 Marco Maggi
   
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
 ** Headers.
 ** ----------------------------------------------------------*/

#define DEBUGGING		0
#include "vectortest.h"

static void
callback (ucl_value_t state, va_list ap)
{
  ucl_value_t	custom = va_arg(ap,ucl_value_t);
  int *		accumulator_p	= state.ptr;
  int *		slot		= custom.ptr;

  *accumulator_p += *slot;
}

void
test (void)
{
  ucl_vector_t		vector;
  int			accumulator = 0;
  ucl_callback_t	cb = {
    .func = callback,
    .data = { .ptr = &accumulator }
  };
  ucl_range_t		range;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);

  fill(vector, 10, 1);
  
  ucl_range_set_min_max(range, 0, 4);
  ucl_vector_for_each_in_range(cb, range, vector);
  assert((1+2+3+4+5) == accumulator);

  accumulator = 0;
  ucl_range_set_min_max(range, 6, 9);
  ucl_vector_for_each_in_range(cb, range, vector);
  assert((7+8+9+10) == accumulator);

  accumulator = 0;
  ucl_range_set_min_max(range, 6, 6);
  ucl_vector_for_each_in_range(cb, range, vector);
  assert(7 == accumulator);

  ucl_vector_destructor(vector);
}



/* end of file */
