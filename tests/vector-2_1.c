/* vector-2_1.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Index related functions
   
   Copyright (c) 2005, 2008 Marco Maggi
   
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

#include "vectortest.h"

static void test_index (ucl_vector_t vector, ucl_vector_index_t index);

void
test (void)
{
  ucl_vector_t		vector;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);

  fill(vector, 100, 0);

  test_index(vector, 0);
  test_index(vector, 10);
  test_index(vector, 99);

  assert( ucl_vector_last_index(vector) == 99 );
  assert( ucl_vector_pointer_is_valid_slot(vector,
					   ucl_vector_index_to_slot(vector,0)) );
  assert( ucl_vector_pointer_is_valid_slot(vector,
					   ucl_vector_index_to_slot(vector,10)) );
  assert( ucl_vector_pointer_is_valid_slot(vector,
					   ucl_vector_index_to_slot(vector,99)) );
  assert( ! ucl_vector_pointer_is_valid_slot(vector,
					   ucl_vector_index_to_new_slot(vector,100)) );

  assert( ucl_vector_index_is_valid(vector, 0) );
  assert( ucl_vector_index_is_valid(vector, 10) );
  assert( ucl_vector_index_is_valid(vector, 99) );
  assert( ! ucl_vector_index_is_valid(vector, 100) );

  ucl_vector_destructor(vector);
}
static void
test_index (ucl_vector_t vector, ucl_vector_index_t _index)
{
  uint8_t *		p;
  ucl_vector_index_t	i;

  p = ucl_vector_index_to_slot(vector, _index);
  assert( vector->first_used_slot+sizeof(int)*_index == p );
  i = ucl_vector_slot_to_index(vector, p);
  assert( i == _index );
}

/* end of file */
