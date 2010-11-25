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

#define LEFT_HIGHER	UCL_LEFT_HIGHER
#define BALANCED	UCL_BALANCED
#define RIGHT_HIGHER	UCL_RIGHT_HIGHER

#define AVL_STATUS(L)	(L)->avl_status

typedef ucl_map_t		map_t;
typedef ucl_map_tag_t		map_struct_t;
typedef ucl_map_link_t		link_t;
typedef ucl_node_tag_t		node_t;
typedef ucl_iterator_t		iterator_t;
typedef ucl_iterator_tag_t	iterator_struct_t;
typedef ucl_value_t		value_t;

static void	map_inorder_iterator_next	(iterator_t iterator);
static void	map_inorder_backward_iterator_next (iterator_t iterator);
static void	map_preorder_iterator_next	(iterator_t iterator);
static void	map_postorder_iterator_next	(iterator_t iterator);
static void	map_levelorder_iterator_next	(iterator_t iterator);
static void	map_upperbound_iterator_next	(iterator_t iterator);
static void	map_lowerbound_iterator_next	(iterator_t iterator);

static void	rot_left			(link_t *cur_pp);
static void	rot_left_right			(link_t *cur_pp);
static void	rot_right			(link_t *cur_pp);
static void	rot_right_left			(link_t *cur_pp);

static void	intersection_iterator_next	(iterator_t iterator);
static void	intersection_find_common	(iterator_t iter1,
						 iterator_t iter2,
						 iterator_t iterator);

static void	union_iterator_next		(iterator_t iterator);
static void	union_find_next			(iterator_t iter1,
						 iterator_t iter2,
						 iterator_t iterator);

static void	complintersect_iterator_next (iterator_t iterator);
static void	subtraction_iterator_next (iterator_t iterator);


/** ------------------------------------------------------------
 ** Construction and destruction.
 ** ----------------------------------------------------------*/

void
ucl_map_constructor (ucl_map_t M, unsigned int flags, ucl_comparison_t keycmp)
{
  assert(M);
  assert(keycmp.func);
  M->root		= NULL;
  M->size		= 0;
  M->keycmp		= keycmp;
  M->flags		= flags;
}


/** ------------------------------------------------------------
 ** Insertion.
 ** ----------------------------------------------------------*/

