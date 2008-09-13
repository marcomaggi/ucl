/* treetest.h --
   
   Part of: Useless Containers Library
   Contents: header file for tree tests
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


#ifndef __TREETEST_H
#define __TREETEST_H 1


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "testmain.h"
	
#define NUMBER	1000
#define DELTA	234

/* The following helper  functions are not used by all  the tests, so we
   do  not declare  them "static"  to avoid  the annoying  warning about
   "defined but not used" function. */
ucl_tree_node_t * alloc_link(void);
int find_node(ucl_tree_node_t **n, ucl_tree_node_t *node);
void assert_node (ucl_tree_node_t *dad, ucl_tree_node_t *nod,
		  ucl_tree_node_t *son, ucl_tree_node_t *nxt,
		  ucl_tree_node_t *fst, ucl_tree_node_t *lst);
void fill_tree(ucl_tree_node_t **n);
void clean_tree(ucl_tree_node_t **n);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

ucl_tree_node_t *
alloc_link(void)
{
  ucl_tree_node_t *linkPtr;

  linkPtr = (ucl_tree_node_t *) malloc(sizeof(ucl_tree_node_t));
  if (linkPtr == NULL) {
    printf("error allocating memory\n");
    exit(EXIT_FAILURE);
  }
  return linkPtr;
}
int
find_node(ucl_tree_node_t **n, ucl_tree_node_t *node)
{
  int i;

  for (i=0; i<100; ++i)
    {
      if (n[i] == node)
	{
	  return i;
	}
    }
  return -1;
}
void
assert_node (ucl_tree_node_t *dad, ucl_tree_node_t *nod,
	     ucl_tree_node_t *son, ucl_tree_node_t *nxt,
	     ucl_tree_node_t *fst, ucl_tree_node_t *lst)
{
  /*	    printf("node %d\n", find_node(nod));fflush(0);*/
  assert(ucl_tree_get_dad(nod)  == dad);
  assert(ucl_tree_get_son(nod)  == son);
  assert(ucl_tree_get_next(nod) == nxt);
  assert(ucl_tree_get_first(nod) == fst);
  assert(ucl_tree_get_last(nod) == lst);
}
void
fill_tree(ucl_tree_node_t **n)
{
  int i;


  for (i=0; i<50; ++i) {
    n[i] = alloc_link();
    assert(n[i]);
  }

  /* build hierarchy

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

  for (i=0; i<50; ++i)
    {
      ucl_tree_constructor(n[i]);
    }

  ucl_tree_set_son(n[0], n[1]);

  ucl_tree_set_bro(n[1], n[2]);
  ucl_tree_set_bro(n[2], n[3]);
  ucl_tree_set_bro(n[3], n[4]);
  ucl_tree_set_bro(n[4], n[5]);

  ucl_tree_set_son(n[1], n[6]);
  ucl_tree_set_bro(n[6], n[7]);
  ucl_tree_set_bro(n[7], n[8]);

  ucl_tree_set_son(n[6], n[9]);
  ucl_tree_set_son(n[9], n[10]);

  ucl_tree_set_son(n[7], n[11]);
  ucl_tree_set_son(n[8], n[12]);
  ucl_tree_set_bro(n[12], n[13]);

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

  ucl_tree_set_son(n[3],  n[14]);
  ucl_tree_set_son(n[14], n[15]);
  ucl_tree_set_son(n[15], n[17]);
  ucl_tree_set_bro(n[15], n[16]);

  ucl_tree_set_bro(n[4],  n[5]);
  ucl_tree_set_son(n[4],  n[18]);
  ucl_tree_set_son(n[18], n[19]);

  ucl_tree_set_son(n[19], n[21]);
  ucl_tree_set_son(n[21], n[22]);
  ucl_tree_set_bro(n[22], n[23]);
  ucl_tree_set_bro(n[23], n[24]);
  ucl_tree_set_bro(n[24], n[20]);

  ucl_tree_set_bro(n[19], n[25]);
  ucl_tree_set_son(n[25], n[26]);
  ucl_tree_set_bro(n[25], n[27]);
  ucl_tree_set_bro(n[27], n[28]);
  ucl_tree_set_son(n[28], n[29]);
}
void
clean_tree(ucl_tree_node_t **n)
{
  int i;

  for (i=0; i<50; ++i) {
    free(n[i]);
  }
}

#endif /* __TREETEST_H */

/* end of file */
