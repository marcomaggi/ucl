/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Fri Oct 26, 2007
   
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

#define UCL_DEBUGGING	0
#include "vectortest.h"

static void
callback (ucl_value_t state UCL_UNUSED, ucl_value_t custom)
{
  ucl_array_of_pointers_t *	slots	= custom.ptr;
  int **			values	= (int **)slots->slots;

  *values[0] = 0;
  for (size_t i=1; i<slots->number_of_slots; ++i)
    {
      ucl_debug("processing slot %u: %d", i, *values[i]);
      *values[0] += *values[i];
    }
  ucl_debug("result for slot %u: %d", slots->data.unum, *values[0]);
}
static void
initialise (ucl_vector_t vector)
{
  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);
}
static int
vector_ref (ucl_vector_t R, ucl_vector_index_t position)
{
  return *((int *)ucl_vector_index_to_slot(R, position));
}

void
test (void)
{
  ucl_vector_t		R, A, B, C;
  ucl_callback_t	cb = {
    .func = callback,
    .data = { .ptr = NULL }
  };


  initialise(R);
  initialise(A);
  initialise(B);
  initialise(C);
  fill(A, 5, 1);
  fill(B, 5, 6);
  fill(C, 5, 11);

  ucl_debug("mapping");
  ucl_vector_map_multiple(R, cb, A, B, C, NULL);

  assert((1+ 6+11) == vector_ref(R, 0));
  assert((2+ 7+12) == vector_ref(R, 1));
  assert((3+ 8+13) == vector_ref(R, 2));
  assert((4+ 9+14) == vector_ref(R, 3));
  assert((5+10+15) == vector_ref(R, 4));

  ucl_vector_destructor(R);
  ucl_vector_destructor(A);
  ucl_vector_destructor(B);
  ucl_vector_destructor(C);
}



/* end of file */
