/* circular-2_1.c --
   
   Part of: Useless Containers Library
   Contents: circular test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.
   
   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/

#define DEBUGGING		0
#include "circulartest.h"

void
test (void)
{
  ucl_circular_t	circ;
  ucl_circular_link_t *	link_p;
  int			i;
  ucl_value_t 		val;


  debug("enter");
  ucl_circular_constructor(circ);

  fill(circ, NUMBER, 0);

  for (i=0; i<NUMBER; ++i)
    {
      ucl_circular_forward(circ, 1);
      link_p = ucl_circular_current(circ);
      val     = ucl_circular_getval(link_p);

      assert(val.integer == i);
    }

  for (i=0; i<NUMBER; ++i)
    {
      ucl_circular_forward(circ, 1);
      link_p = ucl_circular_current(circ);
      val     = ucl_circular_getval(link_p);
      assert(val.integer == i);
    }
  
  clean_circular(circ);
	    
  ucl_circular_destructor(circ);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
