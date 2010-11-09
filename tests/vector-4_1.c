/* vector-4_1.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Erase.
   
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

#include "vectortest.h"

void
test (void)
{
  int			i;
  int *			ptr;
  ucl_vector_t		vector;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  fill(vector, NUMBER, DELTA);
  
  for (i=3; i<NUMBER; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, 3);
      assert(ptr != NULL);
      ucl_vector_erase(vector, ptr);

      assert(ucl_vector_size(vector) == (size_t)((NUMBER - 1) + 3 - i));
    }
  
  ptr = ucl_vector_index_to_slot(vector, 2);
  assert(ptr != NULL);
  ucl_vector_erase(vector, ptr);
  ptr = ucl_vector_index_to_slot(vector, 1);
  assert(ptr != NULL);
  ucl_vector_erase(vector, ptr);
  ptr = ucl_vector_index_to_slot(vector, 0);
  assert(ptr != NULL);
  ucl_vector_erase(vector, ptr);
	    
  assert(ucl_vector_size(vector) == 0);

  ucl_vector_destructor(vector);
}

/* end of file */
