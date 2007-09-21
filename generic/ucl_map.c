/* ucl_map.c  --

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

   Copyright (c) 2001, 2002, 2003 Marco Maggi

   This library is free software;  you can redistribute it and/or modify
   it  under the  terms  of the  GNU  Lesser General  Public License  as
   published by the Free Software  Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.

   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA

   $Id: ucl_map.c,v 2.0.2.17 2003/12/18 15:17:50 marco Exp $

*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"

#ifdef DEBUGGING
#  include <stdio.h>
#endif


/*
  Values of the "avl_status" field in the "ucl_map_link_t" structure.
*/

#define LEFT_HIGHER		0x00
#define BALANCED		0x01
#define RIGHT_HIGHER		0x02


/*
  These will make the code easier to read.
*/

typedef ucl_voidptr_t		voidp_t;
typedef ucl_map_t		map_t;
typedef ucl_map_link_t		link_t;
typedef ucl_btree_node_t	node_t;
typedef ucl_iterator_t		iterator_t;
typedef ucl_value_t		value_t;
typedef ucl_valcmp_t		keycmp_t;


/*
  Static function prototypes.
*/

static void	map_inorder_iterator_next	(iterator_t *iterator);
static void	map_inorder_backward_iterator_next (iterator_t *iterator);
static void	map_preorder_iterator_next	(iterator_t *iterator);
static void	map_postorder_iterator_next	(iterator_t *iterator);
static void	map_levelorder_iterator_next	(iterator_t *iterator);
static void	map_upperbound_iterator_next	(iterator_t *iterator);
static void	map_lowerbound_iterator_next	(iterator_t *iterator);


static void	rot_left			(link_t **curPtrPtr);
static void	rot_left_right			(link_t **curPtrPtr);
static void	rot_right			(link_t **curPtrPtr);
static void	rot_right_left			(link_t **curPtrPtr);


static void	intersection_iterator_next	(iterator_t *iterator);
static void	intersection_find_common	(iterator_t *iter1,
						 iterator_t *iter2,
						 iterator_t *iterator);


static void	union_iterator_next		(iterator_t *iterator);
static void	union_find_next			(iterator_t *iter1,
						 iterator_t *iter2,
						 iterator_t *iterator);

static void	complintersect_iterator_next (iterator_t *iterator);
static void	subtraction_iterator_next (iterator_t *iterator);


/* ucl_map_constructor --

	Initialises an already  allocated map struct.  Map configuration
	flags can be an ORed combination of:

	UCL_ALLOW_MULTIPLE_OBJECTS -	allows  more than one  object to
					be  associated to the  same key,
					with this the map behaves like a
					multimap.

   Arguments:

	this -		pointer to the base structure
	flags -		map configuration flags
	cmpfun -	pointer to the strcmp()-like function used to
			compare strings

   Results:

        All the data structure  fields are initialised. The container is
        in the "empty" status.

   Side effects:

        none

*/

void
ucl_map_constructor (map_t *this, unsigned int flags, keycmp_t *keycmp)
{
  assert(this != 0);
  assert(keycmp != 0);

  this->root		= 0;
  this->size		= 0;
  this->keycmp		= keycmp;
  this->flags		= flags;
}

/* ucl_map_destructor --

	Destroys a map struct. The structure is not deallocated, the
	elements are not freed; it's responsibility of the user code
	to extract all the nodes before invoking this function.

   Arguments:

	this -		pointer to the base structure

   Results:

        All the structure fields are reset to zero.

   Side effects:

        If  the  elements are  not  extracted  before  this function  is
        invoked, at least memory leakage will happen.

*/

void
ucl_map_destructor (map_t *this)
{
  assert(this != 0);
  memset(this, '\0', sizeof(map_t));
}

/* ucl_map_insert --

	Inserts a new link in the map.

   Arguments:

	this -		pointer to the base structure
	linkPtr -	pointer to the created link

   Results:

	If the container is a multimap, the provided link is inserted in
	the  tree and  the tree  itself is  balanced to  keep it  in AVL
	status. If the container is a  map, and an element with the link
	key already exists in it, nothing happens.

   Side effects:

	Trying  to insert  a link  in a  map container  without checking
	before if an  element with such a key  already exists, sooner or
	later  will result  in an  error. After  the invocation  of this
	function there's no way to establish if the element was inserted
	or not.

	  So first invoke "ucl_map_find()" to see if an element with the
	selected key already exists.

*/

