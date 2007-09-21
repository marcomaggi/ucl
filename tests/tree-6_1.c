/* tree-6_1.c --
   
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
   
   $Id: tree-6_1.c,v 1.1.1.1 2003/12/11 09:30:42 marco Exp $
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

  node   = n[27];
  target = ucl_tree_extract_next(node);

  assert(target == n[28]);

  /*          dad    node    son    next  first   last  */
  assert_node(n[18], node,  NULL,  n[29], n[19], n[29]);
  assert_node(n[18],n[29],  NULL,   NULL, n[19], n[29]);

  assert(ucl_tree_get_prev(node) == n[25]);
  assert(ucl_tree_get_prev(n[29]) == node);

  clean_tree(n);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
