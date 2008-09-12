/* Stub header file for the "vector" module.
   This file must be included in the main stub header. */

#ifndef __UCL_VECTOR_H

/** ------------------------------------------------------------ 
 ** Module "vector".
 ** --------------------------------------------------------- */

/* Function prototypes for the "vector" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_vector_initialise (ucl_vector_t self, size_t slot_dimension);
ucl_decl void ucl_vector_initialise_size (ucl_vector_t self, size_t number_of_initial_slots);
ucl_decl void ucl_vector_initialise_pad (ucl_vector_t self, size_t size_of_padding_area);
ucl_decl void ucl_vector_initialise_step_up (ucl_vector_t self, size_t step_up);
ucl_decl void ucl_vector_initialise_step_down (ucl_vector_t self, size_t step_down);
ucl_decl void ucl_vector_initialise_buffer (ucl_vector_t self);
ucl_decl void ucl_vector_register_allocator (ucl_vector_t self, ucl_memory_allocator_t allocator);
ucl_decl void ucl_vector_constructor (ucl_vector_t self);
ucl_decl void ucl_vector_destructor (ucl_vector_t self);
ucl_decl void ucl_vector_swallow_block (ucl_vector_t self, ucl_block_t block);
ucl_decl size_t ucl_vector_number_of_step_up_slots (const ucl_vector_t self);
ucl_decl size_t ucl_vector_number_of_step_down_slots (const ucl_vector_t self);
ucl_decl size_t ucl_vector_number_of_padding_slots (const ucl_vector_t self);
ucl_decl ucl_bool_t ucl_vector_running (const ucl_vector_t self);
ucl_decl void ucl_vector_update_number_of_step_up_slots (ucl_vector_t self, size_t step_up);
ucl_decl void ucl_vector_update_number_of_step_down_slots (ucl_vector_t self, size_t step_down);
ucl_decl void ucl_vector_update_number_of_padding_slots (ucl_vector_t self, size_t padding);
ucl_decl void ucl_vector_mark_all_slots_as_used (ucl_vector_t self);
ucl_decl void ucl_vector_mark_allocated_range_as_used (ucl_vector_t self, ucl_range_t range);
ucl_decl void ucl_vector_reset (ucl_vector_t self);
ucl_decl void ucl_vector_clean (ucl_vector_t self);
ucl_decl void * ucl_vector_index_to_slot (const ucl_vector_t self, ucl_vector_index_t idx);
ucl_decl void * ucl_vector_index_to_new_slot (const ucl_vector_t self, ucl_vector_index_t idx);
ucl_decl ucl_vector_index_t ucl_vector_last_index (const ucl_vector_t self);
ucl_decl ucl_vector_index_t ucl_vector_slot_to_index (const ucl_vector_t self, const void *_pointer_to_slot_p);
ucl_decl ucl_bool_t ucl_vector_pointer_is_valid_slot (const ucl_vector_t self, const void *_pointer_to_slot_p);
ucl_decl ucl_bool_t ucl_vector_index_is_valid (const ucl_vector_t self, ucl_vector_index_t idx);
ucl_decl ucl_bool_t ucl_vector_index_is_valid_new (const ucl_vector_t self, ucl_vector_index_t idx);
ucl_decl ucl_bool_t ucl_vector_range_is_valid (const ucl_vector_t self, ucl_range_t range);
ucl_decl ucl_range_t ucl_vector_range (const ucl_vector_t self);
ucl_decl ucl_range_t ucl_vector_range_from_position_to_end (const ucl_vector_t self, ucl_vector_index_t position);
ucl_decl ucl_range_t ucl_vector_range_from_end_to_position (const ucl_vector_t self, ucl_vector_index_t position);
ucl_decl ucl_range_t ucl_vector_range_from_end_with_span (const ucl_vector_t self, size_t span);
ucl_decl void * ucl_vector_insert (ucl_vector_t self, void *_pointer_to_slot_p);
ucl_decl void * ucl_vector_insert_sort (ucl_vector_t self, void *data_p);
ucl_decl void ucl_vector_erase (ucl_vector_t self, void *_pointer_to_slot_p);
ucl_decl size_t ucl_vector_slot_dimension (const ucl_vector_t self);
ucl_decl size_t ucl_vector_size (const ucl_vector_t self);
ucl_decl void ucl_vector_set_compar (ucl_vector_t self, ucl_valcmp_t compar);
ucl_decl ucl_block_t ucl_vector_get_memory_block (const ucl_vector_t self);
ucl_decl ucl_block_t ucl_vector_get_data_block (const ucl_vector_t self);
ucl_decl ucl_block_t ucl_vector_get_free_block_at_end (ucl_vector_t self, size_t requested_slots);
ucl_decl ucl_block_t ucl_vector_get_free_block_at_beginning (ucl_vector_t self, size_t requested_slots);
ucl_decl void ucl_vector_mark_as_used (ucl_vector_t self, ucl_block_t used_block);
ucl_decl void ucl_vector_quick_sort (ucl_vector_t self);
ucl_decl ucl_bool_t ucl_vector_sorted (ucl_vector_t self);
ucl_decl ucl_block_t ucl_vector_block_from_range (const ucl_vector_t self, const ucl_range_t range);
ucl_decl ucl_range_t ucl_vector_range_from_block (const ucl_vector_t self, const ucl_block_t block);
ucl_decl void ucl_vector_iterator_forward (const ucl_vector_t self, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_backward (const ucl_vector_t self, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_range_forward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_range_backward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
ucl_decl ucl_bool_t ucl_vector_will_enlarge (ucl_vector_t self);
ucl_decl size_t ucl_vector_enlarged_size (ucl_vector_t self);
ucl_decl void ucl_vector_enlarge (ucl_vector_t self);
ucl_decl void ucl_vector_enlarge_for_slots (ucl_vector_t self, size_t required_free_slots);
ucl_decl void ucl_vector_enlarge_for_range (ucl_vector_t self, ucl_range_t range);
ucl_decl ucl_bool_t ucl_vector_will_restrict (ucl_vector_t self);
ucl_decl size_t ucl_vector_restricted_size (ucl_vector_t self);
ucl_decl void ucl_vector_restrict (ucl_vector_t self);
ucl_decl void ucl_vector_set_memory_to_zero (ucl_vector_t self);
ucl_decl size_t ucl_vector_number_of_free_slots (const ucl_vector_t self);
ucl_decl void * ucl_vector_find (const ucl_vector_t self, const void * data_p);
ucl_decl void * ucl_vector_sort_find (const ucl_vector_t self, const void * data_p);
ucl_decl void * ucl_vector_binary_search (const ucl_vector_t self, const void * data_p);
ucl_decl ucl_data_t ucl_vector_push_back (ucl_vector_t self);
ucl_decl ucl_data_t ucl_vector_push_front (ucl_vector_t self);
ucl_decl void ucl_vector_pop_back (ucl_vector_t self);
ucl_decl void ucl_vector_pop_front (ucl_vector_t self);
ucl_decl void ucl_vector_append_block (ucl_vector_t self, const ucl_block_t block);
ucl_decl void ucl_vector_append (ucl_vector_t target, const ucl_vector_t source);
ucl_decl void ucl_vector_append_range (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range);
ucl_decl void ucl_vector_append_more (ucl_vector_t target, const ucl_vector_t source, ...);
ucl_decl void ucl_vector_append_more_from_array (ucl_vector_t target, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_insert_block (ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block);
ucl_decl void ucl_vector_insert_range (ucl_vector_t V, ucl_vector_index_t offset, const ucl_vector_t A, ucl_range_t range);
ucl_decl void ucl_vector_insert_vector (ucl_vector_t T, ucl_vector_index_t offset, const ucl_vector_t S);
ucl_decl void ucl_vector_erase_range (ucl_vector_t self, ucl_range_t index_range);
ucl_decl void ucl_vector_copy_range (ucl_vector_t target, ucl_vector_index_t position, ucl_vector_t source, ucl_range_t source_range);
ucl_decl void ucl_vector_set_block (ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source);
ucl_decl void ucl_vector_get_block (ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source);
ucl_decl int ucl_vector_compare_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb);
ucl_decl int ucl_vector_compare (const ucl_vector_t a, const ucl_vector_t b);
ucl_decl ucl_bool_t ucl_vector_equal (const ucl_vector_t a, const ucl_vector_t b);
ucl_decl void ucl_vector_for_each (ucl_callback_t callback, const ucl_vector_t self);
ucl_decl void ucl_vector_for_each_in_range (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
ucl_decl void ucl_vector_for_each_multiple_from_array (ucl_callback_t callback, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_for_each_multiple (ucl_callback_t callback, const ucl_vector_t first, ...);
ucl_decl void ucl_vector_map (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self);
ucl_decl void ucl_vector_map_range (ucl_vector_t result, ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
ucl_decl void ucl_vector_map_multiple_from_array (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_map_multiple (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "vector" module.
   A typedef for each function in the module. */

