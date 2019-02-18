/*
  Part of: Useless Containers Library
  Contents: tests for raw binary search trees
  Date: Mon Dec  6, 2010

  Abstract



  Copyright (c) 2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#define CCTESTS_DEBUGGING	1
#define NUMBER			50
#define LITTLENUMBER		10

#include "ucl.h"
#include <cctests.h>


/** --------------------------------------------------------------------
 ** Link type definitions.
 ** ----------------------------------------------------------------- */

typedef struct link_tag_t {
  ucl_node_tag_t	node;
  ucl_value_t		key;
} link_tag_t;

typedef link_tag_t *	link_t;

static ucl_value_t
link_key (ucl_value_t context UCL_UNUSED, void * ell_)
{
  link_t	ell = ell_;
  return ell->key;
}

static const ucl_node_getkey_t getkey = {
  .data = { .pointer = NULL },
  .func	= link_key
};

/* to be used as right-side in assignments */
#define DAD_OF(ell)		((void *)(((link_t)(ell))->node.dad))
#define SON_OF(ell)		((void *)(((link_t)(ell))->node.son))
#define BRO_OF(ell)		((void *)(((link_t)(ell))->node.bro))

/* to be used as left-side in assignments */
#define DAD(ell)		(((link_t)(ell))->node.dad)
#define SON(ell)		(((link_t)(ell))->node.son)
#define BRO(ell)		(((link_t)(ell))->node.bro)

#define STATUS(ell)		(((link_t)ell)->node.meta.avl_status)
#define STATUS_STRING(S)	((UCL_EQUAL_DEPTH==S)? "equal-depth": \
				 ((UCL_SON_DEEPER==S)? "son-deeper":\
				  ((UCL_BRO_DEEPER==S)? "bro-deeper": "corrupted")))

#define KEY(ell)			(((link_t)(ell))->key.t_int)
#define MAYBE_KEY(ell)		((ell)? KEY(ell) : -1)


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static link_t
alloc_link (int value)
{
  ucl_memory_allocator_t A = ucl_memory_allocator;
  link_t 	ell = NULL;
  ucl_value_t	K = { .t_int = value };
  A.alloc(A.data, &ell, sizeof(link_tag_t));
  ell->key = K;
  return ell;
}
static void
free_link (link_t  ell)
{
  ucl_memory_allocator_t A = ucl_memory_allocator;
  A.alloc(A.data, &ell, 0);
}
static void *
find (cce_destination_t L, void * root, int key)
{
  ucl_value_t	K = { .t_int = key };
  link_t	ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
  cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", key);
  return ell;
}
static void UCL_UNUSED
print_inorder (void * root)
{
  ucl_iterator_t	I;
  ucl_value_t		K;
  link_t		ell;
  int			i;
  for (ucl_btree_iterator_inorder(I, root), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
    ell = ucl_iterator_ptr(I);
    K = ell->key;
    fprintf(stderr, "i=%d %d\n", i, K.t_int);
  }
}


