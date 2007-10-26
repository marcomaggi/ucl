/* Header files. */

#include "ucl.h"


/* Functions stub table for the "btree" module. */

static const ucl_btree_stub_table_t ucl_btree_stub_table = {
  ucl_btree_constructor,
  ucl_btree_find_leftmost,
  ucl_btree_step_inorder,
  ucl_btree_step_inorder_backward,
  ucl_btree_step_preorder,
  ucl_btree_step_postorder,
  ucl_btree_step_levelorder,
  ucl_btree_find_rightmost,
  ucl_btree_find_deepest_son,
  ucl_btree_destructor,
  ucl_btree_setbro,
  ucl_btree_setson,
  ucl_btree_setdad,
  ucl_btree_getbro,
  ucl_btree_getson,
  ucl_btree_getdad,
  ucl_btree_dadbro,
  ucl_btree_dadson
};

/* Functions stub table for the "base" module. */

static const ucl_base_stub_table_t ucl_base_stub_table = {
  ucl_version,
  ucl_major_version,
  ucl_minor_version,
  ucl_patch_version,
  ucl_intcmp,
  ucl_uintcmp,
  ucl_ptrintcmp,
  ucl_hash_string,
  ucl_interface_major_version,
  ucl_interface_minor_version,
  ucl_strcmp,
  ucl_quicksort
};

/* Functions stub table for the "circular" module. */

static const ucl_circular_stub_table_t ucl_circular_stub_table = {
  ucl_circular_insert,
  ucl_circular_extract,
  ucl_circular_forward,
  ucl_circular_find
};

/* Functions stub table for the "debug" module. */

static const ucl_debug_stub_table_t ucl_debug_stub_table = {
  ucl_debug_on,
  ucl_debug_print,
  ucl_debug_off,
  ucl_question
};

/* Functions stub table for the "graph" module. */

