/* maptest.h --
   
   Part of: Useless Container Library
   Contents: header file for Map tests
   Date: Sat Feb  8, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This  is free  software; you  can redistribute  it and/or  modify it
   under  the  terms  of  the  GNU Lesser  General  Public  License  as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.
   
   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.
   
   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA
   
   $Id: maptest.h,v 1.1.1.6 2003/12/11 09:58:24 marco Exp $
*/


#ifndef __MAPTEST_H
#define __MAPTEST_H 1

#include <stdio.h>
#include <assert.h>
#include "ucl.h"
	
#define NUMBER		50
#define LITTLENUMBER	10

UCL_BEGIN_C_DECL

extern void test UCL_ARGS((void));
extern ucl_map_link_t * alloc_new_link UCL_ARGS((void));
extern void clean_map UCL_ARGS((ucl_map_t *mapPtr));
extern void fill_map UCL_ARGS((ucl_map_t *mapPtr, int begin, int end));

UCL_END_C_DECL

#endif /* __MAPTEST_H */


/* end of file */
