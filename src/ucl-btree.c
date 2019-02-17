/*
  Part of: Useless Containers Library
  Contents: binary tree functions
  Date: Fri Oct 11, 2002

  Abstract

	The btree container is built as a chain of structures; each link
	has a brother, a son and a parent.

              -----    bro     -----
             | no1 |--------->| no2 |
              ----- <--------- -----
                |  ^     dad
            son |  | dad
                v  |
               -----
              | no3 |
               -----

  Copyright (c) 2002-2005, 2008-2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#ifndef DEBUGGING_AND_LOGGING
#  define DEBUGGING_AND_LOGGING	0
#endif
#include "ucl-internals.h"

#define AVL_STATUS(L)		(((ucl_node_t)L)->meta.avl_status)
#define AVL_DEPTH(N)		ucl_btree_avl_depth(N)
#define AVL_FACTOR(N)		(AVL_DEPTH((N)->bro) - AVL_DEPTH((N)->son))

#define IS_EQUAL_DEPTH(N)       (UCL_EQUAL_DEPTH == AVL_STATUS(N))
#define IS_BRO_DEEPER(N)        (UCL_BRO_DEEPER  == AVL_STATUS(N))
#define IS_SON_DEEPER(N)        (UCL_SON_DEEPER  == AVL_STATUS(N))

#define IS_LEAF(N)		((! SON_OF(N)) && (! BRO_OF(N)))
#define IS_AVL_LEAF(N)		(IS_LEAF(N) && IS_EQUAL_DEPTH(N))


void *
ucl_btree_find_value (void * root, ucl_value_t value, ucl_comparison_t compar)
{
  ucl_node_t	N  = root;
  ucl_value_t	D;
  int		v;
  while (NULL != N) {
    D.pointer = N;
    v = compar.func(compar.data, value, D);
    if (v > 0)
      N = N->bro;
    else if (v < 0)
      N = N->son;
    else
      return N;
  }
  return NULL;
}


void *
ucl_btree_find_leftmost (void * node)
{
  ucl_node_t	N = node;
  while (N->son)
    N = N->son;
  return N;
}
void *
ucl_btree_find_rightmost (void * node)
{
  ucl_node_t	N = node;
  while (N->bro)
    N = N->bro;
  return N;
}
void *
ucl_btree_find_deepest_son (void * node)
{
  ucl_node_t	N = node;
  do {
    while (N->son) N = N->son;
    if    (N->bro) N = N->bro;
  } while (N->son || N->bro);
  return N;
}
void *
ucl_btree_find_deepest_bro (void * node)
{
  ucl_node_t	N = node;
  do {
    while (N->bro) N = N->bro;
    if    (N->son) N = N->son;
  } while (N->bro || N->son);
  return N;
}
void *
ucl_btree_find_root (void * node)
{
  ucl_node_t	N = node;
  while (N->dad)
    N = N->dad;
  return N;
}

static int
btree_depth (ucl_node_t N)
{
  int	son_depth = (N->son)? btree_depth(N->son) : 0;
  int	bro_depth = (N->bro)? btree_depth(N->bro) : 0;
  return 1 + ((son_depth > bro_depth)? son_depth : bro_depth);
}
int
ucl_btree_depth (void * N)
{
  return N? btree_depth(N) : 0;
}

void
ucl_btree_swap_out (void * A_, void * B_)
/* Given two  pointers to links, interpret A  as pointer to a  node in a
   tree and B as pointer to a  node out of any tree; store the links and
   meta value of A into B, then resets the links and meta value of A. */
{
  ucl_node_t		A = A_, B = B_;
  B->dad	= A->dad;
  B->son	= A->son;
  B->bro	= A->bro;
  B->meta	= A->meta;
  A->dad	= NULL;
  A->son	= NULL;
  A->bro	= NULL;
  A->meta	= ucl_value_null;
}

