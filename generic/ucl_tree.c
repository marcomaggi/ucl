/* ucl_tree.c  --

   Part of: Useless Container Library
   Contents: code for the tree container.
   Date: early 2002

   Abstract:

	The implementation  is a  binary tree in  which: the son  of the
	node A  is the  first child of  the node  A, the brother  of the
	child is the second son of the node A, and so on.

	   -----
	  | dad |
	   -----
	    ^ |son
        dad | v
	   -----  bro   -----  bro   -----  bro   -----  bro
	  | 1st |----->| 2nd |----->| 3rd |----->| 4th |----->NULL
	   -----        -----        -----        -----
	     |            |
             v son        v son
	   -----        -----
	  | nep |      |     |
	   -----        -----

          Pointers condition meaning:

	node.broPtr == NULL  ->		the  node  has  no brothers,  so
					it's  the  last brother  between
					the children of its father;

	node.sonPtr == NULL  ->		the node has no children;

	node.dadPtr == NULL  ->		the node is  the root node  of a
					tree;

        A.dadPtr == B &&
	B.sonPtr == A        ->         A  is   the  first  between  the
			                children of node B;

        A.dadPtr == B &&
	B.broPtr == A        ->         A   and  B  are   brothers,  and
					children  of   the  same  parent
					node;
 

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

   $Id: ucl_tree.c,v 2.0.1.13 2003/12/09 14:06:40 marco Exp $

*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"

#ifdef DEBUGGING
#  include <stdio.h>
#endif


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_tree_node_t		node_t;
typedef ucl_btree_node_t	bnode_t;
typedef ucl_iterator_t		iterator_t;


static void	tree_inorder_iterator_next	(iterator_t *iterator);
static void	tree_preorder_iterator_next	(iterator_t *iterator);
static void	tree_postorder_iterator_next	(iterator_t *iterator);
/*
static void	tree_levelorder_iterator_next	(iterator_t *iterator);
*/



/* ucl_tree_is_dad --

	Test if a node is the father of another.

   Arguments:

	nodPtr -	pointer to the potential father of "*cldPtr"
	cldPtr -	pointer to the potential child of "*nodPtr"

   Results:

        Returns true if "*nodPtr" is the father of "*cldPtr".

   Side effects:

        None.

*/

int
ucl_tree_is_dad (node_t *nodPtr, node_t *cldPtr)
{
  assert(nodPtr != NULL);
  assert(cldPtr != NULL);

  if (ucl_tree_has_son(nodPtr))
    {
      nodPtr = ucl_tree_get_son(nodPtr);

      do
	{
	  if (nodPtr == cldPtr)
	    {
	      return 1;
	    }
	  else
	    {
	      nodPtr = ucl_tree_get_next(nodPtr);
	    }
	}
      while (nodPtr);
    }

  return 0;
}



/* ucl_tree_is_bro --

	Test if two nodes are brothers.

   Arguments:

	nodPtr -	pointer to a node
	broPtr -	pointer to the potential brother of "*nodPtr"

   Results:

        Returns true if "*nodPtr" is a brother of "*broPtr".

   Side effects:

        None.

*/

int
ucl_tree_is_bro (node_t *nodPtr, node_t *broPtr)
{
  node_t *	tmpPtr;


  /*
    Check if the potential brother is on the right of "*nodPtr".
  */

  tmpPtr = nodPtr->broPtr;
  while (tmpPtr)
    {
      if (tmpPtr == broPtr)
	{
	  return 1;
	}
      tmpPtr = tmpPtr->broPtr;
    }

  /*
    Check if the potential brother is on the left of "*nodPtr".
   */

  tmpPtr = nodPtr->dadPtr;
  while (tmpPtr && (tmpPtr->broPtr = nodPtr))
    {
      if (tmpPtr == broPtr)
	{
	  return 1;
	}
      nodPtr = tmpPtr;
      tmpPtr = nodPtr->dadPtr;
    }

  /*
    If we are here, the two are not brothers.
  */

  return 0;
}




/* ucl_tree_has_dad --

	Tests if a node has a parent.

	  Remember  that if the  "dadPtr" of  the node  is not  NULL, it
	doesn't mean that  the node has a parent: it  could be a brother
	to the left.

   Arguments:

	this -		pointer to a node

   Results:

        Returns true if the node has a parent, otherwise returns false.

   Side effects:

	None.

*/

int
ucl_tree_has_dad (node_t *nodPtr)
{
  assert(nodPtr);

  nodPtr = ucl_tree_get_first(nodPtr);
  return (nodPtr->dadPtr != NULL);
}



