/* vector-9_1.c --
   
   Part of: UCL
   Contents: tests for special initialisers
   Date: Fri Mar  3, 2006
   
   Abstract
   
   
   
   Copyright (c) 2006 Marco Maggi
   
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
#undef NUMBER
#define NUMBER		(10*UCL_VECTOR_BUFFER_PAGE_SIZE)

void
test (void)
{
  ucl_vector_t	vector;
  ucl_block_t	allocated_memory;


  ucl_vector_initialise_buffer(vector);
  ucl_vector_constructor(vector);

  allocated_memory = ucl_vector_get_memory_block(vector);
  assert(allocated_memory.ptr);
  assert(ucl_vector_number_of_free_slots(vector) == UCL_VECTOR_BUFFER_PAGE_SIZE);

  assert( ucl_vector_number_of_step_up_slots(vector) == UCL_VECTOR_BUFFER_PAGE_SIZE );
  assert( ucl_vector_number_of_step_down_slots(vector) == UCL_VECTOR_BUFFER_PAGE_SIZE+1);
  assert( ucl_vector_number_of_padding_slots(vector) == 0);
  
  assert( ucl_vector_slot_dimension(vector) == sizeof(ucl_byte_t) );

  fill(vector, NUMBER, 0);
  assert(ucl_vector_size(vector) == NUMBER);
  clean(vector);
  fill(vector, NUMBER, 0); clean(vector);
  fill(vector, NUMBER, 0); clean(vector);

  ucl_vector_destructor(vector);
}

/* end of file */
