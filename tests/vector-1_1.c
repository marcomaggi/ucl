/* allocation_and_deallocation.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Initialisation, allocation, inspection.
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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

#include "vectortest.h"
#define SIZE		8
#define STEP_UP		6
#define STEP_DOWN	10
#define PAD		0

void
test (void)
{
  ucl_vector_t	vector;
  ucl_block_t	allocated_memory;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector,		SIZE);
  ucl_vector_initialise_step_up(vector,		STEP_UP);
  ucl_vector_initialise_step_down(vector,	STEP_DOWN);
  ucl_vector_initialise_pad(vector,		PAD);
  ucl_vector_constructor(vector);
  allocated_memory = ucl_vector_get_memory_block(vector);
  assert(allocated_memory.ptr);
  assert(ucl_vector_number_of_free_slots(vector) == SIZE);

  assert( ucl_vector_number_of_step_up_slots(vector) == STEP_UP );
  assert( ucl_vector_number_of_step_down_slots(vector) == STEP_DOWN );
  assert( ucl_vector_number_of_padding_slots(vector) == PAD );
  
  assert( ucl_vector_slot_dimension(vector) == sizeof(int) );

  ucl_vector_destructor(vector);
}

/* end of file */
