/* vector-3_1.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Elements insertion.
   
   Copyright (c) 2003, 2004, 2005  Marco Maggi
   
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
#  define UCL_DEBUGGING 0
#endif
#include "vectortest.h"

void
test (void)
{
  int			i;
  int *			ptr;
  ucl_vector_t		vector;


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_enlarge(vector);
  ptr = ucl_vector_index_to_new_slot(vector, 0);
  assert(ptr != NULL);
  ptr = ucl_vector_insert(vector, ptr);
  assert(ptr != NULL);
  *ptr = NUMBER-1;

  assert(ucl_vector_size(vector) == 1);

  ucl_vector_enlarge(vector);
  ptr = ucl_vector_index_to_new_slot(vector, 1);
  assert(ptr != NULL);
  ptr = ucl_vector_insert(vector, ptr);
  assert(ptr != NULL);
  *ptr = 1;

  assert(ucl_vector_size(vector) == 2);

  ucl_vector_enlarge(vector);
  ptr = ucl_vector_index_to_new_slot(vector, 2);
  assert(ptr != NULL);
  ptr = ucl_vector_insert(vector, ptr);
  assert(ptr != NULL);
  *ptr = 0;

  assert(ucl_vector_size(vector) == 3);

  for (i=3; i < NUMBER; ++i)
    {
      ucl_vector_enlarge(vector);
      
      ptr = ucl_vector_index_to_new_slot(vector, 1);
      assert(ptr != NULL);
      ptr = ucl_vector_insert(vector, ptr);
      assert(ptr != NULL);
      *ptr = i-1;
      
      assert(ucl_vector_size(vector) == (size_t)(i+1));
    }
	    
  assert(ucl_vector_size(vector) == NUMBER);
  
  for (i=0; i < NUMBER; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, i);
      assert(ptr != NULL);
      assert(*ptr == NUMBER-1-i);
    }
  
  assert(ucl_vector_size(vector) == NUMBER);

#define BIGSIZE		(10000)

/*   ucl_debug("size %d\n", ucl_vector_size(vector)); */
  ucl_vector_enlarge_for_slots(vector, BIGSIZE);
/*   ucl_debug("size %d\n", ucl_vector_size(vector)); */
  assert(ucl_vector_size(vector) == NUMBER); /* the new slots are unused */

  for (i=NUMBER; i < BIGSIZE; ++i)
    {
      ptr = ucl_vector_index_to_new_slot(vector, i);
      assert(ptr != NULL);
      ptr = ucl_vector_insert(vector, ptr);
      assert(ptr != NULL);
      *ptr = i-1;
      
/*       ucl_debug("size now %d\n", ucl_vector_size(vector)); */
      assert(ucl_vector_size(vector) == (size_t)(i+1));
    }

  assert(ucl_vector_size(vector) == BIGSIZE);

  for (i=0; i < NUMBER; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, i);
      assert(ptr != NULL);
      assert(*ptr == NUMBER-1-i);
    }
  for (i=NUMBER; i < BIGSIZE; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, i);
      assert(ptr != NULL);
      assert(*ptr == i-1);
    }


  ucl_vector_destructor(vector);
}

/* end of file */
