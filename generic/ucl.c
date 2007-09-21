/* ucl.c  --

   Part of: Useless Container Library
   Contents: the table of error message strings.

   Abstract:

	This file  must be included in  all the modules that  use any of
	the UCL containers.

   Copyright (c) 2001, 2002, 2003 Marco Maggi

   This library is free software;  you can redistribute it and/or modify
   it  under the  terms  of the  GNU  Lesser General  Public License  as
   published by the Free Software  Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.

   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA

   $Id: ucl.c,v 2.0.1.14 2003/12/08 20:10:55 marco Exp $

*/


#include "ucl.h"

const char *
ucl_version (void)
{
  return PACKAGE_VERSION;
}



/* ucl_intcmp --

	Compare the integers in the UCL values.

   Arguments:

	a -	the first value
	b -	the second value

   Results:

        Returns zero if the integers are equal, 1 if a>b, 0 if b>a.

   Side effects:

        None.

*/

int
ucl_intcmp (const ucl_value_t a, const ucl_value_t b)
{
  return ((a.num == b.num)? 0 : ((a.num > b.num)? 1 : -1));
}


/* end of file */
