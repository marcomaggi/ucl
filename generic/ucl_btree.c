/* ucl_btree.c --
   
   Part of: Useless Containers Library (UCL)
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
   
   
   Copyright (c) 2002, 2003 Marco Maggi
   
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
   
   $Id: ucl_btree.c,v 1.1.1.11 2003/12/09 10:21:16 marco Exp $
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef  ucl_btree_node_t		Node;


/* ucl_btree_constructor --

	Construct an already allocated btree node.

   Arguments:

	thisPtr -	pointer to the node, it can't be NULL
	dadPtr -	pointer to the uplevel node or NULL

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_btree_constructor (Node *thisPtr, Node *dadPtr)
{
  assert(thisPtr != NULL);

  thisPtr->dadPtr	= dadPtr;
  thisPtr->sonPtr	= NULL;
  thisPtr->broPtr	= NULL;
}


/* ucl_btree_find_leftmost --

	Find the leftmost  node in the subtree of  the supplied node. To
	do this, we  traverse the tree choosing always  the "son" of the
	current node.

	  Example:
   
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8

        starting from "5"  the selected node is "1",  starting from "10"
        the selected node is "6".

   Arguments:

	nodePtr -	pointer to the starting node, it can't be NULL

   Results:

        Returns a pointer  to the leftmost node, or  "nodePtr" itself if
        this is the leftmost.

   Side effects:

        None.

*/

Node *
ucl_btree_find_leftmost (Node *nodePtr)
{
  assert(nodePtr != NULL);

  while (nodePtr->sonPtr != NULL)
    {
      nodePtr = nodePtr->sonPtr;
    }
  return nodePtr;
}


/* ucl_btree_find_rigthmost --

	Find the rightmost node in  the subtree of the supplied node. To
	do this, we  traverse the tree choosing always  the "brother" of
	the current node.

	  Example:
   
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8

	the selected node is "12".

   Arguments:

	nodePtr -	pointer to the starting node

   Results:

        Returns a pointer to the  rightmost node, or "nodePtr" itself if
        this is the rightmost.

   Side effects:

        None.

*/

Node *
ucl_btree_find_rightmost (Node *nodePtr)
{
  assert(nodePtr != NULL);

  while (nodePtr->broPtr != NULL)
    {
      nodePtr = nodePtr->broPtr;
    }
  return nodePtr;
}


/* ucl_btree_find_deepest_son --

	Find the  deepest leftmost son  in a subtree. This  is different
	from  ucl_btree_find_leftmost().

	  Example:
   
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8

	the selected node is "2", while the leftmost node is "1".

   Arguments:

	nodePtr -	pointer to the root node of the search, it can't
			be NULL

   Results:

        Returns a pointer  to the deepest son node,  or "nodePtr" itself
        if this is the deepest son.

   Side effects:

        none

*/

Node *
ucl_btree_find_deepest_son (Node *nodePtr)
{
  assert(nodePtr != NULL);
  
  do
    {
      while (nodePtr->sonPtr != NULL)
	{
	  nodePtr = nodePtr->sonPtr;
	}
      if (nodePtr->broPtr != NULL)
	{
	  nodePtr = nodePtr->broPtr;
	}
    }
  while ((nodePtr->sonPtr != NULL) || (nodePtr->broPtr != NULL));

  return nodePtr;
}


