/* tree-4_5.c --
   
   Part of: Useless Containers Library
   Contents: tree test
   Date: Thu Dec 11, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2005 Marco Maggi
   
   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.
   
   $Id: tree-4_5.c,v 1.1.1.1 2003/12/11 08:48:51 marco Exp $
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

  node   = n[0];
  target = ucl_tree_extract_son(node);

  assert(target == n[1]);

  assert(ucl_tree_get_dad(node)  == NULL);
  assert(ucl_tree_get_prev(node) == NULL);
  assert(ucl_tree_get_next(node) == NULL);
  assert(ucl_tree_get_son(node)  == n[6]);

  assert(ucl_tree_get_dad(n[6])  == node);
  assert(ucl_tree_get_next(n[6]) == n[7]);
  assert(ucl_tree_get_prev(n[6]) == NULL);
  assert(ucl_tree_get_son(n[6])  == n[9]);

  assert(ucl_tree_get_dad(n[7])  == node);
  assert(ucl_tree_get_next(n[7]) == n[8]);
  assert(ucl_tree_get_prev(n[7]) == n[6]);
  assert(ucl_tree_get_son(n[7])  == n[11]);

  assert(ucl_tree_get_dad(n[8])  == node);
  assert(ucl_tree_get_next(n[8]) == n[2]);
  assert(ucl_tree_get_prev(n[8]) == n[7]);
  assert(ucl_tree_get_son(n[8])  == n[12]);

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

  assert(ucl_tree_get_dad(n[2])  == n[0]);
  assert(ucl_tree_get_next(n[2]) == n[3]);
  assert(ucl_tree_get_prev(n[2]) == n[8]);
  assert(ucl_tree_get_son(n[2])  == NULL);

  assert(ucl_tree_get_first(n[3]) == n[6]);
  assert(ucl_tree_get_last(n[6]) == n[5]);

  clean_tree(n);
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
