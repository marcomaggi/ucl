

#ifndef __UCL_STUB_H
#define __UCL_STUB_H


/* ------------------------------------------------------------ 
   Module "btree".
   ------------------------------------------------------------ */

/* Function type declarations for the "btree" module.
   A typedef for each function in the module. */

typedef void       ucl_btree_constructor_proto_t (ucl_btree_node_t * this_p, const ucl_btree_node_t * dad_p);
typedef ucl_btree_node_t * ucl_btree_find_leftmost_proto_t (const ucl_btree_node_t *node_p);
typedef ucl_btree_node_t * ucl_btree_step_inorder_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_inorder_backward_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_preorder_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_postorder_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_levelorder_proto_t (const ucl_btree_node_t * cur_p);
typedef ucl_btree_node_t * ucl_btree_find_rightmost_proto_t (const ucl_btree_node_t *node_p);
typedef ucl_btree_node_t * ucl_btree_find_deepest_son_proto_t (const ucl_btree_node_t *node_p);
typedef void       ucl_btree_destructor_proto_t (ucl_btree_node_t * this);
typedef void       ucl_btree_setbro_proto_t   (ucl_btree_node_t * this, const ucl_btree_node_t * newBro);
typedef void       ucl_btree_setson_proto_t   (ucl_btree_node_t * this, const ucl_btree_node_t * newSon);
typedef void       ucl_btree_setdad_proto_t   (ucl_btree_node_t * this, const ucl_btree_node_t * newDad);
typedef ucl_btree_node_t * ucl_btree_getbro_proto_t (const ucl_btree_node_t * this);
typedef ucl_btree_node_t * ucl_btree_getson_proto_t (const ucl_btree_node_t * this);
typedef ucl_btree_node_t * ucl_btree_getdad_proto_t (const ucl_btree_node_t * this);
typedef void       ucl_btree_dadbro_proto_t   (ucl_btree_node_t * dad, ucl_btree_node_t * bro);
typedef void       ucl_btree_dadson_proto_t   (ucl_btree_node_t * dad, ucl_btree_node_t * son);

/* Stub table type declaration for the "btree" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_btree_stub_table_t {
  ucl_btree_constructor_proto_t       * stub_ucl_btree_constructor;
  ucl_btree_find_leftmost_proto_t     * stub_ucl_btree_find_leftmost;
  ucl_btree_step_inorder_proto_t      * stub_ucl_btree_step_inorder;
  ucl_btree_step_inorder_backward_proto_t * stub_ucl_btree_step_inorder_backward;
  ucl_btree_step_preorder_proto_t     * stub_ucl_btree_step_preorder;
  ucl_btree_step_postorder_proto_t    * stub_ucl_btree_step_postorder;
  ucl_btree_step_levelorder_proto_t   * stub_ucl_btree_step_levelorder;
  ucl_btree_find_rightmost_proto_t    * stub_ucl_btree_find_rightmost;
  ucl_btree_find_deepest_son_proto_t  * stub_ucl_btree_find_deepest_son;
  ucl_btree_destructor_proto_t        * stub_ucl_btree_destructor;
  ucl_btree_setbro_proto_t            * stub_ucl_btree_setbro;
  ucl_btree_setson_proto_t            * stub_ucl_btree_setson;
  ucl_btree_setdad_proto_t            * stub_ucl_btree_setdad;
  ucl_btree_getbro_proto_t            * stub_ucl_btree_getbro;
  ucl_btree_getson_proto_t            * stub_ucl_btree_getson;
  ucl_btree_getdad_proto_t            * stub_ucl_btree_getdad;
  ucl_btree_dadbro_proto_t            * stub_ucl_btree_dadbro;
  ucl_btree_dadson_proto_t            * stub_ucl_btree_dadson;
} ucl_btree_stub_table_t;

/* Stub table macros definition for the "btree" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_btree_stub_table_t * ucl_btree_stub_table_p;

#define ucl_btree_constructor      ((ucl_btree_stub_table_p)->stub_ucl_btree_constructor)
#define ucl_btree_find_leftmost    ((ucl_btree_stub_table_p)->stub_ucl_btree_find_leftmost)
#define ucl_btree_step_inorder     ((ucl_btree_stub_table_p)->stub_ucl_btree_step_inorder)
#define ucl_btree_step_inorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_inorder_backward)
#define ucl_btree_step_preorder    ((ucl_btree_stub_table_p)->stub_ucl_btree_step_preorder)
#define ucl_btree_step_postorder   ((ucl_btree_stub_table_p)->stub_ucl_btree_step_postorder)
#define ucl_btree_step_levelorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_levelorder)
#define ucl_btree_find_rightmost   ((ucl_btree_stub_table_p)->stub_ucl_btree_find_rightmost)
#define ucl_btree_find_deepest_son  ((ucl_btree_stub_table_p)->stub_ucl_btree_find_deepest_son)
#define ucl_btree_destructor       ((ucl_btree_stub_table_p)->stub_ucl_btree_destructor)
#define ucl_btree_setbro           ((ucl_btree_stub_table_p)->stub_ucl_btree_setbro)
#define ucl_btree_setson           ((ucl_btree_stub_table_p)->stub_ucl_btree_setson)
#define ucl_btree_setdad           ((ucl_btree_stub_table_p)->stub_ucl_btree_setdad)
#define ucl_btree_getbro           ((ucl_btree_stub_table_p)->stub_ucl_btree_getbro)
#define ucl_btree_getson           ((ucl_btree_stub_table_p)->stub_ucl_btree_getson)
#define ucl_btree_getdad           ((ucl_btree_stub_table_p)->stub_ucl_btree_getdad)
#define ucl_btree_dadbro           ((ucl_btree_stub_table_p)->stub_ucl_btree_dadbro)
#define ucl_btree_dadson           ((ucl_btree_stub_table_p)->stub_ucl_btree_dadson)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "base".
   ------------------------------------------------------------ */

