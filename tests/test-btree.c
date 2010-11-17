/*
  Part of: Useless Containers Library
  Contents: test for the binary tree container
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


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#define MCL_DEBUGGING		0
#include "mcl-test.h"
#include "ucl.h"

typedef void *	fun_t (void *);
typedef void	iterator_start_fun_t (ucl_iterator_t, void *);


/** ------------------------------------------------------------
 ** Helper functions: interface to btree.
 ** ----------------------------------------------------------*/

typedef struct node_tag_t {
  ucl_node_tag_t	node;
  int			idx;
} node_tag_t;
typedef node_tag_t *	node_t;

ucl_memory_allocator_t	allocator = {
  .data  = NULL,
  .alloc = ucl_memory_alloc
};

static node_t
node_make (int idx)
{
  node_t        p = NULL;

  allocator.alloc(allocator.data, &p, sizeof(node_tag_t));
  p->idx = idx;
  return p;
}
static void
node_final (node_t p)
{
  allocator.alloc(allocator.data, &p, 0);
}
static __inline__ void
node_clean (node_t p)
{
  ucl_struct_clean(p, node_tag_t);
}


/** ------------------------------------------------------------
 ** Hierarchy builder.
 ** ----------------------------------------------------------*/

static void
free_tree (node_t * n)
{
  for (int i=1; i<13; ++i)
    node_final(n[i]);
}
static void
build_tree (node_t * n)
{
  /*
    build hierarchy

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  for (int i=1; i<13; ++i)
    n[i] = node_make(i);

  ucl_btree_set_dadson(n[5],  n[1]);
  ucl_btree_set_dadbro(n[1],  n[3]);
  ucl_btree_set_dadson(n[3],  n[2]);
  ucl_btree_set_dadbro(n[3],  n[4]);
  ucl_btree_set_dadbro(n[5],  n[10]);
  ucl_btree_set_dadson(n[10], n[7]);
  ucl_btree_set_dadson(n[7],  n[6]);
  ucl_btree_set_dadbro(n[7],  n[9]);
  ucl_btree_set_dadson(n[9],  n[8]);
  ucl_btree_set_dadbro(n[10], n[12]);
  ucl_btree_set_dadson(n[12], n[11]);

#if 0
  for (int i=1; i<12; ++i) {
    debug("node %d: %p, dad=%p, son=%p, bro=%p, idx=%d", i, n[i],
	  ucl_btree_ref_dad(n[i]),
	  ucl_btree_ref_son(n[i]),
	  ucl_btree_ref_bro(n[i]),
	  *((int *)ucl_btree_data(n[i])));
  }
#endif
}


/** ------------------------------------------------------------
 ** Iteration results.
 ** ----------------------------------------------------------*/

static const int forward_inorder_iteration[13] = {
  -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};
static const int backward_inorder_iteration[13] = {
  -1, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
};

static const int forward_preorder_iteration[13] = {
  -1, 5, 1, 3, 2, 4, 10, 7, 6, 9, 8, 12, 11
};
static const int backward_preorder_iteration[13] = {
  -1, 5, 10, 12, 11, 7, 9, 8, 6, 1, 3, 4, 2
};

static const int forward_postorder_iteration[13] = {
  -1, 2, 4, 3, 1, 6, 8, 9, 7, 11, 12, 10, 5
};
static const int backward_postorder_iteration[13] = {
  -1, 11, 12, 8, 9, 6, 7, 10, 4, 2, 3, 1, 5
};

static const int forward_levelorder_iteration[13] = {
  -1, 5, 1, 10, 3, 7, 12, 2, 4, 6, 9, 11, 8
};
static const int backward_levelorder_iteration[13] = {
  -1, 5, 10, 1, 12, 7, 3, 11, 9, 6, 4, 2, 8
};

#define validate_iteration_result(RESULT,EXPECTED)	\
  _validate_iteration_result(__func__,__LINE__,RESULT,EXPECTED)
#define validate_iteration_result_range(RESULT,EXPECTED,BEGIN,END)		\
  _validate_iteration_result_range(__func__,__LINE__,RESULT,EXPECTED,BEGIN,END)

static void
_validate_iteration_result_range (const char * funcname, int linenum,
				  const int * result, const int * expected,
				  const int begin, const int end)
{
  for (int i=begin; i<=end; ++i) {
    if (0) {
      fprintf(stderr, "%s (%d): validating index %d expected %d, got %d\n",
	      funcname, linenum, i, expected[i], result[i]);
    }
    if (result[i] != expected[i]) {
      fprintf(stderr, "%s error (%d): *********************** at index %d expected %d, got %d\n",
	      funcname, linenum, i, expected[i], result[i]);
    }
  }
}
static void
_validate_iteration_result (const char * funcname, int linenum,
			    const int * result, const int * expected)
{
  _validate_iteration_result_range(funcname, linenum, result, expected, 1, 12);
}


/** ------------------------------------------------------------
 ** Constructor functions tests.
 ** ----------------------------------------------------------*/

static void
test_constructor_functions (void)
{
  mcl_test_begin("btree-1.1", "constructor functions") {
    node_t	a, b, c, d;
    a = node_make(0);
    b = node_make(1);
    c = node_make(2);
    d = node_make(3);
    /*
      Build hierarchy

		 ---  bro  ---
		| a |---->| c |
		 ---       ---
	          |         |
	      son v     son v
                 ---       ---
                | b |     | d |
                 ---       ---
    */
    ucl_btree_set_dadson(a, b);
    ucl_btree_set_dadbro(a, c);
    ucl_btree_set_dadson(c, d);

    assert((a == ucl_btree_ref_dad(b)) && (b == ucl_btree_ref_son(a)));
    assert((a == ucl_btree_ref_dad(c)) && (c == ucl_btree_ref_bro(a)));
    assert((c == ucl_btree_ref_dad(d)) && (d == ucl_btree_ref_son(c)));
    assert(ucl_btree_ref_dad(a) == NULL);
    assert(ucl_btree_ref_bro(c) == NULL);
    assert((ucl_btree_ref_son(b) == NULL) && (ucl_btree_ref_bro(b) == NULL));
    assert((ucl_btree_ref_son(d) == NULL) && (ucl_btree_ref_bro(d) == NULL));

    assert(0 == *((int *)ucl_btree_data(a)));
    assert(1 == *((int *)ucl_btree_data(b)));
    assert(2 == *((int *)ucl_btree_data(c)));
    assert(3 == *((int *)ucl_btree_data(d)));

    node_final(a);
    node_final(b);
    node_final(c);
    node_final(d);
  }
  mcl_test_end();
}

/** ------------------------------------------------------------
 ** Test hierarchy finder functions.
 ** ----------------------------------------------------------*/

static void
test_hierarchy_finder_functions (void)
{
  mcl_test_begin("btree-2.1", "finder functions") {
    node_t	n[13];
    build_tree(n);
    /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    assert( ucl_btree_find_leftmost(n[1]) == n[1] );
    assert( ucl_btree_find_leftmost(n[2]) == n[2] );
    assert( ucl_btree_find_leftmost(n[3]) == n[2] );
    assert( ucl_btree_find_leftmost(n[4]) == n[4] );
    assert( ucl_btree_find_leftmost(n[5]) == n[1] );
    assert( ucl_btree_find_leftmost(n[6]) == n[6] );
    assert( ucl_btree_find_leftmost(n[7]) == n[6] );
    assert( ucl_btree_find_leftmost(n[8]) == n[8] );
    assert( ucl_btree_find_leftmost(n[9]) == n[8] );
    assert( ucl_btree_find_leftmost(n[10])== n[6] );
    assert( ucl_btree_find_leftmost(n[11])== n[11]);
    assert( ucl_btree_find_leftmost(n[12])== n[11]);
    /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    assert( ucl_btree_find_rightmost(n[1]) == n[4] );
    assert( ucl_btree_find_rightmost(n[2]) == n[2] );
    assert( ucl_btree_find_rightmost(n[3]) == n[4] );
    assert( ucl_btree_find_rightmost(n[4]) == n[4] );
    assert( ucl_btree_find_rightmost(n[5]) == n[12]);
    assert( ucl_btree_find_rightmost(n[6]) == n[6] );
    assert( ucl_btree_find_rightmost(n[7]) == n[9] );
    assert( ucl_btree_find_rightmost(n[8]) == n[8] );
    assert( ucl_btree_find_rightmost(n[9]) == n[9] );
    assert( ucl_btree_find_rightmost(n[11])== n[11]);
    assert( ucl_btree_find_rightmost(n[10])== n[12]);
    assert( ucl_btree_find_rightmost(n[12])== n[12]);
    /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    assert( ucl_btree_find_deepest_son(n[1]) == n[2] );
    assert( ucl_btree_find_deepest_son(n[2]) == n[2] );
    assert( ucl_btree_find_deepest_son(n[3]) == n[2] );
    assert( ucl_btree_find_deepest_son(n[4]) == n[4] );
    assert( ucl_btree_find_deepest_son(n[5]) == n[2] );
    assert( ucl_btree_find_deepest_son(n[6]) == n[6] );
    assert( ucl_btree_find_deepest_son(n[7]) == n[6] );
    assert( ucl_btree_find_deepest_son(n[8]) == n[8] );
    assert( ucl_btree_find_deepest_son(n[9]) == n[8] );
    assert( ucl_btree_find_deepest_son(n[10])== n[6] );
    assert( ucl_btree_find_deepest_son(n[11])== n[11]);
    assert( ucl_btree_find_deepest_son(n[12])== n[11]);
    /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    assert( ucl_btree_find_deepest_bro(n[1]) == n[4] );
    assert( ucl_btree_find_deepest_bro(n[2]) == n[2] );
    assert( ucl_btree_find_deepest_bro(n[3]) == n[4] );
    assert( ucl_btree_find_deepest_bro(n[4]) == n[4] );
    assert( ucl_btree_find_deepest_bro(n[5]) == n[11]);
    assert( ucl_btree_find_deepest_bro(n[6]) == n[6]);
    assert( ucl_btree_find_deepest_bro(n[7]) == n[8] );
    assert( ucl_btree_find_deepest_bro(n[8]) == n[8]);
    assert( ucl_btree_find_deepest_bro(n[9]) == n[8] );
    assert( ucl_btree_find_deepest_bro(n[10])== n[11]);
    assert( ucl_btree_find_deepest_bro(n[11])== n[11]);
    assert( ucl_btree_find_deepest_bro(n[12])== n[11]);
    /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    assert( ucl_btree_find_root(n[1]) == n[5] );
    assert( ucl_btree_find_root(n[2]) == n[5] );
    assert( ucl_btree_find_root(n[3]) == n[5] );
    assert( ucl_btree_find_root(n[4]) == n[5] );
    assert( ucl_btree_find_root(n[5]) == n[5] );
    assert( ucl_btree_find_root(n[6]) == n[5] );
    assert( ucl_btree_find_root(n[7]) == n[5] );
    assert( ucl_btree_find_root(n[8]) == n[5] );
    assert( ucl_btree_find_root(n[9]) == n[5] );
    assert( ucl_btree_find_root(n[10])== n[5] );
    assert( ucl_btree_find_root(n[11])== n[5] );
    assert( ucl_btree_find_root(n[12])== n[5] );
    free_tree(n);
  }
  mcl_test_end();
}

/** ------------------------------------------------------------
 ** Test whole iterations.
 ** ----------------------------------------------------------*/

#define whole_iteration(BTREE,FIRST,STEP,EXPECTED)	\
  _whole_iteration(__func__,__LINE__,BTREE,FIRST,STEP,EXPECTED)

static void
_whole_iteration (const char * funcname, int linenum,
		  node_t * btree, fun_t * first, fun_t * step, const int * expected)
{
  node_t	cur;
  int		result[13] = { -1 };
  int		i;

  for (i=0, cur = first(btree[5]); cur; cur = step(cur))
    result[++i] = cur->idx;
  assert(NULL == cur);
  _validate_iteration_result(funcname,linenum,result,expected);
}

#define whole_iteration_with_iterator(BTREE,START,EXPECTED)	\
  _whole_iteration_with_iterator(__func__,__LINE__,BTREE,START,EXPECTED)

static void
_whole_iteration_with_iterator (const char * funcname, int linenum,
				node_t * btree, iterator_start_fun_t iterator_start, const int * expected)
{
  ucl_iterator_t	iterator;
  node_t		cur;
  int			result[13] = { -1 };
  int			i = 0;

  for (iterator_start(iterator, btree[5]); ucl_iterator_more(iterator); ucl_iterator_next(iterator)) {
    cur = ucl_iterator_ptr(iterator);
    result[++i] = cur->idx;
  }
  _validate_iteration_result(funcname,linenum,result,expected);
}
static void
test_whole_iterations (void)
{
  mcl_test_begin("btree-3.1", "whole iterations") {
    node_t		btree[13];
    build_tree(btree);
    {
      /* forward iterations */
      whole_iteration(btree,
		      ucl_btree_first_inorder,
		      ucl_btree_step_inorder,
		      forward_inorder_iteration);
      whole_iteration(btree,
		      ucl_btree_first_preorder,
		      ucl_btree_step_preorder,
		      forward_preorder_iteration);
      whole_iteration(btree,
		      ucl_btree_first_postorder,
		      ucl_btree_step_postorder,
		      forward_postorder_iteration);
      whole_iteration(btree,
		      ucl_btree_first_levelorder,
		      ucl_btree_step_levelorder,
		      forward_levelorder_iteration);

      /* backward iterations */
      whole_iteration(btree,
		      ucl_btree_first_inorder_backward,
		      ucl_btree_step_inorder_backward,
		      backward_inorder_iteration);
      whole_iteration(btree,
		      ucl_btree_first_preorder_backward,
		      ucl_btree_step_preorder_backward,
		      backward_preorder_iteration);
      whole_iteration(btree,
		      ucl_btree_first_postorder_backward,
		      ucl_btree_step_postorder_backward,
		      backward_postorder_iteration);
      whole_iteration(btree,
		      ucl_btree_first_levelorder_backward,
		      ucl_btree_step_levelorder_backward,
		      backward_levelorder_iteration);

      /* forward iterators */
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_inorder,
				    forward_inorder_iteration);
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_preorder,
				    forward_preorder_iteration);
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_postorder,
				    forward_postorder_iteration);
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_levelorder,
				    forward_levelorder_iteration);

      /* backward iterators */
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_inorder_backward,
				    backward_inorder_iteration);
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_preorder_backward,
				    backward_preorder_iteration);
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_postorder_backward,
				    backward_postorder_iteration);
      whole_iteration_with_iterator(btree,
				    ucl_btree_iterator_levelorder_backward,
				    backward_levelorder_iteration);
    }
    free_tree(btree);
  }
  mcl_test_end();
}

