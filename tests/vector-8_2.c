/* vector-8_2.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Thu Apr 14, 2005
   
   Abstract
   
	Tests for "ucl_vector_append_more()".
   
   Copyright (c) 2005, 2006 Marco Maggi
   
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

#if 0
#  define UCL_DEBUGGING
#endif
#include "vectortest.h"

#define VECTOR_SIZE		10

void
test (void)
{
  ucl_vector_t	sources[4];
  ucl_vector_t	target;
  int		starts[4] = {
    VECTOR_SIZE, 2*VECTOR_SIZE, 3*VECTOR_SIZE, 4*VECTOR_SIZE
  };
  int		i;
  int *		p;


  for (i=0; i<4; ++i)
    {
      ucl_vector_initialise(sources[i], sizeof(int));
      ucl_vector_constructor(sources[i]);
      fill(sources[i], VECTOR_SIZE, starts[i]);
    }

  ucl_vector_initialise(target, sizeof(int));
  ucl_vector_constructor(target);
  fill(target, VECTOR_SIZE, 0);

  ucl_vector_append_more(target, sources[0], sources[1], sources[2], sources[3], NULL);

  ucl_debug("size %d", ucl_vector_size(target));
  assert(ucl_vector_size(target) == 50);

  for (i=0; i<50; ++i)
    {
      p = ucl_vector_index_to_slot(target, i);
      ucl_debug("%d %d", *p, i);
      assert(*p == i);
    }

  for (i=0; i<4; ++i)
    {
      ucl_vector_destructor(sources[i]);
    }
  ucl_vector_destructor(target);
}

/* end of file */
