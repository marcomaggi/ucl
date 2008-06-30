/*
   Part of: Useless Containers Library
   Contents: preprocessor symbols
   Date: Wed Nov  3, 2004
     
   Abstract
     
	This header  file must  be included in  all the source  files of
	UCL.

   Copyright (c) 2004, 2005, 2007, 2008 Marco Maggi
     
   
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>	/* sometimes useful for debugging */
#include "ucl.h"
#include "debug.h"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Stub stuff.
 ** ----------------------------------------------------------*/

/* "stub"  is used  to declare  the  timestamp for  stub functions;  the
   timestamp establishes the order in which the functions will appear in
   the stub tables: do not change them. */
#define stub(TIMESTAMP)		/* empty */

/* "attibute" is used to declare GCC function declaration attributes; it
   is read  by the  headers building script  and added to  the prototype
   declaration.  To  know more about  attributes read the GCC  info node
   "Function Attributes". */
#define attribute(TIMESTAMP)	/* empty */

/* end of file */
