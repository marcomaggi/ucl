/* 
   Part of: Useless Container Library (UCL).
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

   Copyright (c) 2001, 2002, 2003, 2004, 2005, 2007, 2008 Marco Maggi

   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.
   
   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/

/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include "internal.h"

#define stubmodule		map

#if (DEBUGGING == 1)
#  include <stdio.h>
#endif

/* Values   of   the   "avl_status"   field  in   the   "ucl_map_link_t"
   structure. */
#define LEFT_HIGHER		0x00
#define BALANCED		0x01
#define RIGHT_HIGHER		0x02

/* These will make the code easier to read. */
typedef ucl_map_t		map_t;
typedef ucl_map_struct_t	map_struct_t;
typedef ucl_map_link_t		link_t;
typedef ucl_btree_node_t	node_t;
typedef ucl_iterator_t		iterator_t;
typedef ucl_iterator_struct_t	iterator_struct_t;
typedef ucl_value_t		value_t;

/* Static function prototypes. */

static void	map_inorder_iterator_next	(iterator_t iterator);
static void	map_inorder_backward_iterator_next (iterator_t iterator);
static void	map_preorder_iterator_next	(iterator_t iterator);
static void	map_postorder_iterator_next	(iterator_t iterator);
static void	map_levelorder_iterator_next	(iterator_t iterator);
static void	map_upperbound_iterator_next	(iterator_t iterator);
static void	map_lowerbound_iterator_next	(iterator_t iterator);

static void	rot_left			(link_t **cur_pp);
static void	rot_left_right			(link_t **cur_pp);
static void	rot_right			(link_t **cur_pp);
static void	rot_right_left			(link_t **cur_pp);

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

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Construction and destruction.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-11-41) void
ucl_map_constructor (ucl_map_t this, unsigned int flags, ucl_comparison_t keycmp)
{
  assert(this); assert(keycmp.func);

  this->root		= 0;
  this->size		= 0;
  this->keycmp		= keycmp;
  this->flags		= flags;
}
stub(2007-05-16-10-28-48) void
ucl_map_destructor (ucl_map_t this)
{
  ucl_struct_clean((this), ucl_map_t);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Insertion.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-11-44) void
