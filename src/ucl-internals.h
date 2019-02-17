/*
   Part of: Useless Containers Library
   Contents: internal header file
   Date: Wed Nov  3, 2004

   Abstract

	This header file must be included in all the source files of UCL.

   Copyright (c) 2004-2005, 2007-2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

   This program is free software: you can  redistribute it and/or modify it under the
   terms  of  the GNU  General  Public  License as  published  by  the Free  Software
   Foundation,  either version  3  of the  License,  or (at  your  option) any  later
   version.

   This program is  distributed in the hope  that it will be useful,  but WITHOUT ANY
   WARRANTY; without  even the implied warranty  of MERCHANTABILITY or FITNESS  FOR A
   PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of  the GNU General Public License along with this
   program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UCL_INTERNALS_H
#define UCL_INTERNALS_H 1


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>	/* sometimes useful for debugging */
#include <limits.h>
#include "ucl.h"
#include "ucl-debug.h"


/** --------------------------------------------------------------------
 ** Preprocessor macros.
 ** ----------------------------------------------------------------- */

/* Define the "printf()" code to print "size_t" values. */
#if (4 == MMUX_SIZEOF_SIZE_T)
#  define MMUX_PRIuSIZE		"u"
#elif (8 == MMUX_SIZEOF_SIZE_T)
#  define MMUX_PRIuSIZE		"lu"
#else
#  error "unable to determine the size of 'size_t'"
#endif

/* Define the "printf()" code to print "ssize_t" values. */
#if (4 == MMUX_SIZEOF_SSIZE_T)
#  define MMUX_PRIdSIZE		"d"
#elif (8 == MMUX_SIZEOF_SSIZE_T)
#  define MMUX_PRIdSIZE		"ld"
#else
#  error "unable to determine the size of 'ssize_t'"
#endif

/* Define the "printf()" code to print "ptrdiff_t" values. */
#if (4 == MMUX_SIZEOF_PTRDIFF_T)
#  define MMUX_PRIdPTRDIFF	"d"
#elif (8 == MMUX_SIZEOF_PTRDIFF_T)
#  define MMUX_PRIdPTRDIFF	"ld"
#else
#  error "unable to determine the size of 'ptrdiff_t'"
#endif


/** --------------------------------------------------------------------
 ** Constants.
 ** ----------------------------------------------------------------- */




/** --------------------------------------------------------------------
 ** Function prototypes.
 ** ----------------------------------------------------------------- */

extern ucl_callback_apply_fun_t * ucl_callback_application_function;


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UCL_INTERNALS_H */

/* end of file */
