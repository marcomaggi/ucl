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

  Copyright (c) 2001-2005, 2007-2010, 2015, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#include "ucl-internals.h"


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


/** ------------------------------------------------------------
 ** Testing relationships.
 ** ----------------------------------------------------------*/

__attribute__((__nonnull__)) ucl_bool_t
ucl_tree_is_dad (void * _nod, void * _cld)
{
  ucl_node_t nod = _nod;
  ucl_node_t cld = _cld;
  if (ucl_btree_ref_son(nod)) {
    nod = ucl_btree_ref_son(nod);
    do {
      if (nod == cld)
	return 1;
      else
	nod = ucl_btree_ref_bro(nod);
    } while (nod);
  }
  return 0;
}
__attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_is_bro (void * _nod, void * _bro)
{
  ucl_node_t	nod = _nod;
  ucl_node_t	bro   = _bro;
  ucl_node_t 	tmp;
  /* Check if the potential brother is on the right of "*nod". */
  tmp = nod->bro;
  while (tmp) {
    if (tmp == bro)
      return 1;
    tmp = tmp->bro;
  }
  /* Check if the potential brother is on the left of "*nod". */
  tmp = nod->dad;
  while (tmp && (tmp->bro == nod)) {
    if (tmp == bro)
      return 1;
    nod = tmp;
    tmp = nod->dad;
  }
  /* If we are here, the two are not brothers. */
  return 0;
}
/* Remember that if  the "dad" of the node is not  NULL, it doesn't mean
   that the node has a parent: it could be a brother to the left. */
__attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_has_dad (void * _nod)
{
  ucl_node_t nod = _nod;
  nod = ucl_tree_ref_first(nod);
  return (nod->dad != NULL);
}
/* Remember that if  the "dad" of the node is not  NULL, it doesn't mean
   that the node  has a brother to  the left: it could be  the father of
   the node. */
__attribute__((__nonnull__,__pure__)) ucl_bool_t
ucl_tree_has_prev (void * _nod)
{
  ucl_node_t	nod = _nod;
  ucl_node_t 	dad;
  dad = nod->dad;
  return ((dad != NULL) && (dad->bro == nod));
}


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

__attribute__((__nonnull__,__pure__)) void *
ucl_tree_ref_dad (void * _nod)
{
  ucl_node_t	nod = _nod;
  ucl_node_t 	tmp;
  tmp = ucl_tree_ref_first(nod);
  return (tmp->dad)? tmp->dad : NULL;
}
__attribute__((__nonnull__,__pure__)) void *
ucl_tree_ref_prev (void * _nod)
{
  ucl_node_t	nod = _nod;
  return (ucl_tree_has_prev(nod))? nod->dad : NULL;
}
__attribute__((__nonnull__,__pure__)) void *
ucl_tree_ref_first (void * _nod)
{
  ucl_node_t	nod = _nod;
  while (nod->dad && (nod->dad->son != nod))
    nod = nod->dad;
  return nod;
}
__attribute__((__nonnull__,__pure__)) void *
ucl_tree_ref_last (void * _nod)
{
  ucl_node_t	nod = _nod;
  while (nod->bro)
    nod = nod->bro;
  return (ucl_node_t )nod;
}


/* Insert a new dad for a node.   The dad node becomes the father of the
   node and the first son of the old "dad" (if any).  If the node is the
   first child of it's father:

                 -----           -----
                | old |         | old |
                 -----           -----
                  | ^             | ^
               son| |dad       son| |dad
                  v |             v |
                 -----           -----
                | nod |         | dad |
                 -----           -----
                                  | ^
                                  | |
                                  v |
                                 -----
                                | nod |
				 -----
*/

void
ucl_tree_insert_dad (void * _nod, void * _dad)
{
  ucl_node_t	nod = _nod;
  ucl_node_t	dad = _dad;
  ucl_node_t 	tmp;
  nod = ucl_tree_ref_first(nod);
  tmp = nod->dad;
  nod->dad = dad;
  dad->son = nod;
  dad->dad = tmp;
  if (tmp)
    tmp->son = dad;
}


/* Inserts a new son  for a node.  The new node becomes  the last son of
   "nod".

		 -----       -----
		| cur |     | cur |
		 -----       -----
		   |     ->    |
		 -----       -----     -----
		| son |     | son |---| new |
		 -----       -----     -----
*/
void
ucl_tree_insert_son (void * _nod, void * _son)
{
  ucl_node_t	nod = _nod;
  ucl_node_t	son = _son;
  assert(nod);
  assert(son);
  if (nod->son) {
    nod = nod->son;
    while (nod->bro)
      nod = nod->bro;
    nod->bro = son;
  } else {
    nod->son = son;
  }
  son->dad = nod;
}


/* Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| cur |-| bro | -> | cur |-| new |-| bro |
		 -----   -----      -----   -----   -----

   "*nxt" is  inserted as brother to  the right of  "*nod".  The old
   brother to the right becomes brother of "*nxt".
*/

void
ucl_tree_insert_next (void * _nod, void * _nxt)
{
  ucl_node_t	nod = _nod;
  ucl_node_t	nxt = _nxt;
  ucl_node_t 	tmp;
  assert(nod);
  assert(nxt);
  tmp = nod->bro;
  nod->bro = nxt;
  nxt->dad = nod;
  nxt->bro = tmp;
  if (tmp)
    tmp->dad = nxt;
}