void
ucl_map_insert (map_t *this, link_t *linkPtr)
{
  int		v, root_flag;
  link_t *	tmpPtr;
  link_t *	curPtr;
  value_t	key;
  keycmp_t *	keycmp;

  /*
    A flag  used in the loop below,  when stepping up in  the tree doing
    rotation and  "avl_status" updates: true  if the subtree  from which
    we're coming has gotten higher, otherwise false.

    Mr. Niklaus Wirth calls it "h" in his book.
    
    int		h;
  */


  assert(this != NULL);
  assert(linkPtr != NULL);


  /*
    The new link will have AVL status equal to "balanced". Always.
  */

  linkPtr->avl_status = BALANCED;
  linkPtr->node.dadPtr = linkPtr->node.broPtr = linkPtr->node.sonPtr = NULL;


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

      this->root = linkPtr;
      ++(this->size);
      return;
    }


  /*
    If the map is  not empty, we have to look for  the node that will be
    the parent of the new one.

    If looking for this node we find a node with key equal to the key of
    the new link, and this is not a multimap, we silently do nothing.
  */

  tmpPtr = this->root;
  keycmp = this->keycmp;
  key    = ucl_map_getkey(linkPtr);

  while (tmpPtr != NULL)
    {
      curPtr = tmpPtr;

      v = keycmp(key, ucl_map_getkey(tmpPtr));
      if ((v > 0)
	  || ((v == 0) && (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)))
	{
	  tmpPtr = (link_t *) curPtr->node.broPtr;
	}
      else if (v < 0)
	{
	  tmpPtr = (link_t *) curPtr->node.sonPtr;
	}
      else /* v == 0 && !(this->flag & UCL_ALLOW_MULTIPLE_OBJECTS) */
	{
	  return;
	}
    }

  /*
    Here "curPtr"  holds a  pointer to  the parent of  the new  link. If
    "v<0", we have  to insert the new link as son  of "curPtr" (that is:
    in  the  left subtree),  else  as brother  (that  is:  in the  right
    subtree).


    If we append  the new link to the left of  "curPtr", the link itself
    becomes the left subtree of "curPtr".

                    -----
                   | cur |----> ?
                    -----   bro
                      |
                  son v
                    -----
                   |link |
                    -----
 
    There are two cases:
    
    1) the brother  of "*curPtr" is NULL: "*curPtr"  was BALANCED before
       and becomes LEFT_HIGHER now;

    2) the brother of "*curPtr"  is not NULL: "*curPtr" was RIGHT_HIGHER
       before and becomes BALANCED now.


    If we append the new link  to the right of "curPtr", the link itself
    becomes the right subtree of "curPtr".

                    -----       -----
                   | cur |---->|link |
                    -----  bro  -----
                      |
                  son v
                      ?
 
    There are two cases:
    
    3) the  son of "*curPtr" is NULL: "*curPtr" was BALANCED  before and
       becomes RIGHT_HIGHER now;

    4) the  brother of "*curPtr" is not  NULL: "*curPtr" was LEFT_HIGHER
       before and becomes BALANCED now.

    In the cases (1) and (3)  the subtree of which "*curPtr" is the root
    node becomes higher: we'll have  to report this change to the parent
    of "*curPtr", so we set the "h" flag to "true".
  */

  if (v<0)
    {
      curPtr->node.sonPtr  = (node_t *) linkPtr;
      linkPtr->node.dadPtr = (node_t *) curPtr;
      ++(this->size);
	      
      if (curPtr->node.broPtr != NULL)
	{
	  curPtr->avl_status = BALANCED;
/*  	  h = 0; */
	  return;
	}
      else
	{
	  curPtr->avl_status = LEFT_HIGHER;
/*  	  h = 1; */
	}
    }
  else
    {
      curPtr->node.broPtr  = (node_t *) linkPtr;
      linkPtr->node.dadPtr = (node_t *) curPtr;
      ++(this->size);

      if (curPtr->node.sonPtr != NULL)
	{
	  curPtr->avl_status = BALANCED;
/*  	  h = 0; */
	  return;
	}
      else
	{
	  curPtr->avl_status = RIGHT_HIGHER;
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
      tmpPtr = curPtr;
      curPtr = (link_t *) tmpPtr->node.dadPtr;
      if (curPtr == NULL) { return; }

      /*
	If we  are stepping up  from a left  subtree set "v"  to "true",
	otherwise set it to "false".
      */

      v = (curPtr->node.sonPtr == (node_t *) tmpPtr)? 1 : 0;

      if (v) /* stepping up from a left subtree */
	{
	  if (curPtr->avl_status == RIGHT_HIGHER)
	    {
	      curPtr->avl_status = BALANCED;
/*  	      h = 0; */
	      return;
	    }
	  else if (curPtr->avl_status == BALANCED)
	    {
	      curPtr->avl_status = LEFT_HIGHER;
/*  	      h = 1; */
	      continue;
	    }
	  else /* curPtr->avl_status == LEFT_HIGHER */
	    {
	      root_flag = (curPtr == this->root)? 1 : 0;
		      
	      if (tmpPtr->avl_status == LEFT_HIGHER)
		{
		  rot_left(&curPtr);
		}
	      else
		{
		  rot_left_right(&curPtr);
		}
	      curPtr->avl_status = BALANCED;

	      if (root_flag)
		{
		  this->root = curPtr;
		}
/*  	      h = 0; */
	      return;
	    }
	}
      else /* v == 0 -> stepping up from a right subtree */
	{
	  if (curPtr->avl_status == LEFT_HIGHER)
	    {
	      curPtr->avl_status = BALANCED;
/*  	      h = 0; */
	      return;
	    }
	  else if (curPtr->avl_status == BALANCED)
	    {
	      curPtr->avl_status = RIGHT_HIGHER;
/*  	      h = 1; */
	      continue;
	    }
	  else /* curPtr->avl_status == RIGHT_HIGHER */
	    {
	      root_flag = (curPtr == this->root)? 1 : 0;

	      if (tmpPtr->avl_status == RIGHT_HIGHER)
		{
		  rot_right(&curPtr);
		}
	      else
		{
		  rot_right_left(&curPtr);
		}
	      curPtr->avl_status = BALANCED;
	      
	      if (root_flag)
		{
		  this->root = curPtr;
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

	*curPtrPtr -	pointer to the variable holding the current
			node pointer

   Results:

        Returns nothing

   Side effects:

        None.

*/

static void
rot_left (link_t **curPtrPtr)
{
  link_t *	curPtr;
  link_t *	sonPtr;
  link_t *	tmpPtr;


  curPtr = *curPtrPtr;
  sonPtr = (link_t *) curPtr->node.sonPtr;

  tmpPtr = (link_t *) sonPtr->node.broPtr;
  curPtr->node.sonPtr = (node_t *) tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->node.dadPtr = (node_t *) curPtr;
    }
  
  sonPtr->node.broPtr = (node_t *) curPtr;
  sonPtr->node.dadPtr = curPtr->node.dadPtr;
  curPtr->node.dadPtr = (node_t *) sonPtr;

  curPtr->avl_status = 0;

  *curPtrPtr = sonPtr;
  tmpPtr = (link_t *) sonPtr->node.dadPtr;
  if (tmpPtr)
    {
      if (curPtr == (link_t *) tmpPtr->node.broPtr)
	{
	  tmpPtr->node.broPtr = (node_t *) sonPtr;
	}
      else
	{
	  tmpPtr->node.sonPtr = (node_t *) sonPtr;
	}
    }
}

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


   Arguments:

	*curPtrPtr -	pointer to the variable holding the current
			node pointer

   Results:

        Returns nothing

   Side effects:

        None.

*/

static void
rot_left_right (link_t **curPtrPtr)
{
  link_t *	curPtr;
  link_t *	sonPtr;
  link_t *	broPtr;
  link_t *	tmpPtr;


  curPtr = *curPtrPtr;
  sonPtr  = (link_t *) curPtr->node.sonPtr;
  broPtr  = (link_t *) sonPtr->node.broPtr;

  tmpPtr = (link_t *) broPtr->node.sonPtr;
  sonPtr->node.broPtr = (node_t *) tmpPtr;
  if (tmpPtr) /* useless test? */
    {
      tmpPtr->node.dadPtr = (node_t *) sonPtr;
    }

  broPtr->node.sonPtr = (node_t *) sonPtr;
  sonPtr->node.dadPtr = (node_t *) broPtr;

  tmpPtr = (link_t *) broPtr->node.broPtr;
  curPtr->node.sonPtr = (node_t *) tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->node.dadPtr = (node_t *) curPtr;
    }

  broPtr->node.broPtr = (node_t *) curPtr;
  broPtr->node.dadPtr = curPtr->node.dadPtr;
  curPtr->node.dadPtr = (node_t *) broPtr;

  curPtr->avl_status = \
    (broPtr->avl_status == LEFT_HIGHER)? RIGHT_HIGHER : BALANCED;

  sonPtr->avl_status = \
    (broPtr->avl_status == RIGHT_HIGHER)? LEFT_HIGHER : BALANCED;

  *curPtrPtr = broPtr;
  tmpPtr = (link_t *) broPtr->node.dadPtr;
  if (tmpPtr)
    {
      if (curPtr == (link_t *) tmpPtr->node.broPtr)
	{
	  tmpPtr->node.broPtr = (node_t *) broPtr;
	}
      else
	{
	  tmpPtr->node.sonPtr = (node_t *) broPtr;
	}
    }
}

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


   Arguments:

	*curPtrPtr -	pointer to the variable holding the current
			node pointer

   Results:

        Returns nothing

   Side effects:

        None.

*/

static void
rot_right (link_t **curPtrPtr)
{
  link_t *	curPtr;
  link_t *	broPtr;
  link_t *	tmpPtr;


  curPtr = *curPtrPtr;
  broPtr  = (link_t *) curPtr->node.broPtr;

  tmpPtr = (link_t *) broPtr->node.sonPtr;
  curPtr->node.broPtr = (node_t *) tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->node.dadPtr = (node_t *) curPtr;
    }

  broPtr->node.sonPtr = (node_t *) curPtr;
  broPtr->node.dadPtr = curPtr->node.dadPtr;
  curPtr->node.dadPtr = (node_t *) broPtr;

  *curPtrPtr = broPtr;
  tmpPtr = (link_t *) broPtr->node.dadPtr;
  if (tmpPtr)
    {
      if (curPtr == (link_t *) tmpPtr->node.broPtr)
	{
	  tmpPtr->node.broPtr = (node_t *) broPtr;
	}
      else
	{
	  tmpPtr->node.sonPtr = (node_t *) broPtr;
	}
    }
}

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


   Arguments:

	*curPtrPtr -	pointer to the variable holding the current
			node pointer

   Results:

        Returns nothing

   Side effects:

        None.

*/

static void
rot_right_left (link_t **curPtrPtr)
{
  link_t *	curPtr;
  link_t *	sonPtr;
  link_t *	broPtr;
  link_t *	tmpPtr;


  curPtr = *curPtrPtr;
  broPtr = (link_t *) curPtr->node.broPtr;
  sonPtr = (link_t *) broPtr->node.sonPtr;

  
  tmpPtr = (link_t *) sonPtr->node.broPtr;
  broPtr->node.sonPtr = (node_t *) tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->node.dadPtr = (node_t *) broPtr;
    }
  
  sonPtr->node.broPtr = (node_t *) broPtr;
  broPtr->node.dadPtr = (node_t *) sonPtr;

  tmpPtr = (link_t *) sonPtr->node.sonPtr;
  curPtr->node.broPtr = (node_t *) tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->node.dadPtr = (node_t *) curPtr;
    }

  sonPtr->node.sonPtr = (node_t *) curPtr;
  sonPtr->node.dadPtr = curPtr->node.dadPtr;
  curPtr->node.dadPtr = (node_t *) sonPtr;

  /*
    We have to update the parent  of curPtr to point to sonPtr, we'll do
    it later.
  */

  curPtr->avl_status = \
    (sonPtr->avl_status == RIGHT_HIGHER)? LEFT_HIGHER : BALANCED;

  broPtr->avl_status = \
    (sonPtr->avl_status == LEFT_HIGHER)? RIGHT_HIGHER : BALANCED;

  *curPtrPtr = sonPtr;
  tmpPtr = (link_t *) sonPtr->node.dadPtr;
  if (tmpPtr)
    {
      if (curPtr == (link_t *) tmpPtr->node.broPtr)
	{
	  tmpPtr->node.broPtr = (node_t *) sonPtr;
	}
      else
	{
	  tmpPtr->node.sonPtr = (node_t *) sonPtr;
	}
    }
}

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
	linkPtr -	pointer to the link holding the key/value pair
			selected for extraction
	
   Results:

	A target  is extracted from the  tree. Returns a  pointer to the
	extracted link.

   Side effects:

        none

