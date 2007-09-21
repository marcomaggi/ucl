/* map-2.3.c --
   
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
   
   $Id: map-2_3.c,v 1.1.1.6 2003/12/11 10:17:50 marco Exp $
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
    


  mapPtr = &map;
/*
#define DEBUGGING
*/
#undef LITTLENUMBER
#define LITTLENUMBER NUMBER

  ucl_map_constructor(mapPtr, 0, ucl_intcmp);

  for (i=LITTLENUMBER; i>0; --i)
    {
      linkPtr = alloc_new_link();

      key.integer = i;
      val.integer = i;

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
  for (i=LITTLENUMBER; i>0; --i)
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


  i = 1;
  for (ucl_map_iterator_inorder(mapPtr, &iterator);
       ucl_iterator_more(&iterator);
       ucl_iterator_next(&iterator))
    {
      linkPtr = ucl_iterator_ptr(&iterator);
      val = ucl_map_getval(linkPtr);
      assert(i == val.integer);
      ++i;
    }
  assert(i == LITTLENUMBER+1);
  
  linkPtr = ucl_map_first(mapPtr);
  while (linkPtr) {
    linkPtr = ucl_map_remove(mapPtr, linkPtr);
    free(linkPtr);
    linkPtr = ucl_map_first(mapPtr);
  }
  
  ucl_map_destructor(mapPtr);
}


/* end of file */