ucl_map_insert (ucl_map_t this, ucl_map_link_t *link_p)
{
  ucl_comparison_t	keycmp;
  int		v, root_flag;
  link_t *	tmp_p;
  link_t *	cur_p;
  value_t	key;

  /*
    A flag  used in the loop below,  when stepping up in  the tree doing
    rotation and  "avl_status" updates: true  if the subtree  from which
    we're coming has gotten higher, otherwise false.

    Mr. Niklaus Wirth calls it "h" in his book.
    
    int		h;
  */


  assert(this != NULL);
  assert(link_p != NULL);


  /*
    The new link will have AVL status equal to "balanced". Always.
  */

  link_p->avl_status = BALANCED;
  link_p->node.dad_p = link_p->node.bro_p = link_p->node.son_p = NULL;


  /*
    Handle the case of empty map.


                         NULL
         ------            ^
        |      |       dad |
        | base | root    -----  bro
        |      |------->|link |---->NULL
        |struct|         -----
        |      |           |
         ------            v
                         NULL
  */

  if (this->root == NULL)
    {
      assert(this->size == 0);

      this->root = link_p;
      ++(this->size);
      return;
    }


  /*
    If the map is  not empty, we have to look for  the node that will be
    the parent of the new one.

    If looking for this node we find a node with key equal to the key of
    the new link, and this is not a multimap, we silently do nothing.
  */

  tmp_p = this->root;
  keycmp = this->keycmp;
  key    = ucl_map_getkey(link_p);

  while (tmp_p != NULL)
    {
      cur_p = tmp_p;

      v = keycmp.func(keycmp.data, key, ucl_map_getkey(tmp_p));
      if ((v > 0)
	  || ((v == 0) && (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)))
	{
	  tmp_p = (link_t *) cur_p->node.bro_p;
	}
      else if (v < 0)
	{
	  tmp_p = (link_t *) cur_p->node.son_p;
	}
      else /* v == 0 && !(this->flag & UCL_ALLOW_MULTIPLE_OBJECTS) */
	{
	  return;
	}
    }

  /*
    Here "cur_p"  holds a  pointer to  the parent of  the new  link. If
    "v<0", we have  to insert the new link as son  of "cur_p" (that is:
    in  the  left subtree),  else  as brother  (that  is:  in the  right
    subtree).


    If we append  the new link to the left of  "cur_p", the link itself
    becomes the left subtree of "cur_p".

                    -----
                   | cur |----> ?
                    -----   bro
                      |
                  son v
                    -----
                   |link |
                    -----
 
    There are two cases:
    
    1) the brother  of "*cur_p" is NULL: "*cur_p"  was BALANCED before
       and becomes LEFT_HIGHER now;

    2) the brother of "*cur_p"  is not NULL: "*cur_p" was RIGHT_HIGHER
       before and becomes BALANCED now.


    If we append the new link  to the right of "cur_p", the link itself
    becomes the right subtree of "cur_p".

                    -----       -----
                   | cur |---->|link |
                    -----  bro  -----
                      |
                  son v
                      ?
 
    There are two cases:
    
    3) the  son of "*cur_p" is NULL: "*cur_p" was BALANCED  before and
       becomes RIGHT_HIGHER now;

    4) the  brother of "*cur_p" is not  NULL: "*cur_p" was LEFT_HIGHER
       before and becomes BALANCED now.

    In the cases (1) and (3)  the subtree of which "*cur_p" is the root
    node becomes higher: we'll have  to report this change to the parent
    of "*cur_p", so we set the "h" flag to "true".
  */

  if (v<0)
    {
      cur_p->node.son_p  = (node_t *) link_p;
      link_p->node.dad_p = (node_t *) cur_p;
      ++(this->size);
	      
      if (cur_p->node.bro_p != NULL)
	{
	  cur_p->avl_status = BALANCED;
/*  	  h = 0; */
	  return;
	}
      else
	{
	  cur_p->avl_status = LEFT_HIGHER;
/*  	  h = 1; */
	}
    }
  else
    {
      cur_p->node.bro_p  = (node_t *) link_p;
      link_p->node.dad_p = (node_t *) cur_p;
      ++(this->size);

      if (cur_p->node.son_p != NULL)
	{
	  cur_p->avl_status = BALANCED;
/*  	  h = 0; */
	  return;
	}
      else
	{
	  cur_p->avl_status = RIGHT_HIGHER;
/*  	  h = 1; */
	}
    }


  /*
    Now we have  to step up the tree and update  the "avl_status" of the
    parent  nodes,  doing  rotations  when  required to  keep  the  tree
    balanced.

    We have  to do it until the  subtree we come from  has gotten higher
    with the  new insertion:  if a subtree  has not changed  its height,
    nothing changes from the point of view of its the parent node, so no
    "avl_status" updates and rotations are required.
  */

/*    while (h) */
  for (;;)
    {
      tmp_p = cur_p;
      cur_p = (link_t *) tmp_p->node.dad_p;
      if (cur_p == NULL) { return; }

      /*
	If we  are stepping up  from a left  subtree set "v"  to "true",
	otherwise set it to "false".
      */

      v = (cur_p->node.son_p == (node_t *) tmp_p)? 1 : 0;

      if (v) /* stepping up from a left subtree */
	{
	  if (cur_p->avl_status == RIGHT_HIGHER)
	    {
	      cur_p->avl_status = BALANCED;
/*  	      h = 0; */
	      return;
	    }
	  else if (cur_p->avl_status == BALANCED)
	    {
	      cur_p->avl_status = LEFT_HIGHER;
/*  	      h = 1; */
	      continue;
	    }
	  else /* cur_p->avl_status == LEFT_HIGHER */
	    {
	      root_flag = (cur_p == this->root)? 1 : 0;
		      
	      if (tmp_p->avl_status == LEFT_HIGHER)
		{
		  rot_left(&cur_p);
		}
	      else
		{
		  rot_left_right(&cur_p);
		}
	      cur_p->avl_status = BALANCED;

	      if (root_flag)
		{
		  this->root = cur_p;
		}
/*  	      h = 0; */
	      return;
	    }
	}
      else /* v == 0 -> stepping up from a right subtree */
	{
	  if (cur_p->avl_status == LEFT_HIGHER)
	    {
	      cur_p->avl_status = BALANCED;
/*  	      h = 0; */
	      return;
	    }
	  else if (cur_p->avl_status == BALANCED)
	    {
	      cur_p->avl_status = RIGHT_HIGHER;
/*  	      h = 1; */
	      continue;
	    }
	  else /* cur_p->avl_status == RIGHT_HIGHER */
	    {
	      root_flag = (cur_p == this->root)? 1 : 0;

	      if (tmp_p->avl_status == RIGHT_HIGHER)
		{
		  rot_right(&cur_p);
		}
	      else
		{
		  rot_right_left(&cur_p);
		}
	      cur_p->avl_status = BALANCED;
	      
	      if (root_flag)
		{
		  this->root = cur_p;
		}
/*  	      h = 0; */
	      return;
	    }
	}
    }
}

/* rot_left --

	Performs a left rotation.

	  Example:

                    9                 9
                   / \               / \
            (cur) 8   10            7   10
                 /         =>      / \
          (son) 7                 6   8
               /
              6



   Arguments:

	*cur_pp -	pointer to the variable holding the current
			node pointer

*/

static void
rot_left (link_t **cur_pp)
{
  link_t *	cur_p;
  link_t *	son_p;
  link_t *	tmp_p;


  cur_p = *cur_pp;
  son_p = (link_t *) cur_p->node.son_p;

  tmp_p = (link_t *) son_p->node.bro_p;
  cur_p->node.son_p = (node_t *) tmp_p;
  if (tmp_p)
    {
      tmp_p->node.dad_p = (node_t *) cur_p;
    }
  
  son_p->node.bro_p = (node_t *) cur_p;
  son_p->node.dad_p = cur_p->node.dad_p;
  cur_p->node.dad_p = (node_t *) son_p;

  cur_p->avl_status = 0;

  *cur_pp = son_p;
  tmp_p = (link_t *) son_p->node.dad_p;
  if (tmp_p)
    {
      if (cur_p == (link_t *) tmp_p->node.bro_p)
	{
	  tmp_p->node.bro_p = (node_t *) son_p;
	}
      else
	{
	  tmp_p->node.son_p = (node_t *) son_p;
	}
    }
}

/* ------------------------------------------------------------ */


/* rot_left_right --

	Performs a left/right rotation.

          Example:

                 10 (cur)            8
                /  \                / \
         (son) 5    13             5   10
              / \         =>      / \    \
             3   8 (bro)         3   7    13
                /
               7

*/

