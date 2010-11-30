/*
  Part of: Useless Containers Library
  Contents: code for the map/multimap container.

  Abstract:

	The map container  is built as an AVL tree;  the brother of each
	tree link holds an element  with greater or equal key, while the
	son  holds an  element with  lesser  key. For  the multimap:  an
	element with equal key is considered as one with greater key.

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

  Copyright (c) 2001-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

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
 ** -----------------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		1
#endif
#include "internal.h"

#define SON_DEEPER	UCL_SON_DEEPER
#define EQUAL_DEPTH	UCL_EQUAL_DEPTH
#define BRO_DEEPER	UCL_BRO_DEEPER

#define AVL_STATUS(L)	(L)->avl_status

/* to be used as right-side in assignments */
#define DAD_OF(L)		((void *)((L)->node.dad))
#define SON_OF(L)		((void *)((L)->node.son))
#define BRO_OF(L)		((void *)((L)->node.bro))

/* to be used as left-side in assignments */
#define DAD(L)		((L)->node.dad)
#define SON(L)		((L)->node.son)
#define BRO(L)		((L)->node.bro)

static void	map_inorder_iterator_next	(ucl_iterator_t iterator);
static void	map_inorder_backward_iterator_next (ucl_iterator_t iterator);
static void	map_preorder_iterator_next	(ucl_iterator_t iterator);
static void	map_postorder_iterator_next	(ucl_iterator_t iterator);
static void	map_levelorder_iterator_next	(ucl_iterator_t iterator);
static void	map_upperbound_iterator_next	(ucl_iterator_t iterator);
static void	map_lowerbound_iterator_next	(ucl_iterator_t iterator);

static ucl_map_link_t rot_left			(ucl_map_link_t old_cur);
static ucl_map_link_t rot_left_right		(ucl_map_link_t old_cur);
static ucl_map_link_t rot_right			(ucl_map_link_t old_cur);
static ucl_map_link_t rot_right_left		(ucl_map_link_t old_cur);

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


void
ucl_map_initialise (ucl_map_t M, unsigned int flags, ucl_comparison_t keycmp)
{
  assert(M);
  assert(keycmp.func);
  M->root		= NULL;
  M->size		= 0;
  M->keycmp		= keycmp;
  M->flags		= flags;
}

