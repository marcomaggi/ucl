/* hash-2_2.c --
   
   Part of: Useless Containers Library
   Contents: hash table test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2007 Marco Maggi
   
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

#if (0)
#  define UCL_DEBUGGING
#endif
#include "hashtest.h"

void
test (void)
{
  ucl_hash_t 		hash;
  ucl_hash_entry_t *	entPtr;
  ucl_value_t		val, key, val1, key1;
  unsigned		i, j, beg, end;
  ucl_valcmp_t		compar = { NULL, ucl_intcmp };


  ucl_hash_constructor(hash, compar, hash_num);
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
	  
	  key.unum = i;
	  val.unum = i;
		    
	  ucl_hash_setkey(entPtr, key);
	  ucl_hash_setval(entPtr, val);

	  ucl_hash_insert(hash, entPtr);
	  assert(ucl_hash_size(hash) == i+1);
	}

      assert(ucl_hash_size(hash) == end);

      for (i=beg; i<end; ++i)
	{
	  key.unum = i;
	  
	  entPtr = ucl_hash_find(hash, key);
	  assert(entPtr);
		    
	  key1 = ucl_hash_getkey(entPtr);
	  val1 = ucl_hash_getval(entPtr);
		    
	  assert(key1.unum == i);
	  assert(val1.unum == i);
	}
      ucl_debug("post\n");      
      ucl_hash_enlarge(hash); 
      ucl_debug("post2\n");      
      for (i=beg; i<end; ++i)
	{
	  key.unum = i;
	  
	  entPtr = ucl_hash_find(hash, key);
	  assert(entPtr);
	  
	  key1 = ucl_hash_getkey(entPtr);
	  val1 = ucl_hash_getval(entPtr);
	  
	  assert(key1.unum == i);
	  assert(val1.unum == i);
	}
    }

  j = ucl_hash_size(hash);
  for (i=0; i<j; ++i)
    {
      key.unum = i;
      
      entPtr = ucl_hash_find(hash, key);
      assert(entPtr);
      
      key1 = ucl_hash_getkey(entPtr);
      val1 = ucl_hash_getval(entPtr);
      
      assert(key1.unum == i);
      assert(val1.unum == i);
      
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
