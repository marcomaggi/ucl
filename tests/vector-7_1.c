/* vector-7_1.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Iteration
   
   Copyright (c) 2005 Marco Maggi
   
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

#include "vectortest.h"

void
test (void)
{
  ucl_vector_t		vector;
  ucl_iterator_t	iterator;
  int *			p;
  int			i;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_constructor(vector);
  fill(vector, NUMBER, 0);

  for (ucl_vector_iterator_forward(vector, iterator), i=0;
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator), ++i)
    {
      p = ucl_iterator_ptr(iterator);
      assert(i == *p);
    }

  for (ucl_vector_iterator_backward(vector, iterator), i=NUMBER-1;
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator), --i)
    {
      p = ucl_iterator_ptr(iterator);
      assert(i == *p);
    }


  ucl_vector_destructor(vector);
}

/* end of file */
