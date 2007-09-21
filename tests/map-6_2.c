/* map-6_2.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Mar  1, 2003
   
   Abstract
   
	Tests for the union iterator.
   
   Copyright (c) 2003 Marco Maggi
   
   This  is free  software; you  can redistribute  it and/or  modify it
   under  the  terms  of  the  GNU Lesser  General  Public  License  as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.
   
   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.
   
   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA
   
   $Id: map-6_2.c,v 1.1.1.4 2003/12/11 10:34:58 marco Exp $
*/

#include "maptest.h"

void
test (void)
{
  ucl_map_t		map1, map2;
  ucl_map_t *		mapPtr1;
  ucl_map_t *		mapPtr2;
  ucl_iterator_t	iterator, iter1, iter2;
  int			i, j;
  int			size;
  ucl_map_link_t *	linkPtr;
  ucl_value_t		key, val;



  mapPtr1 = &map1;
  mapPtr2 = &map2;

  ucl_map_constructor(mapPtr1, 0, ucl_intcmp);
  ucl_map_constructor(mapPtr2, 0, ucl_intcmp);

  size = ucl_map_size(mapPtr1);  assert(size == 0);
  size = ucl_map_size(mapPtr2);  assert(size == 0);
  

  /* concatenation 1 */

  fill_map(mapPtr1,  0, 20);
  fill_map(mapPtr2, 20, 40);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 40);

  clean_map(mapPtr1);
  clean_map(mapPtr2);

  /* concatenation 2 */

  fill_map(mapPtr1, 20, 40);
  fill_map(mapPtr2,  0, 20);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 40);

  clean_map(mapPtr1);
  clean_map(mapPtr2);


  /* full overlapping */

  fill_map(mapPtr1, 0, 30);
  fill_map(mapPtr2, 0, 30);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  j = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);

      if (j)
	{
	  ++i;
	}
      j = !j;
    }
  assert(i == 30);
  
  clean_map(mapPtr1);
  clean_map(mapPtr2);


  /* empty map 1 */

  fill_map(mapPtr2, 0, 20);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 20);

  clean_map(mapPtr1);
  clean_map(mapPtr2);


  /* empty map 2 */

  fill_map(mapPtr1, 0, 20);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);
      ++i;
    }
  assert(i == 20);

  clean_map(mapPtr1);
  clean_map(mapPtr2);


  /* empty maps */

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  ucl_map_iterator_union(&iter1, &iter2, &iterator);
  assert(ucl_iterator_more(&iterator) == 0);


  /* inclusion 1 */

  fill_map(mapPtr1, 10, 20);
  fill_map(mapPtr2, 0,  30);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  j = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);

      if (i >= 10 && i < 20)
	{
	  if (j)
	    {
	      ++i;
	    }
	  j = !j;
	}
      else
	{
	  ++i;
	}
    }
  assert(i == 30);

  clean_map(mapPtr1);
  clean_map(mapPtr2);


  /* inclusion 2 */

  fill_map(mapPtr1, 0,  30);
  fill_map(mapPtr2, 10, 20);

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  j = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);
      assert(key.integer == i);

      if (i >= 10 && i < 20)
	{
	  if (j)
	    {
	      ++i;
	    }
	  j = !j;
	}
      else
	{
	  ++i;
	}
    }
  assert(i == 30);
  
  clean_map(mapPtr1);
  clean_map(mapPtr2);


  /* intermixed values */

  for (i=0; i<30; i += 2)
    {
      linkPtr = alloc_new_link();
      assert(linkPtr);

      key.num = i;
      val.num = i;
      ucl_map_setkey(linkPtr, key);
      ucl_map_setval(linkPtr, val);

      ucl_map_insert(mapPtr1, linkPtr);
    }
  for (i=1; i<31; i += 2)
    {
      linkPtr = alloc_new_link();
      assert(linkPtr);

      key.num = i;
      val.num = i;
      ucl_map_setkey(linkPtr, key);
      ucl_map_setval(linkPtr, val);

      ucl_map_insert(mapPtr2, linkPtr);
    }

  ucl_map_iterator_inorder(mapPtr1, &iter1);
  ucl_map_iterator_inorder(mapPtr2, &iter2);

  i = 0;
  for (ucl_map_iterator_union(&iter1, &iter2, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next((ucl_iterator_t *)&iterator))
    {
      linkPtr	= ucl_iterator_ptr(&iterator);
      key	= ucl_map_getkey(linkPtr);

      assert(key.integer == i);
      ++i;
    }

  assert(i == 30);

  clean_map(mapPtr1);
  clean_map(mapPtr2);


  ucl_map_destructor(mapPtr1);
  ucl_map_destructor(mapPtr2);
}


/* end of file */
