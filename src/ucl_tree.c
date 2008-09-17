/* 
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

	node.bro_p == NULL  ->		the  node  has  no brothers,  so
					it's  the  last brother  between
					the children of its father;

	node.son_p == NULL  ->		the node has no children;

	node.dad_p == NULL  ->		the node is  the root node  of a
					tree;

        A.dad_p == B &&
	B.son_p == A        ->         A  is   the  first  between  the
			                children of node B;

        A.dad_p == B &&
	B.bro_p == A        ->         A   and  B  are   brothers,  and
					children  of   the  same  parent
					node;
 

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

#define stubmodule		tree

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_btree_node_t	bnode_t;
typedef ucl_iterator_t		iterator_t;


static ucl_iterator_next_t	tree_inorder_iterator_next;
static ucl_iterator_next_t	tree_preorder_iterator_next;
static ucl_iterator_next_t	tree_postorder_iterator_next;
/*
static ucl_iterator_next_t	tree_levelorder_iterator_next;
*/

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Testing relationships.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-15-09) __attribute__((__nonnull__)) ucl_bool_t
ucl_tree_is_dad (ucl_btree_node_t nod_p, ucl_btree_node_t cld_p)
{
  if (ucl_btree_getson(nod_p))
    {
      nod_p = ucl_btree_getson(nod_p);

      do
	{
	  if (nod_p == cld_p)
	    return 1;
	  else
	    nod_p = ucl_btree_getbro(nod_p);
	}
      while (nod_p);
    }

  return 0;
}
stub(2005-09-23-18-15-12) __attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_is_bro (ucl_btree_node_t nod_p, ucl_btree_node_t bro_p)
{
  ucl_btree_node_t 	tmp_p;


  /* Check if the potential brother is on the right of "*nod_p". */
  tmp_p = nod_p->bro_p;
  while (tmp_p)
    {
      if (tmp_p == bro_p)
	return 1;
      tmp_p = tmp_p->bro_p;
    }

  /* Check if the potential brother is on the left of "*nod_p". */
  tmp_p = nod_p->dad_p;
  while (tmp_p && (tmp_p->bro_p == nod_p))
    {
      if (tmp_p == bro_p)
	return 1;
      nod_p = tmp_p;
      tmp_p = nod_p->dad_p;
    }

  /* If we are here, the two are not brothers. */
  return 0;
}
/* Remember that if the "dad_p" of the node is not NULL, it doesn't mean
   that the node has a parent: it could be a brother to the left. */
stub(2005-09-23-18-15-15) __attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_has_dad (ucl_btree_node_t nod_p)
{
  nod_p = ucl_tree_get_first(nod_p);
  return (nod_p->dad_p != NULL);
}
/* Remember that if the "dad_p" of the node is not NULL, it doesn't mean
   that the node  has a brother to  the left: it could be  the father of
   the node. */
stub(2005-09-23-18-15-18) __attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_has_prev (ucl_btree_node_t nod_p)
{
  ucl_btree_node_t 	dad_p;

  dad_p = nod_p->dad_p;
  return ((dad_p != NULL) && (dad_p->bro_p == nod_p));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-15-21) __attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_dad (void * _nod_p)
{
  ucl_btree_node_t	nod_p = _nod_p;
  ucl_btree_node_t 	tmp_p;

  tmp_p = ucl_tree_get_first(nod_p);
  return (tmp_p->dad_p)? tmp_p->dad_p : NULL;
}
stub(2005-09-23-18-15-24) __attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_prev (void * _nod_p)
{
  ucl_btree_node_t	nod_p = _nod_p;

  return (ucl_tree_has_prev(nod_p))? nod_p->dad_p : NULL;
}
stub(2005-09-23-18-15-27) __attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_first (void * _nod_p)
{
  ucl_btree_node_t	nod_p = _nod_p;

  while (nod_p->dad_p && (nod_p->dad_p->son_p != nod_p))
    {
      nod_p = nod_p->dad_p;
    }
  return nod_p;
}
stub(2005-09-23-18-15-30) __attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_last (void * _nod_p)
{
  ucl_btree_node_t	nod_p = _nod_p;

  while (nod_p->bro_p)
    {
      nod_p = nod_p->bro_p;
    }
  return (ucl_btree_node_t )nod_p;
}

