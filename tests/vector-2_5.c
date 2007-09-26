/* vector-2_5.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Front element.
   
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

#include "vectortest.h"

void
test (void)
{
  unsigned		i;
  unsigned *		ptr;
  ucl_vector_t		vector;


  ucl_vector_initialise(vector, sizeof(unsigned));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  fill(vector, NUMBER, DELTA);

  for (i=0; i<NUMBER; ++i)
    {
      ptr = ucl_vector_front(vector);
      assert( *ptr == i + DELTA );

      ucl_vector_erase(vector, ptr);

      ucl_vector_restrict(vector);
      assert(ucl_vector_size(vector) == (NUMBER-1)-i);
    }
  
  ucl_vector_destructor(vector);
}

/* end of file */