static void
rot_left_right (link_t **cur_pp)
{
  link_t *	cur_p;
  link_t *	son_p;
  link_t *	bro_p;
  link_t *	tmp_p;


  cur_p = *cur_pp;
  son_p  = (link_t *) cur_p->node.son_p;
  bro_p  = (link_t *) son_p->node.bro_p;

  tmp_p = (link_t *) bro_p->node.son_p;
  son_p->node.bro_p = (node_t *) tmp_p;
  if (tmp_p) /* useless test? */
    {
      tmp_p->node.dad_p = (node_t *) son_p;
    }

  bro_p->node.son_p = (node_t *) son_p;
  son_p->node.dad_p = (node_t *) bro_p;

  tmp_p = (link_t *) bro_p->node.bro_p;
  cur_p->node.son_p = (node_t *) tmp_p;
  if (tmp_p)
    {
      tmp_p->node.dad_p = (node_t *) cur_p;
    }

  bro_p->node.bro_p = (node_t *) cur_p;
  bro_p->node.dad_p = cur_p->node.dad_p;
  cur_p->node.dad_p = (node_t *) bro_p;

  cur_p->avl_status = \
    (bro_p->avl_status == LEFT_HIGHER)? RIGHT_HIGHER : BALANCED;

  son_p->avl_status = \
    (bro_p->avl_status == RIGHT_HIGHER)? LEFT_HIGHER : BALANCED;

  *cur_pp = bro_p;
  tmp_p = (link_t *) bro_p->node.dad_p;
  if (tmp_p)
    {
      if (cur_p == (link_t *) tmp_p->node.bro_p)
	{
	  tmp_p->node.bro_p = (node_t *) bro_p;
	}
      else
	{
	  tmp_p->node.son_p = (node_t *) bro_p;
	}
    }
}

/* ------------------------------------------------------------ */


/* rot_right --

	Performs a right rotation.

	  Example:

                   9                     9
                  / \                   / \
                 5   10 (cur)          5   11
                       \          =>      /  \
                        11 (bro)        10    12
                          \
                           12

*/

static void
rot_right (link_t **cur_pp)
{
  link_t *	cur_p;
  link_t *	bro_p;
  link_t *	tmp_p;


  cur_p = *cur_pp;
  bro_p  = (link_t *) cur_p->node.bro_p;

  tmp_p = (link_t *) bro_p->node.son_p;
  cur_p->node.bro_p = (node_t *) tmp_p;
  if (tmp_p)
    {
      tmp_p->node.dad_p = (node_t *) cur_p;
    }

  bro_p->node.son_p = (node_t *) cur_p;
  bro_p->node.dad_p = cur_p->node.dad_p;
  cur_p->node.dad_p = (node_t *) bro_p;

  *cur_pp = bro_p;
  tmp_p = (link_t *) bro_p->node.dad_p;
  if (tmp_p)
    {
      if (cur_p == (link_t *) tmp_p->node.bro_p)
	{
	  tmp_p->node.bro_p = (node_t *) bro_p;
	}
      else
	{
	  tmp_p->node.son_p = (node_t *) bro_p;
	}
    }
}

/* ------------------------------------------------------------ */


/* rot_right_left --

	Performs a right/left rotation.

	  Example:

                     10 (cur)              11
                    /  \                  /  \
                   5    13 (bro)        10    13
                       /  \     =>     /     /  \
               (son) 11    15         5    12    15
                       \
                        12

*/

static void
rot_right_left (link_t **cur_pp)
{
  link_t *	cur_p;
  link_t *	son_p;
  link_t *	bro_p;
  link_t *	tmp_p;


  cur_p = *cur_pp;
  bro_p = (link_t *) cur_p->node.bro_p;
  son_p = (link_t *) bro_p->node.son_p;

  
  tmp_p = (link_t *) son_p->node.bro_p;
  bro_p->node.son_p = (node_t *) tmp_p;
  if (tmp_p)
    {
      tmp_p->node.dad_p = (node_t *) bro_p;
    }
  
  son_p->node.bro_p = (node_t *) bro_p;
  bro_p->node.dad_p = (node_t *) son_p;

  tmp_p = (link_t *) son_p->node.son_p;
  cur_p->node.bro_p = (node_t *) tmp_p;
  if (tmp_p)
    {
      tmp_p->node.dad_p = (node_t *) cur_p;
    }

  son_p->node.son_p = (node_t *) cur_p;
  son_p->node.dad_p = cur_p->node.dad_p;
  cur_p->node.dad_p = (node_t *) son_p;

  /*
    We have to update the parent  of cur_p to point to son_p, we'll do
    it later.
  */

  cur_p->avl_status = \
    (son_p->avl_status == RIGHT_HIGHER)? LEFT_HIGHER : BALANCED;

  bro_p->avl_status = \
    (son_p->avl_status == LEFT_HIGHER)? RIGHT_HIGHER : BALANCED;

  *cur_pp = son_p;
  tmp_p = (link_t *) son_p->node.dad_p;
  if (tmp_p)
    {
      if (cur_p == (link_t *) tmp_p->node.bro_p)
	{
	  tmp_p->node.bro_p = (node_t *) son_p;
	}
      else
	{
	  tmp_p->node.son_p = (node_t *) son_p;
	}
    }
}

/* ------------------------------------------------------------ */


/* ucl_map_remove --

	Removes a  link.  If the selected  link is not in  the tree, the
	behaviour of this function is not defined.

	  This is not so difficult:

	(1) in the subtree of the target: find an element to replace the
	    erased one,  it's the max in  the son subtree or  the min in
	    the bro subtree;

	(2)  copy the key  and elm  from the  replacement node  into the
            target  node; now  the replacement  node is  the  new target
            node, with key and value already "destroyed";

        (3) if the target node got NULL son and bro it's over, else loop
            to step (2).

          Now from  the parent of the  target node, step up  in the tree
        updating  the  AVL  status  indicator  and  doing  rotations  if
        required.

   Arguments:

	this -		pointer to the base structure
	link_p -	pointer to the link holding the key/value pair
			selected for extraction
	
   Results:

	A target  is extracted from the  tree. Returns a  pointer to the
	extracted link.
*/

