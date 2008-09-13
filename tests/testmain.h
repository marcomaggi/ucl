/*   
   Part of: Useless Containers Library
   Contents: main test file
   Date: Fri Sep 23, 2005
   
   Abstract
   
   
   
   Copyright (c) 2005, 2008 Marco Maggi
   
   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.
   
   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "ucl.h"

#define ucl_stub_shared_library_LINK_ID		"libuclstub2.0.so"

static void test(void);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc UCL_UNUSED, const char *const argv[])
{
#ifdef UCL_ENABLE_STUB
  const char *	e;

  fprintf(stderr,"%s: stub enabled, opening shared library: %s\n",
      argv[0], ucl_stub_shared_library_LINK_ID);
  e = ucl_init_stub_table();
  if (e)
    {
      fprintf(stderr,"%s: %s\n", argv[0], e);
      exit(EXIT_FAILURE);
    }
  fprintf(stderr,"%s: stub enabled, successfully opened shared library\n", argv[0]);
#endif
  
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);

#ifdef UCL_ENABLE_STUB
  fprintf(stderr,"%s: stub enabled, closing shared library: %s\n",
      argv[0], ucl_stub_shared_library_LINK_ID);
  if (ucl_final_stub_table())
    {
      fprintf(stderr,"%s: %s\n", argv[0], dlerror());
      exit(EXIT_FAILURE);
    }
  fprintf(stderr,"%s: stub enabled, successfully closed shared library\n", argv[0]);
#endif

  exit(EXIT_SUCCESS);
}


/* end of file */
