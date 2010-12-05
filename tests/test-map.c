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

#define MCL_DEBUGGING		1
#include "mcl-test.h"
#include "debug.h"
#include "ucl.h"

#define NUMBER		50
#define LITTLENUMBER	10


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

#define STATUS(L)		(((link_t)L)->node.meta.avl_status)
#define STATUS_STRING(S)	((UCL_EQUAL_DEPTH==S)? "equal-depth": \
				 ((UCL_SON_DEEPER==S)? "son-deeper":\
				  ((UCL_BRO_DEEPER==S)? "bro-deeper": "corrupted")))

#define KEY(L)			(((link_t)(L))->key.t_int)
#define MAYBE_KEY(L)		((L)? KEY(L) : -1)


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static link_t
alloc_link (int value)
{
  ucl_memory_allocator_t A = ucl_memory_allocator;
  link_t 	L = NULL;
  ucl_value_t	K = { .t_int = value };
  A.alloc(A.data, &L, sizeof(link_tag_t));
  L->key = K;
  return L;
}
static void
free_link (link_t  L)
{
  ucl_memory_allocator_t A = ucl_memory_allocator;
  A.alloc(A.data, &L, 0);
}
static void
fill (ucl_map_t M, int first, int upper, int step)
{
  for (int i=first; i<upper; i+=step)
    ucl_map_insert(M, alloc_link(i));
}
static void
clean (ucl_map_t M)
{
#if 0
  for (link_t  L = ucl_map_first(M); L; L = ucl_map_first(M)) {
    ucl_map_remove(M, L);
    free_link(L);
  }
  assert(0 == ucl_map_size(M));
#endif
}

/** --------------------------------------------------------------------
 ** Validating routines.
 ** ----------------------------------------------------------------- */

static void
checked_insertion (ucl_map_t M, int j, link_t * L)
/* Insert a new node with key  "j" and validate the existence of the new
   node  by  finding  it.   Store  the  node  pointer  in  the  location
   referenced by "L". */
{
  ucl_value_t	K, K1;
  size_t	old_size=ucl_map_size(M);
  *L = alloc_link(j);
  ucl_map_insert(M, *L);
  mcl_test_error_if_false(1+old_size == ucl_map_size(M), "invalid size after insertion of %d", j);
  K.t_int = j;
  *L = ucl_map_find(M, K);
  mcl_test_error_if_false(NULL!=*L, "unable to find node with key %d", j);
  K1 = (*L)->key;
  mcl_test_error_if_false(j == K1.t_int, "invalid key value, expected %d got %d", j, K1.t_int);
}
static void
checked_deletion (ucl_map_t M, int j, link_t * L)
/* Delete the node with key "j".*/
{
  link_t	N;
  ucl_value_t	K;
  size_t	old_size=ucl_map_size(M);
  mcl_debug("deleting %d", j);
  K.t_int = j;
  N = ucl_map_find(M, K);
  mcl_test_error_if_true(NULL == N, "unable to find node with key %d", j);
  mcl_test_error_if_false(j == N->key.t_int,
			  "error finding node with key %d, got key %d", j, N->key.t_int);
  mcl_test_error_if_false(N == *L, "unexpected node with key %d, expected %p got %p",
			  j, (void *)*L, (void *)N);
  ucl_map_remove(M, N);
  *L = NULL;
  mcl_test_error_if_false(old_size == 1+ucl_map_size(M), "invalid size after deletion of %d", j);
}
static void
validate_links (ucl_map_t M, int j, void * dad, void * son, void * bro, ucl_avl_status_t expected_status)
/* Extract from "M" the node with key "j", then validate its links to be
   the  given ones,  finally validate  its AVL  status to  be  the given
   one. */
{
  ucl_value_t	K = { .t_int = j };
  link_t	L = ucl_map_find(M, K);
  mcl_test_error_if_false(dad == DAD_OF(L),
			  "invalid dad of node %d, expected %p (key=%d) got %p (key=%d)",
			  j, dad, MAYBE_KEY(dad), DAD_OF(L), MAYBE_KEY(DAD_OF(L)));
  mcl_test_error_if_false(son == SON_OF(L),
			  "invalid son of node %d, expected %p (key=%d) got %p (key=%d)",
			  j, son, MAYBE_KEY(son), SON_OF(L), MAYBE_KEY(SON_OF(L)));
  mcl_test_error_if_false(bro == BRO_OF(L),
			  "invalid bro of node %d, expected %p (key=%d) got %p (key=%d)",
			  j, bro, MAYBE_KEY(bro), BRO_OF(L), MAYBE_KEY(BRO_OF(L)));
  mcl_test_error_if_false(expected_status == STATUS(L),
			  "invalid status of node %d, expected %s got %s",
			  j, STATUS_STRING(expected_status), STATUS_STRING(STATUS(L)));
}
static void
validate_links_m (int key, void * cur, void * dad, void * son, void * bro,
		  ucl_avl_status_t expected_status)
/* Validate the  node referenced  by "cur" with  should have  key "key",
   then validate  its links to be  the given ones,  finally validate its
   AVL status to be the given one. */
{
  link_t	L = cur;
  mcl_test_error_if_false(key == MAYBE_KEY(L), "invalid key %d for slot %p", key, cur);
  mcl_test_error_if_false(dad == DAD_OF(L),
			  "invalid dad of node %d, expected %p (key=%d) got %p (key=%d)",
			  key, dad, MAYBE_KEY(dad), DAD_OF(L), MAYBE_KEY(DAD_OF(L)));
  mcl_test_error_if_false(son == SON_OF(L),
			  "invalid son of node %d, expected %p (key=%d) got %p (key=%d)",
			  key, son, MAYBE_KEY(son), SON_OF(L), MAYBE_KEY(SON_OF(L)));
  mcl_test_error_if_false(bro == BRO_OF(L),
			  "invalid bro of node %d, expected %p (key=%d) got %p (key=%d)",
			  key, bro, MAYBE_KEY(bro), BRO_OF(L), MAYBE_KEY(BRO_OF(L)));
  mcl_test_error_if_false(expected_status == STATUS(L),
			  "invalid status of node %d, expected %s got %s",
			  key, STATUS_STRING(expected_status), STATUS_STRING(STATUS(L)));
}

/** --------------------------------------------------------------------
 ** Test trees.
 ** ----------------------------------------------------------------- */

