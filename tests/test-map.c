/*
  Part of: Useless Container Library
  Contents: test for the Map container
  Date: Sat Feb  8, 2003

  Abstract

	Removing nodes from 0 to 9 produces these trees:

  * remove 0:

  |      3-----7--8--9       3-----7--8--9
  |      |     |             |     |
  |      1--2  5--6      =>  1--2  5--6
  |      |     |                   |
  |      0     4                   4

  * remove 1 (right-left rotation raising 5):

  |      3-----7--8--9       3-----7--8--9     5------7--8--9
  |      |     |             |     |           |      |
  |      1--2  5--6      =>  2     5--6     => 3--4   6
  |            |                   |           |
  |            4                   4           2

  * remove 2:

  |      5------7--8--9      5-----7--8--9
  |      |      |            |     |
  |      3--4   6            3--4  6
  |      |
  |      2

  * remove 3 (right rotation raising 7):

  |      5-----7--8--9       7--8--9
  |      |     |             |
  |      4     6         =>  5--6
  |                          |
  |                          4

  * remove 4:

  |      7--8--9             7--8--9
  |      |                   |
  |      5--6            =>  5--6
  |      |
  |      4

  * remove 5:

  |      7--8--9
  |      |
  |      6

  * remove 6 (right rotation raising 8):

  |      7--8--9             8--9
  |                      =>  |
  |                          7

  * remove 7, 8 and 9 is just detaching them.


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

#define A		ucl_memory_allocator

/* to be used as right-side in assignments */
#define DAD_OF(L)		((void *)((L)->node.dad))
#define SON_OF(L)		((void *)((L)->node.son))
#define BRO_OF(L)		((void *)((L)->node.bro))

/* to be used as left-side in assignments */
#define DAD(L)			((L)->node.dad)
#define SON(L)			((L)->node.son)
#define BRO(L)			((L)->node.bro)

#define STATUS(L)		(((ucl_node_t)L)->meta.avl_status)
#define STATUS_STRING(S)	((UCL_EQUAL_DEPTH==S)? "equal-depth": \
				 ((UCL_SON_DEEPER==S)? "son-deeper":\
				  ((UCL_BRO_DEEPER==S)? "bro-deeper": "corrupted")))

#define KEY(L)			(ucl_map_getkey(L).t_int)
#define MAYBE_KEY(L)		((L)? KEY(L) : -1)


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
  for (ucl_map_link_t  L = ucl_map_first(M); L; L = ucl_map_first(M)) {
    ucl_map_remove(M, L);
    free_link(L);
  }
  assert(0 == ucl_map_size(M));
}

/** --------------------------------------------------------------------
 ** Validating routines.
 ** ----------------------------------------------------------------- */