/* ucl_btree_step_inorder --

	Advance an  inorder iteration.

	  Inorder iteration:  visit all the  nodes from the  leftmost to
	the rightmost. Given a node in a btree, this function performs a
	single  step.

	  Example:
   
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8

	the order is:  0, 1, 2, 3, 4, 5,  6, 7, 8, 9, 10,  11, 12.

	  To perform  a complete  iteration, we have  to start  from the
	leftmost node  ("0" in the example), already  visited, and begin
	from there. Example:

		Node *	cur;

		cur	= select_a_btree(...);
		cur	= ucl_btree_find_leftmost(cur);

		while (cur != NULL);
		  {
		    makes_use_of_node(cur);
		    cur = ucl_btree_step_inorder(cur);
		  }

	  To restrict the iteration to a subtree of a tree or to a range
	of nodes in  a tree, we have to select the  first and last nodes
	and check when the iterator reaches the last.

	  Example of subtree restriction:

		Node *	cur, end;

		cur	= select_a_node(...);
		end	= ucl_btree_find_rightmost(cur);

		cur	= ucl_btree_find_leftmost(cur);
		makes_use_of_node(cur);

		while (cur != end)
		  {
		    cur = ucl_btree_step_inorder(cur);
		    makes_use_of_node(cur);
		  }

	"cur" can't be  NULL because "end" is in the  subtree of the top
	node; this code should work even if: cur == end.

	  Example of range restriction:

		Node *	root, cur, end;

		root	= select_a_node(...);
		cur	= select_first(root, ...);
		end	= ucl_btree_find_rightmost(root);

		makes_use_of_node(cur);

		while (cur != end)
		  {
		    cur = ucl_btree_step_inorder(cur);
		    makes_use_of_node(cur);
		  }

	"cur" can't be  NULL since we selected the  first and last nodes
	in a subtree; this code should work even if: root == cur == end.

   Arguments:

	curPtr -	pointer to the current node in the iteration,
			it can't be NULL

   Results:

        Returns a pointer to the next  node in the iteration, or NULL if
        the iteration is over.

   Side effects:

        If the iteration is already over nothing happens.

*/

Node *
ucl_btree_step_inorder (Node *curPtr)
{
  Node *	dadPtr;


  assert(curPtr != NULL);

  if (curPtr->broPtr != NULL)
    {
      /* There's  a brother, so go to  it and find the  leftmost node in
         the subtree. */

      curPtr = curPtr->broPtr;
      while (curPtr->sonPtr != NULL)
	{
	  curPtr = curPtr->sonPtr;
	}
      return curPtr;
    }
  else
    {
      /* If  there's no brother step  up to the father;  if we're coming
	 from the brother of the father we have to step up again because
	 this  father has  been already  visited.

	 If the node  has NULL father or we're coming  from the son, the
	 step is complete:  we return the father. The  iteration is over
	 if the father is NULL. */

      dadPtr = curPtr->dadPtr;
      while ((dadPtr != NULL) && (dadPtr->broPtr == curPtr))
	{
	  curPtr	= dadPtr;
	  dadPtr	= curPtr->dadPtr;
	}
      return dadPtr;
    }
}

/* ucl_btree_step_inorder_backward --

	Advance a backward inorder iteration.

	  Backward  inorder  iteration: visit  all  the  nodes from  the
	rightmost  to the  leftmost.   Given  a node  in  a btree,  this
	function performs a single step.

	  Example:
   
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8

	the order is: 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0.

	  To perform  a complete  iteration, we have  to start  from the
	rightmost node ("12" in the example), already visited, and begin
	from there. Example:

		Node *	cur;

		cur	= select_a_btree(...);
		cur	= ucl_btree_find_rightmost(cur);

		while (cur != NULL);
		  {
		    makes_use_of_node(cur);
		    cur = ucl_btree_step_inorder_backward(cur);
		  }

	  To restrict the iteration to a subtree of a tree or to a range
	of nodes in  a tree, we have to select the  first and last nodes
	and check when the iterator reaches the last.

	  Example of subtree restriction:

		Node *	cur, end;

		cur	= select_a_node(...);
		end	= ucl_btree_find_leftmost(cur);

		cur	= ucl_btree_find_rightmost(cur);
		makes_use_of_node(cur);

		while (cur != end)
		  {
		    cur = ucl_btree_step_inorder_backward(cur);
		    makes_use_of_node(cur);
		  }

	"cur" can't be  NULL because "end" is in the  subtree of the top
	node; this code should work even if: cur == end.

	  Example of range restriction:

		Node *	root, cur, end;

		root	= select_a_node(...);
		cur	= select_first(root, ...);
		end	= ucl_btree_find_leftmost(root);

		makes_use_of_node(cur);

		while (cur != end)
		  {
		    cur = ucl_btree_step_inorder_backward(cur);
		    makes_use_of_node(cur);
		  }

	"cur" can't be  NULL since we selected the  first and last nodes
	in a subtree; this code should work even if: root == cur == end.

   Arguments:

	curPtr -	pointer to the current node in the iteration,
			it can't be NULL

   Results:

        Returns a pointer to the next  node in the iteration, or NULL if
        the iteration is over.

   Side effects:

        If the iteration is already over nothing happens.

*/

