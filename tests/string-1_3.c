/* string-1_3.c --
   
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
   
   $Id: string-1_3.c,v 1.1.1.1 2003/12/10 17:16:11 marco Exp $
*/

#include "stringtest.h"

void
test (void)
{
  ucl_string_t *	strPtr;
  int			i;
  char			c, d;


  strPtr = ucl_string_constructor(16);
  assert(strPtr);
  assert(strPtr->size == 16);
  assert(strPtr->len  == 0);

  c = 'a';

  for (i=0; i < NUMBER; ++i, ++c)
    {
      if ( ucl_string_isfull(strPtr) ) {
	strPtr = ucl_string_enlarge(strPtr);
	assert(strPtr != NULL);
      }

      ucl_string_pushback(strPtr, c);

      assert(ucl_string_len(strPtr) == i+1);
      d = ucl_string_back(strPtr);

      assert(d == c);
    }
  assert( ucl_string_len(strPtr) == NUMBER );

  c = 'a';

  for (i=0; i < NUMBER; ++i, ++c)
    {
      d = ucl_string_getval(strPtr, i);
      assert(c == d);
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
