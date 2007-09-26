/* map-2.2.c --
   
   Part of: Useless Container Library
   Contents: test for the Map container
   Date: Sat Feb  8, 2003
   
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

#include "maptest.h"

void
test (void)
{
  ucl_map_t		map;
  int			size;
  ucl_map_link_t *	link_p;
  ucl_value_t		key, val;
  int			i;
  ucl_valcmp_t		compar = { NULL, ucl_intcmp };


#undef LITTLENUMBER
#define LITTLENUMBER 10
/*  #define UCL_DEBUGGING */

  ucl_map_constructor(map, 0, compar);

  
  for (i=0; i<LITTLENUMBER; ++i)
    {
      link_p = alloc_new_link();
      
      key.integer = i;
      val.integer = i;
      
      ucl_map_setkey(link_p, key);
      ucl_map_setval(link_p, val);
      
      ucl_map_insert(map, link_p);
    }

  size = ucl_map_size(map);
  assert(size == LITTLENUMBER);
  
#ifdef UCL_DEBUGGING
  /* CHECK THE TREE WITH ITERATORS */
  {
    ucl_iterator_t	iterator;   
  
    key = ucl_map_getkey(map->root);
    printf("top %d\n", key.integer);
    printf("level order ");
    for (ucl_map_iterator_levelorder(map, &iterator);
	 ucl_iterator_more(&iterator);
	 ucl_iterator_next(&iterator))
      {
	link_p = ucl_iterator_ptr(&iterator);
	key = ucl_map_getkey(link_p);
	val = ucl_map_getval(link_p);
	printf("%d ", key.integer);
      }
    printf("\n");
  
  
    printf("inorder: ");
    i = 0;
    for (ucl_map_iterator_inorder(map, &iterator);
	 ucl_iterator_more(&iterator);
	 ucl_iterator_next(&iterator))
      {
	link_p = iterator.iterator;
	val = ucl_map_getval(link_p);
	printf("%d ", val.integer);
	assert(i == val.integer);
	++i;
      }
    printf("\n");
    assert(i == LITTLENUMBER);
  }
#endif
  
  /* REMOVE LINKS */
  
  for (i=0; i<LITTLENUMBER; ++i)
    {	    
      key.integer = i;
      link_p = ucl_map_find(map, key);
      link_p = ucl_map_remove(map, link_p);

      key = ucl_map_getkey(link_p);
      val = ucl_map_getval(link_p);
      assert(key.integer == i);
      assert(val.integer == i);
      
      free(link_p);

#ifdef UCL_DEBUGGING
      if (map->root) {
	key = ucl_map_getkey(map->root);
	printf("top %d\n", key.integer);
      }
      printf("level order ");fflush(stdout);
      for (ucl_map_iterator_levelorder(map, &iterator);
	   ucl_iterator_more(&iterator);
	   ucl_iterator_next(&iterator))
	{
	  link_p = ucl_iterator_ptr(&iterator);
	  key = ucl_map_getkey(link_p);
	  val = ucl_map_getval(link_p);
	  printf("%d ", key.integer);fflush(stdout);
	}
      printf("\n");fflush(stdout);
      
      printf("inorder ");
      j = i+1;
      for (ucl_map_iterator_inorder(map, &iterator);
	   ucl_iterator_more(&iterator);
	   ucl_iterator_next(&iterator))
	{
	  link_p = ucl_iterator_ptr(&iterator);
	  key = ucl_map_getkey(link_p);
	  val = ucl_map_getval(link_p);
	  printf("%d ", key.integer);
	  assert(j == val.integer);
	  ++j;
	}
      printf("\n");
#endif
    }
  ucl_map_destructor(map);
}

/* end of file */
