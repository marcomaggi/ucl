/* list-2_1.c --
   
   Part of: Useless Containers Library
   Contents: list test
   Date: Wed Dec 10, 2003
   
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

#include "listtest.h"

void
test (void)
{
  ucl_list_t		list;
  int			i;
  ucl_list_link_t *	link_p;
  ucl_value_t		val;


  ucl_list_constructor(list);

  for (i=0; i < NUMBER; ++i)
    {
      link_p = alloc_link();
      val.integer = i;
      ucl_list_setval(link_p, val);

      ucl_list_pushback(list, link_p);

      assert(ucl_list_size(list) == (size_t)(i+1));

      link_p = ucl_list_back(list);
      assert(link_p != NULL);
      val = ucl_list_getval(link_p);
      assert(val.integer == i);
    }

  assert( ucl_list_size(list) == NUMBER );

  for (i=0; i < NUMBER; ++i)
    {
      link_p = ucl_list_index(list, i);
      assert(link_p != NULL);
      val = ucl_list_getval(link_p);
      assert(val.integer == i);
    }

  clean_list(list);
  assert( ucl_list_size(list) == 0 );

  ucl_list_destructor(list);

}

/* end of file */