/** ------------------------------------------------------------
 ** Test subtree and range iterations.
 ** ----------------------------------------------------------*/

#define subtree_iteration_with_iterator(BTREE,ITERATOR_START,ROOT,EXPECTED) \
  _subtree_iteration_with_iterator(__func__,__LINE__,BTREE,ITERATOR_START,ROOT,EXPECTED)

static void
_subtree_iteration_with_iterator (const char * funcname, int linenum,
				  node_t * btree, iterator_start_fun_t iterator_start,
				  int root,
				  const int * expected)
{
  ucl_iterator_t iterator;
  int		result[14] = { -1, -1, -1,
			       -1, -1, -1,
			       -1, -1, -1,
			       -1, -1, -1,
			       -1, -1 };
  int		i=0;
  node_t	cur;

  for (iterator_start(iterator, btree[root]); ucl_iterator_more(iterator); ucl_iterator_next(iterator)) {
    cur = ucl_iterator_ptr(iterator);
    result[++i] = cur->idx;
  }
  _validate_iteration_result_range(funcname,linenum,result,expected,0,13);
}
static void
test_subtree_and_range_iterations (void)
{
  mcl_test_begin("btree-4.1", "subtree and range iterations") {
    node_t	btree[13];
    build_tree(btree);
    /* Inorder
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    { int	expected[14] = { -1, +1, +2, +3, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,1,expected); }
    { int	expected[14] = { -1, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,2,expected); }
    { int	expected[14] = { -1, +2, +3, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,3,expected); }
    { int	expected[14] = { -1, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,4,expected); }
    { int	expected[14] = { -1, +1, +2, +3, +4, +5, +6, +7, +8, +9, 10, 11, 12, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,5,expected); }
    { int	expected[14] = { -1, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,6,expected); }
    { int	expected[14] = { -1, +6, +7, +8, +9, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,7,expected); }
    { int	expected[14] = { -1, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,8,expected); }
    { int	expected[14] = { -1, +8, +9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,9,expected); }
    { int	expected[14] = { -1, +6, +7, +8, +9, 10, 11, 12, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,10,expected); }
    { int	expected[14] = { -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,11,expected); }
    { int	expected[14] = { -1, 11, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder,12,expected); }
    /* Preorder
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    { int	expected[14] = { -1, +1, +3, +2, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,1,expected); }
    { int	expected[14] = { -1, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,2,expected); }
    { int	expected[14] = { -1, +3, +2, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,3,expected); }
    { int	expected[14] = { -1, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,4,expected); }
    { int	expected[14] = { -1, +5, +1, +3, +2, +4, 10, +7, +6, +9, +8, 12, 11, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,5,expected); }
    { int	expected[14] = { -1, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,6,expected); }
    { int	expected[14] = { -1, +7, +6, +9, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,7,expected); }
    { int	expected[14] = { -1, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,8,expected); }
    { int	expected[14] = { -1, +9, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,9,expected); }
    { int	expected[14] = { -1, 10, +7, +6, +9, +8, 12, 11, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,10,expected); }
    { int	expected[14] = { -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,11,expected); }
    { int	expected[14] = { -1, 12, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder,12,expected); }
    /* Postorder
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    { int	expected[14] = { -1, +2, +4, +3, +1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,1,expected); }
    { int	expected[14] = { -1, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,2,expected); }
    { int	expected[14] = { -1, +2, +4, +3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,3,expected); }
    { int	expected[14] = { -1, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,4,expected); }
    { int	expected[14] = { -1, +2, +4, +3, +1, +6, +8, +9, +7, 11, 12, 10, +5, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,5,expected); }
    { int	expected[14] = { -1, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,6,expected); }
    { int	expected[14] = { -1, +6, +8, +9, +7, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,7,expected); }
    { int	expected[14] = { -1, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,8,expected); }
    { int	expected[14] = { -1, +8, +9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,9,expected); }
    { int	expected[14] = { -1, +6, +8, +9, +7, 11, 12, 10, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,10,expected); }
    { int	expected[14] = { -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,11,expected); }
    { int	expected[14] = { -1, 11, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder,12,expected); }
    /* Inorder backward

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    { int	expected[14] = { -1, +4, +3, +2, +1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,1,expected); }
    { int	expected[14] = { -1, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,2,expected); }
    { int	expected[14] = { -1, +4, +3, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,3,expected); }
    { int	expected[14] = { -1, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,4,expected); }
    { int	expected[14] = { -1, 12, 11, 10, +9, +8, +7, +6, +5, +4, +3, +2, +1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,5,expected); }
    { int	expected[14] = { -1, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,6,expected); }
    { int	expected[14] = { -1, +9, +8, +7, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,7,expected); }
    { int	expected[14] = { -1, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,8,expected); }
    { int	expected[14] = { -1, +9, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,9,expected); }
    { int	expected[14] = { -1, 12, 11, 10, +9, +8, +7, +6, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,10,expected); }
    { int	expected[14] = { -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,11,expected); }
    { int	expected[14] = { -1, 12, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_inorder_backward,12,expected); }
    /* Preorder backward

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    { int	expected[14] = { -1, +1, +3, +4, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,1,expected); }
    { int	expected[14] = { -1, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,2,expected); }
    { int	expected[14] = { -1, +3, +4, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,3,expected); }
    { int	expected[14] = { -1, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,4,expected); }
    { int	expected[14] = { -1, +5, 10, 12, 11, +7, +9, +8, +6, +1, +3, +4, +2, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,5,expected); }
    { int	expected[14] = { -1, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,6,expected); }
    { int	expected[14] = { -1, +7, +9, +8, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,7,expected); }
    { int	expected[14] = { -1, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,8,expected); }
    { int	expected[14] = { -1, +9, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,9,expected); }
    { int	expected[14] = { -1, 10, 12, 11, +7, +9, +8, +6, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,10,expected); }
    { int	expected[14] = { -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,11,expected); }
    { int	expected[14] = { -1, 12, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_preorder_backward,12,expected); }
    /* Postorder backward

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
    */
    { int	expected[14] = { -1, +4, +2, +3, +1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,1,expected); }
    { int	expected[14] = { -1, +2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,2,expected); }
    { int	expected[14] = { -1, +4, +2, +3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,3,expected); }
    { int	expected[14] = { -1, +4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,4,expected); }
    { int	expected[14] = { -1, 11, 12, +8, +9, +6, +7, 10, +4, +2, +3, +1, +5, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,5,expected); }
    { int	expected[14] = { -1, +6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,6,expected); }
    { int	expected[14] = { -1, +8, +9, +6, +7, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,7,expected); }
    { int	expected[14] = { -1, +8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,8,expected); }
    { int	expected[14] = { -1, +8, +9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,9,expected); }
    { int	expected[14] = { -1, 11, 12, +8, +9, +6, +7, 10, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,10,expected); }
    { int	expected[14] = { -1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,11,expected); }
    { int	expected[14] = { -1, 11, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      subtree_iteration_with_iterator(btree,ucl_btree_subtree_iterator_postorder_backward,12,expected); }
    free_tree(btree);
  }
  mcl_test_end();
}

/** ------------------------------------------------------------
 ** Main test function.
 ** ----------------------------------------------------------*/

int
main (void)
{
  mcl_test_title("btree tests");

  test_constructor_functions();
  test_hierarchy_finder_functions();
  test_whole_iterations();
  test_subtree_and_range_iterations();

  exit(EXIT_SUCCESS);
}

/* end of file */
