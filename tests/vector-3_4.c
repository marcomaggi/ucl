/* sorted predicate --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Fri Oct  6, 2006
   
   Abstract
   
	
   
   Copyright (c) 2006 Marco Maggi
   
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

#define UCL_DEBUGGING
#include "vectortest.h"
#define SIZE		966
#define STEP_UP		884
#define STEP_DOWN	834
#define PAD		74

#undef NUMBER
#define NUMBER		10000
#undef HALF_NUMBER
#define HALF_NUMBER	(NUMBER/2)

void
test (void)
{
  ucl_vector_t	vector;
  int *		p;
  size_t	i;
  ucl_valcmp_t	compar = { NULL, ucl_ptrintcmp };


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector,		SIZE);
  ucl_vector_initialise_step_up(vector,		STEP_UP);
  ucl_vector_initialise_step_down(vector,	STEP_DOWN);
  ucl_vector_initialise_pad(vector,		PAD);
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);


  /* Push numbers [(NUMBER-1), 0]. */
  for (i=0; i<NUMBER; ++i)
    {
      p = ucl_vector_push_back(vector);
      *p = i;
    }

  /* Assert numbers [(NUMBER-1), 0]. */
  for (i=0; i<NUMBER; ++i)
    {
      p = ucl_vector_index_to_slot(vector, i);
      assert((int)i == *p);
    }

  assert(ucl_vector_sorted(vector));

  p = ucl_vector_index_to_slot(vector, 100);
  *p = 90;
  assert(0 == ucl_vector_sorted(vector));

  ucl_vector_destructor(vector);
}

/* end of file */
