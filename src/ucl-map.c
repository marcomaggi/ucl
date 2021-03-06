/*
  Part of: Useless Containers Library
  Contents: code for the map/multimap container.

  Abstract:

	The map  container is  built as an  AVL binary search  tree; the
	brother of each tree link holds an element with greater or equal
	key, while  the son holds an  element with lesser  key.  For the
	multimap: an  element with equal  key is considered as  one with
	greater key.

                 -----    bro     -----
	        | no1 |--------->| no2 |
	         ----- <--------- -----
	         |  ^     dad
             son |  | dad
                 v  |                 key(no3) < key(no1) <= key(no2)
                 -----
                | no3 |
                 -----

	  The knowledge on  how to implement AVL trees  was derived from
	the  book "Algoritmi +  Strutture Dati  = Programmi"  di Niklaus
	Wirth, 1987 Tecniche Nuove.

  Copyright (c) 2001-2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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
 ** -----------------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#include "ucl-internals.h"

#define IS_MULTIMAP(M)          (UCL_ALLOW_MULTIPLE_OBJECTS & M->flags)

#define AVL_STATUS(L)		(((ucl_node_t)L)->meta.avl_status)
#define STATUS_STRING(N)	((UCL_EQUAL_DEPTH==(N))? "equal-depth": \
				 ((UCL_SON_DEEPER==(N))? "son-deeper":	\
				  ((UCL_BRO_DEEPER==(N))? "bro-deeper":	\
				   "corrupted")))

#define IS_EQUAL_DEPTH(N)       (UCL_EQUAL_DEPTH == AVL_STATUS(N))
#define IS_BRO_DEEPER(N)        (UCL_BRO_DEEPER  == AVL_STATUS(N))
#define IS_SON_DEEPER(N)        (UCL_SON_DEEPER  == AVL_STATUS(N))

#define IS_LEAF(N)		((! SON_OF(N)) && (! BRO_OF(N)) && IS_EQUAL_DEPTH(N))

/* to be used as right-side in assignments */
#define DAD_OF(L)		((void *)((L)->dad))
#define SON_OF(L)		((void *)((L)->son))
#define BRO_OF(L)		((void *)((L)->bro))

/* to be used as left-side in assignments */
#define DAD(L)		((L)->dad)
#define SON(L)		((L)->son)
#define BRO(L)		((L)->bro)

static void	map_inorder_iterator_next	(ucl_iterator_t iterator);
static void	map_inorder_backward_iterator_next (ucl_iterator_t iterator);
static void	map_preorder_iterator_next	(ucl_iterator_t iterator);
static void	map_postorder_iterator_next	(ucl_iterator_t iterator);
static void	map_levelorder_iterator_next	(ucl_iterator_t iterator);
static void	map_upperbound_iterator_next	(ucl_iterator_t iterator);
static void	map_lowerbound_iterator_next	(ucl_iterator_t iterator);

static void	intersection_iterator_next	(ucl_iterator_t iterator);
static void	intersection_find_common	(ucl_iterator_t iter1,
						 ucl_iterator_t iter2,
						 ucl_iterator_t iterator);

static void	union_iterator_next		(ucl_iterator_t iterator);
static void	union_find_next			(ucl_iterator_t iter1,
						 ucl_iterator_t iter2,
						 ucl_iterator_t iterator);

static void	complintersect_iterator_next (ucl_iterator_t iterator);
static void	subtraction_iterator_next (ucl_iterator_t iterator);


