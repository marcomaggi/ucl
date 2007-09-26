/* btreetest.h --
   
   Part of: Useless Containers Library
   Contents: common header file for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.
*/

#ifndef __BTREETEST_H
#define __BTREETEST_H 1

#include <stdio.h>
#include "ucl.h"

UCL_BEGIN_C_DECL

#define NUMBER	1000
#define DELTA	234

extern void test (void);
extern ucl_btree_node_t * alloc_link (void);
extern void clean_btree	(ucl_btree_node_t *this);

/* ------------------------------------------------------------ */

ucl_btree_node_t *
alloc_link(void)
{
  ucl_btree_node_t *	link_p;

  link_p = (ucl_btree_node_t *) malloc(sizeof(ucl_btree_node_t));
  if (link_p == NULL)
    {
      printf("error allocating memory\n");
      exit(EXIT_FAILURE);
    }
  return link_p;
}

void
clean_btree(ucl_btree_node_t *btree_p UCL_UNUSED)
{
}

UCL_END_C_DECL

#endif /* __BTREETEST_H */

/* end of file */
