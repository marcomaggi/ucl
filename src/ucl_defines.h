/* ucl_defines.h --
     
   Part of: Useless Containers Library
   Contents: preprocessor symbols
   Date: Wed Nov  3, 2004
     
   Abstract
     
     
   Copyright (c) 2004, 2005, 2007 Marco Maggi
     
   This is free software you  can redistribute it and/or modify it under
   the terms of the GNU General  Public License as published by the Free
   Software Foundation; either version 2,  or (at your option) any later
   version.
   
   This file  is distributed  in the  hope that it  will be  useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with  this file; see  the file COPYING.   If not, write  to the
   Free Software Foundation, Inc., 59  Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.
     
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>	/* sometimes useful for debugging */
#include "ucl.h"

/*
  "stub"  is used  to  declare  the timestamp  for  stub functions;  the
  timestamp establishes the order in  which the functions will appear in
  the stub tables: do not change them.
*/
#define stub(TIMESTAMP)		/* empty */

/*
  "attibute" is used to  declare GCC function declaration attributes; it
  is  read by the  headers building  script and  added to  the prototype
  declaration.  To know  more about  attributes read  the GCC  info node
  "Function Attributes".
*/
#define attribute(TIMESTAMP)	/* empty */

/* end of file */
