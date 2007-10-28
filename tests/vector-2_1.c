/* vector-2_1.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Index related functions
   
   Copyright (c) 2005 Marco Maggi
   
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
  ucl_byte_t *		p;
  ucl_vector_index_t	i;

  p = ucl_vector_index_to_slot(vector, _index);
  assert( vector->first_used_slot+sizeof(int)*_index == p );
  i = ucl_vector_slot_to_index(vector, p);
  assert( i == _index );
}

/* end of file */