/* ucl_tree_has_prev --

	Tests if a node has a brother to the left.

	  Remember  that if the  "dadPtr" of  the node  is not  NULL, it
	doesn't mean that  the node has a brother to  the left: it could
	be the father of the node.

   Arguments:

	this -		pointer to a node

   Results:

        Returns true if the node has a brother to the left.

   Side effects:

	None.

*/

int
ucl_tree_has_prev (node_t *nodPtr)
{
  node_t *	dadPtr;


  assert(nodPtr);

  dadPtr = nodPtr->dadPtr;
  return ((dadPtr != NULL) &&
	  (dadPtr->broPtr == (node_t *) nodPtr))? 1 : 0;
}



/* ucl_tree_get_dad --

	Access the parent of a node.

   Arguments:

	this -		pointer to a node

   Results:

        Returns the  pointer to the parent,  or NULL if the  node has no
        parent.

   Side effects:

        None.

*/

node_t *
ucl_tree_get_dad (node_t *nodPtr)
{
  assert(nodPtr != NULL);

  nodPtr = ucl_tree_get_first(nodPtr);
  return (nodPtr->dadPtr)? nodPtr->dadPtr : NULL;
}



/* ucl_tree_get_prev --

	Access the left brother of a node.

   Arguments:

	this -		pointer to a node

   Results:

        Returns the pointer to the left brother, or NULL if the node has
        no left brother.

   Side effects:

        None.

*/

node_t *
ucl_tree_get_prev (node_t *nodPtr)
{
  assert(nodPtr != NULL);

  return (ucl_tree_has_prev(nodPtr))? nodPtr->dadPtr : NULL;
}



/* ucl_tree_get_first --

	Accesses the first brother of a node.

   Arguments:

	nodPtr -	pointer to a node structure

   Results:

        Returns a  poiter to  the first node  in the chain  of "*nodPtr"
        brothers. It can be "nodPtr" itself if "*nodPtr" is the first.

   Side effects:

        None.

*/

node_t *
ucl_tree_get_first (node_t *nodPtr)
{
  assert(nodPtr);

  while (nodPtr->dadPtr && (nodPtr->dadPtr->sonPtr != nodPtr))
    {
      nodPtr = nodPtr->dadPtr;
    }
  return nodPtr;
}


/* ucl_tree_get_last --

	Accesses the last brother of a node.

   Arguments:

	nodPtr -	poitner to a node structure

   Results:

        Returns  a poiter to  the last  node in  the chain  of "*nodPtr"
        brothers. It can be "nodPtr" itself if "*nodPtr" is the last.

   Side effects:

        None.

*/

node_t *
ucl_tree_get_last (node_t *nodPtr)
{
  assert(nodPtr);

  while (nodPtr->broPtr)
    {
      nodPtr = nodPtr->broPtr;
    }
  return nodPtr;
}



/* ucl_tree_insert_dad --

	Inserts a new dad for a node.

   Arguments:

	nodPtr -	pointer to a node structure
	dadPtr -	pointer to the new father node structure

   Results:

	The dad  node becomes the father  of the node and  the first son
	of the old "dad" (if any).

	  If the node is the first child of it's father:
	

		 ---------           ---------
		| *oldPtr |         | *oldPtr |
		 ---------           ---------
		   |  ^                |   ^
                son|  |dad          son|   |dad
                   v  |                v   |
		 ---------           ---------
		| *nodPtr |         | *dadPtr |
		 ---------           ---------
                                       |   ^
                                       |   |
                                       v   |
                                     ---------
                                    | *nodPtr |
                                     ---------

   Side effects:

	Some pointers are overwritten in the nodes.

*/

void
ucl_tree_insert_dad (node_t *nodPtr, node_t *dadPtr)
{
  node_t *	tmpPtr;


  nodPtr = ucl_tree_get_first(nodPtr);

  tmpPtr = nodPtr->dadPtr;
  nodPtr->dadPtr = dadPtr;
  dadPtr->sonPtr = nodPtr;

  dadPtr->dadPtr = tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->sonPtr = dadPtr;
    }
}


/* ucl_tree_insert_son --

	Inserts a new son for a node.

   Arguments:

	nodPtr -	pointer to a node structure
	sonPtr -	pointer to the new node structure

   Results:

	The new node becomes the last son of "*nodPtr".

		 -----       -----
		| cur |     | cur |
		 -----       -----
		   |     ->    |
		 -----       -----     -----
		| son |     | son |---| new |
		 -----       -----     -----

   Side effects:

	None.

*/

