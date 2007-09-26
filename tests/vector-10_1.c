/*
   Part of: Useless Containers Library
   Contents: test for vector range/block conversion
   Date: Sun Nov 19, 2006
   
   Abstract
   
   
   
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
  ucl_block_t	B;
  ucl_range_t	R;


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

  fill(vector, 100, 0);

  /* ------------------------------------------------------------ */

  ucl_range_set_min_max(R, 0, 99);
  B = ucl_vector_block_from_range(vector, R);
  for (int i=0; i<100; ++i)
    assert(i == ((int *)B.ptr)[i]);

  ucl_range_set_min_max(R, 10, 20);
  B = ucl_vector_block_from_range(vector, R);
  for (int i=0; i<=10; ++i)
    {
/*       fprintf(stderr, "%d %d\n", i, ((int *)B.ptr)[i]); */
      assert(i+10 == ((int *)B.ptr)[i]);
    }

  ucl_range_set_min_max(R, 10, 10);
  B = ucl_vector_block_from_range(vector, R);
  for (int i=0; i<=10; ++i)
    assert(i+10 == ((int *)B.ptr)[i]);

  ucl_range_set_min_max(R, 0, 0);
  B = ucl_vector_block_from_range(vector, R);
  for (int i=0; i<=0; ++i)
    assert(i == ((int *)B.ptr)[i]);

  ucl_range_set_min_max(R, 99, 99);
  B = ucl_vector_block_from_range(vector, R);
  for (int i=0; i<=0; ++i)
    assert(i+99 == ((int *)B.ptr)[i]);

  /* ------------------------------------------------------------ */

  B.ptr = ucl_vector_index_to_slot(vector, 0);
  B.len = ucl_vector_slot_dimension(vector) * ucl_vector_size(vector);  
  R = ucl_vector_range_from_block(vector, B);
/*   fprintf(stderr, "%d %d\n", ucl_range_min(R), ucl_range_max(R)); */
  assert(0  == ucl_range_min(R));
  assert(99 == ucl_range_max(R));

  B.ptr = ucl_vector_index_to_slot(vector, 10);
  B.len = ucl_vector_slot_dimension(vector) * 10;
  R = ucl_vector_range_from_block(vector, B);
  /*   fprintf(stderr, "%d %d\n", ucl_range_min(R), ucl_range_max(R)); */
  /* Ranges  are  inclusive,  and in  the  range  [0,  9] there  are  10
     slots. */
  assert(10  == ucl_range_min(R)); 
  assert(19 == ucl_range_max(R));

  B.ptr = ucl_vector_index_to_slot(vector, 12);
  B.len = ucl_vector_slot_dimension(vector);
  R = ucl_vector_range_from_block(vector, B);
  assert(12 == ucl_range_min(R)); 
  assert(12 == ucl_range_max(R));

  B.ptr = ucl_vector_index_to_slot(vector, 0);
  B.len = ucl_vector_slot_dimension(vector);
  R = ucl_vector_range_from_block(vector, B);
  assert(0 == ucl_range_min(R)); 
  assert(0 == ucl_range_max(R));

  B.ptr = ucl_vector_index_to_slot(vector, 99);
  B.len = ucl_vector_slot_dimension(vector);
  R = ucl_vector_range_from_block(vector, B);
  assert(99 == ucl_range_min(R)); 
  assert(99 == ucl_range_max(R));

  /* ------------------------------------------------------------ */

  ucl_vector_destructor(vector);
}

/* end of file */
