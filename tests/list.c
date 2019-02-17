/*
  Part of: Useless Containers Library
  Contents: list test
  Date: Wed Dec 10, 2003

  Abstract



  Copyright (c) 2003-2005, 2008, 2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#define BIG_NUMBER		1000
#define NUMBER			10

#define VAL(node)		(((ucl_node_t)(node))->meta.t_int)


/** --------------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------------- */

static ucl_node_t
alloc_node (int value)
{
  ucl_node_t	N = ucl_malloc(ucl_memory_allocator, sizeof(ucl_node_tag_t));
  VAL(N) = value;
  return N;
}
static void
free_node (ucl_node_t N)
{
  ucl_free(ucl_memory_allocator, N);
}
static ucl_node_t
fill (int number_of_elements, int first)
{
  ucl_node_t	R, N, M;
  int		i;
  R = alloc_node(first);
  for (i=1, N = R; i<number_of_elements; ++i) {
    M = alloc_node(i + first);
    ucl_list_set_cdr(N, M);
    N = M;
  }
  return R;
}
static void
clean (ucl_node_t N)
{
  ucl_node_t	M = NULL;
  for (int i=0; N; N = M, ++i) {
    M = ucl_list_cdr(N);
    free_node(N);
  }
}
static void UCL_UNUSED
print_list (ucl_node_t N)
{
  ucl_node_t	M = NULL;
  for (; N; N = M) {
    M = ucl_list_cdr(N);
    fprintf(stderr, "%d, ", VAL(N));
  }
  fprintf(stderr, "\n");
}

/** --------------------------------------------------------------------
 ** Callbacks.
 ** ----------------------------------------------------------------- */

/* negation callback to be used in mapping */
static ucl_value_t
neg_fun (ucl_value_t dummy UCL_UNUSED, va_list ap)
{
  ucl_node_t	R = va_arg(ap, ucl_node_t);
  ucl_node_t	O = va_arg(ap, ucl_node_t);
  VAL(R) = - VAL(O);
  return ucl_value_null;
}
static const ucl_callback_t neg = {
  .data = { .pointer = NULL },
  .func = neg_fun
};

/* negation callback to be used in for each */
static ucl_value_t
neg1_fun (ucl_value_t dummy UCL_UNUSED, va_list ap)
{
  ucl_node_t	N = va_arg(ap, ucl_node_t);
  VAL(N) = - VAL(N);
  return ucl_value_null;
}
static const ucl_callback_t neg1 = {
  .data = { .pointer = NULL },
  .func = neg1_fun
};


static void
test_construction_and_destruction (void)
{
  mcl_test_begin("list-1.1", "chaining a list") {
    ucl_node_t	R, N, M;
    int		i;
    R = alloc_node(0);
    for (i=1, N = R; i<BIG_NUMBER; ++i) {
      M = alloc_node(i);
      ucl_list_set_cdr(N, M);
      N = M;
    }
    ucl_list_set_car(N, NULL);
    assert(BIG_NUMBER == ucl_list_length(R));
    for (i=0, N=R; N; N = M, ++i) {
      /* mcl_debug("%d, val %d", i, VAL(N)); */
      assert(i == VAL(N));
      M = ucl_list_cdr(N);
      free_node(N);
    }
    assert(BIG_NUMBER == i);
  }
  mcl_test_end();

  mcl_test_begin("list-1.2", "setting null car") {
    ucl_node_t	P, Q = NULL;
    P = alloc_node(0);
    {
      ucl_list_set_car(P, Q);
      assert(NULL == ucl_list_car(P));
    }
    free_node(P);
  }
  mcl_test_end();

  mcl_test_begin("list-1.3", "setting null cdr") {
    ucl_node_t	P, Q = NULL;
    P = alloc_node(0);
    {
      ucl_list_set_cdr(P, Q);
      assert(NULL == ucl_list_cdr(P));
    }
    free_node(P);
  }
  mcl_test_end();

  mcl_test_begin("list-1.4", "setting null dad, from car") {
    ucl_node_t	P, Q = NULL;
    P = alloc_node(0);
    {
      ucl_list_set_car(Q, P);
      assert(NULL == ucl_btree_ref_dad(P));
    }
    free_node(P);
  }
  mcl_test_end();

  mcl_test_begin("list-1.5", "setting null dad, from bro") {
    ucl_node_t	P, Q = NULL;
    P = alloc_node(0);
    {
      ucl_list_set_cdr(Q, P);
      assert(NULL == ucl_list_prev(P));
    }
    free_node(P);
  }
  mcl_test_end();
}

