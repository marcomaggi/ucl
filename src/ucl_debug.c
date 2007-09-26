/* ucl_debug.c --
   
   Part of: UCL
   Contents: debug functions
   Date: Fri Mar  3, 2006
   
   Abstract
   
   
   
   Copyright (c) 2006, 2007 Marco Maggi
   
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



/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include "ucl_defines.h"

/* stub module name debug */

static ucl_bool_t	on = 1;



stub(2006-03-03-15-39-41) void
ucl_debug_on (void)
{
  on = 1;
}
stub(2006-03-03-15-44-39) void
ucl_debug_off (void)
{
  on = 0;
}
stub(2006-03-03-15-40-58) void
ucl_debug_print (const char *function, const char *template, ...)
{
  va_list	ap;


  if (on)
    {
      fprintf(stderr, "%-25s*** ", function);
      va_start(ap, template);
      vfprintf(stderr, template, ap);
      fprintf(stderr, "\n");
      fflush(stderr);
      va_end(ap);
    }
}
stub(2006-03-03-15-50-43) const char *
ucl_question(ucl_bool_t expr)
{
  return ((expr)? "yes" : "no");
}


/* end of file */