/* ------------------------------------------------------------ */


/* Inserts a new dad for a node.  The dad node becomes the father of the
   node and the first son of the old "dad" (if any).

   If the node is the first child of it's father:
	

		 ---------           ---------
		| *old_p |         | *old_p |
		 ---------           ---------
		   |  ^                |   ^
                son|  |dad          son|   |dad
                   v  |                v   |
		 ---------           ---------
		| *nod_p |         | *dad_p |
		 ---------           ---------
                                       |   ^
                                       |   |
                                       v   |
                                     ---------
                                    | *nod_p |
                                     ---------

*/

stub(2005-09-23-18-15-33) void
ucl_tree_insert_dad (ucl_btree_node_t nod_p, ucl_btree_node_t dad_p)
{
  ucl_btree_node_t 	tmp_p;


  nod_p = ucl_tree_get_first(nod_p);

  tmp_p = nod_p->dad_p;
  nod_p->dad_p = dad_p;
  dad_p->son_p = nod_p;

  dad_p->dad_p = tmp_p;
  if (tmp_p)
    {
      tmp_p->son_p = dad_p;
    }
}

/* ------------------------------------------------------------ */


/* Inserts a new son  for a node.  The new node becomes  the last son of
   "*nod_p".

		 -----       -----
		| cur |     | cur |
		 -----       -----
		   |     ->    |
		 -----       -----     -----
		| son |     | son |---| new |
		 -----       -----     -----

*/

stub(2005-09-23-18-15-36) void
ucl_tree_insert_son (ucl_btree_node_t  nod_p, ucl_btree_node_t  son_p)
{
  assert(nod_p);
  assert(son_p);

  if (nod_p->son_p)
    {
      nod_p = nod_p->son_p;
      while (nod_p->bro_p)
	{
	  nod_p = nod_p->bro_p;
	}
      nod_p->bro_p = son_p;
    }
  else
    {
      nod_p->son_p = son_p;
    }
  son_p->dad_p = nod_p;  
}

/* ------------------------------------------------------------ */


/* Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| cur |-| bro | -> | cur |-| new |-| bro |
		 -----   -----      -----   -----   -----

   "*nxt_p" is  inserted as brother to  the right of  "*nod_p".  The old
   brother to the right becomes brother of "*nxt_p".
*/

stub(2005-09-23-18-15-40) void
ucl_tree_insert_next (ucl_btree_node_t  nod_p, ucl_btree_node_t  nxt_p)
{
  ucl_btree_node_t 	tmp_p;


  assert(nod_p);
  assert(nxt_p);

  tmp_p = nod_p->bro_p;
  nod_p->bro_p = nxt_p;
  nxt_p->dad_p = nod_p;

  nxt_p->bro_p = tmp_p;
  if (tmp_p)
    {
      tmp_p->dad_p = nxt_p;
    }
}

/* ------------------------------------------------------------ */


/* Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| bro |-| cur | -> | bro |-| new |-| cur |
		 -----   -----      -----   -----   -----

   "*prv_p" is  inserted as  brother to the  left of "*nod_p".   The old
   brother to the left becomes brother of "*prv_p".
*/

stub(2005-09-23-18-15-43) void
ucl_tree_insert_prev (ucl_btree_node_t  nod_p, ucl_btree_node_t  prv_p)
{
  ucl_btree_node_t 	tmp_p;


  assert(nod_p);
  assert(prv_p);

  tmp_p = nod_p->dad_p;
  nod_p->dad_p = prv_p;
  prv_p->bro_p = nod_p;

  prv_p->dad_p = tmp_p;
  if (tmp_p)
    {
      if (tmp_p->bro_p == nod_p)
	{
	  tmp_p->bro_p = prv_p;
	}
      else
	{
	  tmp_p->son_p = prv_p;
	}
    }
}

/* ------------------------------------------------------------ */


