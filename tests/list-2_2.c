/* list-2_2.c --
   
   Part of: Useless Containers Library
   Contents: list test
   Date: Wed Dec 10, 2003
   
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

#include "listtest.h"

void
test (void)
{
  ucl_list_t		list;
  int			i;
  ucl_list_link_t *	linkPtr;
  ucl_value_t		val;


  ucl_list_constructor(list);

  for (i=0; i < NUMBER; ++i)
    {
      linkPtr = alloc_link();
      val.integer = i;
      ucl_list_setval(linkPtr, val);
      ucl_list_pushfront(list, linkPtr);

      assert(ucl_list_size(list) == (size_t)(i+1));

      linkPtr = ucl_list_front(list);
      assert(linkPtr != NULL);
      val = ucl_list_getval(linkPtr);
      assert(val.integer == i);
    }
  assert( ucl_list_size(list) == NUMBER );

  for (i=0; i < NUMBER; ++i)
    {
      linkPtr = ucl_list_index(list, i);
      assert(linkPtr != NULL);

      val = ucl_list_getval(linkPtr);
      assert(i == (NUMBER - val.integer - 1));
    }
	    
  clean_list(list);
  assert( ucl_list_size(list) == 0 );

  ucl_list_destructor(list);

}

/* end of file */
