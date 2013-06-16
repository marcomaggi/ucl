/*
  Part of: Useless Containers Libray
  Contents: graph test
  Date: Thu Oct  5, 2006

  Abstract


  Copyright (c) 2006, 2008, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#define MCL_DEBUGGING		0
#include "mcl-test.h"
#include "debug.h"
#include "ucl.h"

/* test graph 1 */
#define NUMBER_OF_NODES_1	9
#define NUMBER_OF_LINKS_1	12


/** ------------------------------------------------------------
 ** Test graph number 1.
 ** ----------------------------------------------------------*/

/* We assume  that the  following macros are  evaluated in a  context in
   which the following pointers are defined:

   ucl_graph_node_t N
   ucl_graph_link_t L

   where "n" is the chain of nodes and "l" is the chain of links. */

/*
  1 -> 2 -> 3
  |    ^    |
  v    |    v
  4 <- 5 -> 6
  |    ^    |
  v    |    v
  7 -> 8 -> 9

  Indexes of links in the "L" array.
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

/*
  1 -> 2 -> 3
  |    ^    |
  v    |    v
  4 <- 5 -> 6
  |    ^    |
  v    |    v
  7 -> 8 -> 9

  Elements of the "L" array.
*/
#define L12	&L[I12]
#define L23	&L[I23]
#define L14	&L[I14]
#define L52	&L[I52]
#define L36	&L[I36]
#define L54	&L[I54]
#define L56	&L[I56]
#define L47	&L[I47]
#define L85	&L[I85]
#define L69	&L[I69]
#define L78	&L[I78]
#define L89	&L[I89]

/*
  1 -> 2 -> 3
  |    ^    |
  v    |    v
  4 <- 5 -> 6
  |    ^    |
  v    |    v
  7 -> 8 -> 9

  Elements of the "N" array.
*/
#define N1	&N[0]
#define N2	&N[1]
#define N3	&N[2]
#define N4	&N[3]
#define N5	&N[4]
#define N6	&N[5]
#define N7	&N[6]
#define N8	&N[7]
#define N9	&N[8]


/** ------------------------------------------------------------
 ** Helper functions: building test graph 1.
 ** ----------------------------------------------------------*/

