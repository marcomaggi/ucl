/* circulartest.c --
   
   Part of: Useless Containers Library
   Contents: main file for circular list test
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
   
   $Id: circulartest.c,v 1.1.1.1 2003/12/10 15:22:28 marco Exp $
*/

#include "circulartest.h"

int
main (int argc, const char *const argv[])
{
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);
  exit(EXIT_SUCCESS);
}

void
fill (ucl_circular_t *circ_p, int number, int first)
{
  int				i;
  ucl_value_t			val;
  ucl_circular_link_t *		link_p;


  for (i=first; i<number; ++i)
    {
      link_p = alloc_link();
      val.integer = i;
      ucl_circular_setval(link_p, val);
      ucl_circular_insert(circ_p, link_p);
    }
}

ucl_circular_link_t *
alloc_link(void)
{
  ucl_circular_link_t *		link_p;
  

  link_p = (ucl_circular_link_t *) 
    malloc(sizeof(ucl_circular_link_t) + sizeof(int));
  if (link_p == NULL)
    {
      printf("error allocating memoryn");
      exit(EXIT_FAILURE);
    }
  return link_p;
}

void
clean_circular (ucl_circular_t *this)
{
  ucl_circular_link_t *		link_p;
  

  while (ucl_circular_size(this))
    {
      ucl_circular_stepforw(this);
      link_p = ucl_circular_extract(this);
      free(link_p);
    }
  assert( ucl_circular_size(this) == 0 );
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
