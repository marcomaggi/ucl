/* btreetest.c --
   
   Part of: Useless Containers Library
   Contents: main test file for the binary tree container
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
   
   $Id: btreetest.c,v 1.1.1.2 2003/12/10 13:44:14 marco Exp $
*/

#include "btreetest.h"
	
int
main (int argc, const char *const argv[])
{
  printf("%s: start\n", argv[0]);
  test();
  printf("%s: end\n", argv[0]);
  exit(EXIT_SUCCESS);
}

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
clean_btree(ucl_btree_node_t *btree_p)
{
}

/* end of file */
/*
Local Variables:
mode: c
page-delimiter: "^$"
End:
*/
