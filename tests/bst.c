/*
  Part of: Useless Containers Library
  Contents: tests for raw binary search trees
  Date: Mon Dec  6, 2010

  Abstract



  Copyright (c) 2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
static void *
find (void * root, int key)
{
  ucl_value_t	K = { .t_int = key };
  link_t	L = ucl_bst_find(root, K, ucl_compare_int, getkey);
  mcl_test_error_if_false(NULL != L, "unable to find node with key %d", key);
  return L;
}
static void UCL_UNUSED
print_inorder (void * root)
{
  ucl_iterator_t	I;
  ucl_value_t		K;
  link_t		L;
  int			i;
  for (ucl_btree_iterator_inorder(I, root), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
    L = ucl_iterator_ptr(I);
    K = L->key;
    fprintf(stderr, "i=%d %d\n", i, K.t_int);
  }
}

static void
test_inorder_insert_find_remove (void)
{
  mcl_test_begin("bst-1.1", "inorder insertion, finding and deletion") {
    link_t		root, root1, L;
    ucl_value_t		K;
    int			j, k;
    ucl_bool_t		print=false;

    j=0;
    root = alloc_link(j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    /* insertion */
    for (j=1; j<LITTLENUMBER; ++j) {
      mcl_debug("inserting %d", j);
      ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    }
    if (print) print_inorder(root);
    /* finding */
    for (j=1; j<LITTLENUMBER; ++j) {
      mcl_debug("finding %d", j);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    }
    /* deleting */
    for (j=1; j<LITTLENUMBER; ++j) {
      mcl_debug("deleting %d", j);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
      root1 = ucl_bst_delete(root, L);
      free_link(L);
      mcl_test_error_if_false(root == root1, "wrong root after deletion");
      if (print) print_inorder(root);
      for (k=1+j; k<LITTLENUMBER; ++k) {
	K.t_int = k;
	L = ucl_bst_find(root, K, ucl_compare_int, getkey);
	mcl_test_error_if_false(NULL != L, "unable to find node with key %d", k);
      }
    }

    j=0;
    mcl_debug("deleting %d", j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, L);
    mcl_test_error_if_false(NULL == root1, "wrong root after deletion");
  }
  mcl_test_end();
}

static void
test_reverse_inorder_insert_find_remove (void)
{
  mcl_test_begin("bst-1.2", "reverse inorder insertion, finding and deletion") {
    link_t		root, root1, L;
    ucl_value_t		K;
    int			j, k;
    ucl_bool_t		print=false;

    j=LITTLENUMBER;
    root = alloc_link(j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    /* insertion */
    for (j=LITTLENUMBER-1; j>0; --j) {
      mcl_debug("inserting %d", j);
      ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    }
    if (print) print_inorder(root);
    /* finding */
    for (j=LITTLENUMBER-1; j>0; --j) {
      mcl_debug("finding %d", j);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    }
    /* deleting */
    for (j=LITTLENUMBER-1; j>0; --j) {
      mcl_debug("deleting %d", j);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
      root1 = ucl_bst_delete(root, L);
      free_link(L);
      mcl_test_error_if_false(root == root1, "wrong root after deletion");
      if (print) print_inorder(root);
      for (k=j-1; k>0; --k) {
	K.t_int = k;
	L = ucl_bst_find(root, K, ucl_compare_int, getkey);
	mcl_test_error_if_false(NULL != L, "unable to find node with key %d", k);
      }
    }

    j=LITTLENUMBER;
    mcl_debug("deleting %d", j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, L);
    mcl_test_error_if_false(NULL == root1, "wrong root after deletion");
  }
  mcl_test_end();
}

static void
test_mixed_insert_find_remove (void)
{
  mcl_test_begin("bst-1.3", "mixed insertion, finding and deletion") {
    link_t		root, root1, L;
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
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=2;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=1;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=0;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=4;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=3;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=7;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=6;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=8;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    j=9;
    ucl_bst_insert(root, alloc_link(j), ucl_compare_int, getkey);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);

    if (print) print_inorder(root);
    /* finding */
    for (j=0; j<LITTLENUMBER; ++j) {
      mcl_debug("finding %d", j);
      K.t_int = j;
      L = ucl_bst_find(root, K, ucl_compare_int, getkey);
      mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
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
    mcl_debug("deleting %d", j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, L);
    free_link(L);
    mcl_test_error_if_false(root == root1, "wrong root after deletion");
    if (print) print_inorder(root);
    find(root, 0);
    find(root, 1);
    find(root, 2);
    find(root, 3);
    find(root, 4);
    find(root, 5);
    find(root, 6);
    find(root, 8);
    find(root, 9);

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
    mcl_debug("deleting %d", j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, L);
    free_link(L);
    mcl_test_error_if_false(root == root1, "wrong root after deletion");
    if (print) print_inorder(root);
    find(root, 0);
    find(root, 1);
    find(root, 3);
    find(root, 4);
    find(root, 5);
    find(root, 6);
    find(root, 8);
    find(root, 9);

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
    mcl_debug("deleting %d", j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, L);
    free_link(L);
    mcl_test_error_if_false(root != root1, "wrong root after deletion");
    root = root1;
    if (print) print_inorder(root);
    find(root, 0);
    L = find(root, 1);
    mcl_test_error_if_false(L == root, "expected root with key %d got %d", 1, L->key);
    find(root, 3);
    find(root, 4);
    find(root, 6);
    find(root, 8);
    find(root, 9);

    /* Removing 1:
     *
     *    1--4--6--8--9      0--4--6--8--9
     *    |  |           =>     |
     *    0  3                  3
     *
     */
    j=1;
    mcl_debug("deleting %d", j);
    K.t_int = j;
    L = ucl_bst_find(root, K, ucl_compare_int, getkey);
    mcl_test_error_if_false(NULL != L, "unable to find node with key %d", j);
    root1 = ucl_bst_delete(root, L);
    free_link(L);
    mcl_test_error_if_false(root != root1, "wrong root after deletion");
    root = root1;
    if (print) print_inorder(root);
    L = find(root, 0);
    mcl_test_error_if_false(L == root, "expected root with key %d got %d", 0, L->key);
    find(root, 3);
    find(root, 4);
    find(root, 6);
    find(root, 8);
    find(root, 9);

  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("map tests");

  mcl_test_subtitle("insertion, finding, removing");
  test_inorder_insert_find_remove ();
  test_reverse_inorder_insert_find_remove ();
  test_mixed_insert_find_remove ();


  exit(EXIT_SUCCESS);
}

/* end of file */




/* end of file */
