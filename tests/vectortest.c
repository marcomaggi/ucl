/* vectortest.c --
   
   Part of: Useless Containers Library
   Contents: main file for vector tests
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
   
   $Id: vectortest.c,v 1.1.1.2 2003/12/10 13:55:43 marco Exp $
*/

#include "vectortest.h"

int
main (int argc, const char *const argv[])
{
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);
  exit(EXIT_SUCCESS);
}

void
fill (ucl_vector_t *vect_p, int number, int first)
{
  int		i;
  int *	ptr;

  for (i=0; i < number; ++i)
    {
      assert( ucl_vector_enlarge(vect_p) );

      ptr = ucl_vector_newindex(vect_p, ucl_vector_size(vect_p));
      ptr = ucl_vector_insert(vect_p, ptr);
      *ptr = i + first;
    }
}

void
clean (ucl_vector_t *vect_p)
{
  int *	ptr;

  while(ucl_vector_size(vect_p))
    {
      ptr = ucl_vector_index(vect_p, 0);
      ucl_vector_erase(vect_p, ptr);
      ptr = ucl_vector_restrict(vect_p);
      assert(ptr);
    }
}

int
intcmp (const int *a, const int *b)
{
  return ((*a == *b)? 0 : ((*a > *b)? 1 : -1));
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
