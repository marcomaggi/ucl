/* heap-2_2.c --
   
   Part of: Useless Containers Library
   Contents: heap test
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
   
   $Id: heap-2_2.c,v 1.1.1.1 2003/12/10 16:48:03 marco Exp $
*/

#include "heaptest.h"

void
test (void)
{
  ucl_heap_t		heap;
  ucl_heap_t *		heapPtr;
  int			i;
  ucl_heap_node_t *	nodePtr;
  ucl_value_t		val;

  static int lst1[15] = { 4, 7, 2, 3, 0, 8, 6, 1, 5, 9, 10, 11, 12, 13, 14 };
  static int lst2[15] = { 0, 2, 3, 4, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

  
  heapPtr = &heap;

#undef NUMBER
#define NUMBER 15
  ucl_heap_constructor(heapPtr, ucl_intcmp);

  for (i=0; i < 6; ++i)
    {
      nodePtr = alloc_link();
      val.integer = lst1[i];
      ucl_heap_setval(nodePtr, val);
      ucl_heap_insert(heapPtr, nodePtr);
    }
  for (i = 0; i < 4; ++i)
    {
      nodePtr = ucl_heap_extract(heapPtr);
      val = ucl_heap_getval(nodePtr);
      assert(val.integer == lst2[i]);

      memset(nodePtr, '\0', sizeof(ucl_heap_node_t));
      free(nodePtr);
    }

  for (i=6; i < NUMBER; ++i)
    {
      nodePtr = alloc_link();
      val.integer = lst1[i];
      ucl_heap_setval(nodePtr, val);
      ucl_heap_insert(heapPtr, nodePtr);
    }
  for (i = 4; i < NUMBER; ++i)
    {
      nodePtr = ucl_heap_extract(heapPtr);
      val = ucl_heap_getval(nodePtr);
      /*
	printf("---------------%d (%d)\n", val.integer, lst2[i]);
      */
      assert(val.integer == lst2[i]);

      memset(nodePtr, '\0', sizeof(ucl_heap_node_t));
      free(nodePtr);
    }


  assert( ucl_heap_size(heapPtr) == 0 );

  ucl_heap_destructor(heapPtr);
}


/* end of file */
/*
  Local Variables:
  mode: c
  page-delimiter: "^$"
  End:
*/
