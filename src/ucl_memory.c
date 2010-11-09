/*
   Part of: Useless Containers Library
   Contents: memory allocator
   Date: Fri Nov 12, 2004

   Abstract

	This  module  declares  the  functions  used  to  handle  memory
	allocation.

   Copyright (c) 2004-2005, 2008-2010 Marco Maggi <marcomaggi@gna.org>

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

#include "internal.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>


/** ------------------------------------------------------------
 ** Allocator.
 ** ----------------------------------------------------------*/

void
ucl_memory_alloc (void * dummy UCL_UNUSED, void * qq, size_t dim)
{
  void **	pp = qq;
  void *	p;


  if (0 == dim)
    {
      if (NULL != *pp)
	{
	  free(*pp);
	  *pp = NULL;
	}
    }
  else
    {
      p = (NULL == *pp)? calloc(1, dim) : realloc(*pp, dim);
      if (NULL == p)
	{
	  perror(strerror(errno));
	  exit(EXIT_FAILURE);
	}
      *pp = p;
    }
}

/* end of file */
