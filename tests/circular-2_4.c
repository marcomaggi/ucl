/* circular-2_4.c --
   
   Part of: Useless Containers Library
   Contents: circular test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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

#include "circulartest.h"

void
test (void)
{
  ucl_circular_t	circ;
  int			i;
  ucl_circular_link_t *	link_p;
  ucl_value_t 		val;
  ucl_valcmp_t		compar = { NULL, ucl_intcmp };


  ucl_circular_constructor(circ);
  ucl_circular_set_compar(circ, compar);

#define NUMBER 20
  fill(circ, NUMBER, 0);

  /* let's go to the element holding "16" (20-4=16) */
  val.integer = i = NUMBER-4;
  link_p = ucl_circular_find(circ, val);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == i);

  /* now we extract 10 elements: from 16 to 19 (4 elms) and
     from 0 to 5 (6 elms) */
  for (i=0; i<10; ++i)
    {
      link_p = ucl_circular_extract(circ);
      free(link_p);
    }
  /* now the current element is "6" */
  
  link_p = ucl_circular_current(circ);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 6);

  /* the next element is "7", followed by unit step increments
     to "15" (=20-4-1) */
  for (i=7; i<NUMBER-4; ++i)
    {
      ucl_circular_forward(circ, 1);
      link_p = ucl_circular_current(circ);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
    }
  link_p = ucl_circular_current(circ);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 15);

  /* next is "6" */
  for (i=6; i<NUMBER-4; ++i)
    {
      ucl_circular_forward(circ, 1);
      link_p = ucl_circular_current(circ);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
    }
  link_p = ucl_circular_current(circ);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 15);

  for (i=NUMBER-5; i>=6; --i)
    {
      link_p = ucl_circular_current(circ);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
      ucl_circular_backward(circ, 1);
    }
  link_p = ucl_circular_current(circ);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 15);

  for (i=NUMBER-5; i>=6; --i)
    {
      link_p = ucl_circular_current(circ);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
      ucl_circular_backward(circ, 1);
    }
  link_p = ucl_circular_current(circ);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == NUMBER-5);
	    
  clean_circular(circ);
  ucl_circular_destructor(circ);
}

/* end of file */