/* Extracts the dad of a node from the tree.

         ---    ---    ---       ---    ---    ---    ---
        | A |--|dad|--| B |     | A |--|nod|--| C |--| B |
         ---    ---    ---       ---    ---    ---    ---
                 |          ->
                ---    ---              ---
	       |nod|--| C |            |dad|
	        ---    ---              ---

   The current node  and all it's brothers are inserted  in the place of
   the dad node.

   Returns a pointer to the extracted node, or NULL if the selected node
   has nod  dad. All  the pointers in  the extracted node  structure are
   reset to NULL.
*/

stub(2005-09-23-18-15-46) ucl_btree_node_t 
ucl_tree_extract_dad (ucl_btree_node_t nod_p)
{
  ucl_btree_node_t 	dad_p;
  ucl_btree_node_t 	frst_p;
  ucl_btree_node_t 	last_p;


  assert(nod_p);

  frst_p = ucl_tree_get_first(nod_p);
  dad_p  = frst_p->dad_p;
  if (dad_p)
    {
      last_p = ucl_tree_get_last(nod_p);

      frst_p->dad_p = dad_p->dad_p;
      if (frst_p->dad_p)
	{
	  if (frst_p->dad_p->son_p == dad_p)
	    {
	      frst_p->dad_p->son_p = frst_p;
	    }
	  else
	    {
	      frst_p->dad_p->bro_p = frst_p;
	    }
	}

      last_p->bro_p = dad_p->bro_p;
      if (last_p->bro_p)
	{
	  last_p->bro_p->dad_p = last_p;
	}

      memset(dad_p, '\0', sizeof(ucl_btree_node_t));
    }

  return dad_p;
}

/* ------------------------------------------------------------ */


/* Extracts the son of a node from the tree.

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

   The son of the selected node  is extracted from the hierarchy. All of
   its children becomes children of the selected node.

   Returns a pointer to the extracted  node or NULL if the selected node
   has  no son. All  the pointers  in the  extracted node  structure are
   reset to NULL.
*/

stub(2005-09-23-18-15-49) ucl_btree_node_t 
ucl_tree_extract_son (ucl_btree_node_t nod_p)
{
  ucl_btree_node_t 	son_p;
  ucl_btree_node_t 	last_p;


  assert(nod_p);

  son_p = nod_p->son_p;
  if (son_p)
    {
      nod_p->son_p = son_p->son_p;
      if (nod_p->son_p)
	{
	  nod_p->son_p->dad_p = nod_p;

	  last_p = ucl_tree_get_last(nod_p->son_p);
	  last_p->bro_p = son_p->bro_p;
	  if (last_p->bro_p)
	    {
	      last_p->bro_p->dad_p = last_p;
	    }
	}
      else if (son_p->bro_p)
	{
	  nod_p->son_p = son_p->bro_p;
	  nod_p->son_p->dad_p = nod_p;
	}

      memset(son_p, '\0', sizeof(ucl_btree_node_t));
    }

  return son_p;
}

/* ------------------------------------------------------------ */


/* Extracts the left brother of a node.

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

   The  left  brother  of  the  selected  node  is  extracted  from  the
   hierarchy. Its children becomes left brothers of the selected node.

   Returns a pointer to the extracted node, or NULL if the selected node
   has  no  left  brother.   All  the pointers  in  the  extracted  node
   structure are reset to NULL.
*/

stub(2005-09-23-18-15-52) ucl_btree_node_t 
ucl_tree_extract_prev (ucl_btree_node_t nod_p)
{
  ucl_btree_node_t 	prv_p;
  ucl_btree_node_t 	dad_p;
  ucl_btree_node_t 	son_p;


  assert(nod_p);

  prv_p = nod_p->dad_p;
  if (prv_p)
    {
      if (prv_p->bro_p != nod_p)
	{
	  return NULL;
	}

      dad_p = prv_p->dad_p;
      son_p = prv_p->son_p;

      if (dad_p && son_p)
	{
	  if (dad_p->son_p == prv_p)
	    {
	      dad_p->son_p = son_p;
	    }
	  else
	    {
	      dad_p->bro_p = son_p;
	    }
	  son_p->dad_p = dad_p;

	  son_p = ucl_tree_get_last(son_p);
	  son_p->bro_p = nod_p;
	  nod_p->dad_p = son_p;
	}
      else if (dad_p) /* son_p == NULL */
	{
	  if (dad_p->son_p == prv_p)
	    {
	      dad_p->son_p = nod_p;
	    }
	  else
	    {
	      dad_p->bro_p = nod_p;
	    }
	  nod_p->dad_p = dad_p;
	}
      else if (son_p) /* dad_p == NULL */
	{
	  son_p->dad_p = NULL;

	  son_p = ucl_tree_get_last(son_p);
	  son_p->bro_p = nod_p;
	  nod_p->dad_p = son_p;
	}
      else /* dad_p == NULL && son_p == NULL */
	{
	  nod_p->dad_p = NULL;
	}

      memset(prv_p, '\0', sizeof(ucl_btree_node_t));
    }

  return prv_p;
}