static const ucl_graph_stub_table_t ucl_graph_stub_table = {
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

/* Functions stub table for the "hash" module. */

static const ucl_hash_stub_table_t ucl_hash_stub_table = {
  ucl_hash_constructor,
  ucl_hash_insert,
  ucl_hash_extract,
  ucl_hash_find,
  ucl_hash_enlarge,
  ucl_hash_iterator,
  ucl_hash_destructor,
  ucl_hash_register_allocator
};

/* Functions stub table for the "heap" module. */

static const ucl_heap_stub_table_t ucl_heap_stub_table = {
  ucl_heap_constructor,
  ucl_heap_insert,
  ucl_heap_extract,
  ucl_heap_merge
};

/* Functions stub table for the "list" module. */

static const ucl_list_stub_table_t ucl_list_stub_table = {
  ucl_list_index,
  ucl_list_insertbefore,
  ucl_list_insertafter,
  ucl_list_pushfront,
  ucl_list_pushback,
  ucl_list_extract
};

/* Functions stub table for the "map" module. */

static const ucl_map_stub_table_t ucl_map_stub_table = {
  ucl_map_constructor,
  ucl_map_insert,
  ucl_map_remove,
  ucl_map_find,
  ucl_map_first,
  ucl_map_last,
  ucl_map_next,
  ucl_map_prev,
  ucl_map_find_or_next,
  ucl_map_find_or_prev,
  ucl_map_count,
  ucl_map_iterator_inorder,
  ucl_map_iterator_inorder_backward,
  ucl_map_iterator_preorder,
  ucl_map_iterator_postorder,
  ucl_map_iterator_levelorder,
  ucl_map_lower_bound,
  ucl_map_upper_bound,
  ucl_map_iterator_union,
  ucl_map_iterator_intersection,
  ucl_map_iterator_complintersect,
  ucl_map_iterator_subtraction,
  ucl_map_destructor
};

/* Functions stub table for the "memory" module. */

static const ucl_memory_stub_table_t ucl_memory_stub_table = {
  ucl_memory_alloc
};

/* Functions stub table for the "tree" module. */

static const ucl_tree_stub_table_t ucl_tree_stub_table = {
  ucl_tree_is_dad,
  ucl_tree_is_bro,
  ucl_tree_has_dad,
  ucl_tree_has_prev,
  ucl_tree_get_dad,
  ucl_tree_get_prev,
  ucl_tree_get_first,
  ucl_tree_get_last,
  ucl_tree_insert_dad,
  ucl_tree_insert_son,
  ucl_tree_insert_next,
  ucl_tree_insert_prev,
  ucl_tree_extract_dad,
  ucl_tree_extract_son,
  ucl_tree_extract_prev,
  ucl_tree_extract_next,
  ucl_tree_iterator_inorder,
  ucl_tree_iterator_preorder,
  ucl_tree_iterator_postorder
};

/* Functions stub table for the "vector" module. */

static const ucl_vector_stub_table_t ucl_vector_stub_table = {
  ucl_vector_initialise,
  ucl_vector_initialise_size,
  ucl_vector_initialise_pad,
  ucl_vector_initialise_step_up,
  ucl_vector_initialise_step_down,
  ucl_vector_constructor,
  ucl_vector_destructor,
  ucl_vector_number_of_step_up_slots,
  ucl_vector_number_of_step_down_slots,
  ucl_vector_number_of_padding_slots,
  ucl_vector_index_to_slot,
  ucl_vector_index_to_new_slot,
  ucl_vector_last_index,
  ucl_vector_slot_to_index,
  ucl_vector_pointer_is_valid_slot,
  ucl_vector_index_is_valid_index,
  ucl_vector_range_is_valid,
  ucl_vector_insert,
  ucl_vector_insert_sort,
  ucl_vector_erase,
  ucl_vector_slot_dimension,
  ucl_vector_size,
  ucl_vector_set_compar,
  ucl_vector_get_memory_block,
  ucl_vector_get_data_block,
  ucl_vector_quick_sort,
  ucl_vector_iterator_forward,
  ucl_vector_iterator_backward,
  ucl_vector_enlarge,
  ucl_vector_restrict,
  ucl_vector_set_memory_to_zero,
  ucl_vector_number_of_free_slots,
  ucl_vector_find,
  ucl_vector_sort_find,
  ucl_vector_binary_search,
  ucl_vector_append_range,
  ucl_vector_append,
  ucl_vector_append_more,
  ucl_vector_insert_vector,
  ucl_vector_erase_range,
  ucl_vector_compare_range,
  ucl_vector_compare,
  ucl_vector_equal,
  ucl_vector_mark_all_slots_as_used,
  ucl_vector_enlarge_for_slots,
  ucl_vector_append_block,
  ucl_vector_initialise_buffer,
  ucl_vector_swallow_block,
  ucl_vector_running,
  ucl_vector_push_back,
  ucl_vector_push_front,
  ucl_vector_pop_back,
  ucl_vector_pop_front,
  ucl_vector_reset,
  ucl_vector_clean,
  ucl_vector_register_allocator,
  ucl_vector_sorted,
  ucl_vector_block_from_range,
  ucl_vector_range_from_block,
  ucl_vector_get_free_block_at_end,
  ucl_vector_get_free_block_at_beginning,
  ucl_vector_mark_as_used,
  ucl_vector_update_number_of_step_up_slots,
  ucl_vector_update_number_of_step_down_slots,
  ucl_vector_update_number_of_padding_slots,
  ucl_vector_for_each,
  ucl_vector_for_each_multiple_from_array,
  ucl_vector_for_each_multiple,
  ucl_vector_map,
  ucl_vector_for_each_in_range,
  ucl_vector_map_range,
  ucl_vector_iterator_range_forward,
  ucl_vector_iterator_range_backward,
  ucl_vector_map_multiple_from_array,
  ucl_vector_map_multiple,
  ucl_vector_range,
  ucl_vector_range_from_position_to_end,
  ucl_vector_range_from_end_to_position,
  ucl_vector_range_from_end_with_span,
  ucl_vector_enlarge_for_range,
  ucl_vector_append_more_from_array,
  ucl_vector_insert_range,
  ucl_vector_insert_block,
  ucl_vector_copy_range,
  ucl_vector_set_block,
  ucl_vector_get_block
};

/* Library stub table.
   A pointer for each module stub table. */

const ucl_stub_table_t ucl_stub_table = {
  &ucl_btree_stub_table,
  &ucl_base_stub_table,
  &ucl_circular_stub_table,
  &ucl_debug_stub_table,
  &ucl_graph_stub_table,
  &ucl_hash_stub_table,
  &ucl_heap_stub_table,
  &ucl_list_stub_table,
  &ucl_map_stub_table,
  &ucl_memory_stub_table,
  &ucl_tree_stub_table,
  &ucl_vector_stub_table
};



/* end of file */