void
ucl_btree_swap (void * A_, void * B_)
/* Given two pointers to links, swap the links and the meta value in the
   node structures. */
{
  ucl_node_t		A = A_, B = B_;
  ucl_node_t		A_dad, A_son, A_bro;
  ucl_node_t		B_dad, B_son, B_bro;
  ucl_value_t		A_meta;

  A_dad		= A->dad;
  A_son		= A->son;
  A_bro		= A->bro;
  A_meta	= A->meta;

  B_dad		= B->dad;
  B_son		= B->son;
  B_bro		= B->bro;

  A->dad	= (B_dad == A)? B : B_dad;
  A->son	= (B_son == A)? B : B_son;
  A->bro	= (B_bro == A)? B : B_bro;
  A->meta = B->meta;
  if (A_dad && A_dad != B) {
    if (A_dad->son == A)
      A_dad->son = B;
    else
      A_dad->bro = B;
  }
  if (A_son && A_son != B) A_son->dad = B;
  if (A_bro && A_bro != B) A_bro->dad = B;

  B->dad	= (A_dad == B)? A : A_dad;
  B->son	= (A_son == B)? A : A_son;
  B->bro	= (A_bro == B)? A : A_bro;
  B->meta	= A_meta;
  if (B_dad && B_dad != A) {
    if (B_dad->son == B)
      B_dad->son = A;
    else
      B_dad->bro = A;
  }
  if (B_son && B_son != A) B_son->dad = A;
  if (B_bro && B_bro != A) B_bro->dad = A;
}

void
ucl_btree_swap_no_meta (void * A_, void * B_)
/* Given two pointers to links, swap the links but not the meta value in
   the node structures. */
{
  ucl_node_t		A = A_, B = B_;
  ucl_node_t		A_dad, A_son, A_bro;
  ucl_node_t		B_dad, B_son, B_bro;

  A_dad		= A->dad;
  A_son		= A->son;
  A_bro		= A->bro;

  B_dad		= B->dad;
  B_son		= B->son;
  B_bro		= B->bro;

  A->dad	= (B_dad == A)? B : B_dad;
  A->son	= (B_son == A)? B : B_son;
  A->bro	= (B_bro == A)? B : B_bro;
  if (A_dad && A_dad != B) {
    if (A_dad->son == A)
      A_dad->son = B;
    else
      A_dad->bro = B;
  }
  if (A_son && A_son != B) A_son->dad = B;
  if (A_bro && A_bro != B) A_bro->dad = B;

  B->dad	= (A_dad == B)? A : A_dad;
  B->son	= (A_son == B)? A : A_son;
  B->bro	= (A_bro == B)? A : A_bro;
  if (B_dad && B_dad != A) {
    if (B_dad->son == B)
      B_dad->son = A;
    else
      B_dad->bro = A;
  }
  if (B_son && B_son != A) B_son->dad = A;
  if (B_bro && B_bro != A) B_bro->dad = A;
}

