/* maptest-1.1.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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
  ucl_map_t	map;
  int		size;
  ucl_valcmp_t	compar = { { .ptr = NULL}, ucl_intcmp };

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
