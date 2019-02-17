/*
  Part of: Useless Containers Library
  Contents: hash table test
  Date: Wed Dec 10, 2003

  Abstract



  Copyright (c) 2003-2005, 2008, 2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
#define DEBUGGING		0
#include "ucl.h"
#include "ucl-debug.h"
#include "mcl-test.h"


#define BUCKETS			100
#define NUMBER			1000
#define THE_NUMBER		100
#define DELTA			234

#define DISPLAY_AVERAGE_SEARCH_DISTANCE		0

#define A	ucl_memory_allocator


/** --------------------------------------------------------------------
 ** Hash function.
 ** ----------------------------------------------------------------- */

static size_t
hash_integer_fun (ucl_value_t data UCL_UNUSED, ucl_value_t val)
{
  return val.t_size;
}
static const ucl_hash_t hash_integer = {
  .data = { .t_unsigned_long = 0 }, .func = hash_integer_fun
};


/** --------------------------------------------------------------------
 ** Entry type definitions.
 ** ----------------------------------------------------------------- */

typedef struct entry_tag_t {
  ucl_node_tag_t	node;
  ucl_value_t		key;
} entry_tag_t;

typedef entry_tag_t *	entry_t;

static ucl_value_t
entry_key (ucl_value_t context UCL_UNUSED, void * E_)
{
  entry_t	E = E_;
  return E->key;
}

