/* string-1_8.c --
   
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
   
   $Id: string-1_8.c,v 1.1.1.1 2003/12/10 17:20:28 marco Exp $
*/

#include "stringtest.h"

void
test (void)
{
  ucl_string_t *	strPtr;
  int			i;
  char			c;
  static char string[] = "abcdefghijklmnopqrstuvwxyz";


  strPtr = ucl_string_constructor(16);
  assert(strPtr);
  assert(strPtr->size == 16);
  assert(strPtr->len  == 0);

  c = string[20-1];
  ucl_string_insert(strPtr, 0, c);
  assert(ucl_string_len(strPtr) == 1);

  c = string[1];
  ucl_string_insert(strPtr, 1, c);
  assert(ucl_string_len(strPtr) == 2);

  c = string[0];
  ucl_string_insert(strPtr, 2, c);
  assert(ucl_string_len(strPtr) == 3);
      
  for (i=3; i < 20; ++i)
    {
      c = string[i-1];
      strPtr = ucl_string_enlarge(strPtr);
      assert(strPtr);
      ucl_string_insert(strPtr, 1, c);
      assert(ucl_string_len(strPtr) == i+1);
    }
	    
  assert(ucl_string_len(strPtr) == 20);
      
  for (i=0; i < 20; ++i, ++c)
    {
      c = ucl_string_getval(strPtr, i);
      assert(c == string[20-1-i]);
    }
	    
  ucl_string_destructor(strPtr);
}


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
