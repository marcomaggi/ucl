/* vector-8_1.c --
   
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
  int		i;
  ucl_range_t	range;


  /* whole */

  ucl_vector_initialise(source, sizeof(int));
  ucl_vector_constructor(source);
  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);

  fill(target, 10, 0);
  fill(source, 10, 10);
  ucl_vector_append(target, source);

  assert(ucl_vector_size(target) == 20);

  for (i=0; i<20; ++i)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("%d %d", *p, i);
      assert(*p == i);
    }

  ucl_vector_destructor(source);
  ucl_vector_destructor(target);

  /* range */

  ucl_vector_initialise(source, sizeof(int));
  ucl_vector_constructor(source);
  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);

#define TARGET_SIZE		10
#define SOURCE_SIZE		10
#define TARGET_FIRST		0
#define SOURCE_FIRST		8
#define RANGE_SIZE		(SOURCE_SIZE-5)

  fill(target, TARGET_SIZE, TARGET_FIRST);
  fill(source, SOURCE_SIZE, SOURCE_FIRST);
  ucl_range_set_min_size(range, (TARGET_SIZE-SOURCE_FIRST), RANGE_SIZE);
  assert( ucl_vector_range_is_valid(source, range) );

#if (DEBUGGING == 1)
  debug("target vector:");
  for (i=0; i<(int)ucl_vector_size(target); ++i)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("%d %d", *p, i);
    }
#endif
#if (DEBUGGING == 1)
  debug("source vector:");
  for (i=0; i<(int)ucl_vector_size(source); ++i)
    {
      p = ucl_vector_index_to_slot(source, i);
      debug("%d %d", *p, i);
    }
#endif

  debug("target vector size before appending %d", ucl_vector_size(target));
  ucl_vector_append_range(target, source, range);
  debug("target vector size after appending %d", ucl_vector_size(target));

  assert(ucl_vector_size(target) == (TARGET_SIZE+RANGE_SIZE));

#if (DEBUGGING == 1)
  debug("target vector:");
  for (i=0; i<(int)ucl_vector_size(target); ++i)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("%d %d", *p, i);
    }
#endif
  for (i=0; i<15; ++i)
    {
      p = ucl_vector_index_to_slot(target, i);
      debug("%d %d", *p, i);
      assert(*p == i);
    }

  ucl_vector_destructor(source);
  ucl_vector_destructor(target);
}

/* end of file */