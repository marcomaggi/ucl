/* vector-2_5.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
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
   
   $Id: vector-2_5.c,v 1.1.1.2 2003/12/10 14:07:27 marco Exp $
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


  assert( ucl_vector_constructor1(vect_p, 8, 6, 10, 0, sizeof(int)) );

  assert( ucl_vector_enlarge(vect_p) );
  ptr = ucl_vector_newindex(vect_p, 0);
  assert(ptr != NULL);
  ptr = ucl_vector_insert(vect_p, ptr);
  assert(ptr != NULL);
  *ptr = NUMBER-1;

  assert(ucl_vector_size(vect_p) == 1);

  assert( ucl_vector_enlarge(vect_p) );
  ptr = ucl_vector_newindex(vect_p, 1);
  assert(ptr != NULL);
  ptr = ucl_vector_insert(vect_p, ptr);
  assert(ptr != NULL);
  *ptr = 1;

  assert(ucl_vector_size(vect_p) == 2);

  assert( ucl_vector_enlarge(vect_p) );
  ptr = ucl_vector_newindex(vect_p, 2);
  assert(ptr != NULL);
  ptr = ucl_vector_insert(vect_p, ptr);
  assert(ptr != NULL);
  *ptr = 0;

  assert(ucl_vector_size(vect_p) == 3);

  for (i=3; i < NUMBER; ++i)
    {
      assert( ucl_vector_enlarge(vect_p) );
      
      ptr = ucl_vector_newindex(vect_p, 1);
      assert(ptr != NULL);
      ptr = ucl_vector_insert(vect_p, ptr);
      assert(ptr != NULL);
      *ptr = i-1;
      
      assert(ucl_vector_size(vect_p) == i+1);
    }
	    
  assert(ucl_vector_size(vect_p) == NUMBER);
  
  for (i=0; i < NUMBER; ++i)
    {
      ptr = ucl_vector_index(vect_p, i);
      assert(ptr != NULL);
      assert(*ptr == NUMBER-1-i);
    }
  
  ucl_vector_destructor(vect_p);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
