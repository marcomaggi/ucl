/* heaptest.h --
   
   Part of: Useless Containers Library
   Contents: header file for heap test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
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


#ifndef __HEAPTEST_H
#define __HEAPTEST_H 1


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "testmain.h"
#include "ucl.h"
	
#define NUMBER	1000
#define DELTA	234

void fill (ucl_heap_t heap, int number, int first);
void clean_heap (ucl_heap_t heap);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

void
fill (ucl_heap_t heap, int number, int first)
{
  int		    i;
  ucl_heap_node_t * node_p;
  ucl_value_t       val;

  for (i=0; i < number; ++i)
    {
      node_p = NULL;
      ucl_memory_alloc(NULL, &node_p, sizeof(ucl_heap_node_t));
      val.integer = i + first;
      ucl_heap_setval(node_p, val);
      ucl_heap_insert(heap, node_p);
    }
}
void
clean_heap (ucl_heap_t heap)
{
  ucl_heap_node_t *   node_p;

  while (ucl_heap_size(heap))
    {
      node_p = ucl_heap_extract(heap);
      ucl_memory_alloc(NULL, &node_p, 0);
    }
}

#endif /* __HEAPTEST_H */

/* end of file */
