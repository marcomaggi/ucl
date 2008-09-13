/* Static stub library for the "ucl" library.
   This file must be compiled and statically linked with the user code. */

#include "ucl.h"


/** ------------------------------------------------------------ 
 ** Pointers to the module stub tables of "ucl".
 ** --------------------------------------------------------- */

const ucl_btree_stub_table_t *        ucl_btree_stub_table_p;
const ucl_base_stub_table_t *         ucl_base_stub_table_p;
const ucl_circular_stub_table_t *     ucl_circular_stub_table_p;
const ucl_graph_stub_table_t *        ucl_graph_stub_table_p;
const ucl_hash_stub_table_t *         ucl_hash_stub_table_p;
const ucl_heap_stub_table_t *         ucl_heap_stub_table_p;
const ucl_list_stub_table_t *         ucl_list_stub_table_p;
const ucl_map_stub_table_t *          ucl_map_stub_table_p;
const ucl_memory_stub_table_t *       ucl_memory_stub_table_p;
const ucl_tree_stub_table_t *         ucl_tree_stub_table_p;
const ucl_vector_stub_table_t *       ucl_vector_stub_table_p;


/** ------------------------------------------------------------ 
 ** Table of symbols to retrieve from the shared stub library.
 ** --------------------------------------------------------- */

typedef struct symbol_tag_t {
   const char * name;
   void **      pointer;
} symbol_tag_t;

typedef symbol_tag_t *   symbol_t;


static const symbol_tag_t	symbols[] = {
  { "ucl_btree_stub_table",		(void **)&ucl_btree_stub_table_p },
  { "ucl_base_stub_table",		(void **)&ucl_base_stub_table_p },
  { "ucl_circular_stub_table",		(void **)&ucl_circular_stub_table_p },
  { "ucl_graph_stub_table",		(void **)&ucl_graph_stub_table_p },
  { "ucl_hash_stub_table",		(void **)&ucl_hash_stub_table_p },
  { "ucl_heap_stub_table",		(void **)&ucl_heap_stub_table_p },
  { "ucl_list_stub_table",		(void **)&ucl_list_stub_table_p },
  { "ucl_map_stub_table",		(void **)&ucl_map_stub_table_p },
  { "ucl_memory_stub_table",		(void **)&ucl_memory_stub_table_p },
  { "ucl_tree_stub_table",		(void **)&ucl_tree_stub_table_p },
  { "ucl_vector_stub_table",		(void **)&ucl_vector_stub_table_p },
  { NULL, NULL }
};


/** ------------------------------------------------------------ 
 ** Loading and unloading the shared stub library.
 ** --------------------------------------------------------- */

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const char *     library_name      = "libuclstub2.so";
static void *           library_handle    = NULL;
static int              debug_notice_flag = 0;

static void
debug_init (void)
{
  const char * string = getenv("UCL_DEBUG");

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
      fprintf(stderr, "%s debug: ", "ucl");
      vfprintf(stderr, template, ap);
      fprintf(stderr, "\n");
      va_end(ap);
    }
}

const char *
ucl_init_stub_table (void)
{
  const char *  err;


  debug_init();
  debug_notice("loading stub library \"%s\"...", library_name);

  /* this requires glibc 2.2 or later, see dlopen(3) for details */
  library_handle = dlopen(library_name, RTLD_NOLOAD|RTLD_GLOBAL|RTLD_NOW);
  if (NULL == library_handle)
    {
      err = dlerror();
      if (err) return err;
      library_handle = dlopen(library_name, RTLD_GLOBAL|RTLD_NOW);
      err = dlerror();
      if (err) return err;
    }
  debug_notice("successfully loaded stub library \"%s\"", library_name);

  for (size_t i = 0; NULL != symbols[i].pointer; ++i)
    {
      *(symbols[i].pointer) = dlsym(library_handle, symbols[i].name);
      err = dlerror();
      if (err) { dlclose(library_handle); return err; }
    }
  debug_notice("successfully loaded stub tables from \"%s\"", library_name);

  return NULL;
}
int
ucl_final_stub_table (void)
{
  int	errcode;

  if (library_handle)
    {
      debug_notice("unloading stub library \"%s\"...", library_name);

      errcode = dlclose(library_handle);
      if (0 == errcode)
        library_handle = NULL;
      else
        return errcode;

      debug_notice("successfully unloaded stub library \"%s\"...", library_name);
    }
  return 0;
}

/* end of file */
