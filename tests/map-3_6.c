/* map-3_6.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
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
   
   $Id: map-3_6.c,v 1.1.1.4 2003/12/11 10:25:41 marco Exp $
*/

#include "maptest.h"

void
test (void)
{
  ucl_map_t		map;
  ucl_map_t *		mapPtr;
  int			size;
  ucl_map_link_t *	linkPtr;
  ucl_value_t		key, val;
  int			i;


  mapPtr = &map;

  ucl_map_constructor(mapPtr, 0, ucl_intcmp);

  size = ucl_map_size(mapPtr);
  assert(size == 0);
  
  for (i=100; i>0; i -= 5)
    {
      linkPtr = alloc_new_link();

      key.integer = i;
      val.integer = i;
      ucl_map_setkey(linkPtr, key);
      ucl_map_setval(linkPtr, val);

      ucl_map_insert(mapPtr, linkPtr);
    }

  for (i=100; i>0; i -= 5)
    {
      key.integer = i;
      linkPtr = ucl_map_find_or_next(mapPtr, key);
      key = ucl_map_getkey(linkPtr);
      assert(key.integer == i);
    }
  
  for (i=100; i>0; i -= 5)
    {
      key.integer = i-3;
      linkPtr = ucl_map_find_or_prev(mapPtr, key);
      if (linkPtr == NULL)
	{
	  break;
	}

      key = ucl_map_getkey(linkPtr);
      assert(key.integer == i-5);
    }
  assert(i == 5);


  linkPtr = ucl_map_first(mapPtr);
  while (linkPtr) {
    linkPtr = ucl_map_remove(mapPtr, linkPtr);
    free(linkPtr);
    linkPtr = ucl_map_first(mapPtr);
  }

  size = ucl_map_size(mapPtr);
  assert(size == 0);

  ucl_map_destructor(mapPtr);
}


/* end of file */