static void
test_visiting_first_and_last (void)
{
  mcl_test_begin("list-2.1", "visiting first and last") {
    ucl_node_t	R = fill(NUMBER, 0);
    ucl_node_t	N;
    {
      N = ucl_list_first(R);
      assert(0 == VAL(N));

      N = ucl_list_last(R);
      assert(NUMBER-1 == VAL(N));

      N = ucl_list_last(N);
      assert(NUMBER-1 == VAL(N));

      N = ucl_list_first(N);
      assert(0 == VAL(N));
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_visiting_positions (void)
{
  mcl_test_begin("list-2.2", "visiting by position") {
    ucl_node_t	R = fill(NUMBER, 0);
    ucl_node_t	N;
    int		j;
    {
      for (j=0; j<NUMBER; ++j) {
	N = ucl_list_ref(R, j);
	assert(j == VAL(N));
      }
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_removing_single_elements (void)
{
  mcl_test_begin("list-3.1", "removing single elements") {
    ucl_node_t	R = fill(NUMBER, 0);
    ucl_node_t	N;
    {
      N = ucl_list_ref(R, 4);
      ucl_list_remove(N);
      /* print_list(R); */
      assert(0 == VAL(ucl_list_ref(R, 0)));
      assert(1 == VAL(ucl_list_ref(R, 1)));
      assert(2 == VAL(ucl_list_ref(R, 2)));
      assert(3 == VAL(ucl_list_ref(R, 3)));
      assert(5 == VAL(ucl_list_ref(R, 4)));
      assert(6 == VAL(ucl_list_ref(R, 5)));
      assert(7 == VAL(ucl_list_ref(R, 6)));
      assert(8 == VAL(ucl_list_ref(R, 7)));
      assert(9 == VAL(ucl_list_ref(R, 8)));

      N = ucl_list_ref(R, 6-1);
      ucl_list_remove(N);
      assert(0 == VAL(ucl_list_ref(R, 0)));
      assert(1 == VAL(ucl_list_ref(R, 1)));
      assert(2 == VAL(ucl_list_ref(R, 2)));
      assert(3 == VAL(ucl_list_ref(R, 3)));
      assert(5 == VAL(ucl_list_ref(R, 4)));
      assert(7 == VAL(ucl_list_ref(R, 5)));
      assert(8 == VAL(ucl_list_ref(R, 6)));
      assert(9 == VAL(ucl_list_ref(R, 7)));

      N = ucl_list_ref(R, 8-2);
      ucl_list_remove(N);
      assert(0 == VAL(ucl_list_ref(R, 0)));
      assert(1 == VAL(ucl_list_ref(R, 1)));
      assert(2 == VAL(ucl_list_ref(R, 2)));
      assert(3 == VAL(ucl_list_ref(R, 3)));
      assert(5 == VAL(ucl_list_ref(R, 4)));
      assert(7 == VAL(ucl_list_ref(R, 5)));
      assert(9 == VAL(ucl_list_ref(R, 6)));
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_removing_front (void)
{
  mcl_test_begin("list-3.2", "removing front") {
    ucl_node_t	R = fill(NUMBER, 0);
    ucl_node_t	N;
    int		j;
    {
      N = ucl_list_popfront(R, (void **)&R);
      /* print_list(R); */
      for (j=0; j<NUMBER-1; ++j) {
	N = ucl_list_ref(R, j);
	assert(1+j == VAL(N));
      }
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_removing_back (void)
{
  mcl_test_begin("list-3.3", "removing back") {
    ucl_node_t	R = fill(NUMBER, 0);
    ucl_node_t	N;
    int		j;
    {
      N = ucl_list_popback(R);
      assert(NUMBER-1 == VAL(N));
      for (j=0; j<NUMBER-2; ++j) {
	N = ucl_list_ref(R, j);
	assert(j == VAL(N));
      }
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_reverse (void)
{
  mcl_test_begin("list-4.1", "chaining a list") {
    ucl_node_t	R = fill(NUMBER, 0);
    ucl_node_t	N;
    int		j;
    {
      for (j=0, N=R; j<NUMBER; ++j) {
	assert(j == VAL(N));
	N = ucl_list_cdr(N);
      }
      R = ucl_list_reverse(R);
      for (j=NUMBER-1, N=R; j>=0; --j) {
	mcl_debug("%d, %d", j, VAL(N));
	assert(j == VAL(N));
	N = ucl_list_cdr(N);
      }
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_length (void)
{
  mcl_test_begin("list-4.2", "computing length") {
    ucl_node_t	R = fill(NUMBER, 0);
    {
      assert(NUMBER == ucl_list_length(R));
    }
    clean(R);

    R = fill(1, 0);
    {
      assert(1 == ucl_list_length(R));
    }
    clean(R);
  }
  mcl_test_end();
}

static void
test_map (void)
{
  mcl_test_begin("list-5.1", "mapping elements") {
    ucl_node_t	result  = fill(NUMBER, 0);
    ucl_node_t	operand = fill(NUMBER, 0);
    ucl_node_t	N;
    int		j;
    {
      ucl_list_map(result, neg, operand);
      for (j=0, N=result; j<NUMBER; ++j, N = ucl_list_cdr(N)) {
	assert(-j == VAL(N));
      }
    }
    clean(result);
    clean(operand);
  }
  mcl_test_end();
}

static void
test_for_each (void)
{
  mcl_test_begin("list-5.2", "for each iteration") {
    ucl_node_t	R  = fill(NUMBER, 0);
    ucl_node_t	N;
    int		j;
    {
      ucl_list_for_each(neg1, R);
      for (j=0, N=R; j<NUMBER; ++j, N = ucl_list_cdr(N)) {
	assert(-j == VAL(N));
      }
    }
    clean(R);
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("list tests");

  mcl_test_subtitle("allocation, construction and destruction");
  test_construction_and_destruction ();

  mcl_test_subtitle("visiting");
  test_visiting_first_and_last();
  test_visiting_positions();

  mcl_test_subtitle("removal");
  test_removing_single_elements ();
  test_removing_front ();
  test_removing_back ();

  mcl_test_subtitle("operations");
  test_reverse ();
  test_length ();

  mcl_test_subtitle("mapping and for each");
  test_map ();
  test_for_each ();

  exit(EXIT_SUCCESS);
}

/* end of file */
