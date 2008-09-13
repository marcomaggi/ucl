/* Global stub header file for the "ucl" library. */

#ifndef __UCL_STUB_H
#define __UCL_STUB_H


/** ------------------------------------------------------------ 
 ** Load stub header files, one for each module.
 ** --------------------------------------------------------- */

#include <stub_ucl_btree.h>
#include <stub_ucl.h>
#include <stub_ucl_circular.h>
#include <stub_ucl_graph.h>
#include <stub_ucl_hash.h>
#include <stub_ucl_heap.h>
#include <stub_ucl_list.h>
#include <stub_ucl_map.h>
#include <stub_ucl_memory.h>
#include <stub_ucl_tree.h>
#include <stub_ucl_vector.h>


/** ------------------------------------------------------------ 
 ** Stub library loading functions.
 ** --------------------------------------------------------- */

#ifdef UCL_ENABLE_STUB

ucl_decl const char * ucl_init_stub_table (void);
ucl_decl int          ucl_final_stub_table (void);

#endif /* defined UCL_ENABLE_STUB */

#endif /* defined __UCL_STUB_H */

/* end of file */