ucl_bool_t
ucl_map_insert (ucl_map_t M, ucl_map_link_t L)
{
  ucl_map_link_t tmp, current;
  ucl_value_t	 K;
  int		 comparison_result;
  ucl_bool_t	 allow_multiple_objects;
  assert(M);
  assert(L);
  /* The new link will have AVL status equal to "balanced".  Always. */
  AVL_STATUS(L) = EQUAL_DEPTH;
  DAD(L) = BRO(L) = SON(L) = NULL;
  /* Handle the case of empty map. */
  if (! M->root) {
    assert(0 == M->size);
    M->root = L;
    ++(M->size);
    return true;
  }
  /* If the map is not empty, we  have to look for the node that will be
     the parent of the new one.  If  "M" is not a multimap and we find a
     node with key equal to the key of "L": we silently do nothing. */
  tmp = M->root;
  K = ucl_map_getkey(L);
  allow_multiple_objects = M->flags & UCL_ALLOW_MULTIPLE_OBJECTS;
  while (tmp) {
    current = tmp;
    comparison_result = M->keycmp.func(M->keycmp.data, K, ucl_map_getkey(tmp));
    if ((comparison_result > 0) || (comparison_result==0 && allow_multiple_objects)) {
      tmp = BRO(current);
    } else if (comparison_result < 0) {
      tmp = SON(current);
    } else {
      assert(comparison_result == 0 && !allow_multiple_objects);
      return false;
    }
  }
  /*Here "current"  holds a pointer to  the parent of the  new link.  If
   *"comparison_result<0",  we have  to insert  the new  link as  son of
   *"current"  (in the  left subtree),  else  as brother  (in the  right
   *subtree).
   *
   *If we append the new link  to the left of "current", the link itself
   *becomes the left subtree of "current".
   *
   *     -----
   *    | cur |----> ?
   *     -----  bro
   *       |
   *   son v
   *     -----
   *    |link |
   *     -----
   *
   *There are two cases:
   *
   *1)  The brother  of  "current" is  NULL:  "current" was  EQUAL_DEPTH
   *before and becomes SON_DEEPER now;  we have to update the AVL status
   *of the upper nodes.
   *
   *2) The  brother of "current"  is not NULL: "current"  was BRO_DEEPER
   *before and becomes  EQUAL_DEPTH now; there is no  need to update the
   *AVL status of the upper nodes.
   *
   *If we append the new link to the right of "current", the link itself
   *becomes the right subtree of "current".
   *
   *     -----       -----
   *    | cur |---->|link |
   *     -----  bro  -----
   *       |
   *   son v
   *       ?
   *
   *There are two cases:
   *
   *3) The  son of "current"  is NULL: "current" was  EQUAL_DEPTH before
   *and becomes BRO_DEEPER now; we have  to update the AVL status of the
   *upper nodes.
   *
   *4) the  brother of "current"  is not NULL: "current"  was SON_DEEPER
   *before and becomes  EQUAL_DEPTH now; there is no  need to update the
   *AVL status of the upper nodes.
   *
   *Summary: in the cases (1) and  (3) the subtree of which "current" is
   *the root  node becomes higher: we  have to update the  AVL status of
   *the upper nodes.
   */
  if (comparison_result<0) {
    ucl_btree_set_dadson(current, L);
    ++(M->size);
    if (BRO(current)) {
      assert(BRO_DEEPER == AVL_STATUS(current));
      AVL_STATUS(current) = EQUAL_DEPTH;
      return true;
    } else {
      assert(EQUAL_DEPTH == AVL_STATUS(current));
      AVL_STATUS(current) = SON_DEEPER;
    }
  } else {
    assert(comparison_result>0);
    ucl_btree_set_dadbro(current, L);
    ++(M->size);
    if (SON(current)) {
      assert(SON_DEEPER == AVL_STATUS(current));
      AVL_STATUS(current) = EQUAL_DEPTH;
      return true;
    } else {
      assert(EQUAL_DEPTH == AVL_STATUS(current));
      AVL_STATUS(current) = BRO_DEEPER;
    }
  }
  /* Now we have to step up  the tree and update the "avl_status" of the
     parent  nodes,  doing rotations  when  required  to  keep the  tree
     balanced.

     We have to  do it while the subtree we come  from has gotten higher
     with the  new insertion: if a  subtree has not  changed its height,
     nothing changes from  the point of view of its  the parent node, so
     no "avl_status" updates and rotations are required.

     *** NOTE ***

     Readjusting  the balancing  after  an insertion  is DIFFERENT  from
     readjusting after a removal.  We CANNOT take the following loop and
     factor  it   out  joining   it  with  the   loop  at  the   end  of
     "ucl_map_remove()". */
  for (ucl_map_link_t dad = DAD_OF(current);; current = dad, dad = DAD_OF(current)) {
    if (! dad)
      return true; /* "current" is the root */
    else if (SON_OF(dad) == current) { /* stepping up from the son subtree */
      switch (AVL_STATUS(dad)) {
      case BRO_DEEPER:
	AVL_STATUS(dad) = EQUAL_DEPTH;
	return true;
      case EQUAL_DEPTH:
	AVL_STATUS(dad) = SON_DEEPER;
	continue;
      case SON_DEEPER:
	{
	  int	dad_is_root = (dad == M->root);
	  if (SON_DEEPER == AVL_STATUS(current))
	    dad = rot_left(dad);
	  else
	    dad = rot_left_right(dad);
	  if (dad_is_root) M->root = dad;
	  return true;
	}
      }
    } else { /* stepping up from the bro subtree */
      switch (AVL_STATUS(dad)) {
      case SON_DEEPER:
	AVL_STATUS(dad) = EQUAL_DEPTH;
	return true;
      case EQUAL_DEPTH:
	AVL_STATUS(dad) = BRO_DEEPER;
	continue;
      case BRO_DEEPER:
	{
	  int	dad_is_root = (dad == M->root);
	  if (BRO_DEEPER == AVL_STATUS(current))
	    dad = rot_right(dad);
	  else
	    dad = rot_right_left(dad);
	  if (dad_is_root) M->root = dad;
	  return true;
	}
      }
    }
  } /* end of for() */
}

static ucl_map_link_t
rot_left (ucl_map_link_t old_cur)
/* Perform a  left rotation  which balances a  left-left-higher subtree.
 * Example:
 *
 *               (top)                      (top)
 *                 |                          |
 *                11 (old_cur)                9 (new_cur)
 *                / \                    -----+-----
 *     (new_cur) 9   12                 7           11 (old_cur)
 *              / \             =>     / \         /  \
 *             7   10 (bro)           6   8      10   12
 *            / \                              (bro)
 *           6   8
 *
 */
{
  ucl_map_link_t	new_cur, bro;
  new_cur	= SON_OF(old_cur);
  bro		= BRO_OF(new_cur);
  assert(SON_DEEPER == AVL_STATUS(old_cur));
  assert(SON_DEEPER == AVL_STATUS(new_cur));
  { /* relink "old_cur" and "new_cur" */
    BRO(new_cur)	= old_cur;
    DAD(new_cur)	= DAD_OF(old_cur);
    DAD(old_cur)	= new_cur;
  }
  { /* relink "old_cur" and "bro" */
    SON(old_cur)	= bro;
    if (bro) DAD(bro)	= old_cur;
  }
  { /* relink "top" */
    ucl_map_link_t	top = DAD_OF(new_cur);
    if (top) {
      if (old_cur == BRO_OF(top))
	BRO(top) = new_cur;
      else {
	assert(old_cur == SON_OF(top));
	SON(top) = new_cur;
      }
    }
  }
  AVL_STATUS(old_cur) = EQUAL_DEPTH;
  AVL_STATUS(new_cur) = EQUAL_DEPTH;
  return new_cur;
}

