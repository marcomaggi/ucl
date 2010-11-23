/*
  Part of: Useless Containers Library
  Contents: heap test
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
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#define MCL_DEBUGGING		0
#include "mcl-test.h"
#include "debug.h"
#include "ucl.h"

#define NUMBER	1000
#define DELTA	234


/** --------------------------------------------------------------------
 ** Nodes and comparison.
 ** ----------------------------------------------------------------- */

typedef struct {
  ucl_node_tag_t  node;
  int             value;
} node_tag_t;

typedef node_tag_t *    node_t;

static int
compare_nodes_fun (ucl_value_t D UCL_UNUSED, const ucl_value_t a, const ucl_value_t b)
{
  node_t        A = a.pointer;
  node_t        B = b.pointer;
  return ((A->value == B->value)? 0 : ((A->value > B->value)? 1 : -1));
}

static const ucl_comparison_t compare_nodes = {
  .data = { .t_unsigned_long = 0 },
  .func = compare_nodes_fun
};


/** --------------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------------- */

static void
fill (ucl_heap_t H, size_t number, size_t first)
{
  node_t	N;
  for (size_t i=0; i < number; ++i) {
    N = ucl_malloc(ucl_memory_allocator, sizeof(node_tag_t));
    N->value = i + first;
    ucl_heap_insert(H, N);
  }
}
static void
clean (ucl_heap_t H)
{
  ucl_node_t	N;
  while (ucl_heap_size(H)) {
    N = ucl_heap_extract(H);
    ucl_free(ucl_memory_allocator, N);
  }
}

static void
test_construction_and_destruction (void)
{
  mcl_test_begin("heap-1.1", "construction and destruction") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int_pointer);
    {
      assert(0 == ucl_heap_size(H));
    }
    clean(H);
  }
  mcl_test_end();

  mcl_test_begin("heap-1.2", "insertions and extractions") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int_pointer);
    {
      assert(0 == ucl_heap_size(H));
      fill(H, NUMBER, 0);
      clean(H);
      assert(0 == ucl_heap_size(H));
    }
  }
  mcl_test_end();
}

static void
test_insertion_and_removal (void)
{
#define NUMBER_OF_INTEGERS	15
  static int lst1[NUMBER_OF_INTEGERS] = {
    /* there are all the integers in the inclusive range 0-14 */
    4, 7, 2, 3, 0, 8, 6, 1, 5, 9, 10, 11, 12, 13, 14
  };
  mcl_test_begin("heap-2.1", "insertion and removal") {
    ucl_heap_t	H;
    node_t	N;
    int		j;
    ucl_heap_initialise(H, ucl_compare_int_pointer);
    {
      assert(0 == ucl_heap_size(H));
      for (j=0; j<NUMBER_OF_INTEGERS; ++j) {
	N = ucl_malloc(ucl_memory_allocator, sizeof(node_t));
	N->value = lst1[j];
	mcl_debug("%p, val %d: %d", (void *)N, j, N->value);
	ucl_heap_insert(H, N);
	mcl_debug("%p, val %d: %d", (void *)N, j, N->value);
      }
      j=0;
      while (ucl_heap_size(H)) {
	N = ucl_heap_extract(H);
	mcl_debug("%p val %d: %d", (void *)N, j, N->value);
	/* assert(N->value == j); */
	++j;
	ucl_free(ucl_memory_allocator, N);
      }
      assert(j == NUMBER_OF_INTEGERS);
      assert(0 == ucl_heap_size(H));
    }
  }
  mcl_test_end();
}

static void
test_more_insertion_and_removal (void)
{
  static int lst1[NUMBER] = { 4, 7, 2, 3, 0, 8, 6, 1, 5, 9, 10, 11, 12, 13, 14 };
  static int lst2[NUMBER] = { 0, 2, 3, 4, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
  mcl_test_begin("heap-2.2", "more insertion and removal") {
    ucl_heap_t	H;
    node_t	N;
    int		j;
    ucl_heap_initialise(H, ucl_compare_int_pointer);
    {
      for (j=0; j < 6; ++j) {
	N = ucl_malloc(ucl_memory_allocator, sizeof(node_t));
	N->value = lst1[j];
	ucl_heap_insert(H, N);
      }
      for (j = 0; j < 4; ++j) {
	N = ucl_heap_extract(H);
	assert(N->value == lst2[j]);
	ucl_free(ucl_memory_allocator, N);
      }
      for (j=6; j < NUMBER; ++j) {
	N = ucl_malloc(ucl_memory_allocator, sizeof(node_t));
	N->value = lst1[j];
	ucl_heap_insert(H, N);
      }
      for (j = 4; j < NUMBER; ++j) {
	N = ucl_heap_extract(H);
	assert(N->value == lst2[j]);
	ucl_free(ucl_memory_allocator, N);
      }
      assert(0 == ucl_heap_size(H));
    }
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("heap tests");

  mcl_test_subtitle("allocation, construction and destruction");
  test_construction_and_destruction ();

  mcl_test_subtitle("insertion and removal");
  test_insertion_and_removal ();
  test_more_insertion_and_removal ();

  exit(EXIT_SUCCESS);
}

/* end of file */
