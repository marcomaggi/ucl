/* stringtest.c --
   
   Part of: Useless Containers Library
   Contents: main file for string test
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
   
   $Id: stringtest.c,v 1.1.1.1 2003/12/10 17:10:08 marco Exp $
*/


#include "stringtest.h"

int
main (int argc, const char *const argv[])
{
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);
  exit(EXIT_SUCCESS);
}

void
fill (ucl_string_t *strPtr, int number)
{
  int		i;
  char	c='a';
  
  for (i=0; i < number; ++i, ++c)
    {
      strPtr = ucl_string_enlarge(strPtr);
      assert(strPtr);
      ucl_string_pushback(strPtr, c);
    }
}


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
