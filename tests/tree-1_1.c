/* tree-1_1.c --
   
   Part of: Useless Containers Library
   Contents: tree test
   Date: Thu Dec 11, 2003
   
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
   
   $Id: tree-1_1.c,v 1.1.1.2 2003/12/11 08:41:24 marco Exp $
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

  /*          dad    node    son    next  first   last  */
  assert_node(NULL,  n[0],  n[1],  NULL,   n[0],  n[0]);
  assert_node(n[0],  n[1],  n[6],  n[2],   n[1],  n[5]);
  assert_node(n[0],  n[2],  NULL,  n[3],   n[1],  n[5]);
  assert_node(n[0],  n[3], n[14],  n[4],   n[1],  n[5]);
  assert_node(n[0],  n[4], n[18],  n[5],   n[1],  n[5]);
  assert_node(n[0],  n[5],  NULL,  NULL,   n[1],  n[5]);

  /*          dad    node    son    next  first   last  */
  assert_node(n[1],  n[6],  n[9],  n[7],   n[6],  n[8]);
  assert_node(n[1],  n[7], n[11],  n[8],   n[6],  n[8]);
  assert_node(n[1],  n[8], n[12],  NULL,   n[6],  n[8]);
  assert_node(n[6],  n[9], n[10],  NULL,   n[9],  n[9]);
  assert_node(n[9], n[10],  NULL,  NULL,  n[10], n[10]);
  assert_node(n[7], n[11],  NULL,  NULL,  n[11], n[11]);
  assert_node(n[8], n[12],  NULL, n[13],  n[12], n[13]);
  assert_node(n[8], n[13],  NULL,  NULL,  n[12], n[13]);


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

  /*          dad    node    son   next   first   last   */
  assert_node(n[3], n[14], n[15],  NULL,  n[14], n[14]);
  assert_node(n[14],n[15], n[17], n[16],  n[15], n[16]);
  assert_node(n[14],n[16],  NULL,  NULL,  n[15], n[16]);
  assert_node(n[15],n[17],  NULL,  NULL,  n[17], n[17]);
  assert_node(n[4], n[18], n[19],  NULL,  n[18], n[18]);
  assert_node(n[18],n[19], n[21], n[25],  n[19], n[28]);
  assert_node(n[21],n[20],  NULL,  NULL,  n[22], n[20]);
  assert_node(n[19],n[21], n[22],  NULL,  n[21], n[21]);
  assert_node(n[21],n[22],  NULL, n[23],  n[22], n[20]);
  assert_node(n[21],n[23],  NULL, n[24],  n[22], n[20]);
  assert_node(n[21],n[24],  NULL, n[20],  n[22], n[20]);

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

  /*          dad    node    son   next   first   last   */
  assert_node(n[18],n[25], n[26], n[27],  n[19], n[28]);
  assert_node(n[25],n[26],  NULL,  NULL,  n[26], n[26]);
  assert_node(n[18],n[27],  NULL, n[28],  n[19], n[28]);
  assert_node(n[18],n[28], n[29],  NULL,  n[19], n[28]);
  assert_node(n[28],n[29],  NULL,  NULL,  n[29], n[29]);

  assert(ucl_tree_get_prev(n[0])  == NULL);
  assert(ucl_tree_get_prev(n[1])  == NULL);
  assert(ucl_tree_get_prev(n[2])  == n[1]);
  assert(ucl_tree_get_prev(n[25]) == n[19]);

  assert(ucl_tree_is_dad(n[7], n[11]));
  assert(ucl_tree_is_dad(n[7], n[12]) == 0);
  assert(ucl_tree_is_dad(n[0], n[2]));
  assert(ucl_tree_is_dad(n[0], n[3]));
  assert(ucl_tree_is_dad(n[0], n[4]));
  assert(ucl_tree_is_dad(n[0], n[5]));


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

  assert(ucl_tree_is_bro(n[1], n[2]));
  assert(ucl_tree_is_bro(n[1], n[3]));
  assert(ucl_tree_is_bro(n[1], n[4]));
  assert(ucl_tree_is_bro(n[1], n[5]));

  assert(ucl_tree_is_bro(n[5], n[4]));
  assert(ucl_tree_is_bro(n[5], n[3]));
  assert(ucl_tree_is_bro(n[5], n[2]));
  assert(ucl_tree_is_bro(n[5], n[1]));
  assert(ucl_tree_is_bro(n[0], n[1]) == 0);

  assert(ucl_tree_has_dad(n[0]) == 0);
  assert(ucl_tree_has_dad(n[1]));
  assert(ucl_tree_has_dad(n[2]));
  assert(ucl_tree_has_dad(n[3]));
  assert(ucl_tree_has_dad(n[4]));
  assert(ucl_tree_has_dad(n[5]));
  assert(ucl_tree_has_dad(n[13]));

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

  assert(ucl_tree_has_prev(n[0]) == 0);
  assert(ucl_tree_has_prev(n[1]) == 0);

  assert(ucl_tree_has_prev(n[2]));
  assert(ucl_tree_has_prev(n[3]));
  assert(ucl_tree_has_prev(n[4]));
  assert(ucl_tree_has_prev(n[5]));

  clean_tree(n);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
