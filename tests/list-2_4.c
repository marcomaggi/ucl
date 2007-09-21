/* list-2_4.c --
   
   Part of: Useless Containers Library
   Contents: list test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: list-2_4.c,v 1.1.1.1 2003/12/10 16:59:36 marco Exp $
*/

#include "listtest.h"

void
test (void)
{
  ucl_list_t		list;
  ucl_list_t *		listPtr;
  int			i, j;
  ucl_list_link_t *	linkPtr;
  ucl_value_t		val;

  listPtr = &list;

  ucl_list_constructor(listPtr);

  fill(listPtr, NUMBER, DELTA);

  for (i=0; i<NUMBER; ++i)
    {
      linkPtr = ucl_list_front(listPtr);
      val = ucl_list_getval(linkPtr);
      assert( val.integer == i + DELTA );

      ucl_list_popfront(listPtr);
      free(linkPtr);
      assert(ucl_list_size(listPtr) == (NUMBER-1)-i);

      for (j=0; j < NUMBER-i-1; ++j)
	{
	  linkPtr = ucl_list_index(listPtr, j);
	  assert(linkPtr != NULL);

	  val = ucl_list_getval(linkPtr);
	  assert(val.integer == j+i+DELTA+1);
	}
    }
      
  assert(ucl_list_size(listPtr) == 0);

  clean_list(listPtr);
  ucl_list_destructor(listPtr);
}

/* end of file */
/*
  Local Variables:
  mode: c
  page-delimiter: "^$"
  End:
*/
