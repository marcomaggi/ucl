/*   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Wed Dec 10, 2003
   
   Abstract
   
	Insert sort.
   
   Copyright (c) 2003, 2004, 2005, 2008 Marco Maggi
   
   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.
   
   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/   

#define DEBUGGING		0
#include "vectortest.h"

void
test (void)
{
  int			i, j;
  int *			ptr;
  ucl_vector_t		vector;
  ucl_comparison_t	compar = { { .ptr = NULL}, ucl_ptrintcmp };


  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);


  for (i=0; i<NUMBER; ++i)
    {
      ucl_vector_enlarge(vector);
      ptr = ucl_vector_insert_sort(vector, &i);
      assert(ptr != NULL);
/*       fprintf(stderr, "insert %d size %d\n", i, ucl_vector_size(vector)); */
      *ptr = i;

      for (j=0; j<i; ++j)
	{
	  ptr = ucl_vector_index_to_slot(vector, j);
	  assert(ptr != NULL);
	  assert(*ptr == j);
	}
    }

  for (i=0; i<NUMBER; ++i)
    {
      ptr = ucl_vector_index_to_slot(vector, i);
      assert(ptr != NULL);
      assert(*ptr == i);
    }
  
  ucl_vector_destructor(vector);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);

  i = 4;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 8;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 6;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  assert(3 == ucl_vector_size(vector));

  debug("checking");

  ptr = ucl_vector_index_to_slot(vector, 0);
  assert(ptr != NULL);
  assert(*ptr == 4);

  ptr = ucl_vector_index_to_slot(vector, 1);
  assert(ptr != NULL);
  assert(*ptr == 6);

  ptr = ucl_vector_index_to_slot(vector, 2);
  assert(ptr != NULL);
  assert(*ptr == 8);

  ucl_vector_destructor(vector);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);

  i = 4;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 6;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 8;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  assert(3 == ucl_vector_size(vector));

  debug("checking");

  ptr = ucl_vector_index_to_slot(vector, 0);
  assert(ptr != NULL);
  assert(*ptr == 4);

  ptr = ucl_vector_index_to_slot(vector, 1);
  assert(ptr != NULL);
  assert(*ptr == 6);

  ptr = ucl_vector_index_to_slot(vector, 2);
  assert(ptr != NULL);
  assert(*ptr == 8);

  ucl_vector_destructor(vector);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);

  i = 6;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 4;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 8;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  assert(3 == ucl_vector_size(vector));

  debug("checking");

  ptr = ucl_vector_index_to_slot(vector, 0);
  assert(ptr != NULL);
  assert(*ptr == 4);

  ptr = ucl_vector_index_to_slot(vector, 1);
  assert(ptr != NULL);
  assert(*ptr == 6);

  ptr = ucl_vector_index_to_slot(vector, 2);
  assert(ptr != NULL);
  assert(*ptr == 8);

  ucl_vector_destructor(vector);

  /* ------------------------------------------------------------ */

  ucl_vector_initialise(vector, sizeof(int));
  ucl_vector_initialise_size(vector, 8);
  ucl_vector_initialise_step_up(vector, 6);
  ucl_vector_initialise_step_down(vector, 10);
  ucl_vector_initialise_pad(vector, 0);
  
  ucl_vector_constructor(vector);

  ucl_vector_set_compar(vector, compar);

  i = 8;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 6;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  i = 4;
  debug("inserting %d", i);
  ptr = ucl_vector_insert_sort(vector, &i);
  *ptr = i;

  assert(3 == ucl_vector_size(vector));

  debug("checking");

  ptr = ucl_vector_index_to_slot(vector, 0);
  assert(ptr != NULL);
  assert(*ptr == 4);

  ptr = ucl_vector_index_to_slot(vector, 1);
  assert(ptr != NULL);
  assert(*ptr == 6);

  ptr = ucl_vector_index_to_slot(vector, 2);
  assert(ptr != NULL);
  assert(*ptr == 8);

  ucl_vector_destructor(vector);

}

/* end of file */
