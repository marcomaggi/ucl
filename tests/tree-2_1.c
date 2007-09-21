/* tree-2_1.c --
   
   Part of: Useless Containers Library
   Contents: tree test
   Date: Thu Dec 11, 2003
   
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
   
   $Id: tree-2_1.c,v 1.1.1.2 2003/12/11 08:38:39 marco Exp $
*/

#include "treetest.h"

void
test (void)
{
  ucl_tree_node_t *		n[100];
  /*
  ucl_tree_node_t *		node;
  ucl_tree_node_t *		target;
  ucl_tree_node_t *		root;
  ucl_iterator_t		iterator;
  int				i, j;
  */


  fill_tree(n);


  /*
	       0
               |
               1------2-----3-----4-----5
	       |            |     |
               6--7--8     14    18
	       |  |  |      |     |
	       9 11 12-13  15-16 19--25--27--28
               |            |     |   |       |
              10           17    21  26      29
                                  |
                                 22--23--24--20
  */

  ucl_tree_insert_dad(n[0], n[30]);
  /*          dad    node    son   next   first   last   */
  assert_node(n[30], n[0],  n[1],  NULL,   n[0],  n[0]);
  assert_node(NULL, n[30],  n[0],  NULL,  n[30], n[30]);

  ucl_tree_insert_son(n[2], n[31]);
  /*          dad    node    son   next   first   last   */
  assert_node(n[0],  n[2], n[31],  n[3],   n[1],  n[5]);
  assert_node(n[2], n[31],  NULL,  NULL,  n[31], n[31]);

  ucl_tree_insert_prev(n[6], n[32]);
  /*          dad    node    son   next   first   last   */
  assert_node(n[1],  n[6],  n[9],  n[7],  n[32],  n[8]);
  assert_node(n[1], n[32],  NULL,  n[6],  n[32],  n[8]);

  ucl_tree_insert_prev(n[4], n[33]);
  /*          dad    node    son   next   first   last   */
  assert_node(n[0],  n[4], n[18],  n[5],  n[1],   n[5]);
  assert_node(n[0], n[33],  NULL,  n[4],  n[1],   n[5]);

  /*
	       0
               |
               1------2-----3-----4-----5
	       |            |     |
               6--7--8     14    18
	       |  |  |      |     |
	       9 11 12-13  15-16 19--25--27--28
               |            |     |   |       |
              10           17    21  26      29
                                  |
                                 22--23--24--20
  */

  ucl_tree_insert_next(n[25], n[34]);
  /*          dad    node    son   next   first   last   */
  assert_node(n[18],n[25], n[26], n[34],  n[19], n[28]);
  assert_node(n[18],n[34],  NULL, n[27],  n[19], n[28]);
  assert(ucl_tree_get_prev(n[34]) == n[25]);

  ucl_tree_insert_next(n[20], n[35]);
  /*          dad    node    son   next   first   last   */
  assert_node(n[21],n[20],  NULL, n[35],  n[22], n[35]);
  assert_node(n[21],n[35],  NULL,  NULL,  n[22], n[35]);
  assert(ucl_tree_get_prev(n[35]) == n[20]);

  clean_tree(n);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
