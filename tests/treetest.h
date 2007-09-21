/* treetest.h --
   
   Part of: Useless Containers Library
   Contents: header file for tree tests
   Date: Thu Dec 11, 2003
   
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
   
   $Id: treetest.h,v 1.1.1.1 2003/12/11 08:32:38 marco Exp $
*/


#ifndef __TREETEST_H
#define __TREETEST_H 1

#include <stdio.h>
#include <assert.h>

#include "ucl.h"
	
#define NUMBER	1000
#define DELTA	234

UCL_BEGIN_C_DECL

extern void test UCL_ARGS((void));
extern ucl_tree_node_t * alloc_link UCL_ARGS((void));
extern void fill_tree UCL_ARGS((ucl_tree_node_t **n));
extern void clean_tree UCL_ARGS((ucl_tree_node_t **n));
extern int find_node UCL_ARGS((ucl_tree_node_t **n, ucl_tree_node_t *node));
extern void assert_node UCL_ARGS((ucl_tree_node_t *dad, ucl_tree_node_t *nod,
				  ucl_tree_node_t *son, ucl_tree_node_t *nxt,
				  ucl_tree_node_t *fst, ucl_tree_node_t *lst));
        


UCL_END_C_DECL

#endif /* __TREETEST_H */


/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
