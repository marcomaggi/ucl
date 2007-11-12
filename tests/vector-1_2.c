/* vector-1_2.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Simple construction destruction test.
   
   Copyright (c) 2003, 2004, 2005, 2006 Marco Maggi
   
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

#define UCL_DEBUGGING		0
#include "vectortest.h"

static void	doit		(ucl_vector_t vector, size_t size,
				 size_t step_up, size_t step_down,
				 size_t pad);

/* ------------------------------------------------------------ */

void
test (void)
{
  ucl_vector_t		vector;
  static const int	sizes[]		= { 1, 2, 3, 4, 5, 100, 1000, 0 };
  static const int	step_ups[]	= { 1, 2, 3, 4, 5, 16, 0 };
  static const int	step_downs[]	= { 2, 3, 4, 5, 6, 32, 0 };
  int			i, j, k;


  for (i=0; sizes[i]; ++i)
    {
      for (j=0; step_ups[j]; ++j)
	{
	  for (k=0; step_downs[k]; ++k)
	    {
	      doit(vector, sizes[i], step_ups[j], step_downs[k], 0);
	    }
	}
    }

#define SIZE		100
#define STEP_UP		10
#define STEP_DOWN	20
#define PAD		50
  doit(vector, SIZE, STEP_UP, STEP_DOWN, PAD);
}

/* ------------------------------------------------------------ */

void
doit (ucl_vector_t vector, size_t size, size_t step_up, size_t step_down, size_t pad)
{
  ucl_debug("size: %d, up: %d, down: %d, pad: %d", size, step_up, step_down, pad);

  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, size);
  ucl_vector_initialise_step_up(vector, step_up);
  ucl_vector_initialise_step_down(vector, step_down);
  ucl_vector_initialise_pad(vector, pad);
  ucl_vector_constructor(vector);
  
  fill(vector, NUMBER, 0); clean(vector);
  fill(vector, NUMBER, 0); clean(vector);
  fill(vector, NUMBER, 0); clean(vector);
	      
  ucl_vector_destructor(vector);
}


/* end of file */
