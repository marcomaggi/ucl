/* vector-8_4.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Comparation.
   
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

#define DEBUGGING		0
#include "vectortest.h"

void
test (void)
{
  ucl_vector_t	a, b;
  ucl_range_t	range_a, range_b;
  ucl_comparison_t	compar = { { .ptr = NULL}, ucl_ptrintcmp };


  /* equal */

  ucl_vector_initialise(a, sizeof(int));
  ucl_vector_constructor(a);
  ucl_vector_initialise(b, sizeof(int));
  ucl_vector_constructor(b);

  ucl_vector_set_compar(a, compar);
  ucl_vector_set_compar(b, compar);

  fill(a, 10, 0);
  fill(b, 10, 0);
  assert( ucl_vector_compare(a, b) == 0);
  assert( ucl_vector_equal(a, b) );

  ucl_vector_destructor(b);
  ucl_vector_destructor(a);

  /* lesser */

  ucl_vector_initialise(a, sizeof(int));
  ucl_vector_constructor(a);
  ucl_vector_initialise(b, sizeof(int));
  ucl_vector_constructor(b);

  ucl_vector_set_compar(a, compar);
  ucl_vector_set_compar(b, compar);

  fill(a, 10, 0);
  fill(b, 10, 1);
  assert( ucl_vector_compare(a, b) == -1);

  ucl_vector_destructor(b);
  ucl_vector_destructor(a);

  /* greater */

  ucl_vector_initialise(a, sizeof(int));
  ucl_vector_constructor(a);
  ucl_vector_initialise(b, sizeof(int));
  ucl_vector_constructor(b);

  ucl_vector_set_compar(a, compar);
  ucl_vector_set_compar(b, compar);

  fill(a, 10, 1);
  fill(b, 10, 0);
  assert( ucl_vector_compare(a, b) == 1);

  ucl_vector_destructor(b);
  ucl_vector_destructor(a);

  /* range equal */

  ucl_vector_initialise(a, sizeof(int));
  ucl_vector_constructor(a);
  ucl_vector_initialise(b, sizeof(int));
  ucl_vector_constructor(b);

  ucl_vector_set_compar(a, compar);

  ucl_range_set_min_size(range_a, 5, 5);
  ucl_range_set_min_size(range_b, 2, 5);

  fill(a, 10, 0);
  fill(b, 10, 3);

  assert( ucl_vector_range_is_valid(a, range_a) );
  assert( ucl_vector_range_is_valid(b, range_b) );
  assert( ucl_vector_compare_range(a, range_a, b, range_b) == 0);

  ucl_vector_destructor(b);
  ucl_vector_destructor(a);

  /* range greater */

  ucl_vector_initialise(a, sizeof(int));
  ucl_vector_constructor(a);
  ucl_vector_initialise(b, sizeof(int));
  ucl_vector_constructor(b);

  ucl_vector_set_compar(a, compar);

  ucl_range_set_min_size(range_a, 5, 5); /* the first is 5*/
  ucl_range_set_min_size(range_b, 1, 5); /* the first is 4 */

  fill(a, 10, 0);
  fill(b, 10, 3);

  assert( ucl_vector_range_is_valid(a, range_a) );
  assert( ucl_vector_range_is_valid(b, range_b) );
  assert( ucl_vector_compare_range(a, range_a, b, range_b) == 1);

  ucl_vector_destructor(b);
  ucl_vector_destructor(a);

  /* range lesser */

  ucl_vector_initialise(a, sizeof(int));
  ucl_vector_constructor(a);
  ucl_vector_initialise(b, sizeof(int));
  ucl_vector_constructor(b);

  ucl_vector_set_compar(a, compar);

  ucl_range_set_min_size(range_a, 5, 5); /* the first is 5*/
  ucl_range_set_min_size(range_b, 3, 5); /* the first is 6 */

  fill(a, 10, 0);
  fill(b, 10, 3);

  assert( ucl_vector_range_is_valid(a, range_a) );
  assert( ucl_vector_range_is_valid(b, range_b) );
  assert( ucl_vector_compare_range(a, range_a, b, range_b) == -1);

  ucl_vector_destructor(b);
  ucl_vector_destructor(a);


}

/* end of file */
