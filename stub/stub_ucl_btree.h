/* Stub header file for the "btree" module.
   This file must be included in the main stub header. */

#ifndef __UCL_BTREE_H

/** ------------------------------------------------------------ 
 ** Module "btree".
 ** --------------------------------------------------------- */

/* Function prototypes for the "btree" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_btree_constructor (ucl_btree_node_t * this_p, const ucl_btree_node_t * dad_p);
ucl_decl void ucl_btree_destructor (ucl_btree_node_t * this);
ucl_decl void ucl_btree_setbro (ucl_btree_node_t * this, const ucl_btree_node_t * newBro);
ucl_decl void ucl_btree_setson (ucl_btree_node_t * this, const ucl_btree_node_t * newSon);
ucl_decl void ucl_btree_setdad (ucl_btree_node_t * this, const ucl_btree_node_t * newDad);
ucl_decl void ucl_btree_dadbro (ucl_btree_node_t * dad, ucl_btree_node_t * bro);
ucl_decl void ucl_btree_dadson (ucl_btree_node_t * dad, ucl_btree_node_t * son);
ucl_decl ucl_btree_node_t * ucl_btree_getbro (const ucl_btree_node_t * this);
ucl_decl ucl_btree_node_t * ucl_btree_getson (const ucl_btree_node_t * this);
ucl_decl ucl_btree_node_t * ucl_btree_getdad (const ucl_btree_node_t * this);
ucl_decl ucl_btree_node_t * ucl_btree_find_leftmost (const ucl_btree_node_t *node_p);
ucl_decl ucl_btree_node_t * ucl_btree_find_rightmost (const ucl_btree_node_t *node_p);
ucl_decl ucl_btree_node_t * ucl_btree_find_deepest_son (const ucl_btree_node_t *node_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_inorder (const ucl_btree_node_t *cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_inorder_backward (const ucl_btree_node_t *cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_preorder (const ucl_btree_node_t *cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_postorder (const ucl_btree_node_t *cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_levelorder (const ucl_btree_node_t * cur_p);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "btree" module.
   A typedef for each function in the module. */

typedef void ucl_btree_constructor_proto_t (ucl_btree_node_t * this_p, const ucl_btree_node_t * dad_p);
typedef void ucl_btree_destructor_proto_t (ucl_btree_node_t * this);
typedef void ucl_btree_setbro_proto_t (ucl_btree_node_t * this, const ucl_btree_node_t * newBro);
typedef void ucl_btree_setson_proto_t (ucl_btree_node_t * this, const ucl_btree_node_t * newSon);
typedef void ucl_btree_setdad_proto_t (ucl_btree_node_t * this, const ucl_btree_node_t * newDad);
typedef void ucl_btree_dadbro_proto_t (ucl_btree_node_t * dad, ucl_btree_node_t * bro);
typedef void ucl_btree_dadson_proto_t (ucl_btree_node_t * dad, ucl_btree_node_t * son);
typedef ucl_btree_node_t * ucl_btree_getbro_proto_t (const ucl_btree_node_t * this);
typedef ucl_btree_node_t * ucl_btree_getson_proto_t (const ucl_btree_node_t * this);
typedef ucl_btree_node_t * ucl_btree_getdad_proto_t (const ucl_btree_node_t * this);
typedef ucl_btree_node_t * ucl_btree_find_leftmost_proto_t (const ucl_btree_node_t *node_p);
typedef ucl_btree_node_t * ucl_btree_find_rightmost_proto_t (const ucl_btree_node_t *node_p);
typedef ucl_btree_node_t * ucl_btree_find_deepest_son_proto_t (const ucl_btree_node_t *node_p);
typedef ucl_btree_node_t * ucl_btree_step_inorder_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_inorder_backward_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_preorder_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_postorder_proto_t (const ucl_btree_node_t *cur_p);
typedef ucl_btree_node_t * ucl_btree_step_levelorder_proto_t (const ucl_btree_node_t * cur_p);

/* Stub table type declaration for the "btree" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

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
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_btree_stub_table_t * ucl_btree_stub_table_p;
ucl_decl const ucl_btree_stub_table_t   ucl_btree_stub_table;

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


#endif /* defined __UCL_BTREE_H */

/* end of file */
