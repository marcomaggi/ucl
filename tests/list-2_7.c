/* list-2_7.c --
   
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
   
   $Id: list-2_7.c,v 1.1.1.1 2003/12/10 17:02:31 marco Exp $
*/

#include "listtest.h"

void
test (void)
{
  ucl_list_t		list;
  ucl_list_t *		listPtr;

  int			i;
  ucl_list_link_t *	linkPtr;

  listPtr = &list;

  ucl_list_constructor(listPtr);

  fill(listPtr, NUMBER, DELTA);

  for (i=3; i<NUMBER; ++i)
    {
      linkPtr = ucl_list_index(listPtr, 3);
      assert(linkPtr != NULL);
      ucl_list_extract(listPtr, linkPtr);
      free(linkPtr);
      assert(ucl_list_size(listPtr) == (NUMBER - 1) + 3 - i);
    }

  linkPtr = ucl_list_index(listPtr, 2);
  ucl_list_extract(listPtr, linkPtr);
  assert(linkPtr != NULL);
  free(linkPtr);

  linkPtr = ucl_list_index(listPtr, 1);
  ucl_list_extract(listPtr, linkPtr);
  assert(linkPtr != NULL);
  free(linkPtr);

  linkPtr = ucl_list_index(listPtr, 0);
  ucl_list_extract(listPtr, linkPtr);
  assert(linkPtr != NULL);
  free(linkPtr);
	    
  assert(ucl_list_size(listPtr) == 0);

  ucl_list_destructor(listPtr);
}

/* end of file */
/*
  Local Variables:
  mode: c
  page-delimiter: "^$"
  End:
*/
