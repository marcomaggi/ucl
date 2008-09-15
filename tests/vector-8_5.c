/* vector-8_5.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Erase range.
   
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

void dump_vector (ucl_vector_t self, const char *title);

void
test (void)
{
  ucl_vector_t	vector;
  ucl_range_t	range;
  int *		p;
  int		i, j;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 50);
  ucl_vector_initialise_pad(vector, 35);
  ucl_vector_constructor(vector);

  fill(vector, 10, 0);

  dump_vector(vector, "before");

  ucl_range_set_min_max(range, 2, 4); /* from "2" to "4", inclusive */

  assert( ucl_vector_range_is_valid(vector, range) );
  debug("erasing range (vector slot size %d, range slot size %d)",
	    ucl_vector_size(vector), ucl_range_size(range));
  ucl_vector_erase_range(vector, range);
  debug("size after %d", ucl_vector_size(vector));
  dump_vector(vector, "after");
  assert( ucl_vector_size(vector) == 7 );

  for (i=0; i<2; ++i)
    {
      p = ucl_vector_index_to_slot(vector, i);
      debug("test value %d expected %d index %d", *p, i, i);
      assert(*p == i);
    }
  for (j=5; (size_t)i<ucl_vector_size(vector); ++i, ++j)
    {
      p = ucl_vector_index_to_slot(vector, i);
      debug("test2 value %d expected %d index %d", *p, j, i);
      assert(*p == j);
    }

  ucl_vector_destructor(vector);
}
void
dump_vector (ucl_vector_t self, const char *title UCL_UNUSED)
{
  int		i;
  uint8_t *	p;

  debug(title);
  for (i=0; (size_t)i<ucl_vector_size(self); ++i)
    {
      p = ucl_vector_index_to_slot(self, i);
      debug("dump value %d index %d", *p, i);
    }
}


/* end of file */
