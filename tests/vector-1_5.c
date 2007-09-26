/* vector-1_5.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Fri Oct  6, 2006
   
   Abstract
   
	Block swallowing.
   
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

/* #define UCL_DEBUGGING */
#include "vectortest.h"

#define SIZE		8
#define STEP_UP		8
#define STEP_DOWN	10

void
test (void)
{
  unsigned		i;
  unsigned *		p;
  ucl_vector_t		vector;
  ucl_block_t		block = { 0, 0 };

  ucl_memory_allocator_t allocator = { NULL, ucl_memory_alloc };


  ucl_vector_initialise(vector, sizeof(unsigned));

  ucl_block_alloc(allocator, block, sizeof(unsigned)*NUMBER);
  p = (unsigned *)block.ptr;
  for (i=0; i<NUMBER; ++i)
    p[i] = i;

  ucl_vector_swallow_block(vector, block);

  assert(ucl_vector_number_of_free_slots(vector) == 0);
  assert(ucl_vector_size(vector) == NUMBER);

  for (i=0; i<NUMBER; ++i)
    {
      p = ucl_vector_index_to_slot(vector, i);
      assert(*p == i);
    }

  ucl_vector_destructor(vector);
}

/* end of file */
