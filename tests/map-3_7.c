/* map-3_7.c --
   
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
   
   $Id: map-3_7.c,v 1.1.1.5 2003/12/11 10:26:18 marco Exp $
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
  int			i, j;


  mapPtr = &map;

  ucl_map_constructor(mapPtr, UCL_ALLOW_MULTIPLE_OBJECTS, ucl_intcmp);

  size = ucl_map_size(mapPtr);
  assert(size == 0);
  
  for (i=0; i<20; ++i)
    {
      key.integer = i;

      for (j=0; j<20; ++j)
	{
	  linkPtr = alloc_new_link();

	  val.integer = i;
	  ucl_map_setkey(linkPtr, key);
	  ucl_map_setval(linkPtr, val);

	  ucl_map_insert(mapPtr, linkPtr);
	}
    }

  size = ucl_map_size(mapPtr);
  assert(size == 20*20);

  key.integer = 4;
  linkPtr = ucl_map_find(mapPtr, key);
  key = ucl_map_getkey(linkPtr);
  assert(key.integer == 4);

  for (i=0; i<9; ++i)
    {
      linkPtr = ucl_map_next(linkPtr);
      key = ucl_map_getkey(linkPtr);
      assert(key.integer == 4);
    }

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
