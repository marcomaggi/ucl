/* hash-2_2.c --
   
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
   
   $Id: hash-2_2.c,v 1.1.1.1 2003/12/10 15:47:51 marco Exp $
*/

#include "hashtest.h"

void
test (void)
{
  ucl_hash_t		hash_struct;
  ucl_hash_t *		hash;
  ucl_voidptr_t *	ptr;

  ucl_hash_entry_t *	entPtr;
  ucl_value_t		val, key, val1, key1;
  int			i, j, beg, end;


  hash = &hash_struct;


  ptr = ucl_hash_constructor(hash, 10, ucl_intcmp, hash_num);
  assert(ptr);
#undef NUMBER
#define NUMBER 20

  for (j=1; j<NUMBER; ++j)
    {
      beg = (j-1) * NUMBER;
      end = j     * NUMBER;
      for (i=beg; i<end; ++i)
	{
	  entPtr = alloc_link();
	  assert(entPtr);
	  
	  key.integer = i;
	  val.integer = i;
		    
	  ucl_hash_setkey(entPtr, key);
	  ucl_hash_setval(entPtr, val);

	  ucl_hash_insert(hash, entPtr);
	  assert(ucl_hash_size(hash) == i+1);
	}

      assert(ucl_hash_size(hash) == end);
      
      for (i=beg; i<end; ++i)
	{
	  key.integer = i;
	  
	  entPtr = ucl_hash_find(hash, key);
	  assert(entPtr);
		    
	  key1 = ucl_hash_getkey(entPtr);
	  val1 = ucl_hash_getval(entPtr);
		    
	  assert(key1.integer == i);
	  assert(val1.integer == i);
	}

      ptr = ucl_hash_enlarge(hash, end);
      assert(ptr);
      
      for (i=beg; i<end; ++i)
	{
	  key.integer = i;
	  
	  entPtr = ucl_hash_find(hash, key);
	  assert(entPtr);
	  
	  key1 = ucl_hash_getkey(entPtr);
	  val1 = ucl_hash_getval(entPtr);
	  
	  assert(key1.integer == i);
	  assert(val1.integer == i);
	}
    }
  
  j = ucl_hash_size(hash);
  for (i=0; i<j; ++i)
    {
      key.integer = i;
      
      entPtr = ucl_hash_find(hash, key);
      assert(entPtr);
      
      key1 = ucl_hash_getkey(entPtr);
      val1 = ucl_hash_getval(entPtr);
      
      assert(key1.integer == i);
      assert(val1.integer == i);
      
      ucl_hash_extract(hash, entPtr);
      free(entPtr);
      assert(ucl_hash_size(hash) == j-i-1);
    }
  assert(ucl_hash_size(hash) == 0);
  
  ucl_hash_destructor(hash);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
