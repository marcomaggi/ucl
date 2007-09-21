/* showsize.c --
   
   Part of: Useless Container Library
   Contents: prints interesting sizes on stdout
   Date: Tue Feb 11, 2003
   
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
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

#include "ucl.h"


int
main (int argc, const char *const argv[])
{
  printf("\nSize of common data types:\n\n");

  printf("char\t\t%d :)\n",	sizeof(char));
  printf("void *\t\t%d\n",	sizeof(void *));
  printf("int\t\t%d\n",		sizeof(int));
  printf("short int\t%d\n",	sizeof(short int));
  printf("long\t\t%d\n",	sizeof(long));
  printf("long long\t%d\n",	sizeof(long long));
  printf("float\t\t%d\n",	sizeof(float));
  printf("double\t\t%d\n",	sizeof(double));
  printf("size_t\t\t%d\n",	sizeof(size_t));
  printf("ptrdiff_t\t%d\n",	sizeof(ptrdiff_t));

  printf("\nLimits\n\n");

  printf("short int (SHRT_MIN, SHRT_MAX)\t%d %d\n", SHRT_MIN, SHRT_MAX);
  printf("integer (INT_MIN, INT_MAX)\t%d %d\n", INT_MIN, INT_MAX);
  printf("unsigned short int (USHRT_MAX)\t%d\n", USHRT_MAX);
  printf("unsigned (UINT_MAX)\t\t%u\n", UINT_MAX);

  printf("\nSize of UCL structures\n\n");

  printf("ucl_circular_t\t%d\n", sizeof(ucl_circular_t));
  printf("ucl_hash_t\t%d\n",	 sizeof(ucl_hash_t));
  printf("ucl_iterator_t\t%d\n", sizeof(ucl_iterator_t));
  printf("ucl_list_t\t%d\n",	 sizeof(ucl_list_t));
  printf("ucl_map_t\t%d\n",	 sizeof(ucl_map_t));
  printf("ucl_strtok_t\t%d\n",	 sizeof(ucl_strtok_t));
  printf("ucl_vector_t\t%d\n",	 sizeof(ucl_vector_t));

  printf("\nSize of UCL link structures\n\n");

  printf("ucl_btree_node_t\t%d\n", sizeof(ucl_btree_node_t));
/*   printf("ucl_graph_node_t\t%d\n", sizeof(ucl_graph_node_t)); */
/*   printf("ucl_graph_link_t\t%d\n", sizeof(ucl_graph_link_t)); */
  printf("ucl_hash_entry_t\t%d\n", sizeof(ucl_hash_entry_t));
  printf("ucl_list_link_t\t\t%d\n",  sizeof(ucl_list_link_t));
  printf("ucl_map_link_t\t\t%d\n",   sizeof(ucl_map_link_t));
  printf("ucl_tree_node_t\t\t%d\n",  sizeof(ucl_tree_node_t));

  exit(EXIT_SUCCESS);
}


/* end of file */
