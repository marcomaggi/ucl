/* strtok-8_1.c --
   
   Part of: Useless Containers Library
   Contents: string token test
   Date: Thu Dec 11, 2003
   
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
   
   $Id: strtok-8_1.c,v 1.1.1.1 2003/12/11 08:01:44 marco Exp $
*/

#include "strtoktest.h"

void
test (void)
{
  ucl_strtok_t		strTok;
  ucl_strtok_t *	strPtr;
  int			i;
  char			c;
  ucl_strtok_t		strTok1;
  ucl_strtok_t *	strPtr1;
  static char string[] = "abcdefghijklmnopqrstuvwxyz";

  strPtr1 = &strTok1;
  strPtr  = &strTok;

  ucl_strtok_constructor(strPtr, 8);
  assert(strPtr->ptr);
  assert(strPtr->size == 8);
  assert(strPtr->len  == 0);

  c = string[20-1];
  ucl_strtok_insert(strPtr, 0, c);
  assert(ucl_strtok_len(strPtr) == 1);

  c = string[1];
  ucl_strtok_insert(strPtr, 1, c);
  assert(ucl_strtok_len(strPtr) == 2);

  c = string[0];
  ucl_strtok_insert(strPtr, 2, c);
  assert(ucl_strtok_len(strPtr) == 3);
      
  for (i=3; i < 20; ++i)
    {
      c = string[i-1];
      ucl_strtok_enlarge(strPtr);
      assert(strPtr->ptr);
      ucl_strtok_insert(strPtr, 1, c);
      assert(ucl_strtok_len(strPtr) == i+1);
    }
	    
  assert(ucl_strtok_len(strPtr) == 20);
      
  for (i=0; i < 20; ++i, ++c)
    {
      c = ucl_strtok_getval(strPtr, i);
      assert(c == string[20-1-i]);
    }
	    
  ucl_strtok_destructor(strPtr);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