/** --------------------------------------------------------------------
 ** Test inorder insertion, finding, removing.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    link_t		root, root1, ell;
    ucl_value_t		K;
    int			j, k;
    ucl_bool_t		print=false;

    j=0;
    root = alloc_link(j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    /* insertion */
    for (j=1; j<LITTLENUMBER; ++j) {
      cctests_debug_print("inserting %d", j);
      ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    }
    if (print) print_inorder(root);
    /* finding */
    for (j=1; j<LITTLENUMBER; ++j) {
      cctests_debug_print("finding %d", j);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    }
    /* deleting */
    for (j=1; j<LITTLENUMBER; ++j) {
      cctests_debug_print("deleting %d", j);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
      root1 = ucl_bst_delete(root, ell);
      free_link(ell);
      cctests_assert_msg(L, root == root1, "wrong root after deletion");
      if (print) print_inorder(root);
      for (k=1+j; k<LITTLENUMBER; ++k) {
	K.t_int = k;
	ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
	cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", k);
      }
    }

    j=0;
    cctests_debug_print("deleting %d", j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, ell);
    cctests_assert_msg(L, NULL == root1, "wrong root after deletion");

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Reverse inorder insertion, finding and deletion.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    link_t		root, root1, ell;
    ucl_value_t		K;
    int			j, k;
    ucl_bool_t		print=false;

    j=LITTLENUMBER;
    root = alloc_link(j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    /* insertion */
    for (j=LITTLENUMBER-1; j>0; --j) {
      cctests_debug_print("inserting %d", j);
      ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    }
    if (print) print_inorder(root);
    /* finding */
    for (j=LITTLENUMBER-1; j>0; --j) {
      cctests_debug_print("finding %d", j);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    }
    /* deleting */
    for (j=LITTLENUMBER-1; j>0; --j) {
      cctests_debug_print("deleting %d", j);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
      root1 = ucl_bst_delete(root, ell);
      free_link(ell);
      cctests_assert_msg(L, root == root1, "wrong root after deletion");
      if (print) print_inorder(root);
      for (k=j-1; k>0; --k) {
	K.t_int = k;
	ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
	cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", k);
      }
    }

    j=LITTLENUMBER;
    cctests_debug_print("deleting %d", j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, ell);
    cctests_assert_msg(L, NULL == root1, "wrong root after deletion");

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Mixed insertion, finding and deletion.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    link_t		root, root1, ell;
    ucl_value_t		K;
    int			j;
    ucl_bool_t		print=false;

    /*
     *    5-----7--8--9
     *    |     |
     *    2--4  6
     *    |  |
     *    1  3
     *    |
     *    0
     */
    j=5;
    root = alloc_link(j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=2;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=1;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=0;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=4;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=3;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=7;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=6;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=8;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    j=9;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);

    if (print) print_inorder(root);
    /* finding */
    for (j=0; j<LITTLENUMBER; ++j) {
      cctests_debug_print("finding %d", j);
      K.t_int = j;
      ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
      cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    }

    /* Removing 7:
     *
     *    5-----7--8--9      5-----6--8--9
     *    |     |            |
     *    2--4  6            2--4
     *    |  |           =>  |  |
     *    1  3               1  3
     *    |                  |
     *    0                  0
     */
    j=7;
    cctests_debug_print("deleting %d", j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, ell);
    free_link(ell);
    cctests_assert_msg(L, root == root1, "wrong root after deletion");
    if (print) print_inorder(root);
    find(L, root, 0);
    find(L, root, 1);
    find(L, root, 2);
    find(L, root, 3);
    find(L, root, 4);
    find(L, root, 5);
    find(L, root, 6);
    find(L, root, 8);
    find(L, root, 9);

    /* Removing 2:
     *
     *    5-----6--8--9      5-----6--8--9
     *    |                  |
     *    2--4               1--4
     *    |  |           =>  |  |
     *    1  3               0  3
     *    |
     *    0
     */
    j=2;
    cctests_debug_print("deleting %d", j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, ell);
    free_link(ell);
    cctests_assert_msg(L, root == root1, "wrong root after deletion");
    if (print) print_inorder(root);
    find(L, root, 0);
    find(L, root, 1);
    find(L, root, 3);
    find(L, root, 4);
    find(L, root, 5);
    find(L, root, 6);
    find(L, root, 8);
    find(L, root, 9);

    /* Removing 5:
     *
     *    5--6--8--9     1--4--6--8--9
     *    |              |  |
     *    1--4        => 0  3
     *    |  |
     *    0  3
     *
     */
    j=5;
    cctests_debug_print("deleting %d", j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, ell);
    free_link(ell);
    cctests_assert_msg(L, root != root1, "wrong root after deletion");
    root = root1;
    if (print) print_inorder(root);
    find(L, root, 0);
    ell = find(L, root, 1);
    cctests_assert_msg(L, ell == root, "expected root with key %d got %d", 1, ell->key);
    find(L, root, 3);
    find(L, root, 4);
    find(L, root, 6);
    find(L, root, 8);
    find(L, root, 9);

    /* Removing 1:
     *
     *    1--4--6--8--9      0--4--6--8--9
     *    |  |           =>     |
     *    0  3                  3
     *
     */
    j=1;
    cctests_debug_print("deleting %d", j);
    K.t_int = j;
    ell = ucl_bst_find(root, K, ucl_compare_int, getkey);
    cctests_assert_msg(L, NULL != ell, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, ell);
    free_link(ell);
    cctests_assert_msg(L, root != root1, "wrong root after deletion");
    root = root1;
    if (print) print_inorder(root);
    ell = find(L, root, 0);
    cctests_assert_msg(L, ell == root, "expected root with key %d got %d", 0, ell->key);
    find(L, root, 3);
    find(L, root, 4);
    find(L, root, 6);
    find(L, root, 8);
    find(L, root, 9);

    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  //ucl_library_init();

  cctests_init("tests for binary search trees");
  {
    cctests_begin_group("insertion, finding, removing");
    {
      cctests_run(test_1_1);
    }
    cctests_end_group();

    cctests_begin_group("reverse inorder insertion, finding and deletion.");
    {
      cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("reverse inorder insertion, finding and deletion.");
    {
      cctests_run(test_3_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