static void
build_test_graph_1 (ucl_graph_node_t N, ucl_graph_link_t L)
{
  ucl_graph_link_t 	link;
  memset(N, '\0', NUMBER_OF_NODES_1 * sizeof(ucl_graph_node_tag_t));
  memset(L, '\0', NUMBER_OF_LINKS_1 * sizeof(ucl_graph_link_tag_t));
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

  /*** FROM HERE ON WE VALIDATE THE GRAPH STRUCTURE ***/

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
  link = ucl_graph_input_link(N1);		assert(NULL == link);

  link = ucl_graph_output_link(N1);		assert(L14 == link);
  link = ucl_graph_next_output_link(link);	assert(L12 == link);

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
  link = ucl_graph_input_link(N2);		assert(L12 == link);
  link = ucl_graph_next_input_link(link);	assert(L52 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N2);		assert(L23 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

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
  link = ucl_graph_input_link(N3);		assert(L23 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N3);		assert(L36 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

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
  link = ucl_graph_input_link(N4);		assert(L14 == link);
  link = ucl_graph_next_input_link(link);	assert(L54 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N4);		assert(L47 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

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
  link = ucl_graph_input_link(N5);		assert(L85 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N5);		assert(L56 == link);
  link = ucl_graph_next_output_link(link);	assert(L52 == link);
  link = ucl_graph_next_output_link(link);	assert(L54 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

  /* node 6 */
  link = ucl_graph_input_link(N6);		assert(L36 == link);
  link = ucl_graph_next_input_link(link);	assert(L56 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N6);		assert(L69 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

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
  link = ucl_graph_input_link(N7);		assert(L47 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N7);		assert(L78 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

  /* node 8 */
  link = ucl_graph_input_link(N8);		assert(L78 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N8);		assert(L89 == link);
  link = ucl_graph_next_output_link(link);	assert(L85 == link);
  link = ucl_graph_next_output_link(link);	assert(NULL == link);

  /* node 9 */
  link = ucl_graph_input_link(N9);		assert(L69 == link);
  link = ucl_graph_next_input_link(link);	assert(L89 == link);
  link = ucl_graph_next_input_link(link);	assert(NULL == link);

  link = ucl_graph_output_link(N9);		assert(NULL == link);

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


/** --------------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------------- */

static int UCL_UNUSED
find_link (ucl_graph_link_t L, ucl_graph_link_t link)
{
  for (size_t i=0; i<NUMBER_OF_LINKS_1; ++i) {
    if (&L[i] == link) return i;
  }
  return -1;
}
static int UCL_UNUSED
find_node (ucl_graph_node_t N, ucl_graph_node_t node)
{
  for (size_t i=0; i<NUMBER_OF_NODES_1; ++i) {
    /* mcl_debug("looking %p %p", &N[i], node); */
    if (&N[i] == node) return (i+1);
  }
  return -1;
}
static void
test_graph_clean_marks (ucl_graph_node_t N)
{
  ucl_value_t	mark = { .t_int = 0 };
  for (size_t i=0; i<NUMBER_OF_NODES_1; ++i)
    ucl_graph_node_set_mark(&N[i], mark);
}

static void
test_construction_and_destruction (void)
{
  mcl_test_begin("graph-1.1", "construction and destruction") {
    ucl_graph_node_tag_t 	N[NUMBER_OF_NODES_1];
    ucl_graph_link_tag_t 	L[NUMBER_OF_LINKS_1];
    build_test_graph_1(N, L);
  }
  mcl_test_end();
}

static void
test_dfs (void)
{
  mcl_test_begin("graph-2.1", "depth first search") {
    ucl_graph_node_tag_t 	N[NUMBER_OF_NODES_1];
    ucl_graph_link_tag_t 	L[NUMBER_OF_LINKS_1];
    ucl_graph_dfs_t		S;
    ucl_vector_config_t		C;
    ucl_vector_t		visited_nodes;

    build_test_graph_1(N, L);
    ucl_vector_initialise_config(C, sizeof(ucl_graph_dfs_item_t), NUMBER_OF_LINKS_1);
    ucl_vector_alloc(visited_nodes, C);
    ucl_graph_dfs_initialise_handle(S, visited_nodes);
    {
      static const int	result_indexes[NUMBER_OF_NODES_1] = {1, 4, 7, 8, 9, 6, 3, 2, 5};
      test_graph_clean_marks(N);
      ucl_graph_dfs(S, N1);
      mcl_debug("visited %u nodes", ucl_vector_size(visited_nodes));
      assert(NUMBER_OF_NODES_1 == ucl_vector_size(visited_nodes));
      /*
	1 -> 2 -> 3
	|    ^    |
	v    |    v
	4 <- 5 -> 6
	|    ^    |
	v    |    v
	7 -> 8 -> 9

	Links inserted first are the  ones on the "right"; the link creation
	order is:

	L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85

	The output links are scanned before the input links.
      */
      for (ucl_index_t i=0; i<ucl_vector_size(visited_nodes); ++i) {
	ucl_graph_dfs_item_t *	item = ucl_vector_index_to_slot(visited_nodes, i);
	size_t			j = find_node(N, item->node);
	mcl_debug("index %d: %p node %d", i, (void *)item->node, j);
	assert(result_indexes[i] == (int)j);
      }
    }
    ucl_vector_free(visited_nodes);
  }
  mcl_test_end();
}

static void
test_directed_dfs (void)
{
  mcl_test_begin("graph-2.2", "directed depth first search") {
    ucl_graph_node_tag_t 	N[NUMBER_OF_NODES_1];
    ucl_graph_link_tag_t 	L[NUMBER_OF_LINKS_1];
    ucl_graph_dfs_t		S;
    ucl_vector_config_t		C;
    ucl_vector_t		visited_nodes;

    build_test_graph_1(N, L);
    ucl_vector_initialise_config(C, sizeof(ucl_graph_dfs_item_t), NUMBER_OF_LINKS_1);
    ucl_vector_alloc(visited_nodes, C);
    ucl_graph_dfs_initialise_handle(S, visited_nodes);
    {
      static const int	result_indexes[NUMBER_OF_NODES_1] = {1, 4, 7, 8, 9, 5, 6, 2, 3};
      test_graph_clean_marks(N);
      ucl_graph_dfs_directed(S, N1);
      mcl_debug("visited %u nodes", ucl_vector_size(visited_nodes));
      assert(NUMBER_OF_NODES_1 == ucl_vector_size(visited_nodes));
      /*
	1 -> 2 -> 3
	|    ^    |
	v    |    v
	4 <- 5 -> 6
	|    ^    |
	v    |    v
	7 -> 8 -> 9

	Links inserted first are the  ones on the "right"; the link creation
	order is:

	L14, L12; L23; L36; L47; L56, L52, L54; L69; L78; L89, L85

	The output links are scanned before the input links.
      */
      for (ucl_index_t i=0; i<ucl_vector_size(visited_nodes); ++i) {
	ucl_graph_dfs_item_t *	item = ucl_vector_index_to_slot(visited_nodes, i);
	size_t			j = find_node(N, item->node);
	mcl_debug("index %d: %p node %d", i, (void *)item->node, j);
	assert(result_indexes[i] == (int)j);
      }
    }
    ucl_vector_free(visited_nodes);
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("graph tests");

  mcl_test_subtitle("construction and destruction");
  test_construction_and_destruction();

  mcl_test_subtitle("depth first search");
  test_dfs ();
  test_directed_dfs ();

  exit(EXIT_SUCCESS);
}

/* end of file */