*/

link_t *
ucl_map_remove (map_t *this, link_t *curPtr)
{
  link_t *	delPtr;
  link_t *	tmpPtr;
  link_t *	linkPtr;
  value_t	tmpkey, tmpval;
  int		root_flag=0;
  /* int h; */


  if (this->size == 1)
    {
      assert(curPtr == this->root);

      this->root = NULL;
      this->size = 0;
      return curPtr;
    }

  /*
    Save the key  and value of the link to be  removed.  We don't remove
    the node referenced by "curPtr": instead we search its subtree for a
    replacement  and store  the  key  and value  of  the replacement  in
    "*curPtr".

    Then we do it again and again until a leaf node is found: that'll be
    the node actually removed from the tree.
   */

  tmpkey = ucl_map_getkey(curPtr);
  tmpval = ucl_map_getval(curPtr);
  delPtr = curPtr;

  while (curPtr->node.sonPtr || curPtr->node.broPtr)
    {
      if (curPtr->node.sonPtr)
	{
	  linkPtr = (link_t *) ucl_btree_find_rightmost(curPtr->node.sonPtr);
	}
      else
	{
	  linkPtr = (link_t *) ucl_btree_find_leftmost(curPtr->node.broPtr);
	}

      /*
	Here "linkPtr" can't be NULL. It's possible that:

		linkPtr == curPtr->node.broPtr

        or that:

		linkPtr == curPtr->node.sonPtr

        but these are not problems.
      */

      curPtr->key = linkPtr->key;
      curPtr->val = linkPtr->val;
      curPtr = linkPtr;
    }

  /*
    Here  "curPtr" references  the  leaf node  that's  removed from  the
    tree. We  store the  key and  value that were  saved before  in this
    node, and leave it alone until  the end of the function: the pointer
    will be the return value.
  */

  delPtr = curPtr;
  ucl_map_setkey(delPtr, tmpkey);
  ucl_map_setval(delPtr, tmpval);


  /*
    Now we have  to step up in the tree,  doing "avl_status" updates and
    rotations when needed.
  */

  linkPtr = (link_t *) curPtr->node.dadPtr; 
  if (curPtr == (link_t *) linkPtr->node.sonPtr)
    {
      linkPtr->node.sonPtr = NULL;
      if (linkPtr->node.broPtr)
	{
	  /*
	    Before  this link  was BALANCED,  now the  right  subtree is
	    higher because we've removed  the left subtree.  The subtree
	    hasn't gotten shorter.
	  */

	  linkPtr->avl_status = RIGHT_HIGHER;
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

	  linkPtr->avl_status = BALANCED;
/*  	  h = 1; */
	}
    }
  else /* curPtr == linkPtr->node.broPtr */
    {
      linkPtr->node.broPtr = NULL;
      if (linkPtr->node.sonPtr)
	{
	  /*
	    Before  this link  was  BALANCED, now  the  left subtree  is
	    higher because we've removed the right subtree.  The subtree
	    hasn't gotten shorter.
	  */

	  linkPtr->avl_status = LEFT_HIGHER;
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

	  linkPtr->avl_status = BALANCED;
/*  	  h = 1; */
	}
    }

/*    while (h) */
  for (;;)
    {
      tmpPtr  = linkPtr;
      linkPtr = (link_t *) tmpPtr->node.dadPtr;
      if (!linkPtr)
	{
	  break;
	}

      if (this->root == linkPtr)
	{
	  root_flag = 1;
	}

      if (tmpPtr == (link_t *) linkPtr->node.sonPtr)
	{
	  if (linkPtr->avl_status == LEFT_HIGHER)
	    {
	      linkPtr->avl_status = BALANCED;
/*  	      h = 0; */
	      break;
	    }
	  else if (linkPtr->avl_status == BALANCED)
	    {
	      linkPtr->avl_status = RIGHT_HIGHER;
/*  	      h = 0; */
	      break;
	    }
	  else /* linkPtr->avl_status == RIGHT_HIGHER */
	    {
	      tmpPtr = (link_t *) linkPtr->node.broPtr;

	      if (tmpPtr->avl_status == RIGHT_HIGHER)
		{
		  rot_right(&linkPtr);
		}
	      else
		{
		  rot_right_left(&linkPtr);
		}

	      linkPtr->avl_status = BALANCED;
	    }
	}
      else /* tmpPtr == linkPtr->node.broPtr */
	{
	  if (linkPtr->avl_status == RIGHT_HIGHER)
	    {
	      linkPtr->avl_status = BALANCED;
/*  	      h = 0; */
	      break;
	    }
	  else if (linkPtr->avl_status == BALANCED)
	    {
	      linkPtr->avl_status = LEFT_HIGHER;
/*  	      h = 0; */
	      break;
	    }
	  else /* linkPtr->avl_status == LEFT_HIGHER */
	    {
	      tmpPtr = (link_t *) linkPtr->node.sonPtr;

	      if (tmpPtr->avl_status == LEFT_HIGHER)
		{
		  rot_left(&linkPtr);
		}
	      else
		{
		  rot_left_right(&linkPtr);
		}

	      linkPtr->avl_status = BALANCED;
	    }
	}

      if (root_flag)
	{
	  this->root = linkPtr;
	}
    }


 End:
  --(this->size);
  return delPtr;
}

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

   Side effects:

        none

