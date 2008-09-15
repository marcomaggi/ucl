/* map-3_4.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
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
  int			i;
  ucl_map_link_t *	otherPtr;
  ucl_comparison_t		compar = { { .ptr = NULL}, ucl_intcmp };


  ucl_map_constructor(map, 0, compar);

  size = ucl_map_size(map);
  assert(size == 0);
  
  for (i=0; i<NUMBER; ++i)
    {
      link_p = alloc_new_link();

      key.integer = i;
      val.integer = i;
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);

      ucl_map_insert(map, link_p);
    }

  link_p = ucl_map_last(map);
  for (i=NUMBER; i>0; --i)
    {
      otherPtr = ucl_map_prev(link_p);
      if (otherPtr == NULL)
	{
	  break;
	}

      key = ucl_map_getkey(link_p);
#if (DEBUGGING == 1)
      printf("key %d\n", key.integer);fflush(stdout);
#endif
      val = ucl_map_getkey(otherPtr);
      assert(key.integer == val.integer+1);

      link_p = otherPtr;
#if (DEBUGGING == 1)
      printf("other %p\n", link_p);fflush(stdout);
#endif
    }
  assert(i == 1);

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