static ucl_map_link_t
rot_right (ucl_map_link_t old_cur)
/* Perform a right rotation which balances a right-right-higher subtree;
 * Example:
 *
 *         (top)                              (top)
 *           |                                  |
 *           7 (old_cur)                        9 (new_cur)
 *          / \                            -----+-----
 *         6   9 (new_cur)   => (old_cur) 7           11
 *            / \                        / \         /  \
 *     (son) 8   11                     6   8      10    12
 *              /  \                      (son)
 *            10    12
 */
{
  ucl_map_link_t	new_cur, son;
  new_cur	= BRO_OF(old_cur);
  son		= SON_OF(new_cur);
  assert(BRO_DEEPER == AVL_STATUS(old_cur));
  assert(BRO_DEEPER == AVL_STATUS(new_cur));
  { /* relink "old_cur" and "new_cur" */
    SON(new_cur)	= old_cur;
    DAD(new_cur)	= DAD_OF(old_cur);
    DAD(old_cur)	= new_cur;
  }
  { /* relink "old_cur" and "son" */
    BRO(old_cur)	= son;
    if (son) DAD(son)	= old_cur;
  }
  { /* relink "top" */
    ucl_map_link_t	top = DAD_OF(new_cur);
    if (top) {
      if (old_cur == BRO_OF(top))
	BRO(top) = new_cur;
      else {
	assert(old_cur == SON_OF(top));
	SON(top) = new_cur;
      }
    }
  }
  AVL_STATUS(old_cur) = EQUAL_DEPTH;
  AVL_STATUS(new_cur) = EQUAL_DEPTH;
  return new_cur;
}

static ucl_map_link_t
rot_left_right (ucl_map_link_t old_cur)
/* Perform  a  left/right rotation  which  balances a  left-right-higher
 * subtree; "cur" is a pointer  to the variable holding the current node
 * pointer.  Example:
 *
 *             (top)                             (top)
 *               |                                 |
 *              10 (old_cur)                       8 (new_cur)
 *             /  \                      ----------+----------
 *      (son) 5    13             (son) 5                    10 (old_cur)
 *           / \               =>      / \                  /  \
 *          3   8 (new_cur)           3   7                9    13
 *             / \                      (deep_son)  (deep_bro)
 * (deep_son) 7   9 (deep_bro)
 *
 *We  perform this  rotation to  balance a  tree in  which  "old_cur" is
 *son-deeper and "son" is bro-deeper.
 *
 */
{
  ucl_map_link_t	new_cur, son, deep_bro, deep_son;
  son		= SON_OF(old_cur);
  new_cur	= BRO_OF(son);
  assert(SON_DEEPER == AVL_STATUS(old_cur));
  assert(BRO_DEEPER == AVL_STATUS(son) || EQUAL_DEPTH == AVL_STATUS(son));
  { /* relink "son" and "deep_son" */
    deep_son = SON_OF(new_cur);
    BRO(son)	= deep_son;
    if (deep_son) DAD(deep_son) = son;
  }
  { /* relink "new_cur" and "son" */
    SON(new_cur)= son;
    DAD(son)	= new_cur;
  }
  { /* relink "old_cur" and "deep_bro" */
    deep_bro = BRO_OF(new_cur);
    SON(old_cur) = deep_bro;
    if (deep_bro) DAD(deep_bro) = old_cur;
  }
  { /* finish relinking "old_cur" and "new_cur" */
    BRO(new_cur)	= old_cur;
    DAD(new_cur)	= DAD_OF(old_cur);
    DAD(old_cur)	= new_cur;
  }
  { /* relink "top" */
    ucl_map_link_t	top = DAD_OF(new_cur);
    if (top) {
      if (old_cur == BRO_OF(top))
	BRO(top) = new_cur;
      else {
	assert(old_cur == SON_OF(top));
	SON(top) = new_cur;
      }
    }
  }
  /* Adjust the statuses. */
  if (deep_son) {
    AVL_STATUS(son) = SON(son)? EQUAL_DEPTH : BRO_DEEPER;
  } else {
    AVL_STATUS(son) = SON(son)? SON_DEEPER  : EQUAL_DEPTH;
  }
  if (deep_bro) {
    AVL_STATUS(old_cur) = BRO(old_cur)? EQUAL_DEPTH : SON_DEEPER;
  } else {
    AVL_STATUS(old_cur) = BRO(old_cur)? BRO_DEEPER  : EQUAL_DEPTH;
  }
  AVL_STATUS(new_cur) = EQUAL_DEPTH;
  return new_cur;
}

