/* testmain.c --
   
   Part of: Useless Containers Library
   Contents: main test file
   Date: Fri Sep 23, 2005
   
   Abstract
   
   
   
   Copyright (c) 2005 Marco Maggi
   
   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.
   
*/


/* #define UCL_DEBUGGING	1 */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <config.h>
#include "ucl.h"
#include "ucl_stub.h"

#define err(...)	fprintf(stderr, __VA_ARGS__)

extern void test(void);

int
main (int argc UCL_UNUSED, const char *const argv[])
{
#ifdef UCL_ENABLE_STUB
  const char *	e;

  err("%s: stub enabled, opening shared library: %s\n",
      argv[0], ucl_stub_SHARED_LIBRARY_LINK_NAME);
  e = ucl_init_stub_table();
  if (e)
    {
      err("%s: %s\n", argv[0], e);
      exit(EXIT_FAILURE);
    }
  err("%s: stub enabled, successfully opened shared library\n", argv[0]);
#endif
  
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);

#ifdef UCL_ENABLE_STUB
  err("%s: stub enabled, closing shared library: %s\n",
      argv[0], ucl_stub_SHARED_LIBRARY_LINK_NAME);
  if (ucl_final_stub_table())
    {
      err("%s: %s\n", argv[0], dlerror());
      exit(EXIT_FAILURE);
    }
  err("%s: stub enabled, successfully closed shared library\n", argv[0]);
#endif

  exit(EXIT_SUCCESS);
}


/* end of file */
