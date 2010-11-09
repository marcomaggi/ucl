/* map-3_8.c --
   
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

#include "maptest.h"

void
test (void)
{
  ucl_map_t		map;
  int			size;
  ucl_map_link_t *	link_p;
  ucl_value_t		key, val;
  int			i, j;
  ucl_comparison_t	compar = { { .ptr = NULL}, ucl_intcmp };


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
      key.integer = i;

      link_p = ucl_map_find_or_next(map, key);
      assert(link_p != NULL);
      
      key = ucl_map_getkey(link_p);
      assert(key.integer == i);
      val = ucl_map_getval(link_p);
      assert(val.integer == 9);

      for (j=0; j<9; ++j)
	{
	  link_p = ucl_map_prev(link_p);
	  assert(link_p);

	  key = ucl_map_getkey(link_p);
	  assert(key.integer == i);
	  val = ucl_map_getval(link_p);
	  assert(val.integer == 8-j);
	}
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