static void
build_base_tree_with_plenty_of_holes (ucl_map_t M, link_t * L)
{
  int		j;

  /* add 0 and 10:
   *
   *
   *      0b--10=
   *
   */
  j=0;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M, j, NULL, NULL, NULL, UCL_EQUAL_DEPTH);

  j=10;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M,  0, NULL, NULL, L[10], UCL_BRO_DEEPER);
  validate_links(M, 10, L[0], NULL, NULL, UCL_EQUAL_DEPTH);

  /* add 2 (bro rotation raising 10):
   *
   *      0--10--20     10=--20=
   *                 => |
   *                    0=
   */
  j=20;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M, 10, NULL,  L[0], L[20], UCL_EQUAL_DEPTH);
  validate_links(M,  0, L[10], NULL, NULL, UCL_EQUAL_DEPTH);
  validate_links(M, 20, L[10], NULL, NULL, UCL_EQUAL_DEPTH);

  /* add 30:
   *
   *      10b--20b--30=
   *      |
   *      0=
   */
  j=30;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M, 10, NULL,  L[0], L[20], UCL_BRO_DEEPER);
  validate_links(M,  0, L[10], NULL, NULL,  UCL_EQUAL_DEPTH);
  validate_links(M, 20, L[10], NULL, L[30], UCL_BRO_DEEPER);
  validate_links(M, 30, L[20], NULL, NULL,  UCL_EQUAL_DEPTH);

  /* add 40 (bro rotation raising 30):
   *
   *      10--20--30--40           10b--30=--40=
   *      |                =>      |     |
   *      0                        0=   20=
   */
  j=40;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M, 10, NULL,  L[0], L[30], UCL_BRO_DEEPER);
  validate_links(M,  0, L[10], NULL, NULL, UCL_EQUAL_DEPTH);
  validate_links(M, 30, L[10], L[20], L[40], UCL_EQUAL_DEPTH);
  validate_links(M, 20, L[30], NULL, NULL, UCL_EQUAL_DEPTH);
  validate_links(M, 40, L[30], NULL, NULL, UCL_EQUAL_DEPTH);
}
static void
fill_multimap (ucl_map_t M, int size, int first, int step, int times)
{
  int	j, k;
  for (j=first; j<size; ++j)
    ucl_map_insert(M, alloc_link(j));
  for (k=0; k<(times-1); ++k)
    for (j=first; j<size; j+=step)
      ucl_map_insert(M, alloc_link(j));
}

/** --------------------------------------------------------------------
 ** Printing functions.
 ** ----------------------------------------------------------------- */

static void UCL_UNUSED
print_inorder (ucl_map_t M)
{
  ucl_iterator_t	I;
  ucl_value_t		K;
  link_t		L;
  int			i;
  for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
    L = ucl_iterator_ptr(I);
    K = L->key;
    fprintf(stderr, "i=%d %d\n", i, K.t_int);
  }
}
static void UCL_UNUSED
print_preorder_links (ucl_map_t M, const char * message, ...)
{
  va_list		ap;
  ucl_iterator_t	I;
  link_t	L, dad, son, bro;
  int			i;
  fprintf(stderr, "*** ");
  va_start(ap, message);
  {
    vfprintf(stderr, message, ap);
  }
  va_end(ap);
  fprintf(stderr, ": preorder links (%u nodes)\n", ucl_map_size(M));
  for (ucl_map_iterator_preorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
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

    fprintf(stderr, "\t(%p, status: %s)\n", (void *)L, STATUS_STRING(STATUS(L)));
  }
}

static void
test_construction_and_destruction (void)
{
  mcl_test_begin("map-1.1", "construction and destruction") {
    ucl_map_t	M;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
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
    link_t 	L;
    ucl_value_t		K;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      assert(0 == ucl_map_size(M));
      {
        L = alloc_link(1);
	ucl_map_insert(M, L);
	mcl_test_error_if_false(1 == ucl_map_size(M),
				"invalid size after insertion of %d", 1);

	K.t_int = 1;
	L = ucl_map_find(M, K);
	mcl_test_error_if_false(NULL != L, "unable to find node with key %d", K.t_int);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(1 == K.t_int,
				"invalid key value, expected %d got %d", 1, K.t_int);

	ucl_map_remove(M, L);
	free_link(L);
      }
      assert(0 == ucl_map_size(M));
    }
  }
  mcl_test_end();
}

static void
test_checked_inorder_insertion (void)
{
  mcl_test_begin("map-2.2",
		 "inorder insertion and check of internal structure (bro rotations)") {
    ucl_map_t		M;
    link_t 	L[10];
    int			j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      /* Inserting 10 numbers  from 0 to 9 produces  the following trees
	 (AVL status at the side of  the number: "s" son deeper, "b" bro
	 deeper, "=" equal depth). */

      /* add 0 and 1:
       *
       *
       *      0b--1=
       *
       */
      j=0;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, j, NULL, NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      j=1;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 0, NULL, NULL, L[1], UCL_BRO_DEEPER);
      validate_links(M, 1, L[0], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 2 (bro rotation raising 1):
       *
       *      0--1--2     1=--2=
       *               => |
       *                  0=
       */
      j=2;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 1, NULL, L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 3:
       *
       *      1b--2b--3=
       *      |
       *      0=
       */
      j=3;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 1, NULL, L[0], L[2], UCL_BRO_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, L[3], UCL_BRO_DEEPER);
      validate_links(M, 3, L[2], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 4 (bro rotation raising 3):
       *
       *      1--2--3--4           1b--3=--4=
       *      |            =>      |   |
       *      0                    0=  2=
       */
      j=4;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 1, NULL, L[0], L[3], UCL_BRO_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[1], L[2], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[3], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[3], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 5 (bro rotation raising 3):
       *
       *      1--3--4--5           3=--4b--5=
       *      |  |         =>      |
       *      0  2                 1=--2=
       *                           |
       *                           0=
       */
      j=5;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 3, NULL, L[1], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[3], NULL, L[5], UCL_BRO_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 6 (bro rotation raising 5):
       *
       *      3--4--5--6           3=------5=--6=
       *      |                    |       |
       *      1--2         =>      1=--2=  4=
       *      |                    |
       *      0                    0=
       */
      j=6;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 3, NULL, L[1], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[3], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 7:
       *
       *      3----5--6--7
       *      |    |
       *      1--2 4
       *      |
       *      0
       */
      j=7;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 3, NULL, L[1], L[5], UCL_BRO_DEEPER);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[3], L[4], L[6], UCL_BRO_DEEPER);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, L[7], UCL_BRO_DEEPER);
      validate_links(M, 7, L[6], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 8 (bro rotation raising 7):
       *
       *      3----5--6--7--8    3b------5b--7=--8=
       *      |    |             |       |   |
       *      1--2 4          => 1=--2=  4=  6=
       *      |                  |
       *      0                  0=
       */
      j=8;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 3, NULL, L[1], L[5], UCL_BRO_DEEPER);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[3], L[4], L[7], UCL_BRO_DEEPER);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[5], L[6], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[7], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 9 (bro rotation raising 7):
       *
       *      3-----5--7--8--9    3b------7=--8b--9=
       *      |     |  |          |       |
       *      1--2  4  6      =>  1=--2=  5=--6=
       *      |                   |       |
       *      0                   0=      4=
       */
      j=9;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 3, NULL, L[1], L[7], UCL_BRO_DEEPER);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[3], L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[7], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      ++j;
      mcl_test_error_if_false((size_t)j == ucl_map_size(M),
			      "invalid size after insertion, expected %d got %d", j, ucl_map_size(M));

      for (j=0; j<10; ++j)
	free_link(L[j]);
    }
  }
  mcl_test_end();
}

