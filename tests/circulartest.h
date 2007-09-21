/* circulartest.h --
   
   Part of: Useless Containers Library
   Contents: main header file for circular list test
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
   
   $Id: circulartest.h,v 1.1.1.1 2003/12/10 15:22:06 marco Exp $
*/


#ifndef __CIRCULARTEST_H
#define __CIRCULARTEST_H 1

#include <stdio.h>
#include <assert.h>
#include "ucl.h"
	
#define NUMBER	20

UCL_BEGIN_C_DECL

extern void test UCL_ARGS((void));
extern void fill UCL_ARGS((ucl_circular_t *circPtr, int number, int first));
extern ucl_circular_link_t *alloc_link UCL_ARGS((void));
extern void clean_circular UCL_ARGS((ucl_circular_t *this));

UCL_END_C_DECL

#endif /* __CIRCULARTEST_H */


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