static ucl_map_link_t
rot_right_left (ucl_map_link_t old_cur)
/* Perform a  bro/son rotation which balances  a bro-son-higher subtree.
 * Example:
 *
 *             (top)                                (top)
 *               |                                    |
 *     (old_cur) 9                                   11 (new_cur)
 *              / \                         ----------+---------
 *             8   13 (bro)      (old_cur) 9                    13 (bro)
 *                /  \         =>         / \                  /  \
 *    (new_cur) 11    14                 8   10              12    14
 *             /  \                        (deep_son)  (deep_bro)
 *           10    12
 *   (deep_son)    (deep_bro)
 *
 *We  perform this  rotation to  balance a  tree in  which  "old_cur" is
 *bro-deeper and  "bro" is son-deeper.
 */
{
  ucl_map_link_t	new_cur, bro, deep_bro, deep_son;
  bro		= BRO_OF(old_cur);
  new_cur	= SON_OF(bro);
  assert(BRO_DEEPER == AVL_STATUS(old_cur));
  assert(SON_DEEPER == AVL_STATUS(bro));
  { /* relink "bro" and "deep_bro" */
    deep_bro = BRO_OF(new_cur);
    SON(bro)	= deep_bro;
    if (deep_bro) DAD(deep_bro) = bro;
  }
  { /* relink "new_cur" and "bro" */
    BRO(new_cur)= bro;
    DAD(bro)	= new_cur;
  }
  { /* relink "old_cur" and "deep_son" */
    deep_son = SON_OF(new_cur);
    BRO(old_cur) = deep_son;
    if (deep_son) DAD(deep_son) = old_cur;
  }
  { /* finish relinking "old_cur" and "new_cur" */
    SON(new_cur)	= old_cur;
    DAD(new_cur)	= DAD_OF(old_cur);
    DAD(old_cur)	= new_cur;
  }
  { /* relink "top" */
    ucl_map_link_t	top = DAD_OF(new_cur);
    if (top) {
      if (old_cur == BRO_OF(top))
	BRO(top) = new_cur;
      else {
	assert(old_cur == SON_OF(top));
	SON(top) = new_cur;
      }
    }
  }
  /* Adjust the statuses. */
  if (deep_son) {
    AVL_STATUS(old_cur) = SON(old_cur)? EQUAL_DEPTH : BRO_DEEPER;
  } else {
    AVL_STATUS(old_cur) = SON(old_cur)? SON_DEEPER  : EQUAL_DEPTH;
  }
  if (deep_bro) {
    AVL_STATUS(bro)	= BRO(bro)? EQUAL_DEPTH : SON_DEEPER;
  } else {
    AVL_STATUS(bro)	= BRO(bro)? BRO_DEEPER  : EQUAL_DEPTH;
  }
  AVL_STATUS(new_cur) = EQUAL_DEPTH;
  return new_cur;
}