static void
checked_insertion (ucl_map_t M, int j, ucl_map_link_t * L)
/* Insert a new node with key  "j" and validate the existence of the new
   node  by  finding  it.   Store  the  node  pointer  in  the  location
   referenced by "L". */
{
  ucl_value_t	K;
  size_t	old_size=ucl_map_size(M);
  *L = alloc_link(j);
  ucl_map_insert(M, *L);
  mcl_test_error_if_false(1+old_size == ucl_map_size(M), "invalid size after insertion of %d", j);
  K.t_int = j;
  *L = ucl_map_find(M, K);
  mcl_test_error_if_false(NULL!=*L, "unable to find node with key %d", j);
  K = ucl_map_getkey(*L);
  mcl_test_error_if_false(j == K.t_int, "invalid key value, expected %d got %d", j, K.t_int);
}
static void
validate_links (ucl_map_t M, int j, void * dad, void * son, void * bro, ucl_avl_status_t expected_status)
/* Extract from "M" the node with  key "j", the validate its links to be
   the  given ones,  finally validate  its AVL  status to  be  the given
   one. */
{
  ucl_value_t		K = { .t_int = j };
  ucl_map_link_t	L = ucl_map_find(M, K);
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

/** --------------------------------------------------------------------
 ** Test trees.
 ** ----------------------------------------------------------------- */

static void
build_base_tree_with_plenty_of_holes (ucl_map_t M, ucl_map_link_t * L)
{
  int		j;

  /* add 0 and 10:

     |
     |      0b--10=
     |
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

     |      0--10--20     10=--20=
     |                 => |
     |                    0=

  */
  j=20;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M, 10, NULL,  L[0], L[20], UCL_EQUAL_DEPTH);
  validate_links(M,  0, L[10], NULL, NULL, UCL_EQUAL_DEPTH);
  validate_links(M, 20, L[10], NULL, NULL, UCL_EQUAL_DEPTH);

  /* add 30:

     |      10b--20b--30=
     |      |
     |      0=
  */
  j=30;
  mcl_debug("inserting %d", j);
  checked_insertion(M, j, &L[j]);
  validate_links(M, 10, NULL,  L[0], L[20], UCL_BRO_DEEPER);
  validate_links(M,  0, L[10], NULL, NULL,  UCL_EQUAL_DEPTH);
  validate_links(M, 20, L[10], NULL, L[30], UCL_BRO_DEEPER);
  validate_links(M, 30, L[20], NULL, NULL,  UCL_EQUAL_DEPTH);

  /* add 40 (bro rotation raising 30):

     |      10--20--30--40           10b--30=--40=
     |      |                =>      |     |
     |      0                        0=   20=
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

/** --------------------------------------------------------------------
 ** Printing functions.
 ** ----------------------------------------------------------------- */

static void
print_inorder (ucl_map_t M)
{
  ucl_iterator_t	I;
  ucl_value_t		K;
  ucl_map_link_t	L;
  int			i;
  for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
    L = ucl_iterator_ptr(I);
    K = ucl_map_getkey(L);
    fprintf(stderr, "i=%d %d\n", i, K.t_int);
  }
}
static void
print_preorder_links (ucl_map_t M, const char * message, ...)
{
  va_list		ap;
  ucl_iterator_t	I;
  ucl_map_link_t	L, dad, son, bro;
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
      fprintf(stderr, "%2d <-", ucl_map_getkey(dad).t_int);
    else
      fprintf(stderr, "nn <-");

    fprintf(stderr, "%2d -> ", ucl_map_getkey(L).t_int);

    if (son)
      fprintf(stderr, "%2d, ", ucl_map_getkey(son).t_int);
    else
      fprintf(stderr, "nn, ");

    if (bro)
      fprintf(stderr, "%2d", ucl_map_getkey(bro).t_int);
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
    ucl_map_initialise(M, 0, ucl_compare_int_pointer);
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
    ucl_map_initialise(M, 0, ucl_compare_int);
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
	K = ucl_map_getkey(L);
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
    ucl_map_link_t 	L[10];
    int			j;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      /* Inserting  10 numbers  from 0  to 9  produces these  trees (AVL
	 status  at the  side of  the number:  "s" son  deeper,  "b" bro
	 deeper, "=" equal depth):

	 * add 0 and 1:

	 |
	 |      0b--1=
	 |
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

       |      0--1--2     1=--2=
       |               => |
       |                  0=

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

       |      1b--2b--3=
       |      |
       |      0=
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

       |      1--2--3--4           1b--3=--4=
       |      |            =>      |   |
       |      0                    0=  2=
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

       |      1--3--4--5           3=--4b--5=
       |      |  |         =>      |
       |      0  2                 1=--2=
       |                           |
       |                           0=
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

       |      3--4--5--6           3=------5=--6=
       |      |                    |       |
       |      1--2         =>      1=--2=  4=
       |      |                    |
       |      0                    0=
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

       |      3----5--6--7
       |      |    |
       |      1--2 4
       |      |
       |      0
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

       |      3----5--6--7--8    3b------5b--7=--8=
       |      |    |             |       |   |
       |      1--2 4          => 1=--2=  4=  6=
       |      |                  |
       |      0                  0=
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

       |      3-----5--7--8--9    3b------7=--8b--9=
       |      |     |  |          |       |
       |      1--2  4  6      =>  1=--2=  5=--6=
       |      |                   |       |
       |      0                   0=      4=

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
    ucl_map_link_t 	L[10];
    int			j;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      /* Inserting  10 numbers  from 0  to 9  produces these  trees (AVL
	 status  at the  side of  the number:  "s" son  deeper,  "b" bro
	 deeper, "=" equal depth):

	 * add 9 and 8:

	 |
	 |      9s
	 |	|
	 |	8=
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

       |      9            8=--9=
       |      |         => |
       |      8            7=
       |      |
       |      7

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

       |      8s--9=
       |      |
       |      7s
       |      |
       |      6=
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

       |      8s--9=       8s--9=
       |      |            |
       |      7s           6=--7=
       |      |        =>  |
       |      6=           5=
       |      |
       |      5
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

       |      8s--9=         6=--8=--9=
       |      |              |   |
       |      6=--7=         5s  7=
       |      |         =>   |
       |      5=             4=
       |      |
       |      4
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

       |      6=--8=--9=       6=------8=--9=
       |      |   |            |       |
       |      5s  7=       =>  4=--5=  7=
       |      |                |
       |      4=               3=
       |      |
       |      3
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

       |      6s------8=--9=
       |      |       |
       |      4s--5=  7=
       |      |
       |      3s
       |      |
       |      2=
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

       |      6s------8=--9=       6s------8=--9=
       |      |       |            |       |
       |      4s--5=  7=           4s--5=  7=
       |      |                    |
       |      3s               =>  2=--3=
       |      |                    |
       |      2=                   1=
       |      |
       |      1
      */
      mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
			      "unbalanced tree after insertion of %d", j);

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

       |      6s------8=--9=        6s-----------8=--9=
       |      |       |             |            |
       |      4s--5=  7=            2=--4=--5=   7=
       |      |                     |   |
       |      2=--3=            =>  1s  3=
       |      |                     |
       |      1=                    0=
       |      |
       |      0
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
    ucl_map_t		M;
    ucl_map_link_t 	L[SIZE];
    int			j;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      mcl_test_error_if_false(0 == ucl_map_size(M), "invalid size after construction");
      memset(L, '\0', SIZE*sizeof(ucl_map_link_t));
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
test_insertion_and_removal (void)
{
  mcl_test_begin("map-3.1", "multiple insertion and removal") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_value_t		K;
    int			i;
    ucl_bool_t		print=false;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      for (i=0; i< LITTLENUMBER; ++i) {
	ucl_map_insert(M, alloc_link(i));
	if (print) print_preorder_links (M, "after inserting link %d", i);
	mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
				"unbalanced tree after insertion of %d", i);
      }
      assert(LITTLENUMBER == ucl_map_size(M));
      for (i=0; i<LITTLENUMBER; ++i) {
	mcl_debug("removing %d", i);
	if (print || i==4 || i==3) print_preorder_links (M, "before removing link %d", i);
      	K.t_int = i;
      	L = ucl_map_find(M, K);
      	ucl_map_remove(M, L);
      	K = ucl_map_getkey(L);
      	assert(K.t_int == i);
      	free_link(L);
	mcl_test_error_if_false(ucl_btree_avl_is_balanced(M->root),
				"unbalanced tree after deletion of %d", i);
	if (print || i==3) print_preorder_links (M, "after removing link %d", i);
      }
    }
  }
  mcl_test_end();
}

