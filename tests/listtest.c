/* listtest.c --
   
   Part of: Useless Containers Library
   Contents: main file for list tests
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
   
   $Id: listtest.c,v 1.1.1.2 2003/12/10 16:55:52 marco Exp $
*/

#include "listtest.h"

int
main (int argc, const char *const argv[])
{
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);
  exit(EXIT_SUCCESS);
}

void
fill (ucl_list_t *listPtr, int number, int first)
{
  int		    i;
  ucl_list_link_t * linkPtr;
  ucl_value_t       val;

  for (i=0; i < number; ++i)
    {
      linkPtr = alloc_link();
      val.integer = i + first;
      ucl_list_setval(linkPtr, val);
      ucl_list_pushback(listPtr, linkPtr);
    }
}

ucl_list_link_t *
alloc_link(void)
{
  ucl_list_link_t *linkPtr;

  linkPtr = (ucl_list_link_t *) 
    malloc(sizeof(ucl_list_link_t) + sizeof(int));
  assert(linkPtr);
  return linkPtr;
}

void
clean_list(ucl_list_t *listPtr)
{
  ucl_list_link_t *linkPtr;

  linkPtr = ucl_list_front(listPtr);
  while (linkPtr != NULL)
    {
      ucl_list_popfront(listPtr);
      free(linkPtr);
      linkPtr = ucl_list_front(listPtr);
    }
}



/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
