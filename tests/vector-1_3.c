/* vector-1_3.c --
   
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
   
   $Id: vector-1_3.c,v 1.1.1.1 2003/12/10 13:57:37 marco Exp $
*/

#include "vectortest.h"

void
test (void)
{
  int *			ptr;
  ucl_vector_t		vector;
  ucl_vector_t *	vect_p;

  vect_p = &vector;

  ptr = ucl_vector_constructor1(vect_p, 2, 10, 6, 0, sizeof(int));
  assert(ptr != NULL);
  
  fill(vect_p, NUMBER, 0); clean(vect_p);
  fill(vect_p, NUMBER, 0); clean(vect_p);
  fill(vect_p, NUMBER, 0); clean(vect_p);

  ucl_vector_destructor(vect_p);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
