/* hashtest.h --
   
   Part of: Useless Containers Library
   Contents: header file for hash table tests
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: hashtest.h,v 1.1.1.2 2003/12/10 15:42:24 marco Exp $
*/


#ifndef __HASHTEST_H
#define __HASHTEST_H 1

#include <stdio.h>
#include <assert.h>

#include "ucl.h"
	
#define BUCKETS	100
#define NUMBER 1000
#define DELTA	234

UCL_BEGIN_C_DECL

extern void test UCL_ARGS((void));
extern ucl_hash_entry_t * alloc_link UCL_ARGS((void));
extern size_t hash_num UCL_ARGS((ucl_value_t val));

UCL_END_C_DECL

#endif /* __HASHTEST_H */


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
