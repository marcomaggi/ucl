/*
   Part of: Useless Containers Library
   Contents: graph test header
   Date: Thu Oct  5, 2006
   
   Abstract
   
   
   
   Copyright (c) 2006, 2008 Marco Maggi
   
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

#ifndef GRAPHTEST_H
#define GRAPHTEST_H 1


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "testmain.h"

static int find_link (ucl_graph_link_t ** l, ucl_graph_link_t * link_p);
static int find_node (ucl_graph_node_t ** l, ucl_graph_node_t * node_p);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Macros.
 ** ----------------------------------------------------------*/

/*
   1 -> 2 -> 3
   |    ^    |
   v    |    v
   4 <- 5 -> 6
   |    ^    |
   v    |    v
   7 -> 8 -> 9
*/
#define I12	0
#define I23	1
#define I14	2
#define I52	3
#define I36	4
#define I54	5
#define I56	6
#define I47	7
#define I85	8
#define I69	9
#define I78	10
#define I89	11

#define L12	l[I12]
#define L23	l[I23]
#define L14	l[I14]
#define L52	l[I52]
#define L36	l[I36]
#define L54	l[I54]
#define L56	l[I56]
#define L47	l[I47]
#define L85	l[I85]
#define L69	l[I69]
#define L78	l[I78]
#define L89	l[I89]

#define N1	n[0]
#define N2	n[1]
#define N3	n[2]
#define N4	n[3]
#define N5	n[4]
#define N6	n[5]
#define N7	n[6]
#define N8	n[7]
#define N9	n[8]


