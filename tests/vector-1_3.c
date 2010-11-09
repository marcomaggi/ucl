/* vector-1_3.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Fri Nov 12, 2004
   
   Abstract
   
	Reallocation proofs with tests of the reallocated block size.
   
   Copyright (c) 2004, 2005 Marco Maggi
   
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

void
test (void)
{
  int			i;
  int *			ptr;
  ucl_vector_t		vector;

  /*
    the assertions below depends on the fact that size==step_up
   */
#define SIZE		8
#define STEP_UP		8
#define STEP_DOWN	10

  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, SIZE);
  ucl_vector_initialise_step_up(vector, STEP_UP);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  for (i=0; i<SIZE; ++i)
    {
      ptr = ucl_vector_index_to_new_slot(vector, i);
      ptr = ucl_vector_insert(vector, ptr);
      debug("number of free slots: %d", ucl_vector_number_of_free_slots(vector));
      assert( ucl_vector_number_of_free_slots(vector) == (size_t)(SIZE-i-1));
    }

  debug("size %d", ucl_vector_size(vector));
  assert( ucl_vector_number_of_free_slots(vector) == 0);
  ucl_vector_enlarge(vector);
  debug("after enlarging: free slots %d, step up were %d",
	    ucl_vector_number_of_free_slots(vector),
	    ucl_vector_number_of_step_up_slots(vector));
  assert( ucl_vector_number_of_free_slots(vector) == \
	  ucl_vector_number_of_step_up_slots(vector) );

  for (i=0; ((size_t)i)<ucl_vector_number_of_step_up_slots(vector); ++i)
    {
      ptr = ucl_vector_index_to_new_slot(vector, i);
      ptr = ucl_vector_insert(vector, ptr);
      debug("number of free slots: %d", ucl_vector_number_of_free_slots(vector));
      assert( ucl_vector_number_of_free_slots(vector) == \
	      ucl_vector_number_of_step_up_slots(vector) - i - 1);
    }

  debug("size %d", ucl_vector_size(vector));
  assert( ucl_vector_number_of_free_slots(vector) == 0);
  ucl_vector_enlarge(vector);
  assert( ucl_vector_number_of_free_slots(vector) == \
	  ucl_vector_number_of_step_up_slots(vector) );

  for (i=0; ((size_t)i)<ucl_vector_number_of_step_up_slots(vector); ++i)
    {
      ptr = ucl_vector_index_to_new_slot(vector, i);
      ptr = ucl_vector_insert(vector, ptr);
      debug("number of free slots: %d", ucl_vector_number_of_free_slots(vector));
      assert( ucl_vector_number_of_free_slots(vector) == \
	      ucl_vector_number_of_step_up_slots(vector) - i - 1);
    }
	    
  /* RESTRICTING */

  debug("beginning to restrict, size %d", ucl_vector_size(vector));

  for (i=0; ((size_t)i)<ucl_vector_number_of_step_down_slots(vector); ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, 0);
      ucl_vector_erase(vector, ptr);
      debug("number of free slots: %d", ucl_vector_number_of_free_slots(vector));
      assert( ucl_vector_number_of_free_slots(vector) == (size_t)(i+1));
    }

  debug("restriction, size %d", ucl_vector_size(vector));
  assert( ucl_vector_number_of_free_slots(vector) == STEP_DOWN);
  ucl_vector_restrict(vector);
  debug("number of free slots: %d", ucl_vector_number_of_free_slots(vector));

  ucl_vector_destructor(vector);
}

/* end of file */
