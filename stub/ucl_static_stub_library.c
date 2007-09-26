/* Library specific header files. */

#include "ucl.h"


/* ------------------------------------------------------------
   Stub tables initialiser for "".
   ------------------------------------------------------------ */

const ucl_btree_stub_table_t *	ucl_btree_stub_table_p;
const ucl_base_stub_table_t *	ucl_base_stub_table_p;
const ucl_circular_stub_table_t *	ucl_circular_stub_table_p;
const ucl_debug_stub_table_t *	ucl_debug_stub_table_p;
const ucl_graph_stub_table_t *	ucl_graph_stub_table_p;
const ucl_hash_stub_table_t *	ucl_hash_stub_table_p;
const ucl_heap_stub_table_t *	ucl_heap_stub_table_p;
const ucl_list_stub_table_t *	ucl_list_stub_table_p;
const ucl_map_stub_table_t *	ucl_map_stub_table_p;
const ucl_memory_stub_table_t *	ucl_memory_stub_table_p;
const ucl_tree_stub_table_t *	ucl_tree_stub_table_p;
const ucl_vector_stub_table_t *	ucl_vector_stub_table_p;


static void
init_pointers (ucl_stub_table_t *table_p)
{
  ucl_btree_stub_table_p         = table_p->btree;
  ucl_base_stub_table_p          = table_p->base;
  ucl_circular_stub_table_p      = table_p->circular;
  ucl_debug_stub_table_p         = table_p->debug;
  ucl_graph_stub_table_p         = table_p->graph;
  ucl_hash_stub_table_p          = table_p->hash;
  ucl_heap_stub_table_p          = table_p->heap;
  ucl_list_stub_table_p          = table_p->list;
  ucl_map_stub_table_p           = table_p->map;
  ucl_memory_stub_table_p        = table_p->memory;
  ucl_tree_stub_table_p          = table_p->tree;
  ucl_vector_stub_table_p        = table_p->vector;

}
#include <dlfcn.h>

static void *library_handle = NULL;

const char *
ucl_init_stub_table (void)
{
  const char *	err;
  void * table;

  /* this requires glibc 2.2 or later, see dlopen(3) for details */
  library_handle = dlopen("libuclstub2.so", RTLD_NOLOAD|RTLD_GLOBAL|RTLD_NOW);
  if (NULL == library_handle)
    {
      err = dlerror();
      if (err) return err;
      library_handle = dlopen("libuclstub2.so", RTLD_GLOBAL|RTLD_NOW);
      err = dlerror();
      if (err) return err;
    }
  table = dlsym(library_handle, "ucl_stub_table");
  err = dlerror();
  if (err) { dlclose(library_handle); return err; }

  init_pointers(table);
  return NULL;
}
int
ucl_final_stub_table (void)
{
  int	errcode;

  if (library_handle)
    {
      errcode = dlclose(library_handle);
      if (0 == errcode) { library_handle = NULL; }
      return errcode;
    }
  return 0;
}


/* end of file */
