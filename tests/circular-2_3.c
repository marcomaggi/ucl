/* circular-2_3.c --
   
   Part of: Useless Containers Library
   Contents: circular test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
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

#include "circulartest.h"

void
test (void)
{
  ucl_circular_t	circ;
  int			i;
  ucl_circular_link_t *	link_p;
  ucl_value_t 		val;
  ucl_valcmp_t		compar = { { .ptr = NULL }, ucl_intcmp };


  ucl_circular_constructor(circ);
  ucl_circular_set_compar(circ, compar);

  fill(circ, NUMBER, 0);

  val.integer = i = 10;
  link_p = ucl_circular_find(circ, val);
  assert(link_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == i);

  val.integer = i = NUMBER-1;
  link_p = ucl_circular_find(circ, val);
  assert(link_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == i);

  val.integer = i = 0;
  link_p = ucl_circular_find(circ, val);
  assert(link_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == i);

  val.integer = i = 4;
  link_p = ucl_circular_find(circ, val);
  assert(link_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == 4);

  val.integer = i = NUMBER-1;
  link_p = ucl_circular_find(circ, val);
  assert(link_p);
  val     = ucl_circular_getval(link_p);
  assert(val.integer == i);

  clean_circular(circ);
  ucl_circular_destructor(circ);
}

/* end of file */