void
ucl_tree_insert_son (node_t * nodPtr, node_t * sonPtr)
{
  assert(nodPtr);
  assert(sonPtr);

  if (nodPtr->sonPtr)
    {
      nodPtr = nodPtr->sonPtr;
      while (nodPtr->broPtr)
	{
	  nodPtr = nodPtr->broPtr;
	}
      nodPtr->broPtr = sonPtr;
    }
  else
    {
      nodPtr->sonPtr = sonPtr;
    }
  sonPtr->dadPtr = nodPtr;  
}


/* ucl_tree_insert_next --

	Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| cur |-| bro | -> | cur |-| new |-| bro |
		 -----   -----      -----   -----   -----


   Arguments:

	nodPtr -	pointer to a node structure
	nxtPtr -	pointer to the new next brother

   Results:

	"*nxtPtr" is inserted as brother  to the right of "*nodPtr". The
	old brother to the right becomes brother of "*nxtPtr".

   Side effects:

	None.

*/

void
ucl_tree_insert_next (node_t * nodPtr, node_t * nxtPtr)
{
  node_t *	tmpPtr;


  assert(nodPtr);
  assert(nxtPtr);


  tmpPtr = nodPtr->broPtr;
  nodPtr->broPtr = nxtPtr;
  nxtPtr->dadPtr = nodPtr;

  nxtPtr->broPtr = tmpPtr;
  if (tmpPtr)
    {
      tmpPtr->dadPtr = nxtPtr;
    }
}


/* ucl_tree_insert_prev --

	Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| bro |-| cur | -> | bro |-| new |-| cur |
		 -----   -----      -----   -----   -----


   Arguments:

	nodPtr -	pointer to a node structure
	prvPtr -	pointer to the new next brother

   Results:

	"*prvPtr" is inserted as brother  to the left of "*nodPtr". The
	old brother to the left becomes brother of "*prvPtr".

   Side effects:

	None.

*/

void
ucl_tree_insert_prev (node_t * nodPtr, node_t * prvPtr)
{
  node_t *	tmpPtr;


  assert(nodPtr);
  assert(prvPtr);


  tmpPtr = nodPtr->dadPtr;
  nodPtr->dadPtr = prvPtr;
  prvPtr->broPtr = nodPtr;

  prvPtr->dadPtr = tmpPtr;
  if (tmpPtr)
    {
      if (tmpPtr->broPtr == nodPtr)
	{
	  tmpPtr->broPtr = prvPtr;
	}
      else
	{
	  tmpPtr->sonPtr = prvPtr;
	}
    }
}



/* ucl_tree_extract_dad --

	Extracts the dad of a node from the tree.

         ---    ---    ---       ---    ---    ---    ---
        | A |--|dad|--| B |     | A |--|nod|--| C |--| B |
         ---    ---    ---       ---    ---    ---    ---
                 |          ->
                ---    ---              ---
	       |nod|--| C |            |dad|
	        ---    ---              ---

   Arguments:

	nodPtr -	pointer to the node structure

   Results:

        The current node and all it's brothers are inserted in the place
        of the dad node.

	  Returns  a pointer  to  the  extracted node,  or  NULL if  the
	selected node  has nod  dad. All the  pointers in  the extracted
	node structure are reset to NULL.

   Side effects:

        None.

*/

node_t *
ucl_tree_extract_dad (node_t *nodPtr)
{
  node_t *	dadPtr;
  node_t *	frstPtr;
  node_t *	lastPtr;


  assert(nodPtr);

  frstPtr = ucl_tree_get_first(nodPtr);
  dadPtr  = frstPtr->dadPtr;
  if (dadPtr)
    {
      lastPtr = ucl_tree_get_last(nodPtr);

      frstPtr->dadPtr = dadPtr->dadPtr;
      if (frstPtr->dadPtr)
	{
	  if (frstPtr->dadPtr->sonPtr == dadPtr)
	    {
	      frstPtr->dadPtr->sonPtr = frstPtr;
	    }
	  else
	    {
	      frstPtr->dadPtr->broPtr = frstPtr;
	    }
	}

      lastPtr->broPtr = dadPtr->broPtr;
      if (lastPtr->broPtr)
	{
	  lastPtr->broPtr->dadPtr = lastPtr;
	}

      memset(dadPtr, '\0', sizeof(node_t));
    }

  return dadPtr;
}


