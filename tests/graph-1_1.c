/*
   Part of: Useless Containers Libray
   Contents: graph test
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


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "graphtest.h"

static void do_directed_dfs (ucl_graph_node_t **n);
static void do_dfs (ucl_graph_node_t **n);


void
test (void)
{
  ucl_graph_node_t *	n[9];
  ucl_graph_link_t *	l[12];
  ucl_graph_node_t **	np = &(n[0]);
  ucl_graph_link_t **	lp = &(l[0]);


  make_graph_1(np, lp);
  if (1) do_directed_dfs(n);
  if (1) do_dfs(n);
  destroy_graph_1(np, lp);
}
void
do_directed_dfs (ucl_graph_node_t **n)
{
  ucl_graph_dfs_t	handle;
  ucl_graph_dfs_item_t *item;
  size_t		i, j;

  static const int	directed_dfs_result_indexes[9] = {1, 4, 7,
							  8, 9, 5,
							  6, 2, 3};
  
  debug("directed dfs:\n");
  ucl_graph_initialise_dfs_handle(&handle);

  test_graph_clean_marks(n);
  ucl_graph_directed_depth_first_search(&handle, n[0]);

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
  */
  for (i=0; i<ucl_vector_size(handle.visited_nodes); ++i)
    {
      item = ucl_vector_index_to_slot(handle.visited_nodes, i);
      j = find_node(n, item->node_p);
      debug("index %d node %d\n", i, j);
      assert(directed_dfs_result_indexes[i] == (int)j);
    }

  ucl_graph_finalise_dfs_handle(&handle);
}
void
do_dfs (ucl_graph_node_t **n)
{
  ucl_graph_dfs_t	handle;
  ucl_graph_dfs_item_t *item;
  size_t		i, j;

  static const int	dfs_result_indexes[9] = {1, 4, 7,
						 8, 9, 6,
						 3, 2, 5};
  
  debug("dfs:\n");
  ucl_graph_initialise_dfs_handle(&handle);

  test_graph_clean_marks(n);
  ucl_graph_depth_first_search(&handle, n[0]);

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
  for (i=0; i<ucl_vector_size(handle.visited_nodes); ++i)
    {
      item = ucl_vector_index_to_slot(handle.visited_nodes, i);
      j = find_node(n, item->node_p);
      debug("index %d node %d\n", i, j);
      assert(dfs_result_indexes[i] == (int)j);
    }

  ucl_graph_finalise_dfs_handle(&handle);
}

/* end of file */