static const ucl_node_getkey_t getkey = {
  .data = { .pointer = NULL },
  .func	= entry_key
};


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static entry_t
alloc_entry (void)
{
  entry_t	E = NULL;
  A.alloc(A.data, &E, sizeof(entry_tag_t));
  return E;
}
static void
free_entry (entry_t E)
{
  A.alloc(A.data, &E, 0);
}
static void
clean (ucl_hash_table_t H)
{
  entry_t  E;
  while ((E = ucl_hash_first(H))) {
    ucl_hash_extract(H, E);
    A.alloc(A.data, &E, 0);
  }
}
static void
insert_elements_in_range (ucl_hash_table_t H, size_t begin, size_t end)
{
  entry_t	E;
  ucl_value_t		K;
  for (size_t i=begin; i<end; ++i) {
    E = alloc_entry();
    assert(E);
    K.t_size = i;
    E->key = K;
    ucl_hash_insert(H, E);
  }
}
static void
extract_elements_in_range (ucl_hash_table_t H, size_t begin, size_t end)
{
  entry_t	E;
  ucl_value_t		K;
  for (size_t i=begin; i<end; ++i) {
    K.t_size = i;
    E = ucl_hash_find(H, K);
    assert(E);
    ucl_hash_extract(H, E);
    free_entry(E);
  }
}
static void
find_elements (ucl_hash_table_t H, size_t upper_limit)
{
  entry_t	E;
  ucl_value_t	K;
  debug("finding elements from 0 to %d", upper_limit);
  for (size_t i=0; i<upper_limit; ++i) {
    K.t_size = i;
    E = ucl_hash_find(H, K);
#if (DEBUGGING == 1)
    if (! E)
      debug("not found %u", i);
#endif
    assert(E);
    K = E->key;
    assert(K.t_size == i);
  }
}


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
      ucl_hash_initialise(H, V, ucl_compare_int, ucl_hash_string, getkey);
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
    entry_t		E;
    ucl_value_t		K, K1;
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      size_t	i;
      ucl_hash_initialise(H, V, ucl_compare_int, hash_integer, getkey);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      {
	assert(0 == ucl_hash_size(H));
	for (i=0; i<NUMBER; ++i) {
	  E = alloc_entry();
	  assert(E);
	  K.t_size = i;
	  E->key = K;
	  ucl_hash_insert(H, E);
	  assert(1+i == ucl_hash_size(H));
	}
	assert(NUMBER == ucl_hash_size(H));
	assert(NUMBER == i);

	for (i=0; i<NUMBER; ++i) {
	  K.t_size = i;
	  E = ucl_hash_find(H, K);
	  assert(E);
	  K1 = E->key;
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
	  E->key = K;
	  ucl_hash_insert(H, E);
	  assert(1+i == ucl_hash_size(H));
	}
	assert(NUMBER == ucl_hash_size(H));
	assert(NUMBER == i);

	for (i=0; i<NUMBER; ++i) {
	  K.t_size = i;
	  E = ucl_hash_find(H, K);
	  assert(E);
	  K1 = E->key;
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

static void
test_hashing_strings (void)
{
#define NUMBER_OF_STRINGS	10
  static const char *strs[NUMBER_OF_STRINGS] = {
    "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten"
  };
  mcl_test_begin("hash-2.2", "hashing strings") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    entry_t		E;
    ucl_value_t		K;
    size_t		j;
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      ucl_hash_initialise(H, V, ucl_compare_string, ucl_hash_string, getkey);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      assert(0 == ucl_hash_size(H));
      {
	/* insert */
	mcl_debug("inserting");
	for (j=0; j<NUMBER_OF_STRINGS; ++j) {
	  E = alloc_entry();
	  assert(E);
	  E->key.chars = (char *)strs[j];
	  ucl_hash_insert(H, E);
	  assert(1+j == ucl_hash_size(H));
	}
	assert(NUMBER_OF_STRINGS == ucl_hash_size(H));

	/* extract */
	mcl_debug("extracting");
	for (j=0; j<NUMBER_OF_STRINGS; ++j) {
	  K.chars = (char *)strs[j];
	  E = ucl_hash_find(H, K);
	  assert(E);
	  assert(E->key.chars == strs[j]);
	  ucl_hash_extract(H, E);
	  free_entry(E);
	  assert(10-j-1 == ucl_hash_size(H));
	}
	assert(0 == ucl_hash_size(H));

	/* insert */
	mcl_debug("inserting");
	for (j=0; j<10; ++j) {
	  E = alloc_entry();
	  assert(E);
	  E->key.chars = (char *)strs[j];
	  ucl_hash_insert(H, E);
	  assert(1+j == ucl_hash_size(H));
	}
	assert(NUMBER_OF_STRINGS == ucl_hash_size(H));

	/* extract */
	mcl_debug("extracting");
	for (j=0; j<NUMBER_OF_STRINGS; ++j) {
	  K.chars = (char *)strs[j];
	  E = ucl_hash_find(H, K);
	  assert(E);
	  assert(E->key.chars == strs[j]);
	  ucl_hash_extract(H, E);
	  free_entry(E);
	  assert(10-j-1 == ucl_hash_size(H));
	}
	assert(0 == ucl_hash_size(H));

	/* insert */
	mcl_debug("inserting");
	for (j=0; j<10; ++j) {
	  E = alloc_entry();
	  assert(E);
	  E->key.chars = (char *)strs[j];
	  ucl_hash_insert(H, E);
	  assert(1+j == ucl_hash_size(H));
	}
	assert(NUMBER_OF_STRINGS == ucl_hash_size(H));
      }
      clean(H);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_resizing (void)
{
  mcl_test_begin("hash-3.1", "enlarging and restricting") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    size_t		j, beg, end;
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      ucl_hash_initialise(H, V, ucl_compare_int, hash_integer, getkey);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      {
	assert(0 == ucl_hash_size(H));
	debug("testing enlarging");
	for (j=1; j<THE_NUMBER; ++j) {
	  beg = (j-1) * THE_NUMBER;
	  end = j     * THE_NUMBER;
	  debug("inserting elements in [%d, %d)", beg, end);
	  insert_elements_in_range(H, beg, end);
	  assert(ucl_hash_size(H) == (size_t)end);
	  find_elements(H, end); /* find the elements BEFORE rehashing */
	  ucl_hash_enlarge(H);
	  find_elements(H, end); /* find the elements AFTER rehashing */
	}

	debug("\n\n\ntesting restriction");
	for (j=(THE_NUMBER-1); j>1; --j) {
	  beg = (j-1) * THE_NUMBER;
	  end = j     * THE_NUMBER;
	  debug("extracting elements in [%d, %d)", beg, end);
	  extract_elements_in_range(H, beg, end);

	  assert(ucl_hash_size(H) == (size_t)beg);

	  find_elements(H, beg-1); /* find the elements BEFORE rehashing */
	  ucl_hash_restrict(H);
	  find_elements(H, beg-1); /* find the elements AFTER rehashing */
	}
	extract_elements_in_range(H, 0, THE_NUMBER);
	assert(ucl_hash_size(H) == 0);
	ucl_hash_restrict(H);
	assert(ucl_hash_size(H) == 0);
#if 0
	debug("length of chain %u: %u", 10, ucl_hash_bucket_chain_length(H, 10));
#endif

	debug("testing enlarging/restricting");
	ucl_vector_update_number_of_step_up_slots(H->buckets, 1000);
	ucl_vector_update_number_of_step_down_slots(H->buckets, 1000);
	ucl_hash_restrict(H);

	debug("before the insertion: size %u, number of buckets %u, used buckets %u",
	      ucl_hash_size(H),
	      ucl_hash_number_of_buckets(H),
	      ucl_hash_number_of_used_buckets(H));
	insert_elements_in_range(H, 0, 1000);
	assert(ucl_hash_size(H) == 1000);

#if (DISPLAY_AVERAGE_SEARCH_DISTANCE == 1)
	fprintf(stderr, "average search distance %f\n", ucl_hash_average_search_distance (H));
#endif

	debug("after the insertion: size %u, number of buckets %u, used buckets %u",
	      ucl_hash_size(H),
	      ucl_hash_number_of_buckets(H),
	      ucl_hash_number_of_used_buckets(H));

	ucl_vector_update_number_of_step_up_slots(H->buckets, 100);
	ucl_vector_update_number_of_step_down_slots(H->buckets, 100);

	find_elements(H, 1000); /* find the elements BEFORE rehashing */
	ucl_hash_enlarge(H);
	find_elements(H, 1000); /* find the elements AFTER rehashing */

	debug("after the enlarging: size %u, number of buckets %u, used buckets %u",
	      ucl_hash_size(H),
	      ucl_hash_number_of_buckets(H),
	      ucl_hash_number_of_used_buckets(H));

#if (DISPLAY_AVERAGE_SEARCH_DISTANCE == 1)
	fprintf(stderr, "average search distance %f\n", ucl_hash_average_search_distance (H));
#endif
#if 0
	debug("length of chain %u: %u",
	      10, ucl_hash_bucket_chain_length(H, 10));
#endif

	extract_elements_in_range(H, 800, 1000);
	assert(ucl_hash_size(H) == 800);

	find_elements(H, 800); /* find the elements BEFORE rehashing */
	ucl_hash_restrict(H);
	find_elements(H, 800); /* find the elements AFTER rehashing */

	debug("after the restriction: size %u, number of buckets %u, used buckets %u",
	      ucl_hash_size(H),
	      ucl_hash_number_of_buckets(H),
	      ucl_hash_number_of_used_buckets(H));

	assert(ucl_hash_size(H) == 800);

	insert_elements_in_range(H, 800, 900);
	assert(ucl_hash_size(H) == 900);

	find_elements(H, 900); /* find the elements BEFORE rehashing */
	ucl_hash_enlarge(H);
	find_elements(H, 900); /* find the elements AFTER rehashing */

	extract_elements_in_range(H, 400, 900);
	assert(ucl_hash_size(H) == 400);

	find_elements(H, 400); /* find the elements BEFORE rehashing */
	ucl_hash_restrict(H);
	find_elements(H, 400); /* find the elements AFTER rehashing */

	assert(ucl_hash_size(H) == 400);
      }
      clean(H);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_empty_iteration (void)
{
  mcl_test_begin("hash-4.1", "iterating over empty table") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    entry_t	E;
    ucl_value_t		K;
    ucl_iterator_t	I;
    size_t		j;
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      ucl_hash_initialise(H, V, ucl_compare_int, hash_integer, getkey);
      assert(0 == ucl_hash_size(H));
      {
	for (ucl_hash_iterator(H, I), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	  E = ucl_iterator_ptr(I);
	  K = E->key;
	}
	assert(0 == j);
	assert(0 == ucl_hash_size(H));
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_full_iteration (void)
{
  mcl_test_begin("hash-4.2", "iterating over all the elements") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    entry_t	E;
    ucl_value_t		K, K1;
    ucl_iterator_t	I;
    size_t		j;
    size_t		results[NUMBER];
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      ucl_hash_initialise(H, V, ucl_compare_int, hash_integer, getkey);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      {
	assert(0 == ucl_hash_size(H));
	/* fill the hash table with NUMBER elements */
	for (j=0; j<NUMBER; ++j) {
	  E = alloc_entry();
	  assert(E);
	  K.t_size = j;
	  E->key = K;
	  ucl_hash_insert(H, E);
	  assert(ucl_hash_size(H) == (j+1));
	}
	assert(ucl_hash_size(H) == NUMBER);
	assert(j == NUMBER);

	/* iterate over all the NUMBER elements */
	for (j=0; j<NUMBER; ++j) {
	  results[j] = 0;
	}
	for (ucl_hash_iterator(H, I), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	  E = ucl_iterator_ptr(I);
	  K1 = E->key;
	  results[K1.t_size] = 1;
	}
	assert(j == NUMBER);
	for (j=0; j<NUMBER; ++j) {
	  assert(1 == results[j]);
	}

	for (j=0; j<NUMBER; ++j) {
	  K.t_size = j;
	  E = ucl_hash_find(H, K);
	  assert(E);
	  K1 = E->key;
	  assert(K1.t_size == j);
	  ucl_hash_extract(H, E);
	  free_entry(E);
	  assert(ucl_hash_size(H) == (NUMBER-j-1));
	}
	assert(ucl_hash_size(H) == 0);
      }
      clean(H);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_other_iteration (void)
{ /* it is unknown why I originally wrote this test */
  mcl_test_begin("hash-4.3", "an iteration") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_hash_table_t	H;
    entry_t	E;
    ucl_value_t		K, K1;
    ucl_iterator_t	I;
    size_t		j;
    size_t		results[NUMBER];
    ucl_vector_initialise_config_hash(C);
    ucl_vector_alloc(V, C);
    {
      ucl_hash_initialise(H, V, ucl_compare_int, hash_integer, getkey);
      assert(UCL_HASH_DEFAULT_SIZE == ucl_vector_size(V));
      assert(0 == ucl_hash_size(H));

      E = alloc_entry();
      assert(E);
      K.t_size = 3;
      E->key = K;
      ucl_hash_insert(H, E);

      E = alloc_entry();
      assert(E);
      K.t_size = 10;
      E->key = K;
      ucl_hash_insert(H, E);

      E = alloc_entry();
      assert(E);
      K.t_size = 50;
      E->key = K;
      ucl_hash_insert(H, E);

      for (j=0; j<NUMBER; ++j) {
	results[j] = 0;
      }
      for (ucl_hash_iterator(H, I), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	E = ucl_iterator_ptr(I);
	K1 = E->key;
	assert(K1.t_size < NUMBER);
	results[K1.t_size] = 1;
      }
      assert(j == 3);
      assert(1 == results[3]);
      assert(1 == results[10]);
      assert(1 == results[50]);

      K.t_size = 3;
      E = ucl_hash_find(H, K);
      assert(E);
      ucl_hash_extract(H, E);
      free_entry(E);

      K.t_size = 10;
      E = ucl_hash_find(H, K);
      assert(E);
      ucl_hash_extract(H, E);
      free_entry(E);

      K.t_size = 50;
      E = ucl_hash_find(H, K);
      assert(E);
      ucl_hash_extract(H, E);
      free_entry(E);

      assert(0 == ucl_hash_size(H));
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
  test_hashing_strings ();

  mcl_test_subtitle("resizing");
  test_resizing ();

  mcl_test_subtitle("iteration");
  test_empty_iteration ();
  test_full_iteration ();
  test_other_iteration ();

  exit(EXIT_SUCCESS);
}

/* end of file */
