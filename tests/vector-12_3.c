/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Wed Oct 24, 2007
   
   Abstract
   
   
   
   Copyright (c) 2007 Marco Maggi
   
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
 ** Headers.
 ** ----------------------------------------------------------*/

#if 0
#  define UCL_DEBUGGING
#endif

#include "vectortest.h"

static void
callback (ucl_value_t state, ucl_value_t custom)
{
  int *				sums	= state.ptr;
  ucl_array_of_pointers_t *	slots	= custom.ptr;
  int **			values	= (int **)slots->slots;

  for (size_t i=0; i<slots->number_of_slots; ++i)
    sums[slots->data.unum] += *values[i];
}
static void
initialise (ucl_vector_t vector)
{
  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);
}

void
test (void)
{
  ucl_vector_t		A, B, C;
  int			sums[5] = { 0, 0, 0, 0, 0 };
  ucl_callback_t	cb = {
    .func = callback,
    .data = { .ptr = &sums }
  };


  initialise(A);
  initialise(B);
  initialise(C);
  fill(A, 5, 1);
  fill(B, 5, 6);
  fill(C, 5, 11);

  ucl_vector_for_each_multiple(cb, A, B, C, NULL);

  assert((1+ 6+11) == sums[0]);
  assert((2+ 7+12) == sums[1]);
  assert((3+ 8+13) == sums[2]);
  assert((4+ 9+14) == sums[3]);
  assert((5+10+15) == sums[4]);

  ucl_vector_destructor(A);
  ucl_vector_destructor(B);
  ucl_vector_destructor(C);
}



/* end of file */
