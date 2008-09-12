/* Stub header file for the "graph" module.
   This file must be included in the main stub header. */

#ifndef __UCL_GRAPH_H

/** ------------------------------------------------------------ 
 ** Module "graph".
 ** --------------------------------------------------------- */

/* Function prototypes for the "graph" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl ucl_graph_link_t * ucl_graph_last_output_link (ucl_graph_link_t * link_p);
ucl_decl ucl_graph_link_t * ucl_graph_last_input_link (ucl_graph_link_t * link_p);
ucl_decl ucl_graph_link_t * ucl_graph_first_output_link (ucl_graph_link_t * link_p);
ucl_decl ucl_graph_link_t * ucl_graph_first_input_link (ucl_graph_link_t * link_p);
ucl_decl void ucl_graph_link (ucl_graph_node_t * s, ucl_graph_link_t * l, ucl_graph_node_t * d);
ucl_decl ucl_bool_t ucl_graph_nodes_are_linked (ucl_graph_node_t * src_p, ucl_graph_node_t * dst_p);
ucl_decl ucl_bool_t ucl_graph_nodes_are_connected (ucl_graph_node_t * a, ucl_graph_node_t * b);
ucl_decl void ucl_graph_set_next_node (ucl_graph_node_t * node_p, const ucl_graph_node_t * next_p);
ucl_decl void ucl_graph_unlink (ucl_graph_link_t *link_p);
ucl_decl void ucl_graph_erase_node_free_links (ucl_graph_node_t * node_p, void (* destructor)(void *));
ucl_decl ucl_link_t * ucl_graph_erase_node_return_links (ucl_graph_node_t * node_p);
ucl_decl ucl_graph_node_t * ucl_graph_remove_next_node (ucl_graph_node_t * node_p);
ucl_decl void ucl_graph_insert_next_node (ucl_graph_node_t * node_p, ucl_graph_node_t * next_p);
ucl_decl void ucl_graph_merge_upon_input_link (ucl_graph_link_t * in, ucl_graph_link_t * out);
ucl_decl void ucl_graph_merge_upon_output_link (ucl_graph_link_t * in, ucl_graph_link_t * out);
ucl_decl void ucl_graph_initialise_dfs_handle (ucl_graph_dfs_t * search_handle);
ucl_decl void ucl_graph_finalise_dfs_handle (ucl_graph_dfs_t * search_handle);
ucl_decl void ucl_graph_directed_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);
ucl_decl void ucl_graph_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "graph" module.
   A typedef for each function in the module. */

typedef ucl_graph_link_t * ucl_graph_last_output_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_graph_link_t * ucl_graph_last_input_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_graph_link_t * ucl_graph_first_output_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_graph_link_t * ucl_graph_first_input_link_proto_t (ucl_graph_link_t * link_p);
typedef void ucl_graph_link_proto_t (ucl_graph_node_t * s, ucl_graph_link_t * l, ucl_graph_node_t * d);
typedef ucl_bool_t ucl_graph_nodes_are_linked_proto_t (ucl_graph_node_t * src_p, ucl_graph_node_t * dst_p);
typedef ucl_bool_t ucl_graph_nodes_are_connected_proto_t (ucl_graph_node_t * a, ucl_graph_node_t * b);
typedef void ucl_graph_set_next_node_proto_t (ucl_graph_node_t * node_p, const ucl_graph_node_t * next_p);
typedef void ucl_graph_unlink_proto_t (ucl_graph_link_t *link_p);
typedef void ucl_graph_erase_node_free_links_proto_t (ucl_graph_node_t * node_p, void (* destructor)(void *));
typedef ucl_link_t * ucl_graph_erase_node_return_links_proto_t (ucl_graph_node_t * node_p);
typedef ucl_graph_node_t * ucl_graph_remove_next_node_proto_t (ucl_graph_node_t * node_p);
typedef void ucl_graph_insert_next_node_proto_t (ucl_graph_node_t * node_p, ucl_graph_node_t * next_p);
typedef void ucl_graph_merge_upon_input_link_proto_t (ucl_graph_link_t * in, ucl_graph_link_t * out);
typedef void ucl_graph_merge_upon_output_link_proto_t (ucl_graph_link_t * in, ucl_graph_link_t * out);
typedef void ucl_graph_initialise_dfs_handle_proto_t (ucl_graph_dfs_t * search_handle);
typedef void ucl_graph_finalise_dfs_handle_proto_t (ucl_graph_dfs_t * search_handle);
typedef void ucl_graph_directed_depth_first_search_proto_t (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);
typedef void ucl_graph_depth_first_search_proto_t (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);