/* ucl_tree_extract_son --

	Extracts the son of a node from the tree.

	    ---               ---
	   |nod|             |nod|
            ---               ---
             |                 |
            ---    ---        ---    ---    ---
           |son|--| C |  ->  | A |--| B |--| C |
            ---    ---        ---    ---    ---
             |
            ---    ---            ---
           | A |--| B |          |son|
            ---    ---            ---


   Arguments:

	nodPtr -	pointer to a node structure

   Results:

        The   son  of   the  selected   node  is   extracted   from  the
        hierarchy. All of its  children becomes children of the selected
        node.

	  Returns  a  pointer to  the  extracted  node  or NULL  if  the
	selected node has no son. All the pointers in the extracted node
	structure are reset to NULL.

   Side effects:

        None.

*/

node_t *
ucl_tree_extract_son (node_t *nodPtr)
{
  node_t *	sonPtr;
  node_t *	lastPtr;


  assert(nodPtr);

  sonPtr = nodPtr->sonPtr;
  if (sonPtr)
    {
      nodPtr->sonPtr = sonPtr->sonPtr;
      if (nodPtr->sonPtr)
	{
	  nodPtr->sonPtr->dadPtr = nodPtr;

	  lastPtr = ucl_tree_get_last(nodPtr->sonPtr);
	  lastPtr->broPtr = sonPtr->broPtr;
	  if (lastPtr->broPtr)
	    {
	      lastPtr->broPtr->dadPtr = lastPtr;
	    }
	}
      else if (sonPtr->broPtr)
	{
	  nodPtr->sonPtr = sonPtr->broPtr;
	  nodPtr->sonPtr->dadPtr = nodPtr;
	}

      memset(sonPtr, '\0', sizeof(node_t));
    }

  return sonPtr;
}



/* ucl_tree_extract_prev --

	Extracts the left brother of a node.

            ---               ---
           | A |             | A |
            ---               ---
             |                 |
            ---    ---        ---    ---
           |prv|--|nod|  ->  | B |--|nod|
            ---    ---        ---    ---
             |
            ---                   ---
           | B |                 |prv|
            ---                   ---


   Arguments:

	nodPtr -	pointer to a node structure

   Results:

        The  left brother  of the  selected node  is extracted  from the
        hierarchy. Its  children becomes  left brothers of  the selected
        node.

	  Returns  a pointer  to  the  extracted node,  or  NULL if  the
	selected  node has  no left  brother.  All the  pointers in  the
	extracted node structure are reset to NULL.

   Side effects:

        None.

*/

node_t *
ucl_tree_extract_prev (node_t *nodPtr)
{
  node_t *	prvPtr;
  node_t *	dadPtr;
  node_t *	sonPtr;


  assert(nodPtr);

  prvPtr = nodPtr->dadPtr;
  if (prvPtr)
    {
      if (prvPtr->broPtr != nodPtr)
	{
	  return NULL;
	}

      dadPtr = prvPtr->dadPtr;
      sonPtr = prvPtr->sonPtr;

      if (dadPtr && sonPtr)
	{
	  if (dadPtr->sonPtr == prvPtr)
	    {
	      dadPtr->sonPtr = sonPtr;
	    }
	  else
	    {
	      dadPtr->broPtr = sonPtr;
	    }
	  sonPtr->dadPtr = dadPtr;

	  sonPtr = ucl_tree_get_last(sonPtr);
	  sonPtr->broPtr = nodPtr;
	  nodPtr->dadPtr = sonPtr;
	}
      else if (dadPtr) /* sonPtr == NULL */
	{
	  if (dadPtr->sonPtr == prvPtr)
	    {
	      dadPtr->sonPtr = nodPtr;
	    }
	  else
	    {
	      dadPtr->broPtr = nodPtr;
	    }
	  nodPtr->dadPtr = dadPtr;
	}
      else if (sonPtr) /* dadPtr == NULL */
	{
	  sonPtr->dadPtr = NULL;

	  sonPtr = ucl_tree_get_last(sonPtr);
	  sonPtr->broPtr = nodPtr;
	  nodPtr->dadPtr = sonPtr;
	}
      else /* dadPtr == NULL && sonPtr == NULL */
	{
	  nodPtr->dadPtr = NULL;
	}

      memset(prvPtr, '\0', sizeof(node_t));
    }

  return prvPtr;
}



/* ucl_tree_extract_next --

	Extracts the right brother of a node.

             ---    ---    ---       ---    ---    ---
            |nod|--|nxt|--| A |     |nod|--| B |--| A |
             ---    ---    ---       ---    ---    ---
                     |           ->
                    ---                 ---
                   | B |               |nxt|
                    ---                 ---

   Arguments:

	nodPtr -	pointer to a node structure

   Results:

        The right  brother of  the selected node  is extracted  from the
        hierarchy. Its  children becomes right brothers  of the selected
        node.

	  Returns  a pointer  to  the  extracted node,  or  NULL if  the
	selected node  has no  right brother.  All  the pointers  in the
	extracted node structure are reset to NULL.

   Side effects:

        None.

*/

