/* Stub header file for the "map" module.
   This file must be included in the main stub header. */

#ifndef __UCL_MAP_H

/** ------------------------------------------------------------ 
 ** Module "map".
 ** --------------------------------------------------------- */

/* Function prototypes for the "map" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_map_constructor (ucl_map_t this, unsigned int flags, ucl_comparison_t keycmp);
ucl_decl void ucl_map_destructor (ucl_map_t this);
ucl_decl void ucl_map_insert (ucl_map_t this, ucl_map_link_t *link_p);
ucl_decl ucl_map_link_t * ucl_map_remove (ucl_map_t this, ucl_map_link_t *cur_p);
ucl_decl ucl_map_link_t * ucl_map_find (const ucl_map_t this, const ucl_value_t key);
ucl_decl ucl_map_link_t * ucl_map_first (const ucl_map_t this);
ucl_decl ucl_map_link_t * ucl_map_last (const ucl_map_t this);
ucl_decl ucl_map_link_t * ucl_map_next (ucl_map_link_t *link_p);
ucl_decl ucl_map_link_t * ucl_map_prev (ucl_map_link_t *link_p);
ucl_decl ucl_map_link_t * ucl_map_find_or_next (const ucl_map_t this, const ucl_value_t key);
ucl_decl ucl_map_link_t * ucl_map_find_or_prev (const ucl_map_t this, const ucl_value_t key);
ucl_decl size_t ucl_map_count (const ucl_map_t this, const ucl_value_t key);
ucl_decl void ucl_map_iterator_inorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_inorder_backward (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_preorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_postorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_levelorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_lower_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
ucl_decl void ucl_map_upper_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
ucl_decl void ucl_map_iterator_union (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_intersection (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_complintersect (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t 	iterator);
ucl_decl void ucl_map_iterator_subtraction (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "map" module.
   A typedef for each function in the module. */

typedef void ucl_map_constructor_proto_t (ucl_map_t this, unsigned int flags, ucl_comparison_t keycmp);
typedef void ucl_map_destructor_proto_t (ucl_map_t this);
typedef void ucl_map_insert_proto_t (ucl_map_t this, ucl_map_link_t *link_p);
typedef ucl_map_link_t * ucl_map_remove_proto_t (ucl_map_t this, ucl_map_link_t *cur_p);
typedef ucl_map_link_t * ucl_map_find_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef ucl_map_link_t * ucl_map_first_proto_t (const ucl_map_t this);
typedef ucl_map_link_t * ucl_map_last_proto_t (const ucl_map_t this);
typedef ucl_map_link_t * ucl_map_next_proto_t (ucl_map_link_t *link_p);
typedef ucl_map_link_t * ucl_map_prev_proto_t (ucl_map_link_t *link_p);
typedef ucl_map_link_t * ucl_map_find_or_next_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef ucl_map_link_t * ucl_map_find_or_prev_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef size_t ucl_map_count_proto_t (const ucl_map_t this, const ucl_value_t key);
typedef void ucl_map_iterator_inorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void ucl_map_iterator_inorder_backward_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void ucl_map_iterator_preorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void ucl_map_iterator_postorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void ucl_map_iterator_levelorder_proto_t (const ucl_map_t this, ucl_iterator_t iterator);
typedef void ucl_map_lower_bound_proto_t (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
typedef void ucl_map_upper_bound_proto_t (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
typedef void ucl_map_iterator_union_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
typedef void ucl_map_iterator_intersection_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
typedef void ucl_map_iterator_complintersect_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t 	iterator);
typedef void ucl_map_iterator_subtraction_proto_t (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);

/* Stub table type declaration for the "map" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

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
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_map_stub_table_t * ucl_map_stub_table_p;
ucl_decl const ucl_map_stub_table_t   ucl_map_stub_table;

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


#endif /* defined __UCL_MAP_H */

/* end of file */
