/* btree-2_1.c --
   
   Part of: Useless Containers Library
   Contents: test for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Test the finder functions.
   
   Copyright (c) 2003, 2005, 2008 Marco Maggi
   
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

#define DEBUGGING		0
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
 ** Test function.
 ** ----------------------------------------------------------*/

void
test (void)
{
  int		i;
  node_t	n[13];


  for (i=0; i<13; ++i)
    {
      n[i] = node_make((char)(64+i));
    }

  /*
    Build hierarchy

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  */

  ucl_btree_dadson(n[5],  n[1]);
  ucl_btree_dadbro(n[1],  n[3]);
  ucl_btree_dadson(n[3],  n[2]);
  ucl_btree_dadbro(n[3],  n[4]);
  ucl_btree_dadbro(n[5],  n[10]);
  ucl_btree_dadson(n[10], n[7]);
  ucl_btree_dadson(n[7],  n[6]);
  ucl_btree_dadbro(n[7],  n[9]);
  ucl_btree_dadson(n[9],  n[8]);
  ucl_btree_dadbro(n[10], n[12]);
  ucl_btree_dadson(n[12], n[11]);
  
#ifdef DEBUGGING
  for (i=1; i<12; ++i)
    {
      debug("node %d: %p, dad=%p, son=%p, bro=%p, c=%c", i, n[i],
	    ucl_btree_getdad(n[i]),
	    ucl_btree_getson(n[i]),
	    ucl_btree_getbro(n[i]),
	    *((char *)ucl_btree_data(n[i])));
    }

#endif

  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  */

  debug("got %p, expected %p", ucl_btree_find_leftmost(n[5]), n[1]);
  debug("got %p, expected %p", ucl_btree_find_rightmost(n[5]), n[12]);

  assert( ucl_btree_find_leftmost(n[5]) == n[1] );
  assert( ucl_btree_find_leftmost(n[1]) == n[1] );
  assert( ucl_btree_find_leftmost(n[3]) == n[2] );
  assert( ucl_btree_find_leftmost(n[2]) == n[2] );
  assert( ucl_btree_find_leftmost(n[4]) == n[4] );
  assert( ucl_btree_find_leftmost(n[10]) == n[6] );
  assert( ucl_btree_find_leftmost(n[7]) == n[6] );
  assert( ucl_btree_find_leftmost(n[12]) == n[11] );


  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  */

  assert( ucl_btree_find_rightmost(n[5]) == n[12] );
  assert( ucl_btree_find_rightmost(n[1]) == n[4] );
  assert( ucl_btree_find_rightmost(n[3]) == n[4] );
  assert( ucl_btree_find_rightmost(n[2]) == n[2] );
  assert( ucl_btree_find_rightmost(n[4]) == n[4] );
  assert( ucl_btree_find_rightmost(n[10]) == n[12] );
  assert( ucl_btree_find_rightmost(n[7]) == n[9] );
  assert( ucl_btree_find_rightmost(n[12]) == n[12] );

  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  */

  assert( ucl_btree_find_deepest_son(n[5]) == n[2] );
  assert( ucl_btree_find_deepest_son(n[1]) == n[2] );
  assert( ucl_btree_find_deepest_son(n[3]) == n[2] );
  assert( ucl_btree_find_deepest_son(n[2]) == n[2] );
  assert( ucl_btree_find_deepest_son(n[4]) == n[4] );
  assert( ucl_btree_find_deepest_son(n[10])== n[6] );
  assert( ucl_btree_find_deepest_son(n[7]) == n[6] );
  assert( ucl_btree_find_deepest_son(n[12])== n[11]);

  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  */

  assert( ucl_btree_find_deepest_bro(n[5]) == n[11]);
  assert( ucl_btree_find_deepest_bro(n[1]) == n[4] );
  assert( ucl_btree_find_deepest_bro(n[3]) == n[4] );
  assert( ucl_btree_find_deepest_bro(n[2]) == n[2] );
  assert( ucl_btree_find_deepest_bro(n[4]) == n[4] );
  assert( ucl_btree_find_deepest_bro(n[10])== n[11]);
  assert( ucl_btree_find_deepest_bro(n[12])== n[11]);
  assert( ucl_btree_find_deepest_bro(n[7]) == n[8] );
  assert( ucl_btree_find_deepest_bro(n[9]) == n[8] );
  assert( ucl_btree_find_deepest_bro(n[6]) == n[6]);
  assert( ucl_btree_find_deepest_bro(n[8]) == n[8]);
  assert( ucl_btree_find_deepest_bro(n[11])== n[11]);

  for (i=0; i<12; ++i)
    {
      node_final(n[i]);
    }
}

/* end of file */
