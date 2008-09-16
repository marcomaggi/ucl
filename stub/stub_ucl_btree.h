/* Stub header file for the "btree" module.
   This file must be included in the main stub header. */

#ifndef __UCL_BTREE_H

/** ------------------------------------------------------------ 
 ** Module "btree".
 ** --------------------------------------------------------- */

/* Function prototypes for the "btree" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void * ucl_btree_find_leftmost (void * _node_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_rightmost (void * _node_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_find_deepest_son (void * _node_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_inorder (void * _cur_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_inorder_backward (void * _cur_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_preorder (void * _cur_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_postorder (void * _cur_p)
	__attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_btree_step_levelorder (void * _cur_p)
	__attribute__((__nonnull__,__pure__));

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "btree" module.
   A typedef for each function in the module. */

typedef void * ucl_btree_find_leftmost_proto_t (void * _node_p);
typedef void * ucl_btree_find_rightmost_proto_t (void * _node_p);
typedef void * ucl_btree_find_deepest_son_proto_t (void * _node_p);
typedef void * ucl_btree_step_inorder_proto_t (void * _cur_p);
typedef void * ucl_btree_step_inorder_backward_proto_t (void * _cur_p);
typedef void * ucl_btree_step_preorder_proto_t (void * _cur_p);
typedef void * ucl_btree_step_postorder_proto_t (void * _cur_p);
typedef void * ucl_btree_step_levelorder_proto_t (void * _cur_p);

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

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_BTREE_H */

/* end of file */
