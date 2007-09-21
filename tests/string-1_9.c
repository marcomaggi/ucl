/* string-1_9.c --
   
   Part of: Useless Containers Library
   Contents: string test
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
   
   $Id: string-1_9.c,v 1.1.1.1 2003/12/10 17:21:21 marco Exp $
*/

#include "stringtest.h"

void
test (void)
{
  ucl_string_t *	strPtr;
  int			i;


  strPtr = ucl_string_constructor(16);
  assert(strPtr);
  assert(strPtr->size == 16);
  assert(strPtr->len  == 0);

  fill(strPtr, 20);

  for (i=3; i<20; ++i)
    {
      ucl_string_erase(strPtr, 3);
      assert(ucl_string_len(strPtr) == (20 - 1) + 3 - i);
    }
  ucl_string_erase(strPtr, 2);
  ucl_string_erase(strPtr, 1);
  ucl_string_erase(strPtr, 0);
	    
  assert(ucl_string_len(strPtr) == 0);

  ucl_string_destructor(strPtr);
}


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
