/*
   Part of: Useless Containers Library
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

	node.bro == NULL  ->		the  node  has  no brothers,  so
					it's  the  last brother  between
					the children of its father;

	node.son == NULL  ->		the node has no children;

	node.dad == NULL  ->		the node is  the root node  of a
					tree;

        A.dad == B &&
	B.son == A        ->         A  is   the  first  between  the
			                children of node B;

        A.dad == B &&
	B.bro == A        ->         A   and  B  are   brothers,  and
					children  of   the  same  parent
					node;

   Copyright (c) 2001-2005, 2007-2010 Marco Maggi <marcomaggi@gna.org>

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


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

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

__attribute__((__nonnull__)) ucl_bool_t
ucl_tree_is_dad (ucl_node_t nod_p, ucl_node_t cld_p)
{
  if (ucl_btree_ref_son(nod_p))
    {
      nod_p = ucl_btree_ref_son(nod_p);

      do
	{
	  if (nod_p == cld_p)
	    return 1;
	  else
	    nod_p = ucl_btree_ref_bro(nod_p);
	}
      while (nod_p);
    }

  return 0;
}
__attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_is_bro (ucl_node_t nod_p, ucl_node_t bro)
{
  ucl_node_t 	tmp_p;


  /* Check if the potential brother is on the right of "*nod_p". */
  tmp_p = nod_p->bro;
  while (tmp_p)
    {
      if (tmp_p == bro)
	return 1;
      tmp_p = tmp_p->bro;
    }

  /* Check if the potential brother is on the left of "*nod_p". */
  tmp_p = nod_p->dad;
  while (tmp_p && (tmp_p->bro == nod_p))
    {
      if (tmp_p == bro)
	return 1;
      nod_p = tmp_p;
      tmp_p = nod_p->dad;
    }

  /* If we are here, the two are not brothers. */
  return 0;
}
/* Remember that if the "dad" of the node is not NULL, it doesn't mean
   that the node has a parent: it could be a brother to the left. */
__attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_has_dad (ucl_node_t nod_p)
{
  nod_p = ucl_tree_get_first(nod_p);
  return (nod_p->dad != NULL);
}
/* Remember that if the "dad" of the node is not NULL, it doesn't mean
   that the node  has a brother to  the left: it could be  the father of
   the node. */
__attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_has_prev (ucl_node_t nod_p)
{
  ucl_node_t 	dad;

  dad = nod_p->dad;
  return ((dad != NULL) && (dad->bro == nod_p));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

__attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_dad (void * _nod_p)
{
  ucl_node_t	nod_p = _nod_p;
  ucl_node_t 	tmp_p;

  tmp_p = ucl_tree_get_first(nod_p);
  return (tmp_p->dad)? tmp_p->dad : NULL;
}
__attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_prev (void * _nod_p)
{
  ucl_node_t	nod_p = _nod_p;

  return (ucl_tree_has_prev(nod_p))? nod_p->dad : NULL;
}
__attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_first (void * _nod_p)
{
  ucl_node_t	nod_p = _nod_p;

  while (nod_p->dad && (nod_p->dad->son != nod_p))
    {
      nod_p = nod_p->dad;
    }
  return nod_p;
}
__attribute__((__nonnull__,__pure__)) void *
ucl_tree_get_last (void * _nod_p)
{
  ucl_node_t	nod_p = _nod_p;

  while (nod_p->bro)
    {
      nod_p = nod_p->bro;
    }
  return (ucl_node_t )nod_p;
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
		| *nod_p |         | *dad |
		 ---------           ---------
                                       |   ^
                                       |   |
                                       v   |
                                     ---------
                                    | *nod_p |
                                     ---------

*/

void
ucl_tree_insert_dad (ucl_node_t nod_p, ucl_node_t dad)
{
  ucl_node_t 	tmp_p;


  nod_p = ucl_tree_get_first(nod_p);

  tmp_p = nod_p->dad;
  nod_p->dad = dad;
  dad->son = nod_p;

  dad->dad = tmp_p;
  if (tmp_p)
    {
      tmp_p->son = dad;
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

void
ucl_tree_insert_son (ucl_node_t  nod_p, ucl_node_t  son)
{
  assert(nod_p);
  assert(son);

  if (nod_p->son)
    {
      nod_p = nod_p->son;
      while (nod_p->bro)
	{
	  nod_p = nod_p->bro;
	}
      nod_p->bro = son;
    }
  else
    {
      nod_p->son = son;
    }
  son->dad = nod_p;
}

/* ------------------------------------------------------------ */


/* Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| cur |-| bro | -> | cur |-| new |-| bro |
		 -----   -----      -----   -----   -----

   "*nxt_p" is  inserted as brother to  the right of  "*nod_p".  The old
   brother to the right becomes brother of "*nxt_p".
*/

void
ucl_tree_insert_next (ucl_node_t  nod_p, ucl_node_t  nxt_p)
{
  ucl_node_t 	tmp_p;


  assert(nod_p);
  assert(nxt_p);

  tmp_p = nod_p->bro;
  nod_p->bro = nxt_p;
  nxt_p->dad = nod_p;

  nxt_p->bro = tmp_p;
  if (tmp_p)
    {
      tmp_p->dad = nxt_p;
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

void
ucl_tree_insert_prev (ucl_node_t  nod_p, ucl_node_t  prv_p)
{
  ucl_node_t 	tmp_p;


  assert(nod_p);
  assert(prv_p);

  tmp_p = nod_p->dad;
  nod_p->dad = prv_p;
  prv_p->bro = nod_p;

  prv_p->dad = tmp_p;
  if (tmp_p)
    {
      if (tmp_p->bro == nod_p)
	{
	  tmp_p->bro = prv_p;
	}
      else
	{
	  tmp_p->son = prv_p;
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

ucl_node_t
ucl_tree_extract_dad (ucl_node_t nod_p)
{
  ucl_node_t 	dad;
  ucl_node_t 	frst_p;
  ucl_node_t 	last_p;


  assert(nod_p);

  frst_p = ucl_tree_get_first(nod_p);
  dad  = frst_p->dad;
  if (dad)
    {
      last_p = ucl_tree_get_last(nod_p);

      frst_p->dad = dad->dad;
      if (frst_p->dad)
	{
	  if (frst_p->dad->son == dad)
	    {
	      frst_p->dad->son = frst_p;
	    }
	  else
	    {
	      frst_p->dad->bro = frst_p;
	    }
	}

      last_p->bro = dad->bro;
      if (last_p->bro)
	{
	  last_p->bro->dad = last_p;
	}

      memset(dad, '\0', sizeof(ucl_node_t));
    }

  return dad;
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

ucl_node_t
ucl_tree_extract_son (ucl_node_t nod_p)
{
  ucl_node_t 	son;
  ucl_node_t 	last_p;


  assert(nod_p);

  son = nod_p->son;
  if (son)
    {
      nod_p->son = son->son;
      if (nod_p->son)
	{
	  nod_p->son->dad = nod_p;

	  last_p = ucl_tree_get_last(nod_p->son);
	  last_p->bro = son->bro;
	  if (last_p->bro)
	    {
	      last_p->bro->dad = last_p;
	    }
	}
      else if (son->bro)
	{
	  nod_p->son = son->bro;
	  nod_p->son->dad = nod_p;
	}

      memset(son, '\0', sizeof(ucl_node_t));
    }

  return son;
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

ucl_node_t
ucl_tree_extract_prev (ucl_node_t nod_p)
{
  ucl_node_t 	prv_p;
  ucl_node_t 	dad;
  ucl_node_t 	son;


  assert(nod_p);

  prv_p = nod_p->dad;
  if (prv_p)
    {
      if (prv_p->bro != nod_p)
	{
	  return NULL;
	}

      dad = prv_p->dad;
      son = prv_p->son;

      if (dad && son)
	{
	  if (dad->son == prv_p)
	    {
	      dad->son = son;
	    }
	  else
	    {
	      dad->bro = son;
	    }
	  son->dad = dad;

	  son = ucl_tree_get_last(son);
	  son->bro = nod_p;
	  nod_p->dad = son;
	}
      else if (dad) /* son == NULL */
	{
	  if (dad->son == prv_p)
	    {
	      dad->son = nod_p;
	    }
	  else
	    {
	      dad->bro = nod_p;
	    }
	  nod_p->dad = dad;
	}
      else if (son) /* dad == NULL */
	{
	  son->dad = NULL;

	  son = ucl_tree_get_last(son);
	  son->bro = nod_p;
	  nod_p->dad = son;
	}
      else /* dad == NULL && son == NULL */
	{
	  nod_p->dad = NULL;
	}

      memset(prv_p, '\0', sizeof(ucl_node_t));
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

ucl_node_t
ucl_tree_extract_next (ucl_node_t nod_p)
{
  ucl_node_t 	nxt_p;
  ucl_node_t 	bro;
  ucl_node_t 	son;


  assert(nod_p);

  nxt_p = nod_p->bro;
  if (nxt_p)
    {
      bro = nxt_p->bro;
      son = nxt_p->son;

      if (son && bro)
	{
	  nod_p->bro = son;
	  son->dad = nod_p;

	  son = ucl_tree_get_last(son);
	  son->bro = bro;
	  bro->dad = son;
	}
      else if (son) /* bro == NULL */
	{
	  nod_p->bro = son;
	  son->dad = nod_p;
	}
      else if (bro) /* son == NULL */
	{
	  nod_p->bro = bro;
	  bro->dad = nod_p;
	}
      else /* son == NULL && bro == NULL */
	{
	  nod_p->bro = NULL;
	}

      memset(nxt_p, '\0', sizeof(ucl_node_t));
    }

  return nxt_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Iterators.
 ** ----------------------------------------------------------*/

void
ucl_tree_iterator_inorder (ucl_node_t  nod_p, ucl_iterator_t iterator)
{
  assert(nod_p);
  assert(iterator);


  iterator->container	= nod_p;
  iterator->iterator	= ucl_btree_find_leftmost(nod_p);
  iterator->next	= tree_inorder_iterator_next;
}
void
ucl_tree_iterator_preorder (ucl_node_t nod_p, ucl_iterator_t iterator)
{
  assert(nod_p);
  assert(iterator);


  iterator->container	= nod_p;
  iterator->iterator	= (void *)nod_p;
  iterator->next	= tree_preorder_iterator_next;
}
void
ucl_tree_iterator_postorder (ucl_node_t nod_p, ucl_iterator_t iterator)
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
