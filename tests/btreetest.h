/* btreetest.h --
   
   Part of: Useless Containers Library
   Contents: common header file for the binary tree container
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
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

#ifndef __BTREETEST_H
#define __BTREETEST_H 1


/** ------------------------------------------------------------
 ** Headers.#include <stdio.h>
 ** ----------------------------------------------------------*/

#include "testmain.h"

#define NUMBER	1000
#define DELTA	234

ucl_btree_node_t * alloc_link (void);
void clean_btree (ucl_btree_node_t * btree_p UCL_UNUSED);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

ucl_btree_node_t *
alloc_link (void)
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
clean_btree (ucl_btree_node_t * btree_p UCL_UNUSED)
{
}

#endif /* __BTREETEST_H */

/* end of file */
