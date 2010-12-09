/*
  Part of: Useless Container Library
  Contents: prints interesting sizes on stdout
  Date: Tue Feb 11, 2003

  Abstract



  Copyright (c) 2003, 2005-2006, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this  program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "ucl.h"

int
main (void)
{
  printf("\nSize of common data types:\n\n");

  printf("char\t\t%d :)\n",	sizeof(char));
  printf("void *\t\t%d\n",	sizeof(void *));
  printf("int\t\t%d\n",		sizeof(int));
  printf("short int\t%d\n",	sizeof(short int));
  printf("long int\t%d\n",	sizeof(long));
  printf("long long\t%d\n",	sizeof(long long));
  printf("float\t\t%d\n",	sizeof(float));
  printf("double\t\t%d\n",	sizeof(double));
#if 1
  printf("long double\t%d\n",	sizeof(long double));
#endif
  printf("size_t\t\t%d\n",	sizeof(size_t));
  printf("ptrdiff_t\t%d\n",	sizeof(ptrdiff_t));
  printf("ucl_value_t\t%d\n",		sizeof(ucl_value_t));

  printf("\nLimits:\n\n");

  printf("short int (SHRT_MIN, SHRT_MAX)\t%d %d\n", SHRT_MIN, SHRT_MAX);
  printf("integer (INT_MIN, INT_MAX)\t%d %d\n", INT_MIN, INT_MAX);
  printf("unsigned short int (USHRT_MAX)\t%d\n", USHRT_MAX);
  printf("unsigned (UINT_MAX)\t\t%u\n", UINT_MAX);

  printf("\nSize of UCL structures:\n\n");

  printf("ucl_circular_t       %d\n",	sizeof(ucl_circular_t));
  printf("ucl_hash_table_t     %d\n",	sizeof(ucl_hash_table_t));
  printf("ucl_heap_t           %d\n",	sizeof(ucl_heap_t));
  printf("ucl_iterator_t       %d\n",	sizeof(ucl_iterator_t));
  printf("ucl_map_t            %d\n",	sizeof(ucl_map_t));
  printf("ucl_vector_t         %d\n",	sizeof(ucl_vector_t));

  printf("\nSize of UCL link structures:\n\n");

  printf("ucl_node_tag_t       %d\n", sizeof(ucl_node_tag_t));
  printf("ucl_graph_node_tag_t %d\n", sizeof(ucl_graph_node_tag_t));
  printf("ucl_graph_link_tag_t %d\n", sizeof(ucl_graph_link_tag_t));

  exit(EXIT_SUCCESS);
}


/* end of file */
