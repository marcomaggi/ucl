/* Stub table for the "graph" module.
   This file must be included in the stub shared library. */

/** ------------------------------------------------------------ 
 ** Stub table for the "graph" module.
 ** --------------------------------------------------------- */

/* Functions stub table for the "graph" module. */

const ucl_graph_stub_table_t ucl_graph_stub_table = {
  ucl_graph_link,
  ucl_graph_unlink,
  ucl_graph_erase_node_free_links,
  ucl_graph_last_output_link,
  ucl_graph_last_input_link,
  ucl_graph_first_output_link,
  ucl_graph_first_input_link,
  ucl_graph_erase_node_return_links,
  ucl_graph_remove_next_node,
  ucl_graph_insert_next_node,
  ucl_graph_merge_upon_input_link,
  ucl_graph_merge_upon_output_link,
  ucl_graph_directed_depth_first_search,
  ucl_graph_initialise_dfs_handle,
  ucl_graph_depth_first_search,
  ucl_graph_finalise_dfs_handle,
  ucl_graph_nodes_are_linked,
  ucl_graph_nodes_are_connected,
  ucl_graph_set_next_node
};


/* end of section */
