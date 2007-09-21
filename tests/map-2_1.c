/* map-2.1.c --
   
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
   
   $Id: map-2_1.c,v 1.1.1.5 2003/12/11 10:16:01 marco Exp $
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

  /*
    int			i, j;
    ucl_map_t		map;
    ucl_map_t *		mapPtr;
    ucl_iterator_t	iterator;
    ucl_value_t	*	keyPtr;
    ucl_value_t *	valPtr;
  */

  mapPtr = &map;


  ucl_map_constructor(mapPtr, 0, ucl_intcmp);

  linkPtr = alloc_new_link();
  ucl_map_setkey(linkPtr, (ucl_value_t) 1);
  ucl_map_setval(linkPtr, (ucl_value_t) 1);

  ucl_map_insert(mapPtr, linkPtr);

  size = ucl_map_size(mapPtr);
  assert( size == 1 );

  linkPtr = ucl_map_find(mapPtr, (ucl_value_t) 1);

  key = ucl_map_getkey(linkPtr);
  val = ucl_map_getval(linkPtr);

  assert(key.integer == 1);
  assert(val.integer == 1);

  ucl_map_remove(mapPtr, linkPtr);
  free(linkPtr);

  ucl_map_destructor(mapPtr);
}


/* end of file */
