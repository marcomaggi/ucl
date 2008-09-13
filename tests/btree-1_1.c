/* btree-1_1.c --
   
   Part of: Useless Containers Library
   Contents: test for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
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

#include "btreetest.h"

void
test (void)
{
  ucl_btree_node_t	*a, *b, *c, *d;


  a = (ucl_btree_node_t *) malloc(sizeof(ucl_btree_node_t));
  b = (ucl_btree_node_t *) malloc(sizeof(ucl_btree_node_t));
  c = (ucl_btree_node_t *) malloc(sizeof(ucl_btree_node_t));
  d = (ucl_btree_node_t *) malloc(sizeof(ucl_btree_node_t));


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


  ucl_btree_constructor(a, NULL);

  ucl_btree_constructor(b, a);
  ucl_btree_setson(a, b);

  ucl_btree_constructor(c, a);
  ucl_btree_setbro(a, c);
  
  ucl_btree_constructor(d, c);
  ucl_btree_setson(c, d);

  /* test all the pointers */

  assert((a == ucl_btree_getdad(b)) && (b == ucl_btree_getson(a)));
  assert((a == ucl_btree_getdad(c)) && (c == ucl_btree_getbro(a)));
  assert((c == ucl_btree_getdad(d)) && (d == ucl_btree_getson(c)));
  assert(ucl_btree_getdad(a) == NULL);
  assert(ucl_btree_getbro(c) == NULL);
  assert((ucl_btree_getson(b) == NULL) && (ucl_btree_getbro(b) == NULL));
  assert((ucl_btree_getson(d) == NULL) && (ucl_btree_getbro(d) == NULL));
  
  free(a);
  free(b);
  free(c);
  free(d);
}

/* end of file */