*/

link_t *
ucl_map_find (const map_t *this, const value_t key)
{
  int		v;
  link_t *	curPtr;
  link_t *	lastPtr;
  keycmp_t *	keycmp;


  curPtr = this->root;
  keycmp = this->keycmp;

  while (curPtr != NULL)
    {
      v = keycmp(key, ucl_map_getkey(curPtr));
      if (v > 0)
	{
	  curPtr = (link_t *) curPtr->node.broPtr;
	}
      else if (v < 0)
	{
	  curPtr = (link_t *) curPtr->node.sonPtr;
	}
      else
	{
	  if (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)
	    {
	      do
		{
		  lastPtr = curPtr;
		  curPtr = (link_t *) \
		    ucl_btree_step_inorder_backward((node_t *) lastPtr);
		}
	      while ((curPtr != NULL)
		     && (keycmp(key, ucl_map_getkey(curPtr)) == 0));

	      return lastPtr;
	    }

	  return curPtr;
	}
    }

  return NULL;
}

/* ucl_map_first --

	Finds the element with lesser key.

   Arguments:

	this -		pointer to the base structure

   Results:

        Returns a  pointer to the link  with lesser key, or  NULL if the
        container is empty.

   Side effects:

        None.

*/

link_t *
ucl_map_first	(const map_t *this)
{
  if (this->size == 0)
    {
      return NULL;
    }

  return (link_t *) ucl_btree_find_leftmost((node_t *) this->root);
}