/* Inserts a new brother for a node.

		 -----   -----      -----   -----   -----
		| bro |-| cur | -> | bro |-| new |-| cur |
		 -----   -----      -----   -----   -----

   "*prv" is  inserted as  brother to the  left of "*nod".   The old
   brother to the left becomes brother of "*prv".
*/

void
ucl_tree_insert_prev (void * _nod, void * _prv)
{
  ucl_node_t	nod = _nod;
  ucl_node_t	prv = _prv;
  ucl_node_t 	tmp;
  assert(nod);
  assert(prv);
  tmp = nod->dad;
  nod->dad = prv;
  prv->bro = nod;
  prv->dad = tmp;
  if (tmp) {
    if (tmp->bro == nod)
      tmp->bro = prv;
    else
      tmp->son = prv;
  }
}


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

void *
ucl_tree_extract_dad (void * _nod)
{
  ucl_node_t	nod = _nod;
  ucl_node_t 	dad;
  ucl_node_t 	frst;
  ucl_node_t 	last;
  assert(nod);
  frst = ucl_tree_ref_first(nod);
  dad  = frst->dad;
  if (dad) {
    last = ucl_tree_ref_last(nod);
    frst->dad = dad->dad;
    if (frst->dad) {
      if (frst->dad->son == dad)
	frst->dad->son = frst;
      else
	frst->dad->bro = frst;
    }
    last->bro = dad->bro;
    if (last->bro)
      last->bro->dad = last;
    memset(dad, '\0', sizeof(ucl_node_tag_t));
  }
  return dad;
}


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

void *
ucl_tree_extract_son (void * _nod)
{
  ucl_node_t	nod = _nod;
  ucl_node_t 	son;
  ucl_node_t 	last;
  assert(nod);
  son = nod->son;
  if (son) {
    nod->son = son->son;
    if (nod->son) {
      nod->son->dad = nod;

      last = ucl_tree_ref_last(nod->son);
      last->bro = son->bro;
      if (last->bro)
	last->bro->dad = last;
    } else if (son->bro) {
      nod->son = son->bro;
      nod->son->dad = nod;
    }
    memset(son, '\0', sizeof(ucl_node_tag_t));
  }
  return son;
}


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

void *
ucl_tree_extract_prev (void * _nod)
{
  ucl_node_t	nod = _nod;
  ucl_node_t 	prv;
  ucl_node_t 	dad;
  ucl_node_t 	son;
  assert(nod);
  prv = nod->dad;
  if (prv) {
    if (prv->bro != nod)
      return NULL;
    dad = prv->dad;
    son = prv->son;
    if (dad && son) {
      if (dad->son == prv)
	dad->son = son;
      else
	dad->bro = son;
      son->dad = dad;
      son = ucl_tree_ref_last(son);
      son->bro = nod;
      nod->dad = son;
    } else if (dad) { /* son == NULL */
      if (dad->son == prv)
	dad->son = nod;
      else
	dad->bro = nod;
      nod->dad = dad;
    } else if (son) { /* dad == NULL */
      son->dad = NULL;
      son = ucl_tree_ref_last(son);
      son->bro = nod;
      nod->dad = son;
    } else /* dad == NULL && son == NULL */
      nod->dad = NULL;
    memset(prv, '\0', sizeof(ucl_node_tag_t));
  }
  return prv;
}


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

void *
ucl_tree_extract_next (void * _nod)
{
  ucl_node_t	nod = _nod;
  ucl_node_t 	nxt;
  ucl_node_t 	bro;
  ucl_node_t 	son;
  assert(nod);
  nxt = nod->bro;
  if (nxt) {
    bro = nxt->bro;
    son = nxt->son;
    if (son && bro) {
      nod->bro = son;
      son->dad = nod;
      son = ucl_tree_ref_last(son);
      son->bro = bro;
      bro->dad = son;
    } else if (son) { /* bro == NULL */
      nod->bro = son;
      son->dad = nod;
    } else if (bro) { /* son == NULL */
      nod->bro = bro;
      bro->dad = nod;
    } else /* son == NULL && bro == NULL */
      nod->bro = NULL;
    memset(nxt, '\0', sizeof(ucl_node_tag_t));
  }
  return nxt;
}


/** ------------------------------------------------------------
 ** Iterators.
 ** ----------------------------------------------------------*/

void
ucl_tree_iterator_inorder (void * _nod, ucl_iterator_t iterator)
{
  ucl_node_t	nod = _nod;
  assert(nod);
  assert(iterator);
  iterator->container	= nod;
  iterator->iterator	= ucl_btree_find_leftmost(nod);
  iterator->next	= tree_inorder_iterator_next;
}
void
ucl_tree_iterator_preorder (void * _nod, ucl_iterator_t iterator)
{
  ucl_node_t	nod = _nod;
  assert(nod);
  assert(iterator);
  iterator->container	= nod;
  iterator->iterator	= (void *)nod;
  iterator->next	= tree_preorder_iterator_next;
}
void
ucl_tree_iterator_postorder (void * _nod, ucl_iterator_t iterator)
{
  ucl_node_t	nod = _nod;
  assert(nod);
  assert(iterator);
  iterator->container	= nod;
  iterator->iterator	= ucl_btree_find_deepest_son(nod);
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
