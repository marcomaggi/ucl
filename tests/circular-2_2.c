/* circular-2_2.c --
   
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
   
   $Id: circular-2_2.c,v 1.1.1.1 2003/12/10 15:29:54 marco Exp $
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

  fill(circ_p, NUMBER, 0);

  for (i=NUMBER-1; i>=0; --i)
    {
      link_p = ucl_circular_current(circ_p);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
      ucl_circular_backward(circ_p, 1);
    }
  for (i=NUMBER-1; i>=0; --i)
    {
      link_p = ucl_circular_current(circ_p);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
      ucl_circular_backward(circ_p, 1);
    }

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
