/* circulartest.h --
   
   Part of: Useless Containers Library
   Contents: main header file for circular list test
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

#ifndef __CIRCULARTEST_H
#define __CIRCULARTEST_H 1

#include <stdio.h>
#include "ucl.h"
#include "debug.h"
	
#define NUMBER	20

UCL_BEGIN_C_DECL

extern void test (void);
extern void fill (ucl_circular_t circ, int number, int first);
extern void clean_circular (ucl_circular_t this);

/* ------------------------------------------------------------ */

void
fill (ucl_circular_t circ_p, int number, int first)
{
  int				i;
  ucl_value_t			val;
  ucl_circular_link_t *		link_p;


  for (i=first; i<number; ++i)
    {
      link_p = NULL;
      ucl_memory_alloc(NULL, &link_p, sizeof(ucl_circular_link_t));
      val.integer = i;
      ucl_circular_setval(link_p, val);
      ucl_circular_insert(circ_p, link_p);
    }
}
void
clean_circular (ucl_circular_t this)
{
  ucl_circular_link_t *		link_p;
  

  while (ucl_circular_size(this))
    {
      link_p = ucl_circular_extract(this);
      ucl_memory_alloc(NULL, &link_p, 0);
    }
  assert( ucl_circular_size(this) == 0 );
}

UCL_END_C_DECL

#endif /* __CIRCULARTEST_H */

/* end of file */
