/* tree-5_3.c --
   
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
   
   $Id: tree-5_3.c,v 1.1.1.1 2003/12/11 08:53:11 marco Exp $
*/

#include "treetest.h"

void
test (void)
{
  ucl_tree_node_t *		n[100];
  ucl_tree_node_t *		node;
  ucl_tree_node_t *		target;


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

  node   = n[3];
  target = ucl_tree_extract_next(node);

  assert(target == n[4]);

  assert(ucl_tree_get_dad(node)  == n[0]);
  assert(ucl_tree_get_prev(node) == n[2]);
  assert(ucl_tree_get_next(node) == n[18]);
  assert(ucl_tree_get_son(node)  == n[14]);

  assert(ucl_tree_get_prev(n[5]) == n[18]);

  assert(ucl_tree_get_dad(n[18])  == n[0]);
  assert(ucl_tree_get_next(n[18]) == n[5]);
  assert(ucl_tree_get_prev(n[18]) == n[3]);

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

  node   = n[3];
  target = ucl_tree_extract_next(node);

  assert(target == n[18]);

  assert(ucl_tree_get_dad(node)  == n[0]);
  assert(ucl_tree_get_prev(node) == n[2]);
  assert(ucl_tree_get_next(node) == n[19]);
  assert(ucl_tree_get_son(node)  == n[14]);

  assert(ucl_tree_get_dad(n[19])  == n[0]);
  assert(ucl_tree_get_prev(n[19]) == n[3]);
  assert(ucl_tree_get_next(n[19]) == n[25]);
  assert(ucl_tree_get_son(n[19])  == n[21]);

  assert(ucl_tree_get_dad(n[28])  == n[0]);
  assert(ucl_tree_get_prev(n[28]) == n[27]);
  assert(ucl_tree_get_next(n[28]) == n[5]);
  assert(ucl_tree_get_son(n[28])  == n[29]);

  assert(ucl_tree_get_first(n[5]) == n[1]);
  assert(ucl_tree_get_last(n[1]) == n[5]);

  clean_tree(n);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
