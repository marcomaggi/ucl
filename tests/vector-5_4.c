/* vector-5_4.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Binary search.
   
   Copyright (c) 2005, 2008 Marco Maggi
   
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

#define DEBUGGING		0
#include "vectortest.h"

void
test (void)
{
  int		i, j;
  int *		ptr;
  ucl_vector_t	vector;
  ucl_comparison_t	compar = { { .ptr = NULL }, ucl_ptrintcmp };


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);

  fill(vector, NUMBER, DELTA);
#if 1
  for (i=0; i<NUMBER; ++i)
    {
      j = i + DELTA;
      ptr = ucl_vector_binary_search(vector, &j);
      assert(ptr != NULL);
      assert(*ptr == j);
    }
#endif
	    
  j=5*NUMBER;
  ptr = ucl_vector_binary_search(vector, &j);
  assert(ptr == NULL);

  j=-1;
  ptr = ucl_vector_binary_search(vector, &j);
  assert(ptr == NULL);

  ucl_vector_destructor(vector);
}

/* end of file */
