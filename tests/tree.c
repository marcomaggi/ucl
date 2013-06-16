/*
  Part of: Useless Containers Library
  Contents: tree test
  Date: Thu Dec 11, 2003

  Abstract



  Copyright (c) 2003, 2005, 2008, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this  program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#define MCL_DEBUGGING		0
#include "mcl-test.h"
#include "debug.h"
#include "ucl.h"

#define NUMBER	100
/* #define DELTA	234 */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static ucl_node_t
alloc_link(void)
{
  ucl_node_t linkPtr;
  linkPtr = calloc(sizeof(ucl_node_tag_t), 1);
  if (linkPtr == NULL) {
    printf("error allocating memory\n");
    exit(EXIT_FAILURE);
  }
  return linkPtr;
}
#if 0
static int
find_node(ucl_node_t *n, ucl_node_t node)
{
  int i;
  for (i=0; i<NUMBER; ++i) {
    if (n[i] == node)
      return i;
  }
  return -1;
}
#endif
static void
assert_node (ucl_node_t dad, ucl_node_t nod,
	     ucl_node_t son, ucl_node_t nxt,
	     ucl_node_t fst, ucl_node_t lst)
{
  /*	    printf("node %d\n", find_node(nod));fflush(0);*/
  assert(ucl_tree_ref_dad(nod)  == dad);
  assert(ucl_tree_ref_son(nod)  == son);
  assert(ucl_tree_ref_next(nod) == nxt);
  assert(ucl_tree_ref_first(nod) == fst);
  assert(ucl_tree_ref_last(nod) == lst);
}
static void
fill_tree(ucl_node_t *n)
{
  int i;
  for (i=0; i<NUMBER; ++i)
    n[i] = NULL;
  for (i=0; i<50; ++i) {
    n[i] = alloc_link();
    assert(n[i]);
  }
  /*
     0
     |
     1------2-----3-----4-----5
     |            |     |
     6--7--8      14    18
     |  |  |      |     |
     9  11 12-13  15-16 19--25--27--28
     |            |     |   |       |
     10           17    21  26      29
     |
     22--23--24--20
  */
  ucl_tree_set_dadson(n[0], n[1]);

  ucl_tree_set_prevnext(n[1], n[2]);
  ucl_tree_set_prevnext(n[2], n[3]);
  ucl_tree_set_prevnext(n[3], n[4]);
  ucl_tree_set_prevnext(n[4], n[5]);

  ucl_tree_set_dadson(n[1], n[6]);
  ucl_tree_set_dadson(n[3], n[14]);
  ucl_tree_set_dadson(n[4], n[18]);

  ucl_tree_set_prevnext(n[6], n[7]);
  ucl_tree_set_prevnext(n[7], n[8]);

  ucl_tree_set_dadson(n[6], n[9]);
  ucl_tree_set_dadson(n[7], n[11]);
  ucl_tree_set_dadson(n[8], n[12]);
  ucl_tree_set_dadson(n[14], n[15]);
  ucl_tree_set_dadson(n[18], n[19]);
  /*
    0
    |
    1------2-----3-----4-----5
    |            |     |
    6--7--8      14    18
    |  |  |      |     |
    9  11 12-13  15-16 19--25--27--28
    |            |     |   |       |
    10           17    21  26      29
    |
    22--23--24--20
  */
  ucl_tree_set_prevnext(n[12], n[13]);
  ucl_tree_set_prevnext(n[15], n[16]);

  ucl_tree_set_prevnext(n[19], n[25]);
  ucl_tree_set_prevnext(n[25], n[27]);
  ucl_tree_set_prevnext(n[27], n[28]);

  ucl_tree_set_dadson(n[9],  n[10]);
  ucl_tree_set_dadson(n[15], n[17]);
  ucl_tree_set_dadson(n[19], n[21]);
  ucl_tree_set_dadson(n[25], n[26]);
  ucl_tree_set_dadson(n[28], n[29]);

  ucl_tree_set_dadson(n[10],  n[22]);

  ucl_tree_set_prevnext(n[22], n[23]);
  ucl_tree_set_prevnext(n[23], n[24]);
  ucl_tree_set_prevnext(n[24], n[20]);
}
static void
clean_tree(ucl_node_t *n)
{
  int i;
  for (i=0; i<NUMBER; ++i) {
    if (n[i])
      free(n[i]);
  }
}