/* Function type declarations for the "base" module.
   A typedef for each function in the module. */

typedef const char * ucl_version_proto_t      (void);
typedef const char * ucl_major_version_proto_t (void);
typedef const char * ucl_minor_version_proto_t (void);
typedef const char * ucl_patch_version_proto_t (void);
typedef int        ucl_intcmp_proto_t         (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
typedef int        ucl_uintcmp_proto_t        (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
typedef int        ucl_ptrintcmp_proto_t      (ucl_value_t data, const ucl_value_t a, const ucl_value_t b);
typedef size_t     ucl_hash_string_proto_t    (ucl_value_t data UCL_UNUSED, const ucl_value_t val);
typedef unsigned   ucl_interface_major_version_proto_t (void);
typedef unsigned   ucl_interface_minor_version_proto_t (void);
typedef int        ucl_strcmp_proto_t         (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
typedef void       ucl_quicksort_proto_t      (void *const pbase, size_t total_elems, size_t size, ucl_valcmp_t cmp);

/* Stub table type declaration for the "base" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_base_stub_table_t {
  ucl_version_proto_t                 * stub_ucl_version;
  ucl_major_version_proto_t           * stub_ucl_major_version;
  ucl_minor_version_proto_t           * stub_ucl_minor_version;
  ucl_patch_version_proto_t           * stub_ucl_patch_version;
  ucl_intcmp_proto_t                  * stub_ucl_intcmp;
  ucl_uintcmp_proto_t                 * stub_ucl_uintcmp;
  ucl_ptrintcmp_proto_t               * stub_ucl_ptrintcmp;
  ucl_hash_string_proto_t             * stub_ucl_hash_string;
  ucl_interface_major_version_proto_t * stub_ucl_interface_major_version;
  ucl_interface_minor_version_proto_t * stub_ucl_interface_minor_version;
  ucl_strcmp_proto_t                  * stub_ucl_strcmp;
  ucl_quicksort_proto_t               * stub_ucl_quicksort;
} ucl_base_stub_table_t;

/* Stub table macros definition for the "base" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_base_stub_table_t * ucl_base_stub_table_p;

#define ucl_version                ((ucl_base_stub_table_p)->stub_ucl_version)
#define ucl_major_version          ((ucl_base_stub_table_p)->stub_ucl_major_version)
#define ucl_minor_version          ((ucl_base_stub_table_p)->stub_ucl_minor_version)
#define ucl_patch_version          ((ucl_base_stub_table_p)->stub_ucl_patch_version)
#define ucl_intcmp                 ((ucl_base_stub_table_p)->stub_ucl_intcmp)
#define ucl_uintcmp                ((ucl_base_stub_table_p)->stub_ucl_uintcmp)
#define ucl_ptrintcmp              ((ucl_base_stub_table_p)->stub_ucl_ptrintcmp)
#define ucl_hash_string            ((ucl_base_stub_table_p)->stub_ucl_hash_string)
#define ucl_interface_major_version  ((ucl_base_stub_table_p)->stub_ucl_interface_major_version)
#define ucl_interface_minor_version  ((ucl_base_stub_table_p)->stub_ucl_interface_minor_version)
#define ucl_strcmp                 ((ucl_base_stub_table_p)->stub_ucl_strcmp)
#define ucl_quicksort              ((ucl_base_stub_table_p)->stub_ucl_quicksort)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "circular".
   ------------------------------------------------------------ */

/* Function type declarations for the "circular" module.
   A typedef for each function in the module. */

typedef void       ucl_circular_insert_proto_t (ucl_circular_t this, ucl_circular_link_t *new);
typedef ucl_circular_link_t * ucl_circular_extract_proto_t (ucl_circular_t this);
typedef void       ucl_circular_forward_proto_t (ucl_circular_t this, int times);
typedef ucl_circular_link_t * ucl_circular_find_proto_t (ucl_circular_t this, ucl_value_t val);

/* Stub table type declaration for the "circular" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_circular_stub_table_t {
  ucl_circular_insert_proto_t         * stub_ucl_circular_insert;
  ucl_circular_extract_proto_t        * stub_ucl_circular_extract;
  ucl_circular_forward_proto_t        * stub_ucl_circular_forward;
  ucl_circular_find_proto_t           * stub_ucl_circular_find;
} ucl_circular_stub_table_t;

/* Stub table macros definition for the "circular" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_circular_stub_table_t * ucl_circular_stub_table_p;

#define ucl_circular_insert        ((ucl_circular_stub_table_p)->stub_ucl_circular_insert)
#define ucl_circular_extract       ((ucl_circular_stub_table_p)->stub_ucl_circular_extract)
#define ucl_circular_forward       ((ucl_circular_stub_table_p)->stub_ucl_circular_forward)
#define ucl_circular_find          ((ucl_circular_stub_table_p)->stub_ucl_circular_find)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "debug".
   ------------------------------------------------------------ */

/* Function type declarations for the "debug" module.
   A typedef for each function in the module. */

typedef void       ucl_debug_on_proto_t       (void);
typedef void       ucl_debug_print_proto_t    (const char *function, const char *template, ...);
typedef void       ucl_debug_off_proto_t      (void);
typedef const char * ucl_question_proto_t     (ucl_bool_t expr);

/* Stub table type declaration for the "debug" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_debug_stub_table_t {
  ucl_debug_on_proto_t                * stub_ucl_debug_on;
  ucl_debug_print_proto_t             * stub_ucl_debug_print;
  ucl_debug_off_proto_t               * stub_ucl_debug_off;
  ucl_question_proto_t                * stub_ucl_question;
} ucl_debug_stub_table_t;

/* Stub table macros definition for the "debug" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_debug_stub_table_t * ucl_debug_stub_table_p;

#define ucl_debug_on               ((ucl_debug_stub_table_p)->stub_ucl_debug_on)
#define ucl_debug_print            ((ucl_debug_stub_table_p)->stub_ucl_debug_print)
#define ucl_debug_off              ((ucl_debug_stub_table_p)->stub_ucl_debug_off)
#define ucl_question               ((ucl_debug_stub_table_p)->stub_ucl_question)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "graph".
   ------------------------------------------------------------ */

/* Function type declarations for the "graph" module.
   A typedef for each function in the module. */

typedef void       ucl_graph_link_proto_t     (ucl_graph_node_t * s, ucl_graph_link_t * l, ucl_graph_node_t * d);
typedef void       ucl_graph_unlink_proto_t   (ucl_graph_link_t *link_p);
typedef void       ucl_graph_erase_node_free_links_proto_t (ucl_graph_node_t * node_p, void (* destructor)(void *));
typedef ucl_graph_link_t * ucl_graph_last_output_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_graph_link_t * ucl_graph_last_input_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_graph_link_t * ucl_graph_first_output_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_graph_link_t * ucl_graph_first_input_link_proto_t (ucl_graph_link_t * link_p);
typedef ucl_link_t * ucl_graph_erase_node_return_links_proto_t (ucl_graph_node_t * node_p);
typedef ucl_graph_node_t * ucl_graph_remove_next_node_proto_t (ucl_graph_node_t * node_p);
typedef void       ucl_graph_insert_next_node_proto_t (ucl_graph_node_t * node_p, ucl_graph_node_t * next_p);
typedef void       ucl_graph_merge_upon_input_link_proto_t (ucl_graph_link_t * in, ucl_graph_link_t * out);
typedef void       ucl_graph_merge_upon_output_link_proto_t (ucl_graph_link_t * in, ucl_graph_link_t * out);
typedef void       ucl_graph_directed_depth_first_search_proto_t (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);
typedef void       ucl_graph_initialise_dfs_handle_proto_t (ucl_graph_dfs_t * search_handle);
typedef void       ucl_graph_depth_first_search_proto_t (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);
typedef void       ucl_graph_finalise_dfs_handle_proto_t (ucl_graph_dfs_t * search_handle);
typedef ucl_bool_t ucl_graph_nodes_are_linked_proto_t (ucl_graph_node_t * src_p, ucl_graph_node_t * dst_p);
typedef ucl_bool_t ucl_graph_nodes_are_connected_proto_t (ucl_graph_node_t * a, ucl_graph_node_t * b);
typedef void       ucl_graph_set_next_node_proto_t (ucl_graph_node_t * node_p, const ucl_graph_node_t * next_p);

/* Stub table type declaration for the "graph" module.
   A struct holding a pointer for each function in the module. */

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
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_graph_stub_table_t * ucl_graph_stub_table_p;

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

/* ------------------------------------------------------------ 
   Module "hash".
   ------------------------------------------------------------ */

/* Function type declarations for the "hash" module.
   A typedef for each function in the module. */

typedef void       ucl_hash_constructor_proto_t (ucl_hash_t this);
typedef void       ucl_hash_insert_proto_t    (ucl_hash_t this, ucl_hash_entry_t *entry_p);
typedef void       ucl_hash_extract_proto_t   (ucl_hash_t this, ucl_hash_entry_t *entry_p);
typedef ucl_hash_entry_t * ucl_hash_find_proto_t (const ucl_hash_t this, const ucl_value_t key);
typedef void       ucl_hash_enlarge_proto_t   (ucl_hash_t this);
typedef void       ucl_hash_iterator_proto_t  (const ucl_hash_t this, ucl_iterator_t iterator);
typedef void       ucl_hash_destructor_proto_t (ucl_hash_t this);
typedef void       ucl_hash_initialise_proto_t (ucl_hash_t this, ucl_valcmp_t compar, ucl_hashcmp_t hash);
typedef ucl_hash_entry_t * ucl_hash_first_proto_t (const ucl_hash_t this);

/* Stub table type declaration for the "hash" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_hash_stub_table_t {
  ucl_hash_constructor_proto_t        * stub_ucl_hash_constructor;
  ucl_hash_insert_proto_t             * stub_ucl_hash_insert;
  ucl_hash_extract_proto_t            * stub_ucl_hash_extract;
  ucl_hash_find_proto_t               * stub_ucl_hash_find;
  ucl_hash_enlarge_proto_t            * stub_ucl_hash_enlarge;
  ucl_hash_iterator_proto_t           * stub_ucl_hash_iterator;
  ucl_hash_destructor_proto_t         * stub_ucl_hash_destructor;
  ucl_hash_initialise_proto_t         * stub_ucl_hash_initialise;
  ucl_hash_first_proto_t              * stub_ucl_hash_first;
} ucl_hash_stub_table_t;

/* Stub table macros definition for the "hash" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_hash_stub_table_t * ucl_hash_stub_table_p;

#define ucl_hash_constructor       ((ucl_hash_stub_table_p)->stub_ucl_hash_constructor)
#define ucl_hash_insert            ((ucl_hash_stub_table_p)->stub_ucl_hash_insert)
#define ucl_hash_extract           ((ucl_hash_stub_table_p)->stub_ucl_hash_extract)
#define ucl_hash_find              ((ucl_hash_stub_table_p)->stub_ucl_hash_find)
#define ucl_hash_enlarge           ((ucl_hash_stub_table_p)->stub_ucl_hash_enlarge)
#define ucl_hash_iterator          ((ucl_hash_stub_table_p)->stub_ucl_hash_iterator)
#define ucl_hash_destructor        ((ucl_hash_stub_table_p)->stub_ucl_hash_destructor)
#define ucl_hash_initialise        ((ucl_hash_stub_table_p)->stub_ucl_hash_initialise)
#define ucl_hash_first             ((ucl_hash_stub_table_p)->stub_ucl_hash_first)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "heap".
   ------------------------------------------------------------ */

/* Function type declarations for the "heap" module.
   A typedef for each function in the module. */

typedef void       ucl_heap_constructor_proto_t (ucl_heap_t this, ucl_valcmp_t compar);
typedef void       ucl_heap_insert_proto_t    (ucl_heap_t this, ucl_heap_node_t *node_p);
typedef ucl_heap_node_t * ucl_heap_extract_proto_t (ucl_heap_t this);
typedef void       ucl_heap_merge_proto_t     (ucl_heap_t this, ucl_heap_t other);

/* Stub table type declaration for the "heap" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_heap_stub_table_t {
  ucl_heap_constructor_proto_t        * stub_ucl_heap_constructor;
  ucl_heap_insert_proto_t             * stub_ucl_heap_insert;
  ucl_heap_extract_proto_t            * stub_ucl_heap_extract;
  ucl_heap_merge_proto_t              * stub_ucl_heap_merge;
} ucl_heap_stub_table_t;

/* Stub table macros definition for the "heap" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_heap_stub_table_t * ucl_heap_stub_table_p;

#define ucl_heap_constructor       ((ucl_heap_stub_table_p)->stub_ucl_heap_constructor)
#define ucl_heap_insert            ((ucl_heap_stub_table_p)->stub_ucl_heap_insert)
#define ucl_heap_extract           ((ucl_heap_stub_table_p)->stub_ucl_heap_extract)
#define ucl_heap_merge             ((ucl_heap_stub_table_p)->stub_ucl_heap_merge)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "list".
   ------------------------------------------------------------ */

/* Function type declarations for the "list" module.
   A typedef for each function in the module. */

typedef ucl_list_link_t * ucl_list_index_proto_t (ucl_list_t this, size_t idx);
typedef void       ucl_list_insertbefore_proto_t (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
typedef void       ucl_list_insertafter_proto_t (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
typedef void       ucl_list_pushfront_proto_t (ucl_list_t this, ucl_list_link_t *link_p);
typedef void       ucl_list_pushback_proto_t  (ucl_list_t this, ucl_list_link_t *link_p);
typedef void       ucl_list_extract_proto_t   (ucl_list_t this, ucl_list_link_t *link_p);

/* Stub table type declaration for the "list" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_list_stub_table_t {
  ucl_list_index_proto_t              * stub_ucl_list_index;
  ucl_list_insertbefore_proto_t       * stub_ucl_list_insertbefore;
  ucl_list_insertafter_proto_t        * stub_ucl_list_insertafter;
  ucl_list_pushfront_proto_t          * stub_ucl_list_pushfront;
  ucl_list_pushback_proto_t           * stub_ucl_list_pushback;
  ucl_list_extract_proto_t            * stub_ucl_list_extract;
} ucl_list_stub_table_t;

/* Stub table macros definition for the "list" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_list_stub_table_t * ucl_list_stub_table_p;

#define ucl_list_index             ((ucl_list_stub_table_p)->stub_ucl_list_index)
#define ucl_list_insertbefore      ((ucl_list_stub_table_p)->stub_ucl_list_insertbefore)
#define ucl_list_insertafter       ((ucl_list_stub_table_p)->stub_ucl_list_insertafter)
#define ucl_list_pushfront         ((ucl_list_stub_table_p)->stub_ucl_list_pushfront)
#define ucl_list_pushback          ((ucl_list_stub_table_p)->stub_ucl_list_pushback)
#define ucl_list_extract           ((ucl_list_stub_table_p)->stub_ucl_list_extract)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "map".
   ------------------------------------------------------------ */

/* Function type declarations for the "map" module.
   A typedef for each function in the module. */

typedef void       ucl_map_constructor_proto_t (ucl_map_t this, unsigned int flags, ucl_keycmp_t keycmp);
typedef void       ucl_map_insert_proto_t     (ucl_map_t this, ucl_map_link_t *link_p);
typedef ucl_map_link_t * ucl_map_remove_proto_t (ucl_map_t this, ucl_map_link_t *cur_p);
typedef ucl_map_link_t * ucl_map_find_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef ucl_map_link_t * ucl_map_first_proto_t (const ucl_map_t this);
typedef ucl_map_link_t * ucl_map_last_proto_t (const ucl_map_t this);
typedef ucl_map_link_t * ucl_map_next_proto_t (ucl_map_link_t *link_p);
typedef ucl_map_link_t * ucl_map_prev_proto_t (ucl_map_link_t *link_p);
typedef ucl_map_link_t * ucl_map_find_or_next_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef ucl_map_link_t * ucl_map_find_or_prev_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef size_t     ucl_map_count_proto_t      (const ucl_map_t this, const ucl_value_t key);
typedef void       ucl_map_iterator_inorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void       ucl_map_iterator_inorder_backward_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void       ucl_map_iterator_preorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void       ucl_map_iterator_postorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void       ucl_map_iterator_levelorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void       ucl_map_lower_bound_proto_t (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
typedef void       ucl_map_upper_bound_proto_t (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
typedef void       ucl_map_iterator_union_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
typedef void       ucl_map_iterator_intersection_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
typedef void       ucl_map_iterator_complintersect_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t 	iterator);
typedef void       ucl_map_iterator_subtraction_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
typedef void       ucl_map_destructor_proto_t (ucl_map_t this);

/* Stub table type declaration for the "map" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_map_stub_table_t {
  ucl_map_constructor_proto_t         * stub_ucl_map_constructor;
  ucl_map_insert_proto_t              * stub_ucl_map_insert;
  ucl_map_remove_proto_t              * stub_ucl_map_remove;
  ucl_map_find_proto_t                * stub_ucl_map_find;
  ucl_map_first_proto_t               * stub_ucl_map_first;
  ucl_map_last_proto_t                * stub_ucl_map_last;
  ucl_map_next_proto_t                * stub_ucl_map_next;
  ucl_map_prev_proto_t                * stub_ucl_map_prev;
  ucl_map_find_or_next_proto_t        * stub_ucl_map_find_or_next;
  ucl_map_find_or_prev_proto_t        * stub_ucl_map_find_or_prev;
  ucl_map_count_proto_t               * stub_ucl_map_count;
  ucl_map_iterator_inorder_proto_t    * stub_ucl_map_iterator_inorder;
  ucl_map_iterator_inorder_backward_proto_t * stub_ucl_map_iterator_inorder_backward;
  ucl_map_iterator_preorder_proto_t   * stub_ucl_map_iterator_preorder;
  ucl_map_iterator_postorder_proto_t  * stub_ucl_map_iterator_postorder;
  ucl_map_iterator_levelorder_proto_t * stub_ucl_map_iterator_levelorder;
  ucl_map_lower_bound_proto_t         * stub_ucl_map_lower_bound;
  ucl_map_upper_bound_proto_t         * stub_ucl_map_upper_bound;
  ucl_map_iterator_union_proto_t      * stub_ucl_map_iterator_union;
  ucl_map_iterator_intersection_proto_t * stub_ucl_map_iterator_intersection;
  ucl_map_iterator_complintersect_proto_t * stub_ucl_map_iterator_complintersect;
  ucl_map_iterator_subtraction_proto_t * stub_ucl_map_iterator_subtraction;
  ucl_map_destructor_proto_t          * stub_ucl_map_destructor;
} ucl_map_stub_table_t;

/* Stub table macros definition for the "map" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_map_stub_table_t * ucl_map_stub_table_p;

#define ucl_map_constructor        ((ucl_map_stub_table_p)->stub_ucl_map_constructor)
#define ucl_map_insert             ((ucl_map_stub_table_p)->stub_ucl_map_insert)
#define ucl_map_remove             ((ucl_map_stub_table_p)->stub_ucl_map_remove)
#define ucl_map_find               ((ucl_map_stub_table_p)->stub_ucl_map_find)
#define ucl_map_first              ((ucl_map_stub_table_p)->stub_ucl_map_first)
#define ucl_map_last               ((ucl_map_stub_table_p)->stub_ucl_map_last)
#define ucl_map_next               ((ucl_map_stub_table_p)->stub_ucl_map_next)
#define ucl_map_prev               ((ucl_map_stub_table_p)->stub_ucl_map_prev)
#define ucl_map_find_or_next       ((ucl_map_stub_table_p)->stub_ucl_map_find_or_next)
#define ucl_map_find_or_prev       ((ucl_map_stub_table_p)->stub_ucl_map_find_or_prev)
#define ucl_map_count              ((ucl_map_stub_table_p)->stub_ucl_map_count)
#define ucl_map_iterator_inorder   ((ucl_map_stub_table_p)->stub_ucl_map_iterator_inorder)
#define ucl_map_iterator_inorder_backward  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_inorder_backward)
#define ucl_map_iterator_preorder  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_preorder)
#define ucl_map_iterator_postorder  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_postorder)
#define ucl_map_iterator_levelorder  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_levelorder)
#define ucl_map_lower_bound        ((ucl_map_stub_table_p)->stub_ucl_map_lower_bound)
#define ucl_map_upper_bound        ((ucl_map_stub_table_p)->stub_ucl_map_upper_bound)
#define ucl_map_iterator_union     ((ucl_map_stub_table_p)->stub_ucl_map_iterator_union)
#define ucl_map_iterator_intersection  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_intersection)
#define ucl_map_iterator_complintersect  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_complintersect)
#define ucl_map_iterator_subtraction  ((ucl_map_stub_table_p)->stub_ucl_map_iterator_subtraction)
#define ucl_map_destructor         ((ucl_map_stub_table_p)->stub_ucl_map_destructor)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "memory".
   ------------------------------------------------------------ */

/* Function type declarations for the "memory" module.
   A typedef for each function in the module. */

typedef void       ucl_memory_alloc_proto_t   (void * dummy UCL_UNUSED, void * qq, size_t dim);

/* Stub table type declaration for the "memory" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_memory_stub_table_t {
  ucl_memory_alloc_proto_t            * stub_ucl_memory_alloc;
} ucl_memory_stub_table_t;

/* Stub table macros definition for the "memory" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_memory_stub_table_t * ucl_memory_stub_table_p;

#define ucl_memory_alloc           ((ucl_memory_stub_table_p)->stub_ucl_memory_alloc)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "tree".
   ------------------------------------------------------------ */

/* Function type declarations for the "tree" module.
   A typedef for each function in the module. */

typedef ucl_bool_t ucl_tree_is_dad_proto_t    (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *cld_p);
typedef ucl_bool_t ucl_tree_is_bro_proto_t    (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *bro_p);
typedef ucl_bool_t ucl_tree_has_dad_proto_t   (const ucl_tree_node_t *nod_p);
typedef ucl_bool_t ucl_tree_has_prev_proto_t  (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_dad_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_prev_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_first_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_last_proto_t (const ucl_tree_node_t *nod_p);
typedef void       ucl_tree_insert_dad_proto_t (ucl_tree_node_t *nod_p, ucl_tree_node_t *dad_p);
typedef void       ucl_tree_insert_son_proto_t (ucl_tree_node_t * nod_p, ucl_tree_node_t * son_p);
typedef void       ucl_tree_insert_next_proto_t (ucl_tree_node_t * nod_p, ucl_tree_node_t * nxt_p);
typedef void       ucl_tree_insert_prev_proto_t (ucl_tree_node_t * nod_p, ucl_tree_node_t * prv_p);
typedef ucl_tree_node_t * ucl_tree_extract_dad_proto_t (ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_extract_son_proto_t (ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_extract_prev_proto_t (ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_extract_next_proto_t (ucl_tree_node_t *nod_p);
typedef void       ucl_tree_iterator_inorder_proto_t (const ucl_tree_node_t * nod_p, ucl_iterator_t iterator);
typedef void       ucl_tree_iterator_preorder_proto_t (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);
typedef void       ucl_tree_iterator_postorder_proto_t (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);

/* Stub table type declaration for the "tree" module.
   A struct holding a pointer for each function in the module. */

typedef struct ucl_tree_stub_table_t {
  ucl_tree_is_dad_proto_t             * stub_ucl_tree_is_dad;
  ucl_tree_is_bro_proto_t             * stub_ucl_tree_is_bro;
  ucl_tree_has_dad_proto_t            * stub_ucl_tree_has_dad;
  ucl_tree_has_prev_proto_t           * stub_ucl_tree_has_prev;
  ucl_tree_get_dad_proto_t            * stub_ucl_tree_get_dad;
  ucl_tree_get_prev_proto_t           * stub_ucl_tree_get_prev;
  ucl_tree_get_first_proto_t          * stub_ucl_tree_get_first;
  ucl_tree_get_last_proto_t           * stub_ucl_tree_get_last;
  ucl_tree_insert_dad_proto_t         * stub_ucl_tree_insert_dad;
  ucl_tree_insert_son_proto_t         * stub_ucl_tree_insert_son;
  ucl_tree_insert_next_proto_t        * stub_ucl_tree_insert_next;
  ucl_tree_insert_prev_proto_t        * stub_ucl_tree_insert_prev;
  ucl_tree_extract_dad_proto_t        * stub_ucl_tree_extract_dad;
  ucl_tree_extract_son_proto_t        * stub_ucl_tree_extract_son;
  ucl_tree_extract_prev_proto_t       * stub_ucl_tree_extract_prev;
  ucl_tree_extract_next_proto_t       * stub_ucl_tree_extract_next;
  ucl_tree_iterator_inorder_proto_t   * stub_ucl_tree_iterator_inorder;
  ucl_tree_iterator_preorder_proto_t  * stub_ucl_tree_iterator_preorder;
  ucl_tree_iterator_postorder_proto_t * stub_ucl_tree_iterator_postorder;
} ucl_tree_stub_table_t;

/* Stub table macros definition for the "tree" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_tree_stub_table_t * ucl_tree_stub_table_p;

#define ucl_tree_is_dad            ((ucl_tree_stub_table_p)->stub_ucl_tree_is_dad)
#define ucl_tree_is_bro            ((ucl_tree_stub_table_p)->stub_ucl_tree_is_bro)
#define ucl_tree_has_dad           ((ucl_tree_stub_table_p)->stub_ucl_tree_has_dad)
#define ucl_tree_has_prev          ((ucl_tree_stub_table_p)->stub_ucl_tree_has_prev)
#define ucl_tree_get_dad           ((ucl_tree_stub_table_p)->stub_ucl_tree_get_dad)
#define ucl_tree_get_prev          ((ucl_tree_stub_table_p)->stub_ucl_tree_get_prev)
#define ucl_tree_get_first         ((ucl_tree_stub_table_p)->stub_ucl_tree_get_first)
#define ucl_tree_get_last          ((ucl_tree_stub_table_p)->stub_ucl_tree_get_last)
#define ucl_tree_insert_dad        ((ucl_tree_stub_table_p)->stub_ucl_tree_insert_dad)
#define ucl_tree_insert_son        ((ucl_tree_stub_table_p)->stub_ucl_tree_insert_son)
#define ucl_tree_insert_next       ((ucl_tree_stub_table_p)->stub_ucl_tree_insert_next)
#define ucl_tree_insert_prev       ((ucl_tree_stub_table_p)->stub_ucl_tree_insert_prev)
#define ucl_tree_extract_dad       ((ucl_tree_stub_table_p)->stub_ucl_tree_extract_dad)
#define ucl_tree_extract_son       ((ucl_tree_stub_table_p)->stub_ucl_tree_extract_son)
#define ucl_tree_extract_prev      ((ucl_tree_stub_table_p)->stub_ucl_tree_extract_prev)
#define ucl_tree_extract_next      ((ucl_tree_stub_table_p)->stub_ucl_tree_extract_next)
#define ucl_tree_iterator_inorder  ((ucl_tree_stub_table_p)->stub_ucl_tree_iterator_inorder)
#define ucl_tree_iterator_preorder  ((ucl_tree_stub_table_p)->stub_ucl_tree_iterator_preorder)
#define ucl_tree_iterator_postorder  ((ucl_tree_stub_table_p)->stub_ucl_tree_iterator_postorder)

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Module "vector".
   ------------------------------------------------------------ */

/* Function type declarations for the "vector" module.
   A typedef for each function in the module. */

typedef void       ucl_vector_initialise_proto_t (ucl_vector_t self, size_t slot_dimension);
typedef void       ucl_vector_initialise_size_proto_t (ucl_vector_t self, size_t number_of_initial_slots);
typedef void       ucl_vector_initialise_pad_proto_t (ucl_vector_t self, size_t size_of_padding_area);
typedef void       ucl_vector_initialise_step_up_proto_t (ucl_vector_t self, size_t step_up);
typedef void       ucl_vector_initialise_step_down_proto_t (ucl_vector_t self, size_t step_down);
typedef void       ucl_vector_constructor_proto_t (ucl_vector_t self);
typedef void       ucl_vector_destructor_proto_t (ucl_vector_t self);
typedef size_t     ucl_vector_number_of_step_up_slots_proto_t (const ucl_vector_t self);
typedef size_t     ucl_vector_number_of_step_down_slots_proto_t (const ucl_vector_t self);
typedef size_t     ucl_vector_number_of_padding_slots_proto_t (const ucl_vector_t self);
typedef void *     ucl_vector_index_to_slot_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef void *     ucl_vector_index_to_new_slot_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef ucl_vector_index_t ucl_vector_last_index_proto_t (const ucl_vector_t self);
typedef ucl_vector_index_t ucl_vector_slot_to_index_proto_t (const ucl_vector_t self, const void *_pointer_to_slot_p);
typedef ucl_bool_t ucl_vector_pointer_is_valid_slot_proto_t (const ucl_vector_t self, const void *_pointer_to_slot_p);
typedef ucl_bool_t ucl_vector_index_is_valid_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);
typedef ucl_bool_t ucl_vector_range_is_valid_proto_t (const ucl_vector_t self, ucl_range_t range);
typedef void *     ucl_vector_insert_proto_t  (ucl_vector_t self, void *_pointer_to_slot_p);
typedef void *     ucl_vector_insert_sort_proto_t (ucl_vector_t self, void *data_p);
typedef void       ucl_vector_erase_proto_t   (ucl_vector_t self, void *_pointer_to_slot_p);
typedef size_t     ucl_vector_slot_dimension_proto_t (const ucl_vector_t self);
typedef size_t     ucl_vector_size_proto_t    (const ucl_vector_t self);
typedef void       ucl_vector_set_compar_proto_t (ucl_vector_t self, ucl_valcmp_t compar);
typedef ucl_block_t ucl_vector_get_memory_block_proto_t (const ucl_vector_t self);
typedef ucl_block_t ucl_vector_get_data_block_proto_t (const ucl_vector_t self);
typedef void       ucl_vector_quick_sort_proto_t (ucl_vector_t self);
typedef void       ucl_vector_iterator_forward_proto_t (const ucl_vector_t self, ucl_iterator_t iterator);
typedef void       ucl_vector_iterator_backward_proto_t (const ucl_vector_t self, ucl_iterator_t iterator);
typedef void       ucl_vector_enlarge_proto_t (ucl_vector_t self);
typedef void       ucl_vector_restrict_proto_t (ucl_vector_t self);
typedef void       ucl_vector_set_memory_to_zero_proto_t (ucl_vector_t self);
typedef size_t     ucl_vector_number_of_free_slots_proto_t (const ucl_vector_t self);
typedef void *     ucl_vector_find_proto_t    (const ucl_vector_t self, const void * data_p);
typedef void *     ucl_vector_sort_find_proto_t (const ucl_vector_t self, const void * data_p);
typedef void *     ucl_vector_binary_search_proto_t (const ucl_vector_t self, const void * data_p);
typedef void       ucl_vector_append_range_proto_t (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range);
typedef void       ucl_vector_append_proto_t  (ucl_vector_t target, const ucl_vector_t source);
typedef void       ucl_vector_append_more_proto_t (ucl_vector_t target, const ucl_vector_t source, ...);
typedef void       ucl_vector_insert_vector_proto_t (ucl_vector_t T, ucl_vector_index_t offset, const ucl_vector_t S);
typedef void       ucl_vector_erase_range_proto_t (ucl_vector_t self, ucl_range_t index_range);
typedef int        ucl_vector_compare_range_proto_t (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb);
typedef int        ucl_vector_compare_proto_t (const ucl_vector_t a, const ucl_vector_t b);
typedef ucl_bool_t ucl_vector_equal_proto_t   (const ucl_vector_t a, const ucl_vector_t b);
typedef void       ucl_vector_mark_all_slots_as_used_proto_t (ucl_vector_t self);
typedef void       ucl_vector_enlarge_for_slots_proto_t (ucl_vector_t self, size_t required_free_slots);
typedef void       ucl_vector_append_block_proto_t (ucl_vector_t self, const ucl_block_t block);
typedef void       ucl_vector_initialise_buffer_proto_t (ucl_vector_t self);
typedef void       ucl_vector_swallow_block_proto_t (ucl_vector_t self, ucl_block_t block);
typedef ucl_bool_t ucl_vector_running_proto_t (const ucl_vector_t self);
typedef ucl_data_t ucl_vector_push_back_proto_t (ucl_vector_t self);
typedef ucl_data_t ucl_vector_push_front_proto_t (ucl_vector_t self);
typedef void       ucl_vector_pop_back_proto_t (ucl_vector_t self);
typedef void       ucl_vector_pop_front_proto_t (ucl_vector_t self);
typedef void       ucl_vector_reset_proto_t   (ucl_vector_t self);
typedef void       ucl_vector_clean_proto_t   (ucl_vector_t self);
typedef void       ucl_vector_register_allocator_proto_t (ucl_vector_t self, ucl_memory_allocator_t allocator);
typedef ucl_bool_t ucl_vector_sorted_proto_t  (ucl_vector_t self);
typedef ucl_block_t ucl_vector_block_from_range_proto_t (const ucl_vector_t self, const ucl_range_t range);
typedef ucl_range_t ucl_vector_range_from_block_proto_t (const ucl_vector_t self, const ucl_block_t block);
typedef ucl_block_t ucl_vector_get_free_block_at_end_proto_t (ucl_vector_t self, size_t requested_slots);
typedef ucl_block_t ucl_vector_get_free_block_at_beginning_proto_t (ucl_vector_t self, size_t requested_slots);
typedef void       ucl_vector_mark_as_used_proto_t (ucl_vector_t self, ucl_block_t used_block);
typedef void       ucl_vector_update_number_of_step_up_slots_proto_t (ucl_vector_t self, size_t step_up);
typedef void       ucl_vector_update_number_of_step_down_slots_proto_t (ucl_vector_t self, size_t step_down);
typedef void       ucl_vector_update_number_of_padding_slots_proto_t (ucl_vector_t self, size_t padding);
typedef void       ucl_vector_for_each_proto_t (ucl_callback_t callback, const ucl_vector_t self);
typedef void       ucl_vector_for_each_multiple_from_array_proto_t (ucl_callback_t callback, const ucl_vector_array_t * vectors);
typedef void       ucl_vector_for_each_multiple_proto_t (ucl_callback_t callback, const ucl_vector_t first, ...);
typedef void       ucl_vector_map_proto_t     (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self);
typedef void       ucl_vector_for_each_in_range_proto_t (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
typedef void       ucl_vector_map_range_proto_t (ucl_vector_t result, ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
typedef void       ucl_vector_iterator_range_forward_proto_t (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
typedef void       ucl_vector_iterator_range_backward_proto_t (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
typedef void       ucl_vector_map_multiple_from_array_proto_t (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_array_t * vectors);
typedef void       ucl_vector_map_multiple_proto_t (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...);
typedef ucl_range_t ucl_vector_range_proto_t  (const ucl_vector_t self);
typedef ucl_range_t ucl_vector_range_from_position_to_end_proto_t (const ucl_vector_t self, ucl_vector_index_t position);
typedef ucl_range_t ucl_vector_range_from_end_to_position_proto_t (const ucl_vector_t self, ucl_vector_index_t position);
typedef ucl_range_t ucl_vector_range_from_end_with_span_proto_t (const ucl_vector_t self, size_t span);
typedef void       ucl_vector_enlarge_for_range_proto_t (ucl_vector_t self, ucl_range_t range);
typedef void       ucl_vector_append_more_from_array_proto_t (ucl_vector_t target, const ucl_vector_array_t * vectors);
typedef void       ucl_vector_insert_range_proto_t (ucl_vector_t V, ucl_vector_index_t offset, const ucl_vector_t A, ucl_range_t range);
typedef void       ucl_vector_insert_block_proto_t (ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block);
typedef void       ucl_vector_copy_range_proto_t (ucl_vector_t target, ucl_vector_index_t position, ucl_vector_t source, ucl_range_t source_range);
typedef void       ucl_vector_set_block_proto_t (ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source);
typedef void       ucl_vector_get_block_proto_t (ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source);
typedef ucl_bool_t ucl_vector_index_is_valid_new_proto_t (const ucl_vector_t self, ucl_vector_index_t idx);

/* Stub table type declaration for the "vector" module.
   A struct holding a pointer for each function in the module. */

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
} ucl_vector_stub_table_t;

/* Stub table macros definition for the "vector" module.
   A preprocessor macro for each function in the module,
   used to invoke the function through the stub table. */

#ifdef UCL_ENABLE_STUB

extern const ucl_vector_stub_table_t * ucl_vector_stub_table_p;

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

#endif /* defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ 
   Main stub table.
   ------------------------------------------------------------ */

typedef struct ucl_stub_table_t {
  const ucl_btree_stub_table_t *                    btree;
  const ucl_base_stub_table_t *                     base;
  const ucl_circular_stub_table_t *                 circular;
  const ucl_debug_stub_table_t *                    debug;
  const ucl_graph_stub_table_t *                    graph;
  const ucl_hash_stub_table_t *                     hash;
  const ucl_heap_stub_table_t *                     heap;
  const ucl_list_stub_table_t *                     list;
  const ucl_map_stub_table_t *                      map;
  const ucl_memory_stub_table_t *                   memory;
  const ucl_tree_stub_table_t *                     tree;
  const ucl_vector_stub_table_t *                   vector;
} ucl_stub_table_t;

extern const char * ucl_init_stub_table (void);
extern int ucl_final_stub_table (void);
extern const ucl_stub_table_t ucl_stub_table;


#endif /* defined __UCL_STUB_H */


/* end of file */
