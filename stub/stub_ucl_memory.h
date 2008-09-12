/* Stub header file for the "memory" module.
   This file must be included in the main stub header. */

#ifndef __UCL_MEMORY_H

/** ------------------------------------------------------------ 
 ** Module "memory".
 ** --------------------------------------------------------- */

/* Function prototypes for the "memory" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_memory_alloc (void * dummy UCL_UNUSED, void * qq, size_t dim);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "memory" module.
   A typedef for each function in the module. */

typedef void ucl_memory_alloc_proto_t (void * dummy UCL_UNUSED, void * qq, size_t dim);

/* Stub table type declaration for the "memory" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_memory_stub_table_t {
  ucl_memory_alloc_proto_t            * stub_ucl_memory_alloc;
} ucl_memory_stub_table_t;

/* Stub table macros definition for the "memory" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_memory_stub_table_t * ucl_memory_stub_table_p;
ucl_decl const ucl_memory_stub_table_t   ucl_memory_stub_table;

#define ucl_memory_alloc           ((ucl_memory_stub_table_p)->stub_ucl_memory_alloc)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_MEMORY_H */

/* end of file */
