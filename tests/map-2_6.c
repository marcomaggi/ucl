/* map-2_6.c --
   
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
  ucl_iterator_t	iterator;
  int			i;
  ucl_comparison_t		compar = { { .ptr = NULL}, ucl_intcmp };

  /*
    int			i, j;
    ucl_map_t		map;
    ucl_map_t *		map;

    ucl_value_t	*	keyPtr;
    ucl_value_t *	valPtr;
  */

#include "numbers1.h"

  ucl_map_constructor(map, 0, compar);
  
  for (i=0; i<LITTLENUMBER; ++i)
    {
      link_p = alloc_new_link();
      
      key.integer = array[i];
      val.integer = array[i];
      
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);
      
      ucl_map_insert(map, link_p);
    }
  
  size = ucl_map_size(map);
  
  assert(size == LITTLENUMBER);
  
  i = 0;
  for (ucl_map_iterator_inorder(map, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      link_p = ucl_iterator_ptr(iterator);
      val = ucl_map_getval(link_p);
      assert(i == val.integer);
      ++i;
    }
  assert(i == LITTLENUMBER);
  
  link_p = ucl_map_first(map);
  while (link_p) {
    link_p = ucl_map_remove(map, link_p);
    free(link_p);
    link_p = ucl_map_first(map);
  }
  
  ucl_map_destructor(map);
}


/* end of file */
