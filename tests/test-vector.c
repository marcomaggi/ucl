/*
  Part of: Useless Containers Library
  Contents: test for vector
  Date: Wed Dec 10, 2003

  Abstract

	Initialisation, allocation, inspection.

  Copyright (c) 2003-2005, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#define MCL_DEBUGGING		0
#include "mcl-test.h"
#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "ucl.h"

#define NUMBER	1000
#define DELTA	234


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static void
fill (ucl_vector_t vect, int number, int first)
{
  int *	ptr;
  for (int i=0; i < number; ++i) {
    ucl_vector_enlarge(vect);
    ptr = ucl_vector_index_to_new_slot(vect, ucl_vector_size(vect));
    ptr = ucl_vector_insert(vect, ptr);
    *ptr = i + first;
  }
}
static void
clean (ucl_vector_t vect)
{
  while(ucl_vector_size(vect)) {
    int * ptr = ucl_vector_index_to_slot(vect, 0);
    ucl_vector_erase(vect, ptr);
    ucl_vector_restrict(vect);
  }
}


static void
test_allocation (void)
{
  static const unsigned SIZE		= 8;
  static const unsigned STEP_UP		= 6;
  static const unsigned STEP_DOWN	= 10;
  static const unsigned PAD		= 0;

  mcl_test_begin("vector-1.1", "allocation") {
    ucl_vector_config_t	config;
    ucl_vector_t	vector;
    ucl_block_t		allocated_memory;
    size_t		slot_dimension  = sizeof(int);
    size_t		number_of_slots = SIZE;

    ucl_vector_initialise_config(config, slot_dimension, number_of_slots);
    config->step_up			= STEP_UP;
    config->step_down			= STEP_DOWN;
    config->size_of_padding_area	= PAD;
    ucl_vector_alloc(vector, config);
    {
      allocated_memory = ucl_vector_get_memory_block(vector);
      assert(allocated_memory.ptr);
      assert(ucl_vector_number_of_free_slots(vector)		== SIZE);
      assert(ucl_vector_number_of_step_up_slots(vector)		== STEP_UP);
      assert(ucl_vector_number_of_step_down_slots(vector)	== STEP_DOWN);
      assert(ucl_vector_number_of_padding_slots(vector)		== PAD);
      assert(ucl_vector_slot_dimension(vector)			== slot_dimension);
    }
    ucl_vector_free(vector);
  }
  mcl_test_end();
}

static void
doit (size_t number_of_slots, size_t step_up, size_t step_down, size_t pad)
{
  ucl_vector_config_t	config;
  ucl_vector_t		vector;
  size_t		slot_dimension = sizeof(int);
  ucl_vector_initialise_config(config, slot_dimension, number_of_slots);
  config->step_up		= step_up;
  config->step_down		= step_down;
  config->size_of_padding_area	= pad;
  ucl_vector_alloc(vector, config);
  {
    fill(vector, NUMBER, 0); clean(vector);
    fill(vector, NUMBER, 0); clean(vector);
    fill(vector, NUMBER, 0); clean(vector);
  }
  ucl_vector_free(vector);
}
static void
test_construction_and_destruction (void)
{
  mcl_test_begin("vector-1.2", "construction and destruction") {
    static const int	sizes[]		= { 1, 2, 3, 4, 5, 100, 1000, 0 };
    static const int	step_ups[]	= { 1, 2, 3, 4, 5, 16, 0 };
    static const int	step_downs[]	= { 2, 3, 4, 5, 6, 32, 0 };
    for (int i=0; sizes[i]; ++i) {
      for (int j=0; step_ups[j]; ++j) {
	for (int k=0; step_downs[k]; ++k) {
	  doit(sizes[i], step_ups[j], step_downs[k], 0);
	}
      }
    }
    {
      static const unsigned SIZE	= 100;
      static const unsigned STEP_UP	=  10;
      static const unsigned STEP_DOWN	=  20;
      static const unsigned PAD		=  50;
      doit(SIZE, STEP_UP, STEP_DOWN, PAD);
    }
  }
  mcl_test_end();
}

int
main (void)
{
  mcl_test_title("vector tests");

  mcl_test_subtitle("allocation, construction and destruction");
  test_allocation();
  test_construction_and_destruction();

  exit(EXIT_SUCCESS);
}

/* end of file */