ucl_map_link_t
ucl_map_remove (ucl_map_t M, ucl_map_link_t cur)
/* Remove  a  link.   If the  selected  link is  not  in  the tree,  the
   behaviour of this function is not defined.  This is not so difficult:

	(1) in the subtree of the target: find an element to replace the
	    erased one,  it's the max in  the son subtree or  the min in
	    the bro subtree;

	(2)  copy the key  and elm  from the  replacement node  into the
            target  node; now  the replacement  node is  the  new target
            node, with key and value already "destroyed";

        (3) if the target node got NULL son and bro it's over, else loop
            to step (2).

   Now from the parent of the  target node, step up in the tree updating
   the AVL status indicator and doing rotations if required.
*/
{
  ucl_map_link_t	removed = cur;
  debug("enter, removing link %p from map with %u nodes", (void *)cur, ucl_map_size(M));
  if (1 == M->size) {
    assert(cur == M->root);
    M->root = NULL;
    M->size = 0;
    return cur;
  } else {
    ucl_map_link_t 	dad, son, bro, L;
    while (SON_OF(cur) || BRO_OF(cur)) {
      son = SON_OF(cur);
      bro = BRO_OF(cur);
      L = son? ucl_btree_find_rightmost(son) : ucl_btree_find_leftmost(bro);
      ucl_btree_swap(cur, L);
    }
    /* Assert that "cur" is a leaf. */
    assert(! SON_OF(cur));
    assert(! BRO_OF(cur));
    /* Remove "cur" from the tree and adjust the status of its dad. */
    dad	= DAD_OF(cur);
    --(M->size);
    debug("dad of node to remove after pushing down: %p, son=%p, bro=%p, status %s",
	  (void *)dad, SON_OF(dad), BRO_OF(dad),
	  debug_avl_status(AVL_STATUS(dad)));
    if (cur == SON_OF(dad)) {
      SON(dad) = NULL;
      if (BRO_OF(dad)) {
	/* Before  the removal  the dad  was EQUAL_DEPTH,  now  the right
	   subtree is  higher because we've removed  the left subtree.
	   The subtree has unchanged depth: return directly. */
	assert(EQUAL_DEPTH == AVL_STATUS(dad));
	AVL_STATUS(dad) = BRO_DEEPER;
	return cur;
      } else {
	/* Before  the  removal  the  dad  was  SON_DEEPER,  now  it's
	   EQUAL_DEPTH  because  we've  removed  the left  subtree.   The
	   subtree has gotten shorter: we have to update the status of
	   the upper nodes. */
	assert(SON_DEEPER == AVL_STATUS(dad));
	AVL_STATUS(dad) = EQUAL_DEPTH;
      }
    } else {
      /* assert(!dad || cur == BRO_OF(dad)); */
      BRO(dad) = NULL;
      if (SON_OF(dad)) {
	/* Before  the  removal the  dat  was  EQUAL_DEPTH,  now the  son
	   subtree is higher because  we've removed the right subtree.
	   The subtree has unchanged depth: return directly. */
	assert(EQUAL_DEPTH == AVL_STATUS(dad));
	AVL_STATUS(dad) = SON_DEEPER;
	return cur;
      } else {
	/* Before  the  removal  the  dad  was  BRO_DEEPER,  now  it's
	   EQUAL_DEPTH  because  we've removed  the  right subtree.   The
	   subtree has gotten shorter: we have to update the status of
	   the upper nodes. */
	/* assert(BRO_DEEPER == AVL_STATUS(dad)); */
	AVL_STATUS(dad) = EQUAL_DEPTH;
      }
    }
    /* Now we  have to step up  the tree and update  the "avl_status" of
       the parent nodes, doing rotations  when required to keep the tree
       balanced.

       We have to do it while the subtree we come from has gotten higher
       with the new insertion: if  a subtree has not changed its height,
       nothing changes from the point of view of its the parent node, so
       no "avl_status" updates and rotations are required.

       *** NOTE ***

       Readjusting  the  balancing after  a  removal  is DIFFERENT  from
       readjusting  after an  insertion.  We  CANNOT take  the following
       loop and  factor it out  joining it with  the loop at the  end of
       "ucl_map_insert()". */
    for (cur=dad, dad=DAD_OF(cur);; cur=dad, dad=DAD_OF(cur)) {
      if (!dad) {
	M->root = cur;
	break;
      }
      debug("balancing %p from status %s", (void *)dad, debug_avl_status(AVL_STATUS(dad)));
      if (SON_OF(dad) == cur) {
	/* We have removed a link in the son subtree. */
	debug("we have removed a link in the son subtree");
	switch (AVL_STATUS(dad)) {
	case SON_DEEPER:
	  AVL_STATUS(dad) = EQUAL_DEPTH;
	  break;
	case EQUAL_DEPTH:
	  if (AVL_STATUS((ucl_map_link_t)BRO_OF(dad)) == BRO_DEEPER) {
	    debug("rot left");
	    dad = rot_left(dad);
	  } else {
	    debug("rot left-right");
	    dad = rot_left_right(dad);
	  }
	  AVL_STATUS(dad) = EQUAL_DEPTH;
	  break;
	case BRO_DEEPER:
	  if (AVL_STATUS((ucl_map_link_t)BRO_OF(dad)) == BRO_DEEPER) {
	    debug("rot right");
	    dad = rot_right(dad);
	  } else {
	    debug("rot right-left");
	    dad = rot_right_left(dad);
	  }
	  AVL_STATUS(dad) = EQUAL_DEPTH;
	}
      } else {
	assert(BRO_OF(dad) == cur);
	/* We have removed a link in the bro subtree. */
	debug("we have removed a link in the bro subtree");
	switch (AVL_STATUS(dad)) {
	case BRO_DEEPER:
	  AVL_STATUS(dad) = EQUAL_DEPTH;
	  break;
	case EQUAL_DEPTH:
	  AVL_STATUS(dad) = SON_DEEPER;
	  break;
	case SON_DEEPER:
	  if (AVL_STATUS((ucl_map_link_t)SON_OF(dad)) == SON_DEEPER)
	    dad = rot_left(dad);
	  else
	    dad = rot_left_right(dad);
	  AVL_STATUS(dad) = EQUAL_DEPTH;
	}
      }
    } /* end of for() loop */
    return removed;
  }












#if 0
  ucl_map_link_t 	del, tmp, link;
  ucl_value_t	tmpkey, tmpval;
  int		root_flag=0;
  /* Save the key and value of  the link to be removed.  We don't remove
     the node referenced  by "cur": instead we search  its subtree for a
     replacement  and store  the key  and  value of  the replacement  in
     "cur".

     Then we do  it again and again until a leaf  node is found: that'll
     be the node actually removed from the tree. */
  tmpkey = ucl_map_getkey(cur);
  tmpval = ucl_map_getval(cur);
  while (cur->node.son || cur->node.bro) {
    if (cur->node.son)
      link = (ucl_map_link_t ) ucl_btree_find_rightmost(cur->node.son);
    else
      link = (ucl_map_link_t ) ucl_btree_find_leftmost(cur->node.bro);
    /* Here "link" can't be NULL.  It's possible that:

       link == cur->node.bro

       or that:

       link == cur->node.son

       but these are not problems. */
    cur->key = link->key;
    cur->val = link->val;
    cur = link;
  }
  /* Here  "cur"  references  the  leaf  node that's  removed  from  the
     tree. We  store the key  and value that  were saved before  in this
     node, and leave it alone until the end of the function: the pointer
     will be the return value. */
  del = cur;
  ucl_map_setkey(del, tmpkey);
  ucl_map_setval(del, tmpval);
  /* Now we have to step up  in the tree, doing "avl_status" updates and
     rotations when needed. */
  link = (ucl_map_link_t ) cur->node.dad;
  if (cur == (ucl_map_link_t ) link->node.son) {
    link->node.son = NULL;
    if (link->node.bro) {
      /* Before this link was EQUAL_DEPTH,  now the right subtree is higher
	 because  we've removed  the left  subtree.  The  subtree hasn't
	 gotten shorter. */
      AVL_STATUS(link) = BRO_DEEPER;
      goto end;
    } else {
      /* Before  this link  was SON_DEEPER,  now it's  EQUAL_DEPTH because
	 we've  removed  the  left  subtree.   The  subtree  has  gotten
	 shorter. */
      AVL_STATUS(link) = EQUAL_DEPTH;
    }
  } else { /* cur == link->node.bro */
    link->node.bro = NULL;
    if (link->node.son) {
      /* Before this link  was EQUAL_DEPTH, now the left  subtree is higher
	 because we've  removed the  right subtree.  The  subtree hasn't
	 gotten shorter. */
      AVL_STATUS(link) = SON_DEEPER;
      goto end;
    } else {
      /* Before this  link was  BRO_DEEPER, now it's  EQUAL_DEPTH because
	 we've  removed  the  right  subtree.  The  subtree  has  gotten
	 shorter. */
      AVL_STATUS(link) = EQUAL_DEPTH;
    }
  }
  for (;;) {
    tmp  = link;
    link = (ucl_map_link_t) tmp->node.dad;
    if (!link) break;
    if (M->root == link)
      root_flag = 1;
    if (tmp == (ucl_map_link_t) link->node.son) {
      if (AVL_STATUS(link) == SON_DEEPER) {
	AVL_STATUS(link) = EQUAL_DEPTH;
	break;
      } else if (AVL_STATUS(link) == EQUAL_DEPTH) {
	AVL_STATUS(link) = BRO_DEEPER;
	break;
      } else { /* AVL_STATUS(link) == BRO_DEEPER */
	tmp = (ucl_map_link_t) link->node.bro;
	if (AVL_STATUS(tmp) == BRO_DEEPER)
	  link = rot_right(link);
	else
	  link = rot_right_left(link);
	AVL_STATUS(link) = EQUAL_DEPTH;
      }
    } else { /* tmp == link->node.bro */
      if (AVL_STATUS(link) == BRO_DEEPER) {
	AVL_STATUS(link) = EQUAL_DEPTH;
	break;
      } else if (AVL_STATUS(link) == EQUAL_DEPTH) {
	AVL_STATUS(link) = SON_DEEPER;
	break;
      } else { /* AVL_STATUS(link) == SON_DEEPER */
	tmp = (ucl_map_link_t ) link->node.son;
	if (AVL_STATUS(tmp) == SON_DEEPER)
	  link = rot_left(link);
	else
	  link = rot_left_right(link);
	AVL_STATUS(link) = EQUAL_DEPTH;
      }
    }
    if (root_flag)
      M->root = link;
  }
 end:
  --(M->size);
  return del;
#endif
}

