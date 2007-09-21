/* circular-2_4.c --
   
   Part of: Useless Containers Library
   Contents: circular test
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
   
   $Id: circular-2_4.c,v 1.1.1.1 2003/12/10 15:33:33 marco Exp $
*/

#include "circulartest.h"

void
test (void)
{
  ucl_circular_t	circ;
  ucl_circular_t *	circ_p;
  int			i;
  ucl_circular_link_t *	link_p;
  ucl_value_t 		val;


  circ_p = &circ;


  ucl_circular_constructor(circ_p);

#define NUMBER 20
  fill(circ_p, NUMBER, 0);

  /* let's go to the element holding "16" (20-4=16) */
  val.integer = i = NUMBER-4;
  link_p = ucl_circular_find(circ_p, val, ucl_intcmp);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == i);

  /* now we extract 10 elements: from 16 to 19 (4 elms) and
     from 0 to 5 (6 elms) */
  for (i=0; i<10; ++i)
    {
      link_p = ucl_circular_extract(circ_p);
      free(link_p);
    }
  /* now the current element is "6" */
  
  link_p = ucl_circular_current(circ_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 6);

  /* the next element is "7", followed by unit step increments
     to "15" (=20-4-1) */
  for (i=7; i<NUMBER-4; ++i)
    {
      ucl_circular_forward(circ_p, 1);
      link_p = ucl_circular_current(circ_p);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
    }
  link_p = ucl_circular_current(circ_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 15);

  /* next is "6" */
  for (i=6; i<NUMBER-4; ++i)
    {
      ucl_circular_forward(circ_p, 1);
      link_p = ucl_circular_current(circ_p);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
    }
  link_p = ucl_circular_current(circ_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 15);

  for (i=NUMBER-5; i>=6; --i)
    {
      link_p = ucl_circular_current(circ_p);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
      ucl_circular_backward(circ_p, 1);
    }
  link_p = ucl_circular_current(circ_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 15);

  for (i=NUMBER-5; i>=6; --i)
    {
      link_p = ucl_circular_current(circ_p);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
      ucl_circular_backward(circ_p, 1);
    }
  link_p = ucl_circular_current(circ_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == NUMBER-5);
	    
  clean_circular(circ_p);
  ucl_circular_destructor(circ_p);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
