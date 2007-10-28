/*
   Part of: Useless Containers Library
   Contents: tests for vector
   Date: Sun Oct 28, 2007
   
   Abstract
   
   
   
   Copyright (c) 2007 Marco Maggi
   
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


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#define UCL_DEBUGGING		0
#include "vectortest.h"

void
test (void)
{
  ucl_vector_t	V;
  int		numbers[5];
  ucl_block_t	block = {
    .len = sizeof(int)*5,
    .ptr = (void *)numbers
  };
  int		i, j;


  /* ------------------------------------------------------------ */

  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_constructor(V);

  fill(V, 10, 1);
  
  ucl_vector_get_block(block, 3, V);

  for (i=0, j=4; i<5; ++i, ++j)
    assert(j == numbers[i]);

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_constructor(V);

  fill(V, 10, 1);
  
  ucl_vector_get_block(block, 0, V);

  for (i=0, j=1; i<5; ++i, ++j)
    assert(j == numbers[i]);

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_constructor(V);

  fill(V, 10, 1);
  
  ucl_vector_get_block(block, 5, V);

  for (i=0, j=6; i<5; ++i, ++j)
    assert(j == numbers[i]);

  ucl_vector_destructor(V);
  
}


/* end of file */
