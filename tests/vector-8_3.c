/* vector-8_3.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Append.
   
   Copyright (c) 2005 Marco Maggi
   
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
  ucl_vector_t	source;
  ucl_vector_t	target;
  int *		p;
  int		i, j;


  ucl_vector_initialise(source, sizeof(int));
  ucl_vector_constructor(source);
  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);

  fill(target, 10, 0);
  fill(source, 10, 20);
  debug("size of target %u", ucl_vector_size(target));
  debug("size of source %u", ucl_vector_size(source));

  ucl_vector_insert_vector(target, 5, source);

  debug("size of target after %u", ucl_vector_size(target));
  assert(ucl_vector_size(target) == 20);

  for (i=0, j=0; i<5; ++i, ++j)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("value %d expected %d index %d", *p, j, i);
      assert(*p == j);
    }
  for (j=20; i<15; ++i, ++j)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("value %d expected %d index %d", *p, j, i);
      assert(*p == j);
    }
  for (j=5; i<20; ++i, ++j)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("value %d expected %d index %d", *p, j, i);
      assert(*p == j);
    }

  ucl_vector_destructor(source);
  ucl_vector_destructor(target);
}

/* end of file */