/* ucl_map_last --

	Finds the element with the greater key.

   Arguments:

	this -		pointer to the base structure

   Results:

        Returns a pointer  to the link with greater key,  or NULL if the
        container is empty.

   Side effects:

        None.

*/

link_t *
ucl_map_last	(const map_t *this)
{
  if (this->size == 0)
    {
      return NULL;
    }

  return (link_t *) ucl_btree_find_rightmost((node_t *) this->root);
}

/* ucl_map_next --

	Given a pointer a map link, finds the adjacent link with greater
	key.

   Arguments:

	linkPtr -	pointer to a map link

   Results:

        Returns a pointer to the next link, or NULL if the selected link
        is the one with greater key in the map.

   Side effects:

        None.

*/

link_t *
ucl_map_next (const link_t *linkPtr)
{
  return (link_t *) ucl_btree_step_inorder((node_t *) linkPtr);
}

/* ucl_map_prev --

	Given a pointer a map  link, finds the adjacent link with lesser
	key.

   Arguments:

	linkPtr -	pointer to a map link

   Results:

        Returns a pointer to the  previous link, or NULL if the selected
        link is the one with lesser key in the map.

   Side effects:

        None.

*/

link_t *
ucl_map_prev (const link_t *linkPtr)
{
  return (link_t *) ucl_btree_step_inorder_backward((node_t *) linkPtr);
}

/* ucl_map_find_or_next --

	Given a key  find the element in the map  associated with it, or
	the element with the lesser key greater than the selected one.

   Arguments:

	this -		pointer to the base struct
	key -		the selected key

   Results:

        Returns a pointer to the requested  link or NULL if all the keys
        in the map are lesser than the selected one.

   Side effects:

        none

*/

link_t *
ucl_map_find_or_next (const map_t *this, const value_t key)
{
  int		v;
  link_t *	curPtr;
  link_t *	lastPtr;
  keycmp_t *	keycmp;


  assert(this != 0);

  /*
    Handle the case of empty map.
  */

  curPtr = this->root;
  if (curPtr == NULL)
    {
      return curPtr;
    }

  /*
    Dive in the tree to find the key.
  */

  keycmp = this->keycmp;

  while (curPtr != NULL)
    {
      lastPtr = curPtr;

      v = keycmp(key, ucl_map_getkey(curPtr));
      if (v > 0)
	{
	  curPtr = (link_t *) lastPtr->node.broPtr;
	}
      else if (v < 0)
	{
	  curPtr = (link_t *) lastPtr->node.sonPtr;
	}
      else /* v == 0 */
	{
	  if (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)
	    {
	      do
		{
		  lastPtr = curPtr;
		  curPtr  = (link_t *) ucl_btree_step_inorder((node_t *) lastPtr);
		  if (curPtr == NULL)
		    {
		      break;
		    }
		  v = keycmp(key, ucl_map_getkey(curPtr));
		}
	      while (v == 0);
	    }

	  return lastPtr;
	}
    }

  /*
    If we're  here "lastPtr"  holds a  pointer to the  last node  in the
    previous search:  if "v <  0", this node  is the one we  are looking
    for;  else  we have  to  do  an inorder  step  forward  to find  the
    requested one.
   */

  return (v < 0)? lastPtr : (link_t *) ucl_btree_step_inorder((node_t *) lastPtr);
}

