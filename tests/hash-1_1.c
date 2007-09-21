/* hash-1_1.c --
   
   Part of: Useless Containers Library
   Contents: hash table test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: hash-1_1.c,v 1.1.1.2 2003/12/10 15:44:28 marco Exp $
*/

#include "hashtest.h"

void
test (void)
{
  ucl_hash_t		hash_struct;
  ucl_hash_t *		hash;
  ucl_voidptr_t *	ptr;

  /*
  ucl_hash_entry_t *	entPtr;
  ucl_value_t		val, key, val1, key1;
  ucl_iterator_t	iterator;
  int			i, j, beg, end;

  static const char *strs[] = {
    "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten"
  };
  */

  hash = &hash_struct;

  ptr = ucl_hash_constructor(hash, BUCKETS,
			     ucl_intcmp, ucl_hash_string);
  assert(ptr);
  ucl_hash_destructor(hash);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