Node *
ucl_btree_step_inorder_backward (Node *curPtr)
{
  Node *	dadPtr;


  assert(curPtr != NULL);

  /*  If there's  a son  go to  it and  find the  rightmost node  in the
     subtree. */

  if (curPtr->sonPtr != NULL)
    {
      curPtr = curPtr->sonPtr;
      while (curPtr->broPtr)
	{
	  curPtr = curPtr->broPtr;
	}
      return curPtr;
    }
  else  
    {
      /* If there's  no son step up to the father;  if we're coming from
	 the son  of the father  we have to  step up again  because this
	 father has been already visited.
     
	 If the node  has NULL father or we're  coming from the brother,
	 the step  is complete: we  return the father. The  iteration is
	 over if the father is NULL. */

      dadPtr = curPtr->dadPtr;
      while ((dadPtr != NULL)  && (dadPtr->sonPtr == curPtr))
	{
	  curPtr	= dadPtr;
	  dadPtr	= curPtr->dadPtr;
	}
      return dadPtr;
    }
}

/* ucl_btree_step_preorder --

	Advance a preorder iteration.

	  Preorder iteration:  visit the current node then  the son then
	the brother.  Given a node in  a btree this  function performs a
	single step.

	  Example:

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  
	the order is: 5,  1, 0, 3, 2, 4, 10, 7, 6, 9,  8, 12, 11. It's a
	worm that always turns "right".

	  To perform a complete iteration, we have to start from the top
	node  of the  tree ("5"  in the  example), already  visited, and
	begin from there. Example:

		Node *	cur;

		cur	= select_a_btree(...);

		while (cur != NULL);
		  {
		    makes_use_of_node(cur);
		    cur = ucl_btree_step_pre(cur);
		  }

	  To restrict the  iteration to a subtree of a  tree, we have to
	check when the iterator reaches it the top node. Example:

		Node *	cur, end;

		end = cur = select_a_node(...);

		makes_use_of_node(cur);
		cur = ucl_btree_step_preorder(cur);

		while (cur != end);
		  {
		    makes_use_of_node(cur);
		    cur = ucl_btree_step_preorder(cur);
		  }

   Arguments:

	curPtr -	pointer to the current node in the iteration

   Results:

        Returns a pointer to the next node in the preorder iteration, or
        NULL if the iteration is over.

   Side effects:

        None.

*/

Node *
ucl_btree_step_preorder (Node *curPtr)
{
  Node *	dadPtr;


  /* Visit the son, if any. */

  if (curPtr->sonPtr != 0)
    {
      return curPtr->sonPtr;
    }

  /* If no son: visit the brother, if any. */

  else if (curPtr->broPtr != 0)
    {
      return curPtr->broPtr;
    }

  /* No son and no brother so step up to the father:

     - if there's no father, ends the iteration;

     - if  coming from the father's  son visit the  father's brother, if
       any;

     - if coming from the father's brother step up and loop.
  */

  else
    {
      for (;;)
	{
	  dadPtr = curPtr->dadPtr;

	  if (dadPtr == NULL)
	    {
	      break;
	    }
	  else if ((dadPtr->sonPtr == curPtr) && (dadPtr->broPtr != NULL))
	    {
	      return dadPtr->broPtr;
	    }
	  else
	    {
	      curPtr = dadPtr;
	    }
	}
    }
  return dadPtr;
}