static void
test_checked_inverse_inorder_insertion (void)
{
  mcl_test_begin("map-2.3",
		 "inverse inorder insertion and check of internal structure (son rotations)") {
    ucl_map_t		M;
    link_t 	L[10];
    int			j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      /* Inserting  10 numbers  from 0  to 9  produces these  trees (AVL
	 status  at the  side of  the number:  "s" son  deeper,  "b" bro
	 deeper, "=" equal depth). */

      /* add 9 and 8:
       *
       *    9s
       *    |
       *    8=
       */
      j=9;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, j, NULL, NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      j=8;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 9, NULL, L[8], NULL, UCL_SON_DEEPER);
      validate_links(M, 8, L[9], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 7 (son rotation raising 1):
       *
       *      9            8=--9=
       *      |         => |
       *      8            7=
       *      |
       *      7
       */
      j=7;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 8, NULL, L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 6:
       *
       *      8s--9=
       *      |
       *      7s
       *      |
       *      6=
       */
      j=6;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 8, NULL, L[7], L[9], UCL_SON_DEEPER);
      validate_links(M, 7, L[8], L[6], NULL, UCL_SON_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[7], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 5 (son rotation raising 6):
       *
       *      8s--9=       8s--9=
       *      |            |
       *      7s           6=--7=
       *      |        =>  |
       *      6=           5=
       *      |
       *      5
       */
      j=5;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 8, NULL, L[6], L[9], UCL_SON_DEEPER);
      validate_links(M, 6, L[8], L[5], L[7], UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[6], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[6], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 4 (son rotation raising 6):
       *
       *      8s--9=         6=--8=--9=
       *      |              |   |
       *      6=--7=         5s  7=
       *      |         =>   |
       *      5=             4=
       *      |
       *      4
       */
      j=4;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 6, NULL, L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[6], L[4], NULL, UCL_SON_DEEPER);
      validate_links(M, 8, L[6], L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 3 (son rotation raising 4):
       *
       *      6=--8=--9=       6=------8=--9=
       *      |   |            |       |
       *      5s  7=       =>  4=--5=  7=
       *      |                |
       *      4=               3=
       *      |
       *      3
       */
      j=3;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 6, NULL, L[4], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[6], L[3], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[6], L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 2:
       *
       *      6s------8=--9=
       *      |       |
       *      4s--5=  7=
       *      |
       *      3s
       *      |
       *      2=
       */
      j=2;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 6, NULL, L[4], L[8], UCL_SON_DEEPER);
      validate_links(M, 4, L[6], L[3], L[5], UCL_SON_DEEPER);
      validate_links(M, 8, L[6], L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], L[2], NULL, UCL_SON_DEEPER);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[3], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 1 (son rotation raising 2):
       *
       *      6s------8=--9=       6s------8=--9=
       *      |       |            |       |
       *      4s--5=  7=           4s--5=  7=
       *      |                    |
       *      3s               =>  2=--3=
       *      |                    |
       *      2=                   1=
       *      |
       *      1
       */
      j=1;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 6, NULL, L[4], L[8], UCL_SON_DEEPER);
      validate_links(M, 4, L[6], L[2], L[5], UCL_SON_DEEPER);
      validate_links(M, 8, L[6], L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[4], L[1], L[3], UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[2], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[2], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 0 (son rotation raising 2):
       *
       *      6s------8=--9=        6s-----------8=--9=
       *      |       |             |            |
       *      4s--5=  7=            2=--4=--5=   7=
       *      |                     |   |
       *      2=--3=            =>  1s  3=
       *      |                     |
       *      1=                    0=
       *      |
       *      0
       */
      j=0;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 6, NULL, L[2], L[8], UCL_SON_DEEPER);
      validate_links(M, 2, L[6], L[1], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[6], L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 4, L[2], L[3], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      mcl_test_error_if_false(10 == ucl_map_size(M),
			      "invalid size after insertion, expected %d got %d",
			      j, ucl_map_size(M));

      for (j=0; j<10; ++j)
	free_link(L[j]);
    }
  }
  mcl_test_end();
}

static void
test_checked_insertion_random (void)
{
#undef SIZE
#define SIZE		100
  mcl_test_begin("map-2.4",
		 "random insertions and check of internal structure (son-bro rotations)") {
    ucl_map_t	M;
    link_t 	L[SIZE];
    int		j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      memset(L, '\0', SIZE*sizeof(link_t));
      /* First we  build a  base tree with  an inorder insertion  of the
	 numbers 0, 10, 20, 30, 40; we have plenty of integers available
	 between each of them. */
      build_base_tree_with_plenty_of_holes(M, L);

      /* add 15 (bro-son rotation raising 20):
       *
       *   10b--30=--40=      20=-------30b--40=
       *   |     |             |
       *   0=   20=       =>  10=--15=
       *         |             |
       *        15             0=
       */
      j=15;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 20,  NULL, L[10], L[30], UCL_EQUAL_DEPTH);
      validate_links(M, 10, L[20], L[0],  L[15], UCL_EQUAL_DEPTH);
      validate_links(M, 30, L[20],  NULL, L[40], UCL_BRO_DEEPER);
      validate_links(M,  0, L[10],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 15, L[10],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 40, L[30],  NULL,  NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 12:
       *
       *   20s-------30b--40=
       *    |
       *   10b--15s
       *    |    |
       *    0=  12=
       */
      j=12;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 20,  NULL, L[10], L[30], UCL_SON_DEEPER);
      validate_links(M, 10, L[20],  L[0], L[15], UCL_BRO_DEEPER);
      validate_links(M, 30, L[20],  NULL, L[40], UCL_BRO_DEEPER);
      validate_links(M,  0, L[10],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 15, L[10], L[12],  NULL, UCL_SON_DEEPER);
      validate_links(M, 12, L[15],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 40, L[30],  NULL,  NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 13 (son-bro rotation raising 13):
       *
       *   20s-------30b--40=      20s-------30b--40=
       *    |                       |
       *   10b--15s            =>  10b--13=--15=
       *    |    |                  |    |
       *    0=  12=--13             0=  12=
       *
       */
      j=13;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 20,  NULL, L[10], L[30], UCL_SON_DEEPER);
      validate_links(M, 10, L[20],  L[0], L[13], UCL_BRO_DEEPER);
      validate_links(M, 30, L[20],  NULL, L[40], UCL_BRO_DEEPER);
      validate_links(M,  0, L[10],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 13, L[10], L[12], L[15], UCL_EQUAL_DEPTH);
      validate_links(M, 12, L[13],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 15, L[13],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 40, L[30],  NULL,  NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add 11 (son-bro rotation raising 12):
       *
       *   20s-------30b--40=      20s-------30b--40=
       *    |                       |
       *   10b--13=--15=           12=-------13b--15=
       *    |    |             =>   |
       *    0=  12=                10=--11=
       *         |                  |
       *        11                  0=
       */
      j=11;
      mcl_debug("inserting %d", j);
      checked_insertion(M, j, &L[j]);
      validate_links(M, 20,  NULL, L[12], L[30], UCL_SON_DEEPER);
      validate_links(M, 12, L[20], L[10], L[13], UCL_EQUAL_DEPTH);
      validate_links(M, 30, L[20],  NULL, L[40], UCL_BRO_DEEPER);
      validate_links(M, 10, L[12],  L[0], L[11], UCL_EQUAL_DEPTH);
      validate_links(M, 13, L[12],  NULL, L[15], UCL_BRO_DEEPER);
      validate_links(M,  0, L[10],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 11, L[10],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 15, L[13],  NULL,  NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 40, L[30],  NULL,  NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      for (j=0; j<SIZE; ++j)
	if (L[j]) free_link(L[j]);
    }
  }
  mcl_test_end();
}

