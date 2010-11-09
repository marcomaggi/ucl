/* Stub header file for the "circular" module.
   This file must be included in the main stub header. */

#ifndef __UCL_CIRCULAR_H

/** ------------------------------------------------------------ 
 ** Module "circular".
 ** --------------------------------------------------------- */

/* Function prototypes for the "circular" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_circular_insert (ucl_circular_t this, ucl_circular_link_t *new);
ucl_decl ucl_circular_link_t * ucl_circular_extract (ucl_circular_t this)
	__attribute__((__noinline__));
ucl_decl void ucl_circular_forward (ucl_circular_t this, int times);
ucl_decl ucl_circular_link_t * ucl_circular_find (ucl_circular_t this, ucl_value_t val);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "circular" module.
   A typedef for each function in the module. */

typedef void ucl_circular_insert_proto_t (ucl_circular_t this, ucl_circular_link_t *new);
typedef ucl_circular_link_t * ucl_circular_extract_proto_t (ucl_circular_t this);
typedef void ucl_circular_forward_proto_t (ucl_circular_t this, int times);
typedef ucl_circular_link_t * ucl_circular_find_proto_t (ucl_circular_t this, ucl_value_t val);

/* Stub table type declaration for the "circular" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_circular_stub_table_t {
  ucl_circular_insert_proto_t         * stub_ucl_circular_insert;
  ucl_circular_extract_proto_t        * stub_ucl_circular_extract;
  ucl_circular_forward_proto_t        * stub_ucl_circular_forward;
  ucl_circular_find_proto_t           * stub_ucl_circular_find;
} ucl_circular_stub_table_t;

/* Stub table macros definition for the "circular" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_circular_stub_table_t * ucl_circular_stub_table_p;
ucl_decl const ucl_circular_stub_table_t   ucl_circular_stub_table;

#define ucl_circular_insert        ((ucl_circular_stub_table_p)->stub_ucl_circular_insert)
#define ucl_circular_extract       ((ucl_circular_stub_table_p)->stub_ucl_circular_extract)
#define ucl_circular_forward       ((ucl_circular_stub_table_p)->stub_ucl_circular_forward)
#define ucl_circular_find          ((ucl_circular_stub_table_p)->stub_ucl_circular_find)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_CIRCULAR_H */

/* end of file */
