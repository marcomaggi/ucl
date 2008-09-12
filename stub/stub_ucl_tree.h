/* Stub header file for the "tree" module.
   This file must be included in the main stub header. */

#ifndef __UCL_TREE_H

/** ------------------------------------------------------------ 
 ** Module "tree".
 ** --------------------------------------------------------- */

/* Function prototypes for the "tree" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl ucl_bool_t ucl_tree_is_dad (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *cld_p);
ucl_decl ucl_bool_t ucl_tree_is_bro (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *bro_p);
ucl_decl ucl_bool_t ucl_tree_has_dad (const ucl_tree_node_t *nod_p);
ucl_decl ucl_bool_t ucl_tree_has_prev (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_dad (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_prev (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_first (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_last (const ucl_tree_node_t *nod_p);
ucl_decl void ucl_tree_insert_dad (ucl_tree_node_t *nod_p, ucl_tree_node_t *dad_p);
ucl_decl void ucl_tree_insert_son (ucl_tree_node_t * nod_p, ucl_tree_node_t * son_p);
ucl_decl void ucl_tree_insert_next (ucl_tree_node_t * nod_p, ucl_tree_node_t * nxt_p);
ucl_decl void ucl_tree_insert_prev (ucl_tree_node_t * nod_p, ucl_tree_node_t * prv_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_dad (ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_son (ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_prev (ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_next (ucl_tree_node_t *nod_p);
ucl_decl void ucl_tree_iterator_inorder (const ucl_tree_node_t * nod_p, ucl_iterator_t iterator);
ucl_decl void ucl_tree_iterator_preorder (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);
ucl_decl void ucl_tree_iterator_postorder (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "tree" module.
   A typedef for each function in the module. */

typedef ucl_bool_t ucl_tree_is_dad_proto_t (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *cld_p);
typedef ucl_bool_t ucl_tree_is_bro_proto_t (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *bro_p);
typedef ucl_bool_t ucl_tree_has_dad_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_bool_t ucl_tree_has_prev_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_dad_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_prev_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_first_proto_t (const ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_get_last_proto_t (const ucl_tree_node_t *nod_p);
typedef void ucl_tree_insert_dad_proto_t (ucl_tree_node_t *nod_p, ucl_tree_node_t *dad_p);
typedef void ucl_tree_insert_son_proto_t (ucl_tree_node_t * nod_p, ucl_tree_node_t * son_p);
typedef void ucl_tree_insert_next_proto_t (ucl_tree_node_t * nod_p, ucl_tree_node_t * nxt_p);
typedef void ucl_tree_insert_prev_proto_t (ucl_tree_node_t * nod_p, ucl_tree_node_t * prv_p);
typedef ucl_tree_node_t * ucl_tree_extract_dad_proto_t (ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_extract_son_proto_t (ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_extract_prev_proto_t (ucl_tree_node_t *nod_p);
typedef ucl_tree_node_t * ucl_tree_extract_next_proto_t (ucl_tree_node_t *nod_p);
typedef void ucl_tree_iterator_inorder_proto_t (const ucl_tree_node_t * nod_p, ucl_iterator_t iterator);
typedef void ucl_tree_iterator_preorder_proto_t (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);
typedef void ucl_tree_iterator_postorder_proto_t (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);

/* Stub table type declaration for the "tree" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

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
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_tree_stub_table_t * ucl_tree_stub_table_p;
ucl_decl const ucl_tree_stub_table_t   ucl_tree_stub_table;

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


#endif /* defined __UCL_TREE_H */

/* end of file */
