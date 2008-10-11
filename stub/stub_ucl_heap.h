/* Stub header file for the "heap" module.
   This file must be included in the main stub header. */

#ifndef __UCL_HEAP_H

/** ------------------------------------------------------------ 
 ** Module "heap".
 ** --------------------------------------------------------- */

/* Function prototypes for the "heap" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_heap_insert (ucl_heap_t this, void * _node_p);
ucl_decl ucl_node_t  ucl_heap_extract (ucl_heap_t this);
ucl_decl void ucl_heap_merge (ucl_heap_t this, ucl_heap_t other);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "heap" module.
   A typedef for each function in the module. */

typedef void ucl_heap_insert_proto_t (ucl_heap_t this, void * _node_p);
typedef ucl_node_t  ucl_heap_extract_proto_t (ucl_heap_t this);
typedef void ucl_heap_merge_proto_t (ucl_heap_t this, ucl_heap_t other);

/* Stub table type declaration for the "heap" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_heap_stub_table_t {
  ucl_heap_insert_proto_t             * stub_ucl_heap_insert;
  ucl_heap_extract_proto_t            * stub_ucl_heap_extract;
  ucl_heap_merge_proto_t              * stub_ucl_heap_merge;
} ucl_heap_stub_table_t;

/* Stub table macros definition for the "heap" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_heap_stub_table_t * ucl_heap_stub_table_p;
ucl_decl const ucl_heap_stub_table_t   ucl_heap_stub_table;

#define ucl_heap_insert            ((ucl_heap_stub_table_p)->stub_ucl_heap_insert)
#define ucl_heap_extract           ((ucl_heap_stub_table_p)->stub_ucl_heap_extract)
#define ucl_heap_merge             ((ucl_heap_stub_table_p)->stub_ucl_heap_merge)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_HEAP_H */

/* end of file */
