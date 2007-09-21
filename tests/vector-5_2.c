/* vector-5_2.c --
   
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
  int			i;
  int *			ptr;
  ucl_vector_t		vector;
  ucl_vector_t *	vect_p;

  vect_p = &vector;


  assert(ucl_vector_constructor1(vect_p,8,6,10,0, sizeof(int)));
  
  for (i=0; i<NUMBER; i+=3)
    {
      ptr = ucl_vector_enlarge(vect_p);
      assert(ptr);
      ptr = ucl_vector_insertsort(vect_p, &i, intcmp);
      assert(ptr != NULL);
      *ptr = i;
    }

  for (i=1; i<NUMBER; i+=3)
    {
      ptr = ucl_vector_enlarge(vect_p);
      assert(ptr);
      ptr = ucl_vector_insertsort(vect_p, &i, intcmp);
      assert(ptr != NULL);
      *ptr = i;
    }

  for (i=2; i<NUMBER; i+=3)
    {
      ptr = ucl_vector_enlarge(vect_p);
      assert(ptr);
      ptr = ucl_vector_insertsort(vect_p, &i, (void *)intcmp);
      assert(ptr != NULL);
      *ptr = i;
    }

  for (i=0; i<NUMBER; ++i)
    {
      ptr = ucl_vector_index(vect_p, i);
      assert(ptr != NULL);
      assert(*ptr == i);
    }
	    
  ucl_vector_destructor(vect_p);
}

/* end of file */