static void
test_inorder_iteration (void)
{
  mcl_test_begin("map-4.1", "inorder iteration, few nodes inorder insertion") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      for (i=0; i<LITTLENUMBER; ++i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == LITTLENUMBER);
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = ucl_map_getkey(L);
	assert(i == K.t_int);
      }
      assert(i == LITTLENUMBER);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.2", "inorder iteration, few nodes reverse inorder insertion") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      for (i=LITTLENUMBER; i>0; --i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == LITTLENUMBER);
      for (ucl_map_iterator_inorder(M, I), i=1; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = ucl_map_getkey(L);
	assert(i == K.t_int);
      }
      assert(i == LITTLENUMBER+1);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.3", "inorder iteration, mixed insertion order") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
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
	K = ucl_map_getkey(L);
	assert(inorder[i] == K.t_int);
      }
      assert(i == SIZE);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.4", "inorder iteration, mixed insertion order") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
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
	K = ucl_map_getkey(L);
	assert(inorder[i] == K.t_int);
      }
      assert(i == SIZE);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.5", "inorder iteration, many numbers inorder insertion") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      for (i=0; i<NUMBER; ++i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == NUMBER);
      for (ucl_map_iterator_inorder(M, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = ucl_map_getkey(L);
	assert(i == K.t_int);
      }
      assert(i == NUMBER);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.6", "inorder iteration, many numbers reverse inorder insertion") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
    {
      for (i=NUMBER; i>0; --i) {
	L = alloc_link(i);
	ucl_map_insert(M, L);
      }
      assert(ucl_map_size(M) == NUMBER);
      for (ucl_map_iterator_inorder(M, I), i=1; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	L = ucl_iterator_ptr(I);
	K = ucl_map_getkey(L);
	assert(i == K.t_int);
      }
      assert(i == NUMBER+1);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.7", "inorder iteration, mixed insertion") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
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
	K = ucl_map_getkey(L);
	assert(i == K.t_int);
      }
      assert(i == 40);
    }
    clean(M);
  }
  mcl_test_end();

  mcl_test_begin("map-4.8", "inorder iteration, mixed insertion") {
    ucl_map_t		M;
    ucl_map_link_t	L;
    ucl_iterator_t	I;
    ucl_value_t		K;
    int			i;
    ucl_map_initialise(M, 0, ucl_compare_int);
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
	K = ucl_map_getkey(L);
	assert(i == K.t_int);
      }
      assert(i == 41);
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

  mcl_test_subtitle("insertion and removal");
  test_insertion_and_removal ();

  mcl_test_subtitle("iterators");
  test_inorder_iteration ();

  exit(EXIT_SUCCESS);
}

/* end of file */
