/* vector-4_3.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004 Marco Maggi
   
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
*/

#include "vectortest.h"

void
test (void)
{
  int			i, j;
  int *			ptr;
  ucl_vector_t		vector;
  ucl_vector_t *	vect_p;

  vect_p = &vector;


  assert(ucl_vector_constructor1(vect_p,8,6,10,0, sizeof(int)));

  fill(vect_p, NUMBER+1, DELTA);

  for (i=0; i<NUMBER+1; ++i)
    {
      j = i + DELTA;
      ptr = ucl_vector_sortfind(vect_p, &j, intcmp);
      assert(ptr != NULL);
      assert(*ptr == j);
    }
	    
  ucl_vector_destructor(vect_p);
}

/* end of file */