/* ucl_btree_step_postorder --

	Advance a postorder iteration.

	  Postorder iteration: visit the son, then the brother, then the
	father node.

	  Example:

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  
	the order is: 2, 4, 3, 1, 6, 8, 9, 7, 11, 12, 10, 5.

	  To perform a complete iteration, we have to select the deepest
	leftmost son  in the  tree ("2" in  the example) and  begin from
	there. Example:

		Node *	cur;

		cur	= select_a_node(...);
		cur	= ucl_btree_find_deepest_son(cur);

		while (cur != NULL);
		  {
		    makes_use_of_node(cur);
		    cur = ucl_btree_step_postorder(cur);
		  }

	  To restrict the  iteration to a subtree of a  tree, we have to
	check when the iterator reaches it the top node. Example:

		Node *	cur, end;

		cur = end = select_a_node(...);

		makes_use_of_node(cur);
		cur = ucl_btree_step_postorder(cur);

		while (cur != end);
		  {
		    cur = ucl_btree_step_postorder(cur);
		    makes_use_of_node(cur);
		  }

	"cur" can't be null in the loop.

   Arguments:

	curPtr -	pointer to the current node in the iteration

   Results:

        Returns a pointer  to the next node in  the postorder iteration,
        or NULL if the iteration is over.

   Side effects:

        None.

*/

Node *
ucl_btree_step_postorder (Node *curPtr)
{
  Node *	dadPtr;


  dadPtr = curPtr->dadPtr;
  if (dadPtr == NULL)
    {
      return dadPtr;
    }
  else if ((dadPtr->broPtr) && (dadPtr->broPtr != curPtr))
    {
      return ucl_btree_find_deepest_son(dadPtr->broPtr);
    }
  else
    {
      return dadPtr;
    }
}

/* ucl_btree_step_levelorder --

	Advance a levelorder iteration.

	  Level order iteration: visit the tree level by level.

	  Example:
  
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8

	the order is: 5, 1, 10, 0, 3, 7, 12, 2, 4, 6, 9, 11, 8

	  We need a moving cursor  that always "turns right" keeping the
	count of the level.

	  To perform  a complete iteration,  we just call  this function
	until it returns NULL. Example:

		Node *	cur;

		cur	= select_a_btree(...);

		while (cur != NULL);
		  {
		    makes_use_of_node(cur);
		    cur = ucl_btree_step_levelorder(cur);
		  }

   Arguments:

	curPtr -	pointer to the current node in the iteration,
			it can't be NULL

   Results:

        Returns a pointer to the  next node in the levelorder iteration,
        or NULL if the iteration is over.

   Side effects:

        None.

*/

Node *
ucl_btree_step_levelorder (Node *curPtr)
{
  int		i=0;
  Node		*orgPtr, *lastPtr;


  assert(curPtr != NULL);

  if ((curPtr->dadPtr == NULL)
      && (curPtr->sonPtr == NULL)
      && (curPtr->broPtr == NULL))
    {
      return NULL;
    }

  orgPtr = curPtr;

  for (;;)
    {
      if (curPtr->dadPtr)
	{
	  lastPtr = curPtr;
	  curPtr = curPtr->dadPtr;
	  ++i;
	  if (curPtr->broPtr && curPtr->broPtr != lastPtr)
	    {
	      lastPtr = curPtr;
	      curPtr = curPtr->broPtr;
	      --i;
	      if (i == 0)
		{
		  return curPtr;
		}
	      if (curPtr == orgPtr && !curPtr->sonPtr && !curPtr->broPtr)
		{
		  return 0;
		}
	      while (curPtr->sonPtr || curPtr->broPtr)
		{
		  if (curPtr->sonPtr)
		    {
		      lastPtr = curPtr;
		      curPtr = curPtr->sonPtr;
		    }
		  else if (curPtr->broPtr)
		    {
		      lastPtr = curPtr;
		      curPtr = curPtr->broPtr;
		    }
		  --i;
		  if (i == 0)
		    {
		      return curPtr;
		    }
		  if (curPtr == orgPtr && !curPtr->sonPtr && !curPtr->broPtr)
		    {
		      return 0;
		    }
		}
	    }
	}
      else
	{
	  ++i;
	  while (curPtr->sonPtr || curPtr->broPtr)
	    {
	      if (curPtr->sonPtr)
		{
		  lastPtr = curPtr;
		  curPtr = curPtr->sonPtr;
		}
	      else if (curPtr->broPtr)
		{
		  lastPtr = curPtr;
		  curPtr = curPtr->broPtr;
		}
	      --i;
	      if (i == 0)
		{
		  return curPtr;
		}
	      if ((curPtr == orgPtr) && (!curPtr->sonPtr) && (!curPtr->broPtr))
		{
		  return 0;
		}
	    }
	}
    }
  return curPtr;
}

/* end of file */
