/*
  Part of: Useless Containers Library
  Contents: unbalanced binary search tree
  Date: Mon Dec  6, 2010

  Abstract



  Copyright (c) 2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** -----------------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#include "ucl-internals.h"


static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_value_t
node_key (ucl_node_getkey_t getkey, void * N)
{
  return getkey.func(getkey.data, N);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_key_key (ucl_comparison_t keycmp, ucl_value_t K1, ucl_value_t K2)
{
  return keycmp.func(keycmp.data, K1, K2);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_key_node (ucl_comparison_t keycmp, ucl_node_getkey_t getkey, ucl_value_t K, ucl_node_t N)
{
  return keycmp.func(keycmp.data, K, node_key(getkey, N));
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
comparison_node_node (ucl_comparison_t keycmp, ucl_node_getkey_t getkey, ucl_node_t N1, ucl_node_t N2)
{
  return keycmp.func(keycmp.data, node_key(getkey, N1), node_key(getkey, N2));
}

void
ucl_bst_insert (void * top_, void * new_, ucl_comparison_t keycmp, ucl_node_getkey_t getkey)
{
  ucl_node_t	cur = top_, new = new_;
  for (;;) {
    if (0 <= comparison_node_node(keycmp, getkey, new, cur)) {
      if (cur->bro)
	cur = cur->bro;
      else {
	cur->bro = new;
	new->dad = cur;
	return;
      }
    } else {
      if (cur->son) {
	cur = cur->son;
      } else {
	cur->son = new;
	new->dad = cur;
	return;
      }
    }
  }
}

void *
ucl_bst_find (void * top_, ucl_value_t K, ucl_comparison_t keycmp, ucl_node_getkey_t getkey)
{
  for (ucl_node_t cur = top_;;) {
    int	comparison_result = comparison_key_node(keycmp, getkey, K, cur);
    if (0 < comparison_result) {
      if (cur->bro)
	cur = cur->bro;
      else
	return NULL;
    } else if (0 > comparison_result) {
      if (cur->son)
	cur = cur->son;
      else
	return NULL;
    } else {
      assert(0 == comparison_result);
      return cur;
    }
  }
}

void *
ucl_bst_delete (void * root_, void * cur_)
{
  ucl_node_t	root = root_, cur = cur_, dad, son, bro, new_root = NULL;
  if (!cur->son && !cur->bro) {
    debug("deleting node with no son and no bro");
    /* Example:
     *
     *     root       root
     *      |     =>
     *     cur
     */
    dad = cur->dad;
    if (dad) {
      if (cur == dad->son)
	dad->son = NULL;
      else
	dad->bro = NULL;
    }
    new_root = (cur == root)? NULL : root;
  } else if (cur->son && cur->bro) {
    debug("deleting node with both son and bro");
    /* Example:
     *
     *    root          root
     *     |             |
     *    cur--bro  =>  son--bro
     *     |
     *    son
     */
    dad = cur->dad;
    son = cur->son;
    if (dad) {
      if (cur == dad->son)
	dad->son = son;
      else
	dad->bro = son;
    }
    son->dad = dad;
    while (son->bro)
      son = son->bro;
    son->bro = cur->bro;
    new_root = (cur == root)? cur->son : root;
  } else if (cur->son) {
    assert(NULL == cur->bro);
    debug("deleting node with son only");
    /* Example:
     *
     *    root      root
     *     |         |
     *    cur   =>  son
     *     |
     *    son
     */
    dad = cur->dad;
    son = cur->son;
    if (dad) {
      if (cur == dad->son)
	dad->son = son;
      else
	dad->bro = son;
    }
    son->dad = dad;
    new_root = (cur == root)? son : root;
  } else {
    debug("deleting node with bro only");
    assert(cur->bro);
    assert(NULL == cur->son);
    /* Example:
     *
     *    root--cur--bro  =>  root--bro
     *
     */
    dad = cur->dad;
    bro = cur->bro;
    if (dad) {
      debug("cur is dad's son %d", cur == dad->son);
      if (cur == dad->son)
	dad->son = bro;
      else
	dad->bro = bro;
    }
    bro->dad = dad;
    new_root = (cur == root)? bro : root;
  }
  cur->dad = cur->son = cur->bro = NULL;
  return new_root;
}

/* end of file */