/* ------------------------------------------------------------ */


/* Extracts the right brother of a node.

             ---    ---    ---       ---    ---    ---
            |nod|--|nxt|--| A |     |nod|--| B |--| A |
             ---    ---    ---       ---    ---    ---
                     |           ->
                    ---                 ---
                   | B |               |nxt|
                    ---                 ---

   The  right  brother  of  the  selected node  is  extracted  from  the
   hierarchy. Its children becomes right brothers of the selected node.

   Returns a pointer to the extracted node, or NULL if the selected node
   has  no  right brother.   All  the  pointers  in the  extracted  node
   structure are reset to NULL.
*/

stub(2005-09-23-18-15-55) ucl_btree_node_t 
ucl_tree_extract_next (ucl_btree_node_t nod_p)
{
  ucl_btree_node_t 	nxt_p;
  ucl_btree_node_t 	bro_p;
  ucl_btree_node_t 	son_p;


  assert(nod_p);

  nxt_p = nod_p->bro_p;
  if (nxt_p)
    {
      bro_p = nxt_p->bro_p;
      son_p = nxt_p->son_p;

      if (son_p && bro_p)
	{
	  nod_p->bro_p = son_p;
	  son_p->dad_p = nod_p;

	  son_p = ucl_tree_get_last(son_p);
	  son_p->bro_p = bro_p;
	  bro_p->dad_p = son_p;
	}
      else if (son_p) /* bro_p == NULL */
	{
	  nod_p->bro_p = son_p;
	  son_p->dad_p = nod_p;
	}
      else if (bro_p) /* son_p == NULL */
	{
	  nod_p->bro_p = bro_p;
	  bro_p->dad_p = nod_p;
	}
      else /* son_p == NULL && bro_p == NULL */
	{
	  nod_p->bro_p = NULL;
	}

      memset(nxt_p, '\0', sizeof(ucl_btree_node_t));
    }

  return nxt_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Iterators.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-15-58) void
ucl_tree_iterator_inorder (ucl_btree_node_t  nod_p, ucl_iterator_t iterator)
{
  assert(nod_p);
  assert(iterator);


  iterator->container	= nod_p;
  iterator->iterator	= ucl_btree_find_leftmost(nod_p);
  iterator->next	= tree_inorder_iterator_next;
}
stub(2005-09-23-18-16-01) void
ucl_tree_iterator_preorder (ucl_btree_node_t nod_p, ucl_iterator_t iterator)
{
  assert(nod_p);
  assert(iterator);


  iterator->container	= nod_p;
  iterator->iterator	= (void *)nod_p;
  iterator->next	= tree_preorder_iterator_next;
}
stub(2005-09-23-18-16-04) void
ucl_tree_iterator_postorder (ucl_btree_node_t nod_p, ucl_iterator_t iterator)
{
  assert(nod_p);
  assert(iterator);


  iterator->container	= nod_p;
  iterator->iterator	= ucl_btree_find_deepest_son( nod_p);
  iterator->next	= tree_postorder_iterator_next;
}
static void
tree_inorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_inorder(iterator->iterator);
}
static void
tree_preorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_preorder(iterator->iterator);
}
static void
tree_postorder_iterator_next (iterator_t iterator)
{
  iterator->iterator = ucl_btree_step_postorder(iterator->iterator);
}


/* end of file */
