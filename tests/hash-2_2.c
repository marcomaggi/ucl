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

#define UCL_DEBUGGING		0
#include "hashtest.h"

#undef NUMBER
#define NUMBER			100
#undef HALF_NUMBER
#define HALF_NUMBER		(NUMBER/2)

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions: insertiona and removal.
 ** ----------------------------------------------------------*/

static void
insert_elements_in_range (ucl_hash_t table, int begin, int end)
{
  ucl_hash_entry_t *	entry_p;
  ucl_value_t		key, val;


  for (int i=begin; i<end; ++i)
    {
      entry_p = alloc_link();
      assert(entry_p);
	  
      key.num = i;
      val.num = i;
      ucl_hash_setkey(entry_p, key);
      ucl_hash_setval(entry_p, val);
      ucl_hash_insert(table, entry_p);
    }
}
static void
extract_elements_in_range (ucl_hash_t table, int begin, int end)
{
  ucl_hash_entry_t *	entry_p;
  ucl_value_t		key;


  for (int i=begin; i<end; ++i)
    {
      key.num = i;
      entry_p = ucl_hash_find(table, key);
      assert(entry_p);
      ucl_hash_extract(table, entry_p);
      free(entry_p);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions: finding.
 ** ----------------------------------------------------------*/

static void
find_elements (ucl_hash_t table, int upper_limit)
{
  ucl_hash_entry_t *	entry_p;
  ucl_value_t		key, val;


  ucl_debug("finding elements from 0 to %d", upper_limit);
  for (int i=0; i<upper_limit; ++i)
    {
      key.num = i;
      entry_p = ucl_hash_find(table, key);
#if (UCL_DEBUGGING == 1)
      if (! entry_p)
	ucl_debug("not found %d", i);
#endif
      assert(entry_p);
      key = ucl_hash_getkey(entry_p);
      val = ucl_hash_getval(entry_p);
      assert(key.num == i);
      assert(val.num == i);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Test.
 ** ----------------------------------------------------------*/

void
test (void)
{
  ucl_hash_t 		table;
  ucl_hash_entry_t *	entry_p;
  int			j, beg, end;
  ucl_hashcmp_t		key_hash_function = {
    .data = { .ptr = NULL},
    .func = hash_num
  };
  ucl_valcmp_t		key_comparison_function = {
    .data = { .ptr = NULL},
    .func = ucl_intcmp
  };


  ucl_hash_initialise(table, key_comparison_function, key_hash_function);
  ucl_hash_constructor(table);

/*   ucl_debug_off(); */

  /* ------------------------------------------------------------ */

  ucl_debug("testing enlarging");
  for (j=1; j<NUMBER; ++j)
    {
      beg = (j-1) * NUMBER;
      end = j     * NUMBER;
      ucl_debug("inserting elements in [%d, %d)", beg, end);
      insert_elements_in_range(table, beg, end);

      assert(ucl_hash_size(table) == (size_t)end);

      find_elements(table, end); /* find the elements BEFORE rehashing */
      ucl_hash_enlarge(table); 
      find_elements(table, end); /* find the elements AFTER rehashing */
    }

  /* ------------------------------------------------------------ */

  ucl_debug("\n\n\ntesting restriction");
  ucl_debug_on();
  for (j=(NUMBER-1); j>1; --j)
    {
      beg = (j-1) * NUMBER;
      end = j     * NUMBER;
      ucl_debug("extracting elements in [%d, %d)", beg, end);
      extract_elements_in_range(table, beg, end);

      assert(ucl_hash_size(table) == (size_t)beg);

      find_elements(table, beg-1); /* find the elements BEFORE rehashing */
      ucl_hash_restrict(table); 
      find_elements(table, beg-1); /* find the elements AFTER rehashing */
    }
  extract_elements_in_range(table, 0, NUMBER);
  assert(ucl_hash_size(table) == 0);
  ucl_hash_restrict(table); 
  assert(ucl_hash_size(table) == 0);

  /* ------------------------------------------------------------ */

  ucl_debug("testing enlarging/restricting");
  insert_elements_in_range(table, 0, 1000);
  assert(ucl_hash_size(table) == 1000);

  find_elements(table, 100); /* find the elements BEFORE rehashing */
  ucl_hash_enlarge(table); 
  find_elements(table, 100); /* find the elements AFTER rehashing */

  extract_elements_in_range(table, 800, 1000);
  assert(ucl_hash_size(table) == 800);

  find_elements(table, 800); /* find the elements BEFORE rehashing */
  ucl_hash_restrict(table); 
  find_elements(table, 800); /* find the elements AFTER rehashing */

  assert(ucl_hash_size(table) == 800);

  insert_elements_in_range(table, 800, 900);
  assert(ucl_hash_size(table) == 900);

  find_elements(table, 900); /* find the elements BEFORE rehashing */
  ucl_hash_enlarge(table); 
  find_elements(table, 900); /* find the elements AFTER rehashing */

  extract_elements_in_range(table, 400, 900);
  assert(ucl_hash_size(table) == 400);

  find_elements(table, 400); /* find the elements BEFORE rehashing */
  ucl_hash_restrict(table); 
  find_elements(table, 400); /* find the elements AFTER rehashing */

  assert(ucl_hash_size(table) == 400);

  /* ------------------------------------------------------------ */
  
  /* Destroy the table. */
  while ((entry_p = ucl_hash_first(table)))
    {
      ucl_hash_extract(table, entry_p);
      free(entry_p);
    }
  ucl_hash_destructor(table);
}

/* end of file */
