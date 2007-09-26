/* listtest.h --
   
   Part of: Useless Containers Library
   Contents: header file for list tests
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


#ifndef __LISTTEST_H
#define __LISTTEST_H 1

#include <stdio.h>
#include "ucl.h"
	
#define NUMBER	1000
#define DELTA	234

UCL_BEGIN_C_DECL

extern void test (void);
extern void fill (ucl_list_t list, int number, int first);
extern ucl_list_link_t * alloc_link (void);
extern void clean_list (ucl_list_t this);

/* ------------------------------------------------------------ */

void
fill (ucl_list_t list, int number, int first)
{
  int		    i;
  ucl_list_link_t * link_p;
  ucl_value_t       val;

  for (i=0; i < number; ++i)
    {
      link_p = alloc_link();
      val.integer = i + first;
      ucl_list_setval(link_p, val);
      ucl_list_pushback(list, link_p);
    }
}

ucl_list_link_t *
alloc_link(void)
{
  ucl_list_link_t *link_p;

  link_p = (ucl_list_link_t *) malloc(sizeof(ucl_list_link_t) + sizeof(int));
  assert(link_p);
  return link_p;
}

void
clean_list(ucl_list_t list)
{
  ucl_list_link_t *link_p;

  link_p = ucl_list_front(list);
  while (link_p != NULL)
    {
      ucl_list_popfront(list);
      free(link_p);
      link_p = ucl_list_front(list);
    }
}

UCL_END_C_DECL

#endif /* __LISTTEST_H */

/* end of file */