void
ucl_map_insert (ucl_map_t M, ucl_map_link_t L)
{
  ucl_map_link_t tmp, cur;
  ucl_value_t	 key;
  int		 v, root_flag;
  ucl_bool_t	 allow_multiple_objects;
  assert(M);
  assert(L);
  /* The new link will have AVL status equal to "balanced".  Always. */
  AVL_STATUS(L) = BALANCED;
  L->node.dad   = L->node.bro = L->node.son = NULL;
  /* Handle the case of empty map.
   *
   *      NULL
   *     ------            ^
   *    |      |       dad |
   *    | base | root    -----  bro
   *    |      |------->|link |---->NULL
   *    |struct|         -----
   *    |      |           |
   *     ------            v
   *     NULL
   */
  if (NULL == M->root) {
    assert(0 == M->size);
    M->root = L;
    ++(M->size);
    return;
  }
  /* If the map is not empty, we  have to look for the node that will be
     the parent of the new one.  If  "M" is not a multimap and we find a
     node with key equal to the key of "L": we silently do nothing. */
  tmp = M->root;
  key = ucl_map_getkey(L);
  allow_multiple_objects = M->flags & UCL_ALLOW_MULTIPLE_OBJECTS;
  while (tmp) {
    cur = tmp;
    v = M->keycmp.func(M->keycmp.data, key, ucl_map_getkey(tmp));
    if ((v > 0) || (v==0 && allow_multiple_objects)) {
      tmp = (ucl_map_link_t) cur->node.bro;
    } else if (v < 0) {
      tmp = (ucl_map_link_t) cur->node.son;
    } else { /* v == 0 && !allow_multiple_objects */
      return;
    }
  }
  /*Here "cur" holds a pointer to the parent of the new link.  If "v<0",
   *we  have to  insert  the  new link  as  son of  "cur"  (in the  left
   *subtree), else as brother (in the right subtree).
   *
   *If we  append the  new link to  the left  of "cur", the  link itself
   *becomes the left subtree of "cur".
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
   *1)  the brother  of "cur"  is NULL:  "cur" was  BALANCED  before and
   *becomes LEFT_HIGHER now;
   *
   *2) the brother  of "cur" is not NULL:  "cur" was RIGHT_HIGHER before
   *and becomes BALANCED now.
   *
   *If we  append the new  link to the  right of "cur", the  link itself
   *becomes the right subtree of "cur".
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
   *3) the son  of "cur" is NULL: "cur" was  BALANCED before and becomes
   *RIGHT_HIGHER now;
   *
   *4) the  brother of "cur" is  not NULL: "cur"  was LEFT_HIGHER before
   *and becomes BALANCED now.
   *
   *In the cases (1) and (3) the subtree of which "cur" is the root node
   *becomes higher:  we'll have  to report the  change to the  parent of
   *"cur".
   */
  if (v<0) {
    /* ucl_btree_set_dadson(cur, L); */
    cur->node.son = (ucl_node_t) L;
    L->node.dad   = (ucl_node_t) cur;
    ++(M->size);
    if (cur->node.bro) {
      assert(RIGHT_HIGHER == AVL_STATUS(cur));
      AVL_STATUS(cur) = BALANCED;
      return;
    } else {
      assert(BALANCED == AVL_STATUS(cur));
      AVL_STATUS(cur) = LEFT_HIGHER;
    }
  } else {
    assert(v>0);
    /* ucl_btree_set_dadbro(cur, L); */
    cur->node.bro = (ucl_node_t) L;
    L->node.dad   = (ucl_node_t) cur;
    ++(M->size);
    if (cur->node.son) {
      assert(LEFT_HIGHER == AVL_STATUS(cur));
      AVL_STATUS(cur) = BALANCED;
      return;
    } else {
      assert(BALANCED == AVL_STATUS(cur));
      AVL_STATUS(cur) = RIGHT_HIGHER;
    }
  }
  /* Now we have to step up  the tree and update the "avl_status" of the
     parent  nodes,  doing rotations  when  required  to  keep the  tree
     balanced.

     We have to  do it until the subtree we come  from has gotten higher
     with the  new insertion: if a  subtree has not  changed its height,
     nothing changes from  the point of view of its  the parent node, so
     no "avl_status" updates and rotations are required. */
  for (;;) {
    tmp = cur;
    cur = (link_t ) tmp->node.dad;
    if (NULL == cur) return;
    /* If we  are stepping  up from  a left subtree  set "v"  to "true",
       otherwise set it to "false". */
    v = cur->node.son == (ucl_node_t) tmp;
    if (v) { /* stepping up from a left subtree */
      if (RIGHT_HIGHER == AVL_STATUS(cur)) {
	AVL_STATUS(cur) = BALANCED;
	return;
      } else if (BALANCED == AVL_STATUS(cur)) {
	AVL_STATUS(cur) = LEFT_HIGHER;
	continue;
      } else { /* AVL_STATUS(cur) == LEFT_HIGHER */
	root_flag = (cur == M->root)? 1 : 0;
	if (LEFT_HIGHER == AVL_STATUS(tmp)) {
	  rot_left(&cur);
	} else {
	  rot_left_right(&cur);
	}
	AVL_STATUS(cur) = BALANCED;
	if (root_flag) {
	  M->root = cur;
	}
	return;
      }
    } else { /* v == 0 -> stepping up from a right subtree */
      if (LEFT_HIGHER == AVL_STATUS(cur)) {
	AVL_STATUS(cur) = BALANCED;
	return;
      } else if (BALANCED == AVL_STATUS(cur)) {
	AVL_STATUS(cur) = RIGHT_HIGHER;
	continue;
      } else { /* AVL_STATUS(cur) == RIGHT_HIGHER */
	root_flag = cur == M->root;
	if (RIGHT_HIGHER == AVL_STATUS(tmp)) {
	  rot_right(&cur);
	} else {
	  rot_right_left(&cur);
	}
	AVL_STATUS(cur) = BALANCED;
	if (root_flag) {
	  M->root = cur;
	}
	return;
      }
    }
  }
}

