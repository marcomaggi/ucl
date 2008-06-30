/* hash-4_1.c --
   
   Part of: Useless Containers Library
   Contents: hash table test
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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
  ucl_hash_t 		hash;
  ucl_hash_entry_t *	entry_p;
  ucl_value_t		val, key, val1, key1;
  unsigned		i;
  ucl_hashcmp_t		key_hash_function = {
    .data = { .ptr = NULL},
    .func = ucl_hash_string
  };
  ucl_valcmp_t		key_comparison_function = {
    .data = { .ptr = NULL},
    .func = ucl_strcmp
  };


  static const char *strs[] = {
    "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten"
  };


  ucl_hash_initialise(hash, key_comparison_function, key_hash_function);
  ucl_hash_constructor(hash);
  assert(ucl_hash_size(hash) == 0);

  /* insert */

  for (i=0; i<10; ++i)
    {
      entry_p = alloc_link();
      assert(entry_p);

      key.str = (char *)strs[i];
      val.str = (char *)strs[i];

      ucl_hash_setkey(entry_p, key);
      ucl_hash_setval(entry_p, val);

      ucl_hash_insert(hash, entry_p);

      assert(ucl_hash_size(hash) == i+1);
    }
  assert(ucl_hash_size(hash) == 10);

  /* extract */

  for (i=0; i<10; ++i)
    {
      key.str = (char *)strs[i];

      entry_p = ucl_hash_find(hash, key);
      assert(entry_p);

      key1 = ucl_hash_getkey(entry_p);
      val1 = ucl_hash_getval(entry_p);

      assert(key1.str == strs[i]);
      assert(val1.str == strs[i]);

      ucl_hash_extract(hash, entry_p);
      free(entry_p);

      assert(ucl_hash_size(hash) == 10-i-1);
    }
  assert(ucl_hash_size(hash) == 0);

  /* insert */

  for (i=0; i<10; ++i)
    {
      entry_p = alloc_link();
      assert(entry_p);

      key.str = (char *)strs[i];
      val.str = (char *)strs[i];

      ucl_hash_setkey(entry_p, key);
      ucl_hash_setval(entry_p, val);

      ucl_hash_insert(hash, entry_p);

      assert(ucl_hash_size(hash) == i+1);
    }
  assert(ucl_hash_size(hash) == 10);

  /* extract */

  for (i=0; i<10; ++i)
    {
      key.str = (char *)strs[i];
      entry_p = ucl_hash_find(hash, key);
      assert(entry_p);

      key1 = ucl_hash_getkey(entry_p);
      val1 = ucl_hash_getval(entry_p);

      assert(key1.str == strs[i]);
      assert(val1.str == strs[i]);

      ucl_hash_extract(hash, entry_p);
      free(entry_p);

      assert(ucl_hash_size(hash) == 10-i-1);
    }

  assert(ucl_hash_size(hash) == 0);

  /* insert */

  for (i=0; i<10; ++i)
    {
      entry_p = alloc_link();
      assert(entry_p);

      key.str = (char *)strs[i];
      val.str = (char *)strs[i];

      ucl_hash_setkey(entry_p, key);
      ucl_hash_setval(entry_p, val);

      ucl_hash_insert(hash, entry_p);

      assert(ucl_hash_size(hash) == i+1);
    }
  assert(ucl_hash_size(hash) == 10);

  /* destroy */

  while ((entry_p = ucl_hash_first(hash)))
    {
      ucl_hash_extract(hash, entry_p);
      free(entry_p);
    }

  ucl_hash_destructor(hash);
}

/* end of file */
