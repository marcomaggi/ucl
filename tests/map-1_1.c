/* maptest-1.1.c --
   
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
  ucl_map_t	map;
  int		size;
  ucl_valcmp_t	compar = { NULL, ucl_intcmp };

  /*
    int			i, j;
    ucl_map_t		map;
    ucl_map_t *		mapPtr;
    ucl_map_link_t *	linkPtr;
    ucl_iterator_t	iterator;
    ucl_value_t		key, val;
    ucl_value_t	*	keyPtr;
    ucl_value_t *	valPtr;
  */

  ucl_map_constructor(map, 0, compar);

  size = ucl_map_size(map);
  assert( size == 0 );
  
  ucl_map_destructor(map);
}


/* end of file */