void *
ucl_btree_avl_rot_left (void * old_cur_)
/* Perform a clockwise rotation which balances a son deeper subtree.  We
 * use  this  rotation  in  two  cases: "new_cur"  is  son  deeper  too,
 * "new_cur" is equal depth.
 *
 * Example with  "old_cur" being  "11" and having  son being  son deeper
 * (balance factors in parentheses):
 *
 *                (top)                   (top)
 *                  |                       |
 *                 11(-2)                   9(0)
 *                /  \                 -----+-----
 *            9(-1)   12              7          11(0)
 *              / \             =>   / \        /  \
 *             7   10               6   8     10    12
 *            / \
 *           6   8
 *
 * Example with  "old_cur" being "12"  and having son being  equal depth
 * (balance factors in parentheses):
 *
 *               (top)                      (top)
 *                 |                          |
 *                12(-2)                      9(+1)
 *               /  \                    -----+-----
 *             9(0)  13                 7          12(-1)
 *             / \              =>     / \         /  \
 *            7   10                  6   8      10    13
 *           / \    \                              \
 *          6   8    11                             11
 */
{
  debug("enter left rotation");
  ucl_node_t		old_cur = old_cur_;
  ucl_node_t		top	= old_cur->dad;
  ucl_node_t		new_cur = old_cur->son;
  ucl_node_t		bro	= new_cur->bro;
  ucl_avl_status_t	new_cur_old_status = AVL_STATUS(new_cur);
  assert(IS_SON_DEEPER(old_cur));
  assert(IS_SON_DEEPER(new_cur) || IS_EQUAL_DEPTH(new_cur));
  assert(-2 == ucl_btree_avl_factor(old_cur));
  assert((IS_SON_DEEPER(new_cur)? -1 : 0)  == ucl_btree_avl_factor(new_cur));
  /* relink "old_cur" and "new_cur" */
  new_cur->dad	= old_cur->dad;
  new_cur->bro	= old_cur;
  old_cur->dad	= new_cur;
  /* relink "old_cur" and "bro" */
  old_cur->son	= bro;
  if (bro)
    bro->dad	= old_cur;
  /* relink "top" */
  if (top) {
    if (old_cur == top->bro)
      top->bro = new_cur;
    else {
      assert(old_cur == top->son);
      top->son = new_cur;
    }
  }
  if (UCL_SON_DEEPER == new_cur_old_status) {
    /* The situation with balance factors in parentheses:
     *
     *      old_cur(-2)      new_cur(0)
     *       /           =>       \
     *   new_cur(-1)            old_cur(0)
     *        \                  /
     *         bro            bro
     */
    AVL_STATUS(old_cur) = UCL_EQUAL_DEPTH;
    AVL_STATUS(new_cur) = UCL_EQUAL_DEPTH;
    assert(ucl_btree_avl_is_correct(new_cur));
    assert(ucl_btree_avl_is_correct(old_cur));
  } else {
    assert(UCL_EQUAL_DEPTH == new_cur_old_status);
    /* The situation with balance factors in parentheses:
     *
     *      old_cur(-2)      new_cur(+1)
     *       /           =>       \
     *   new_cur(0)             old_cur(-1)
     *        \                  /
     *         bro            bro
     */
    AVL_STATUS(new_cur) = UCL_BRO_DEEPER;
    AVL_STATUS(old_cur) = UCL_SON_DEEPER;
    assert(ucl_btree_avl_is_correct(new_cur));
    assert(ucl_btree_avl_is_correct(old_cur));
    assert(+1 == ucl_btree_avl_factor(new_cur));
    assert(-1 == ucl_btree_avl_factor(old_cur));
  }
  return new_cur;
}

