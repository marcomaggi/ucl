/* tree-10_1.c --
   
   Part of: Useless Containers Library
   Contents: tree test
   Date: Thu Dec 11, 2003
   
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

#include "treetest.h"

void
test (void)
{
  ucl_btree_node_t	n[100];
  ucl_btree_node_t 	node;
  ucl_btree_node_t 	order[30];
  ucl_iterator_t	iterator;
  int			i;


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
  i=0;
  order[i++] = n[10];
  order[i++] = n[9];
  order[i++] = n[6];
  order[i++] = n[11];
  order[i++] = n[7];
  order[i++] = n[12];
  order[i++] = n[13];
  order[i++] = n[8];
  order[i++] = n[1];
  order[i++] = n[2];
  order[i++] = n[17];
  order[i++] = n[15];
  order[i++] = n[16];
  order[i++] = n[14];
  order[i++] = n[3];
  order[i++] = n[22];
  order[i++] = n[23];
  order[i++] = n[24];
  order[i++] = n[20];
  order[i++] = n[21];
  order[i++] = n[19];
  order[i++] = n[26];
  order[i++] = n[25];
  order[i++] = n[27];
  order[i++] = n[29];
  order[i++] = n[28];
  order[i++] = n[18];
  order[i++] = n[4];
  order[i++] = n[5];
  order[i++] = n[0];

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

  i=0;
  for (ucl_tree_iterator_inorder(n[0], iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      node = ucl_iterator_ptr(iterator);
      assert(node == order[i]);
      ++i;
    }
  assert(i = 30);

  clean_tree(n);
}

/* end of file */
