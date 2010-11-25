/*
  Part of: Useless Container Library
  Contents: test for the Map container
  Date: Sat Feb  8, 2003

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
#include "ucl.h"

#define NUMBER		50
#define LITTLENUMBER	10

#define A		ucl_memory_allocator


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static ucl_map_link_t
alloc_link (int value)
{
  ucl_map_link_t 	L = NULL;
  ucl_value_t		K = { .t_int = value };
  A.alloc(A.data, &L, sizeof(ucl_map_link_tag_t));
  ucl_map_setkey(L, K);
  ucl_map_setval(L, K);
  return L;
}
static void
free_link (ucl_map_link_t  L)
{
  A.alloc(A.data, &L, 0);
}
static void
fill (ucl_map_t M, int begin, int end)
{
  assert(0 == ucl_map_size(M));
  for (int i=begin; i<end; ++i) {
    ucl_map_link_t 	L = alloc_link(i);
    ucl_map_insert(M, L);
  }
}
static void
clean (ucl_map_t M)
{
  for (ucl_map_link_t  L = ucl_map_first(M); L; L = ucl_map_first(M))
    free_link(ucl_map_remove(M, L));
  assert(0 == ucl_map_size(M));
}

static void
test_construction_and_destruction (void)
{
  mcl_test_begin("map-1.1", "construction and destruction") {
    ucl_map_t	M;
    ucl_map_constructor(M, 0, ucl_compare_int_pointer);
    {
      assert(0 == ucl_map_size(M));
    }
  }
  mcl_test_end();
}

static void
test_insert_and_find (void)
{
  mcl_test_begin("map-2.1", "simple insertion and finding") {
    ucl_map_t		M;
    ucl_map_link_t 	L;
    ucl_value_t		K;
    ucl_map_constructor(M, 0, ucl_compare_int);
    {
      assert(0 == ucl_map_size(M));
      {
        L = alloc_link(1);
	ucl_map_insert(M, L);
	assert(1 == ucl_map_size(M));

	K.t_int = 1;
	L = ucl_map_find(M, K);
	assert(L);
	K = ucl_map_getkey(L);
	assert(1 == K.t_int);

	ucl_map_remove(M, L);
	free_link(L);
      }
      assert(0 == ucl_map_size(M));
    }
  }
  mcl_test_end();
}

static void
test_insertion_and_removal (void)
{
  mcl_test_begin("map-2.2", "multiple insertion and removal") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_value_t		K;
    int			i;
    ucl_map_constructor(M, 0, ucl_compare_int);
    {
      for (i=0; i< LITTLENUMBER; ++i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(LITTLENUMBER == ucl_map_size(M));
      for (i=0; i<LITTLENUMBER; ++i) {
      	K.t_int = i;
      	L = ucl_map_find(M, K);
      	L = ucl_map_remove(M, L);
      	K = ucl_map_getkey(L);
      	assert(K.t_int == i);
      	free_link(L);
      }
    }
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("map tests");

  mcl_test_subtitle("allocation, construction and destruction");
  test_construction_and_destruction ();

  mcl_test_subtitle("insertions and finding");
  test_insert_and_find ();
  test_insertion_and_removal ();

  exit(EXIT_SUCCESS);
}

/* end of file */
