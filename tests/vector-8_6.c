/* vector-8_6.c --
   
   Part of: Useless Containers Library
   Contents: test for vector
   Date: Fri Oct 26, 2007
   
   Abstract
   
	Block insertion.
   
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

#define UCL_DEBUGGING		0
#include "vectortest.h"

#define NUMBER_OF_NUMBERS	5
#define SIZE_OF_VECTOR		10

static int
vector_ref (ucl_vector_t V, ucl_vector_index_t position)
{
  return *((int *) ucl_vector_index_to_slot(V, position));
}
static void
initialise (ucl_vector_t V)
{
  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_initialise_size(V, 15);
  ucl_vector_initialise_pad(V, 5);
  ucl_vector_constructor(V);

  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (int i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
}
static void
initialise2 (ucl_vector_t V)
{
  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_initialise_size(V, 15);
  ucl_vector_initialise_pad(V, 2);
  ucl_vector_constructor(V);

  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (int i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
}
static void
initialise3 (ucl_vector_t V)
{
  ucl_vector_initialise(V, sizeof(int));
  ucl_vector_initialise_size(V, 15);
  ucl_vector_initialise_pad(V, 5);
  ucl_vector_constructor(V);

  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (int i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
}

void
test (void)
{
  int		numbers[NUMBER_OF_NUMBERS] = { 11, 12, 13, 14, 15 };
  ucl_block_t	block = {
    .ptr = (void *)numbers,
    .len = sizeof(int) * NUMBER_OF_NUMBERS
  };
  ucl_vector_t	V;
  int		i, j;


  /* ------------------------------------------------------------ */

  initialise(V);

  ucl_debug("inserting block at beginning ");
  ucl_vector_insert_block(V, 0, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<NUMBER_OF_NUMBERS; ++i)
    {
      assert(numbers[i] == vector_ref(V, i));
    }
  for (i=NUMBER_OF_NUMBERS, j=0; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      assert(j == vector_ref(V, i));
    }

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise(V);

  ucl_debug("inserting block in the middle");
  ucl_vector_insert_block(V, 7, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<7; ++i)
    {
      ucl_debug("elm %d", vector_ref(V, i));
      assert(i == vector_ref(V, i));
    }
  for (i=7, j=0; i<(7+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      ucl_debug("+elm %d", vector_ref(V, i));
      assert(numbers[j] == vector_ref(V, i));
    }
  for (i=7+NUMBER_OF_NUMBERS, j=7; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      ucl_debug("++elm %d", vector_ref(V, i));
      assert(j == vector_ref(V, i));
    }

  ucl_debug("finished");

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise(V);

  ucl_debug("inserting block at end ");
  ucl_vector_insert_block(V, SIZE_OF_VECTOR, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
  for (i=SIZE_OF_VECTOR, j=0; i<NUMBER_OF_NUMBERS; ++i, ++j)
    {
      assert(numbers[j] == vector_ref(V, i));
    }

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise2(V);

  ucl_debug("inserting block at beginning ");
  ucl_vector_insert_block(V, 0, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<NUMBER_OF_NUMBERS; ++i)
    {
      assert(numbers[i] == vector_ref(V, i));
    }
  for (i=NUMBER_OF_NUMBERS, j=0; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      assert(j == vector_ref(V, i));
    }

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise2(V);

  ucl_debug("inserting block in the middle");
  ucl_vector_insert_block(V, 7, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<7; ++i)
    {
      ucl_debug("elm %d", vector_ref(V, i));
      assert(i == vector_ref(V, i));
    }
  for (i=7, j=0; i<(7+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      ucl_debug("+elm %d", vector_ref(V, i));
      assert(numbers[j] == vector_ref(V, i));
    }
  for (i=7+NUMBER_OF_NUMBERS, j=7; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      ucl_debug("++elm %d", vector_ref(V, i));
      assert(j == vector_ref(V, i));
    }

  ucl_debug("finished");

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise2(V);

  ucl_debug("inserting block at end ");
  ucl_vector_insert_block(V, SIZE_OF_VECTOR, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
  for (i=SIZE_OF_VECTOR, j=0; i<NUMBER_OF_NUMBERS; ++i, ++j)
    {
      assert(numbers[j] == vector_ref(V, i));
    }

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise3(V);

  ucl_debug("inserting block at beginning ");
  ucl_vector_insert_block(V, 0, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<NUMBER_OF_NUMBERS; ++i)
    {
      assert(numbers[i] == vector_ref(V, i));
    }
  for (i=NUMBER_OF_NUMBERS, j=0; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      assert(j == vector_ref(V, i));
    }

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise3(V);

  ucl_debug("inserting block in the middle");
  ucl_vector_insert_block(V, 7, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<7; ++i)
    {
      ucl_debug("elm %d", vector_ref(V, i));
      assert(i == vector_ref(V, i));
    }
  for (i=7, j=0; i<(7+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      ucl_debug("+elm %d", vector_ref(V, i));
      assert(numbers[j] == vector_ref(V, i));
    }
  for (i=7+NUMBER_OF_NUMBERS, j=7; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j)
    {
      ucl_debug("++elm %d", vector_ref(V, i));
      assert(j == vector_ref(V, i));
    }

  ucl_debug("finished");

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

  initialise3(V);

  ucl_debug("inserting block at end ");
  ucl_vector_insert_block(V, SIZE_OF_VECTOR, block);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
  ucl_debug("inserted");

  for (i=0; i<SIZE_OF_VECTOR; ++i)
    {
      assert(i == vector_ref(V, i));
    }
  for (i=SIZE_OF_VECTOR, j=0; i<NUMBER_OF_NUMBERS; ++i, ++j)
    {
      assert(numbers[j] == vector_ref(V, i));
    }

  ucl_vector_destructor(V);

  /* ------------------------------------------------------------ */

}

/* end of file */