stub(2005-09-23-18-11-51) ucl_map_link_t *
ucl_map_remove (ucl_map_t this, ucl_map_link_t *cur_p)
{
  link_t *	del_p;
  link_t *	tmp_p;
  link_t *	link_p;
  value_t	tmpkey, tmpval;
  int		root_flag=0;
  /* int h; */


  if (this->size == 1)
    {
      assert(cur_p == this->root);

      this->root = NULL;
      this->size = 0;
      return cur_p;
    }

  /*
    Save the key  and value of the link to be  removed.  We don't remove
    the node referenced by "cur_p": instead we search its subtree for a
    replacement  and store  the  key  and value  of  the replacement  in
    "*cur_p".

    Then we do it again and again until a leaf node is found: that'll be
    the node actually removed from the tree.
   */

  tmpkey = ucl_map_getkey(cur_p);
  tmpval = ucl_map_getval(cur_p);
  del_p = cur_p;

  while (cur_p->node.son_p || cur_p->node.bro_p)
    {
      if (cur_p->node.son_p)
	{
	  link_p = (link_t *) ucl_btree_find_rightmost(cur_p->node.son_p);
	}
      else
	{
	  link_p = (link_t *) ucl_btree_find_leftmost(cur_p->node.bro_p);
	}

      /*
	Here "link_p" can't be NULL. It's possible that:

		link_p == cur_p->node.bro_p

        or that:

		link_p == cur_p->node.son_p

        but these are not problems.
      */

      cur_p->key = link_p->key;
      cur_p->val = link_p->val;
      cur_p = link_p;
    }

  /*
    Here  "cur_p" references  the  leaf node  that's  removed from  the
    tree. We  store the  key and  value that were  saved before  in this
    node, and leave it alone until  the end of the function: the pointer
    will be the return value.
  */

  del_p = cur_p;
  ucl_map_setkey(del_p, tmpkey);
  ucl_map_setval(del_p, tmpval);


  /*
    Now we have  to step up in the tree,  doing "avl_status" updates and
    rotations when needed.
  */

  link_p = (link_t *) cur_p->node.dad_p; 
  if (cur_p == (link_t *) link_p->node.son_p)
    {
      link_p->node.son_p = NULL;
      if (link_p->node.bro_p)
	{
	  /*
	    Before  this link  was BALANCED,  now the  right  subtree is
	    higher because we've removed  the left subtree.  The subtree
	    hasn't gotten shorter.
	  */

	  link_p->avl_status = RIGHT_HIGHER;
/*  	  h = 0; */
	  goto End;
	}
      else
	{
	  /*
	    Before this link was  LEFT_HIGHER, now it's BALANCED because
	    we've  removed the  left  subtree.  The  subtree has  gotten
	    shorter.
	  */

	  link_p->avl_status = BALANCED;
/*  	  h = 1; */
	}
    }
  else /* cur_p == link_p->node.bro_p */
    {
      link_p->node.bro_p = NULL;
      if (link_p->node.son_p)
	{
	  /*
	    Before  this link  was  BALANCED, now  the  left subtree  is
	    higher because we've removed the right subtree.  The subtree
	    hasn't gotten shorter.
	  */

	  link_p->avl_status = LEFT_HIGHER;
/*  	  h = 0; */
	  goto End;
	}
      else
	{
	  /*
	    Before this link was RIGHT_HIGHER, now it's BALANCED because
	    we've  removed the  right subtree.   The subtree  has gotten
	    shorter.
	  */

	  link_p->avl_status = BALANCED;
/*  	  h = 1; */
	}
    }

/*    while (h) */
  for (;;)
    {
      tmp_p  = link_p;
      link_p = (link_t *) tmp_p->node.dad_p;
      if (!link_p)
	{
	  break;
	}

      if (this->root == link_p)
	{
	  root_flag = 1;
	}

      if (tmp_p == (link_t *) link_p->node.son_p)
	{
	  if (link_p->avl_status == LEFT_HIGHER)
	    {
	      link_p->avl_status = BALANCED;
/*  	      h = 0; */
	      break;
	    }
	  else if (link_p->avl_status == BALANCED)
	    {
	      link_p->avl_status = RIGHT_HIGHER;
/*  	      h = 0; */
	      break;
	    }
	  else /* link_p->avl_status == RIGHT_HIGHER */
	    {
	      tmp_p = (link_t *) link_p->node.bro_p;

	      if (tmp_p->avl_status == RIGHT_HIGHER)
		{
		  rot_right(&link_p);
		}
	      else
		{
		  rot_right_left(&link_p);
		}

	      link_p->avl_status = BALANCED;
	    }
	}
      else /* tmp_p == link_p->node.bro_p */
	{
	  if (link_p->avl_status == RIGHT_HIGHER)
	    {
	      link_p->avl_status = BALANCED;
/*  	      h = 0; */
	      break;
	    }
	  else if (link_p->avl_status == BALANCED)
	    {
	      link_p->avl_status = LEFT_HIGHER;
/*  	      h = 0; */
	      break;
	    }
	  else /* link_p->avl_status == LEFT_HIGHER */
	    {
	      tmp_p = (link_t *) link_p->node.son_p;

	      if (tmp_p->avl_status == LEFT_HIGHER)
		{
		  rot_left(&link_p);
		}
	      else
		{
		  rot_left_right(&link_p);
		}

	      link_p->avl_status = BALANCED;
	    }
	}

      if (root_flag)
	{
	  this->root = link_p;
	}
    }


 End:
  --(this->size);
  return del_p;
}

/* ------------------------------------------------------------ */


