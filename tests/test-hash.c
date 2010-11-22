/*
  Part of: Useless Containers Library
  Contents: hash table test
  Date: Wed Dec 10, 2003

  Abstract



  Copyright (c) 2003-2005, 2008, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this  program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#define MCL_DEBUGGING		0
#include "mcl-test.h"
#include "debug.h"
#include "ucl.h"

#define BUCKETS	100
#define NUMBER 1000
#define DELTA	234

#define A	ucl_memory_allocator


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static ucl_hash_entry_t
alloc_entry (void)
{
  ucl_hash_entry_t	E = NULL;
  A.alloc(A.data, &E, sizeof(ucl_hash_entry_tag_t));
  return E;
}
static void
free_entry (ucl_hash_entry_t E)
{
  A.alloc(A.data, &E, 0);
}
static void
clean (ucl_hash_table_t H)
{
  ucl_hash_entry_t  E;
  while ((E = ucl_hash_first(H))) {
    ucl_hash_extract(H, E);
    A.alloc(A.data, &E, 0);
  }
}
static size_t
hash_integer_fun (ucl_value_t data UCL_UNUSED, ucl_value_t val)
{
  return val.t_size;
}
static const ucl_hash_t hash_integer = {
  .data = { .t_unsigned_long = 0 }, .func = hash_integer_fun
};

static void
test_construction_and_destruction (void)
{
  mcl_test_begin("hash-1.1", "construction and destruction") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      ucl_hash_initialise(H, V, ucl_compare_int, ucl_hash_string);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      {
	assert(0 == ucl_hash_size(H));
      }
      clean(H);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_usage (void)
{
  mcl_test_begin("hash-2.1", "inserting, finding, removing") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    ucl_hash_entry_t	E;
    ucl_value_t		K, K1;
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      size_t	i;
      ucl_hash_initialise(H, V, ucl_compare_int, hash_integer);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      {
	assert(0 == ucl_hash_size(H));
	for (i=0; i<NUMBER; ++i) {
	  E = alloc_entry();
	  assert(E);
	  K.t_size = i;
	  ucl_hash_entry_set_key(E, K);
	  ucl_hash_insert(H, E);
	  assert(1+i == ucl_hash_size(H));
	}
	assert(NUMBER == ucl_hash_size(H));
	assert(NUMBER == i);

	for (i=0; i<NUMBER; ++i) {
	  K.t_size = i;
	  E = ucl_hash_find(H, K);
	  assert(E);
	  K1 = ucl_hash_entry_ref_key(E);
	  assert(K1.t_size == i);
	  ucl_hash_extract(H, E);
	  free_entry(E);
	  assert((NUMBER-i-1) == ucl_hash_size(H));
	}
	assert(0 == ucl_hash_size(H));

	for (i=0; i<NUMBER; ++i) {
	  E = alloc_entry();
	  assert(E);
	  K.t_size = i;
	  ucl_hash_entry_set_key(E, K);
	  ucl_hash_insert(H, E);
	  assert(1+i == ucl_hash_size(H));
	}
	assert(NUMBER == ucl_hash_size(H));
	assert(NUMBER == i);

	for (i=0; i<NUMBER; ++i) {
	  K.t_size = i;
	  E = ucl_hash_find(H, K);
	  assert(E);
	  K1 = ucl_hash_entry_ref_key(E);
	  assert(K1.t_size == i);
	  ucl_hash_extract(H, E);
	  free_entry(E);
	  assert(ucl_hash_size(H) == NUMBER-i-1);
	}
	assert(0 == ucl_hash_size(H));
      }
      clean(H);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("hash tests");

  mcl_test_subtitle("allocation, construction and destruction");
  test_construction_and_destruction ();

  mcl_test_subtitle("usage");
  test_usage ();

  exit(EXIT_SUCCESS);
}

/* end of file */
