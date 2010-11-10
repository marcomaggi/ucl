/*
  Part of: Marco's C Library
  Contents: debugging macros and functions
  Date: Sat Jun 28, 2008

  Abstract

        Define a set of generic macros and functions.

  Copyright (c) 2008, 2009 Marco Maggi <marcomaggi@gna.org>

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

#ifndef MCL_DEBUG_H
#define MCL_DEBUG_H 1



#include <mcl-common.h>

#ifndef MCL_DEBUGGING
#  define MCL_DEBUGGING			0
#endif
#ifndef MCL_DEBUGGING_HERE
#  define MCL_DEBUGGING_HERE		1
#endif
#ifndef MCL_FPRINTF
#  define MCL_FPRINTF			fprintf
#endif
#ifndef MCL_VFPRINTF
#  define MCL_VFPRINTF			vfprintf
#endif



#if (MCL_DEBUGGING == 1)
#  warning debugging is on

#  define mcl_debug_question(EXPR)		((EXPR)?"yes":"no")
#  define mcl_debug_mark()			mcl_debug("mark")

#  define mcl_debug(...)					\
								\
  if (MCL_DEBUGGING_HERE)					\
    mcl_debug_print(__FILE__,__func__,__LINE__,__VA_ARGS__)

static void MCL_UNUSED
mcl_debug_print (const char * file, const char * function, int line, const char * template, ...)
{
  va_list	ap;
  int		len, i;
  /* Move "file" to the last '/' char, so that we print file's tail name
     but not the directory part. */
  len = strlen(file);
  for (i=len-1; (i>=0) && ('/' != file[i]) ; --i);
  if ('/' == file[i])
    ++i;
  file += i;
/*   MCL_FPRINTF(stderr, "%s:%-25s (%d) *** ", file, function, line); */
  MCL_FPRINTF(stderr, "%-30s(%s:%d) *** ", function, file, line);
  va_start(ap, template);
  MCL_VFPRINTF(stderr, template, ap);
  MCL_FPRINTF(stderr, "\n");
  fflush(stderr);
  va_end(ap);
}

#else
#  define mcl_debug(...)		/* empty */
#  define mcl_debug_mark()		/* empty */
#  define mcl_debug_question(...)	/* empty */
#endif



#endif /* MCL_DEBUG_H */

/* end of file */
