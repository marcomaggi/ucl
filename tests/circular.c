/*
  Part of: Useless Containers Library
  Contents: circular test
  Date: Wed Dec 10, 2003

  Abstract



  Copyright (c) 2003, 2005, 2008, 2010, 2019 Marco Maggi

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
#include <cctests.h>

#define NUMBER	20

static ucl_memory_allocator_t	A;


/** --------------------------------------------------------------------
 ** Helpers.
 ** ----------------------------------------------------------------- */

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


/** --------------------------------------------------------------------
 ** Test constructors.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ucl_circular_t	circ;

    ucl_circular_constructor(circ);
    {
      cctests_assert(L, 0 == ucl_circular_size(circ));
      cctests_assert(L, NULL == ucl_circular_current(circ));
    }
    ucl_circular_destructor(circ);

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Test forward movements.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
/* Test backwards movement. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ucl_circular_t	circ;
    ucl_circular_constructor(circ);
    {
      fill(circ, NUMBER, 0);
      {
	ucl_node_t	link;
	for (int i=0; i<NUMBER; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	}
	for (int i=0; i<NUMBER; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	}
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
    cce_run_body_handlers(L);
  }
}

void
test_2_2 (cce_destination_t upper_L)
/* Test backwards movement. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ucl_circular_t	circ;
    ucl_circular_constructor(circ);
    {
      fill(circ, NUMBER, 0);
      {
	ucl_node_t	link;
	for (int i=NUMBER-1; i>=0; --i) {
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
	for (int i=NUMBER-1; i>=0; --i) {
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Test searching.
 ** ----------------------------------------------------------------- */

int
compare (ucl_value_t data, ucl_value_t outer, ucl_value_t inner)
{
  inner.t_int = ((ucl_node_t)inner.pointer)->meta.t_int;
  return ucl_compare_int_fun(data, outer, inner);
}

void
test_3_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
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
	cctests_assert(L, link);
	cctests_assert(L, val.t_int == link->meta.t_int);

	val.t_int= NUMBER-1;
	link = ucl_circular_find(circ, val);
	cctests_assert(L, link);
	cctests_assert(L, val.t_int == link->meta.t_int);

	val.t_int = 0;
	link = ucl_circular_find(circ, val);
	cctests_assert(L, link);
	cctests_assert(L, val.t_int == link->meta.t_int);

	val.t_int = 4;
	link = ucl_circular_find(circ, val);
	cctests_assert(L, link);
	cctests_assert(L, val.t_int == 4);

	val.t_int = NUMBER-1;
	link = ucl_circular_find(circ, val);
	cctests_assert(L, link);
	cctests_assert(L, val.t_int == link->meta.t_int);
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Test miscellaneous operations.
 ** ----------------------------------------------------------------- */

void
test_4_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
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
	cctests_assert(L, link->meta.t_int == val.t_int);

	/* now we extract 10 elements: from 16 to 19 (4 elms) and from 0
	   to 5 (6 elms) */
	for (int i=0; i<10; ++i) {
	  link = ucl_circular_extract(circ);
	  A.alloc(A.data, &link, 0);
	}
	/* now the current element is "6" */
	link = ucl_circular_current(circ);
	cctests_assert(L, link->meta.t_int == 6);

	/* the next element is "7",  followed by unit step increments to
	   "15" (=20-4-1) */
	for (int i=7; i<NUMBER-4; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	}
	link = ucl_circular_current(circ);
	cctests_assert(L, link->meta.t_int == 15);

	/* next is "6" */
	for (int i=6; i<NUMBER-4; ++i) {
	  ucl_circular_forward(circ, 1);
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	}
	link = ucl_circular_current(circ);
	cctests_assert(L, link->meta.t_int == 15);

	for (int i=NUMBER-5; i>=6; --i) {
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
	link = ucl_circular_current(circ);
	cctests_assert(L, link->meta.t_int == 15);

	for (int i=NUMBER-5; i>=6; --i) {
	  link = ucl_circular_current(circ);
	  cctests_assert(L, link->meta.t_int == i);
	  ucl_circular_backward(circ, 1);
	}
	link = ucl_circular_current(circ);
	cctests_assert(L, link->meta.t_int == NUMBER-5);
      }
      clean(circ);
    }
    ucl_circular_destructor(circ);
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  //ucl_library_init();
  A = ucl_memory_allocator;

  cctests_init("tests for circular lists");
  {
    cctests_begin_group("constructors");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();

    cctests_begin_group("forward and backward movement");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
    }
    cctests_end_group();

    cctests_begin_group("search");
    {
      cctests_run(test_3_1);
    }
    cctests_end_group();

    cctests_begin_group("miscellaneous operations");
    {
      cctests_run(test_4_1);
    }
    cctests_end_group();

  }
  cctests_final();
}

/* end of file */
