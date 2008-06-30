/* vector-3_3.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Insert sort
   
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

#define DEBUGGING			0
#include "vectortest.h"

/* #undef NUMBER */
/* #define NUMBER 20 */

void
test (void)
{
  int			i, j, k, l;
  int *			ptr;
  ucl_vector_t		vector;
  ucl_valcmp_t		compar = { { .ptr = NULL}, ucl_ptrintcmp };


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);
  ucl_vector_set_compar(vector, compar);
  
  for (j=0; j<3; ++j)
    {
      for (i=j; i<NUMBER; i+=3)
	{
	  debug("inserting %d", i);
	  ucl_vector_enlarge(vector);
	  ptr = ucl_vector_insert_sort(vector, &i);
	  assert(ptr != NULL);
	  *ptr = i;

	  ptr = ucl_vector_index_to_slot(vector, 0);
	  l = *ptr;
	  for (k=1; (size_t)k<ucl_vector_size(vector); ++k)
	    {
	      ptr = ucl_vector_index_to_slot(vector, k);
	      assert(ptr != NULL);
	      assert(l < *ptr);
	      l = *ptr;
	    }
	}
    }

  for (i=0; i<NUMBER; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, i);
      assert(ptr != NULL);
      assert(*ptr == i);
    }
	    
  ucl_vector_destructor(vector);
}

/* end of file */
