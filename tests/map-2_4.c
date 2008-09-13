/* map-2.4.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
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

#include "maptest.h"

void
test (void)
{
  ucl_map_t		map;
  int			size;
  ucl_map_link_t *	link_p;
  ucl_value_t		key, val;
  int			i;
  ucl_iterator_t	iterator;
  ucl_valcmp_t		compar = { { .ptr = NULL}, ucl_intcmp };

  /*
    int			i, j;
    ucl_map_t		map;
    ucl_map_t *		map;
    ucl_value_t	*	keyPtr;
    ucl_value_t *	valPtr;
  */

  int array[]   = { 10, 13, 5, 3, 8, 7 };
  int inorder[] = { 3, 5, 7, 8, 10, 13 };


#undef LITTLENUMBER
#define LITTLENUMBER 6
  
  ucl_map_constructor(map, 0, compar);
  
  for (i=0; i<LITTLENUMBER; ++i)
    {
      link_p = alloc_new_link();
      
      key.integer = array[i];
      val.integer = array[i];
      
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);

      ucl_map_insert(map, link_p);

#if (DEBUGGING == 1)
      printf("\n");
#endif
    }
#if (DEBUGGING == 1)
  printf("\n");
#endif
  size = ucl_map_size(map);
  assert(size == LITTLENUMBER);
	    
/*
	    for (i=0; i<LITTLENUMBER; ++i)
	    {
		printf("find key: %d; ", i);
		key.integer = i;
		link_p = ucl_map_find(map, key);

		if (link_p != NULL)
		{
		    key = ucl_map_getkey(link_p);
		    val = ucl_map_getval(link_p);
		    printf("key: %d, val: %d\n", key.integer, val.integer);

		    if (link_p->node.sonPtr != NULL) {
			key = ucl_map_getkey(((UCL_Map_Link *)
                                  link_p->node.sonPtr));
			val = ucl_map_getval((UCL_Map_Link *) 
                                  link_p->node.sonPtr);
			printf("son key: %d, val: %d\n",
                                 key.integer, val.integer);
		    }
		    if (link_p->node.broPtr != NULL) {
			key = ucl_map_getkey((UCL_Map_Link *)link_p->node.broPtr);
			val = ucl_map_getval((UCL_Map_Link *)link_p->node.broPtr);
			printf("bro key: %d, val: %d\n",
                                 key.integer, val.integer);
		    }
		    printf("\n");
		}
		else
		{
		    printf("not found %d\n", i);
		}
	    }
*/


  i = 0;
  for (ucl_map_iterator_inorder(map, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      link_p = ucl_iterator_ptr(iterator);
      val = ucl_map_getval(link_p);
      assert(inorder[i] == val.integer);
      ++i;
    }
  assert(i == LITTLENUMBER);
  
  link_p = ucl_map_first(map);
  while (link_p) {
    link_p = ucl_map_remove(map, link_p);
    free(link_p);
    link_p = ucl_map_first(map);
  }
  
  ucl_map_destructor(map);
}


/* end of file */