/* ucl_map_find --

	Finds the node with the selected key.

   Arguments:

	this -		pointer to the base struct
	key -		the selected key

   Results:

        Returns the  pointer to  the node  or NULL if  no such  node was
        found. For  a multimap, this  function returns a pointer  to the
        first element with  the selected key, so that  the others can be
        found with repeated invocations of "ucl_map_next()".

*/

stub(2005-09-23-18-12-55) ucl_map_link_t *
ucl_map_find (const ucl_map_t this, const ucl_value_t key)
{
  ucl_comparison_t	keycmp;
  int			v;
  link_t *		cur_p;
  link_t *		last_p;


  cur_p = this->root;
  keycmp = this->keycmp;

  while (cur_p != NULL)
    {
      v = keycmp.func(keycmp.data, key, ucl_map_getkey(cur_p));
      if (v > 0)
	{
	  cur_p = (link_t *) cur_p->node.bro_p;
	}
      else if (v < 0)
	{
	  cur_p = (link_t *) cur_p->node.son_p;
	}
      else
	{
	  if (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)
	    {
	      do
		{
		  last_p = cur_p;
		  cur_p = (link_t *) \
		    ucl_btree_step_inorder_backward((node_t *) last_p);
		}
	      while ((cur_p != NULL)
		     && (keycmp.func(keycmp.data, key, ucl_map_getkey(cur_p)) == 0));

	      return last_p;
	    }

	  return cur_p;
	}
    }

  return NULL;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Traversing.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-13-31) ucl_map_link_t *
ucl_map_first (const ucl_map_t this)
{
  return (this->size)? \
    (link_t *) ucl_btree_find_leftmost((node_t *) this->root) : NULL;
}

stub(2005-09-23-18-13-38) ucl_map_link_t *
ucl_map_last (const ucl_map_t this)
{
  return (this->size)? \
    (link_t *) ucl_btree_find_rightmost((node_t *) this->root) : NULL;
}
stub(2005-09-23-18-13-42) ucl_map_link_t *
ucl_map_next (ucl_map_link_t *link_p)
{
  return (link_t *) ucl_btree_step_inorder((node_t *) link_p);
}
stub(2005-09-23-18-13-45) ucl_map_link_t *
ucl_map_prev (ucl_map_link_t *link_p)
{
  return (link_t *) ucl_btree_step_inorder_backward((node_t *) link_p);
}

/* ------------------------------------------------------------ */


/* ucl_map_find_or_next --

	Given a key  find the element in the map  associated with it, or
	the element with the lesser key greater than the selected one.

   Arguments:

	this -		pointer to the base struct
	key -		the selected key

   Results:

        Returns a pointer to the requested  link or NULL if all the keys
        in the map are lesser than the selected one.

*/

stub(2005-09-23-18-13-48) ucl_map_link_t *
ucl_map_find_or_next (const ucl_map_t this, const ucl_value_t key)
{
  int			v;
  link_t *		cur_p;
  link_t *		last_p;
  ucl_comparison_t	keycmp;


  assert(this != 0);

  /* Handle the case of empty map. */
  cur_p = this->root;
  if (cur_p == NULL)
    {
      return cur_p;
    }

  /* Dive in the tree to find the key. */
  keycmp = this->keycmp;

  while (cur_p != NULL)
    {
      last_p = cur_p;

      v = keycmp.func(keycmp.data, key, ucl_map_getkey(cur_p));
      if (v > 0)
	{
	  cur_p = (link_t *) last_p->node.bro_p;
	}
      else if (v < 0)
	{
	  cur_p = (link_t *) last_p->node.son_p;
	}
      else /* v == 0 */
	{
	  if (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)
	    {
	      do
		{
		  last_p = cur_p;
		  cur_p  = (link_t *) ucl_btree_step_inorder((node_t *) last_p);
		  if (cur_p == NULL)
		    {
		      break;
		    }
		  v = keycmp.func(keycmp.data, key, ucl_map_getkey(cur_p));
		}
	      while (v == 0);
	    }

	  return last_p;
	}
    }

  /*
    If we're  here "last_p"  holds a  pointer to the  last node  in the
    previous search:  if "v <  0", this node  is the one we  are looking
    for;  else  we have  to  do  an inorder  step  forward  to find  the
    requested one.
   */

  return (v < 0)? last_p : (link_t *) ucl_btree_step_inorder((node_t *) last_p);
}

/* ------------------------------------------------------------ */


/* ucl_map_find_or_prev --

	Given a key  find the element in the map  associated with it, or
	the element with the greater key lesser than the selected one.

   Arguments:

	this -		pointer to the base struct
	key -		the selected key

   Results:

        Returns a pointer to the requested  link or NULL if all the keys
        in the map are greater than the selected one.

*/

stub(2005-09-23-18-13-51) ucl_map_link_t *
ucl_map_find_or_prev (const ucl_map_t this, const ucl_value_t key)
{
  int			v;
  link_t *		cur_p;
  link_t *		last_p;
  ucl_comparison_t	keycmp;

  assert(this != 0);

  /*
    Handle the case of empty map.
  */

  cur_p = this->root;
  if (cur_p == NULL)
    {
      return cur_p;
    }

  /*
    Dive in the tree to find the key.
  */

  keycmp = this->keycmp;

  while (cur_p != NULL)
    {
      last_p = cur_p;

      v = keycmp.func(keycmp.data, key, ucl_map_getkey(cur_p));
      if (v > 0)
	{
	  cur_p = (link_t *) last_p->node.bro_p;
	}
      else if (v < 0)
	{
	  cur_p = (link_t *) last_p->node.son_p;
	}
      else /* v == 0 */
	{
	  if (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)
	    {
	      do
		{
		  last_p = cur_p;		  
		  cur_p = (link_t *) \
		    ucl_btree_step_inorder_backward((node_t *) last_p);
		  if (cur_p == NULL)
		    {
		      break;
		    }
		  v = keycmp.func(keycmp.data, key, ucl_map_getkey(cur_p));
		}
	      while (v == 0);
	    }

	  return last_p;
	}
    }

  /*
    If we're  here "last_p"  holds a  pointer to the  last node  in the
    previous search:  if "v >  0", this node  is the one we  are looking
    for;  else we  have  to do  an  inorder step  backward  to find  the
    requested one.
   */

  return (v > 0)? last_p : (link_t *) \
    ucl_btree_step_inorder_backward((node_t *) last_p);
}

