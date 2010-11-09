/* tree-6_4.c --
   
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
   
   $Id: tree-6_4.c,v 1.1.1.1 2003/12/11 09:35:22 marco Exp $
*/

#include "treetest.h"

void
test (void)
{
  ucl_tree_node_t *		n[100];
  ucl_tree_node_t *		node;
  ucl_tree_node_t *		target;
  /*
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

  node   = n[13];
  target = ucl_tree_extract_prev(node);

  assert(target == n[12]);

  /*          dad    node    son    next  first   last  */
  assert_node(n[8],  node,  NULL,  NULL,  node,   node);
  assert_node(n[1],  n[8],  node,  NULL,  n[6],   n[8]);

  assert(ucl_tree_get_prev(node) == NULL);
  assert(ucl_tree_get_prev(n[8]) == n[7]);

  clean_tree(n);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/