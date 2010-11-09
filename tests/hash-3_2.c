/* hash-3_2.c --
   
   Part of: Useless Containers Library
   Contents: hash table test
   Date: Wed Dec 10, 2003
   
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

#include "hashtest.h"

void
test (void)
{
  ucl_hash_table_tag_t 	hash[1];
  ucl_hash_entry_t *	entPtr;
  ucl_value_t		val1, key1;
  ucl_iterator_t	iterator;
  int			i;
  ucl_hash_t		key_hash_function = {
    .data = { .ptr = NULL},
    .func = hash_num
  };
  ucl_comparison_t		key_comparison_function = {
    .data = { .ptr = NULL},
    .func = ucl_intcmp
  };


  ucl_hash_initialise(hash, key_comparison_function, key_hash_function);
  ucl_hash_constructor(hash);
  
  assert(ucl_hash_size(hash) == 0);
  
  i=0;
  for (ucl_hash_iterator(hash, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator)) {
    entPtr = ucl_iterator_ptr(iterator);
    
    key1 = ucl_hash_getkey(entPtr);
    val1 = ucl_hash_getval(entPtr);
    
    assert(key1.integer < NUMBER);
    assert(val1.integer < NUMBER);
    ++i;
  }
  
  assert(i == 0);
  assert(ucl_hash_size(hash) == 0);
  
  ucl_hash_destructor(hash);
}

/* end of file */