void *
ucl_btree_avl_rot_right (void * old_cur_)
/* Perform  a  counterclockwise rotation  which  balances  a bro  deeper
 * subtree.  We use this rotation  in two cases: "new_cur" is bro deeper
 * too, "new_cur" is equal depth.
 *
 * Example  with "old_cur"  being "7"  and having  bro being  bro deeper
 * (balance factors in parentheses):
 *
 *         (top)                              (top)
 *           |                                  |
 *           7(+2)                              9(0)
 *          / \                            -----+-----
 *         6   9(+1)         =>           7(0)        11
 *            / \                        / \         /  \
 *           8   11                     6   8      10    12
 *              /  \
 *            10    12
 *
 * Example with  "old_cur" being  "6" and having  bro being  equal depth
 * (balance factors in parentheses):
 *
 *         (top)                              (top)
 *           |                                  |
 *           6(+2)                              9(-1)
 *          / \                            -----+-----
 *         5   9(0)          =>           6(+1)       11
 *            / \                        / \         /  \
 *           8   11                     5=  8      10    12
 *          /   /  \                       /
 *         7  10    12                    7
 */
{
  debug("enter right rotation");
  ucl_node_t		old_cur = old_cur_;
  ucl_node_t		top	= old_cur->dad;
  ucl_node_t		new_cur	= old_cur->bro;
  ucl_node_t		son	= new_cur->son;
  ucl_avl_status_t	new_cur_old_status = AVL_STATUS(new_cur);
  assert(IS_BRO_DEEPER(old_cur));
  assert(IS_BRO_DEEPER(new_cur) || IS_EQUAL_DEPTH(new_cur));
  assert(+2 == ucl_btree_avl_factor(old_cur));
  assert((IS_BRO_DEEPER(new_cur)? +1 : 0) == ucl_btree_avl_factor(new_cur));
  /* relink "old_cur" and "new_cur" */
  new_cur->dad	= old_cur->dad;
  old_cur->dad	= new_cur;
  new_cur->son	= old_cur;
  /* relink "old_cur" and "son" */
  old_cur->bro	= son;
  if (son)
    son->dad	= old_cur;
  /* relink "top" */
  if (top) {
    if (old_cur == top->bro)
      top->bro = new_cur;
    else {
      assert(old_cur == top->son);
      top->son = new_cur;
    }
  }
  if (UCL_BRO_DEEPER == new_cur_old_status) {
    /* The situation with balance factors in parentheses:
     *
     *   old_cur(+2)           new_cur(0)
     *        \          =>     /
     *      new_cur(+1)      old_cur(0)
     *       /                   \
     *    bro                     bro
     */
    AVL_STATUS(old_cur) = UCL_EQUAL_DEPTH;
    AVL_STATUS(new_cur) = UCL_EQUAL_DEPTH;
    debug("leave ==");
    assert(ucl_btree_avl_is_correct(new_cur));
    assert(ucl_btree_avl_is_correct(old_cur));
  } else {
    /* The situation with balance factors in parentheses:
     *
     *   old_cur(+2)            new_cur(-1)
     *        \         =>      /
     *      new_cur(0)      old_cur(+1)
     *      /                     \
     *   bro                       bro
     */
    AVL_STATUS(new_cur) = UCL_SON_DEEPER;
    AVL_STATUS(old_cur) = UCL_BRO_DEEPER;
    debug("leave sb");
    assert(ucl_btree_avl_is_correct(new_cur));
    assert(ucl_btree_avl_is_correct(old_cur));
    assert(-1 == ucl_btree_avl_factor(new_cur));
    assert(+1 == ucl_btree_avl_factor(old_cur));
  }
  return new_cur;
}

void *
ucl_btree_avl_rot_left_right (void * old_cur_)
/* Perform   a   double    rotation   which   balances   a   "son-higher
 * son-bro-higher"  tree.   The  core  of  it  is  (balance  factors  in
 * parentheses):
 *
 *            old_cur(-2)              new_cur(0)
 *             /                         / \
 *          son(+1)           =>      son   old_cur
 *             \
 *            new_cur
 *
 * notice  that  the  status  changes  only  for  the  nodes  "old_cur",
 * "new_cur"  and "son";  the new  status of  "new_cur" is  always equal
 * depth, while the status of the others must be computed.
 *
 * It can  be seen as the  sequence of a  right rotation on "son"  and a
 * left rotation on "old_cur", though the computation of statuses at the
 * end of  this double rotation is  different from the  one performed at
 * the end of the simple rotation functions:
 *
 *            old_cur(-2)     old_cur        new_cur(0)
 *             /               /             /    \
 *          son(+1)    =>  new_cur     =>  son    old_cur
 *             \             /
 *            new_cur     son
 *
 * Example (balance factors in parentheses):
 *
 *             (top)                             (top)
 *               |                                 |
 *              10(-2) (old_cur)                   8(0) (new_cur)
 *             /  \                      ----------+----------
 *    (son) 5(+1)  13             (son) 5(0)                 10(0) (old_cur)
 *           / \               =>      / \                  /  \
 *          3   8 (new_cur)           3   7                9    13
 *             / \                      (deep_son)  (deep_bro)
 * (deep_son) 7   9 (deep_bro)
 */
{
  debug("enter left/right rotation");
  ucl_node_t	old_cur = old_cur_;
  ucl_node_t	top	= old_cur->dad;
  ucl_node_t	son	= old_cur->son;
  ucl_node_t	new_cur	= son->bro;
  ucl_node_t	deep_bro, deep_son;
  assert(IS_SON_DEEPER(old_cur));
  assert(IS_BRO_DEEPER(son));
  assert(-2 == ucl_btree_avl_factor(old_cur));
  assert(+1 == ucl_btree_avl_factor(son));
  /* relink "son" and "deep_son" */
  deep_son	= new_cur->son;
  son->bro	= deep_son;
  if (deep_son)
    deep_son->dad = son;
  /* relink "new_cur" and "son" */
  new_cur->son	= son;
  son->dad	= new_cur;
  /* relink "old_cur" and "deep_bro" */
  deep_bro	= new_cur->bro;
  old_cur->son	= deep_bro;
  if (deep_bro)
    deep_bro->dad = old_cur;
  /* finish relinking "old_cur" and "new_cur" */
  new_cur->dad	= old_cur->dad;
  new_cur->bro	= old_cur;
  old_cur->dad	= new_cur;
  /* relink "top" */
  if (top) {
    if (old_cur == top->bro)
      top->bro = new_cur;
    else {
      assert(old_cur == top->son);
      top->son = new_cur;
    }
  }
  /* Adjust the statuses. */
  AVL_STATUS(new_cur)	= UCL_EQUAL_DEPTH;
  AVL_STATUS(old_cur)	= old_cur->son?
    (old_cur->bro? AVL_FACTOR(old_cur) : UCL_SON_DEEPER  ):
    (old_cur->bro? UCL_BRO_DEEPER      : UCL_EQUAL_DEPTH );
  AVL_STATUS(son)	= son->bro?
    (son->son?     AVL_FACTOR(son)     : UCL_BRO_DEEPER  ):
    (son->son?     UCL_SON_DEEPER      : UCL_EQUAL_DEPTH );
  return new_cur;
}