ucl_map_link_t
ucl_map_find (const ucl_map_t M, const ucl_value_t key)
{
  ucl_map_link_t 	cur = M->root, last;
  int			v;
  while (cur) {
    v = M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(cur));
    if (v > 0)
      cur = (ucl_map_link_t) cur->node.bro;
    else if (v < 0)
      cur = (ucl_map_link_t) cur->node.son;
    else {
      if (M->flags & UCL_ALLOW_MULTIPLE_OBJECTS) {
	do {
	  last = cur;
	  cur = ucl_btree_step_inorder_backward(last);
	} while ((cur != NULL) && (M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(cur)) == 0));
	return last;
      }
      return cur;
    }
  }
  return NULL;
}

/** ------------------------------------------------------------
 ** Traversing.
 ** ----------------------------------------------------------*/

ucl_map_link_t
ucl_map_first (const ucl_map_t M)
{
  return (M->size)? ucl_btree_find_leftmost(M->root) : NULL;
}

ucl_map_link_t
ucl_map_last (const ucl_map_t M)
{
  return (M->size)? ucl_btree_find_rightmost(M->root) : NULL;
}
ucl_map_link_t
ucl_map_next (ucl_map_link_t L)
{
  return ucl_btree_step_inorder(L);
}
ucl_map_link_t
ucl_map_prev (ucl_map_link_t L)
{
  return ucl_btree_step_inorder_backward(L);
}

