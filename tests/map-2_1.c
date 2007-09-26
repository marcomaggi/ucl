/* map-2.1.c --
   
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
  ucl_valcmp_t		compar = { NULL, ucl_intcmp };


  ucl_map_constructor(map, 0, compar);

  link_p = alloc_new_link();
  val.num = 1;
  ucl_map_setkey(link_p, val);
  ucl_map_setval(link_p, val);

  ucl_map_insert(map, link_p);

  size = ucl_map_size(map);
  assert( size == 1 );

  val.num = 1;
  link_p = ucl_map_find(map, val);

  key = ucl_map_getkey(link_p);
  val = ucl_map_getval(link_p);

  assert(key.integer == 1);
  assert(val.integer == 1);

  ucl_map_remove(map, link_p);
  free(link_p);

  ucl_map_destructor(map);
}

/* end of file */