/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static void
make_graph_1 (ucl_graph_node_t ** n, ucl_graph_link_t ** l)
{
  ucl_graph_link_t *	link_a;


  memset(n, '\0', 9 * sizeof(void *));
  memset(l, '\0', 12 * sizeof(void *));

  for (size_t i=0; i<9; ++i)
    ucl_memory_alloc(NULL, (void **)&(n[i]), sizeof(ucl_graph_node_t));
  
  for (size_t i=0; i<12; ++i)
    ucl_memory_alloc(NULL, (void **)&(l[i]), sizeof(ucl_graph_link_t));

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Links are created in "counterclockwise" order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  ucl_graph_link(N1, L14, N4); /* node 1 */
  ucl_graph_link(N1, L12, N2);

  ucl_graph_link(N2, L23, N3); /* node 2 */

  ucl_graph_link(N3, L36, N6); /* node 3 */

  ucl_graph_link(N4, L47, N7); /* node 4 */

  ucl_graph_link(N5, L56, N6); /* node 5 */
  ucl_graph_link(N5, L52, N2);
  ucl_graph_link(N5, L54, N4);

  ucl_graph_link(N6, L69, N9); /* node 6 */

  ucl_graph_link(N7, L78, N8); /* node 7 */

  ucl_graph_link(N8, L89, N9); /* node 8 */
  ucl_graph_link(N8, L85, N5);


  ucl_graph_set_next_node(N1, N2);
  ucl_graph_set_next_node(N2, N3);
  ucl_graph_set_next_node(N3, N4);
  ucl_graph_set_next_node(N4, N5);
  ucl_graph_set_next_node(N5, N6);
  ucl_graph_set_next_node(N6, N7);
  ucl_graph_set_next_node(N7, N8);
  ucl_graph_set_next_node(N8, N9);
  ucl_graph_set_next_node(N9, NULL);

  assert(N2 == ucl_graph_get_next_node(N1));
  assert(N3 == ucl_graph_get_next_node(N2));
  assert(N4 == ucl_graph_get_next_node(N3));
  assert(N5 == ucl_graph_get_next_node(N4));
  assert(N6 == ucl_graph_get_next_node(N5));
  assert(N7 == ucl_graph_get_next_node(N6));
  assert(N8 == ucl_graph_get_next_node(N7));
  assert(N9 == ucl_graph_get_next_node(N8));
  assert(NULL == ucl_graph_get_next_node(N9));

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Link order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  /* node 1 */
  link_a = ucl_graph_input_link(N1); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N1); assert(L14 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(L12 == link_a);

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Link order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  /* node 2 */
  link_a = ucl_graph_input_link(N2); assert(L12 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(L52 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N2); assert(L23 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Link order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  /* node 3 */
  link_a = ucl_graph_input_link(N3); assert(L23 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N3); assert(L36 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Link order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  /* node 4 */
  link_a = ucl_graph_input_link(N4); assert(L14 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(L54 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N4); assert(L47 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Link order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  /* node 5 */
  link_a = ucl_graph_input_link(N5); assert(L85 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N5); assert(L56 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(L52 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(L54 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /* node 6 */
  link_a = ucl_graph_input_link(N6); assert(L36 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(L56 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N6); assert(L69 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9

    Link order:

    L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85
  */

  /* node 7 */
  link_a = ucl_graph_input_link(N7); assert(L47 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N7); assert(L78 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /* node 8 */
  link_a = ucl_graph_input_link(N8); assert(L78 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N8); assert(L89 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(L85 == link_a);
  link_a = ucl_graph_next_output_link(link_a); assert(NULL == link_a);

  /* node 9 */
  link_a = ucl_graph_input_link(N9); assert(L69 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(L89 == link_a);
  link_a = ucl_graph_next_input_link(link_a); assert(NULL == link_a);

  link_a = ucl_graph_output_link(N9); assert(NULL == link_a);

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9
  */
  assert(ucl_graph_nodes_are_linked(N1, N2));
  assert(ucl_graph_nodes_are_linked(N1, N4));
  assert(ucl_graph_nodes_are_linked(N2, N3));
  assert(ucl_graph_nodes_are_linked(N3, N6));
  assert(ucl_graph_nodes_are_linked(N4, N7));
  assert(ucl_graph_nodes_are_linked(N5, N4));
  assert(ucl_graph_nodes_are_linked(N5, N2));
  assert(ucl_graph_nodes_are_linked(N5, N6));
  assert(ucl_graph_nodes_are_linked(N6, N9));
  assert(ucl_graph_nodes_are_linked(N7, N8));
  assert(ucl_graph_nodes_are_linked(N8, N9));

  assert(0 == ucl_graph_nodes_are_linked(N2, N1));
  assert(0 == ucl_graph_nodes_are_linked(N4, N1));
  assert(0 == ucl_graph_nodes_are_linked(N3, N2));
  assert(0 == ucl_graph_nodes_are_linked(N6, N3));
  assert(0 == ucl_graph_nodes_are_linked(N7, N4));
  assert(0 == ucl_graph_nodes_are_linked(N4, N5));
  assert(0 == ucl_graph_nodes_are_linked(N2, N5));
  assert(0 == ucl_graph_nodes_are_linked(N6, N5));
  assert(0 == ucl_graph_nodes_are_linked(N9, N6));
  assert(0 == ucl_graph_nodes_are_linked(N8, N7));
  assert(0 == ucl_graph_nodes_are_linked(N9, N8));

  assert(0 == ucl_graph_nodes_are_linked(N1, N5));
  assert(0 == ucl_graph_nodes_are_linked(N5, N1));
  assert(0 == ucl_graph_nodes_are_linked(N5, N9));
  assert(0 == ucl_graph_nodes_are_linked(N9, N5));
  assert(0 == ucl_graph_nodes_are_linked(N3, N5));
  assert(0 == ucl_graph_nodes_are_linked(N5, N3));
  assert(0 == ucl_graph_nodes_are_linked(N5, N7));
  assert(0 == ucl_graph_nodes_are_linked(N7, N5));

  /*
    1 -> 2 -> 3
    |    ^    |
    v    |    v
    4 <- 5 -> 6
    |    ^    |
    v    |    v
    7 -> 8 -> 9
  */

  assert(ucl_graph_nodes_are_connected(N1,N2));
  assert(ucl_graph_nodes_are_connected(N2,N1));
  assert(ucl_graph_nodes_are_connected(N1,N4));
  assert(ucl_graph_nodes_are_connected(N4,N1));
  assert(ucl_graph_nodes_are_connected(N2,N3));
  assert(ucl_graph_nodes_are_connected(N3,N2));
  assert(ucl_graph_nodes_are_connected(N3,N6));
  assert(ucl_graph_nodes_are_connected(N6,N3));
  assert(ucl_graph_nodes_are_connected(N4,N7));
  assert(ucl_graph_nodes_are_connected(N7,N4));
  assert(ucl_graph_nodes_are_connected(N5,N4));
  assert(ucl_graph_nodes_are_connected(N4,N5));
  assert(ucl_graph_nodes_are_connected(N5,N2));
  assert(ucl_graph_nodes_are_connected(N2,N5));
  assert(ucl_graph_nodes_are_connected(N5,N6));
  assert(ucl_graph_nodes_are_connected(N6,N5));
  assert(ucl_graph_nodes_are_connected(N6,N9));
  assert(ucl_graph_nodes_are_connected(N9,N6));
  assert(ucl_graph_nodes_are_connected(N7,N8));
  assert(ucl_graph_nodes_are_connected(N8,N7));
  assert(ucl_graph_nodes_are_connected(N8,N9));
  assert(ucl_graph_nodes_are_connected(N9,N8));

  assert(0 == ucl_graph_nodes_are_connected(N1, N5));
  assert(0 == ucl_graph_nodes_are_connected(N5, N1));
  assert(0 == ucl_graph_nodes_are_connected(N5, N9));
  assert(0 == ucl_graph_nodes_are_connected(N9, N5));
  assert(0 == ucl_graph_nodes_are_connected(N3, N5));
  assert(0 == ucl_graph_nodes_are_connected(N5, N3));
  assert(0 == ucl_graph_nodes_are_connected(N5, N7));
  assert(0 == ucl_graph_nodes_are_connected(N7, N5));
}

/* ------------------------------------------------------------ */



static void
destroy_graph_1 (ucl_graph_node_t ** n, ucl_graph_link_t ** l)
{
  ucl_graph_node_t *	next_p;
  ucl_graph_link_t *	link_p;
  int			i;
  ucl_graph_node_t *	other_p;


  UCL_GRAPH_NODE_LIST_DELETE_LOOP(n[0], next_p, other_p)
    {
      debug("scanning input links node %d, other node %d\n",
	    find_node(n, next_p), find_node(n, other_p));
      UCL_GRAPH_FIRST_INPUT_LINK_LOOP(next_p, link_p)
	{
	  ucl_graph_unlink(link_p);
	  i = find_link(l, link_p);
	  debug("unlinked %i\n", i);
	  ucl_memory_alloc(NULL, (void *)&(l[i]), 0);  
	}
      debug("scanning output links\n");
      UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(next_p, link_p)
	{
	  ucl_graph_unlink(link_p);
	  i = find_link(l, link_p);
	  debug("unlinked %i\n", i);
	  ucl_memory_alloc(NULL, (void *)&(l[i]), 0);  
	}

      i = find_node(n, next_p);
      ucl_memory_alloc(NULL, &(n[i-1]), 0);
      debug("other node %d\n\n", find_node(n, other_p));
    }

  for (size_t j = 0; j < 9; ++j)
    assert(NULL == n[j]);
  for (size_t k = 0; k < 12; ++k)
    assert(NULL == l[k]);
}

/* ------------------------------------------------------------ */



static int
find_link (ucl_graph_link_t ** l, ucl_graph_link_t * link_p)
{
  for (size_t i=0; i<12; ++i)
    {
      if (l[i] == link_p) return i;
    }
  return -1;
}
static int
find_node (ucl_graph_node_t ** l, ucl_graph_node_t * node_p)
{
  for (size_t i=0; i<9; ++i)
    {
      if (l[i] == node_p) return (i+1);
    }
  return -1;
}

static void
test_graph_clean_marks (ucl_graph_node_t **n)
{
  ucl_value_t		mark;
  ucl_graph_node_t *	next_p;

  mark.integer = 0;
  UCL_GRAPH_NODE_LIST_LOOP(n[0], next_p)
    ucl_graph_node_set_mark(next_p, mark);
}


#endif /* GRAPHTEST_H */

/* end of file */