/* ucl_map_find_or_prev --

	Given a key  find the element in the map  associated with it, or
	the element with the greater key lesser than the selected one.

   Arguments:

	this -		pointer to the base struct
	key -		the selected key

   Results:

        Returns a pointer to the requested  link or NULL if all the keys
        in the map are greater than the selected one.

   Side effects:

        none

*/

link_t *
ucl_map_find_or_prev (const map_t *this, const value_t key)
{
  int		v;
  link_t *	curPtr;
  link_t *	lastPtr;
  keycmp_t *	keycmp;

  assert(this != 0);

  /*
    Handle the case of empty map.
  */

  curPtr = this->root;
  if (curPtr == NULL)
    {
      return curPtr;
    }

  /*
    Dive in the tree to find the key.
  */

  keycmp = this->keycmp;

  while (curPtr != NULL)
    {
      lastPtr = curPtr;

      v = keycmp(key, ucl_map_getkey(curPtr));
      if (v > 0)
	{
	  curPtr = (link_t *) lastPtr->node.broPtr;
	}
      else if (v < 0)
	{
	  curPtr = (link_t *) lastPtr->node.sonPtr;
	}
      else /* v == 0 */
	{
	  if (this->flags & UCL_ALLOW_MULTIPLE_OBJECTS)
	    {
	      do
		{
		  lastPtr = curPtr;		  
		  curPtr = (link_t *) \
		    ucl_btree_step_inorder_backward((node_t *) lastPtr);
		  if (curPtr == NULL)
		    {
		      break;
		    }
		  v = keycmp(key, ucl_map_getkey(curPtr));
		}
	      while (v == 0);
	    }

	  return lastPtr;
	}
    }

  /*
    If we're  here "lastPtr"  holds a  pointer to the  last node  in the
    previous search:  if "v >  0", this node  is the one we  are looking
    for;  else we  have  to do  an  inorder step  backward  to find  the
    requested one.
   */

  return (v > 0)? lastPtr : (link_t *) \
    ucl_btree_step_inorder_backward((node_t *) lastPtr);
}

/* ucl_map_count --

	Count the elements with a given key.

   Arguments:

	this -		pointer to the base structure
	key -		the selected key

   Results:

        Returns the number of elements.

   Side effects:

        For a map the count is always 0 or 1.

*/

size_t
ucl_map_count (const map_t *this, const value_t key)
{
  size_t	count;
  link_t *	linkPtr;
  keycmp_t *	keycmp;


  assert(this != 0);

  count   = 0;
  linkPtr = ucl_map_find(this, key);

  if (linkPtr != NULL)
    {
      keycmp = this->keycmp;

      do
	{
	  ++count;
	  linkPtr = ucl_map_next(linkPtr);
	}
      while ((linkPtr != NULL) && (keycmp(key, ucl_map_getkey(linkPtr)) == 0));
    }
  
  return count;
}

/* ucl_map_iterator_inorder --

	Initialises an inorder iteration.

   Arguments:

	this -		pointer to the base struct
	iterator -	pointer to an already allocated iterator struct

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

*/

void
ucl_map_iterator_inorder (const map_t *this, iterator_t *iterator)
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

/* ucl_map_iterator_inorder_backward --

	Initialises a backward inorder iteration.

   Arguments:

	this -		pointer to the base struct
	iterator -	pointer to an already allocated iterator struct

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

*/

void
ucl_map_iterator_inorder_backward (const map_t *this, iterator_t *iterator)
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

/* ucl_map_iterator_preorder --

	Initialises a preorder iteration.

   Arguments:

	this -		pointer to the base struct
	iterator -	pointer to an already allocated iterator struct

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

*/

void
ucl_map_iterator_preorder (const map_t *this, iterator_t *iterator)
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

/* ucl_map_iterator_postorder --

	Initialise an postorder iteration.

   Arguments:

	this -		ptr to the base struct
	iterator: ptr to an already allocated iterator struct

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

*/

void
ucl_map_iterator_postorder (const map_t *this, iterator_t *iterator)
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

/* ucl_map_iterator_levelorder --

	Initialise an levelorder iteration.

   Arguments:

	this -		ptr to the base struct
	iterator -	ptr to an already allocated iterator struct

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

*/

void
ucl_map_iterator_levelorder (const map_t *this, iterator_t *iterator)
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

/* ucl_map_lower_bound --

	Initialises a lower bound iteration.

   Arguments:

	this -		pointer to the base struct
	iterator -	pointer to an already allocated iterator struct
	key -		the selected key

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

        For a map  container the lower bound iteration  always visits at
        most a single node.

*/

void
ucl_map_lower_bound(this, iterator, key)
     const map_t *	this;
     iterator_t *		iterator;
     const value_t	key;
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
      iterator->iterator = ucl_map_find(this, key);
      iterator->next	 = map_lowerbound_iterator_next;
    }
}