static void
test_multimap_insert_and_find (void)
{
#undef SIZE
#define SIZE		100
#undef DIM
#define DIM		10
  mcl_test_begin("map-2.5", "multimap insertion and validation of internal structure") {
    ucl_map_t		M;
    link_t		L[SIZE];
    int			j, k;
    ucl_map_initialise(M, UCL_ALLOW_MULTIPLE_OBJECTS, ucl_compare_int, getkey);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      /* Inserting 10 numbers  from 0 to 9 produces  the following trees
       * (AVL status at the side of  the number: "s" son deeper, "b" bro
       * deeper, "=" equal depth).
       *
       *      3b------7=--8b--9=
       *      |       |
       *      1=--2=  5=--6=
       *      |       |
       *      0=      4=
       */
      memset(L, '\0', SIZE*sizeof(link_t));
      for (j=0; j<DIM; ++j) {
	mcl_debug("inserting %d", j);
	checked_insertion(M, j, &L[j]);
      }
      validate_links(M, 3, NULL, L[1], L[7], UCL_BRO_DEEPER);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[3], L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[7], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* add another 5 (bro/son rotation raising 5 the upper 5):
       *
       *   3b------7=--8b--9=      3b------7=--8b--9=      5=------7=--8b--9=
       *   |       |               |       |               |       |
       *   1=--2=  5=--6=      =>  1=--2=  5=--6s      =>  3s--4=  6s
       *   |       |               |       |   |           |       |
       *   0=      4=              0=      4=  5=          1=--2=  5(10)=
       *                                                   |
       *                                                   0=
       */
      j=10;
      k=5;
      mcl_debug("inserting %d", k);
      checked_insertion(M, k, &L[j]);
      validate_links_m( 5,  L[5],  NULL,  L[3],  L[7], UCL_EQUAL_DEPTH);
#if 0
      validate_links_m(3, L[3], NULL, L[1], L[7], UCL_BRO_DEEPER);
      validate_links_m(1, L[1], L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links_m(7, L[7], L[3], L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links_m(0, L[0], L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links_m(2, L[2], L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links_m(4, L[4], L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links_m(6, L[6], L[5], L[10], NULL, UCL_SON_DEEPER);
      validate_links_m(8, L[8], L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links_m(9, L[9], L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links_m(5, L[10], L[6], NULL, NULL, UCL_EQUAL_DEPTH);
#endif
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d in slot", k, j);

      mcl_test_error_if_false((size_t)1+j == ucl_map_size(M),
			      "invalid size after overall insertion, expected %d got %d",
			      1+j, ucl_map_size(M));

    }
    clean(M);
  }
  mcl_test_end();
}

static void
test_inorder_insertion_and_deletion (void)
{
#undef  SIZE
#define SIZE	10
  mcl_test_begin("map-3.1", "multiple inorder insertion and deletion") {
    ucl_map_t	M;
    link_t 	L[SIZE];
    int		j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (j=0; j<SIZE; ++j)
	checked_insertion(M, j, &L[j]);

      /* Inserting numbers from 0 to 9 inorder has produced this tree:
       *
       *   3b------7=--8b--9=
       *   |       |
       *   1=--2=  5=--6=
       *   |       |
       *   0=      4=
       */
      validate_links(M, 3, NULL, L[1], L[7], UCL_BRO_DEEPER);
      validate_links(M, 1, L[3], L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[3], L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[7], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after inorder insertion of nodes");

      /* delete 0:
       *
       *   3b------7=--8b--9=       3b------7=--8b--9=
       *   |       |                |       |
       *   1=--2=  5=--6=       =>  1b--2=  5=--6=
       *   |       |                        |
       *   0=      4=                       4=
       */
      j=0;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 3, NULL, L[1], L[7], UCL_BRO_DEEPER);
      validate_links(M, 1, L[3], NULL, L[2], UCL_BRO_DEEPER);
      validate_links(M, 7, L[3], L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[7], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 1:
       *
       *   3b------7=--8b--9=      7s--8b--9=
       *   |       |               |
       *   1b--2=  5=--6=      =>  3b--5=--6=
       *           |               |   |
       *           4=              2=  4=
       */
      j=1;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 7, NULL, L[3], L[8], UCL_SON_DEEPER);
      validate_links(M, 3, L[7], L[2], L[5], UCL_BRO_DEEPER);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 2, L[3], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[3], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 2:
       *
       *   7s--8b--9=         7s--8b--9=
       *   |                  |
       *   3b--5=--6=    =>   5s--6=
       *   |   |              |
       *   2=  4=             3b--4=
       */
      j=2;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 7, NULL, L[5], L[8], UCL_SON_DEEPER);
      validate_links(M, 5, L[7], L[3], L[6], UCL_SON_DEEPER);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 3, L[5], NULL, L[4], UCL_BRO_DEEPER);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[3], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 3:
       *
       *   7s--8b--9=      7=--8b--9=
       *   |               |
       *   5s--6=      =>  5=--6=
       *   |               |
       *   3b--4=          4=
       */
      j=3;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 7, NULL, L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[7], L[4], L[6], UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 4, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 4:
       *
       *   7=--8b--9=      7=--8b--9=
       *   |               |
       *   5=--6=      =>  5b--6=
       *   |
       *   4=
       */
      j=4;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 7, NULL, L[5], L[8], UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[7], NULL, L[6], UCL_BRO_DEEPER);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 6, L[5], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 5:
       *
       *   7=--8b--9=      7b--8b--9=
       *   |               |
       *   5b--6=      =>  6=
       *
       */
      j=5;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 7, NULL, L[6], L[8], UCL_BRO_DEEPER);
      validate_links(M, 6, L[7], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[7], NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 6:
       *
       *   7b--8b--9=      8=--9=
       *   |               |
       *   6=          =>  7=
       *
       */
      j=6;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 8, NULL, L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 7:
       *
       *   8=--9=      8=--9=
       *   |       =>
       *   7=
       *
       */
      j=7;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 8, NULL, NULL, L[9], UCL_BRO_DEEPER);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 8:
       *
       *   8=--9=  =>  9=
       *
       */
      j=8;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 9, NULL, NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);

      /* delete 9 */
      j=9;
      checked_deletion(M, j, &L[j]);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after deletion of %d", j);
      mcl_test_error_if_false(0 == ucl_map_size(M),
			      "expected empty map after removing all the nodes, got size %u",
			      ucl_map_size(M));
    }
  }
  mcl_test_end();
}

