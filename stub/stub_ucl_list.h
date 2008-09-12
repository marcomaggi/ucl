/* Stub header file for the "list" module.
   This file must be included in the main stub header. */

#ifndef __UCL_LIST_H

/** ------------------------------------------------------------ 
 ** Module "list".
 ** --------------------------------------------------------- */

/* Function prototypes for the "list" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl ucl_list_link_t * ucl_list_index (ucl_list_t this, size_t idx);
ucl_decl void ucl_list_insertbefore (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
ucl_decl void ucl_list_insertafter (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
ucl_decl void ucl_list_pushfront (ucl_list_t this, ucl_list_link_t *link_p);
ucl_decl void ucl_list_pushback (ucl_list_t this, ucl_list_link_t *link_p);
ucl_decl void ucl_list_extract (ucl_list_t this, ucl_list_link_t *link_p);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "list" module.
   A typedef for each function in the module. */

typedef ucl_list_link_t * ucl_list_index_proto_t (ucl_list_t this, size_t idx);
typedef void ucl_list_insertbefore_proto_t (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
typedef void ucl_list_insertafter_proto_t (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
typedef void ucl_list_pushfront_proto_t (ucl_list_t this, ucl_list_link_t *link_p);
typedef void ucl_list_pushback_proto_t (ucl_list_t this, ucl_list_link_t *link_p);
typedef void ucl_list_extract_proto_t (ucl_list_t this, ucl_list_link_t *link_p);

/* Stub table type declaration for the "list" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

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
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_list_stub_table_t * ucl_list_stub_table_p;
ucl_decl const ucl_list_stub_table_t   ucl_list_stub_table;

#define ucl_list_index             ((ucl_list_stub_table_p)->stub_ucl_list_index)
#define ucl_list_insertbefore      ((ucl_list_stub_table_p)->stub_ucl_list_insertbefore)
#define ucl_list_insertafter       ((ucl_list_stub_table_p)->stub_ucl_list_insertafter)
#define ucl_list_pushfront         ((ucl_list_stub_table_p)->stub_ucl_list_pushfront)
#define ucl_list_pushback          ((ucl_list_stub_table_p)->stub_ucl_list_pushback)
#define ucl_list_extract           ((ucl_list_stub_table_p)->stub_ucl_list_extract)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_LIST_H */

/* end of file */
