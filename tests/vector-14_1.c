/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Sun Oct 28, 2007
   
   Abstract
   
   
   
   Copyright (c) 2007, 2008 Marco Maggi
   
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



/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#define DEBUGGING		0
#include "vectortest.h"

static int
vector_ref (ucl_vector_t V, ucl_vector_index_t position)
{
  return *((int *)ucl_vector_index_to_slot(V, position));
}

void
test (void)
{
  ucl_vector_t	target, source;
  ucl_range_t	source_range;
  ucl_vector_index_t i;
  int		j;


  /* ------------------------------------------------------------ */

  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);
  ucl_vector_initialise(source, sizeof(int));
  ucl_vector_constructor(source);

  fill(source, 10, 11);
  fill(target, 15, 1);
  

  ucl_range_set_min_size(source_range, 4, 5);
  ucl_vector_copy_range(target, 3, source, source_range);


  for (i=0, j=1; i<3; ++i, ++j)
    assert(j == vector_ref(target, i));

  for (i=3, j=(11+4); i<(3+5); ++i, ++j)
    {
      debug("j %d, %d", j, vector_ref(target, i));
      assert(j == vector_ref(target, i));
    }

  for (i=(3+5), j=(3+5+1); i<15; ++i, ++j)
    assert(j == vector_ref(target, i));

  ucl_vector_destructor(target);
  ucl_vector_destructor(source);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);
  ucl_vector_initialise(source, sizeof(int));
  ucl_vector_constructor(source);

  fill(source, 10, 11);
  fill(target, 15, 1);
  

  ucl_range_set_min_size(source_range, 0, 5);
  ucl_vector_copy_range(target, 3, source, source_range);


  for (i=0, j=1; i<3; ++i, ++j)
    assert(j == vector_ref(target, i));

  for (i=3, j=11; i<(3+5); ++i, ++j)
    {
      debug("j %d, %d", j, vector_ref(target, i));
      assert(j == vector_ref(target, i));
    }

  for (i=(3+5), j=(3+5+1); i<15; ++i, ++j)
    assert(j == vector_ref(target, i));


  ucl_vector_destructor(target);
  ucl_vector_destructor(source);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);
  ucl_vector_initialise(source, sizeof(int));
  ucl_vector_constructor(source);

  fill(source, 10, 11);
  fill(target, 15, 1);
  

  ucl_range_set_min_size(source_range, 5, 5);
  ucl_vector_copy_range(target, 10, source, source_range);


  for (i=0, j=1; i<10; ++i, ++j)
    assert(j == vector_ref(target, i));

  for (i=10, j=(11+5); i<(10+5); ++i, ++j)
    {
      debug("j %d, %d", j, vector_ref(target, i));
      assert(j == vector_ref(target, i));
    }

  ucl_vector_destructor(target);
  ucl_vector_destructor(source);
}

/* end of file */