static void
test_inspection (void)
{
  mcl_test_begin("tree-1.1", "inspecting tree") {
    ucl_node_t 	n[NUMBER];
    fill_tree(n);
    {
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
      assert_node(n[9], n[10], n[22],  NULL,  n[10], n[10]);
      assert_node(n[7], n[11],  NULL,  NULL,  n[11], n[11]);
      assert_node(n[8], n[12],  NULL, n[13],  n[12], n[13]);
      assert_node(n[8], n[13],  NULL,  NULL,  n[12], n[13]);
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
      /*          dad    node    son   next   first   last   */
      assert_node(n[18],n[19], n[21], n[25],  n[19], n[28]);
      assert_node(n[10],n[20],  NULL,  NULL,  n[22], n[20]);
      assert_node(n[19],n[21],  NULL,  NULL,  n[21], n[21]);
      assert_node(n[10],n[22],  NULL, n[23],  n[22], n[20]);
      assert_node(n[10],n[23],  NULL, n[24],  n[22], n[20]);
      assert_node(n[10],n[24],  NULL, n[20],  n[22], n[20]);
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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

      assert(ucl_tree_ref_prev(n[0])  == NULL);
      assert(ucl_tree_ref_prev(n[1])  == NULL);
      assert(ucl_tree_ref_prev(n[2])  == n[1]);
      assert(ucl_tree_ref_prev(n[25]) == n[19]);

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
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_insertion (void)
{
  mcl_test_begin("tree-2.1", "inserting nodes") {
    ucl_node_t	n[NUMBER];
    fill_tree(n);
    {
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      n[30] = alloc_link();
      ucl_tree_insert_dad(n[0], n[30]);
      /*          dad    node    son   next   first   last   */
      assert_node(n[30], n[0],  n[1],  NULL,   n[0],  n[0]);
      assert_node(NULL, n[30],  n[0],  NULL,  n[30], n[30]);

      n[31] = alloc_link();
      ucl_tree_insert_son(n[2], n[31]);
      /*          dad    node    son   next   first   last   */
      assert_node(n[0],  n[2], n[31],  n[3],   n[1],  n[5]);
      assert_node(n[2], n[31],  NULL,  NULL,  n[31], n[31]);

      n[32] = alloc_link();
      ucl_tree_insert_prev(n[6], n[32]);
      /*          dad    node    son   next   first   last   */
      assert_node(n[1],  n[6],  n[9],  n[7],  n[32],  n[8]);
      assert_node(n[1], n[32],  NULL,  n[6],  n[32],  n[8]);

      n[33] = alloc_link();
      ucl_tree_insert_prev(n[4], n[33]);
      /*          dad    node    son   next   first   last   */
      assert_node(n[0],  n[4], n[18],  n[5],  n[1],   n[5]);
      assert_node(n[0], n[33],  NULL,  n[4],  n[1],   n[5]);

      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */

      n[34] = alloc_link();
      ucl_tree_insert_next(n[25], n[34]);
      /*          dad    node    son   next   first   last   */
      assert_node(n[18],n[25], n[26], n[34],  n[19], n[28]);
      assert_node(n[18],n[34],  NULL, n[27],  n[19], n[28]);
      assert(ucl_tree_ref_prev(n[34]) == n[25]);

      n[35] = alloc_link();
      ucl_tree_insert_next(n[20], n[35]);
      /*          dad    node    son   next   first   last   */
      assert_node(n[10],n[20],  NULL, n[35],  n[22], n[35]);
      assert_node(n[10],n[35],  NULL,  NULL,  n[22], n[35]);
      assert(ucl_tree_ref_prev(n[35]) == n[20]);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_dad_extraction (void)
{
  mcl_test_begin("tree-3.1", "dad extraction") {
    ucl_node_t	n[NUMBER];
    fill_tree(n);
    {
      ucl_node_t	node;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node = ucl_tree_extract_dad(n[14]);
      assert(node == n[3]);
      assert(ucl_tree_ref_dad(n[14])  == n[0]);
      assert(ucl_tree_ref_prev(n[14]) == n[2]);
      assert(ucl_tree_ref_next(n[14]) == n[4]);
      assert(ucl_tree_ref_son(n[14])  == n[15]);
      assert(ucl_tree_ref_next(n[2])  == n[14]);
      assert(ucl_tree_ref_prev(n[4])  == n[14]);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[12];
      target = ucl_tree_extract_dad(node);
      assert(target == n[8]);
      assert(ucl_tree_ref_dad(node)  == n[1]);
      assert(ucl_tree_ref_prev(node) == n[7]);
      assert(ucl_tree_ref_next(node) == n[13]);
      assert(ucl_tree_ref_son(node)  == NULL);
      assert(ucl_tree_ref_next(n[7])  == node);
      assert(ucl_tree_ref_prev(n[13]) == node);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[9];
      target = ucl_tree_extract_dad(node);
      assert(target == n[6]);
      assert(ucl_tree_ref_dad(node)  == n[1]);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == n[7]);
      assert(ucl_tree_ref_son(node)  == n[10]);
      assert(ucl_tree_ref_prev(n[7]) == node);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[1];
      target = ucl_tree_extract_dad(node);
      assert(target == n[0]);
      assert(ucl_tree_ref_dad(node)  == NULL);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == n[2]);
      assert(ucl_tree_ref_son(node)  == n[6]);
      assert(ucl_tree_ref_dad(n[6])  == node);
      assert(ucl_tree_ref_prev(n[2]) == node);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[25];
      target = ucl_tree_extract_dad(node);
      assert(target == n[18]);
      assert(ucl_tree_ref_dad(node)  == n[4]);
      assert(ucl_tree_ref_prev(node) == n[19]);
      assert(ucl_tree_ref_next(node) == n[27]);
      assert(ucl_tree_ref_son(node)  == n[26]);
      assert(ucl_tree_ref_next(n[19]) == node);
      assert(ucl_tree_ref_prev(n[27]) == node);
      assert(ucl_tree_ref_dad(n[19]) == n[4]);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_son_extraction (void)
{
  mcl_test_begin("tree-3.2", "son extraction") {
    ucl_node_t	n[NUMBER];
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[3];
      target = ucl_tree_extract_son(node);
      assert(target == n[14]);
      assert(ucl_tree_ref_dad(node)  == n[0]);
      assert(ucl_tree_ref_prev(node) == n[2]);
      assert(ucl_tree_ref_next(node) == n[4]);
      assert(ucl_tree_ref_son(node)  == n[15]);
      assert(ucl_tree_ref_dad(n[15])  == node);
      assert(ucl_tree_ref_next(n[15]) == n[16]);
      assert(ucl_tree_ref_prev(n[15]) == NULL);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[14];
      target = ucl_tree_extract_son(node);
      assert(target == n[15]);
      assert(ucl_tree_ref_dad(node)  == n[3]);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == NULL);
      assert(ucl_tree_ref_son(node)  == n[17]);
      assert(ucl_tree_ref_dad(n[17])  == node);
      assert(ucl_tree_ref_next(n[17]) == n[16]);
      assert(ucl_tree_ref_prev(n[17]) == NULL);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[15];
      target = ucl_tree_extract_son(node);
      assert(target == n[17]);
      assert(ucl_tree_ref_dad(node)  == n[14]);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == n[16]);
      assert(ucl_tree_ref_son(node)  == NULL);
      assert(ucl_tree_ref_prev(n[16]) == node);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[21];
      target = ucl_tree_extract_son(node);
      assert(target == NULL);
      assert(ucl_tree_ref_dad(node)  == n[19]);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == NULL);
      assert(ucl_tree_ref_son(node)  == NULL);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[0];
      target = ucl_tree_extract_son(node);
      assert(target == n[1]);

      assert(ucl_tree_ref_dad(node)  == NULL);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == NULL);
      assert(ucl_tree_ref_son(node)  == n[6]);

      assert(ucl_tree_ref_dad(n[6])  == node);
      assert(ucl_tree_ref_next(n[6]) == n[7]);
      assert(ucl_tree_ref_prev(n[6]) == NULL);
      assert(ucl_tree_ref_son(n[6])  == n[9]);

      assert(ucl_tree_ref_dad(n[7])  == node);
      assert(ucl_tree_ref_next(n[7]) == n[8]);
      assert(ucl_tree_ref_prev(n[7]) == n[6]);
      assert(ucl_tree_ref_son(n[7])  == n[11]);

      assert(ucl_tree_ref_dad(n[8])  == node);
      assert(ucl_tree_ref_next(n[8]) == n[2]);
      assert(ucl_tree_ref_prev(n[8]) == n[7]);
      assert(ucl_tree_ref_son(n[8])  == n[12]);

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
      assert(ucl_tree_ref_dad(n[2])  == n[0]);
      assert(ucl_tree_ref_next(n[2]) == n[3]);
      assert(ucl_tree_ref_prev(n[2]) == n[8]);
      assert(ucl_tree_ref_son(n[2])  == NULL);

      assert(ucl_tree_ref_first(n[3]) == n[6]);
      assert(ucl_tree_ref_last(n[6]) == n[5]);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_next_extraction (void)
{
  mcl_test_begin("tree-3.3", "next extraction") {
    ucl_node_t	n[NUMBER];
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */

      node   = n[1];
      target = ucl_tree_extract_next(node);

      assert(target == n[2]);

      assert(ucl_tree_ref_dad(node)  == n[0]);
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_next(node) == n[3]);
      assert(ucl_tree_ref_son(node)  == n[6]);

      assert(ucl_tree_ref_dad(n[3])  == n[0]);
      assert(ucl_tree_ref_next(n[3]) == n[4]);
      assert(ucl_tree_ref_prev(n[3]) == n[1]);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[7];
      target = ucl_tree_extract_next(node);

      assert(target == n[8]);

      assert(ucl_tree_ref_dad(node)  == n[1]);
      assert(ucl_tree_ref_prev(node) == n[6]);
      assert(ucl_tree_ref_next(node) == n[12]);
      assert(ucl_tree_ref_son(node)  == n[11]);
      assert(ucl_tree_ref_last(node) == n[13]);

      assert(ucl_tree_ref_dad(n[12]) == n[1]);
      assert(ucl_tree_ref_next(n[12])== n[13]);
      assert(ucl_tree_ref_prev(n[12])== node);

      assert(ucl_tree_ref_first(n[13])== n[6]);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[3];
      target = ucl_tree_extract_next(node);

      assert(target == n[4]);

      assert(ucl_tree_ref_dad(node)  == n[0]);
      assert(ucl_tree_ref_prev(node) == n[2]);
      assert(ucl_tree_ref_next(node) == n[18]);
      assert(ucl_tree_ref_son(node)  == n[14]);

      assert(ucl_tree_ref_prev(n[5]) == n[18]);

      assert(ucl_tree_ref_dad(n[18])  == n[0]);
      assert(ucl_tree_ref_next(n[18]) == n[5]);
      assert(ucl_tree_ref_prev(n[18]) == n[3]);

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

      assert(ucl_tree_ref_dad(node)  == n[0]);
      assert(ucl_tree_ref_prev(node) == n[2]);
      assert(ucl_tree_ref_next(node) == n[19]);
      assert(ucl_tree_ref_son(node)  == n[14]);

      assert(ucl_tree_ref_dad(n[19])  == n[0]);
      assert(ucl_tree_ref_prev(n[19]) == n[3]);
      assert(ucl_tree_ref_next(n[19]) == n[25]);
      assert(ucl_tree_ref_son(n[19])  == n[21]);

      assert(ucl_tree_ref_dad(n[28])  == n[0]);
      assert(ucl_tree_ref_prev(n[28]) == n[27]);
      assert(ucl_tree_ref_next(n[28]) == n[5]);
      assert(ucl_tree_ref_son(n[28])  == n[29]);

      assert(ucl_tree_ref_first(n[5]) == n[1]);
      assert(ucl_tree_ref_last(n[1]) == n[5]);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */

      node   = n[4];
      target = ucl_tree_extract_next(node);
      assert(target == n[5]);
      /*          dad    node    son    next  first   last  */
      assert_node(n[0],  node,  n[18], NULL,  n[1],   node);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
      assert(ucl_tree_ref_prev(node) == n[25]);
      assert(ucl_tree_ref_prev(n[29]) == node);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
      assert(ucl_tree_ref_prev(node) == n[25]);
      assert(ucl_tree_ref_prev(n[29]) == node);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_prev_extraction (void)
{
  mcl_test_begin("tree-3.4", "pref extraction") {
    ucl_node_t	n[NUMBER];
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[5];
      target = ucl_tree_extract_prev(node);
      assert(target == n[4]);
      /*          dad    node    son    next  first   last  */
      assert_node(n[0],  node,   NULL,  NULL, n[1],   node);
      assert_node(n[0],  n[3],  n[14], n[18], n[1],   node);
      assert_node(n[0], n[18],  n[19], node,  n[1],   node);
      assert(ucl_tree_ref_prev(node) == n[18]);
      assert(ucl_tree_ref_prev(n[18]) == n[3]);
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[5];
      target = ucl_tree_extract_prev(node);
      assert(target == n[18]);
      /*          dad    node    son    next  first   last  */
      assert_node(n[0],  node,   NULL,  NULL, n[1],   node);
      assert_node(n[0],  n[3],  n[14], n[19], n[1],   node);
      assert_node(n[0], n[19],  n[21], n[25], n[1],   node);
      assert_node(n[0], n[28],  n[29],  node, n[1],   node);
      assert(ucl_tree_ref_prev(node)  == n[28]);
      assert(ucl_tree_ref_prev(n[19]) == n[3]);
      assert(ucl_tree_ref_prev(n[28]) == n[27]);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      node   = n[2];
      target = ucl_tree_extract_prev(node);
      assert(target == n[1]);
      /*          dad    node    son    next  first   last  */
      assert_node(n[0],  node,   NULL,  n[3], n[6],   n[5]);
      assert_node(n[0],  n[6],   n[9],  n[7], n[6],   n[5]);
      assert_node(n[0],  n[8],  n[12],  n[2], n[6],   n[5]);
      assert(ucl_tree_ref_prev(node) == n[8]);
      assert(ucl_tree_ref_prev(n[6]) == NULL);
      assert(ucl_tree_ref_prev(n[8]) == n[7]);
    }
    clean_tree(n);
    fill_tree(n);
    {
      ucl_node_t node, target;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
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
      assert(ucl_tree_ref_prev(node) == NULL);
      assert(ucl_tree_ref_prev(n[8]) == n[7]);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_inorder_iteration (void)
{
  mcl_test_begin("tree-4.1", "inorder iteration") {
    ucl_node_t	n[NUMBER];
    ucl_node_t	order[NUMBER];
    fill_tree(n);
    {
      ucl_node_t 	node;
      ucl_iterator_t	I;
      int		j;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      j=0;
      order[j++] = n[22];
      order[j++] = n[23];
      order[j++] = n[24];
      order[j++] = n[20];
      order[j++] = n[10];
      order[j++] = n[9];
      order[j++] = n[6];
      order[j++] = n[11];
      order[j++] = n[7];
      order[j++] = n[12];
      order[j++] = n[13];
      order[j++] = n[8];
      order[j++] = n[1];
      order[j++] = n[2];
      order[j++] = n[17];
      order[j++] = n[15];
      order[j++] = n[16];
      order[j++] = n[14];
      order[j++] = n[3];
      order[j++] = n[21];
      order[j++] = n[19];
      order[j++] = n[26];
      order[j++] = n[25];
      order[j++] = n[27];
      order[j++] = n[29];
      order[j++] = n[28];
      order[j++] = n[18];
      order[j++] = n[4];
      order[j++] = n[5];
      order[j++] = n[0];
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */


      for (ucl_tree_iterator_inorder(n[0], I), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	node = ucl_iterator_ptr(I);
	/* fprintf(stderr, "j=%u\n", j); */
	assert(node == order[j]);
      }
      assert(j = 30);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_preorder_iteration (void)
{
  mcl_test_begin("tree-4.2", "preorder iteration") {
    ucl_node_t	n[NUMBER];
    ucl_node_t	order[NUMBER];
    fill_tree(n);
    {
      ucl_node_t 	node;
      ucl_iterator_t	I;
      int		j;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      j=0;
      order[j++] = n[0];
      order[j++] = n[1];
      order[j++] = n[6];
      order[j++] = n[9];
      order[j++] = n[10];
      order[j++] = n[22];
      order[j++] = n[23];
      order[j++] = n[24];
      order[j++] = n[20];
      order[j++] = n[7];
      order[j++] = n[11];
      order[j++] = n[8];
      order[j++] = n[12];
      order[j++] = n[13];
      order[j++] = n[2];
      order[j++] = n[3];
      order[j++] = n[14];
      order[j++] = n[15];
      order[j++] = n[17];
      order[j++] = n[16];
      order[j++] = n[4];
      order[j++] = n[18];
      order[j++] = n[19];
      order[j++] = n[21];
      order[j++] = n[25];
      order[j++] = n[26];
      order[j++] = n[27];
      order[j++] = n[28];
      order[j++] = n[29];
      order[j++] = n[5];
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      for (ucl_tree_iterator_preorder(n[0], I), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	node = ucl_iterator_ptr(I);
	/* fprintf(stderr, "j=%u\n", j); */
	assert(node == order[j]);
      }
      assert(j = 30);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

static void
test_postorder_iteration (void)
{
  mcl_test_begin("tree-4.3", "postorder iteration") {
    ucl_node_t	n[NUMBER];
    ucl_node_t	order[NUMBER];
    fill_tree(n);
    {
      ucl_node_t 	node;
      ucl_iterator_t	I;
      int		j;
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      j=0;
      order[j++] = n[20];
      order[j++] = n[24];
      order[j++] = n[23];
      order[j++] = n[22];
      order[j++] = n[10];
      order[j++] = n[9];
      order[j++] = n[11];
      order[j++] = n[13];
      order[j++] = n[12];
      order[j++] = n[8];
      order[j++] = n[7];
      order[j++] = n[6];
      order[j++] = n[17];
      order[j++] = n[16];
      order[j++] = n[15];
      order[j++] = n[14];
      order[j++] = n[21];
      order[j++] = n[26];
      order[j++] = n[29];
      order[j++] = n[28];
      order[j++] = n[27];
      order[j++] = n[25];
      order[j++] = n[19];
      order[j++] = n[18];
      order[j++] = n[5];
      order[j++] = n[4];
      order[j++] = n[3];
      order[j++] = n[2];
      order[j++] = n[1];
      order[j++] = n[0];
      /*
	0
	|
	1------2-----3-----4-----5
	|            |     |
	6--7--8      14    18
	|  |  |      |     |
	9  11 12-13  15-16 19--25--27--28
	|            |     |   |       |
	10           17    21  26      29
	|
	22--23--24--20
      */
      for (ucl_tree_iterator_postorder(n[0], I), j=0; ucl_iterator_more(I); ucl_iterator_next(I), ++j) {
	node = ucl_iterator_ptr(I);
	assert(node == order[j]);
      }
      assert(j = 30);
    }
    clean_tree(n);
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("tree tests");

  mcl_test_subtitle("inspection");
  test_inspection();

  mcl_test_subtitle("insertion");
  test_insertion ();

  mcl_test_subtitle("extraction");
  test_dad_extraction ();
  test_son_extraction ();
  test_next_extraction ();
  test_prev_extraction ();

  mcl_test_subtitle("iteration");
  test_inorder_iteration ();
  test_preorder_iteration ();
  test_postorder_iteration ();

  exit(EXIT_SUCCESS);
}

/* end of file */
