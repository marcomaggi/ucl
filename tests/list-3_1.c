/* list-3_1.c --
   
   Part of: Useless Containers Library
   Contents: list test
   Date: Wed Dec 10, 2003
   
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

#include "listtest.h"

void
test (void)
{
  ucl_list_t		list;
  int			i;
  ucl_list_link_t *	link_p;
  ucl_value_t		val;


  ucl_list_constructor(list);

  fill(list, NUMBER, DELTA);

  i = 0;
  for (link_p = ucl_list_front(list); link_p; link_p = ucl_list_next(link_p))
    {
      val = ucl_list_getval(link_p);
      assert(val.integer == i+DELTA);
      ++i;
    }
  assert(i == NUMBER);
	    
  clean_list(list);
  assert( ucl_list_size(list) == 0 );
	    
  ucl_list_destructor(list);
}

/* end of file */
