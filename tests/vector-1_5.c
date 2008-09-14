/* vector-1_5.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Fri Oct  6, 2006
   
   Abstract
   
	Block swallowing.
   
   Copyright (c) 2006, 2008 Marco Maggi
   
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

#define DEBUGGING		0
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

  block = ucl_block_alloc(allocator, sizeof(unsigned)*NUMBER);
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
