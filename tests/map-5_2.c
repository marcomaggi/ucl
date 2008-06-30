/* map-5_2.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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
#include "maptest.h"

void
test (void)
{
  ucl_map_t		map;
  int			size;
  ucl_map_link_t *	link_p;
  ucl_value_t		key, val;
  int			i, j;
  ucl_iterator_t	iterator;
  ucl_valcmp_t		compar = { { .ptr = NULL}, ucl_intcmp };


  ucl_map_constructor(map, UCL_ALLOW_MULTIPLE_OBJECTS, compar);

  size = ucl_map_size(map);
  assert(size == 0);
  
  for (i=0; i<20; ++i)
    {
      key.integer = i;

      for (j=0; j<10; ++j)
	{
	  link_p = alloc_new_link();

	  val.integer = j;
	  ucl_map_setkey(link_p, key);
	  ucl_map_setval(link_p, val);

	  ucl_map_insert(map, link_p);
	}
    }
  size = ucl_map_size(map);
  assert(size == 20*10);

  for (i=0; i<20; ++i)
    {
      j = 0;

#if (DEBUGGING == 1)
      printf("search %d\n", i);
#endif
      key.integer = i;

      for (ucl_map_upper_bound(map, iterator, key);
	   ucl_iterator_more(iterator);
	   ucl_iterator_next(iterator))
	{
#if (DEBUGGING == 1)
  	  printf("iter %d\n", j);
#endif
	  link_p = ucl_iterator_ptr(iterator);
	  key = ucl_map_getkey(link_p);
#if (DEBUGGING == 1)
      	  printf("found %d\n", key.integer);
#endif
  	  assert(key.integer == i);
	  ++j;
	}
#if (DEBUGGING == 1)
      printf("end j %d\n", j);
      printf("end %d\n", i);
#endif
      assert(j == 10);
    }


  link_p = ucl_map_first(map);
  while (link_p) {
    link_p = ucl_map_remove(map, link_p);
    free(link_p);
    link_p = ucl_map_first(map);
  }

  size = ucl_map_size(map);
  assert(size == 0);

  ucl_map_destructor(map);
}


/* end of file */
