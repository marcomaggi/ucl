/* Stub header file for the "btree" module.
   This file must be included in the main stub header. */

#ifndef __UCL_BTREE_H

/** ------------------------------------------------------------ 
 ** Module "btree".
 ** --------------------------------------------------------- */

/* Function prototypes for the "btree" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void * ucl_btree_find_value (void * node, void * value, ucl_comparison_t compar)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_leftmost (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_rightmost (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_deepest_son (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_deepest_bro (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_root (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_inorder (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_inorder_backward (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_preorder (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_preorder_backward (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_postorder (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_postorder_backward (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_levelorder (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_levelorder_backward (void * node)
	__attribute__((__nonnull__,__pure__));
ucl_decl void ucl_btree_iterator_inorder (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_inorder_backward (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_preorder (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_preorder_backward (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_postorder (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_postorder_backward (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_levelorder (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_iterator_levelorder_backward (ucl_iterator_t iterator, void * root_node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_inorder (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_inorder_backward (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_preorder (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_preorder_backward (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_postorder (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_postorder_backward (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_levelorder (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_levelorder_backward (ucl_iterator_t iterator, void * node)
	__attribute__((__nonnull__));

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "btree" module.
   A typedef for each function in the module. */

typedef void * ucl_btree_find_value_proto_t (void * node, void * value, ucl_comparison_t compar);
typedef void * ucl_btree_find_leftmost_proto_t (void * node);
typedef void * ucl_btree_find_rightmost_proto_t (void * node);
typedef void * ucl_btree_find_deepest_son_proto_t (void * node);
typedef void * ucl_btree_find_deepest_bro_proto_t (void * node);
typedef void * ucl_btree_find_root_proto_t (void * node);
typedef void * ucl_btree_step_inorder_proto_t (void * node);
typedef void * ucl_btree_step_inorder_backward_proto_t (void * node);
typedef void * ucl_btree_step_preorder_proto_t (void * node);
typedef void * ucl_btree_step_preorder_backward_proto_t (void * node);
typedef void * ucl_btree_step_postorder_proto_t (void * node);
typedef void * ucl_btree_step_postorder_backward_proto_t (void * node);
typedef void * ucl_btree_step_levelorder_proto_t (void * node);
typedef void * ucl_btree_step_levelorder_backward_proto_t (void * node);
typedef void ucl_btree_iterator_inorder_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_inorder_backward_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_preorder_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_preorder_backward_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_postorder_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_postorder_backward_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_levelorder_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_iterator_levelorder_backward_proto_t (ucl_iterator_t iterator, void * root_node);
typedef void ucl_btree_subtree_iterator_inorder_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_inorder_backward_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_preorder_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_preorder_backward_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_postorder_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_postorder_backward_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_levelorder_proto_t (ucl_iterator_t iterator, void * node);
typedef void ucl_btree_subtree_iterator_levelorder_backward_proto_t (ucl_iterator_t iterator, void * node);