/* ------------------------------------------------------------ */


/* ucl_map_count --

	Count the elements with a given key.

   Arguments:

	this -		pointer to the base structure
	key -		the selected key

   Results:

        Returns the number of elements.

*/

stub(2005-09-23-18-13-55) size_t
ucl_map_count (const ucl_map_t this, const ucl_value_t key)
{
  size_t		count;
  link_t *		link_p;
  ucl_comparison_t	keycmp;


  assert(this != 0);

  count   = 0;
  link_p = ucl_map_find(this, key);

  if (link_p != NULL)
    {
      keycmp = this->keycmp;

      do
	{
	  ++count;
	  link_p = ucl_map_next(link_p);
	}
      while ((link_p != NULL) && (keycmp.func(keycmp.data, key, ucl_map_getkey(link_p)) == 0));
    }
  
  return count;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Iterators.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-13-58) void
ucl_map_iterator_inorder (const ucl_map_t this, ucl_iterator_t iterator)
{
  assert(this != 0);
  assert(iterator != 0);


  iterator->container = this;

  if (this->size == 0)
    {
      iterator->iterator = 0;
    }
  else
    {
      iterator->iterator = ucl_btree_find_leftmost((node_t *) (this->root));
      iterator->next	 = map_inorder_iterator_next;
    }
}
stub(2005-09-23-18-14-01) void
ucl_map_iterator_inorder_backward (const ucl_map_t this, ucl_iterator_t iterator)
{
  assert(this != 0);
  assert(iterator != 0);


  iterator->container = this;

  if (this->size == 0)
    {
      iterator->iterator = 0;
    }
  else
    {
      iterator->iterator = ucl_btree_find_rightmost((node_t *) (this->root));
      iterator->next	 = map_inorder_backward_iterator_next;
    }
}
stub(2005-09-23-18-14-04) void
ucl_map_iterator_preorder (const ucl_map_t this, ucl_iterator_t iterator)
{
  assert(this != 0);
  assert(iterator != 0);

  iterator->container = this;

  if (this->size == 0)
    {
      iterator->iterator = 0;
    }
  else
    {
      iterator->iterator = this->root;
      iterator->next	 = map_preorder_iterator_next;
    }
}
stub(2005-09-23-18-14-07) void
ucl_map_iterator_postorder (const ucl_map_t this, ucl_iterator_t iterator)
{
  assert(this != 0);
  assert(iterator != 0);

  iterator->container = this;

  if (this->size == 0)
    {
      iterator->iterator = 0;
    }
  else
    {
      iterator->iterator = ucl_btree_find_deepest_son((node_t *) this->root);
      iterator->next	 = map_postorder_iterator_next;
    }
}
stub(2005-09-23-18-14-10) void
ucl_map_iterator_levelorder (const ucl_map_t this, ucl_iterator_t iterator)
{
  assert(this != 0);
  assert(iterator != 0);

  iterator->container = this;

  if (this->size == 0)
    {
      iterator->iterator = 0;
    }
  else
    {
      iterator->iterator = this->root;
      iterator->next	 = map_levelorder_iterator_next;
    }
}
stub(2005-09-23-18-14-14) void
ucl_map_lower_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key)
{
  assert(this != 0);
  assert(iterator != 0);


  iterator->container = this;

  if (! this->size)
    {
      iterator->iterator = 0;
    }
  else
    {
      iterator->iterator = ucl_map_find(this, key);
      iterator->next	 = map_lowerbound_iterator_next;
    }
}
stub(2005-09-23-18-14-17) void
ucl_map_upper_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key)
{
  link_t *	link_p;
  value_t	key1;

  assert(this != 0);
  assert(iterator != 0);


  iterator->container = this;

  if (this->size == 0)
    {
      iterator->iterator = NULL;
    }
  else
    {
      link_p = ucl_map_find_or_next(this, key);
      if (link_p == NULL)
	{
	  iterator->iterator = link_p;
	  return;
	}
      else
	{
	  key1 = ucl_map_getkey(link_p);

	  if (this->keycmp.func(this->keycmp.data, key, key1) == 0)
	    {
	      iterator->iterator	= link_p;
	      iterator->next		= map_upperbound_iterator_next;
	    }
	  else
	    {
	      iterator->iterator = NULL;
	    }
	}
    }
}

/* ------------------------------------------------------------ */

