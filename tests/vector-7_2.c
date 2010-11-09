/* vector-7_2.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Oct 25, 2007
   
   Abstract
   
	Iteration
   
   Copyright (c) 2007 Marco Maggi
   
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
  ucl_vector_t		vector;
  ucl_iterator_t	iterator;
  int *			p;
  int			i;
  ucl_range_t		range;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);
  fill(vector, NUMBER, 0);
  
  {
    static const int	results[] = { 1, 2, 3, 4 };

    ucl_range_set_min_max(range, 1, 4);

    for (ucl_vector_iterator_range_forward(vector, range, iterator), i=0;
	 ucl_iterator_more(iterator);
	 ucl_iterator_next(iterator), ++i)
      {
	p = ucl_iterator_ptr(iterator);
	assert(results[i] == *p);
      }
  }

  {
    static const int	results[] = { 4, 3, 2, 1 };

    ucl_range_set_min_max(range, 1, 4);

    for (ucl_vector_iterator_range_backward(vector, range, iterator), i=0;
	 ucl_iterator_more(iterator);
	 ucl_iterator_next(iterator), ++i)
      {
	p = ucl_iterator_ptr(iterator);
	assert(results[i] == *p);
      }
  }

  ucl_vector_destructor(vector);
}

/* end of file */