void *
ucl_btree_avl_rot_right_left (void * old_cur_)
/* Perform   a   double    rotation   which   balances   a   "bro-higher
 * bro-son-higher"  tree.   The  core  of  it  is  (balance  factors  in
 * parentheses):
 *
 *         old_cur(+2)                 new_cur(0)
 *                \                      /  \
 *                 bro(-1)  =>    old_cur    bro
 *                /
 *         new_cur
 *
 * notice  that  the  status  changes  only  for  the  nodes  "old_cur",
 * "new_cur"  and "son";  the new  status of  "new_cur" is  always equal
 * depth, while the status of the others must be computed.
 *
 * It can  be seen as  the sequence  of a left  rotation on "bro"  and a
 * right rotation  on "old_cur", though  the computation of  statuses at
 * the end of  this double rotation is different  from the one performed
 * at the end of the simple rotation functions:
 *
 *         old_cur(+2)         old_cur                new_cur(0)
 *                \                \                    /  \
 *                 bro(-1)  =>    new_cur  =>    old_cur    bro
 *                /                   \
 *         new_cur                     bro
 *
 * Example (balance factors in parentheses):
 *
 *             (top)                                (top)
 *               |                                    |
 *     (old_cur) 9(+2)                               11(0) (new_cur)
 *              / \                         ----------+---------
 *             8   13(-1) (bro)  (old_cur) 9(0)                 13(0) (bro)
 *                /  \         =>         / \                  /  \
 *   (new_cur) 11(0)  14                 8   10              12    14
 *             /  \                        (deep_son)  (deep_bro)
 *           10    12
 *   (deep_son)    (deep_bro)
 */
{
  debug("enter right/left rotation");
  ucl_node_t	old_cur = old_cur_;
  ucl_node_t	top	= old_cur->dad;
  ucl_node_t	bro	= old_cur->bro;
  ucl_node_t	new_cur	= bro->son;
  ucl_node_t	deep_bro, deep_son;
  assert(UCL_BRO_DEEPER == old_cur->meta.avl_status);
  assert(UCL_SON_DEEPER == bro->meta.avl_status);
  assert(+2 == ucl_btree_avl_factor(old_cur));
  assert(-1 == ucl_btree_avl_factor(bro));
  /* relink "bro" and "deep_bro" */
  deep_bro	= new_cur->bro;
  bro->son	= deep_bro;
  if (deep_bro)
    deep_bro->dad = bro;
  /* relink "new_cur" and "bro" */
  new_cur->bro	= bro;
  bro->dad	= new_cur;
  /* relink "old_cur" and "deep_son" */
  deep_son	= new_cur->son;
  old_cur->bro	= deep_son;
  if (deep_son)
    deep_son->dad = old_cur;
  /* finish relinking "old_cur" and "new_cur" */
  new_cur->dad	= old_cur->dad;
  new_cur->son	= old_cur;
  old_cur->dad	= new_cur;
  /* relink "top" */
  if (top) {
    if (old_cur == top->bro)
      top->bro = new_cur;
    else {
      assert(old_cur == top->son);
      top->son = new_cur;
    }
  }
  /* Adjust the statuses. */
  AVL_STATUS(new_cur)	= UCL_EQUAL_DEPTH;
  AVL_STATUS(old_cur)	= old_cur->bro?
    (old_cur->son? AVL_FACTOR(old_cur) : UCL_BRO_DEEPER  ):
    (old_cur->son? UCL_SON_DEEPER      : UCL_EQUAL_DEPTH );
  AVL_STATUS(bro)	= bro->son?
    (bro->bro?     AVL_FACTOR(bro)     : UCL_SON_DEEPER  ):
    (bro->bro?     UCL_BRO_DEEPER      : UCL_EQUAL_DEPTH );
  return new_cur;
}