static void
map_inorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_inorder(iterator->iterator);
}
static void
map_inorder_backward_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_inorder_backward(iterator->iterator);
}
static void
map_preorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_preorder(iterator->iterator);
}
static void
map_postorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_postorder(iterator->iterator);
}
static void
map_levelorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_levelorder(iterator->iterator);
}
static void
map_upperbound_iterator_next (iterator_t iterator)
{
  const map_struct_t *this;
  value_t	key, key1;
  link_t *	link_p;


  assert(iterator->iterator != NULL);

  this	  = (const map_struct_t *) iterator->container;
  link_p = iterator->iterator;
  key	  = ucl_map_getkey(link_p);

  link_p = (link_t *) ucl_btree_step_inorder_backward((node_t *) link_p);  
  if (link_p != NULL)
    {
      ucl_comparison_t	keycmp = this->keycmp;

      key1 = ucl_map_getkey(link_p);
      iterator->iterator = (keycmp.func(keycmp.data, key, key1) == 0)? link_p : NULL;
    }
  else
    {
      iterator->iterator = link_p;
    }
}
static void
map_lowerbound_iterator_next (iterator_t iterator)
{
  const map_struct_t *this;
  value_t	key, key1;
  link_t *	link_p;


  assert(iterator->iterator != NULL);

  this	  = (const map_struct_t *) iterator->container;
  link_p = iterator->iterator;
  key	  = ucl_map_getkey(link_p);

  link_p = (link_t *) ucl_btree_step_inorder((node_t *) link_p);  
  if (link_p != NULL)
    {
      ucl_comparison_t	keycmp = this->keycmp;

      key1 = ucl_map_getkey(link_p);
      iterator->iterator = (keycmp.func(keycmp.data, key, key1) == 0)? link_p : NULL;
    }
  else
    {
      iterator->iterator = link_p;
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Set iterators.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-14-23) void
ucl_map_iterator_union (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator)
{
  assert(iter1); assert(iter2); assert(iterator);


  if (ucl_iterator_more(iter1) || ucl_iterator_more(iter2))
    {
      iterator->ptr1	= iter1;
      iterator->ptr2	= iter2;
      iterator->next	= union_iterator_next;

      union_find_next(iter1, iter2, iterator);
    }
  else
    {
      iterator->iterator = NULL;
    }
}
static void
union_iterator_next (iterator_t iterator)
{
  iterator_struct_t * 	iter1;
  iterator_struct_t *	iter2;


  iter1 = iterator->ptr1;
  iter2 = iterator->ptr2;

  if (iterator->container == iter1)
    {
      ucl_iterator_next(iter1);
    }
  else
    {
      ucl_iterator_next(iter2);
    }

  union_find_next(iter1, iter2, iterator);
}
static void
union_find_next (iter1, iter2, iterator)
     iterator_t 	iter1;
     iterator_t 	iter2;
     iterator_t 	iterator;
{
  value_t		key1, key2;
  link_t *		node1;
  link_t *		node2;
  ucl_comparison_t	compar;
  int			v;


  if (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
    {
      compar = ((const map_struct_t *) (iter1->container))->keycmp;

      node1	= ucl_iterator_ptr(iter1);
      node2	= ucl_iterator_ptr(iter2);
    
      key1	= ucl_map_getkey(node1);
      key2	= ucl_map_getkey(node2);

      v = compar.func(compar.data, key1, key2);
      if (v <= 0)
	{
	  iterator->container = iter1;
	  iterator->iterator  = iter1->iterator;
	}
      else 
	{
	  iterator->container = iter2;
	  iterator->iterator  = iter2->iterator;
	}
    }
  else if (ucl_iterator_more(iter1))
    {
      iterator->container = iter1;
      iterator->iterator  = iter1->iterator;
    }
  else if (ucl_iterator_more(iter2))
    {
      iterator->container = iter2;
      iterator->iterator  = iter2->iterator;
    }
  else 
    {
      iterator->iterator = NULL;
    }
}

/* ------------------------------------------------------------ */

stub(2005-09-23-18-14-29) void
ucl_map_iterator_intersection (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator)
{
  assert(iter1); assert(iter2); assert(iterator);


  if (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
    {
      iterator->ptr1	 = iter1;
      iterator->ptr2	 = iter2;
      iterator->next	 = intersection_iterator_next;

      intersection_find_common(iter1, iter2, iterator);
    }
  else
    {
      iterator->iterator = NULL;
    }
}
static void
intersection_iterator_next (iterator_t iterator)
{
  iterator_struct_t * 	iter1;
  iterator_struct_t * 	iter2;

  iter1 = iterator->ptr1;
  iter2 = iterator->ptr2;

  ucl_iterator_next(iter1);
  ucl_iterator_next(iter2);

  intersection_find_common(iter1, iter2, iterator);
}
static void
intersection_find_common (iter1, iter2, iterator)
     iterator_t 	iter1;
     iterator_t 	iter2;
     iterator_t 	iterator;
{
  value_t		key1, key2;
  link_t *		node1;
  link_t *		node2;
  ucl_comparison_t	compar;
  int			v;


  compar = ((const map_struct_t *) (iter1->container))->keycmp;
  while (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
    {
      node1	= ucl_iterator_ptr(iter1);
      node2	= ucl_iterator_ptr(iter2);      

      key1	= ucl_map_getkey(node1);
      key2	= ucl_map_getkey(node2);

      v = compar.func(compar.data, key1, key2);
      if (v == 0)
	{
	  iterator->iterator = iter1->iterator;
	  return;
	}
      else if (v < 0)
	{
	  ucl_iterator_next(iter1);
	}
      else
	{
	  ucl_iterator_next(iter2);
	}
    }

  iterator->iterator = NULL;
}

/* ------------------------------------------------------------ */

stub(2005-09-23-18-14-34) void
ucl_map_iterator_complintersect	(ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t 	iterator)
{
  assert(iter1); assert(iter2); assert(iterator);


  if (ucl_iterator_more(iter1) || ucl_iterator_more(iter2))
    {
      iterator->ptr1	 = iter1;
      iterator->ptr2	 = iter2;
      iterator->next	 = complintersect_iterator_next;

      iterator->container = NULL;

      complintersect_iterator_next(iterator);
    }
  else
    {
      iterator->iterator = NULL;
    }
}
static void
complintersect_iterator_next (iterator_t iterator)
{
  iterator_struct_t * 	iter1;
  iterator_struct_t *	iter2;
  ucl_comparison_t	compar;
  value_t		key1, key2;
  link_t *		node1;
  link_t *		node2;
  int		v;


  iter1 = iterator->ptr1;
  iter2 = iterator->ptr2;

  compar = ((const map_struct_t *) (iter1->container))->keycmp;

  if (iterator->container == iter1)
    {
      ucl_iterator_next(iter1);
    }
  else if (iterator->container == iter2)
    {
      ucl_iterator_next(iter2);
    }

  if (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
    {
      node1 = ucl_iterator_ptr(iter1);
      node2 = ucl_iterator_ptr(iter2);

      key1  = ucl_map_getkey(node1);
      key2  = ucl_map_getkey(node2);

      v = compar.func(compar.data, key1, key2);
      while (v == 0)
	{
	  ucl_iterator_next(iter1);
	  ucl_iterator_next(iter2);
	  
	  if ((! ucl_iterator_more(iter1)) || (! ucl_iterator_more(iter2)))
	    {
	      break;
	    }
	  
	  node1 = ucl_iterator_ptr(iter1);
	  node2 = ucl_iterator_ptr(iter2);

	  key1  = ucl_map_getkey(node1);
	  key2  = ucl_map_getkey(node2);

	  v = compar.func(compar.data, key1, key2);
	}

      if (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
	{
	  if (v < 0)
	    {
	      iterator->iterator  = iter1->iterator;
	      iterator->container = iter1;
	      return;
	    }
	  else if (v > 0)
	    {
	      iterator->iterator  = iter2->iterator;
	      iterator->container = iter2;
	      return;
	    }
	}
    }

  if (ucl_iterator_more(iter1))
    {
      iterator->iterator  = iter1->iterator;
      iterator->container = iter1;
    }
  else if (ucl_iterator_more(iter2))
    {
      iterator->iterator  = iter2->iterator;
      iterator->container = iter2;
    }
  else
    {
      iterator->iterator = NULL;
    }
}

/* ------------------------------------------------------------ */

stub(2005-09-23-18-14-38) void
ucl_map_iterator_subtraction (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator)
{
  assert(iter1); assert(iter2); assert(iterator);


  if (ucl_iterator_more(iter1))
    {
      iterator->ptr1	 = iter1;
      iterator->ptr2	 = iter2;
      iterator->next	 = subtraction_iterator_next;

      iterator->container = (ucl_iterator_more(iter2))? NULL : iter1;
      subtraction_iterator_next(iterator);
    }
  else
    {
      iterator->iterator = NULL;
    }
}
/* subtraction_iterator_next --

	Advances the subtraction iterator. 

	  When  this function  is invoked,  the iterators  for  both the
	sequences  must be  already initialised.  The elements  from the
	sequences referenced  by the  iterators must be  the next  to be
	examined.

	  This means that  we must end this function  invocation with an
	advancement of the  iterators. So that at the  next call the new
	elements are ready to be analysed.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        If the  iteration over sequence 2  is over, all  the values from
        sequence 1 are visited.

	  The algorithm is:

	* if the value from 1 is lesser than the value from 2, select it
	  and return;

	* if the value  from 1 is greater or equal to  the value from 2,
	  advance the second  iterator until the value from  1 is lesser
	  than  the value  from  2; then  select  the value  from 1  and
	  return.

   Side effects:

        None.

*/

static void
subtraction_iterator_next (iterator_t iterator)
{
  iterator_struct_t * 	iter1;
  iterator_struct_t *	iter2;
  ucl_comparison_t	compar;
  value_t		key1, key2;
  link_t *		node1;
  link_t *		node2;
  int			v;


  /*
    If no more values in sequence 1, end.

    This cannot  happen at the  first invocation of this  function: this
    condition is recognised in the initialisation function.
  */

  iter1 = iterator->ptr1;
  if (! ucl_iterator_more(iter1))
    {
      iterator->iterator = NULL;
      return;
    }

  /*
    Examine the second  iterator.  If the iteration over  sequence 2 was
    ended in a previous step, select  the next value from sequence 1 and
    return.
  */

  iter2 = iterator->ptr2;
  if (! ucl_iterator_more(iter2))
    {
      goto Advance;
    }

  /*
    If  the element  from sequence  1 is  lesser than  the  element from
    sequence 2, we select it and return.
   */

  compar = ((const map_struct_t *) (iter1->container))->keycmp;

  node1 = ucl_iterator_ptr(iter1);
  node2 = ucl_iterator_ptr(iter2);

  key1  = ucl_map_getkey(node1);
  key2  = ucl_map_getkey(node2);

  v = compar.func(compar.data, key1, key2);
  if (v < 0)
    {
      goto Advance;
    }

  /*
    Here we  have to advance the  iterator over sequence 2  until we end
    it, or until the value is grater than the value from sequence 1.
  */

  do
    {
      ucl_iterator_next(iter2);
      if (! ucl_iterator_more(iter2))
        {
	  break;
	}
      if (v == 0)
	{
	  ucl_iterator_next(iter1);
	  if (! ucl_iterator_more(iter1))
	    {
	      iterator->iterator = NULL;
	      return;
	    }
	}

      node1 = ucl_iterator_ptr(iter1);
      node2 = ucl_iterator_ptr(iter2);

      key1  = ucl_map_getkey(node1);
      key2  = ucl_map_getkey(node2);

      v = compar.func(compar.data, key1, key2);
    }
  while (v >= 0);

  if ((! ucl_iterator_more(iter2)) && v == 0)
    {
      ucl_iterator_next(iter1);
      if (! ucl_iterator_more(iter1))
	{
	  iterator->iterator = NULL;
	  return;
	}
    }

 Advance:
  iterator->iterator = iter1->iterator;
  ucl_iterator_next(iter1);
  return;
}

/* end of file */
