/* btree-3_1.c --
   
   Part of: Useless Containers Library
   Contents: test for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Test iterations.
   
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
  node_t	ptr;

  
  for (i=1; i<13; ++i)
    {
      n[i] = node_make(i);
    }

  /*
    build hierarchy

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

/* ------------------------------------------------------------ */

  /*
    Forward inorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = ucl_btree_find_leftmost(n[5]); assert(ptr == n[1]);

  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[2]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[3]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[4]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[5]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[6]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[7]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[8]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[9]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[10]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[11]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == n[12]);
  ptr = ucl_btree_step_inorder(ptr); assert(ptr == NULL);

/* ------------------------------------------------------------ */

  /*
    Backward inorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = ucl_btree_find_rightmost(n[5]); assert(ptr == n[12]);

  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[11]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[10]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[9]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[8]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[7]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[6]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[5]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[4]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[3]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[2]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == n[1]);
  ptr = ucl_btree_step_inorder_backward(ptr); assert(ptr == NULL);
  
/* ------------------------------------------------------------ */

  /*
    Forward preorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = n[5];

  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[2]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[8]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[11]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == NULL);

/* ------------------------------------------------------------ */

  /*
    Backward preorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = n[5];

  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[11]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[8]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == n[2]);
  ptr = ucl_btree_step_preorder_backward(ptr);assert(ptr == NULL);

/* ------------------------------------------------------------ */

  /*
    Forward postorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = ucl_btree_find_deepest_son(n[5]); assert(ptr == n[2]);

  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[8]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[11]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[5]);
  ptr = ucl_btree_step_postorder(ptr);assert(ptr == NULL);
  
/* ------------------------------------------------------------ */

  /*
    Backward postorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = ucl_btree_find_deepest_bro(n[5]); assert(ptr == n[11]);

  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[8]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[2]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == n[5]);
  ptr = ucl_btree_step_postorder_backward(ptr);assert(ptr == NULL);
  
/* ------------------------------------------------------------ */

  /*
    Forward levelorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = n[5];

  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[2]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[11]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[8]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == NULL);
  
/* ------------------------------------------------------------ */

  /*
    Backward levelorder iteration.

    5-------10----12
    |        |     |
    1--3--4  7--9 11
       |     |  |
       2     6  8
  */

  ptr = n[5];

  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[11]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[2]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == n[8]);
  ptr = ucl_btree_step_levelorder_backward(ptr);assert(ptr == NULL);
  
/* ------------------------------------------------------------ */

  for (i=0; i<12; ++i)
    {
      node_final(n[i]);
    }
}

/* end of file */
