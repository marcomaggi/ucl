/* btreetest.h --
   
   Part of: Useless Containers Library
   Contents: common header file for the binary tree container
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
   
   $Id: btreetest.h,v 1.1.1.1 2003/12/10 12:42:21 marco Exp $
*/


#ifndef __BTREETEST_H
#define __BTREETEST_H 1

#include <stdio.h>
#include <assert.h>
#include "ucl.h"

UCL_BEGIN_C_DECL

#define NUMBER	1000
#define DELTA	234


extern void test UCL_ARGS((void));
extern ucl_btree_node_t * alloc_link UCL_ARGS((void));
extern void clean_btree	UCL_ARGS((ucl_btree_node_t *this));


UCL_END_C_DECL

#endif /* __BTREETEST_H */


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
