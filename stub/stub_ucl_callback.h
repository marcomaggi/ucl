/* Stub header file for the "callback" module.
   This file must be included in the main stub header. */

#ifndef __UCL_CALLBACK_H

/** ------------------------------------------------------------ 
 ** Module "callback".
 ** --------------------------------------------------------- */

/* Function prototypes for the "callback" module.
   A prototype for each function in the module. */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_callback_apply (ucl_callback_t callback, ...);
ucl_decl void ucl_callback_eval_thunk (ucl_callback_t callback);
ucl_decl void ucl_callback_set_application_function (ucl_callback_apply_fun_t * f);

#endif /* not defined UCL_ENABLE_STUB */

/* Function type declarations for the "callback" module.
   A typedef for each function in the module. */

typedef void ucl_callback_apply_proto_t (ucl_callback_t callback, ...);
typedef void ucl_callback_eval_thunk_proto_t (ucl_callback_t callback);
typedef void ucl_callback_set_application_function_proto_t (ucl_callback_apply_fun_t * f);

/* Stub table type declaration for the "callback" module.
   A struct holding a pointer for each function in the module
   sorted by timestamp. */

typedef struct ucl_callback_stub_table_t {
  ucl_callback_apply_proto_t          * stub_ucl_callback_apply;
  ucl_callback_eval_thunk_proto_t     * stub_ucl_callback_eval_thunk;
  ucl_callback_set_application_function_proto_t * stub_ucl_callback_set_application_function;
} ucl_callback_stub_table_t;

/* Stub table macros definition for the "callback" module.
   A preprocessor macro for each function in the module,
   used to invoke the functions through the stub table. */

#ifdef UCL_ENABLE_STUB

ucl_decl const ucl_callback_stub_table_t * ucl_callback_stub_table_p;
ucl_decl const ucl_callback_stub_table_t   ucl_callback_stub_table;

#define ucl_callback_apply         ((ucl_callback_stub_table_p)->stub_ucl_callback_apply)
#define ucl_callback_eval_thunk    ((ucl_callback_stub_table_p)->stub_ucl_callback_eval_thunk)
#define ucl_callback_set_application_function  ((ucl_callback_stub_table_p)->stub_ucl_callback_set_application_function)

#endif /* defined UCL_ENABLE_STUB */


#endif /* defined __UCL_CALLBACK_H */

/* end of file */