static void
rot_left (link_t * cur_p)
/* Perform a  left rotation  which balances a  left-left-higher subtree;
 * "cur_p"  is  a pointer  to  the  variable  holding the  current  node
 * pointer.  Example:
 *
 *                    9                 9
 *                   / \               / \
 *            (cur) 8   10      (cur) 7   10
 *                 /         =>      /     \
 *          (son) 7                 6       8
 *               /
 *              6
 *
 */
{
  ucl_map_link_t cur, son, tmp;
  cur = *cur_p;
  son = (ucl_map_link_t) cur->node.son;
  tmp = (ucl_map_link_t) son->node.bro;
  cur->node.son = (ucl_node_t) tmp;
  if (tmp) {
    tmp->node.dad = (ucl_node_t) cur;
  }
  son->node.bro = (ucl_node_t) cur;
  son->node.dad = cur->node.dad;
  cur->node.dad = (ucl_node_t) son;
  /* AVL_STATUS(cur) = 0; */
  AVL_STATUS(cur) = LEFT_HIGHER;
  *cur_p = son;
  tmp = (ucl_map_link_t) son->node.dad;
  if (tmp) {
    if (cur == (ucl_map_link_t ) tmp->node.bro)
      tmp->node.bro = (ucl_node_t) son;
    else
      tmp->node.son = (ucl_node_t) son;
  }
}

static void
rot_left_right (ucl_map_link_t * cur_p)
/* Perform  a  left/right rotation  which  balances a  left-right-higher
 * subtree; "cur" is a pointer  to the variable holding the current node
 * pointer.  Example:
 *
 *                 10 (cur)            8 (cur)
 *                /  \                / \
 *         (son) 5    13             5   10
 *              / \         =>      / \    \
 *             3   8 (bro)         3   7    13
 *                /
 *               7
 */
{
  ucl_map_link_t cur, son, bro, tmp;
  cur = *cur_p;
  son = (ucl_map_link_t) cur->node.son;
  bro = (ucl_map_link_t) son->node.bro;
  tmp = (ucl_map_link_t) bro->node.son;
  son->node.bro = (ucl_node_t) tmp;
  if (tmp) /* useless test? */
    tmp->node.dad = (ucl_node_t) son;
  bro->node.son = (ucl_node_t) son;
  son->node.dad = (ucl_node_t) bro;
  tmp = (ucl_map_link_t) bro->node.bro;
  cur->node.son = (ucl_node_t) tmp;
  if (tmp)
    tmp->node.dad = (ucl_node_t) cur;
  bro->node.bro = (ucl_node_t) cur;
  bro->node.dad = cur->node.dad;
  cur->node.dad = (ucl_node_t) bro;
  AVL_STATUS(cur) = (AVL_STATUS(bro) == LEFT_HIGHER)?  RIGHT_HIGHER : BALANCED;
  AVL_STATUS(son) = (AVL_STATUS(bro) == RIGHT_HIGHER)? LEFT_HIGHER  : BALANCED;
  *cur_p = bro;
  tmp = (ucl_map_link_t ) bro->node.dad;
  if (tmp) {
    if (cur == (ucl_map_link_t ) tmp->node.bro)
      tmp->node.bro = (ucl_node_t) bro;
    else
      tmp->node.son = (ucl_node_t) bro;
  }
}

