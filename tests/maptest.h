/* maptest.h --
   
   Part of: Useless Container Library
   Contents: header file for Map tests
   Date: Sat Feb  8, 2003
   
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


#ifndef __MAPTEST_H
#define __MAPTEST_H 1

#include <stdio.h>
#include "ucl.h"
	
#define NUMBER		50
#define LITTLENUMBER	10

UCL_BEGIN_C_DECL

extern void test (void);
extern ucl_map_link_t * alloc_new_link (void);
extern void clean_map (ucl_map_t map);
extern void fill_map (ucl_map_t map, int begin, int end);

/* ------------------------------------------------------------ */

ucl_map_link_t *
alloc_new_link (void)
{
  ucl_map_link_t * link_p;
    
  link_p = (ucl_map_link_t *) malloc(sizeof(ucl_map_link_t));
  assert(link_p != NULL);
  return link_p;
}

void
fill_map (ucl_map_t map, int begin, int end)
{
  ucl_map_link_t *	link_p;
  int			i;
  ucl_value_t		key, val;


  for (i=begin; i<end; ++i)
    {
      link_p = alloc_new_link();
      assert(link_p);

      key.num = i;
      val.num = i;
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);

      ucl_map_insert(map, link_p);
    }
}

void
clean_map (ucl_map_t map)
{
  size_t		size;
  ucl_map_link_t *	link_p;


  link_p = ucl_map_first(map);

  while (link_p) {
    link_p = ucl_map_remove(map, link_p);
    free(link_p);
    link_p = ucl_map_first(map);
  }

  size = ucl_map_size(map);
  assert(size == 0);
}

UCL_END_C_DECL

#endif /* __MAPTEST_H */


/* end of file */
