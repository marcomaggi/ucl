/*
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

#ifndef DEBUG_H
#define DEBUG_H 1


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif

#if (DEBUGGING == 1)
#  include <stdio.h>
#  include <stdarg.h>
#  include <string.h>
#endif


/** ------------------------------------------------------------
 ** Macros.
 ** ----------------------------------------------------------*/

#if (DEBUGGING == 1)
#  warning debugging is on
#  define debug(...)			debug_print(__FILE__,__func__,__LINE__,__VA_ARGS__)
#  define debug_question(EXPR)		((EXPR)?"yes":"no")

static void __attribute__((unused))
debug_print (const char * file, const char * function, int line, const char * template, ...)
{
  va_list	ap;
  int		len, i;


  /* Move file to  the last '/' char, so that we  print file's tail name
     but not the directory part. */
  len = strlen(file);
  for (i=len-1; i >=0 ; --i)
    {
      if ('/' == file[i])
	{
	  ++i;
	  break;
	}
    }
  file += i;

  fprintf(stderr, "%s:%-25s (%d) *** ", file, function, line);
  va_start(ap, template);
  vfprintf(stderr, template, ap);
  fprintf(stderr, "\n");
  fflush(stderr);
  va_end(ap);
}

#else
#  define debug(...)		/* empty */
#  define debug_question(...)	/* empty */
#endif


#endif /* DEBUG_H */

/* end of file */