int
ucl_btree_avl_depth (void * N_)
{
  ucl_node_t	N = N_;
  if (!N) return 0;
  int		depth = 1;
  for (;;) {
    switch (AVL_STATUS(N)) {
    case UCL_SON_DEEPER:
      assert(N->son);
      ++depth;
      N = N->son;
      break; /* from the switch() */
    case UCL_BRO_DEEPER:
      assert(N->bro);
      ++depth;
      N = N->bro;
      break; /* from the switch() */
    case UCL_EQUAL_DEPTH:
      if (N->son) {
	++depth;
	N = N->son;
      } else if (N->bro) {
	++depth;
	N = N->bro;
      } else {
	return depth;
      }
      break; /* from the switch() */
    }
  }
}
int
ucl_btree_avl_factor (void * N_)
{
  ucl_node_t	N = N_;
  if (!N) return 0;
  return ucl_btree_avl_depth(N->bro) - ucl_btree_avl_depth(N->son);
}

ucl_bool_t
ucl_btree_avl_is_balanced (void * N_)
{
  ucl_node_t	N = N_;
  if (!N) return true;
#if 1
  int	son_depth = ucl_btree_depth(N->son);
  int	bro_depth = ucl_btree_depth(N->bro);
#else
  int	son_depth = (N->son)? (int)ucl_btree_avl_depth(N->son) : 0;
  int	bro_depth = (N->bro)? (int)ucl_btree_avl_depth(N->bro) : 0;
#endif
  int	factor    = bro_depth - son_depth;
#if DEBUGGING_AND_LOGGING
  debug("factor=%d", factor);
#endif
  if ((-1 <= factor) && (factor <= +1))
    return
      ((N->son)? ucl_btree_avl_is_balanced(N->son) : true) &&
      ((N->bro)? ucl_btree_avl_is_balanced(N->bro) : true);
  else
    return false;
}
ucl_bool_t
ucl_btree_avl_is_correct (void * N_)
{
  ucl_node_t	N = N_;
  if (!N) return true;
#if 1
  int	son_depth = ucl_btree_depth(N->son);
  int	bro_depth = ucl_btree_depth(N->bro);
#else
  int	son_depth = (N->son)? (int)ucl_btree_avl_depth(N->son) : 0;
  int	bro_depth = (N->bro)? (int)ucl_btree_avl_depth(N->bro) : 0;
#endif
  int	factor    = bro_depth - son_depth;
#if DEBUGGING_AND_LOGGING
  debug("factor=%d, son depth=%d, bro depth=%d", factor, son_depth, bro_depth);
#endif
  switch (factor) {
  case -1:
    if (UCL_SON_DEEPER != AVL_STATUS(N))
      return false;
    else
      break;
  case 0:
    if (UCL_EQUAL_DEPTH != AVL_STATUS(N))
      return false;
    else
      break;
  case +1:
    if (UCL_BRO_DEEPER != AVL_STATUS(N))
      return false;
    else
      break;
  default:
    return false;
  }
  return
    ((N->son)? ucl_btree_avl_is_balanced(N->son) : true) &&
    ((N->bro)? ucl_btree_avl_is_balanced(N->bro) : true);
}

