/* vector-3_2.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Back element.
   
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
  unsigned			i, j;
  unsigned *			ptr;
  ucl_vector_t		vector;


  ucl_vector_initialise(vector, sizeof(unsigned));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_constructor(vector);

  for (i=0; i < NUMBER; ++i)
    {
      ucl_vector_enlarge(vector);

      ucl_debug("pre - i=%d, size %d, last index %d", i,
		ucl_vector_size(vector), ucl_vector_last_index(vector));
      ptr = ucl_vector_index_to_new_slot(vector, ucl_vector_size(vector));
      assert(ptr != NULL);

      ptr = ucl_vector_insert(vector, ptr);
      *ptr = i;

      for (j=0; j<=i; ++j)
	{
	  ucl_debug("idx %d val %d ptr %p %p",
		    j, *(unsigned *)ucl_vector_index(vector,j),
		    ucl_vector_index_to_slot(vector,j), ptr);
	}

      ucl_debug("size %d, last index %d, slot %p back %p value %d back value %d",
		ucl_vector_size(vector), ucl_vector_last_index(vector),
		ptr, ucl_vector_back(vector), *ptr, *(unsigned *)ucl_vector_back(vector));
      assert(ptr == ucl_vector_back(vector));
      assert(ptr != NULL);
     
      assert(ucl_vector_size(vector) == i+1);
      
      ptr = ucl_vector_back(vector);
      assert(ptr != NULL);
      assert(*ptr == i);
    }
  assert( ucl_vector_size(vector) == NUMBER );
  
  for (i=0; i < NUMBER; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, i);
      assert(ptr != NULL);
      assert(*ptr == i);
    }
  
  ucl_vector_destructor(vector);
}

/* end of file */