static void
test_inverse_inorder_insertion_and_deletion (void)
{
#undef  SIZE
#define SIZE	10
  mcl_test_begin("map-3.2", "multiple reverse inorder insertion and deletion") {
    ucl_map_t	M;
    link_t 	L[SIZE];
    int		j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (j=SIZE-1; j>=0; --j)
	checked_insertion(M, j, &L[j]);
      mcl_test_error_if_false(10 == ucl_map_size(M),
			      "invalid size after insertion, expected %d got %d",
			      SIZE, ucl_map_size(M));

      /* Inserting numbers from 9 to 0 has produced the following tree:
       *
       *    6s-----------8=--9=
       *    |            |
       *    2=--4=--5=   7=
       *    |   |
       *    1s  3=
       *    |
       *    0=
       */
      validate_links(M, 6, NULL, L[2], L[8], UCL_SON_DEEPER);
      validate_links(M, 2, L[6], L[1], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[6], L[7], L[9], UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 4, L[2], L[3], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 9, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 9:
       *
       *    6s-----------8=--9=      6s-----------8s
       *    |            |           |            |
       *    2=--4=--5=   7=          2=--4=--5=   7=
       *    |   |                =>  |   |
       *    1s  3=                   1s  3=
       *    |                        |
       *    0=                       0=
       */
      j=9;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 6, NULL, L[2], L[8], UCL_SON_DEEPER);
      validate_links(M, 2, L[6], L[1], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 8, L[6], L[7], NULL, UCL_SON_DEEPER);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 4, L[2], L[3], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[8], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 8:
       *
       *    6s-----------8s      6s--7=            2b--6s--7=
       *    |            |       |                 |   |
       *    2=--4=--5=   7=      2=--4=--5=        1s  4=--5=
       *    |   |            =>  |   |         =>  |   |
       *    1s  3=               1s  3=            0=  3=
       *    |                    |
       *    0=                   0=
       */
      j=8;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 2, NULL, L[1], L[6], UCL_BRO_DEEPER);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 6, L[2], L[4], L[7], UCL_SON_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 4, L[6], L[3], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 7, L[6], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 7:
       *
       *    2b--6s--7=      2b--6s          2b--4b--6s
       *    |   |           |   |           |   |   |
       *    1s  4=--5=  =>  1s  4=--5=  =>  1s  3=  5=
       *    |   |           |   |           |
       *    0=  3=          0=  3=          0=
       */
      j=7;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 2, NULL, L[1], L[4], UCL_BRO_DEEPER);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 4, L[2], L[3], L[6], UCL_BRO_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 6, L[4], L[5], NULL, UCL_SON_DEEPER);
      validate_links(M, 5, L[6], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 6:
       *
       *    2b--4b--6s      2=--4=--5=
       *    |   |   |       |   |
       *    1s  3=  5=  =>  1s  3=
       *    |               |
       *    0=              0=
       */
      j=6;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 2, NULL, L[1], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 4, L[2], L[3], L[5], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 5, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 5:
       *
       *    2=--4=--5=      2=--4s
       *    |   |           |   |
       *    1s  3=      =>  1s  3=
       *    |               |
       *    0=              0=
       */
      j=5;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 2, NULL, L[1], L[4], UCL_EQUAL_DEPTH);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 4, L[2], L[3], NULL, UCL_SON_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 3, L[4], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 4:
       *
       *    2=--4=--5=      2s--3=
       *    |   |           |
       *    1s  3=      =>  1s
       *    |               |
       *    0=              0=
       */
      j=4;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 2, NULL, L[1], L[3], UCL_SON_DEEPER);
      validate_links(M, 1, L[2], L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 3, L[2], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 3:
       *
       *    2s--3=      2s      1=--2=
       *    |           |       |
       *    1s      =>  1s  =>  0=
       *    |           |
       *    0=          0=
       */
      j=3;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 1, NULL, L[0], L[2], UCL_EQUAL_DEPTH);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      validate_links(M, 2, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 2:
       *
       *    1=--2=      1s
       *    |       =>  |
       *    0=          0=
       */
      j=2;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 1, NULL, L[0], NULL, UCL_SON_DEEPER);
      validate_links(M, 0, L[1], NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 1:
       *
       *    1s      0=
       *    |   =>
       *    0=
       */
      j=1;
      checked_deletion(M, j, &L[j]);
      validate_links(M, 0, NULL, NULL, NULL, UCL_EQUAL_DEPTH);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

      /* delete 0 */
      j=0;
      checked_deletion(M, j, &L[j]);
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after deletion of all nodes");
    }
  }
  mcl_test_end();
}

static void
test_inorder_iteration (void)
{
  mcl_test_begin("map-4.1", "inorder iteration, few nodes inorder insertion") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (i=0; i<LITTLENUMBER; ++i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == LITTLENUMBER);
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(i == K.t_int);
      }
      assert(i == LITTLENUMBER);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.2", "inorder iteration, few nodes reverse inorder insertion") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (i=LITTLENUMBER; i>0; --i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == LITTLENUMBER);
      for (ucl_map_iterator_inorder(M, I), i=1; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(i == K.t_int);
      }
      assert(i == LITTLENUMBER+1);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.3", "inorder iteration, mixed insertion order") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
#undef SIZE
#define SIZE	6
      static const int array[SIZE]   = { 10, 13, 5, 3, 8, 7 };
      static const int inorder[SIZE] = { 3, 5, 7, 8, 10, 13 };
      for (i=0; i<SIZE; ++i) {
	L = alloc_link(array[i]);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == SIZE);
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(inorder[i] == K.t_int);
      }
      assert(i == SIZE);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.4", "inorder iteration, mixed insertion order") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
#undef SIZE
#define SIZE	6
      static const int array[SIZE]   = { 10, 13, 5, 11, 15, 12 };
      static const int inorder[SIZE] = { 5, 10, 11, 12, 13, 15 };
      for (i=0; i<SIZE; ++i) {
	L = alloc_link(array[i]);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == SIZE);
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(inorder[i] == K.t_int);
      }
      assert(i == SIZE);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.5", "inorder iteration, many numbers inorder insertion") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (i=0; i<NUMBER; ++i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == NUMBER);
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(i == K.t_int);
      }
      assert(i == NUMBER);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.6", "inorder iteration, many numbers reverse inorder insertion") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (i=NUMBER; i>0; --i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == NUMBER);
      for (ucl_map_iterator_inorder(M, I), i=1; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(i == K.t_int);
      }
      assert(i == NUMBER+1);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.7", "inorder iteration, mixed insertion") {
    ucl_map_t		M;
    link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      /* insert in this order: 0-9, 30-39, 20-29, 10-19 */
      for (i=0; i<10; ++i)
	ucl_map_insert(M, alloc_link(i));
      for (i=30; i<40; ++i)
	ucl_map_insert(M, alloc_link(i));
      for (i=20; i<30; ++i)
	ucl_map_insert(M, alloc_link(i));
      for (i=10; i<20; ++i)
	ucl_map_insert(M, alloc_link(i));
      mcl_test_error_if_false(ucl_map_size(M) == 40, "wrong size after insertion");
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(i == K.t_int);
      }
      assert(i == 40);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.8", "inorder iteration, mixed insertion") {
    ucl_map_t		M;
    link_t		L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      /* insert in this order: 20-11, 30-21, 40-31, 10-1 */
      for (i=20; i>10; --i)
	mcl_test_error_if_false(ucl_map_insert(M, alloc_link(i)), "error inserting element %i", i);
      for (i=30; i>20; --i)
	ucl_map_insert(M, alloc_link(i));
      for (i=40; i>30; --i)
	ucl_map_insert(M, alloc_link(i));
      for (i=10; i>0;  --i)
	ucl_map_insert(M, alloc_link(i));
      mcl_test_error_if_false(ucl_map_size(M) == 40, "wrong size after insertion");
      for (ucl_map_iterator_inorder(M, I), i=1; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	assert(i == K.t_int);
      }
      assert(i == 41);
    }
    clean(M);
  }
  mcl_test_end();
}