static __inline__ __attribute__((__always_inline__,__nonnull__))
void
size_incr (ucl_map_t M)
{
  ++(M->size);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
size_decr (ucl_map_t M)
{
  --(M->size);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_value_t
node_key (const ucl_map_t M, void * N)
{
  return M->getkey.func(M->getkey.data, N);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_key_node (const ucl_map_t M, ucl_value_t K, ucl_node_t N)
{
  return M->keycmp.func(M->keycmp.data, K, node_key(M, N));
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_key_key (const ucl_map_t M, ucl_value_t K1, ucl_value_t K2)
{
  return M->keycmp.func(M->keycmp.data, K1, K2);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_node_node (const ucl_map_t M, ucl_node_t N1, ucl_node_t N2)
{
  return M->keycmp.func(M->keycmp.data, node_key(M, N1), node_key(M, N2));
}

void
ucl_map_initialise (ucl_map_t M, unsigned int flags, ucl_comparison_t keycmp, ucl_node_getkey_t getkey)
{
  assert(M);
  assert(keycmp.func);
  M->root		= NULL;
  M->size		= 0;
  M->keycmp		= keycmp;
  M->getkey		= getkey;
  M->flags		= flags;
}

ucl_bool_t
ucl_map_insert (ucl_map_t M, void * new_)
{
  ucl_node_t	new = new_, dad=NULL, child=NULL;
  int		comparison_result=0;
  ucl_bool_t	dad_is_root;
  assert(M);
  assert(new);
  /* The  new link  always  has AVL  status  set to  equal depth  before
     rebalancing. */
  AVL_STATUS(new) = UCL_EQUAL_DEPTH;
  DAD(new) = BRO(new) = SON(new) = NULL;
  /* Handle the case of empty map. */
  if (! M->root) {
    assert(0 == ucl_map_size(M));
    M->root = new;
    size_incr(M);
    return true;
  }
  /* If the map  is not empty, we have descend the  tree looking for the
     node that will be the dad of  "L".  If "M" is not a multimap and we
     find a node with key equal to the key of "L": we return false. */
  {
    ucl_bool_t	 allow_multiple_objects	= IS_MULTIMAP(M);
    ucl_value_t	 K			= node_key(M, new);
    for (ucl_node_t cursor = M->root; cursor;) {
      dad = cursor;
      comparison_result = comparison_key_node(M, K, cursor);
      if ((comparison_result > 0) || (0==comparison_result && allow_multiple_objects))
	cursor = BRO(cursor);
      else if (comparison_result < 0)
	cursor = SON(cursor);
      else {
	assert(0==comparison_result && !allow_multiple_objects);
	return false;
      }
    }
  }
  /* Here "dad" references  the node that will become  the dad of "new";
     if "comparison_result<0": we have to  insert "new" as son of "dad",
     else as brother (this happens  also when "new" and "dad" have equal
     key). */
  if (comparison_result<0) {
    /*If we append "new" as son  of "dad", "new" itself becomes its left
     *subtree:
     *
     *   dad -- ?
     *    |
     *   new
     *
     *there are two cases:
     *
     *1)  "NULL  ==  dad->bro":  "dad" was  UCL_EQUAL_DEPTH  before  and
     *becomes UCL_SON_DEEPER  now; we have  to update the AVL  status of
     *the upper  nodes, because  the tree having  "dad" as  root changed
     *depth.
     *
     *2) "NULL != dad->bro": "dad" was UCL_BRO_DEEPER before and becomes
     *UCL_EQUAL_DEPTH now; there is no  need to update the AVL status of
     *the upper  nodes, because  the tree having  "dad" as root  HAS NOT
     *changed depth.
     */
    ucl_btree_set_dadson(dad, new);
    size_incr(M);
    if (BRO(dad)) {
      assert(IS_BRO_DEEPER(dad));
      AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
      return true;
    } else {
      assert(IS_EQUAL_DEPTH(dad));
      AVL_STATUS(dad) = UCL_SON_DEEPER;
    }
  } else {
    /*If we append "new" as bro of "dad", "new" itself becomes the right
     *subtree of "dad":
     *
     *   dad -- new
     *    |
     *    ?
     *
     *there are two cases:
     *
     *3)  "NULL  ==  dad->son":  "dad" was  UCL_EQUAL_DEPTH  before  and
     *becomes UCL_BRO_DEEPER  now; we have  to update the AVL  status of
     *the upper nodes, because the tree havind "dad" as root HAS changed
     *depth.
     *
     *4) "NULL != dad->son": "dad" was UCL_SON_DEEPER before and becomes
     *UCL_EQUAL_DEPTH now; there is no  need to update the AVL status of
     *the upper  nodes, because  the tree having  "dad" as root  HAS NOT
     *changed depth.
     */
    assert(comparison_result>=0);
    ucl_btree_set_dadbro(dad, new);
    size_incr(M);
    if (SON(dad)) {
      assert(IS_SON_DEEPER(dad));
      AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
      return true;
    } else {
      assert(IS_EQUAL_DEPTH(dad));
      AVL_STATUS(dad) = UCL_BRO_DEEPER;
    }
  }
  /* If we are  still here, we have  to step up the tree  and update the
     AVL status of the ancestors of "dad", doing rotations when required
     to keep the tree balanced.

     We enter  a loop in  which "child" is  the node we come  from while
     stepping up  and "dad"  is the node  that needs status  update.  We
     have to  loop while the  tree having "dad"  as root has  got deeper
     with the insertion: if suchtree  has not changed its depth, nothing
     changes  from the  point of  view of  its ancestors,  so  no status
     updates are required. */
  /* *** NOTE ***

     Adjusting  the  balancing  after  an insertion  is  DIFFERENT  from
     adjusting after a  removal.  We CANNOT take the  following loop and
     factor  it   out  joining   it  with  the   loop  at  the   end  of
     "ucl_map_delete()". */
  for (;;) {
    child = dad;
    dad   = DAD_OF(child);
    if (! dad)
      return true; /* "child" is the root */
    else if (SON_OF(dad) == child) { /* stepping up from the son subtree */
      switch (AVL_STATUS(dad)) {
      case UCL_BRO_DEEPER:
	/* Stepping up  from a son subtree  which got deeper,  we find a
	 * "dad" which  was bro deeper:  it becomes equal depth  now and
	 * its depth HAS NOT changed.  Example:
	 *
	 *    before insertion          insertion              adjustment
	 *
	 *   (dad) 20b--25=--28=    (dad) 20b--25=--28=    (dad) 20=--25=--28=
	 *          |    |                 |    |                 |    |
	 *   (chi) 15=  22=      => (chi) 15s  22=      => (chi) 15s  22=
	 *                                 |                      |
	 *   (new) 10               (new) 10=              (new) 10=
	 */
	AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
	assert(ucl_btree_avl_is_correct(dad));
	return true;
      case UCL_EQUAL_DEPTH:
	/* Stepping up  from a son subtree  which got deeper,  we find a
	 * "dad" which  was equal depth:  it becomes son deeper  now and
	 * its depth HAS changed.  Example:
	 *
	 *    before insertion      insertion         adjustment
	 *
	 *   (dad) 20=--25=       (dad) 20=--25=    (dad) 20s--25=
	 *          |                    |                 |
	 *   (chi) 15=         => (chi) 15s      => (chi) 15s
	 *                               |                 |
	 *   (new) 10             (new) 10=         (new) 10=
	 */
	AVL_STATUS(dad) = UCL_SON_DEEPER;
	assert(ucl_btree_avl_is_correct(dad));
	break;
      case UCL_SON_DEEPER:
	/* Stepping up  from a son subtree  which got deeper,  we find a
	 * "dad"  which was  already son  deeper; we  have to  perform a
	 * rotation after which:  the new "dad" will be  equal depth and
	 * its depth will be equal to the one of the old "dad".
	 *
	 * Example requiring left rotation:
	 *
	 *    before insertion      insertion           adjustment
	 *
	 *   (dad) 20s            (dad) 20s      (chi) 15=--20= (dad)
	 *          |                    |              |
	 *   (chi) 15=         => (chi) 15s   => (new) 10=
	 *                               |
	 *   (new) 10             (new) 10=
	 *
	 * Example requiring left/right rotation:
	 *
	 *    before insertion      insertion           adjustment
	 *
	 *   (dad) 20s          (dad) 20s          (new) 10=--20= (dad)
	 *          |        =>        |        =>        |
	 *   (chi) 15=  10      (chi) 15b--10=     (chi) 15=
	 *             (new)              (new)
	 */
	dad_is_root = (dad == M->root);
	dad = IS_SON_DEEPER(child)? ucl_btree_avl_rot_left(dad) : ucl_btree_avl_rot_left_right(dad);
	if (dad_is_root) M->root = dad;
	assert(ucl_btree_avl_is_correct(dad));
	return true;
      }
    } else { /* stepping up from the bro subtree */
      switch (AVL_STATUS(dad)) {
      case UCL_SON_DEEPER:
	/* Stepping up  from a bro subtree  which got deeper,  we find a
	 * "dad" which  was son deeper:  it becomes equal depth  now and
	 * its depth HAS NOT changed.  Example:
	 *
	 *    before insertion       insertion         adjustment
	 *
	 *   (dad) (chi) (new)   (dad) (chi) (new)  (dad) (chi) (new)
	 *     20s--25=  28        20s--25b--28=      20=--25b--28=
	 *      |                   |                  |
	 *     15=--18=        =>  15=--18=      =>   15=--18=
	 *      |                   |                  |
	 *     10=                 10=                10=
	 */
	AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
	assert(ucl_btree_avl_is_correct(dad));
	return true;
      case UCL_EQUAL_DEPTH:
	/* Stepping up  from a bro subtree  which got deeper,  we find a
	 * "dad" which  was equal depth:  it becomes bro deeper  now and
	 * its depth HAS changed.  Example:
	 *
	 *    before insertion       insertion         adjustment
	 *
	 *   (dad) (chi) (new)   (dad) (chi) (new)  (dad) (chi) (new)
	 *     20=--25=  28        20=--25b--28=      20b--25b--28=
	 *      |             =>    |            =>    |
	 *     15=                 15=                15=
	 */
	AVL_STATUS(dad) = UCL_BRO_DEEPER;
	assert(ucl_btree_avl_is_correct(dad));
	break;
      case UCL_BRO_DEEPER:
	/* Stepping up  from a bro subtree  which got deeper,  we find a
	 * "dad"  which was  already bro  deeper; we  have to  perform a
	 * rotation after which:  the new "dad" will be  equal depth and
	 * its depth will be equal to the one of the old "dad".
	 *
	 * Example requiring right rotation:
	 *
	 *    before insertion       insertion         adjustment
	 *
	 *   (dad) (chi) (new)   (dad) (chi) (new)  (chi) (new)
	 *     20b--25=  28        20b--25b--28=      25=--28=
	 *                    =>                 =>    |
	 *                                            20= (dad)
	 *
	 * Example requiring right/left rotation:
	 *
	 *   before insertion      insertion      adjustment
	 *
	 *   (dad) (chi)          (dad) (chi)     (new) (chi)
	 *     20b--25=             20b--25s        28=--25=
	 *                     =>         |    =>    |
	 *    (new) 28             (new) 28=        20= (dad)
	 */
	dad_is_root = (dad == M->root);
	dad = IS_BRO_DEEPER(child)? ucl_btree_avl_rot_right(dad) : ucl_btree_avl_rot_right_left(dad);
	if (dad_is_root) M->root = dad;
	assert(ucl_btree_avl_is_correct(dad));
	return true;
      }
    }
  } /* end of for() */
}

void
ucl_map_delete (ucl_map_t M, void * del_)
/*
 * INTRODUCTION TO THE DELETION ALGORITHM
 *
 * This  is a brief  introduction to  make it  easier to  understand the
 * source code.
 *
 * To remove a node  from an AVL search tree: first we  push it down the
 * tree so that it becomes a leaf, then we detach it, finally we step up
 * in the tree updating the status of the ancestor nodes.
 *
 * It is always possible to swap the node N of a binary search tree with
 * the rightmost element  in its son's subtree, or  the leftmost element
 * in its bro's subtree, keeping  correct both the tree ordering and the
 * statuses of all the nodes but N itself; this is a desirable operation
 * because detaching a leaf is really easy.
 *
 * We mark the  status of a node  with "=" for equal depth,  "s" for son
 * deeper, "b" for bro deeper.  For example, pushing down the node 20 in
 * the following tree:
 *
 *   20s--30=        15s--30=
 *    |               |
 *   10=--15=   =>   10=--20
 *    |               |
 *    5=              5=
 *
 * keeps correct ordering and statuses, with the exception of 20; we see
 * that 15 gets the status that 20 had.
 *
 * We can then detach 20: the resulting tree is no more balanced and the
 * statuses are  no more  correct; we  can fix them  by stepping  up the
 * tree.  Node 10 lost the bro so now it is son deeper:
 *
 *    15s--30=
 *     |
 *   *10s
 *     |
 *     5=
 *
 * removing 20 has  not changed the depth of the subtree  of 10, so node
 * 15 must not change its status.
 *
 *   *15s--30=
 *     |
 *    10s
 *     |
 *     5=
 *
 * A more  complex example, removing 3  in the tree below  by pushing it
 * down its bro subtree.  First we swap it with 4, which is the leftmost
 * in the bro subtree:
 *
 *   3b------7=--8b--9=      4b------7=--8b--9=
 *   |       |               |       |
 *   1=--2=  5=--6=      =>  1=--2=  5=--6=
 *   |       |               |       |
 *   0=      4=              0=      3
 *
 * then we  detach it adjusting the  status of its dad,  notice that the
 * subtree of 5 has not got shorter:
 *
 *   4b------7=--8b--9=      4b------7=--8b--9=
 *   |       |               |       |
 *   1=--2=  5=--6=      =>  1=--2= *5b--6=
 *   |                       |
 *   0=      3               0=
 *
 * then  we step  up to  node 7,  we do  not need  to adjust  its status
 * because the subtree we come from has not got shorter:
 *
 *                                   *
 *   4b------7=--8b--9=      4b------7=--8b--9=
 *   |       |               |       |
 *   1=--2= *5b--6=      =>  1=--2=  5b--6=
 *   |                       |
 *   0=                      0=
 *
 * finally we  step up to node  4, we do  not need to adjust  its status
 * because the subtree we come from has not got shorter:
 *
 *   *
 *   4b------7=--8b--9=
 *   |       |
 *   1=--2=  5b--6=
 *   |
 *   0=
 *
 * Now what if we  remove 6?  We adjust the status of  5 and notice that
 * its subtree has got shorter:
 *
 *   4b------7=--8b--9=
 *   |       |
 *   1=--2= *5=
 *   |
 *   0=
 *
 * then we step  up to node 7;  it was equal depth before  and its son's
 * subtree has got shorter: its new status is bro deeper; the subtree of
 * 7 has NOT got shorter because its previous status was equal depth:
 *
 *           *
 *   4b------7b--8b--9=
 *   |       |
 *   1=--2=  5=
 *   |
 *   0=
 *
 * finally we  step up to node  4, we do  not need to adjust  its status
 * because the subtree we come from has not got shorter.
 *
 * Now let's remove node 9 and adjust the status of its dad; the subtree
 * of 8 has got shorter:
 *
 *               *
 *   4b------7b--8=
 *   |       |
 *   1=--2=  5=
 *   |
 *   0=
 *
 * the we step up to node 7: we come from its bro subtree, which has got
 * shorter, and its  status is bro deeper; the new status  of 7 is equal
 * depth; the  subtree of 7 has  got shorter, because it  was bro deeper
 * before:
 *
 *           *
 *   4b------7=--8=
 *   |       |
 *   1=--2=  5=
 *   |
 *   0=
 *
 * finally we step  up to node 4:  its status is bro deeper  and we come
 * from a  bro subtree  which has got  shorter; the  new status of  4 is
 * equal depth:
 *
 *   *
 *   4=------7=--8=
 *   |       |
 *   1=--2=  5=
 *   |
 *   0=
 */
{
  ucl_node_t	del = del_, dad, son, bro, tmp, child;
  ucl_bool_t	dad_is_root;
  assert(ucl_map_find_node(M, del));
  /* Handle the case of map with single node.  Notice that this function
     cannot be called with an empty map. */
  if (1 == M->size) {
    assert(del == M->root);
    M->root = NULL;
    M->size = 0;
    return;
  }
  /* It is always possible to swap the root of a binary search tree with
     the rightmost element in its son's subtree, or the leftmost element
     in its bro's  subtree, and keep correct both  the tree ordering and
     the AVL statuses of all the nodes but the old root itself.

     The  following  loop swaps  "del"  with  elements  in its  subtree,
     pushing it  down until  it becomes  a leaf; this  is because  it is
     really easy to remove a leaf. */
  while (SON_OF(del) || BRO_OF(del)) {
    son = SON_OF(del);
    bro = BRO_OF(del);
    tmp = son? ucl_btree_find_rightmost(son) : ucl_btree_find_leftmost(bro);
    ucl_btree_swap(del, tmp);
    if (del == M->root)
      M->root = tmp;
  }
  assert(IS_LEAF(del));
  /* Remove the  leaf "del" from the  tree and adjust the  status of its
     dad.  Notice  that "dad" cannot be  NULL because we  have made sure
     that the case of map with one element has been handled earlier. */
  dad = DAD_OF(del);
  size_decr(M);
  if (del == SON_OF(dad)) {
    SON(dad) = NULL;
    if (BRO_OF(dad)) {
      assert(! IS_SON_DEEPER(dad));
      /* We have removed the leaf son of a tree having a bro.
       *
       *       dad--bro ...      dad--bro ...
       *        |    .       =>        .
       *       del   .                 .
       */
      if (IS_EQUAL_DEPTH(dad)) {
        /* We have  removed the  leaf son of  a node being  equal depth;
         * example:
         *
         *  (dad) 5=--6= (bro)      (dad) 5b--6= (bro)
         *        |             =>
         *  (del) 4=
         *
         * notice that  the depth of "dad"  tree does not  change: we do
         * NOT have to update the status of the upper nodes.
         */
        AVL_STATUS(dad) = UCL_BRO_DEEPER;
	assert(IS_LEAF(dad->bro));
	assert(ucl_btree_avl_is_correct(dad));
	return;
      } else {
        assert(IS_BRO_DEEPER(dad));
        /* We have removed  the leaf son of a node  being bro deeper; we
	 * need a rotation to rebalance the tree; there are 3 cases.
         *
         * The  bro of  "dad" is  bro  deeper: we  can only  do a  right
         * rotation, after which the "dad" tree HAS got shorter:
         *
         *           (bro)                (bro)
         *  (dad) 5b--8b--9=     (dad) 5b--8b--9=     (bro) 8=--9=
         *        |           =>                   =>       |
         *  (del) 4=                                  (dad) 5=
         *
         * The bro of  "dad" is son deeper: we can  only do a right/left
         * rotation, after which the "dad" tree HAS got shorter:
         *
         *  (dad) 5b--8s (bro)    (dad) 5b--8s (bro)          7=--8= (bro)
         *        |   |        =>           |        =>       |
         *  (del) 4=  7=                    7=          (dad) 5=
         *
         * The bro  of "dad" is equal  depth: in principle,  we could do
         * either a right rotation or  a right/lerft rotation.  If we do
         * a right rotation "dad" tree DOES NOT get shorter:
         *
         *          (bro)                 (bro)
         *  (dad) 5b--8=--9=      (dad) 5b--8=--9=     (bro) 8s--9=
         *        |   |       =>            |       =>       |
         *  (del) 4=  7=                    7=         (dad) 5b--7=
	 *
         * if  we do  a  right/left  rotation "dad"  tree  DOES NOT  get
         * shorter:
         *
         *          (bro)                 (bro)                (bro)
         *  (dad) 5b--8=--9=      (dad) 5b--8=--9=           7b--8b--9=
         *        |   |       =>            |       =>       |
         *  (del) 4=  7=                    7=         (dad) 5=
         *
         * we select the right rotation.
         */
        dad_is_root = (M->root == dad);
	switch (AVL_STATUS(dad->bro)) {
	case UCL_BRO_DEEPER:
	  dad = ucl_btree_avl_rot_right(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_SON_DEEPER:
	  dad = ucl_btree_avl_rot_right_left(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_EQUAL_DEPTH:
	  dad = ucl_btree_avl_rot_right(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  return;
	}
      }
    } else {
      assert(! BRO_OF(dad));
      assert(IS_SON_DEEPER(dad));
      /* We have removed  the leaf son of a tree  being son deeper.  The
       * tree has got shorter: we have to update the status of the upper
       * nodes.
       *
       *       dad      dad
       *	|   =>
       *       del
       */
      AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
      assert(IS_LEAF(dad));
    }
  } else {
    assert(del == BRO_OF(dad));
    BRO(dad) = NULL;
    if (SON_OF(dad)) {
      assert(! IS_BRO_DEEPER(dad));
      /* We have removed the leaf bro of a tree having a son.
       *
       *	dad--del       dad
       *	 |         =>   |
       *	son ...        son ...
       *	 .              .
       *	 .              .
       */
      if (IS_EQUAL_DEPTH(dad)) {
        /* We have  removed the  leaf bro of  a node being  equal depth;
         * example:
         *
         *  (dad) 5=--6= (del)      (dad) 5s
         *        |             =>        |
         *  (son) 4=                (son) 4=
         *
         * notice that  the depth of "dad"  tree does not  change: we do
         * NOT have to update the status of the upper nodes.
         */
        AVL_STATUS(dad) = UCL_SON_DEEPER;
	assert(IS_LEAF(dad->son));
	assert(ucl_btree_avl_is_correct(dad));
	return;
      } else {
        assert(IS_SON_DEEPER(dad));
        /* We have removed  the leaf bro of a node  being son deeper; we
         * need a  rotation to rebalance  the tree; there are  3 cases.
         *
         * The  son of  "dad"  is son  deeper:  we can  only  do a  left
         * rotation, after which "dad" tree HAS got shorter.
         *
         *  (dad) 5s--8= (del)    (dad) 5s         (son) 3=--5= (dad)
         *        |                     |                |
         *  (son) 3s           => (son) 3s      =>       2=
         *        |                     |
         *        2=                    2=
         *
	 * The son of  "dad" is bro deeper: we can  only do a left/right
	 * rotation, after which "dad" tree HAS got shorter:
         *
         *  (dad) 5s--8= (del)    (dad) 5s               4=--5= (dad)
         *        |                     |                |
         *  (son) 3b--4=       => (son) 3b--4=  => (son) 3=
         *
         * The son  of "dad" is equal  depth: in principle,  we could do
         * either a left rotation or  a left/right rotation.  If we do a
         * left rotation "dad" tree DOES NOT get shorter:
         *
         *  (dad) 5s--8= (del)    (dad) 5s         (son) 3b--5s (dad)
         *        |                     |                |   |
         *  (son) 3=--4=       => (son) 3=--4=  =>       2=  4=
         *        |                     |
         *        2=                    2=
         *
         * if  we  do a  left/right  rotation  the  three DOES  NOT  get
         * shorter:
         *
         *  (dad) 5s--8= (del)    (dad) 5s               4s--5= (dad)
         *        |                     |                |
         *  (son) 3=--4=       => (son) 3=--4=  => (son) 3s
         *        |                     |                |
         *        2=                    2=               2=
         *
         * we select a left rotation.
         */
        dad_is_root = (M->root == dad);
	switch (AVL_STATUS(dad->son)) {
	case UCL_SON_DEEPER:
	  dad = ucl_btree_avl_rot_left(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_BRO_DEEPER:
	  dad = ucl_btree_avl_rot_left_right(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_EQUAL_DEPTH:
	  dad = ucl_btree_avl_rot_left(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  return;
	}
      }
    } else {
      assert(! SON_OF(dad));
      assert(IS_BRO_DEEPER(dad));
      /* We have removed  the leaf bro of a node  being bro deeper.  The
       * tree has got shorter: we have to update the status of the upper
       * nodes.
       *
       *     dad--del   =>   dad
       *
       */
      AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
      assert(IS_LEAF(dad));
    }
  }
  /* If the function has not  returned yet, it is because removing "del"
     made a subtree shorter than before,  so we have to step up the tree
     and update the  AVL status of ancestor nodes,  doing rotations when
     required to keep the tree balanced.

     We perform  a loop in  which "child" is  the node we come  from and
     "dad" is the node that needs  to be balanced.  We iterate while the
     tree having "dad"  as roo has got shorter with  the deletion: if it
     has not changed  its depth, nothing changes from  the point of view
     of its ancestor nodes, so no status updates are required. */
  /* *** NOTE ***

     Readjusting  the  balancing  after  a deletion  is  DIFFERENT  from
     readjusting after an insertion.   We CANNOT take the following loop
     and  factor  it  out  joining  it  with the  loop  at  the  end  of
     "ucl_map_insert()". */
  for (;;) {
    child=dad;
    dad=DAD_OF(child);
    if (!dad) return; /* "child" is the root */
    if (child == SON_OF(dad)) { /* The son subtree has got shorter. */
      switch (AVL_STATUS(dad)) {
      case UCL_SON_DEEPER:
	/* We come from  the son of "dad" and "dad"  was son deeper: the
	 * tree having "dad" as root HAS got shorter.  Example:
	 *
	 *    dad--bro       dad--bro
	 *     |        =>    |
	 *    chi            chi
	 *     |
	 *    del
	 */
	AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
	assert(ucl_btree_avl_is_correct(dad));
	break;
      case UCL_EQUAL_DEPTH:
	/* We come from the son of  "dad" and "dad" was equal depth: the
	 * tree having "dad" as root HAS NOT got shorter.  Example:
	 *
	 *    dad--bro--two       dad--bro--two
	 *     |    |        =>    |    |
	 *    chi  one            chi  one
	 *     |
	 *    del
	 */
	AVL_STATUS(dad) = UCL_BRO_DEEPER;
	assert(ucl_btree_avl_is_correct(dad));
	return;
      case UCL_BRO_DEEPER:
	/* We come from  the son of "dad" and "dad"  was bro deeper: the
	 * tree  having "dad"  as root  HAS NOT  got shorter  but  it is
	 * unbalanced in the bro.  There are 3 cases.
	 *
	 * The  bro of  "dad"  is son  deeper:  we can  do a  right/left
	 * rotation, after which "dad" tree HAS got shorter:
	 *
	 *    dad--bro--two      dad--bro--two      one--bro--two
	 *     |    |             |    |             |
	 *    chi  one       =>  chi  one       =>  dad--thr
	 *     |    |                  |             |
	 *    del  thr                thr           chi
	 *
	 * The bro of  "dad" is bro deeper: we can  do a right rotation,
	 * after which "dad" tree HAS got shorter:
	 *
	 *    dad--bro--two--fou    dad--bro--two--fou    bro-------two--fou
	 *     |    |    |           |    |    |           |         |
	 *    chi  one  thr      => chi  one  thr      => dad--one  thr
	 *     |                                           |
	 *    del                                         chi
	 *
	 * The bro  of "dad" is equal  depth: in principle,  we could do
	 * either a right rotation or a right/left rotation.  If we do a
	 * right rotation, "dad" tree DOES NOT get shorter:
	 *
	 *    dad--bro--two--fou    dad--bro--two--fou    bro--two--fou
	 *     |    |                |    |                |
	 *    chi  one           => chi  one           => dad--one
	 *     |    |                     |                |    |
	 *    del  thr                   thr              chi  thr
	 *
	 * if  we do  a right/left  rotation,  "dad" tree  DOES NOT  get
	 * shorter  and  there  is  the  case  that  the  tree  is  left
	 * unbalanced in the bro subtree:
	 *
	 *    dad--bro--two--fou    dad--bro--two--fou    one--bro--two--fou
	 *     |    |                |    |                |
	 *    chi  one           => chi  one           => dad--thr
	 *     |    |                     |                |
	 *    del  thr                   thr              chi
	 *
	 * so we must do a right rotation.
         */
        dad_is_root = (M->root == dad);
	switch (AVL_STATUS(dad->bro)) {
	case UCL_BRO_DEEPER:
	  dad = ucl_btree_avl_rot_right(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_SON_DEEPER:
	  dad = ucl_btree_avl_rot_right_left(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_EQUAL_DEPTH:
	  dad = ucl_btree_avl_rot_right(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  return;
	}
      }
    } else { /* The bro subtree has got shorter. */
      assert(child == BRO_OF(dad));
      switch (AVL_STATUS(dad)) {
      case UCL_BRO_DEEPER:
	/* We come from  the bro of "dad" and "dad"  was bro deeper: the
	 * tree having "dad" as root HAS got shorter.  Example:
	 *
	 *    dad--chi--del      dad--chi
	 *     |             =>   |
	 *    son                son
	 */
	AVL_STATUS(dad) = UCL_EQUAL_DEPTH;
	assert(ucl_btree_avl_is_correct(dad));
	break; /* from the switch() */
      case UCL_EQUAL_DEPTH:
	/* We come from the bro of  "dad" and "dad" was equal depth: the
	 * tree having "dad" as root HAS NOT got shorter.  Example:
	 *
	 *    dad--chi--del      dad--chi
	 *     |             =>   |
	 *    son--one           son--one
	 *     |                  |
	 *    two                two
	 */
	AVL_STATUS(dad) = UCL_SON_DEEPER;
	assert(ucl_btree_avl_is_correct(dad));
	return;
      case UCL_SON_DEEPER:
	/* We come from  the bro of "dad" and "dad"  was son deeper: the
         * tree  having "dad" as  root HAS  NOT got  shorter but  it has
         * become unbalanced in the son.  There are 3 cases.
         *
         * The son of "dad" is son  deeper: we do a left rotation, after
         * which "dad" tree HAS got shorter:
         *
         *   dad--chi--del    dad--chi        son--dad--chi
         *    |                |               |    |
         *   son--one      => son--one    =>   |   one
         *    |                |               |
         *   two--thr         two--thr        two--thr
         *
         * The son of "dad" is  bro deeper: we do a left/right rotation,
         * after which "dad" tree HAS got shorter:
         *
         *   dad--chi--del     dad--chi        one--dad--chi
         *    |                 |               |
         *   son--one      =>  son--one    =>  son--thr
         *    |    |            |    |          |
         *   two  thr          two  thr        two
	 *
         * The son  of "dad"  is equal depth:  in principle, we  coul do
         * either a left rotation or  a left/right rotation.  If we do a
         * left rotation, "dad" tree DOES NOT get shorter:
         *
         *   dad--chi--del    dad--chi         son--dad--chi
         *    |                |                |    |
         *   son--two--fou => son--two--fou => one  two--fou
         *    |                |                |
         *   one              one              thr
         *    |                |
         *   thr              thr
         *
	 * If  we do  a left/right  rotation,  "dad" tree  DOES NOT  get
	 * shorter  but the  tree  may  be left  unbalanced  in the  son
	 * subtree:
         *
         *   dad--chi--del    dad--chi         two--dad--chi
         *    |                |                |    |
         *   son--two--fou => son--two--fou => son  fou
         *    |                |                |
         *   one              one              one
         *    |                |                |
         *   thr              thr              thr
	 *
         * we must do a left rotation.
         */
	debug("dad's son status is %s", STATUS_STRING(AVL_STATUS(SON_OF(dad))));
        dad_is_root = (M->root == dad);
	switch (AVL_STATUS(dad->son)) {
	case UCL_SON_DEEPER:
	  dad = ucl_btree_avl_rot_left(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_BRO_DEEPER:
	  dad = ucl_btree_avl_rot_left_right(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  break;
	case UCL_EQUAL_DEPTH:
	  dad = ucl_btree_avl_rot_left(dad);
	  if (dad_is_root) M->root = dad;
	  assert(ucl_btree_avl_is_correct(dad));
	  return;
	}
      }
    }
  } /* end of for() loop */
}

void *
ucl_map_find (const ucl_map_t M, const ucl_value_t K)
{
  ucl_node_t 	cur = M->root, last;
  int		comparison_result;
  while (cur) {
    comparison_result = comparison_key_node(M, K, cur);
    if (comparison_result > 0)
      cur = BRO(cur);
    else if (comparison_result < 0)
      cur = SON(cur);
    else {
      if (M->flags & UCL_ALLOW_MULTIPLE_OBJECTS) {
	do {
	  last = cur;
	  cur  = ucl_btree_step_inorder_backward(last);
	} while (cur && (0 == comparison_key_node(M, K, cur)));
	return last;
      } else
	return cur;
    }
  }
  return NULL;
}
ucl_bool_t
ucl_map_find_node (const ucl_map_t M, void * N)
{
  ucl_value_t		K = node_key(M, N);
  ucl_iterator_t	I;
  for (ucl_map_lower_bound(M, I, K); ucl_iterator_more(I); ucl_iterator_next(I))
    if (N == ucl_iterator_ptr(I))
      return true;
  return false;
}
size_t
ucl_map_depth (const ucl_map_t M)
{
  return ucl_btree_avl_depth(M->root);
}

/** ------------------------------------------------------------
 ** Traversing.
 ** ----------------------------------------------------------*/

void *
ucl_map_first (const ucl_map_t M)
{
  return (M->size)? ucl_btree_find_leftmost(M->root) : NULL;
}
void *
ucl_map_last (const ucl_map_t M)
{
  return (M->size)? ucl_btree_find_rightmost(M->root) : NULL;
}
void *
ucl_map_next (void * L)
{
  return ucl_btree_step_inorder(L);
}
void *
ucl_map_prev (void * L)
{
  return ucl_btree_step_inorder_backward(L);
}

void *
ucl_map_find_or_next (const ucl_map_t M, const ucl_value_t K)
{
  ucl_node_t 	cur, last;
  int		v;
  assert(M);
  /* Handle the case of empty map. */
  cur = M->root;
  if (! cur) return cur;
  /* Dive in the tree to find the key. */
  while (cur) {
    last = cur;
    v = comparison_key_node(M, K, cur);
    if (v > 0)
      cur = BRO_OF(last);
    else if (v < 0)
      cur = SON_OF(last);
    else { /* v == 0 */
      /* If there  are more links with  key K, we step  to the rightmost
         one. */
      if (M->flags & UCL_ALLOW_MULTIPLE_OBJECTS) {
	do {
	  last = cur;
	  cur  = ucl_btree_step_inorder(last);
	  if (! cur) break;
	  v = comparison_key_node(M, K, cur);
	} while (0 == v);
      }
      return last;
    }
  }
  /* If  we're here  "last" holds  a  pointer to  the last  node in  the
     previous search:  if "v < 0", this  node is the one  we are looking
     for;  else we  have  to do  an  inorder step  forward  to find  the
     requested one. */
  return (v < 0)? last : ucl_btree_step_inorder(last);
}

void *
ucl_map_find_or_prev (const ucl_map_t M, const ucl_value_t K)
{
  ucl_node_t 	cur, last;
  int		v;
  assert(M);
  /* Handle the case of empty map. */
  cur = M->root;
  if (! cur) return cur;
  /* Dive in the tree to find the key. */
  while (cur) {
    last = cur;
    v = comparison_key_node(M, K, cur);
    if (v > 0)
      cur = BRO_OF(last);
    else if (v < 0)
      cur = SON_OF(last);
    else { /* v == 0 */
      /* If there  are more links  with key K,  we step to  the leftmost
         one. */
      if (M->flags & UCL_ALLOW_MULTIPLE_OBJECTS) {
	do {
	  last = cur;
	  cur  = ucl_btree_step_inorder_backward(last);
	  if (! cur) break;
	  v = comparison_key_node(M, K, cur);
	} while (0 == v);
      }
      return last;
    }
  }
  /* If  we're here  "last" holds  a  pointer to  the last  node in  the
     previous search:  if "v > 0", this  node is the one  we are looking
     for;  else we  have to  do  an inorder  step backward  to find  the
     requested one. */
  return (v > 0)? last : ucl_btree_step_inorder_backward(last);
}

size_t
ucl_map_count (const ucl_map_t M, const ucl_value_t K)
{
  size_t	count = 0;
  ucl_node_t 	L, first;
  assert(M);
  L = first = ucl_map_find(M, K);
  if (L) {
    if (! (UCL_ALLOW_MULTIPLE_OBJECTS & M->flags))
      return 1;
    do {
      ++count;
      L = ucl_map_next(L);
    } while (L && (0 == comparison_key_node(M, K, L)));
    L = ucl_map_prev(first);
    while (L && (0 == comparison_key_node(M, K, L))) {
      ++count;
      L = ucl_map_prev(L);
    }
  }
  return count;
}

/** ------------------------------------------------------------
 ** Iterators.
 ** ----------------------------------------------------------*/

void
ucl_map_iterator_inorder (const ucl_map_t M, ucl_iterator_t I)
{
  assert(M);
  assert(I);
  I->container = M;
  if (0 == M->size)
    I->iterator = 0;
  else {
    I->iterator	= ucl_btree_find_leftmost(M->root);
    I->next	= map_inorder_iterator_next;
  }
}
void
ucl_map_iterator_inorder_backward (const ucl_map_t M, ucl_iterator_t I)
{
  assert(M);
  assert(I);
  I->container = M;
  if (M->size == 0)
    I->iterator = 0;
  else {
    I->iterator	= ucl_btree_find_rightmost(M->root);
    I->next	= map_inorder_backward_iterator_next;
  }
}
void
ucl_map_iterator_preorder (const ucl_map_t M, ucl_iterator_t I)
{
  assert(M);
  assert(I);
  I->container = M;
  if (0 == M->size)
    I->iterator = 0;
  else {
    I->iterator	= M->root;
    I->next	= map_preorder_iterator_next;
  }
}
void
ucl_map_iterator_postorder (const ucl_map_t M, ucl_iterator_t I)
{
  assert(M);
  assert(I);
  I->container = M;
  if (M->size == 0)
    I->iterator = 0;
  else {
    I->iterator	= ucl_btree_find_deepest_son(M->root);
    I->next	= map_postorder_iterator_next;
  }
}
void
ucl_map_iterator_levelorder (const ucl_map_t M, ucl_iterator_t I)
{
  assert(M);
  assert(I);
  I->container = M;
  if (M->size == 0)
    I->iterator = 0;
  else {
    I->iterator	= M->root;
    I->next	= map_levelorder_iterator_next;
  }
}
void
ucl_map_lower_bound (const ucl_map_t M, ucl_iterator_t I, const ucl_value_t K)
{
  assert(M);
  assert(I);
  I->container = M;
  if (! M->size)
    I->iterator = 0;
  else {
    I->next	= map_lowerbound_iterator_next;
    I->iterator	= ucl_map_find(M, K);
    if (I->iterator && IS_MULTIMAP(M)) {
      /* When the container is a multimap: the first node found with key
         "K" may not be the leftmost, so we need to step backwards until
         we are sure to have found the leftmost. */
      for (ucl_node_t N = ucl_btree_step_inorder_backward(I->iterator);
           N && 0 == comparison_key_node(M, K, N);
           N = ucl_btree_step_inorder_backward(N))
        I->iterator = N;
    }
  }
}
void
ucl_map_upper_bound (const ucl_map_t M, ucl_iterator_t I, const ucl_value_t K)
{
  assert(M);
  assert(I);
  I->container = M;
  if (0 == M->size)
    I->iterator = NULL;
  else {
    I->next	= map_upperbound_iterator_next;
    I->iterator = ucl_map_find(M, K);
    if (I->iterator && IS_MULTIMAP(M)) {
      /* When the container is a multimap: the first node found with key
         "K" may not be the rightmost, so we need to step forwards until
         we are sure to have found the rightmost. */
      for (ucl_node_t N = ucl_btree_step_inorder(I->iterator);
           N && 0 == comparison_key_node(M, K, N);
           N = ucl_btree_step_inorder(N))
        I->iterator = N;
    }
  }
}

/* ------------------------------------------------------------ */

static void
map_inorder_iterator_next (ucl_iterator_t I)
{
  I->iterator = ucl_btree_step_inorder(I->iterator);
}
static void
map_inorder_backward_iterator_next (ucl_iterator_t I)
{
  I->iterator = ucl_btree_step_inorder_backward(I->iterator);
}
static void
map_preorder_iterator_next (ucl_iterator_t I)
{
  I->iterator = ucl_btree_step_preorder(I->iterator);
}
static void
map_postorder_iterator_next (ucl_iterator_t I)
{
  I->iterator = ucl_btree_step_postorder(I->iterator);
}
static void
map_levelorder_iterator_next (ucl_iterator_t I)
{
  I->iterator = ucl_btree_step_levelorder(I->iterator);
}
static void
map_lowerbound_iterator_next (ucl_iterator_t I)
{
  assert(I->iterator);
  ucl_node_t	L = I->iterator;
  ucl_value_t	K = node_key(I->container, L);
  L = ucl_btree_step_inorder(L);
  if (L)
    I->iterator = (0 == comparison_key_node(I->container, K, L))? L : NULL;
  else
    I->iterator = L;
}
static void
map_upperbound_iterator_next (ucl_iterator_t I)
{
  assert(I->iterator);
  ucl_node_t	L = I->iterator;
  ucl_value_t	K = node_key(I->container, L);
  L = ucl_btree_step_inorder_backward(L);
  if (L)
    I->iterator = (0 == comparison_key_node(I->container, K, L))? L : NULL;
  else
    I->iterator = L;
}

void
ucl_map_iterator_union (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  assert(I1);
  assert(I2);
  assert(I);
  if (ucl_iterator_more(I1) || ucl_iterator_more(I2)) {
    I->internal1.pointer	= I1;
    I->internal2.pointer	= I2;
    I->next			= union_iterator_next;
    union_find_next(I1, I2, I);
  } else
    I->iterator = NULL;
}
static void
union_iterator_next (ucl_iterator_t I)
{
  ucl_iterator_tag_t * 	I1;
  ucl_iterator_tag_t *	I2;
  I1 = I->internal1.pointer;
  I2 = I->internal2.pointer;
  ucl_iterator_next((I->container == I1)? I1 : I2);
  union_find_next(I1, I2, I);
}
static void
union_find_next (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    int	v = comparison_node_node(I1->container, ucl_iterator_ptr(I1), ucl_iterator_ptr(I2));
    if (v <= 0) {
      I->container = I1;
      I->iterator  = I1->iterator;
    } else {
      I->container = I2;
      I->iterator  = I2->iterator;
    }
  } else if (ucl_iterator_more(I1)) {
    I->container = I1;
    I->iterator  = I1->iterator;
  } else if (ucl_iterator_more(I2)) {
    I->container = I2;
    I->iterator  = I2->iterator;
  } else {
    I->iterator = NULL;
  }
}

void
ucl_map_iterator_intersection (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  assert(I1);
  assert(I2);
  assert(I);
  if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    I->internal1.pointer	= I1;
    I->internal2.pointer	= I2;
    I->next			= intersection_iterator_next;
    intersection_find_common(I1, I2, I);
  } else
    I->iterator = NULL;
}
static void
intersection_iterator_next (ucl_iterator_t I)
{
  ucl_iterator_tag_t * 	I1 = I->internal1.pointer;;
  ucl_iterator_tag_t * 	I2 = I->internal2.pointer;;
  ucl_iterator_next(I1);
  ucl_iterator_next(I2);
  intersection_find_common(I1, I2, I);
}
static void
intersection_find_common (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  while (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    int v = comparison_node_node(I1->container, ucl_iterator_ptr(I1), ucl_iterator_ptr(I2));
    if (0 == v) {
      I->iterator = I1->iterator;
      return;
    } else if (v < 0)
      ucl_iterator_next(I1);
    else
      ucl_iterator_next(I2);
  }
  I->iterator = NULL;
}

void
ucl_map_iterator_complintersect	(ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  assert(I1);
  assert(I2);
  assert(I);
  if (ucl_iterator_more(I1) || ucl_iterator_more(I2)) {
    I->internal1.pointer	= I1;
    I->internal2.pointer	= I2;
    I->next			= complintersect_iterator_next;
    I->container		= NULL;
    complintersect_iterator_next(I);
  } else
    I->iterator = NULL;
}
static void
complintersect_iterator_next (ucl_iterator_t I)
{
#undef COMPARISON
#define COMPARISON()    comparison_node_node(I1->container, ucl_iterator_ptr(I1), ucl_iterator_ptr(I2))
  ucl_iterator_tag_t * 	I1 = I->internal1.pointer;
  ucl_iterator_tag_t *	I2 = I->internal2.pointer;
  int			comparison_result;
  /* Advance  the sub-iterator  that  was  last used  as  source of  the
     current element:  that element has  been consumed.  If this  is the
     first call to iterator_next: do not advance any iterator. */
  if (I->container == I1)
    ucl_iterator_next(I1);
  else if (I->container == I2)
    ucl_iterator_next(I2);
  else
    assert(NULL == I->container);
  /* If both sub-iterators have elements: we have to decide which one is
     the new current. */
  if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    /* We  discard all  the elements  present in  both  the subiterators
       until  we find  the first  couple  of different  elements or  one
       sub-iterator runs out of elements. */
    for (comparison_result = COMPARISON(); 0 == comparison_result; comparison_result = COMPARISON()) {
      ucl_iterator_next(I1);
      ucl_iterator_next(I2);
      if (! (ucl_iterator_more(I1) && ucl_iterator_more(I2)))
	break;
    }
    /* If the last comparison was  non-zero and there are still elements
       in  both sub-iterators:  select as  current the  smallest element
       from the sub-iterators. */
    if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
      if (comparison_result < 0) {
	I->iterator  = I1->iterator;
	I->container = I1;
      } else if (comparison_result > 0) {
	I->iterator  = I2->iterator;
	I->container = I2;
      }
      return;
    }
  }
  /* We are here when  one or both "I1" and "I2" has  no more values; we
     just replace "I" with the one that still has values and go on until
     it is empty, too.  If both are empty: we just end the iteration. */
  if (ucl_iterator_more(I1)) {
    assert(! ucl_iterator_more(I2));
    I->iterator  = I1->iterator;
    I->container = I1;
  } else if (ucl_iterator_more(I2)) {
    assert(! ucl_iterator_more(I1));
    I->iterator  = I2->iterator;
    I->container = I2;
  } else
    I->iterator = NULL;
}

void
ucl_map_iterator_subtraction (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  assert(I1);
  assert(I2);
  assert(I);
  if (ucl_iterator_more(I1)) {
    I->internal1.pointer	= I1;
    I->internal2.pointer	= I2;
    I->next			= subtraction_iterator_next;
    I->container		= (ucl_iterator_more(I2))? NULL : I1;
    subtraction_iterator_next(I);
  } else
    I->iterator = NULL;
}
static void
subtraction_iterator_next (ucl_iterator_t I)
/* Advances the  subtraction iterator.   When this function  is invoked,
   the  iterators for both  the sequences  must be  already initialised.
   The elements from  the sequences referenced by the  iterators must be
   the next to be examined.

   This  means  that  we  must  end this  function  invocation  with  an
   advancement  of the  iterators.  So that  at  the next  call the  new
   elements are ready to be analysed.

   If  the  iteration over  sequence  2 is  over,  all  the values  from
   sequence 1 are visited.

   The algorithm is:

   * if the value from 1 is lesser  than the value from 2, select it and
   return;

   * if  the value  from 1  is greater  or equal  to the  value  from 2,
   advance the second iterator until the value from 1 is lesser than the
   value from 2; then select the value from 1 and return.
*/
{
  ucl_iterator_tag_t * 	I1;
  ucl_iterator_tag_t *	I2;
  int			v;
  /* If no  more values in sequence  1, end.  This cannot  happen at the
     first invocation of this  function: this condition is recognised in
     the initialisation function. */
  I1 = I->internal1.pointer;
  if (! ucl_iterator_more(I1)) {
    I->iterator = NULL;
    return;
  }
  /* Examine the second iterator.  If  the iteration over sequence 2 was
     ended in a previous step, select the next value from sequence 1 and
     return. */
  I2 = I->internal2.pointer;
  if (! ucl_iterator_more(I2))
    goto Advance;
  /* If  the element from  sequence 1  is lesser  than the  element from
     sequence 2, we select it and return. */
  v = comparison_node_node(I1->container, ucl_iterator_ptr(I1), ucl_iterator_ptr(I2));
  if (v < 0)
    goto Advance;
  /* Here we have  to advance the iterator over sequence  2 until we end
     it, or until the value is grater than the value from sequence 1. */
  do {
    ucl_iterator_next(I2);
    if (! ucl_iterator_more(I2))
      break;
    if (0 == v) {
      ucl_iterator_next(I1);
      if (! ucl_iterator_more(I1)) {
	I->iterator = NULL;
	return;
      }
    }
    v = comparison_node_node(I1->container, ucl_iterator_ptr(I1), ucl_iterator_ptr(I2));
  } while (v >= 0);
  if ((! ucl_iterator_more(I2)) && 0 == v) {
    ucl_iterator_next(I1);
    if (! ucl_iterator_more(I1)) {
      I->iterator = NULL;
      return;
    }
  }
 Advance:
  I->iterator = I1->iterator;
  ucl_iterator_next(I1);
}

/* end of file */
