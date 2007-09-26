/* list-1_1.c --
   
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
  /*
  int			i, j;
  ucl_list_link_t *	linkPtr;
  ucl_list_link_t *	linkPtr1;
  ucl_iterator_t	iterator;
  ucl_value_t		val;
  */
  ucl_list_constructor(list);
  ucl_list_destructor(list);
}

/* end of file */
