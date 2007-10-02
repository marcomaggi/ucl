/* map-3_9.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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

#include "maptest.h"

void
test (void)
{
  ucl_map_t		map;
  int			size;
  ucl_map_link_t *	link_p;
  ucl_value_t		key, val;
  int			i, j;
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
      key.integer = i;
/*        printf("key %d ", i);fflush(stdout); */

      link_p = ucl_map_find_or_prev(map, key);
      assert(link_p != NULL);
      
      key = ucl_map_getkey(link_p);
      val = ucl_map_getval(link_p);
/*        printf("key %d val %d\n", key.integer, val.integer); */
      assert(key.integer == i);
      assert(val.integer == 0);

      for (j=1; j<10; ++j)
	{
	  link_p = ucl_map_next(link_p);
	  assert(link_p);
/*  	  printf("i %d j %d ", i, j); */

	  key = ucl_map_getkey(link_p);
	  val = ucl_map_getval(link_p);
/*  	  printf("key %d val %d\n", key.integer, val.integer); */
	  assert(key.integer == i);
	  assert(val.integer == j);
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
