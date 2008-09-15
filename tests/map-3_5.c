/* mat-3_5.c --
   
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
  int			i;
  ucl_comparison_t		compar = { { .ptr = NULL}, ucl_intcmp };


  ucl_map_constructor(map, 0, compar);

  size = ucl_map_size(map);
  assert(size == 0);
  
  for (i=0; i<100; i += 5)
    {
      link_p = alloc_new_link();

      key.integer = i;
      val.integer = i;
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);

      ucl_map_insert(map, link_p);
    }

  for (i=0; i<100; i += 5)
    {
      key.integer = i;
      link_p = ucl_map_find_or_next(map, key);
      key = ucl_map_getkey(link_p);
      assert(key.integer == i);
    }
  
  for (i=0; i<100; i+= 5)
    {
      key.integer = i+3;
      link_p = ucl_map_find_or_next(map, key);
      if (link_p == NULL)
	{
	  break;
	}

      key = ucl_map_getkey(link_p);
      assert(key.integer == i+5);
    }
  assert(i == 95);


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
