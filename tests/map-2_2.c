/* map-2.2.c --
   
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
   
   $Id: map-2_2.c,v 1.1.1.5 2003/12/11 10:17:04 marco Exp $
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

  /*
    int			i, j;
    ucl_map_t		map;
    ucl_map_t *		mapPtr;
    ucl_iterator_t	iterator;
    ucl_value_t	*	keyPtr;
    ucl_value_t *	valPtr;
  */

  mapPtr = &map;

#undef LITTLENUMBER
#define LITTLENUMBER 10
/*  #define DEBUGGING */

  ucl_map_constructor(mapPtr, 0, ucl_intcmp);

  
  for (i=0; i<LITTLENUMBER; ++i)
    {
      linkPtr = alloc_new_link();
      
      key.integer = i;
      val.integer = i;
      
      ucl_map_setkey(linkPtr, key);
      ucl_map_setval(linkPtr, val);
      
      ucl_map_insert(mapPtr, linkPtr);
    }

  size = ucl_map_size(mapPtr);
  assert(size == LITTLENUMBER);
  
#ifdef DEBUGGING
  /* CHECK THE TREE WITH ITERATORS */
  
  key = ucl_map_getkey(mapPtr->root);
  printf("top %d\n", key.integer);
  printf("level order ");
  for (ucl_map_iterator_levelorder(mapPtr, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next(&iterator))
    {
      linkPtr = ucl_iterator_ptr(&iterator);
      key = ucl_map_getkey(linkPtr);
      val = ucl_map_getval(linkPtr);
      printf("%d ", key.integer);
    }
  printf("\n");
  
  
  printf("inorder: ");
  i = 0;
  for (ucl_map_iterator_inorder(mapPtr, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next(&iterator))
    {
      linkPtr = iterator.iterator;
      val = ucl_map_getval(linkPtr);
      printf("%d ", val.integer);
      assert(i == val.integer);
      ++i;
    }
  printf("\n");
  assert(i == LITTLENUMBER);
#endif
  
  /* REMOVE LINKS */
  
  for (i=0; i<LITTLENUMBER; ++i)
    {	    
      key.integer = i;
      linkPtr = ucl_map_find(mapPtr, key);
      linkPtr = ucl_map_remove(mapPtr, linkPtr);

      key = ucl_map_getkey(linkPtr);
      val = ucl_map_getval(linkPtr);
      assert(key.integer == i);
      assert(val.integer == i);
      
      free(linkPtr);

#ifdef DEBUGGING
      if (mapPtr->root) {
	key = ucl_map_getkey(mapPtr->root);
	printf("top %d\n", key.integer);
      }
      printf("level order ");fflush(stdout);
      for (ucl_map_iterator_levelorder(mapPtr, &iterator);
	   ucl_iterator_more(&iterator);
	   ucl_iterator_next(&iterator))
	{
	  linkPtr = ucl_iterator_ptr(&iterator);
	  key = ucl_map_getkey(linkPtr);
	  val = ucl_map_getval(linkPtr);
	  printf("%d ", key.integer);fflush(stdout);
	}
      printf("\n");fflush(stdout);
      
      printf("inorder ");
      j = i+1;
      for (ucl_map_iterator_inorder(mapPtr, &iterator);
	   ucl_iterator_more(&iterator);
	   ucl_iterator_next(&iterator))
	{
	  linkPtr = ucl_iterator_ptr(&iterator);
	  key = ucl_map_getkey(linkPtr);
	  val = ucl_map_getval(linkPtr);
	  printf("%d ", key.integer);
	  assert(j == val.integer);
	  ++j;
	}
      printf("\n");
#endif
    }
  ucl_map_destructor(mapPtr);
}


/* end of file */