typedef void ucl_vector_initialise_proto_t (ucl_vector_t self, size_t slot_dimension);
typedef void ucl_vector_initialise_size_proto_t (ucl_vector_t self, size_t number_of_initial_slots);
typedef void ucl_vector_initialise_pad_proto_t (ucl_vector_t self, size_t size_of_padding_area);
typedef void ucl_vector_initialise_step_up_proto_t (ucl_vector_t self, size_t step_up);
typedef void ucl_vector_initialise_step_down_proto_t (ucl_vector_t self, size_t step_down);
typedef void ucl_vector_initialise_buffer_proto_t (ucl_vector_t self);
typedef void ucl_vector_register_allocator_proto_t (ucl_vector_t self, ucl_memory_allocator_t allocator);
typedef void ucl_vector_constructor_proto_t (ucl_vector_t self);
typedef void ucl_vector_destructor_proto_t (ucl_vector_t self);
typedef void ucl_vector_swallow_block_proto_t (ucl_vector_t self, ucl_block_t block);
typedef size_t ucl_vector_number_of_step_up_slots_proto_t (const ucl_vector_t self);
typedef size_t ucl_vector_number_of_step_down_slots_proto_t (const ucl_vector_t self);
typedef size_t ucl_vector_number_of_padding_slots_proto_t (const ucl_vector_t self);
typedef ucl_bool_t ucl_vector_running_proto_t (const ucl_vector_t self);
typedef void ucl_vector_update_number_of_step_up_slots_proto_t (ucl_vector_t self, size_t step_up);
typedef void ucl_vector_update_number_of_step_down_slots_proto_t (ucl_vector_t self, size_t step_down);
typedef void ucl_vector_update_number_of_padding_slots_proto_t (ucl_vector_t self, size_t padding);
typedef void ucl_vector_mark_all_slots_as_used_proto_t (ucl_vector_t self);
typedef void ucl_vector_mark_allocated_range_as_used_proto_t (ucl_vector_t self, ucl_range_t range);
typedef void ucl_vector_reset_proto_t (ucl_vector_t self);
typedef void ucl_vector_clean_proto_t (ucl_vector_t self);
typedef void * ucl_vector_index_to_slot_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef void * ucl_vector_index_to_new_slot_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef ucl_vector_index_t ucl_vector_last_index_proto_t (const ucl_vector_t self);
typedef ucl_vector_index_t ucl_vector_slot_to_index_proto_t (const ucl_vector_t self, const void *_pointer_to_slot_p);
typedef ucl_bool_t ucl_vector_pointer_is_valid_slot_proto_t (const ucl_vector_t self, const void *_pointer_to_slot_p);
typedef ucl_bool_t ucl_vector_index_is_valid_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef ucl_bool_t ucl_vector_index_is_valid_new_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef ucl_bool_t ucl_vector_range_is_valid_proto_t (const ucl_vector_t self, ucl_range_t range);
typedef ucl_range_t ucl_vector_range_proto_t (const ucl_vector_t self);
typedef ucl_range_t ucl_vector_range_from_position_to_end_proto_t (const ucl_vector_t self, ucl_vector_index_t position);
typedef ucl_range_t ucl_vector_range_from_end_to_position_proto_t (const ucl_vector_t self, ucl_vector_index_t position);
typedef ucl_range_t ucl_vector_range_from_end_with_span_proto_t (const ucl_vector_t self, size_t span);
typedef void * ucl_vector_insert_proto_t (ucl_vector_t self, void *_pointer_to_slot_p);
typedef void * ucl_vector_insert_sort_proto_t (ucl_vector_t self, void *data_p);
typedef void ucl_vector_erase_proto_t (ucl_vector_t self, void *_pointer_to_slot_p);
typedef size_t ucl_vector_slot_dimension_proto_t (const ucl_vector_t self);
typedef size_t ucl_vector_size_proto_t (const ucl_vector_t self);
typedef void ucl_vector_set_compar_proto_t (ucl_vector_t self, ucl_valcmp_t compar);
typedef ucl_block_t ucl_vector_get_memory_block_proto_t (const ucl_vector_t self);
typedef ucl_block_t ucl_vector_get_data_block_proto_t (const ucl_vector_t self);
typedef ucl_block_t ucl_vector_get_free_block_at_end_proto_t (ucl_vector_t self, size_t requested_slots);
typedef ucl_block_t ucl_vector_get_free_block_at_beginning_proto_t (ucl_vector_t self, size_t requested_slots);
typedef void ucl_vector_mark_as_used_proto_t (ucl_vector_t self, ucl_block_t used_block);
typedef void ucl_vector_quick_sort_proto_t (ucl_vector_t self);
typedef ucl_bool_t ucl_vector_sorted_proto_t (ucl_vector_t self);
typedef ucl_block_t ucl_vector_block_from_range_proto_t (const ucl_vector_t self, const ucl_range_t range);
typedef ucl_range_t ucl_vector_range_from_block_proto_t (const ucl_vector_t self, const ucl_block_t block);
typedef void ucl_vector_iterator_forward_proto_t (const ucl_vector_t self, ucl_iterator_t iterator);
typedef void ucl_vector_iterator_backward_proto_t (const ucl_vector_t self, ucl_iterator_t iterator);
typedef void ucl_vector_iterator_range_forward_proto_t (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
typedef void ucl_vector_iterator_range_backward_proto_t (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
typedef ucl_bool_t ucl_vector_will_enlarge_proto_t (ucl_vector_t self);
typedef size_t ucl_vector_enlarged_size_proto_t (ucl_vector_t self);
typedef void ucl_vector_enlarge_proto_t (ucl_vector_t self);
typedef void ucl_vector_enlarge_for_slots_proto_t (ucl_vector_t self, size_t required_free_slots);
typedef void ucl_vector_enlarge_for_range_proto_t (ucl_vector_t self, ucl_range_t range);
typedef ucl_bool_t ucl_vector_will_restrict_proto_t (ucl_vector_t self);
typedef size_t ucl_vector_restricted_size_proto_t (ucl_vector_t self);
typedef void ucl_vector_restrict_proto_t (ucl_vector_t self);
typedef void ucl_vector_set_memory_to_zero_proto_t (ucl_vector_t self);
typedef size_t ucl_vector_number_of_free_slots_proto_t (const ucl_vector_t self);
typedef void * ucl_vector_find_proto_t (const ucl_vector_t self, const void * data_p);
typedef void * ucl_vector_sort_find_proto_t (const ucl_vector_t self, const void * data_p);
typedef void * ucl_vector_binary_search_proto_t (const ucl_vector_t self, const void * data_p);
typedef ucl_data_t ucl_vector_push_back_proto_t (ucl_vector_t self);
typedef ucl_data_t ucl_vector_push_front_proto_t (ucl_vector_t self);
typedef void ucl_vector_pop_back_proto_t (ucl_vector_t self);
typedef void ucl_vector_pop_front_proto_t (ucl_vector_t self);
typedef void ucl_vector_append_block_proto_t (ucl_vector_t self, const ucl_block_t block);
typedef void ucl_vector_append_proto_t (ucl_vector_t target, const ucl_vector_t source);
typedef void ucl_vector_append_range_proto_t (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range);
typedef void ucl_vector_append_more_proto_t (ucl_vector_t target, const ucl_vector_t source, ...);
typedef void ucl_vector_append_more_from_array_proto_t (ucl_vector_t target, const ucl_vector_array_t * vectors);
typedef void ucl_vector_insert_block_proto_t (ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block);
typedef void ucl_vector_insert_range_proto_t (ucl_vector_t V, ucl_vector_index_t offset, const ucl_vector_t A, ucl_range_t range);
typedef void ucl_vector_insert_vector_proto_t (ucl_vector_t T, ucl_vector_index_t offset, const ucl_vector_t S);
typedef void ucl_vector_erase_range_proto_t (ucl_vector_t self, ucl_range_t index_range);
typedef void ucl_vector_copy_range_proto_t (ucl_vector_t target, ucl_vector_index_t position, ucl_vector_t source, ucl_range_t source_range);
typedef void ucl_vector_set_block_proto_t (ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source);
typedef void ucl_vector_get_block_proto_t (ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source);
typedef int ucl_vector_compare_range_proto_t (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb);
typedef int ucl_vector_compare_proto_t (const ucl_vector_t a, const ucl_vector_t b);
typedef ucl_bool_t ucl_vector_equal_proto_t (const ucl_vector_t a, const ucl_vector_t b);
typedef void ucl_vector_for_each_proto_t (ucl_callback_t callback, const ucl_vector_t self);
typedef void ucl_vector_for_each_in_range_proto_t (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
typedef void ucl_vector_for_each_multiple_from_array_proto_t (ucl_callback_t callback, const ucl_vector_array_t * vectors);
typedef void ucl_vector_for_each_multiple_proto_t (ucl_callback_t callback, const ucl_vector_t first, ...);
typedef void ucl_vector_map_proto_t (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self);
typedef void ucl_vector_map_range_proto_t (ucl_vector_t result, ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
typedef void ucl_vector_map_multiple_from_array_proto_t (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_array_t * vectors);
typedef void ucl_vector_map_multiple_proto_t (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...);

/* Stub table type declaration for the "vector" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_vector_stub_table_t {
  ucl_vector_initialise_proto_t       * stub_ucl_vector_initialise;
  ucl_vector_initialise_size_proto_t  * stub_ucl_vector_initialise_size;
  ucl_vector_initialise_pad_proto_t   * stub_ucl_vector_initialise_pad;
  ucl_vector_initialise_step_up_proto_t * stub_ucl_vector_initialise_step_up;
  ucl_vector_initialise_step_down_proto_t * stub_ucl_vector_initialise_step_down;
  ucl_vector_constructor_proto_t      * stub_ucl_vector_constructor;
  ucl_vector_destructor_proto_t       * stub_ucl_vector_destructor;
  ucl_vector_number_of_step_up_slots_proto_t * stub_ucl_vector_number_of_step_up_slots;
  ucl_vector_number_of_step_down_slots_proto_t * stub_ucl_vector_number_of_step_down_slots;
  ucl_vector_number_of_padding_slots_proto_t * stub_ucl_vector_number_of_padding_slots;
  ucl_vector_index_to_slot_proto_t    * stub_ucl_vector_index_to_slot;
  ucl_vector_index_to_new_slot_proto_t * stub_ucl_vector_index_to_new_slot;
  ucl_vector_last_index_proto_t       * stub_ucl_vector_last_index;
  ucl_vector_slot_to_index_proto_t    * stub_ucl_vector_slot_to_index;
  ucl_vector_pointer_is_valid_slot_proto_t * stub_ucl_vector_pointer_is_valid_slot;
  ucl_vector_index_is_valid_proto_t   * stub_ucl_vector_index_is_valid;
  ucl_vector_range_is_valid_proto_t   * stub_ucl_vector_range_is_valid;
  ucl_vector_insert_proto_t           * stub_ucl_vector_insert;
  ucl_vector_insert_sort_proto_t      * stub_ucl_vector_insert_sort;
  ucl_vector_erase_proto_t            * stub_ucl_vector_erase;
  ucl_vector_slot_dimension_proto_t   * stub_ucl_vector_slot_dimension;
  ucl_vector_size_proto_t             * stub_ucl_vector_size;
  ucl_vector_set_compar_proto_t       * stub_ucl_vector_set_compar;
  ucl_vector_get_memory_block_proto_t * stub_ucl_vector_get_memory_block;
  ucl_vector_get_data_block_proto_t   * stub_ucl_vector_get_data_block;
  ucl_vector_quick_sort_proto_t       * stub_ucl_vector_quick_sort;
  ucl_vector_iterator_forward_proto_t * stub_ucl_vector_iterator_forward;
  ucl_vector_iterator_backward_proto_t * stub_ucl_vector_iterator_backward;
  ucl_vector_enlarge_proto_t          * stub_ucl_vector_enlarge;
  ucl_vector_restrict_proto_t         * stub_ucl_vector_restrict;
  ucl_vector_set_memory_to_zero_proto_t * stub_ucl_vector_set_memory_to_zero;
  ucl_vector_number_of_free_slots_proto_t * stub_ucl_vector_number_of_free_slots;
  ucl_vector_find_proto_t             * stub_ucl_vector_find;
  ucl_vector_sort_find_proto_t        * stub_ucl_vector_sort_find;
  ucl_vector_binary_search_proto_t    * stub_ucl_vector_binary_search;
  ucl_vector_append_range_proto_t     * stub_ucl_vector_append_range;
  ucl_vector_append_proto_t           * stub_ucl_vector_append;
  ucl_vector_append_more_proto_t      * stub_ucl_vector_append_more;
  ucl_vector_insert_vector_proto_t    * stub_ucl_vector_insert_vector;
  ucl_vector_erase_range_proto_t      * stub_ucl_vector_erase_range;
  ucl_vector_compare_range_proto_t    * stub_ucl_vector_compare_range;
  ucl_vector_compare_proto_t          * stub_ucl_vector_compare;
  ucl_vector_equal_proto_t            * stub_ucl_vector_equal;
  ucl_vector_mark_all_slots_as_used_proto_t * stub_ucl_vector_mark_all_slots_as_used;
  ucl_vector_enlarge_for_slots_proto_t * stub_ucl_vector_enlarge_for_slots;
  ucl_vector_append_block_proto_t     * stub_ucl_vector_append_block;
  ucl_vector_initialise_buffer_proto_t * stub_ucl_vector_initialise_buffer;
  ucl_vector_swallow_block_proto_t    * stub_ucl_vector_swallow_block;
  ucl_vector_running_proto_t          * stub_ucl_vector_running;
  ucl_vector_push_back_proto_t        * stub_ucl_vector_push_back;
  ucl_vector_push_front_proto_t       * stub_ucl_vector_push_front;
  ucl_vector_pop_back_proto_t         * stub_ucl_vector_pop_back;
  ucl_vector_pop_front_proto_t        * stub_ucl_vector_pop_front;
  ucl_vector_reset_proto_t            * stub_ucl_vector_reset;
  ucl_vector_clean_proto_t            * stub_ucl_vector_clean;
  ucl_vector_register_allocator_proto_t * stub_ucl_vector_register_allocator;
  ucl_vector_sorted_proto_t           * stub_ucl_vector_sorted;
  ucl_vector_block_from_range_proto_t * stub_ucl_vector_block_from_range;
  ucl_vector_range_from_block_proto_t * stub_ucl_vector_range_from_block;
  ucl_vector_get_free_block_at_end_proto_t * stub_ucl_vector_get_free_block_at_end;
  ucl_vector_get_free_block_at_beginning_proto_t * stub_ucl_vector_get_free_block_at_beginning;
  ucl_vector_mark_as_used_proto_t     * stub_ucl_vector_mark_as_used;
  ucl_vector_update_number_of_step_up_slots_proto_t * stub_ucl_vector_update_number_of_step_up_slots;
  ucl_vector_update_number_of_step_down_slots_proto_t * stub_ucl_vector_update_number_of_step_down_slots;
  ucl_vector_update_number_of_padding_slots_proto_t * stub_ucl_vector_update_number_of_padding_slots;
  ucl_vector_for_each_proto_t         * stub_ucl_vector_for_each;
  ucl_vector_for_each_multiple_from_array_proto_t * stub_ucl_vector_for_each_multiple_from_array;
  ucl_vector_for_each_multiple_proto_t * stub_ucl_vector_for_each_multiple;
  ucl_vector_map_proto_t              * stub_ucl_vector_map;
  ucl_vector_for_each_in_range_proto_t * stub_ucl_vector_for_each_in_range;
  ucl_vector_map_range_proto_t        * stub_ucl_vector_map_range;
  ucl_vector_iterator_range_forward_proto_t * stub_ucl_vector_iterator_range_forward;
  ucl_vector_iterator_range_backward_proto_t * stub_ucl_vector_iterator_range_backward;
  ucl_vector_map_multiple_from_array_proto_t * stub_ucl_vector_map_multiple_from_array;
  ucl_vector_map_multiple_proto_t     * stub_ucl_vector_map_multiple;
  ucl_vector_range_proto_t            * stub_ucl_vector_range;
  ucl_vector_range_from_position_to_end_proto_t * stub_ucl_vector_range_from_position_to_end;
  ucl_vector_range_from_end_to_position_proto_t * stub_ucl_vector_range_from_end_to_position;
  ucl_vector_range_from_end_with_span_proto_t * stub_ucl_vector_range_from_end_with_span;
  ucl_vector_enlarge_for_range_proto_t * stub_ucl_vector_enlarge_for_range;
  ucl_vector_append_more_from_array_proto_t * stub_ucl_vector_append_more_from_array;
  ucl_vector_insert_range_proto_t     * stub_ucl_vector_insert_range;
  ucl_vector_insert_block_proto_t     * stub_ucl_vector_insert_block;
  ucl_vector_copy_range_proto_t       * stub_ucl_vector_copy_range;
  ucl_vector_set_block_proto_t        * stub_ucl_vector_set_block;
  ucl_vector_get_block_proto_t        * stub_ucl_vector_get_block;
  ucl_vector_index_is_valid_new_proto_t * stub_ucl_vector_index_is_valid_new;
  ucl_vector_will_enlarge_proto_t     * stub_ucl_vector_will_enlarge;
  ucl_vector_will_restrict_proto_t    * stub_ucl_vector_will_restrict;
  ucl_vector_enlarged_size_proto_t    * stub_ucl_vector_enlarged_size;
  ucl_vector_restricted_size_proto_t  * stub_ucl_vector_restricted_size;
  ucl_vector_mark_allocated_range_as_used_proto_t * stub_ucl_vector_mark_allocated_range_as_used;
} ucl_vector_stub_table_t;

/* Stub table macros definition for the "vector" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_vector_stub_table_t * ucl_vector_stub_table_p;
ucl_decl const ucl_vector_stub_table_t   ucl_vector_stub_table;

#define ucl_vector_initialise      ((ucl_vector_stub_table_p)->stub_ucl_vector_initialise)
#define ucl_vector_initialise_size  ((ucl_vector_stub_table_p)->stub_ucl_vector_initialise_size)
#define ucl_vector_initialise_pad  ((ucl_vector_stub_table_p)->stub_ucl_vector_initialise_pad)
#define ucl_vector_initialise_step_up  ((ucl_vector_stub_table_p)->stub_ucl_vector_initialise_step_up)
#define ucl_vector_initialise_step_down  ((ucl_vector_stub_table_p)->stub_ucl_vector_initialise_step_down)
#define ucl_vector_constructor     ((ucl_vector_stub_table_p)->stub_ucl_vector_constructor)
#define ucl_vector_destructor      ((ucl_vector_stub_table_p)->stub_ucl_vector_destructor)
#define ucl_vector_number_of_step_up_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_number_of_step_up_slots)
#define ucl_vector_number_of_step_down_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_number_of_step_down_slots)
#define ucl_vector_number_of_padding_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_number_of_padding_slots)
#define ucl_vector_index_to_slot   ((ucl_vector_stub_table_p)->stub_ucl_vector_index_to_slot)
#define ucl_vector_index_to_new_slot  ((ucl_vector_stub_table_p)->stub_ucl_vector_index_to_new_slot)
#define ucl_vector_last_index      ((ucl_vector_stub_table_p)->stub_ucl_vector_last_index)
#define ucl_vector_slot_to_index   ((ucl_vector_stub_table_p)->stub_ucl_vector_slot_to_index)
#define ucl_vector_pointer_is_valid_slot  ((ucl_vector_stub_table_p)->stub_ucl_vector_pointer_is_valid_slot)
#define ucl_vector_index_is_valid  ((ucl_vector_stub_table_p)->stub_ucl_vector_index_is_valid)
#define ucl_vector_range_is_valid  ((ucl_vector_stub_table_p)->stub_ucl_vector_range_is_valid)
#define ucl_vector_insert          ((ucl_vector_stub_table_p)->stub_ucl_vector_insert)
#define ucl_vector_insert_sort     ((ucl_vector_stub_table_p)->stub_ucl_vector_insert_sort)
#define ucl_vector_erase           ((ucl_vector_stub_table_p)->stub_ucl_vector_erase)
#define ucl_vector_slot_dimension  ((ucl_vector_stub_table_p)->stub_ucl_vector_slot_dimension)
#define ucl_vector_size            ((ucl_vector_stub_table_p)->stub_ucl_vector_size)
#define ucl_vector_set_compar      ((ucl_vector_stub_table_p)->stub_ucl_vector_set_compar)
#define ucl_vector_get_memory_block  ((ucl_vector_stub_table_p)->stub_ucl_vector_get_memory_block)
#define ucl_vector_get_data_block  ((ucl_vector_stub_table_p)->stub_ucl_vector_get_data_block)
#define ucl_vector_quick_sort      ((ucl_vector_stub_table_p)->stub_ucl_vector_quick_sort)
#define ucl_vector_iterator_forward  ((ucl_vector_stub_table_p)->stub_ucl_vector_iterator_forward)
#define ucl_vector_iterator_backward  ((ucl_vector_stub_table_p)->stub_ucl_vector_iterator_backward)
#define ucl_vector_enlarge         ((ucl_vector_stub_table_p)->stub_ucl_vector_enlarge)
#define ucl_vector_restrict        ((ucl_vector_stub_table_p)->stub_ucl_vector_restrict)
#define ucl_vector_set_memory_to_zero  ((ucl_vector_stub_table_p)->stub_ucl_vector_set_memory_to_zero)
#define ucl_vector_number_of_free_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_number_of_free_slots)
#define ucl_vector_find            ((ucl_vector_stub_table_p)->stub_ucl_vector_find)
#define ucl_vector_sort_find       ((ucl_vector_stub_table_p)->stub_ucl_vector_sort_find)
#define ucl_vector_binary_search   ((ucl_vector_stub_table_p)->stub_ucl_vector_binary_search)
#define ucl_vector_append_range    ((ucl_vector_stub_table_p)->stub_ucl_vector_append_range)
#define ucl_vector_append          ((ucl_vector_stub_table_p)->stub_ucl_vector_append)
#define ucl_vector_append_more     ((ucl_vector_stub_table_p)->stub_ucl_vector_append_more)
#define ucl_vector_insert_vector   ((ucl_vector_stub_table_p)->stub_ucl_vector_insert_vector)
#define ucl_vector_erase_range     ((ucl_vector_stub_table_p)->stub_ucl_vector_erase_range)
#define ucl_vector_compare_range   ((ucl_vector_stub_table_p)->stub_ucl_vector_compare_range)
#define ucl_vector_compare         ((ucl_vector_stub_table_p)->stub_ucl_vector_compare)
#define ucl_vector_equal           ((ucl_vector_stub_table_p)->stub_ucl_vector_equal)
#define ucl_vector_mark_all_slots_as_used  ((ucl_vector_stub_table_p)->stub_ucl_vector_mark_all_slots_as_used)
#define ucl_vector_enlarge_for_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_enlarge_for_slots)
#define ucl_vector_append_block    ((ucl_vector_stub_table_p)->stub_ucl_vector_append_block)
#define ucl_vector_initialise_buffer  ((ucl_vector_stub_table_p)->stub_ucl_vector_initialise_buffer)
#define ucl_vector_swallow_block   ((ucl_vector_stub_table_p)->stub_ucl_vector_swallow_block)
#define ucl_vector_running         ((ucl_vector_stub_table_p)->stub_ucl_vector_running)
#define ucl_vector_push_back       ((ucl_vector_stub_table_p)->stub_ucl_vector_push_back)
#define ucl_vector_push_front      ((ucl_vector_stub_table_p)->stub_ucl_vector_push_front)
#define ucl_vector_pop_back        ((ucl_vector_stub_table_p)->stub_ucl_vector_pop_back)
#define ucl_vector_pop_front       ((ucl_vector_stub_table_p)->stub_ucl_vector_pop_front)
#define ucl_vector_reset           ((ucl_vector_stub_table_p)->stub_ucl_vector_reset)
#define ucl_vector_clean           ((ucl_vector_stub_table_p)->stub_ucl_vector_clean)
#define ucl_vector_register_allocator  ((ucl_vector_stub_table_p)->stub_ucl_vector_register_allocator)
#define ucl_vector_sorted          ((ucl_vector_stub_table_p)->stub_ucl_vector_sorted)
#define ucl_vector_block_from_range  ((ucl_vector_stub_table_p)->stub_ucl_vector_block_from_range)
#define ucl_vector_range_from_block  ((ucl_vector_stub_table_p)->stub_ucl_vector_range_from_block)
#define ucl_vector_get_free_block_at_end  ((ucl_vector_stub_table_p)->stub_ucl_vector_get_free_block_at_end)
#define ucl_vector_get_free_block_at_beginning  ((ucl_vector_stub_table_p)->stub_ucl_vector_get_free_block_at_beginning)
#define ucl_vector_mark_as_used    ((ucl_vector_stub_table_p)->stub_ucl_vector_mark_as_used)
#define ucl_vector_update_number_of_step_up_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_update_number_of_step_up_slots)
#define ucl_vector_update_number_of_step_down_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_update_number_of_step_down_slots)
#define ucl_vector_update_number_of_padding_slots  ((ucl_vector_stub_table_p)->stub_ucl_vector_update_number_of_padding_slots)
#define ucl_vector_for_each        ((ucl_vector_stub_table_p)->stub_ucl_vector_for_each)
#define ucl_vector_for_each_multiple_from_array  ((ucl_vector_stub_table_p)->stub_ucl_vector_for_each_multiple_from_array)
#define ucl_vector_for_each_multiple  ((ucl_vector_stub_table_p)->stub_ucl_vector_for_each_multiple)
#define ucl_vector_map             ((ucl_vector_stub_table_p)->stub_ucl_vector_map)
#define ucl_vector_for_each_in_range  ((ucl_vector_stub_table_p)->stub_ucl_vector_for_each_in_range)
#define ucl_vector_map_range       ((ucl_vector_stub_table_p)->stub_ucl_vector_map_range)
#define ucl_vector_iterator_range_forward  ((ucl_vector_stub_table_p)->stub_ucl_vector_iterator_range_forward)
#define ucl_vector_iterator_range_backward  ((ucl_vector_stub_table_p)->stub_ucl_vector_iterator_range_backward)
#define ucl_vector_map_multiple_from_array  ((ucl_vector_stub_table_p)->stub_ucl_vector_map_multiple_from_array)
#define ucl_vector_map_multiple    ((ucl_vector_stub_table_p)->stub_ucl_vector_map_multiple)
#define ucl_vector_range           ((ucl_vector_stub_table_p)->stub_ucl_vector_range)
#define ucl_vector_range_from_position_to_end  ((ucl_vector_stub_table_p)->stub_ucl_vector_range_from_position_to_end)
#define ucl_vector_range_from_end_to_position  ((ucl_vector_stub_table_p)->stub_ucl_vector_range_from_end_to_position)
#define ucl_vector_range_from_end_with_span  ((ucl_vector_stub_table_p)->stub_ucl_vector_range_from_end_with_span)
#define ucl_vector_enlarge_for_range  ((ucl_vector_stub_table_p)->stub_ucl_vector_enlarge_for_range)
#define ucl_vector_append_more_from_array  ((ucl_vector_stub_table_p)->stub_ucl_vector_append_more_from_array)
#define ucl_vector_insert_range    ((ucl_vector_stub_table_p)->stub_ucl_vector_insert_range)
#define ucl_vector_insert_block    ((ucl_vector_stub_table_p)->stub_ucl_vector_insert_block)
#define ucl_vector_copy_range      ((ucl_vector_stub_table_p)->stub_ucl_vector_copy_range)
#define ucl_vector_set_block       ((ucl_vector_stub_table_p)->stub_ucl_vector_set_block)
#define ucl_vector_get_block       ((ucl_vector_stub_table_p)->stub_ucl_vector_get_block)
#define ucl_vector_index_is_valid_new  ((ucl_vector_stub_table_p)->stub_ucl_vector_index_is_valid_new)
#define ucl_vector_will_enlarge    ((ucl_vector_stub_table_p)->stub_ucl_vector_will_enlarge)
#define ucl_vector_will_restrict   ((ucl_vector_stub_table_p)->stub_ucl_vector_will_restrict)
#define ucl_vector_enlarged_size   ((ucl_vector_stub_table_p)->stub_ucl_vector_enlarged_size)
#define ucl_vector_restricted_size  ((ucl_vector_stub_table_p)->stub_ucl_vector_restricted_size)
#define ucl_vector_mark_allocated_range_as_used  ((ucl_vector_stub_table_p)->stub_ucl_vector_mark_allocated_range_as_used)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_VECTOR_H */

/* end of file */
