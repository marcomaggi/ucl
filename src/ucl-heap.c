/*
  Part of: Useless Containers Library
  Contents: heap implementation
  Date: Fri Feb 28, 2003

  Abstract



  Copyright (c) 2003-2005, 2008-2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** ------------------------------------------------------------
 ** Header files and type declarations.
 ** ----------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#include "ucl-internals.h"

#define SWAP(A, B)	do { void * tmp=(A); (A)=(B); (B)=tmp; } while(0)

#define KEY(N)		((N)->meta.t_int)


static __inline__ __attribute__((__always_inline__,__nonnull__))
void
size_incr (ucl_heap_t H)
{
  ++(H->size);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
size_decr (ucl_heap_t H)
{
  --(H->size);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_value_t
node_key (const ucl_heap_t H, void * N)
{
  return H->getkey.func(H->getkey.data, N);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_key_node (const ucl_heap_t H, ucl_value_t K, ucl_node_t N)
{
  return H->keycmp.func(H->keycmp.data, K, node_key(H, N));
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_key_key (const ucl_heap_t H, ucl_value_t K1, ucl_value_t K2)
{
  return H->keycmp.func(H->keycmp.data, K1, K2);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_node_node (const ucl_heap_t H, ucl_node_t N1, ucl_node_t N2)
{
  return H->keycmp.func(H->keycmp.data, node_key(H, N1), node_key(H, N2));
}

void
ucl_heap_initialise (ucl_heap_t H, ucl_comparison_t keycmp, ucl_node_getkey_t getkey)
{
  ucl_struct_clean(H, ucl_heap_tag_t);
  H->keycmp = keycmp;
  H->getkey = getkey;
}

void
ucl_heap_insert (ucl_heap_t H, void * new_)
{
  ucl_node_t	new = new_, dad;
  int		j;
  assert(H);
  assert(new);
  /* Handle the case of empty heap. */
  if (! H->root) {
    assert(! H->next);
    assert(! ucl_heap_size(H));
    H->root	= H->next = new;
    H->size	= 1;
    H->append_to_bro_of_next = false;
    return;
  }
  debug("inserting node with key %d (%p), root is %d, next is %d (%p)",
	KEY(new), new, KEY(H->root), KEY(H->next), H->next);
  /* Search the node that will be the dad of "new". */
  while (H->next->son && H->next->bro) {
    H->next = ucl_btree_step_levelorder(H->next);
    if (! H->next) {
      H->next = ucl_btree_find_leftmost(H->root);
      break;
    }
  }
  /* Append "new" as child of "H->next"  and leaf of the tree, first try
     as son. */
  new->dad = H->next;
  new->son = new->bro = NULL;
  if (H->next->son)
    H->next->bro = new;
  else
    H->next->son = new;
  size_incr(H);
  /* While  "new" is lesser  than its  dad: swap  "new" with  its "dad".
     Stop when "new" is greater or  equal to its dad or "new" has become
     the root of the tree.

     Notice  that  "H->next" must  be  updated  only  at the  first  two
     iterations,  after  those "new"  is  in  the  upper levels  of  the
     tree.  */
  for (j = 2, dad = new->dad; 0 < comparison_node_node(H, dad, new); --j, dad = new->dad) {
    switch (j) {
    case 2:
      H->next = new;
      break;
    case 1:
      H->next = dad;
      break;
    }
    ucl_btree_swap_no_meta(dad, new);
    if (dad == H->root) {
      H->root = new;
      assert(! H->root->dad);
      break;
    }
  }
}

void *
ucl_heap_extract (ucl_heap_t H)
{
  ucl_node_t	cur, dad, tmp;
  /* http://en.wikipedia.org/wiki/Binary_heap */
  assert(H);
  /* Handle empty heap. */
  if (! H->root) {
    assert(! H->size);
    assert(! H->next);
    return NULL;
  }
  debug("extracting root with key %d", KEY(H->root));
  /* Handle heap with single node. */
  if (1 == H->size) {
    assert(H->root == H->next);
    cur		= H->root;
    H->root	= H->next = NULL;
    H->size	= 0;
    return cur;
  }
  /* Push the root down the tree,  swapping it with the lesser among its
   * son and its bro.
   *
   *      lesser son                        lesser bro
   *
   *     dad          dad               dad           dad
   *      |            |                 |             |
   *     cur    =>    son               cur     =>    bro
   *     / \            \               / \           / \
   *  son   bro     cur  bro         son   bro     son   cur
   */
  cur = H->root;
  while (cur->son && cur->bro) {
    tmp = (0 > comparison_node_node(H, cur->son, cur->bro))? cur->son : cur->bro;
    debug("cur has both son and bro, swapping cur=%d with %s=%d",
	  KEY(cur), ((tmp == cur->son)? "son" : "bro"), KEY(tmp));
    ucl_btree_swap_no_meta(cur, tmp);
    if (H->root == cur)
      H->root = tmp;
  }
  /* Here only the  son or the bro are non-NULL, or  they are both NULL.
   * In the first  case join the dad with the only  child; in the second
   * case "cur" is a leaf, just detach it.
   *
   *     dad      dad           dad         dad
   *      |        |             |           |
   *     cur  =>  son           cur     =>  bro
   *     /                        \
   *  son         cur              bro      cur
   */
  dad = cur->dad;
  if (cur->son) { /* The son is the only child. */
    assert(! cur->bro);
    if (dad) {
      debug("joining dad=%d of cur=%d with son=%d", KEY(dad), KEY(cur), KEY(cur->son));
      cur->son->dad = dad;
      if (cur == dad->son)
	dad->son = cur->son;
      else
	dad->bro = cur->son;
    } else {
      assert(H->root == cur);
      debug("setting cur->son=%d as root and next", KEY(cur->son));
      H->root = cur->son;
      H->next = cur->son;
      cur->son->dad = NULL;
    }
  } else if (cur->bro) { /* The bro is the only child. */
    assert(! cur->son);
    if (dad) {
      debug("joining dad=%d of cur=%d with son=%d", KEY(dad), KEY(cur), KEY(cur->bro));
      cur->bro->dad = dad;
      if (cur == dad->son)
	dad->son = cur->bro;
      else
	dad->bro = cur->bro;
    } else {
      assert(H->root == cur);
      debug("setting cur->bro=%d as root and next", KEY(cur->bro));
      H->root = cur->bro;
      H->next = cur->bro;
      cur->bro->dad = NULL;
    }
  } else { /* "cur" is a leaf. */
    debug("detaching the leaf cur=%d with dad %p", KEY(cur), dad);
    if (dad) {
      if (cur == dad->son)
	dad->son = NULL;
      else
	dad->bro = NULL;
      if (H->next == cur)
	H->next = dad;
    } else {
      H->root = NULL;
      H->next = NULL;
    }
  }
  size_decr(H);
  debug("end of extraction: root is %d, next is %d (%p), size is %u",
	((H->root)? KEY(H->root) : -1), ((H->next)? KEY(H->next) : -1), H->next, ucl_heap_size(H));
  cur->dad = cur->son = cur->bro = NULL;
  return cur;
}

void
ucl_heap_merge (ucl_heap_t dst, ucl_heap_t src)
{
  ucl_node_t 	N;
  while ((N = ucl_heap_extract(src)))
    ucl_heap_insert(dst, N);
}

/* end of file */