void *
ucl_btree_step_inorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->bro) {
    N = N->bro;
    return ucl_btree_find_leftmost(N);
  } else {
    dad = N->dad;
    while (dad && (dad->bro == N)) {
      N		= dad;
      dad	= N->dad;
    }
    return dad;
  }
}
void *
ucl_btree_step_inorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->son) {
    N = N->son;
    return ucl_btree_find_rightmost(N);
  } else {
    dad = N->dad;
    while (dad && (dad->son == N)) {
      N		= dad;
      dad	= N->dad;
    }
    return dad;
  }
}


void *
ucl_btree_step_preorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->son)
    return N->son;
  else if (N->bro)
    return N->bro;
  else {
    for (;;) {
      dad = N->dad;
      if (! dad)
	break;
      else if ((dad->son == N) && (dad->bro))
	return dad->bro;
      else
	N = dad;
    }
  }
  return dad;
}
void *
ucl_btree_step_preorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  if (N->bro)
    return N->bro;
  else if (N->son)
    return N->son;
  else {
    for (;;) {
      dad = N->dad;
      if (! dad)
	break;
      else if ((dad->bro == N) && (dad->son))
	return dad->son;
      else
	N = dad;
    }
  }
  return dad;
}


void *
ucl_btree_step_postorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  dad = N->dad;
  if (! dad)
    return dad;
  else if ((dad->bro) && (dad->bro != N))
    return ucl_btree_find_deepest_son(dad->bro);
  else
    return dad;
}
void *
ucl_btree_step_postorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	dad;
  dad = N->dad;
  if (! dad)
    return dad;
  else if ((dad->son) && (dad->son != N))
    return ucl_btree_find_deepest_bro(dad->son);
  else
    return dad;
}


void *
ucl_btree_step_levelorder (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	org_p;
  ucl_node_t	last_p;
  int		i=0;
  if ((! N->dad) && (! N->son) && (! N->bro))
    return NULL;
  org_p = N;
  for (;;) {
    if (N->dad) {
      last_p = N;
      N = N->dad;
      ++i;
      if (N->bro && N->bro != last_p) {
	last_p = N;
	N = N->bro;
	--i;
	if (i == 0)
	  return N;
	if (N == org_p && !N->son && !N->bro)
	  return NULL;
	while (N->son || N->bro) {
	  if (N->son) {
	    last_p = N;
	    N = N->son;
	  } else if (N->bro) {
	    last_p = N;
	    N = N->bro;
	  }
	  --i;
	  if (i == 0)
	    return N;
	  if (N == org_p && !N->son && !N->bro)
	    return NULL;
	}
      }
    } else {
      ++i;
      while (N->son || N->bro) {
	if (N->son) {
	  last_p = N;
	  N = N->son;
	} else if (N->bro) {
	  last_p = N;
	  N = N->bro;
	}
	--i;
	if (i == 0)
	  return N;
	if ((N == org_p) && (!N->son) && (!N->bro))
	  return NULL;
      }
    }
  }
  return N;
}


