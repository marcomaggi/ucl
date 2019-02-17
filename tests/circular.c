/*
  Part of: Useless Containers Library
  Contents: circular test
  Date: Wed Dec 10, 2003

  Abstract



  Copyright (c) 2003, 2005, 2008, 2010, 2019 Marco Maggi

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
#include "ucl.h"
#include "ucl-debug.h"
#include "mcl-test.h"

#define NUMBER	20

static ucl_memory_allocator_t	A;


static void
fill (ucl_circular_t circ, int number, int first)
{
  ucl_node_t 	link;
  for (int i=first; i<number; ++i) {
    link = NULL;
    A.alloc(A.data, &link, sizeof(ucl_node_tag_t));
    link->meta.t_int = i;
    ucl_circular_insert(circ, link);
  }
}
static void
clean (ucl_circular_t circ)
{
  ucl_node_t	link;
  while (ucl_circular_size(circ)) {
    link = ucl_circular_extract(circ);
    A.alloc(A.data, &link, 0);
  }
  assert(0 == ucl_circular_size(circ));
}

static void
test_constructor (void)
{
  mcl_test_begin("circular-1.1", "construction and destruction") {
    ucl_circular_t	circ;
    ucl_circular_constructor(circ);
    {
      assert(0 == ucl_circular_size(circ));
      assert(NULL == ucl_circular_current(circ));
    }
    ucl_circular_destructor(circ);
  }
  mcl_test_end();
}

static void
test_forward_movement (void)
{
  mcl_test_begin("circular-2.1", "forward movement") {
    ucl_circular_t	circ;
    ucl_circular_constructor(circ);
    {
      fill(circ, NUMBER, 0);
      {
	ucl_node_t	link;
	for (int i=0; i<NUMBER; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	}
	for (int i=0; i<NUMBER; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	}
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
  }
  mcl_test_end();
}

static void
test_backward_movement (void)
{
  mcl_test_begin("circular-2.2", "backward movement") {
    ucl_circular_t	circ;
    ucl_circular_constructor(circ);
    {
      fill(circ, NUMBER, 0);
      {
	ucl_node_t	link;
	for (int i=NUMBER-1; i>=0; --i) {
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
	for (int i=NUMBER-1; i>=0; --i) {
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
  }
  mcl_test_end();
}

static int
compare (ucl_value_t data, ucl_value_t outer, ucl_value_t inner)
{
  inner.t_int = ((ucl_node_t)inner.pointer)->meta.t_int;
  return ucl_compare_int_fun(data, outer, inner);
}
static void
test_finding (void)
{
  mcl_test_begin("circular-3.1", "finding elements") {
    ucl_circular_t	circ;
    ucl_comparison_t	compar = { .data = { .pointer = NULL }, .func = compare };
    ucl_circular_constructor(circ);
    ucl_circular_set_compar(circ, compar);
    {
      fill(circ, NUMBER, 0);
      {
	ucl_node_t	link;
	ucl_value_t	val;

	val.t_int = 10;
	link = ucl_circular_find(circ, val);
	assert(link);
	assert(val.t_int == link->meta.t_int);

	val.t_int= NUMBER-1;
	link = ucl_circular_find(circ, val);
	assert(link);
	assert(val.t_int == link->meta.t_int);

	val.t_int = 0;
	link = ucl_circular_find(circ, val);
	assert(link);
	assert(val.t_int == link->meta.t_int);

	val.t_int = 4;
	link = ucl_circular_find(circ, val);
	assert(link);
	assert(val.t_int == 4);

	val.t_int = NUMBER-1;
	link = ucl_circular_find(circ, val);
	assert(link);
	assert(val.t_int == link->meta.t_int);
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
  }
  mcl_test_end();
}

static void
test_misc (void)
{
  mcl_test_begin("circular-4.1", "miscellaneous movements") {
    ucl_circular_t	circ;
    ucl_comparison_t	compar = { .data = { .pointer = NULL }, .func = compare };
    ucl_circular_constructor(circ);
    ucl_circular_set_compar(circ, compar);
    {
      fill(circ, NUMBER, 0);
      {
	ucl_node_t	link;
	ucl_value_t 	val;

	/* let's go to the element holding "16" (20-4=16) */
	val.t_int = NUMBER-4;
	link = ucl_circular_find(circ, val);
	assert(link->meta.t_int == val.t_int);

	/* now we extract 10 elements: from 16 to 19 (4 elms) and from 0
	   to 5 (6 elms) */
	for (int i=0; i<10; ++i) {
	  link = ucl_circular_extract(circ);
	  A.alloc(A.data, &link, 0);
	}
	/* now the current element is "6" */
	link = ucl_circular_current(circ);
	assert(link->meta.t_int == 6);

	/* the next element is "7",  followed by unit step increments to
	   "15" (=20-4-1) */
	for (int i=7; i<NUMBER-4; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	}
	link = ucl_circular_current(circ);
	assert(link->meta.t_int == 15);

	/* next is "6" */
	for (int i=6; i<NUMBER-4; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	}
	link = ucl_circular_current(circ);
	assert(link->meta.t_int == 15);

	for (int i=NUMBER-5; i>=6; --i) {
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
	link = ucl_circular_current(circ);
	assert(link->meta.t_int == 15);

	for (int i=NUMBER-5; i>=6; --i) {
	  link = ucl_circular_current(circ);
	  assert(link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
	link = ucl_circular_current(circ);
	assert(link->meta.t_int == NUMBER-5);
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("circular tests");
  A = ucl_memory_allocator;

  mcl_test_subtitle("construction");
  test_constructor();

  mcl_test_subtitle("movement");
  test_forward_movement ();
  test_backward_movement ();

  mcl_test_subtitle("searching");
  test_finding ();

  mcl_test_subtitle("miscellaneous operations");
  test_misc ();

  exit(EXIT_SUCCESS);
}

/* end of file */
