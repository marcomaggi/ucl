/* Library specific header files. */

#include "ucl.h"


/* ------------------------------------------------------------
   Stub tables initialiser for "".
   ------------------------------------------------------------ */

const ucl_btree_stub_table_t *	ucl_btree_stub_table_p;
const ucl_base_stub_table_t *	ucl_base_stub_table_p;
const ucl_circular_stub_table_t *	ucl_circular_stub_table_p;
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define LIBRARY_IDENTIFIER              "ucl"
#define DEBUG_ENVIRONMENT_VARIABLE      "UCL_DEBUG"
#define SHARED_LIBRARY_NAME             "libuclstub2.so"
#define MAIN_STUB_TABLE_NAME            "ucl_stub_table"
#define MAIN_STUB_TABLE_INIT_FUNCTION   ucl_init_stub_table
#define MAIN_STUB_TABLE_FINAL_FUNCTION  ucl_final_stub_table

static void *   library_handle    = NULL;
static int      debug_notice_flag = 0;

static void
debug_init (void)
{
  const char * string = getenv(DEBUG_ENVIRONMENT_VARIABLE);

  if (NULL != string)
    {
      debug_notice_flag = (0 == strcmp(string, "notice"));
    }
}
static void
debug_notice (const char *template, ...)
{
  va_list	ap;

  if (debug_notice_flag)
    {
      va_start(ap, template);
      fprintf(stderr, "%s debug: ", LIBRARY_IDENTIFIER);
      vfprintf(stderr, template, ap);
      fprintf(stderr, "\n");
      va_end(ap);
    }
}

const char *
MAIN_STUB_TABLE_INIT_FUNCTION (void)
{
  const char *  err;
  void *        table;

  debug_init();
  debug_notice("loading stub library \"%s\"...", SHARED_LIBRARY_NAME);

  /* this requires glibc 2.2 or later, see dlopen(3) for details */
  library_handle = dlopen(SHARED_LIBRARY_NAME, RTLD_NOLOAD|RTLD_GLOBAL|RTLD_NOW);
  if (NULL == library_handle)
    {
      err = dlerror();
      if (err) return err;
      library_handle = dlopen(SHARED_LIBRARY_NAME, RTLD_GLOBAL|RTLD_NOW);
      err = dlerror();
      if (err) return err;
    }
  table = dlsym(library_handle, MAIN_STUB_TABLE_NAME);
  err = dlerror();
  if (err) { dlclose(library_handle); return err; }

  debug_notice("successfully loaded stub library \"%s\"", SHARED_LIBRARY_NAME);
  init_pointers(table);
  return NULL;
}
int
MAIN_STUB_TABLE_FINAL_FUNCTION (void)
{
  int	errcode;

  if (library_handle)
    {
      debug_notice("unloading stub library \"%s\"...", SHARED_LIBRARY_NAME);

      errcode = dlclose(library_handle);
      if (0 == errcode)
        library_handle = NULL;
      else
        return errcode;

      debug_notice("successfully unloaded stub library \"%s\"...", SHARED_LIBRARY_NAME);
    }
  return 0;
}


/* end of file */