static void
test_lower_bound_multimap_iterator (void)
{
  mcl_test_begin("map-5.1", "lower bound iteration on multi map") {
    ucl_iterator_t	I;
    ucl_map_t		M;
    link_t		L;
    ucl_value_t		K, K1;
    int			j, first=0, step=5, times=3, expected_count;
    ucl_map_initialise(M, UCL_ALLOW_MULTIPLE_OBJECTS, ucl_compare_int, getkey);
    {
      fill_multimap(M, 1+LITTLENUMBER, first, step, times);

      K.t_int = 5;
      expected_count = times;
      for (ucl_map_lower_bound(M, I, K), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K1 = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == K1.t_int,
				"wrong key value, expected %d got %d", K.t_int, K1.t_int);
      }
      mcl_test_error_if_false(expected_count == j,
			      "wrong number of equal links, expected %d, got %d", expected_count, j);

      K.t_int = 3;
      expected_count = 1;
      for (ucl_map_lower_bound(M, I, K), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K1 = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == K1.t_int,
				"wrong key value, expected %d got %d", K.t_int, K1.t_int);
      }
      mcl_test_error_if_false(expected_count == j,
			      "wrong number of equal links, expected %d, got %d", expected_count, j);

      K.t_int = LITTLENUMBER+10;
      expected_count = 0;
      for (ucl_map_lower_bound(M, I, K), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j);
      mcl_test_error_if_false(expected_count == j,
			      "wrong number of equal links, expected %d, got %d", expected_count, j);

    }
    clean(M);
  }
  mcl_test_end();
}

static void
test_upper_bound_multimap_iterator (void)
{
  mcl_test_begin("map-5.2", "upper bound iteration on multi map") {
    ucl_iterator_t	I;
    ucl_map_t		M;
    link_t		L;
    ucl_value_t		K, K1;
    int			j, first=0, step=5, times=3, expected_count;
    ucl_map_initialise(M, UCL_ALLOW_MULTIPLE_OBJECTS, ucl_compare_int, getkey);
    {
      fill_multimap(M, LITTLENUMBER, first, step, times);

      K.t_int = 5;
      expected_count = times;
      for (ucl_map_upper_bound(M, I, K), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K1 = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == K1.t_int,
				"wrong key value, expected %d got %d", K.t_int, K1.t_int);
      }
      mcl_test_error_if_false(expected_count == j,
			      "wrong number of equal links, expected %d, got %d", expected_count, j);

      K.t_int = 3;
      expected_count = 1;
      for (ucl_map_upper_bound(M, I, K), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K1 = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == K1.t_int,
				"wrong key value, expected %d got %d", K.t_int, K1.t_int);
      }
      mcl_test_error_if_false(expected_count == j,
			      "wrong number of equal links, expected %d, got %d", expected_count, j);

      K.t_int = LITTLENUMBER+10;
      expected_count = 0;
      for (ucl_map_upper_bound(M, I, K), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j);
      mcl_test_error_if_false(expected_count == j,
			      "wrong number of equal links, expected %d, got %d", expected_count, j);

    }
    clean(M);
  }
  mcl_test_end();
}