/* Stub table type declaration for the "graph" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_graph_stub_table_t {
  ucl_graph_link_proto_t              * stub_ucl_graph_link;
  ucl_graph_unlink_proto_t            * stub_ucl_graph_unlink;
  ucl_graph_erase_node_free_links_proto_t * stub_ucl_graph_erase_node_free_links;
  ucl_graph_last_output_link_proto_t  * stub_ucl_graph_last_output_link;
  ucl_graph_last_input_link_proto_t   * stub_ucl_graph_last_input_link;
  ucl_graph_first_output_link_proto_t * stub_ucl_graph_first_output_link;
  ucl_graph_first_input_link_proto_t  * stub_ucl_graph_first_input_link;
  ucl_graph_erase_node_return_links_proto_t * stub_ucl_graph_erase_node_return_links;
  ucl_graph_remove_next_node_proto_t  * stub_ucl_graph_remove_next_node;
  ucl_graph_insert_next_node_proto_t  * stub_ucl_graph_insert_next_node;
  ucl_graph_merge_upon_input_link_proto_t * stub_ucl_graph_merge_upon_input_link;
  ucl_graph_merge_upon_output_link_proto_t * stub_ucl_graph_merge_upon_output_link;
  ucl_graph_directed_depth_first_search_proto_t * stub_ucl_graph_directed_depth_first_search;
  ucl_graph_initialise_dfs_handle_proto_t * stub_ucl_graph_initialise_dfs_handle;
  ucl_graph_depth_first_search_proto_t * stub_ucl_graph_depth_first_search;
  ucl_graph_finalise_dfs_handle_proto_t * stub_ucl_graph_finalise_dfs_handle;
  ucl_graph_nodes_are_linked_proto_t  * stub_ucl_graph_nodes_are_linked;
  ucl_graph_nodes_are_connected_proto_t * stub_ucl_graph_nodes_are_connected;
  ucl_graph_set_next_node_proto_t     * stub_ucl_graph_set_next_node;
} ucl_graph_stub_table_t;

/* Stub table macros definition for the "graph" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_graph_stub_table_t * ucl_graph_stub_table_p;
ucl_decl const ucl_graph_stub_table_t   ucl_graph_stub_table;

#define ucl_graph_link             ((ucl_graph_stub_table_p)->stub_ucl_graph_link)
#define ucl_graph_unlink           ((ucl_graph_stub_table_p)->stub_ucl_graph_unlink)
#define ucl_graph_erase_node_free_links  ((ucl_graph_stub_table_p)->stub_ucl_graph_erase_node_free_links)
#define ucl_graph_last_output_link  ((ucl_graph_stub_table_p)->stub_ucl_graph_last_output_link)
#define ucl_graph_last_input_link  ((ucl_graph_stub_table_p)->stub_ucl_graph_last_input_link)
#define ucl_graph_first_output_link  ((ucl_graph_stub_table_p)->stub_ucl_graph_first_output_link)
#define ucl_graph_first_input_link  ((ucl_graph_stub_table_p)->stub_ucl_graph_first_input_link)
#define ucl_graph_erase_node_return_links  ((ucl_graph_stub_table_p)->stub_ucl_graph_erase_node_return_links)
#define ucl_graph_remove_next_node  ((ucl_graph_stub_table_p)->stub_ucl_graph_remove_next_node)
#define ucl_graph_insert_next_node  ((ucl_graph_stub_table_p)->stub_ucl_graph_insert_next_node)
#define ucl_graph_merge_upon_input_link  ((ucl_graph_stub_table_p)->stub_ucl_graph_merge_upon_input_link)
#define ucl_graph_merge_upon_output_link  ((ucl_graph_stub_table_p)->stub_ucl_graph_merge_upon_output_link)
#define ucl_graph_directed_depth_first_search  ((ucl_graph_stub_table_p)->stub_ucl_graph_directed_depth_first_search)
#define ucl_graph_initialise_dfs_handle  ((ucl_graph_stub_table_p)->stub_ucl_graph_initialise_dfs_handle)
#define ucl_graph_depth_first_search  ((ucl_graph_stub_table_p)->stub_ucl_graph_depth_first_search)
#define ucl_graph_finalise_dfs_handle  ((ucl_graph_stub_table_p)->stub_ucl_graph_finalise_dfs_handle)
#define ucl_graph_nodes_are_linked  ((ucl_graph_stub_table_p)->stub_ucl_graph_nodes_are_linked)
#define ucl_graph_nodes_are_connected  ((ucl_graph_stub_table_p)->stub_ucl_graph_nodes_are_connected)
#define ucl_graph_set_next_node    ((ucl_graph_stub_table_p)->stub_ucl_graph_set_next_node)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_GRAPH_H */

/* end of file */
