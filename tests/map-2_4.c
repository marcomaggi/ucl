/* map-2.4.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This  is free  software; you  can redistribute  it and/or  modify it
   under  the  terms  of  the  GNU Lesser  General  Public  License  as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.
   
   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.
   
   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA
   
   $Id: map-2_4.c,v 1.1.1.5 2003/12/11 10:18:43 marco Exp $
*/

#include "maptest.h"


void
test (void)
{
  ucl_map_t		map;
  ucl_map_t *		mapPtr;
  int			size;
  ucl_map_link_t *	linkPtr;
  ucl_value_t		key, val;
  int			i;
  ucl_iterator_t	iterator;

  /*
    int			i, j;
    ucl_map_t		map;
    ucl_map_t *		mapPtr;
    ucl_value_t	*	keyPtr;
    ucl_value_t *	valPtr;
  */

  int array[]   = { 10, 13, 5, 3, 8, 7 };
  int inorder[] = { 3, 5, 7, 8, 10, 13 };


  mapPtr = &map;
/*
#define DEBUGGING
*/
#undef LITTLENUMBER
#define LITTLENUMBER 6
  
  ucl_map_constructor(mapPtr, 0, ucl_intcmp);
  
  for (i=0; i<LITTLENUMBER; ++i)
    {
      linkPtr = alloc_new_link();
      
      key.integer = array[i];
      val.integer = array[i];
      
      ucl_map_setkey(linkPtr, key);
      ucl_map_setval(linkPtr, val);

      ucl_map_insert(mapPtr, linkPtr);

#ifdef DEBUGGING
      printf("\n");
#endif
    }
#ifdef DEBUGGING
  printf("\n");
#endif
  size = ucl_map_size(mapPtr);
  assert(size == LITTLENUMBER);
	    
/*
	    for (i=0; i<LITTLENUMBER; ++i)
	    {
		printf("find key: %d; ", i);
		key.integer = i;
		linkPtr = ucl_map_find(mapPtr, key);

		if (linkPtr != NULL)
		{
		    key = ucl_map_getkey(linkPtr);
		    val = ucl_map_getval(linkPtr);
		    printf("key: %d, val: %d\n", key.integer, val.integer);

		    if (linkPtr->node.sonPtr != NULL) {
			key = ucl_map_getkey(((UCL_Map_Link *)
                                  linkPtr->node.sonPtr));
			val = ucl_map_getval((UCL_Map_Link *) 
                                  linkPtr->node.sonPtr);
			printf("son key: %d, val: %d\n",
                                 key.integer, val.integer);
		    }
		    if (linkPtr->node.broPtr != NULL) {
			key = ucl_map_getkey((UCL_Map_Link *)linkPtr->node.broPtr);
			val = ucl_map_getval((UCL_Map_Link *)linkPtr->node.broPtr);
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
  for (ucl_map_iterator_inorder(mapPtr, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next(&iterator))
    {
      linkPtr = ucl_iterator_ptr(&iterator);
      val = ucl_map_getval(linkPtr);
      assert(inorder[i] == val.integer);
      ++i;
    }
  assert(i == LITTLENUMBER);
  
  linkPtr = ucl_map_first(mapPtr);
  while (linkPtr) {
    linkPtr = ucl_map_remove(mapPtr, linkPtr);
    free(linkPtr);
    linkPtr = ucl_map_first(mapPtr);
  }
  
  ucl_map_destructor(mapPtr);
}


/* end of file */
