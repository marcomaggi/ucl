/* maptest.c --
   
   Part of: Useless Container Library
   Contents: main file for Map tests
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
   
   $Id: maptest.c,v 1.11 2003/12/11 10:04:46 marco Exp $
*/


#include "maptest.h"

int
main (int argc, const char *const argv[])
{
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);
  exit(EXIT_SUCCESS);
}

ucl_map_link_t *
alloc_new_link (void)
{
  ucl_map_link_t * linkPtr;
    
  linkPtr = (ucl_map_link_t *) malloc(sizeof(ucl_map_link_t));
  assert(linkPtr != NULL);
  return linkPtr;
}

void
fill_map (ucl_map_t *mapPtr, int begin, int end)
{
  ucl_map_link_t *	linkPtr;
  int			i;
  ucl_value_t		key, val;


  for (i=begin; i<end; ++i)
    {
      linkPtr = alloc_new_link();
      assert(linkPtr);

      key.num = i;
      val.num = i;
      ucl_map_setkey(linkPtr, key);
      ucl_map_setval(linkPtr, val);

      ucl_map_insert(mapPtr, linkPtr);
    }
}

void
clean_map (ucl_map_t *mapPtr)
{
  size_t		size;
  ucl_map_link_t *	linkPtr;


  linkPtr = ucl_map_first(mapPtr);

  while (linkPtr) {
    linkPtr = ucl_map_remove(mapPtr, linkPtr);
    free(linkPtr);
    linkPtr = ucl_map_first(mapPtr);
  }

  size = ucl_map_size(mapPtr);
  assert(size == 0);
}


/* end of file */
