/* Global stub header file for the "ucl" library. */

#ifndef __UCL_STUB_H
#define __UCL_STUB_H


/** ------------------------------------------------------------ 
 ** Load stub header files, one for each module.
 ** --------------------------------------------------------- */

#include <stub_ucl_btree.h>


/** ------------------------------------------------------------ 
 ** Stub library loading functions.
 ** --------------------------------------------------------- */

#ifdef UCL_ENABLE_STUB

ucl_decl const char * ucl_init_stub_table (void);
ucl_decl int          ucl_final_stub_table (void);

#endif /* defined UCL_ENABLE_STUB */

#endif /* defined __UCL_STUB_H */

/* end of file */
