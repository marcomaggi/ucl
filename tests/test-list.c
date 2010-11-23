/*
  Part of: Useless Containers Library
  Contents: list test
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

#define MCL_DEBUGGING		1
#include "mcl-test.h"
#include "debug.h"
#include "ucl.h"

#define BIG_NUMBER		1000

#define VAL(node)		((node)->meta.t_int)


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

static void
test_construction_and_destruction (void)
{
  mcl_test_begin("list-1.1", "construction and destruction") {
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
}

int
main (void)
{
  mcl_test_title("list tests");

  mcl_test_subtitle("allocation, construction and destruction");
  test_construction_and_destruction ();


  exit(EXIT_SUCCESS);
}

/* end of file */