ucl_map_link_t
ucl_map_find_or_next (const ucl_map_t M, const ucl_value_t key)
{
  ucl_map_link_t 	cur, last;
  int			v;
  assert(M);
  /* Handle the case of empty map. */
  cur = M->root;
  if (cur == NULL)
    return cur;
  /* Dive in the tree to find the key. */
  while (cur) {
    last = cur;
    v = M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(cur));
    if (v > 0)
      cur = (ucl_map_link_t) last->node.bro;
    else if (v < 0)
      cur = (ucl_map_link_t) last->node.son;
    else { /* v == 0 */
      if (M->flags & UCL_ALLOW_MULTIPLE_OBJECTS) {
	do {
	  last = cur;
	  cur  = ucl_btree_step_inorder(last);
	  if (NULL == cur) break;
	  v = M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(cur));
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


ucl_map_link_t
ucl_map_find_or_prev (const ucl_map_t M, const ucl_value_t key)
{
  int			v;
  ucl_map_link_t 	cur, last;
  assert(M != 0);
  /* Handle the case of empty map. */
  cur = M->root;
  if (NULL == cur)
    return cur;
  /* Dive in the tree to find the key. */
  while (cur) {
    last = cur;
    v = M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(cur));
    if (v > 0)
      cur = BRO_OF(last);
    else if (v < 0)
      cur = SON_OF(last);
    else { /* v == 0 */
      if (M->flags & UCL_ALLOW_MULTIPLE_OBJECTS) {
	do {
	  last = cur;
	  cur  = ucl_btree_step_inorder_backward(last);
	  if (NULL == cur) break;
	  v = M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(cur));
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
ucl_map_count (const ucl_map_t M, const ucl_value_t key)
{
  size_t		count;
  ucl_map_link_t 	L;
  assert(M);
  count = 0;
  L  = ucl_map_find(M, key);
  if (L) {
    do {
      ++count;
      L = ucl_map_next(L);
    } while (L && (0 == M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(L))));
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
ucl_map_lower_bound (const ucl_map_t M, ucl_iterator_t I, const ucl_value_t key)
{
  assert(M);
  assert(I);
  I->container = M;
  if (! M->size)
    I->iterator = 0;
  else {
    I->iterator	= ucl_map_find(M, key);
    I->next	= map_lowerbound_iterator_next;
  }
}
void
ucl_map_upper_bound (const ucl_map_t M, ucl_iterator_t I, const ucl_value_t key)
{
  ucl_map_link_t 	L;
  ucl_value_t		key1;
  assert(M);
  assert(I);
  I->container = M;
  if (0 == M->size)
    I->iterator = NULL;
  else {
    L = ucl_map_find_or_next(M, key);
    if (NULL == L) {
      I->iterator = L;
      return;
    } else {
      key1 = ucl_map_getkey(L);
      if (M->keycmp.func(M->keycmp.data, key, key1) == 0) {
	I->iterator	= L;
	I->next		= map_upperbound_iterator_next;
      } else
	I->iterator = NULL;
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
map_upperbound_iterator_next (ucl_iterator_t I)
{
  const ucl_map_tag_t *	M;
  ucl_value_t		key, key1;
  ucl_map_link_t 	L;
  assert(I->iterator);
  M   = I->container;
  L   = I->iterator;
  key = ucl_map_getkey(L);
  L   = ucl_btree_step_inorder_backward(L);
  if (L) {
    key1 = ucl_map_getkey(L);
    I->iterator = (0 == M->keycmp.func(M->keycmp.data, key, key1))? L : NULL;
  } else
    I->iterator = L;
}
static void
map_lowerbound_iterator_next (ucl_iterator_t I)
{
  const ucl_map_tag_t *	M;
  ucl_value_t		key, key1;
  ucl_map_link_t 	L;
  assert(I->iterator);
  M   = I->container;
  L   = I->iterator;
  key = ucl_map_getkey(L);
  L   = ucl_btree_step_inorder(L);
  if (L) {
    key1 = ucl_map_getkey(L);
    I->iterator = (M->keycmp.func(M->keycmp.data, key, key1) == 0)? L : NULL;
  } else
    I->iterator = L;
}

/** ------------------------------------------------------------
 ** Set iterators.
 ** ----------------------------------------------------------*/

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
  ucl_value_t		key1, key2;
  ucl_map_link_t 	node1;
  ucl_map_link_t 	node2;
  ucl_comparison_t	compar;
  int			v;
  if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    compar	= ((const ucl_map_tag_t *) (I1->container))->keycmp;
    node1	= ucl_iterator_ptr(I1);
    node2	= ucl_iterator_ptr(I2);
    key1	= ucl_map_getkey(node1);
    key2	= ucl_map_getkey(node2);
    v = compar.func(compar.data, key1, key2);
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

/* ------------------------------------------------------------ */

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
  ucl_value_t		key1, key2;
  ucl_map_link_t 	node1, node2;
  ucl_comparison_t	compar = ((const ucl_map_tag_t *) (I1->container))->keycmp;
  int			v;
  while (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    node1	= ucl_iterator_ptr(I1);
    node2	= ucl_iterator_ptr(I2);
    key1	= ucl_map_getkey(node1);
    key2	= ucl_map_getkey(node2);
    v = compar.func(compar.data, key1, key2);
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

/* ------------------------------------------------------------ */

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
  ucl_iterator_tag_t * 	I1 = I->internal1.pointer;
  ucl_iterator_tag_t *	I2 = I->internal2.pointer;
  ucl_comparison_t	compar = ((const ucl_map_tag_t *) (I1->container))->keycmp;
  ucl_value_t		key1, key2;
  ucl_map_link_t 	node1, node2;
  int			v;
  if (I->container == I1)
    ucl_iterator_next(I1);
  else if (I->container == I2)
    ucl_iterator_next(I2);
  if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
    node1 = ucl_iterator_ptr(I1);
    node2 = ucl_iterator_ptr(I2);
    key1  = ucl_map_getkey(node1);
    key2  = ucl_map_getkey(node2);
    v = compar.func(compar.data, key1, key2);
    while (0 == v) {
      ucl_iterator_next(I1);
      ucl_iterator_next(I2);
      if ((! ucl_iterator_more(I1)) || (! ucl_iterator_more(I2)))
	break;
      node1 = ucl_iterator_ptr(I1);
      node2 = ucl_iterator_ptr(I2);
      key1  = ucl_map_getkey(node1);
      key2  = ucl_map_getkey(node2);
      v = compar.func(compar.data, key1, key2);
    }
    if (ucl_iterator_more(I1) && ucl_iterator_more(I2)) {
      if (v < 0) {
	I->iterator  = I1->iterator;
	I->container = I1;
	return;
      } else if (v > 0) {
	I->iterator  = I2->iterator;
	I->container = I2;
	return;
      }
    }
  }
  if (ucl_iterator_more(I1)) {
    I->iterator  = I1->iterator;
    I->container = I1;
  } else if (ucl_iterator_more(I2)) {
    I->iterator  = I2->iterator;
    I->container = I2;
  } else
    I->iterator = NULL;
}

/* ------------------------------------------------------------ */

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
  ucl_comparison_t	compar;
  ucl_value_t		key1, key2;
  ucl_map_link_t 	node1, node2;
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
  compar = ((const ucl_map_tag_t *) (I1->container))->keycmp;
  node1 = ucl_iterator_ptr(I1);
  node2 = ucl_iterator_ptr(I2);
  key1  = ucl_map_getkey(node1);
  key2  = ucl_map_getkey(node2);
  v = compar.func(compar.data, key1, key2);
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
    node1 = ucl_iterator_ptr(I1);
    node2 = ucl_iterator_ptr(I2);
    key1  = ucl_map_getkey(node1);
    key2  = ucl_map_getkey(node2);
    v = compar.func(compar.data, key1, key2);
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