node_t *
ucl_tree_extract_next (node_t *nodPtr)
{
  node_t *	nxtPtr;
  node_t *	broPtr;
  node_t *	sonPtr;


  assert(nodPtr);

  nxtPtr = nodPtr->broPtr;
  if (nxtPtr)
    {
      broPtr = nxtPtr->broPtr;
      sonPtr = nxtPtr->sonPtr;

      if (sonPtr && broPtr)
	{
	  nodPtr->broPtr = sonPtr;
	  sonPtr->dadPtr = nodPtr;

	  sonPtr = ucl_tree_get_last(sonPtr);
	  sonPtr->broPtr = broPtr;
	  broPtr->dadPtr = sonPtr;
	}
      else if (sonPtr) /* broPtr == NULL */
	{
	  nodPtr->broPtr = sonPtr;
	  sonPtr->dadPtr = nodPtr;
	}
      else if (broPtr) /* sonPtr == NULL */
	{
	  nodPtr->broPtr = broPtr;
	  broPtr->dadPtr = nodPtr;
	}
      else /* sonPtr == NULL && broPtr == NULL */
	{
	  nodPtr->broPtr = NULL;
	}

      memset(nxtPtr, '\0', sizeof(node_t));
    }

  return nxtPtr;
}


/* ucl_tree_iterator_inorder --

	Initialises an inorder iteration.

   Arguments:

	nodPtr -	pointer to a node structure
	iterator -	pointer to the iterator structure

   Results:

        The members  of the iterator  structure are initialised.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no extract operations must be performed).

*/

void
ucl_tree_iterator_inorder (node_t * nodPtr, iterator_t *iterator)
{
  assert(nodPtr);
  assert(iterator);


  iterator->container	= nodPtr;
  iterator->iterator	= ucl_btree_find_leftmost((bnode_t *) nodPtr);
  iterator->next	= tree_inorder_iterator_next;
}



/* ucl_tree_iterator_preorder --

	Initialises a preorder iteration.

   Arguments:

	nodPtr -	pointer to a node structure
	iterator -	pointer to the iterator structure

   Results:

        The members  of the iterator  structure are initialised.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no extract operations must be performed).

*/

void
ucl_tree_iterator_preorder (node_t *nodPtr, iterator_t *iterator)
{
  assert(nodPtr);
  assert(iterator);


  iterator->container	= nodPtr;
  iterator->iterator	= nodPtr;
  iterator->next	= tree_preorder_iterator_next;
}


/* ucl_tree_iterator_postorder --

	Initialises a postorder iterator.

   Arguments:

	nodPtr -	pointer to the node structure
	iterator -	pointer to the iterator structure

   Results:

        The members  of the iterator  structure are initialised.

   Side effects:

	There's no need for the  caller to destroy the iterator's object
	at the end  of the iteration.  This is  not a "robust iterator",
	that is: while  the iteration is going on  the internal state of
	the container must not change (no new elements must be added and
	no extract operations must be performed).

*/

void
ucl_tree_iterator_postorder (node_t *nodPtr, iterator_t *iterator)
{
  assert(nodPtr);
  assert(iterator);


  iterator->container	= nodPtr;
  iterator->iterator	= ucl_btree_find_deepest_son((bnode_t *) nodPtr);
  iterator->next	= tree_postorder_iterator_next;
}



/* tree_inorder_iterator_next --

	Advances an inorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        The "iterator" member is updated.  If the iteration is over it's
        set  to NULL, and  the next  call to  "ucl_iterator_more()" will
        return false.

   Side effects:

        None.

*/

static void
tree_inorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_inorder(iterator->iterator);
}



/* tree_preorder_iterator_next --

	Advances a preorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        The "iterator" member is updated.  If the iteration is over it's
        set  to NULL, and  the next  call to  "ucl_iterator_more()" will
        return false.


   Side effects:

        None.

*/

static void
tree_preorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_preorder(iterator->iterator);
}



/* tree_postorder_iterator_next --

	Advances a postorder iteration.

   Arguments:

	iterator -	pointer to the iterator structure

   Results:

        The "iterator" member is updated.  If the iteration is over it's
        set  to NULL, and  the next  call to  "ucl_iterator_more()" will
        return false.

   Side effects:

        None.

*/

static void
tree_postorder_iterator_next (iterator_t *iterator)
{
  iterator->iterator = ucl_btree_step_postorder(iterator->iterator);
}


/* end of file */
