/* vector-8_6.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Fri Oct 26, 2007
   
   Abstract
   
	Range insertion.
   
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

#define UCL_DEBUGGING		0
#include "vectortest.h"

#define NUMBER_OF_NUMBERS	5
#define SIZE_OF_VECTOR		10

static int
vector_ref (ucl_vector_t V, ucl_vector_index_t position)
{
  return *((int *) ucl_vector_index_to_slot(V, position));
}
static void
initialise (ucl_vector_t V)
{
  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_initialise_size(V, 15);
  ucl_vector_initialise_pad(V, 5);
  ucl_vector_constructor(V);

  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (int i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
}

void
test (void)
{
  ucl_vector_t	T, S;
  ucl_range_t	range;
  int		i, j;


  /* ------------------------------------------------------------ */

  initialise(T);
  initialise(S);

  ucl_range_set_min_size(range, 3, 5);
  ucl_vector_insert_range(T, 5, S, range);
  assert(ucl_vector_size(T) == 15);

  for (i=0; i<5; ++i)
    {
      assert(i == vector_ref(T, i));
    }
  for (i=5, j=3; i<10; ++i, ++j)
    {
      assert(j == vector_ref(T, i));
    }
  for (i=10, j=5; i<15; ++i, ++j)
    {
      assert(j == vector_ref(T, i));
    }


  ucl_vector_destructor(T);
  ucl_vector_destructor(S);

  /* ------------------------------------------------------------ */

}

/* end of file */