/* Stub table type declaration for the "btree" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_btree_stub_table_t {
  ucl_btree_find_leftmost_proto_t     * stub_ucl_btree_find_leftmost;
  ucl_btree_step_inorder_proto_t      * stub_ucl_btree_step_inorder;
  ucl_btree_step_inorder_backward_proto_t * stub_ucl_btree_step_inorder_backward;
  ucl_btree_step_preorder_proto_t     * stub_ucl_btree_step_preorder;
  ucl_btree_step_postorder_proto_t    * stub_ucl_btree_step_postorder;
  ucl_btree_step_levelorder_proto_t   * stub_ucl_btree_step_levelorder;
  ucl_btree_find_rightmost_proto_t    * stub_ucl_btree_find_rightmost;
  ucl_btree_find_deepest_son_proto_t  * stub_ucl_btree_find_deepest_son;
  ucl_btree_find_deepest_bro_proto_t  * stub_ucl_btree_find_deepest_bro;
  ucl_btree_step_preorder_backward_proto_t * stub_ucl_btree_step_preorder_backward;
  ucl_btree_step_postorder_backward_proto_t * stub_ucl_btree_step_postorder_backward;
  ucl_btree_step_levelorder_backward_proto_t * stub_ucl_btree_step_levelorder_backward;
  ucl_btree_iterator_inorder_proto_t  * stub_ucl_btree_iterator_inorder;
  ucl_btree_iterator_inorder_backward_proto_t * stub_ucl_btree_iterator_inorder_backward;
  ucl_btree_iterator_preorder_proto_t * stub_ucl_btree_iterator_preorder;
  ucl_btree_iterator_preorder_backward_proto_t * stub_ucl_btree_iterator_preorder_backward;
  ucl_btree_iterator_postorder_proto_t * stub_ucl_btree_iterator_postorder;
  ucl_btree_iterator_postorder_backward_proto_t * stub_ucl_btree_iterator_postorder_backward;
  ucl_btree_iterator_levelorder_backward_proto_t * stub_ucl_btree_iterator_levelorder_backward;
  ucl_btree_iterator_levelorder_proto_t * stub_ucl_btree_iterator_levelorder;
  ucl_btree_subtree_iterator_inorder_proto_t * stub_ucl_btree_subtree_iterator_inorder;
  ucl_btree_subtree_iterator_inorder_backward_proto_t * stub_ucl_btree_subtree_iterator_inorder_backward;
  ucl_btree_subtree_iterator_preorder_proto_t * stub_ucl_btree_subtree_iterator_preorder;
  ucl_btree_subtree_iterator_preorder_backward_proto_t * stub_ucl_btree_subtree_iterator_preorder_backward;
  ucl_btree_subtree_iterator_postorder_proto_t * stub_ucl_btree_subtree_iterator_postorder;
  ucl_btree_subtree_iterator_postorder_backward_proto_t * stub_ucl_btree_subtree_iterator_postorder_backward;
  ucl_btree_subtree_iterator_levelorder_proto_t * stub_ucl_btree_subtree_iterator_levelorder;
  ucl_btree_subtree_iterator_levelorder_backward_proto_t * stub_ucl_btree_subtree_iterator_levelorder_backward;
  ucl_btree_find_root_proto_t         * stub_ucl_btree_find_root;
  ucl_btree_find_value_proto_t        * stub_ucl_btree_find_value;
} ucl_btree_stub_table_t;

/* Stub table macros definition for the "btree" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_btree_stub_table_t * ucl_btree_stub_table_p;
ucl_decl const ucl_btree_stub_table_t   ucl_btree_stub_table;

#define ucl_btree_find_leftmost    ((ucl_btree_stub_table_p)->stub_ucl_btree_find_leftmost)
#define ucl_btree_step_inorder     ((ucl_btree_stub_table_p)->stub_ucl_btree_step_inorder)
#define ucl_btree_step_inorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_inorder_backward)
#define ucl_btree_step_preorder    ((ucl_btree_stub_table_p)->stub_ucl_btree_step_preorder)
#define ucl_btree_step_postorder   ((ucl_btree_stub_table_p)->stub_ucl_btree_step_postorder)
#define ucl_btree_step_levelorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_levelorder)
#define ucl_btree_find_rightmost   ((ucl_btree_stub_table_p)->stub_ucl_btree_find_rightmost)
#define ucl_btree_find_deepest_son  ((ucl_btree_stub_table_p)->stub_ucl_btree_find_deepest_son)
#define ucl_btree_find_deepest_bro  ((ucl_btree_stub_table_p)->stub_ucl_btree_find_deepest_bro)
#define ucl_btree_step_preorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_preorder_backward)
#define ucl_btree_step_postorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_postorder_backward)
#define ucl_btree_step_levelorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_step_levelorder_backward)
#define ucl_btree_iterator_inorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_inorder)
#define ucl_btree_iterator_inorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_inorder_backward)
#define ucl_btree_iterator_preorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_preorder)
#define ucl_btree_iterator_preorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_preorder_backward)
#define ucl_btree_iterator_postorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_postorder)
#define ucl_btree_iterator_postorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_postorder_backward)
#define ucl_btree_iterator_levelorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_levelorder_backward)
#define ucl_btree_iterator_levelorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_iterator_levelorder)
#define ucl_btree_subtree_iterator_inorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_inorder)
#define ucl_btree_subtree_iterator_inorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_inorder_backward)
#define ucl_btree_subtree_iterator_preorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_preorder)
#define ucl_btree_subtree_iterator_preorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_preorder_backward)
#define ucl_btree_subtree_iterator_postorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_postorder)
#define ucl_btree_subtree_iterator_postorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_postorder_backward)
#define ucl_btree_subtree_iterator_levelorder  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_levelorder)
#define ucl_btree_subtree_iterator_levelorder_backward  ((ucl_btree_stub_table_p)->stub_ucl_btree_subtree_iterator_levelorder_backward)
#define ucl_btree_find_root        ((ucl_btree_stub_table_p)->stub_ucl_btree_find_root)
#define ucl_btree_find_value       ((ucl_btree_stub_table_p)->stub_ucl_btree_find_value)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_BTREE_H */

/* end of file */