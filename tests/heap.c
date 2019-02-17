/*
  Part of: Useless Containers Library
  Contents: heap test
  Date: Wed Dec 10, 2003

  Abstract



  Copyright (c) 2003-2005, 2008, 2010, 2015, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#define MCL_DEBUGGING		0
#include "ucl.h"
#include "ucl-debug.h"
#include "mcl-test.h"

#define NUMBER		1000
#define LITTLENUMBER	10

static void print_preorder_links (ucl_heap_t H, const char * message, ...);



/** --------------------------------------------------------------------
 ** Link type definitions.
 ** ----------------------------------------------------------------- */

typedef struct link_tag_t {
  ucl_node_tag_t	node;
  ucl_value_t		key;
} link_tag_t;

typedef link_tag_t *	link_t;

static ucl_value_t
link_key (ucl_value_t context UCL_UNUSED, void * L_)
{
  link_t	L = L_;
  return L->key;
}

static const ucl_node_getkey_t getkey = {
  .data = { .pointer = NULL },
  .func	= link_key
};

/* to be used as right-side in assignments */
#define DAD_OF(L)		((void *)(((link_t)(L))->node.dad))
#define SON_OF(L)		((void *)(((link_t)(L))->node.son))
#define BRO_OF(L)		((void *)(((link_t)(L))->node.bro))

/* to be used as left-side in assignments */
#define DAD(L)			(((link_t)(L))->node.dad)
#define SON(L)			(((link_t)(L))->node.son)
#define BRO(L)			(((link_t)(L))->node.bro)

#define KEY(L)			(((link_t)(L))->key.t_int)
#define MAYBE_KEY(L)		((L)? KEY(L) : -1)


/** --------------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------------- */

static link_t
alloc_link (int value)
{
  ucl_memory_allocator_t A = ucl_memory_allocator;
  link_t 	L = NULL;
  A.alloc(A.data, &L, sizeof(link_tag_t));
  L->key.t_int = L->node.meta.t_int = value;
  return L;
}
static void
free_link (link_t  L)
{
  ucl_memory_allocator_t A = ucl_memory_allocator;
  A.alloc(A.data, &L, 0);
}
static void
fill (ucl_heap_t H, int first, int upper, int step)
{
  for (int j=first; j<upper; j+=step)
    ucl_heap_insert(H, alloc_link(j));
}
static void
fill_reverse (ucl_heap_t H, int first, int upper, int step)
{
  for (int j=first; j>upper; j-=step)
    ucl_heap_insert(H, alloc_link(j));
}
static void
clean (ucl_heap_t H)
{
  link_t	L;
  int		prev_key=-1;
  mcl_debug("cleaning heap with %d nodes", ucl_heap_size(H));
  for (L = ucl_heap_extract(H); L; L = ucl_heap_extract(H)) {
#if 0
    fprintf(stderr, "extracted key %d\n", KEY(L));
    print_preorder_links(H, "after extraction of %d", KEY(L));
#endif
    mcl_test_error_if_false(prev_key < KEY(L),
			    "extracted key %d is not greater than previous %d", KEY(L), prev_key);
    prev_key = KEY(L);
    free_link(L);
  }
}

/** --------------------------------------------------------------------
 ** Printing functions.
 ** ----------------------------------------------------------------- */

static void UCL_UNUSED
print_preorder_links (ucl_heap_t H, const char * message, ...)
{
  va_list		ap;
  ucl_iterator_t	I;
  link_t		L, dad, son, bro;
  int			i;
  fprintf(stderr, "*** ");
  va_start(ap, message);
  {
    vfprintf(stderr, message, ap);
  }
  va_end(ap);
  fprintf(stderr, ": preorder links (%ld nodes)\n", ucl_heap_size(H));
  for (ucl_btree_iterator_preorder(I, H->root), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
    L = ucl_iterator_ptr(I);
    dad = ucl_btree_ref_dad(L);
    son = ucl_btree_ref_son(L);
    bro = ucl_btree_ref_bro(L);

    if (dad)
      fprintf(stderr, "%2d <-", dad->key.t_int);
    else
      fprintf(stderr, "nn <-");

    fprintf(stderr, "%2d -> ", L->key.t_int);

    if (son)
      fprintf(stderr, "%2d, ", son->key.t_int);
    else
      fprintf(stderr, "nn, ");

    if (bro)
      fprintf(stderr, "%2d", bro->key.t_int);
    else
      fprintf(stderr, "nn");

    fprintf(stderr, "\t(%p)\n", (void *)L);
  }
}


