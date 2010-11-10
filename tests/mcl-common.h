/*
  Part of: Marco's C Library
  Contents: common definitions
  Date: Mon Apr 20, 2009

  Abstract

	Common definitions for C language code.

  Copyright (c) 2009 Marco Maggi <marcomaggi@gna.org>

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

#ifndef MCL_DEFINES_H
#define MCL_DEFINES_H 1



#ifdef HAVE_STDBOOL_H
#  include <stdbool.h>
# else
typedef int		bool;
#  define true			1
#  define false			0
#endif

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>



#undef MCL_BEGIN_C_DECL
#undef MCL_END_C_DECL
#ifdef __cplusplus
#  define MCL_BEGIN_C_DECL		extern "C" {
#  define MCL_END_C_DECL		}
#else
#  define MCL_BEGIN_C_DECL		/* empty */
#  define MCL_END_C_DECL		/* empty */
#endif

MCL_BEGIN_C_DECL

/* The macro MCL_UNUSED indicates  that a function, function argument or
   variable may potentially be unused.  Usage examples:

   static int unused_function (char arg) MCL_UNUSED;
   static int MCL_UNUSED
   unused_function (char arg) {
     ...
   }
   int foo (char unused_argument MCL_UNUSED);
   int unused_variable MCL_UNUSED;
*/
#ifdef __GNUC__
#  define MCL_UNUSED		__attribute__((unused))
#else
#  define MCL_UNUSED		/* empty */
#  define __attribute__(...)	/* empty */
#endif

#ifndef mcl_decl
#  define mcl_decl		extern
#endif



typedef struct {
  size_t	len;	/* this is the number of chars NOT including the
			   trailing zero byte */
  char *	ptr;
} mcl_ascii_t;

typedef struct {
  size_t	len;
  char **	ptr;
} mcl_ascii_list_t;

typedef struct {
  size_t	len;
  uint8_t *	ptr;
} mcl_block_t;

typedef union {
    char		t_char;
    unsigned char	t_unsigned_char;
    int			t_int;
    unsigned int	t_unsigned_int;
    long		t_long;
    unsigned long	t_unsigned_long;

    int8_t		t_int8;
    uint8_t		t_uint8;
    int16_t		t_int16;
    uint16_t		t_uint16;
    int32_t		t_int32;
    uint32_t		t_uint32;

    intptr_t		t_intptr;
    uintptr_t		t_uintptr;

    void *		pointer;
    uint8_t *		bytes;
    char *		string;
} mcl_value_t;



#include <mcl-debug.h>

#endif /* MCL_DEFINES_H */

/* end of file */
