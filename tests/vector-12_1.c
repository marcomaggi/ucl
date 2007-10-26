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
  int *	accumulator_p	= state.ptr;
  int *	slot		= custom.ptr;

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


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);

  fill(vector, 5, 1);
  
  ucl_vector_for_each(cb, vector);
  assert((1+2+3+4+5) == accumulator);

  ucl_vector_destructor(vector);
}



/* end of file */
