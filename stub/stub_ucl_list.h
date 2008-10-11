/* Stub header file for the "list" module.
   This file must be included in the main stub header. */

#ifndef __UCL_LIST_H

/** ------------------------------------------------------------ 
 ** Module "list".
 ** --------------------------------------------------------- */

/* Function prototypes for the "list" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl size_t ucl_list_length (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_caar (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cadr (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cdar (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cddr (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_caaar (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_caadr (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cadar (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_caddr (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cdaar (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cdadr (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cddar (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_cdddr (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_remove (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_popfront (ucl_node_t node);
ucl_decl ucl_node_t ucl_list_popback (ucl_node_t node);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "list" module.
   A typedef for each function in the module. */

typedef size_t ucl_list_length_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_caar_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cadr_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cdar_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cddr_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_caaar_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_caadr_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cadar_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_caddr_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cdaar_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cdadr_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cddar_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_cdddr_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_remove_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_popfront_proto_t (ucl_node_t node);
typedef ucl_node_t ucl_list_popback_proto_t (ucl_node_t node);

/* Stub table type declaration for the "list" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_list_stub_table_t {
  ucl_list_length_proto_t             * stub_ucl_list_length;
  ucl_list_popfront_proto_t           * stub_ucl_list_popfront;
  ucl_list_remove_proto_t             * stub_ucl_list_remove;
  ucl_list_popback_proto_t            * stub_ucl_list_popback;
  ucl_list_caar_proto_t               * stub_ucl_list_caar;
  ucl_list_cadr_proto_t               * stub_ucl_list_cadr;
  ucl_list_cdar_proto_t               * stub_ucl_list_cdar;
  ucl_list_cddr_proto_t               * stub_ucl_list_cddr;
  ucl_list_caaar_proto_t              * stub_ucl_list_caaar;
  ucl_list_caadr_proto_t              * stub_ucl_list_caadr;
  ucl_list_cadar_proto_t              * stub_ucl_list_cadar;
  ucl_list_caddr_proto_t              * stub_ucl_list_caddr;
  ucl_list_cdaar_proto_t              * stub_ucl_list_cdaar;
  ucl_list_cdadr_proto_t              * stub_ucl_list_cdadr;
  ucl_list_cddar_proto_t              * stub_ucl_list_cddar;
  ucl_list_cdddr_proto_t              * stub_ucl_list_cdddr;
} ucl_list_stub_table_t;

/* Stub table macros definition for the "list" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_list_stub_table_t * ucl_list_stub_table_p;
ucl_decl const ucl_list_stub_table_t   ucl_list_stub_table;

#define ucl_list_length            ((ucl_list_stub_table_p)->stub_ucl_list_length)
#define ucl_list_popfront          ((ucl_list_stub_table_p)->stub_ucl_list_popfront)
#define ucl_list_remove            ((ucl_list_stub_table_p)->stub_ucl_list_remove)
#define ucl_list_popback           ((ucl_list_stub_table_p)->stub_ucl_list_popback)
#define ucl_list_caar              ((ucl_list_stub_table_p)->stub_ucl_list_caar)
#define ucl_list_cadr              ((ucl_list_stub_table_p)->stub_ucl_list_cadr)
#define ucl_list_cdar              ((ucl_list_stub_table_p)->stub_ucl_list_cdar)
#define ucl_list_cddr              ((ucl_list_stub_table_p)->stub_ucl_list_cddr)
#define ucl_list_caaar             ((ucl_list_stub_table_p)->stub_ucl_list_caaar)
#define ucl_list_caadr             ((ucl_list_stub_table_p)->stub_ucl_list_caadr)
#define ucl_list_cadar             ((ucl_list_stub_table_p)->stub_ucl_list_cadar)
#define ucl_list_caddr             ((ucl_list_stub_table_p)->stub_ucl_list_caddr)
#define ucl_list_cdaar             ((ucl_list_stub_table_p)->stub_ucl_list_cdaar)
#define ucl_list_cdadr             ((ucl_list_stub_table_p)->stub_ucl_list_cdadr)
#define ucl_list_cddar             ((ucl_list_stub_table_p)->stub_ucl_list_cddar)
#define ucl_list_cdddr             ((ucl_list_stub_table_p)->stub_ucl_list_cdddr)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_LIST_H */

/* end of file */
