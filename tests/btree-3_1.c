/* btree-3_1.c --
   
   Part of: Useless Containers Library
   Contents: test for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2005 Marco Maggi
   
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
  int			i;
  ucl_btree_node_t *	n[13];
  ucl_btree_node_t *	ptr;

  
  for (i=0; i<13; ++i)
    {
      n[i] = malloc(sizeof(ucl_btree_node_t));
    }

  /*
    build hierarchy

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  */

  ucl_btree_constructor(n[5], NULL);
  ucl_btree_constructor(n[1], NULL);  ucl_btree_dadson(n[5],  n[1]);
  ucl_btree_constructor(n[0], NULL);  ucl_btree_dadson(n[1],  n[0]);
  ucl_btree_constructor(n[3], NULL);  ucl_btree_dadbro(n[1],  n[3]);
  ucl_btree_constructor(n[2], NULL);  ucl_btree_dadson(n[3],  n[2]);
  ucl_btree_constructor(n[4], NULL);  ucl_btree_dadbro(n[3],  n[4]);
  ucl_btree_constructor(n[10], NULL); ucl_btree_dadbro(n[5],  n[10]);
  ucl_btree_constructor(n[7], NULL);  ucl_btree_dadson(n[10], n[7]);
  ucl_btree_constructor(n[6], NULL);  ucl_btree_dadson(n[7],  n[6]);
  ucl_btree_constructor(n[9], NULL);  ucl_btree_dadbro(n[7],  n[9]);
  ucl_btree_constructor(n[8], NULL);  ucl_btree_dadson(n[9],  n[8]);
  ucl_btree_constructor(n[12], NULL); ucl_btree_dadbro(n[10], n[12]);
  ucl_btree_constructor(n[11], NULL); ucl_btree_dadson(n[12], n[11]);

  /*
    printf("\n");
    for (i=0; i<13; ++i)
    {
    printf("node %2d: %p,\tbro: %p,\tson: %p,\tdad: %p\n",
    i, n[i], n[i]->broPtr, n[i]->sonPtr, n[i]->dadPtr);
    }
  */
  /* tests */

  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  */

  ptr = ucl_btree_find_leftmost(n[5]);
  assert(ptr == n[0]);

  for (i=1; i<13; ++i)
    {
      ptr = ucl_btree_step_inorder(ptr);
      assert(ptr == n[i]);
    }


  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  */

  ptr = ucl_btree_find_rightmost(n[5]);
  assert(ptr == n[12]);

  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[11]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[10]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[9]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[8]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[7]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[6]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[5]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[4]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[3]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[2]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[1]);
  ptr = ucl_btree_step_inorder_backward(ptr);
  assert(ptr == n[0]);
  
  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  */

  ptr = n[5];

  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_preorder(ptr);assert(ptr == n[0]);
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

  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  */

  ptr = ucl_btree_find_deepest_son(n[5]);
  assert(ptr == n[0]);

  ptr = ucl_btree_step_postorder(ptr);assert(ptr == n[2]);
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
  
  /*
		5-------10----12
		|        |     |
		1--3--4  7--9 11
		|  |     |  |
		0  2     6  8
  */

  ptr = n[5];

  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[1]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[10]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[0]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[3]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[7]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[12]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[2]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[4]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[6]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[9]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[11]);
  ptr = ucl_btree_step_levelorder(ptr);assert(ptr == n[8]);
  
  for (i=0; i<12; ++i)
    {
      free(n[i]);
    }
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
