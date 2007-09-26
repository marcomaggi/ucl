/* heap-2_2.c --
   
   Part of: Useless Containers Library
   Contents: heap test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.
   
*/

#include "heaptest.h"

#undef NUMBER
#define NUMBER 15

void
test (void)
{
  ucl_heap_t		heap;
  int			i;
  ucl_heap_node_t *	node_p;
  ucl_value_t		val;
  ucl_valcmp_t		compar = { NULL, ucl_intcmp };

  static int lst1[NUMBER] = { 4, 7, 2, 3, 0, 8, 6, 1, 5, 9, 10, 11, 12, 13, 14 };
  static int lst2[NUMBER] = { 0, 2, 3, 4, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

  
  ucl_heap_constructor(heap, compar);

  for (i=0; i < 6; ++i)
    {
      node_p = NULL;
      ucl_memory_alloc(NULL, &node_p, sizeof(ucl_heap_node_t));
      val.integer = lst1[i];
      ucl_heap_setval(node_p, val);
      ucl_heap_insert(heap, node_p);
    }
  for (i = 0; i < 4; ++i)
    {
      node_p = ucl_heap_extract(heap);
      val = ucl_heap_getval(node_p);
      assert(val.integer == lst2[i]);

      memset(node_p, '\0', sizeof(ucl_heap_node_t));
      free(node_p);
    }

  for (i=6; i < NUMBER; ++i)
    {
      node_p = NULL;
      ucl_memory_alloc(NULL, &node_p, sizeof(ucl_heap_node_t));
      val.integer = lst1[i];
      ucl_heap_setval(node_p, val);
      ucl_heap_insert(heap, node_p);
    }
  for (i = 4; i < NUMBER; ++i)
    {
      node_p = ucl_heap_extract(heap);
      val = ucl_heap_getval(node_p);
      /*
	printf("---------------%d (%d)\n", val.integer, lst2[i]);
      */
      assert(val.integer == lst2[i]);

      memset(node_p, '\0', sizeof(ucl_heap_node_t));
      ucl_memory_alloc(NULL, &node_p, 0);
    }

  assert( ucl_heap_size(heap) == 0 );

  ucl_heap_destructor(heap);
}

/* end of file */