/* ucl_map_upper_bound --

	Initialises an upper bound iteration.

   Arguments:

	this -		pointer to the base struct
	iterator -	pointer to an already allocated iterator struct
	key -		the selected key

   Results:

        The members  of the iterator  structure are initialised.   It is
        possible for the iteration to be immediatly over.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no remove operations must be performed).

        For a map  container the upper bound iteration  always visits at
        most a single node.

*/

void
ucl_map_upper_bound(this, iterator, key)
     const map_t *	this;
     iterator_t *		iterator;
     const value_t 	key;
{
  link_t *	linkPtr;
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
      linkPtr = ucl_map_find_or_next(this, key);
      if (linkPtr == NULL)
	{
	  iterator->iterator = linkPtr;
	  return;
	}
      else
	{
	  key1 = ucl_map_getkey(linkPtr);

	  if (this->keycmp(key, key1) == 0)
	    {
	      iterator->iterator	= linkPtr;
	      iterator->next		= map_upperbound_iterator_next;
	    }
	  else
	    {
	      iterator->iterator = NULL;
	    }
	}
    }
}

/* map_inorder_iterator_next --

	Performs a step in the inorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
map_inorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_inorder(iterator->iterator);
}

/* map_inorder_backward_iterator_next --

	Performs a step in the backward inorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
map_inorder_backward_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_inorder_backward(iterator->iterator);
}

/* map_preorder_iterator_next --

	Performs a step in the preorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
map_preorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_preorder(iterator->iterator);
}

/* map_postorder_iterator_next --

	Performs a step in the postorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
map_postorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_postorder(iterator->iterator);
}

/* map_levelorder_iterator_next --

	Performs a step in the level order iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
map_levelorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_levelorder(iterator->iterator);
}

/* map_upperbound_iterator_next --

	Performs a step in the upper bound iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing

   Side effects:

        None.

*/

static void
map_upperbound_iterator_next (iterator_t *iterator)
{
  map_t *		this;
  value_t		key, key1;
  link_t *	linkPtr;


  assert(iterator->iterator != NULL);

  this	  = (map_t *) iterator->container;
  linkPtr = iterator->iterator;
  key	  = ucl_map_getkey(linkPtr);

  linkPtr = (link_t *) ucl_btree_step_inorder_backward((node_t *) linkPtr);  
  if (linkPtr != NULL)
    {
      key1 = ucl_map_getkey(linkPtr);
      iterator->iterator = (this->keycmp(key, key1) == 0)? linkPtr : NULL;
    }
  else
    {
      iterator->iterator = linkPtr;
    }
}

/* map_lowerbound_iterator_next --

	Performs a step in the lower bound iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
map_lowerbound_iterator_next (iterator_t *iterator)
{
  map_t *		this;
  value_t		key, key1;
  link_t *	linkPtr;


  assert(iterator->iterator != NULL);

  this	  = (map_t *) iterator->container;
  linkPtr = iterator->iterator;
  key	  = ucl_map_getkey(linkPtr);

  linkPtr = (link_t *) ucl_btree_step_inorder((node_t *) linkPtr);  
  if (linkPtr != NULL)
    {
      key1 = ucl_map_getkey(linkPtr);
      iterator->iterator = (this->keycmp(key, key1) == 0)? linkPtr : NULL;
    }
  else
    {
      iterator->iterator = linkPtr;
    }
}

/* ucl_map_iterator_union --

	Initialises  an iterator  for  the union  operation between  two
	maps.

   Arguments:

	iter1 -		pointer to the inorder iterator for map 1
	iter2 -		pointer to the inorder iterator for map 2
	iterator -	pointer to a set iterator structure

   Results:

        A union iteration is used to iterate through all the elements of
        two maps, in order.

   Side effects:

        None.

*/

