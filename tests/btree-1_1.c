/* btree-1_1.c --
   
   Part of: Useless Containers Library
   Contents: test for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Test  the  hierarchy  builder  functions and  the  generic  data
	accessor function.
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
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
 ** Headers.
 ** ----------------------------------------------------------*/

#include "btreetest.h"

typedef struct node_tag_t {
  ucl_btree_node_tag_t	node;
  char			c;
} node_tag_t;
typedef node_tag_t *	node_t;

ucl_memory_allocator_t	allocator = {
  .data = NULL, .alloc = ucl_memory_alloc
};

static node_t
node_make (char c)
{
  node_t        p = NULL;

  allocator.alloc(allocator.data, &p, sizeof(node_tag_t));
  p->c = c;
  return p;
}
static void
node_final (node_t p)
{
  allocator.alloc(allocator.data, &p, 0);
}
static __inline__ void
node_clean (node_t p)
{
  ucl_struct_clean(p, node_tag_t);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Test.
 ** ----------------------------------------------------------*/

void
test (void)
{
  node_t	a, b, c, d;


  a = node_make('a');
  b = node_make('b');
  c = node_make('c');
  d = node_make('d');


  /*
    Build hierarchy

		 ---  bro  ---
		| a |---->| c |
		 ---       ---
	          |         |
	      son v     son v
                 ---       ---
                | b |     | d |
                 ---       ---
  */


  ucl_btree_dadson(a, b);
  ucl_btree_dadbro(a, c);
  ucl_btree_dadson(c, d);

  assert((a == ucl_btree_getdad(b)) && (b == ucl_btree_getson(a)));
  assert((a == ucl_btree_getdad(c)) && (c == ucl_btree_getbro(a)));
  assert((c == ucl_btree_getdad(d)) && (d == ucl_btree_getson(c)));
  assert(ucl_btree_getdad(a) == NULL);
  assert(ucl_btree_getbro(c) == NULL);
  assert((ucl_btree_getson(b) == NULL) && (ucl_btree_getbro(b) == NULL));
  assert((ucl_btree_getson(d) == NULL) && (ucl_btree_getbro(d) == NULL));
  
  assert('a' == *((char *)ucl_btree_data(a)));
  assert('b' == *((char *)ucl_btree_data(b)));
  assert('c' == *((char *)ucl_btree_data(c)));
  assert('d' == *((char *)ucl_btree_data(d)));

  node_final(a);
  node_final(b);
  node_final(c);
  node_final(d);
}

/* end of file */
