/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Thu Oct 25, 2007
   
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
callback (ucl_value_t state UCL_UNUSED, va_list ap)
{
  ucl_value_t custom = va_arg(ap,ucl_value_t);
  ucl_array_of_pointers_t * slots   = custom.ptr;
  int *                     result  = slots->slots[0];
  int *                     operand = slots->slots[1];
  
  *result = - *operand;
}

void
test (void)
{
  ucl_vector_t		result, operand;
  ucl_callback_t	cb = {
    .func = callback,
    .data = { .ptr = NULL }
  };


  ucl_vector_initialise(result, sizeof(int));
  ucl_vector_constructor(result);
  ucl_vector_initialise(operand, sizeof(int));
  ucl_vector_constructor(operand);

  fill(operand, 5, 1);
  
  ucl_vector_map(result, cb, operand);

  for (ucl_vector_index_t i=0; i<5; ++i)
    {
      int *	slot = ucl_vector_index_to_slot(result, i);

      assert(-(int)(i+1) == *slot);
    }

  ucl_vector_destructor(result);
  ucl_vector_destructor(operand);
}



/* end of file */
