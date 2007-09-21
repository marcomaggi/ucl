/* strtok-4_1.c --
   
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
   
   $Id: strtok-4_1.c,v 1.1.1.1 2003/12/11 07:58:11 marco Exp $
*/

#include "strtoktest.h"

void
test (void)
{
  ucl_strtok_t		strTok;
  ucl_strtok_t *	strPtr;
  int			i;
  char			c, d;
  ucl_strtok_t		strTok1;
  ucl_strtok_t *	strPtr1;


  strPtr1 = &strTok1;
  strPtr  = &strTok;

  ucl_strtok_constructor(strPtr, 16);
  assert(strPtr->ptr);
  assert(strPtr->size == 16);
  assert(strPtr->len  == 0);

  c = 'a';

  for (i=0; i < NUMBER; ++i, ++c)
    {
      if ( ucl_strtok_isfull(strPtr) ) {
	ucl_strtok_enlarge(strPtr);
	assert(strPtr->ptr != NULL);
      }

      ucl_strtok_pushfront(strPtr, c);

      assert(ucl_strtok_len(strPtr) == i+1);

      d = ucl_strtok_front(strPtr);
      assert(d == c);
    }
  assert( ucl_strtok_len(strPtr) == NUMBER );

  ucl_strtok_destructor(strPtr);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
