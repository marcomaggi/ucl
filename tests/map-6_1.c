/*   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Mar  1, 2003
   
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
  ucl_map_t		map1, map2;
  ucl_iterator_t	iterator, iter1, iter2;
  int			i;
  int			size;
  ucl_map_link_t *	link_p;
  ucl_value_t		key, val;
  ucl_comparison_t	compar = { { .ptr = NULL}, ucl_intcmp };


  ucl_map_constructor(map1, 0, compar);
  ucl_map_constructor(map2, 0, compar);

  size = ucl_map_size(map1);  assert(size == 0);
  size = ucl_map_size(map2);  assert(size == 0);
  

  /* fifteen elements in common (5-19) */

  fill_map(map1, 0, 20);
  fill_map(map2, 5, 25);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  i = 5;
  for (ucl_map_iterator_intersection(iter1, iter2, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      link_p	= ucl_iterator_ptr(iterator);
      key	= ucl_map_getkey(link_p);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 20);
  
  clean_map(map1);
  clean_map(map2);


  /* null intersection */

  fill_map(map1, 0, 10);
  fill_map(map2, 20, 25);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  ucl_map_iterator_intersection(iter1, iter2, iterator);
  assert(ucl_iterator_more(iterator) == 0);
  
  clean_map(map1);
  clean_map(map2);


  /* full intersection */

  fill_map(map1, 0, 20);
  fill_map(map2, 0, 20);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  i = 0;
  for (ucl_map_iterator_intersection(iter1, iter2, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      link_p	= ucl_iterator_ptr(iterator);
      key	= ucl_map_getkey(link_p);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 20);

  clean_map(map1);
  clean_map(map2);


  /* inclusion 1 */

  fill_map(map1, 10, 20);
  fill_map(map2, 0,  30);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  i = 10;
  for (ucl_map_iterator_intersection(iter1, iter2, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      link_p	= ucl_iterator_ptr(iterator);
      key	= ucl_map_getkey(link_p);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 20);

  clean_map(map1);
  clean_map(map2);


  /* inclusion 2 */

  fill_map(map1, 0,  30);
  fill_map(map2, 10, 20);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  i = 10;
  for (ucl_map_iterator_intersection(iter1, iter2, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      link_p	= ucl_iterator_ptr(iterator);
      key	= ucl_map_getkey(link_p);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 20);
  
  clean_map(map1);
  clean_map(map2);


  /* intermixed values, null intersection */

  for (i=0; i<30; i += 2)
    {
      link_p = alloc_new_link();
      assert(link_p);

      key.num = i;
      val.num = i;
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);

      ucl_map_insert(map1, link_p);
    }
  for (i=1; i<31; i += 2)
    {
      link_p = alloc_new_link();
      assert(link_p);

      key.num = i;
      val.num = i;
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);

      ucl_map_insert(map2, link_p);
    }

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  i = 0;
  ucl_map_iterator_intersection(iter1, iter2, iterator);
  assert(ucl_iterator_more(iterator) == 0);
  
  clean_map(map1);
  clean_map(map2);



  /* empty map 1 */

  fill_map(map2, 0, 20);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  ucl_map_iterator_intersection(iter1, iter2, iterator);
  assert(ucl_iterator_more(iterator) == 0);

  clean_map(map1);
  clean_map(map2);


  /* empty map 2 */

  fill_map(map1, 0, 20);

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  ucl_map_iterator_intersection(iter1, iter2, iterator);
  assert(ucl_iterator_more(iterator) == 0);

  clean_map(map1);
  clean_map(map2);


  /* empty maps */

  ucl_map_iterator_inorder(map1, iter1);
  ucl_map_iterator_inorder(map2, iter2);

  i = 0;
  ucl_map_iterator_intersection(iter1, iter2, iterator);
  assert(ucl_iterator_more(iterator) == 0);

  ucl_map_destructor(map1);
  ucl_map_destructor(map2);
}


/* end of file */
