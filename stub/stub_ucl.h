/* Stub header file for the "base" module.
   This file must be included in the main stub header. */

#ifndef __UCL_BASE_H

/** ------------------------------------------------------------ 
 ** Module "base".
 ** --------------------------------------------------------- */

/* Function prototypes for the "base" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl const char * ucl_version (void);
ucl_decl const char * ucl_major_version (void);
ucl_decl const char * ucl_minor_version (void);
ucl_decl const char * ucl_patch_version (void);
ucl_decl unsigned ucl_interface_major_version (void);
ucl_decl unsigned ucl_interface_minor_version (void);
ucl_decl int ucl_intcmp (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
ucl_decl int ucl_uintcmp (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
ucl_decl int ucl_strcmp (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
ucl_decl int ucl_ptrintcmp (ucl_value_t data, const ucl_value_t a, const ucl_value_t b);
ucl_decl size_t ucl_hash_string (ucl_value_t data UCL_UNUSED, const ucl_value_t val);
ucl_decl void ucl_quicksort (void *const pbase, size_t total_elems, size_t size, ucl_valcmp_t cmp);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "base" module.
   A typedef for each function in the module. */

typedef const char * ucl_version_proto_t (void);
typedef const char * ucl_major_version_proto_t (void);
typedef const char * ucl_minor_version_proto_t (void);
typedef const char * ucl_patch_version_proto_t (void);
typedef unsigned ucl_interface_major_version_proto_t (void);
typedef unsigned ucl_interface_minor_version_proto_t (void);
typedef int ucl_intcmp_proto_t (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
typedef int ucl_uintcmp_proto_t (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
typedef int ucl_strcmp_proto_t (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b);
typedef int ucl_ptrintcmp_proto_t (ucl_value_t data, const ucl_value_t a, const ucl_value_t b);
typedef size_t ucl_hash_string_proto_t (ucl_value_t data UCL_UNUSED, const ucl_value_t val);
typedef void ucl_quicksort_proto_t (void *const pbase, size_t total_elems, size_t size, ucl_valcmp_t cmp);

/* Stub table type declaration for the "base" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_base_stub_table_t {
  ucl_version_proto_t                 * stub_ucl_version;
  ucl_major_version_proto_t           * stub_ucl_major_version;
  ucl_minor_version_proto_t           * stub_ucl_minor_version;
  ucl_patch_version_proto_t           * stub_ucl_patch_version;
  ucl_intcmp_proto_t                  * stub_ucl_intcmp;
  ucl_uintcmp_proto_t                 * stub_ucl_uintcmp;
  ucl_ptrintcmp_proto_t               * stub_ucl_ptrintcmp;
  ucl_hash_string_proto_t             * stub_ucl_hash_string;
  ucl_interface_major_version_proto_t * stub_ucl_interface_major_version;
  ucl_interface_minor_version_proto_t * stub_ucl_interface_minor_version;
  ucl_strcmp_proto_t                  * stub_ucl_strcmp;
  ucl_quicksort_proto_t               * stub_ucl_quicksort;
} ucl_base_stub_table_t;

/* Stub table macros definition for the "base" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_base_stub_table_t * ucl_base_stub_table_p;
ucl_decl const ucl_base_stub_table_t   ucl_base_stub_table;

#define ucl_version                ((ucl_base_stub_table_p)->stub_ucl_version)
#define ucl_major_version          ((ucl_base_stub_table_p)->stub_ucl_major_version)
#define ucl_minor_version          ((ucl_base_stub_table_p)->stub_ucl_minor_version)
#define ucl_patch_version          ((ucl_base_stub_table_p)->stub_ucl_patch_version)
#define ucl_intcmp                 ((ucl_base_stub_table_p)->stub_ucl_intcmp)
#define ucl_uintcmp                ((ucl_base_stub_table_p)->stub_ucl_uintcmp)
#define ucl_ptrintcmp              ((ucl_base_stub_table_p)->stub_ucl_ptrintcmp)
#define ucl_hash_string            ((ucl_base_stub_table_p)->stub_ucl_hash_string)
#define ucl_interface_major_version  ((ucl_base_stub_table_p)->stub_ucl_interface_major_version)
#define ucl_interface_minor_version  ((ucl_base_stub_table_p)->stub_ucl_interface_minor_version)
#define ucl_strcmp                 ((ucl_base_stub_table_p)->stub_ucl_strcmp)
#define ucl_quicksort              ((ucl_base_stub_table_p)->stub_ucl_quicksort)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_BASE_H */

/* end of file */
