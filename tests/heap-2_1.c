/* heap-2_1.c --
   
   Part of: Useless Containers Library
   Contents: heap test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.
   
   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/

#include "heaptest.h"

void
test (void)
{
  ucl_heap_t		heap;
  int			i;
  ucl_heap_node_t *	node_p;
  ucl_value_t		val;
  ucl_valcmp_t		compar = { .data = { .ptr = NULL }, .func = ucl_intcmp };

  static int lst1[15] = { 4, 7, 2, 3, 0, 8, 6, 1, 5, 9, 10, 11, 12, 13, 14 };


#undef NUMBER
#define NUMBER 15
  ucl_heap_constructor(heap, compar);

  for (i=0; i < NUMBER; ++i)
    {
      node_p = NULL;
      ucl_memory_alloc(NULL, &node_p, sizeof(ucl_heap_node_t));
      val.integer = lst1[i];
      ucl_heap_setval(node_p, val);
      ucl_heap_insert(heap, node_p);
    }

  i=0;
  while (ucl_heap_size(heap))
    {
      node_p = ucl_heap_extract(heap);
      val = ucl_heap_getval(node_p);
      assert(val.integer == i);
      ++i;
      memset(node_p, '\0', sizeof(ucl_heap_node_t));
      ucl_memory_alloc(NULL, &node_p, 0);
    }
  assert(i == NUMBER);

  assert( ucl_heap_size(heap) == 0 );

  ucl_heap_destructor(heap);
}


/* end of file */
