/* btree-2_1.c --
   
   Part of: Useless Containers Library
   Contents: test for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: btree-2_1.c,v 1.1.1.2 2003/12/10 12:57:17 marco Exp $
*/


#include "btreetest.h"


void
test (void)
{
  int			i;
  ucl_btree_node_t *	n[13];


  for (i=0; i<13; ++i)
    {
      n[i] = malloc(sizeof(ucl_btree_node_t));
    }

  /*
    Build hierarchy

		5-------10----12
		|        |     |
		1--3--4  7--9 11
		   |     |  |
		   2     6  8
  */

  ucl_btree_constructor(n[5], NULL);
  ucl_btree_init(n[1]);  ucl_btree_dadson(n[5],  n[1]);
  ucl_btree_init(n[3]);  ucl_btree_dadbro(n[1],  n[3]);
  ucl_btree_init(n[2]);  ucl_btree_dadson(n[3],  n[2]);
  ucl_btree_init(n[4]);  ucl_btree_dadbro(n[3],  n[4]);
  ucl_btree_init(n[10]); ucl_btree_dadbro(n[5],  n[10]);
  ucl_btree_init(n[7]);  ucl_btree_dadson(n[10], n[7]);
  ucl_btree_init(n[6]);  ucl_btree_dadson(n[7],  n[6]);
  ucl_btree_init(n[9]);  ucl_btree_dadbro(n[7],  n[9]);
  ucl_btree_init(n[8]);  ucl_btree_dadson(n[9],  n[8]);
  ucl_btree_init(n[12]); ucl_btree_dadbro(n[10], n[12]);
  ucl_btree_init(n[11]); ucl_btree_dadson(n[12], n[11]);
  
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
		   |     |  |
		   2     6  8
  */

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
  assert( ucl_btree_find_deepest_son(n[10]) == n[6] );
  assert( ucl_btree_find_deepest_son(n[7]) == n[6] );
  assert( ucl_btree_find_deepest_son(n[12]) == n[11] );

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