static void
test_set_iteration_intersection (void)
{
  ucl_iterator_t	I, IA, IB;
  ucl_map_t		A, B;
  link_t		L;
  ucl_value_t		K;
  int			j, first_A, first_B, upper_A, upper_B, step=1;

  mcl_test_begin("map-6.1.1", "set iteration, intersection, range in common") {
    /* fifteen elements in common (5-19) */
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 5;
      upper_A	= 20;
      upper_B	= 35;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_intersection(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == 20,
			      "wrong upper value for elements in common, expected %d got %d", 20, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.2", "set iteration, intersection, null intersection") {
    /* null intersection */
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 20;
      upper_A	= 10;
      upper_B	= 10;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_intersection(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iterator");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.3", "set iteration, intersection, full intersection") {
    /* full intersection */
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 0;
      upper_A	= 20;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_intersection(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_A,
			      "wrong upper value for full intersection, expected %d got %d",
			      upper_A, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.4", "set iteration, intersection, full inclusion of first map") {
    /* full inclusion of A */
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 5;
      upper_A	= 20;
      upper_B	= 15;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_intersection(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_B,
			      "wrong upper value for full intersection, expected %d got %d",
			      upper_B, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.5", "set iteration, intersection, full inclusion of second map") {
    /* full inclusion of B */
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 5;
      first_B	= 0;
      upper_A	= 15;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_intersection(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_A,
			      "wrong upper value for full intersection, expected %d got %d",
			      upper_A, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.6", "set iteration, intersection, intermixed values, null intersection") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      first_A	= 0;
      first_B	= 1;
      upper_A	= 20;
      upper_B	= 20;
      fill(A, first_A, upper_A, 2);
      fill(B, first_B, upper_B, 2);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_intersection(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
      for (ucl_map_iterator_intersection(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_debug("key %d", K.t_int);
      }
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.7", "set iteration, intersection, empty first map") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_B	= 0;
      upper_B	= 20;
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_intersection(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.8", "set iteration, intersection, empty second map") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= 20;
      fill(A, first_A, upper_A, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_intersection(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.1.9", "set iteration, intersection, empty maps") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_intersection(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();
}

static void
test_set_iteration_union (void)
{
  ucl_iterator_t	I, IA, IB;
  ucl_map_t		A, B;
  link_t		L;
  ucl_value_t		K;
  int			j, first_A, first_B, upper_A, upper_B, step=1;

  mcl_test_begin("map-6.2.1", "set iteration, union, adjacent A and B") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 20;
      upper_A	= 20;
      upper_B	= 40;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_B,
			      "wrong upper value for adjacent maps, expected %d got %d",
			      upper_B, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.2", "set iteration, union, adjacent B and A") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 20;
      first_B	= 0;
      upper_A	= 40;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_A,
			      "wrong upper value for adjacent maps, expected %d got %d",
			      upper_A, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.3", "set iteration, union, full overlapping") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 0;
      upper_A	= 20;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);

	mcl_test_error_if_false(ucl_iterator_more(I), "expected second value for %d", j);
	ucl_iterator_next(I);
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_A,
			      "wrong upper value for full overlapping, expected %d got %d",
			      upper_A, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.4", "set iteration, union, empty map A") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_B	= 0;
      upper_B	= 20;
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_B,
			      "wrong upper value for empty map A, expected %d got %d",
			      upper_B, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.5", "set iteration, union, empty map B") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= 20;
      fill(A, first_A, upper_A, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_B,
			      "wrong upper value for empty map B, expected %d got %d",
			      upper_B, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.6", "set iteration, union, empty maps") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_union(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration for empty maps");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.7", "set iteration, union, A included in B") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 10;
      first_B	= 0;
      upper_A	= 20;
      upper_B	= 40;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);

	if (first_A <= j && j < upper_A) {
	  mcl_test_error_if_false(ucl_iterator_more(I), "expected second value for %d", j);
	  ucl_iterator_next(I);
	  L = ucl_iterator_ptr(I);
	  K = getkey.func(getkey.data, L);
	  mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
	}
      }
      mcl_test_error_if_false(j == upper_B,
			      "wrong upper value for A included in B, expected %d got %d",
			      upper_B, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.8", "set iteration, union, B included in A") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      first_B	= 10;
      upper_A	= 40;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);

	if (first_B <= j && j < upper_B) {
	  mcl_test_error_if_false(ucl_iterator_more(I), "expected second value for %d", j);
	  ucl_iterator_next(I);
	  L = ucl_iterator_ptr(I);
	  K = getkey.func(getkey.data, L);
	  mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
	}
      }
      mcl_test_error_if_false(j == upper_A,
			      "wrong upper value for B included in A, expected %d got %d",
			      upper_A, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.2.9", "set iteration, union, intermixed values") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 2;
      first_A	= 0;
      first_B	= 1;
      upper_A	= 20;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_union(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(K.t_int == j, "wrong key value, expected %d got %d", j, K.t_int);
      }
      mcl_test_error_if_false(j == upper_B,
			      "wrong upper value, expected %d got %d", upper_B, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();
}

static void
test_set_iteration_complintersect (void)
{
  ucl_iterator_t	I, IA, IB;
  ucl_map_t		A, B;
  link_t		L;
  ucl_value_t		K;
  int			key_A, key_B;
  int			j, first_A, first_B, upper_A, upper_B, step=1, results_count;

  mcl_test_begin("map-6.3.1", "set iteration, complementary intersection, first overlaps second") {
#undef SIZE
#define SIZE	6
    static const int expected_results[SIZE] = { 0, 1, 2, 3, 4, 10 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= 10;
      first_B	= 5;
      upper_B	= 11;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("I key %d, IA key %d, IB key %d", K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.2", "set iteration, complementary intersection, second overlaps second") {
#undef SIZE
#define SIZE	6
    static const int expected_results[SIZE] = { 0, 1, 2, 3, 4, 10 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 5;
      upper_A	= 11;
      first_B	= 0;
      upper_B	= 10;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("I key %d, IA key %d, IB key %d", K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.3", "set iteration, complementary intersection, full overlapping") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= SIZE;
      first_B	= 0;
      upper_B	= SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_complintersect(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iterator in full overlapping");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.4", "set iteration, complementary intersection, empty first map") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_B	= 0;
      upper_B	= SIZE;
      results_count = SIZE;
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == j,
				  "wrong key value, expected %d got %d", j, K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d", results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results_count value, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.5", "set iteration, complementary intersection, empty second map") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= SIZE;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == j,
				  "wrong key value, expected %d got %d", j, K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results_count %d got %d", results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.6", "set iteration, complementary intersection, empty maps") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_complintersect(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration for empty maps");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.7", "set iteration, complementary intersection, first map included") {
#undef SIZE
#define SIZE	10
    static const int expected_results[SIZE] = { 0, 1, 2, 3, 4, 15, 16, 17, 18, 19 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 5;
      upper_A	= 15;
      first_B	= 0;
      upper_B	= 20;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_B; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("I key %d, IA key %d, IB key %d", K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d", results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.8", "set iteration, complementary intersection, second map included") {
#undef SIZE
#define SIZE	10
    static const int expected_results[SIZE] = { 0, 1, 2, 3, 4, 15, 16, 17, 18, 19 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= 20;
      first_B	= 5;
      upper_B	= 15;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("I key %d, IA key %d, IB key %d", K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d", results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.3.9", "set iteration, complementary intersection, intermixed values") {
#undef SIZE
#define SIZE	20
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 2;
      first_A	= 0;
      upper_A	= 20;
      first_B	= 1;
      upper_B	= 20;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_complintersect(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("I key %d, IA key %d, IB key %d", K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == j,
				  "wrong key value, expected %d got %d", j, K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d", results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();
}

static void
test_set_iteration_subtraction (void)
{
  ucl_iterator_t	I, IA, IB;
  ucl_map_t		A, B;
  link_t		L;
  ucl_value_t		K;
  int			key_A, key_B;
  int			j, first_A, first_B, upper_A, upper_B, step=1, results_count;

  mcl_test_begin("map-6.4.1", "set iteration, subtraction, first map overlaps second") {
#undef SIZE
#define SIZE	5
    static const int expected_results[SIZE] = { 0, 1, 2, 3, 4 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= 10;
      first_B	= 5;
      upper_B	= 11;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_subtraction(IA, IB, I), j = first_A; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("I key %d, IA key %d, IB key %d", K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.2", "set iteration, subtraction, second map overlaps first") {
#undef SIZE
#define SIZE	5
    static const int expected_results[SIZE] = { 10, 11, 12, 13, 14 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 5;
      upper_A	= 15;
      first_B	= 0;
      upper_B	= 10;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_subtraction(IA, IB, I), j = 0; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("j=%d, I key %d, IA key %d, IB key %d", j, K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d\n",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.3", "set iteration, subtraction, full overlapping") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= SIZE;
      first_B	= 0;
      upper_B	= SIZE;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_subtraction(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.4", "set iteration, subtraction, empty first map") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_B	= 0;
      upper_B	= SIZE;
      results_count = SIZE;
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_subtraction(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.5", "set iteration, subtraction, empty secong map") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_B	= SIZE;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_subtraction(IA, IB, I), j = 0; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("j=%d, I key %d, IA key %d, IB key %d", j, K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == j,
				  "wrong key value, expected %d got %d", j, K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d\n",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.6", "set iteration, subtraction, empty maps") {
#undef SIZE
#define SIZE	10
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_subtraction(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.7", "set iteration, subtraction, first map included in second") {
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 5;
      upper_A	= 15;
      first_B	= 0;
      upper_B	= 20;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      ucl_map_iterator_subtraction(IA, IB, I);
      mcl_test_error_if_true(ucl_iterator_more(I), "expected empty iteration");
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.8", "set iteration, subtraction, second map included in first") {
#undef SIZE
#define SIZE	10
    static const int expected_results[SIZE] = { 0, 1, 2, 3, 4, 15, 16, 17, 18, 19 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 1;
      first_A	= 0;
      upper_A	= 20;
      first_B	= 5;
      upper_B	= 15;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_subtraction(IA, IB, I), j = 0; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("j=%d, I key %d, IA key %d, IB key %d", j, K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d\n",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();

  mcl_test_begin("map-6.4.9", "set iteration, subtraction, intermixed values") {
#undef SIZE
#define SIZE	10
    static const int expected_results[SIZE] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
    ucl_map_initialise(A, 0, ucl_compare_int, getkey);
    ucl_map_initialise(B, 0, ucl_compare_int, getkey);
    {
      step	= 2;
      first_A	= 0;
      upper_A	= 20;
      first_B	= 1;
      upper_B	= 20;
      results_count = SIZE;
      fill(A, first_A, upper_A, step);
      fill(B, first_B, upper_B, step);
      ucl_map_iterator_inorder(A, IA);
      ucl_map_iterator_inorder(B, IB);
      for (ucl_map_iterator_subtraction(IA, IB, I), j = 0; ucl_iterator_more(I);
	   ucl_iterator_next(I), ++j) {
	L = ucl_iterator_ptr(I);
	K = getkey.func(getkey.data, L);
	key_A = ucl_iterator_more(IA)? getkey.func(getkey.data,ucl_iterator_ptr(IA)).t_int : -1;
	key_B = ucl_iterator_more(IB)? getkey.func(getkey.data,ucl_iterator_ptr(IB)).t_int : -1;
	mcl_debug("j=%d, I key %d, IA key %d, IB key %d", j, K.t_int, key_A, key_B);
	if (j < results_count)
	  mcl_test_error_if_false(K.t_int == expected_results[j],
				  "wrong key value, expected %d got %d", expected_results[j], K.t_int);
	else
	  mcl_test_error("index value out of bounds, expected results count %d got %d\n",
			 results_count, j);
      }
      mcl_test_error_if_false(j == results_count,
			      "wrong results count, expected %d got %d", results_count, j);
    }
    clean(A);
    clean(B);
  }
  mcl_test_end();
}

static void
test_first_last_next_prev (void)
{
  mcl_test_begin("map-10.1", "first, last, next and prev link") {
    ucl_map_t	M;
    link_t	L;
    ucl_value_t	K;
    int		j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (j=0; j<LITTLENUMBER; ++j)
	ucl_map_insert(M, alloc_link(j));

      L = ucl_map_first(M);
      mcl_test_error_if_false(NULL != L, "NULL first link");
      K = getkey.func(getkey.data, L);
      mcl_test_error_if_false(0 == K.t_int, "wrong first key value, expected %d got %d", 0, K.t_int);

      L = ucl_map_last(M);
      mcl_test_error_if_false(NULL != L, "NULL first link");
      K = getkey.func(getkey.data, L);
      mcl_test_error_if_false(LITTLENUMBER-1 == K.t_int,
			      "wrong last key value, expected %d got %d",
			      LITTLENUMBER-1, K.t_int);


      L = ucl_map_first(M);
      for (j=1; j<LITTLENUMBER; ++j) {
	L = ucl_map_next(L);
	mcl_test_error_if_false(NULL != L, "NULL first link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(j == K.t_int, "wrong next key value, expected %d got %d", j, K.t_int);
      }
      L = ucl_map_next(L);
      mcl_test_error_if_false(NULL == L, "wrong last next, expected NULL got %p", (void *)L);

      L = ucl_map_last(M);
      for (j=LITTLENUMBER-2; j>=0; --j) {
	L = ucl_map_prev(L);
	mcl_test_error_if_false(NULL != L, "NULL prev link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(j == K.t_int, "wrong prev key value, expected %d got %d", j, K.t_int);
      }
      L = ucl_map_prev(L);
      mcl_test_error_if_false(NULL == L, "wrong last prev, expected NULL got %p", (void *)L);
    }
    clean(M);
  }
  mcl_test_end();
}

static void
test_find_or_next_prev (void)
{
  mcl_test_begin("map-10.2", "find or next/prev") {
    ucl_map_t	M;
    link_t	L;
    ucl_value_t	K;
    int		j;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (j=0; j<=100; j+=10)
	ucl_map_insert(M, alloc_link(j));

      j = -10;
      {
	K.t_int = j;
	L = ucl_map_find_or_next(M, K);
	mcl_test_error_if_false(NULL != L, "NULL next link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(0 == K.t_int, "wrong next key value, expected %d got %d", 0, K.t_int);

	K.t_int = j;
	L = ucl_map_find_or_prev(M, K);
	mcl_test_error_if_false(NULL == L, "expected NULL prev link");
      }

      j = 45;
      {
	K.t_int = j;
	L = ucl_map_find_or_next(M, K);
	mcl_test_error_if_false(NULL != L, "NULL next link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(50 == K.t_int, "wrong next key value, expected %d got %d", 50, K.t_int);

	K.t_int = j;
	L = ucl_map_find_or_prev(M, K);
	mcl_test_error_if_false(NULL != L, "NULL prev link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(40 == K.t_int, "wrong prev key value, expected %d got %d", 40, K.t_int);
      }

      j = 55;
      {
	K.t_int = j;
	L = ucl_map_find_or_next(M, K);
	mcl_test_error_if_false(NULL != L, "NULL next link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(60 == K.t_int, "wrong next key value, expected %d got %d", 60, K.t_int);

	K.t_int = j;
	L = ucl_map_find_or_prev(M, K);
	mcl_test_error_if_false(NULL != L, "NULL prev link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(50 == K.t_int, "wrong prev key value, expected %d got %d", 50, K.t_int);
      }

      j = 110;
      {
	K.t_int = j;
	L = ucl_map_find_or_next(M, K);
	mcl_test_error_if_false(NULL == L, "expected NULL next link");

	K.t_int = j;
	L = ucl_map_find_or_prev(M, K);
	mcl_test_error_if_false(NULL != L, "NULL prev link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(100 == K.t_int, "wrong prev key value, expected %d got %d", 100, K.t_int);
      }

      for (j = 51; j<60; ++j) {
	int	next = 60, prev = 50;

	K.t_int = j;
	L = ucl_map_find_or_next(M, K);
	mcl_test_error_if_false(NULL != L, "NULL next link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(next == K.t_int,
				"wrong next key value, expected %d got %d", next, K.t_int);

	K.t_int = j;
	L = ucl_map_find_or_prev(M, K);
	mcl_test_error_if_false(NULL != L, "NULL prev link");
	K = getkey.func(getkey.data, L);
	mcl_test_error_if_false(prev == K.t_int,
				"wrong prev key value, expected %d got %d", prev, K.t_int);
      }

    }
    clean(M);
  }
  mcl_test_end();
}

static void
test_count (void)
{
  mcl_test_begin("map-10.3", "count elements in simple map") {
    ucl_map_t	M;
    ucl_value_t	K;
    int		j;
    size_t	count;
    ucl_map_initialise(M, 0, ucl_compare_int, getkey);
    {
      for (j=0; j<=LITTLENUMBER; ++j)
	ucl_map_insert(M, alloc_link(j));

      for (j=0; j<=LITTLENUMBER; ++j) {
	K.t_int = j;
	count = ucl_map_count(M, K);
	mcl_test_error_if_false(1 == count, "wrong count of %d, expected %u got %u", j, 1, count);
      }

    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-10.4", "count elements in multi map") {
    ucl_map_t	M;
    ucl_value_t	K;
    int		j;
    size_t	count;
    ucl_map_initialise(M, UCL_ALLOW_MULTIPLE_OBJECTS, ucl_compare_int, getkey);
    {
      fill_multimap(M, LITTLENUMBER, 0, 5, 3);

      for (j=1; j<=LITTLENUMBER; ++j) {
	K.t_int = j;
	count = ucl_map_count(M, K);
	mcl_test_error_if_false(1 == count, "wrong count of %d, expected %u got %u", j, 1, count);
	if (0 == (1+j) % 5) ++j;
      }

      for (j=0; j<=LITTLENUMBER; j+=5) {
	K.t_int = j;
	count = ucl_map_count(M, K);
	mcl_test_error_if_false(3 == count, "wrong count of %d, expected %u got %u", j, 3, count);
      }
    }
    clean(M);
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
  test_checked_inorder_insertion ();
  test_checked_inverse_inorder_insertion ();
  test_checked_insertion_random ();
  test_multimap_insert_and_find ();

  mcl_test_subtitle("insertion and deletion");
  test_inorder_insertion_and_deletion ();
  test_inverse_inorder_insertion_and_deletion ();

  mcl_test_subtitle("iterators");
  test_inorder_iteration ();

  mcl_test_subtitle("multimap iterators");
  test_lower_bound_multimap_iterator ();
  test_upper_bound_multimap_iterator ();

  mcl_test_subtitle("set iterators");
  test_set_iteration_intersection ();
  test_set_iteration_union ();
  test_set_iteration_complintersect ();
  test_set_iteration_subtraction ();

  mcl_test_subtitle("miscellaneous functions");
  test_first_last_next_prev ();
  test_find_or_next_prev ();
  test_count ();

  exit(EXIT_SUCCESS);
}

/* end of file */