static void
test_construction_and_destruction (void)
{
  mcl_test_begin("heap-1.1", "construction and destruction") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after initialisation");
    }
    clean(H);
  }
  mcl_test_end();

  mcl_test_begin("heap-1.2", "small number of insertions and extractions") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after initialisation");
      fill(H, 0, LITTLENUMBER, 1);
      mcl_test_error_if_false(LITTLENUMBER == ucl_heap_size(H),
			      "invalid size after filling, expected %d got %d",
			      LITTLENUMBER, ucl_heap_size(H));
#if 0
      print_preorder_links(H, "after filling");
#endif
      clean(H);
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after cleaning");
    }
  }
  mcl_test_end();

  mcl_test_begin("heap-1.3", "small number of insertions and extractions, reverse") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after initialisation");
      fill_reverse(H, LITTLENUMBER, 0, 1);
      mcl_test_error_if_false(LITTLENUMBER == ucl_heap_size(H),
			      "invalid size after filling, expected %d got %d",
			      LITTLENUMBER, ucl_heap_size(H));
#if 0
      print_preorder_links(H, "after filling");
#endif
      clean(H);
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after cleaning");
    }
  }
  mcl_test_end();

  mcl_test_begin("heap-1.4", "big number of insertions and extractions") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after initialisation");
      fill(H, 0, NUMBER, 1);
      mcl_test_error_if_false(NUMBER == ucl_heap_size(H),
			      "invalid size after filling, expected %d got %d",
			      NUMBER, ucl_heap_size(H));
      clean(H);
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after cleaning");
    }
  }
  mcl_test_end();

  mcl_test_begin("heap-1.5", "big number of insertions and extractions, reverse") {
    ucl_heap_t	H;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after initialisation");
      fill_reverse(H, NUMBER, 0, 1);
      mcl_test_error_if_false(NUMBER == ucl_heap_size(H),
			      "invalid size after filling, expected %d got %d",
			      NUMBER, ucl_heap_size(H));
      clean(H);
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after cleaning");
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
    link_t	N;
    int		j;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after initialisation");
      for (j=0; j<NUMBER_OF_INTEGERS; ++j) {
	N = alloc_link(lst1[j]);
	ucl_heap_insert(H, N);
      }
      j=0;
      while (ucl_heap_size(H)) {
	N = ucl_heap_extract(H);
	mcl_test_error_if_false(N->key.t_int == j,
				"invalid extracted key, expected %d got %d", j, N->key.t_int);
	++j;
	free_link(N);
      }
      mcl_test_error_if_false(j == NUMBER_OF_INTEGERS,
			      "invalid index count, expected %d got %d", NUMBER_OF_INTEGERS, j);
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after cleaning");
    }
  }
  mcl_test_end();
}

static void
test_more_insertion_and_removal (void)
{
#undef SIZE
#define SIZE	15
  static int lst1[SIZE] = { 4, 7, 2, 3, 0, 8, 6, 1, 5, 9, 10, 11, 12, 13, 14 };
  static int lst2[SIZE] = { 0, 2, 3, 4, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
  mcl_test_begin("heap-2.2", "more insertion and removal") {
    ucl_heap_t	H;
    link_t	N;
    int		j;
    ucl_heap_initialise(H, ucl_compare_int, getkey);
    {
      for (j=0; j < 6; ++j) {
	mcl_debug("inserting %d elm: %d", j, lst1[j]);
#if 0
	print_preorder_links(H, "before inserting %d elm: %d", j, lst1[j]);
#endif
	N = alloc_link(lst1[j]);
	ucl_heap_insert(H, N);
      }
      for (j=0; j < 4; ++j) {
	mcl_debug("extracting %d elm: %d", j, lst2[j]);
	N = ucl_heap_extract(H);
	mcl_test_error_if_false(N->key.t_int == lst2[j],
				"wrong extracted key, expected %d got %d", lst2[j], N->key.t_int);
	free_link(N);
      }
      for (j=6; j < SIZE; ++j) {
	mcl_debug("inserting %d elm: %d", j, lst1[j]);
#if 0
	print_preorder_links(H, "before inserting %d elm: %d", j, lst1[j]);
#endif
	N = alloc_link(lst1[j]);
	ucl_heap_insert(H, N);
      }
      for (j=4; j < SIZE; ++j) {
	mcl_debug("extracting %d elm: %d", j, lst2[j]);
	N = ucl_heap_extract(H);
	mcl_test_error_if_false(N->key.t_int == lst2[j],
				"wrong extracted key, expected %d got %d", lst2[j], N->key.t_int);
	free_link(N);
      }
      mcl_test_error_if_false(0 == ucl_heap_size(H), "invalid size after cleaning");
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