void *
ucl_btree_step_levelorder_backward (void * node)
{
  ucl_node_t	N = node;
  ucl_node_t	org_p;
  ucl_node_t	last_p;
  int		i=0;
  if ((! N->dad) && (! N->bro) && (! N->son))
    return NULL;
  org_p = N;
  for (;;) {
    if (N->dad) {
      last_p = N;
      N = N->dad;
      ++i;
      if (N->son && N->son != last_p) {
	last_p = N;
	N = N->son;
	--i;
	if (i == 0)
	  return N;
	if (N == org_p && !N->bro && !N->son)
	  return NULL;
	while (N->bro || N->son) {
	  if (N->bro) {
	    last_p = N;
	    N = N->bro;
	  } else if (N->son) {
	    last_p = N;
	    N = N->son;
	  }
	  --i;
	  if (i == 0)
	    return N;
	  if (N == org_p && !N->bro && !N->son)
	    return NULL;
	}
      }
    } else {
      ++i;
      while (N->bro || N->son) {
	if (N->bro) {
	  last_p = N;
	  N = N->bro;
	} else if (N->son) {
	  last_p = N;
	  N = N->son;
	}
	--i;
	if (i == 0)
	  return N;
	if ((N == org_p) && (!N->bro) && (!N->son))
	  return NULL;
      }
    }
  }
  return N;
}


static void __attribute__((__nonnull__))
ucl_btree_generic_iteration_next (ucl_iterator_t iterator)
{
  iterator->iterator = iterator->internal2.pointer_map_function(iterator->iterator);
}

/* ------------------------------------------------------------ */

void
ucl_btree_iterator_inorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_inorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder;
}
void
ucl_btree_iterator_inorder_backward (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_inorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_iterator_preorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_preorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder;
}
void
ucl_btree_iterator_preorder_backward (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_preorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder_backward;
}

/* ------------------------------------------------------------ */

__attribute__((__nonnull__)) void
ucl_btree_iterator_postorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_postorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder;
}
__attribute__((__nonnull__)) void
ucl_btree_iterator_postorder_backward (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_postorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_iterator_levelorder (ucl_iterator_t iterator, void * root_node)
{
  iterator->iterator	= ucl_btree_first_levelorder(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_levelorder;
}
__attribute__((__nonnull__)) void
ucl_btree_iterator_levelorder_backward (ucl_iterator_t iterator,
					void * root_node)
{
  iterator->iterator	= ucl_btree_first_levelorder_backward(root_node);
  iterator->next	= ucl_btree_generic_iteration_next;
  iterator->internal2.pointer_map_function = ucl_btree_step_levelorder_backward;
}


static void __attribute__((__nonnull__,__hot__))
ucl_btree_subtree_generic_next (ucl_iterator_t iterator)
{
  iterator->iterator = (iterator->iterator == iterator->internal1.pointer)?
    NULL : iterator->internal2.pointer_map_function(iterator->iterator);
}

/* ------------------------------------------------------------ */

void
ucl_btree_subtree_iterator_inorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_inorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_rightmost(node);	/* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder;
}
void
ucl_btree_subtree_iterator_inorder_backward (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_inorder_backward(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_leftmost(node);	/* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_inorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_subtree_iterator_preorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_preorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_deepest_bro(node); /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder;
}
void
ucl_btree_subtree_iterator_preorder_backward (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_preorder_backward(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_deepest_son(node); /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_preorder_backward;
}

/* ------------------------------------------------------------ */

void
ucl_btree_subtree_iterator_postorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_postorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = node; /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder;
}
void
ucl_btree_subtree_iterator_postorder_backward (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_postorder_backward(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = node; /* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_postorder_backward;
}

/* ------------------------------------------------------------ */

#if 0
void
ucl_btree_subtree_iterator_levelorder (ucl_iterator_t iterator, void * node)
{
  iterator->iterator	= ucl_btree_first_levelorder(node);
  iterator->next	= ucl_btree_subtree_generic_next;
  iterator->internal1.pointer = ucl_btree_find_rightmost(node);	/* the final node */
  iterator->internal2.pointer_map_function = ucl_btree_step_levelorder;
}
void
ucl_btree_subtree_iterator_levelorder_backward (ucl_iterator_t iterator, void * node)
{
}
#endif

/* end of file */