void
ucl_map_iterator_union	(iter1, iter2, iterator)
     iterator_t *	iter1;
     iterator_t *	iter2;
     iterator_t *	iterator;
{
  assert(iter1 != NULL);
  assert(iter2 != NULL);
  assert(iterator != NULL);


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

/* union_iterator_next --

	Advances a union iteration.

   Arguments:

	iterator -	ponter to the set iterator structure

   Results:

        The two internal iterations are  advanced until one of them it's
        over or two equal elements are found.

   Side effects:

        None.

*/

static void
union_iterator_next (iterator_t *iterator)
{
  iterator_t *	iter1;
  iterator_t *	iter2;


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

/* union_find_next --

	Find the next element in the union iteration.

   Arguments:

	iter1 -		pointer to the inorder iterator for map 1
	iter2 -		pointer to the inorder iterator for map 2
	iterator -	pointer to the union iterator

   Results:

        Returns nothing.

   Side effects:

        None.

*/

static void
union_find_next (iter1, iter2, iterator)
     iterator_t *	iter1;
     iterator_t *	iter2;
     iterator_t *	iterator;
{
  value_t		key1, key2;
  link_t *	node1;
  link_t *	node2;
  keycmp_t *	compar;
  int		v;


  if (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
    {
      compar = ((map_t *) (iter1->container))->keycmp;

      node1	= ucl_iterator_ptr(iter1);
      node2	= ucl_iterator_ptr(iter2);
    
      key1	= ucl_map_getkey(node1);
      key2	= ucl_map_getkey(node2);

      v = compar(key1, key2);
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

/* ucl_map_iterator_intersection --

	Initialises an  iterator for the  intersection operation between
	two maps.

   Arguments:

	iter1 -		pointer to the inorder iterator for map 1
	iter2 -		pointer to the inorder iterator for map 2
	iterator -	pointer to a set iterator structure

   Results:

        An  intersection  iteration  is  used  to  iterate  through  the
        elements common to both the maps.

   Side effects:

        None.

*/

void
ucl_map_iterator_intersection (iter1, iter2, iterator)
     iterator_t *	iter1;
     iterator_t *	iter2;
     iterator_t *	iterator;
{
  assert(iter1 != NULL);
  assert(iter2 != NULL);
  assert(iterator != NULL);


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

/* intersection_iterator_next --

	Advances an intersection iteration.

   Arguments:

	iterator -	ponter to the set iterator structure

   Results:

        The two internal iterations are  advanced until one of them it's
        over or two equal elements are found.

   Side effects:

        None.

*/

static void
intersection_iterator_next (iterator_t *iterator)
{
  iterator_t *	iter1;
  iterator_t *	iter2;

  iter1 = iterator->ptr1;
  iter2 = iterator->ptr2;

  ucl_iterator_next(iter1);
  ucl_iterator_next(iter2);

  intersection_find_common(iter1, iter2, iterator);
}

/* intersection_find_common --

	Finds the next two equal elements in the iterators.

   Arguments:

	iter1 -		pointer to the iterator for the first map
	iter2 -		pointer to the iterator for the second map

   Results:

        Returns a pointer to the map target link.

   Side effects:

        None.

*/

static void
intersection_find_common (iter1, iter2, iterator)
     iterator_t *	iter1;
     iterator_t *	iter2;
     iterator_t *	iterator;
{
  value_t		key1, key2;
  link_t *	node1;
  link_t *	node2;
  keycmp_t *	compar;
  int		v;


  compar = ((map_t *) (iter1->container))->keycmp;
  while (ucl_iterator_more(iter1) && ucl_iterator_more(iter2))
    {
      node1	= ucl_iterator_ptr(iter1);
      node2	= ucl_iterator_ptr(iter2);      

      key1	= ucl_map_getkey(node1);
      key2	= ucl_map_getkey(node2);

      v = compar(key1, key2);
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

/* ucl_map_iterator_complintersect --

	Initialises the complementary intersection iterator.

   Arguments:

	iter1 -		pointer to the first container iterator
	iter2 -		pointer to the second container iterator
	iterator -	pointer to the iterator

   Results:

        The  complementary intersection iterator  will iterate  over all
	the elements that the containers have not in common.

   Side effects:

        None.

*/

void
ucl_map_iterator_complintersect	(iter1, iter2, iterator)
     iterator_t *	iter1;
     iterator_t *	iter2;
     iterator_t *	iterator;
{
  assert(iter1 != NULL);
  assert(iter2 != NULL);
  assert(iterator != NULL);


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

/* complintersect_iterator_next --

	Advances the complementary intersection iterator.

   Arguments:

	iterator -	poiter to the iterator structure

   Results:

        

   Side effects:

        None.

*/

static void
complintersect_iterator_next (iterator_t *iterator)
{
  iterator_t *	iter1;
  iterator_t *	iter2;
  keycmp_t *	compar;
  value_t	key1, key2;
  link_t *	node1;
  link_t *	node2;
  int		v;


  iter1 = iterator->ptr1;
  iter2 = iterator->ptr2;

  compar = ((map_t *) (iter1->container))->keycmp;

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

      v = compar(key1, key2);
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

	  v = compar(key1, key2);
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

/* ucl_map_iterator_subtraction --

	Initialises the  subtraction iterator. The  subtraction iterator
	will visit all the elements  from the first sequence not present
	in the second sequence.

	  Both the sequences  must visit the sources in  order: from the
	lesser to the greater value.

	  Through the  iteration the "container" member  of the iterator
	is used as a state variable, with the following meaning:

	NULL -		the iteration is at the first step;
	iter1 -		the second sequence is ended, so each step
			should directly visit the next element from
			the first sequence.

   Arguments:

	iter1 -		pointer to the first sequence iterator
	iter2 -		pointer to the second sequence iterator
	iterator -	pointer to the iterator

   Results:

	The iterator structure fields are initialised.

	If the first sequence is  empty: the iteration is ended here. If
	the second sequence is empty, we signal it by storing "iter1" in
	the "container" member of the iterator.

   Side effects:

        None.

*/

void
ucl_map_iterator_subtraction (iter1, iter2, iterator)
     iterator_t *	iter1;
     iterator_t *	iter2;
     iterator_t *	iterator;
{
  assert(iter1 != NULL);
  assert(iter2 != NULL);
  assert(iterator != NULL);


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
subtraction_iterator_next (iterator_t *iterator)
{
  iterator_t *	iter1;
  iterator_t *	iter2;
  keycmp_t *	compar;
  value_t	key1, key2;
  link_t *	node1;
  link_t *	node2;
  int		v;


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

  compar = ((map_t *) (iter1->container))->keycmp;

  node1 = ucl_iterator_ptr(iter1);
  node2 = ucl_iterator_ptr(iter2);

  key1  = ucl_map_getkey(node1);
  key2  = ucl_map_getkey(node2);

  v = compar(key1, key2);
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

      v = compar(key1, key2);
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