static void
rot_right (ucl_map_link_t * cur_p)
/* Perform a right rotation which balances a right-right-higher subtree;
 * "cur_p"  is  a pointer  to  the  variable  holding the  current  node
 * pointer.  Example:
 *
 *                   9                     9
 *                  / \                   / \
 *                 5   10 (cur)          5   11 (cur)
 *                       \          =>      /  \
 *                        11 (bro)        10    12
 *                          \
 *                           12
 */
{
  ucl_map_link_t cur, bro, tmp;
  cur = *cur_p;
  bro = (ucl_map_link_t) cur->node.bro;
  tmp = (ucl_map_link_t) bro->node.son;
  cur->node.bro = (ucl_node_t) tmp;
  if (tmp)
    tmp->node.dad = (ucl_node_t) cur;
  bro->node.son = (ucl_node_t) cur;
  bro->node.dad = cur->node.dad;
  cur->node.dad = (ucl_node_t) bro;
  *cur_p = bro;
  tmp = (ucl_map_link_t ) bro->node.dad;
  if (tmp) {
    if (cur == (ucl_map_link_t ) tmp->node.bro)
      tmp->node.bro = (ucl_node_t) bro;
    else
      tmp->node.son = (ucl_node_t) bro;
  }
}

static void
rot_right_left (ucl_map_link_t * cur_p)
/* Perform  a  right/left rotation  which  balances a  right-left-higher
 * subtree; "cur"  is a  pointer to the  variable holding  the current
 * node pointer.  Example:
 *
 *                     10 (cur)           11 (cur)
 *                    /  \               /  \
 *                   5    13           10    13
 *                       /  \    =>   /     /  \
 *                     11    15      5    12    15
 *                       \
 *                        12
 */
{
  ucl_map_link_t cur, son, bro, tmp;
  cur = *cur_p;
  bro = (ucl_map_link_t) cur->node.bro;
  son = (ucl_map_link_t) bro->node.son;
  tmp = (ucl_map_link_t) son->node.bro;
  bro->node.son = (ucl_node_t) tmp;
  if (tmp)
    tmp->node.dad = (ucl_node_t) bro;
  son->node.bro = (ucl_node_t) bro;
  bro->node.dad = (ucl_node_t) son;
  tmp = (ucl_map_link_t ) son->node.son;
  cur->node.bro = (ucl_node_t) tmp;
  if (tmp)
    tmp->node.dad = (ucl_node_t) cur;
  son->node.son = (ucl_node_t) cur;
  son->node.dad = cur->node.dad;
  cur->node.dad = (ucl_node_t) son;
  /* We have to  update the parent of  cur to point to son,  we'll do it
     later. */
  AVL_STATUS(cur) = (AVL_STATUS(son) == RIGHT_HIGHER)? LEFT_HIGHER : BALANCED;
  AVL_STATUS(bro) = (AVL_STATUS(son) == LEFT_HIGHER)? RIGHT_HIGHER : BALANCED;
  *cur_p = son;
  tmp = (ucl_map_link_t ) son->node.dad;
  if (tmp) {
    if (cur == (ucl_map_link_t ) tmp->node.bro)
      tmp->node.bro = (ucl_node_t) son;
    else
      tmp->node.son = (ucl_node_t) son;
  }
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
  ucl_map_link_t 	del, tmp, link;
  ucl_value_t		tmpkey, tmpval;
  int			root_flag=0;
  if (1 == M->size) {
    assert(cur == M->root);
    M->root = NULL;
    M->size = 0;
    return cur;
  }
  /* Save the key and value of  the link to be removed.  We don't remove
     the node referenced  by "cur": instead we search  its subtree for a
     replacement  and store  the key  and  value of  the replacement  in
     "cur".

     Then we do  it again and again until a leaf  node is found: that'll
     be the node actually removed from the tree. */
  tmpkey = ucl_map_getkey(cur);
  tmpval = ucl_map_getval(cur);
  del = cur;
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
      /* Before this link was BALANCED,  now the right subtree is higher
	 because  we've removed  the left  subtree.  The  subtree hasn't
	 gotten shorter. */
      AVL_STATUS(link) = RIGHT_HIGHER;
      goto end;
    } else {
      /* Before  this link  was LEFT_HIGHER,  now it's  BALANCED because
	 we've  removed  the  left  subtree.   The  subtree  has  gotten
	 shorter. */
      AVL_STATUS(link) = BALANCED;
    }
  } else { /* cur == link->node.bro */
    link->node.bro = NULL;
    if (link->node.son) {
      /* Before this link  was BALANCED, now the left  subtree is higher
	 because we've  removed the  right subtree.  The  subtree hasn't
	 gotten shorter. */
      AVL_STATUS(link) = LEFT_HIGHER;
      goto end;
    } else {
      /* Before this  link was  RIGHT_HIGHER, now it's  BALANCED because
	 we've  removed  the  right  subtree.  The  subtree  has  gotten
	 shorter. */
      AVL_STATUS(link) = BALANCED;
    }
  }
  for (;;) {
    tmp  = link;
    link = (ucl_map_link_t) tmp->node.dad;
    if (!link) break;
    if (M->root == link)
      root_flag = 1;
    if (tmp == (ucl_map_link_t) link->node.son) {
      if (AVL_STATUS(link) == LEFT_HIGHER) {
	AVL_STATUS(link) = BALANCED;
	break;
      } else if (AVL_STATUS(link) == BALANCED) {
	AVL_STATUS(link) = RIGHT_HIGHER;
	break;
      } else { /* AVL_STATUS(link) == RIGHT_HIGHER */
	tmp = (ucl_map_link_t) link->node.bro;
	if (AVL_STATUS(tmp) == RIGHT_HIGHER)
	  rot_right(&link);
	else
	  rot_right_left(&link);
	AVL_STATUS(link) = BALANCED;
      }
    } else { /* tmp == link->node.bro */
      if (AVL_STATUS(link) == RIGHT_HIGHER) {
	AVL_STATUS(link) = BALANCED;
	break;
      } else if (AVL_STATUS(link) == BALANCED) {
	AVL_STATUS(link) = LEFT_HIGHER;
	break;
      } else { /* AVL_STATUS(link) == LEFT_HIGHER */
	tmp = (ucl_map_link_t ) link->node.son;
	if (AVL_STATUS(tmp) == LEFT_HIGHER)
	  rot_left(&link);
	else
	  rot_left_right(&link);
	AVL_STATUS(link) = BALANCED;
      }
    }
    if (root_flag)
      M->root = link;
  }
 end:
  --(M->size);
  return del;
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
  link_t 		cur;
  link_t 		last;
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
      cur = (link_t ) last->node.bro;
    else if (v < 0)
      cur = (link_t ) last->node.son;
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
  const map_struct_t *	M;
  value_t		key, key1;
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
  const map_struct_t *	M;
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
  iterator_struct_t * 	I1;
  iterator_struct_t *	I2;
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
    compar	= ((const map_struct_t *) (I1->container))->keycmp;
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
  iterator_struct_t * 	I1 = I->internal1.pointer;;
  iterator_struct_t * 	I2 = I->internal2.pointer;;
  ucl_iterator_next(I1);
  ucl_iterator_next(I2);
  intersection_find_common(I1, I2, I);
}
static void
intersection_find_common (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I)
{
  ucl_value_t		key1, key2;
  ucl_map_link_t 	node1, node2;
  ucl_comparison_t	compar = ((const map_struct_t *) (I1->container))->keycmp;
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
complintersect_iterator_next (iterator_t I)
{
  iterator_struct_t * 	I1 = I->internal1.pointer;
  iterator_struct_t *	I2 = I->internal2.pointer;
  ucl_comparison_t	compar = ((const map_struct_t *) (I1->container))->keycmp;
  value_t		key1, key2;
  link_t 		node1;
  link_t 		node2;
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
  iterator_struct_t * 	I1;
  iterator_struct_t *	I2;
  ucl_comparison_t	compar;
  value_t		key1, key2;
  link_t 		node1;
  link_t 		node2;
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
  compar = ((const map_struct_t *) (I1->container))->keycmp;
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
