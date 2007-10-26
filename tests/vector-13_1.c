/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Thu Oct 25, 2007
   
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
callback (ucl_value_t state UCL_UNUSED, ucl_value_t custom)
{
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
