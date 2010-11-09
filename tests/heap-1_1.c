/* heap-1_1.c --
   
   Part of: Useless Containers Library
   Contents: heap test
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

#include "heaptest.h"

void
test (void)
{
  ucl_heap_t		heap;
  ucl_comparison_t		compar = { .data = { .ptr = NULL }, .func = ucl_intcmp };
  
  ucl_heap_constructor(heap, compar);
  ucl_heap_destructor(heap);
}

/* end of file */
