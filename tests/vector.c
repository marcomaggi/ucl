/*
  Part of: Useless Containers Library
  Contents: test for vector
  Date: Wed Dec 10, 2003

  Abstract

	Initialisation, allocation, inspection.

  Copyright (c) 2003-2005, 2010, 2013, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#define MCL_DEBUGGING		1
#include "ucl.h"
#include "ucl-debug.h"
#include "mcl-test.h"


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
  const size_t SIZE		= 8;
  const size_t STEP_UP		= 6;
  const size_t STEP_DOWN	= 10;
  const size_t PAD		= 0;
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
  const size_t NUMBER	= 1000;
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
    static const size_t	sizes[]		= { 1, 2, 3, 4, 5, 100, 1000, 0 };
    static const size_t	step_ups[]	= { 1, 2, 3, 4, 5, 16, 0 };
    static const size_t	step_downs[]	= { 2, 3, 4, 5, 6, 32, 0 };
    for (int i=0; sizes[i]; ++i) {
      for (int j=0; step_ups[j]; ++j) {
	for (int k=0; step_downs[k]; ++k) {
	  doit(sizes[i], step_ups[j], step_downs[k], 0);
	}
      }
    }
    {
      const size_t SIZE		= 100;
      const size_t STEP_UP	=  10;
      const size_t STEP_DOWN	=  20;
      const size_t PAD		=  50;
      doit(SIZE, STEP_UP, STEP_DOWN, PAD);
    }
  }
  mcl_test_end();
}

static void
test_reallocation (void)
{
  /* the assertions below depends on the fact that size==step_up */
  const size_t SIZE		= 8;
  const size_t STEP_UP		= 8;
  const size_t STEP_DOWN	= 10;
  const size_t DIM		= sizeof(size_t);
  mcl_test_begin("vector-1.3", "reallocation") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, DIM, SIZE);
    C->step_up		= STEP_UP;
    C->step_down	= STEP_DOWN;
    ucl_vector_alloc(V, C);
    {
      size_t *	P;
      /* Fill the vector  with integers from 0 to  SIZE-1; this requires
      	 no enlargement. */
      for (size_t i=0; i<SIZE; ++i) {
      	P = ucl_vector_index_to_new_slot(V, i);
      	P = ucl_vector_insert(V, P);
      	*P = i;
      	assert(ucl_vector_number_of_free_slots(V) == SIZE-i-1);
      }
      assert(0 == ucl_vector_number_of_free_slots(V));
      /* Enlarge. */
      ucl_vector_enlarge(V);
      assert(ucl_vector_number_of_free_slots(V) == ucl_vector_number_of_step_up_slots(V));
      /* Fill the newly allocated slots. */
      for (size_t i=0; i<ucl_vector_number_of_step_up_slots(V); ++i) {
      	P = ucl_vector_index_to_new_slot(V, i);
      	P = ucl_vector_insert(V, P);
      	*P = i;
      	assert(ucl_vector_number_of_free_slots(V) == ucl_vector_number_of_step_up_slots(V)-i-1);
      }
      assert(0 == ucl_vector_number_of_free_slots(V));
      /* Enlarge again. */
      ucl_vector_enlarge(V);
      assert(ucl_vector_number_of_free_slots(V) == ucl_vector_number_of_step_up_slots(V));
      /* Fill the newly allocated slots. */
      for (size_t i=0; i<ucl_vector_number_of_step_up_slots(V); ++i) {
      	P = ucl_vector_index_to_new_slot(V, i);
      	P = ucl_vector_insert(V, P);
      	*P = i;
      	debug("number of free slots: %d", ucl_vector_number_of_free_slots(V));
      	assert(ucl_vector_number_of_free_slots(V) == ucl_vector_number_of_step_up_slots(V)-i-1);
      }
      /* Restricting */
      for (size_t i=0; i<ucl_vector_number_of_step_down_slots(V); ++i) {
      	P = ucl_vector_index_to_slot(V, 0);
      	ucl_vector_erase(V, P);
      	assert(ucl_vector_number_of_free_slots(V) == i+1);
      }
      assert(ucl_vector_number_of_free_slots(V) == STEP_DOWN);
      ucl_vector_restrict(V);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_pushing_and_popping (void)
{
  const size_t SIZE		= 966;
  const size_t STEP_UP		= 884;
  const size_t STEP_DOWN	= 834;
  const size_t PAD		= 74;
  const size_t NUMBER		= 10000;
  const size_t HALF_NUMBER	= (NUMBER/2);
  const size_t DIM		= sizeof(size_t);
  mcl_test_begin("vector-1.4", "pushing and popping") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, DIM, SIZE);
    C->step_up			= STEP_UP;
    C->step_down		= STEP_DOWN;
    C->size_of_padding_area	= PAD;
    ucl_vector_alloc(V, C);
    {
      size_t *	p;
      size_t	i;
      /* FRONT/FRONT */
      {
	/* Push numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_push_front(V);
	  *p = i;
	}
	/* Assert numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(NUMBER-i-1 == *p);
	}
	/* Pop numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  ucl_vector_pop_front(V);
	}
	assert(ucl_vector_size(V) == 0);

	/* Push numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_push_front(V);
	  *p = i;
	}
	/* Assert numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(NUMBER-i-1 == *p);
	}
	/* Pop numbers [(NUMBER-1), HALF_NUMBER]. */
	for (i=HALF_NUMBER; i<NUMBER; ++i) {
	  ucl_vector_pop_front(V);
	}
	assert(ucl_vector_size(V) == HALF_NUMBER);
	/* Assert numbers [(NUMBER-1), HALF_NUMBER]. */
	for (i=0; i<NUMBER-HALF_NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(NUMBER-HALF_NUMBER-i-1 == *p);
	}
	/* Pop numbers [(HALF_NUMBER-1), 0]. */
	for (i=0; i<NUMBER-HALF_NUMBER; ++i) {
	  ucl_vector_pop_front(V);
	}
	assert(ucl_vector_size(V) == 0);
      }
      /* BACK/BACK */
      {
	/* Push numbers [0, NUMBER-1). */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_push_back(V);
	  *p = i;
	}
	/* Assert numbers [0, NUMBER). */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(i == *p);
	}
	/* Pop numbers [HALF_NUMBER, NUMBER). */
	for (i=0; i<HALF_NUMBER; ++i) {
	  ucl_vector_pop_back(V);
	}
	assert(ucl_vector_size(V) == HALF_NUMBER);
	/* Assert numbers [0, HALF_NUMBER). */
	for (i=0; i<HALF_NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  debug("got %d", *p);
	  assert(i == *p);
	}
	/* Pop numbers [(NUMBER-1), HALF_NUMBER+1]. */
	for (i=0; i<NUMBER; ++i) {
	  ucl_vector_pop_back(V);
	}
	assert(ucl_vector_size(V) == 0);

	/* Push numbers [0, (NUMBER-1)]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_push_back(V);
	  *p = i;
	}
	/* Assert numbers [0, (NUMBER-1)]. */
	for (i=0; i<NUMBER; ++i) {
	  assert(ucl_vector_index_is_valid(V, i));
	  p = ucl_vector_index_to_slot(V, i);
	  assert(i == *p);
	}
	/* Pop numbers [0, HALF_NUMBER]. */
	for (i=0; i<HALF_NUMBER; ++i) {
	  ucl_vector_pop_back(V);
	}
	assert(ucl_vector_size(V) == HALF_NUMBER);
	/* Assert numbers [0, HALF_NUMBER). */
	for (i=0; i<HALF_NUMBER; ++i) {
	  assert(ucl_vector_index_is_valid(V, i));
	  p = ucl_vector_index_to_slot(V, i);
	  assert(i == *p);
	}
	/* Pop numbers [HALF_NUMBER, (NUMBER-1)]. */
	for (i=HALF_NUMBER; i<NUMBER; ++i) {
	  ucl_vector_pop_back(V);
	}
	assert(ucl_vector_size(V) == 0);
      }
      /* FRONT/BACK */
      {
	/* Push numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_push_front(V);
	  *p = i;
	}
	/* Assert numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(NUMBER-i-1 == *p);
	}
	/* Pop numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  ucl_vector_pop_back(V);
	}
	assert(ucl_vector_size(V) == 0);
      }
      /* BACK/FRONT */
      {
	/* Push numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_push_back(V);
	  *p = i;
	}
	/* Assert numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(i == *p);
	}
	/* Pop numbers [(NUMBER-1), 0]. */
	for (i=0; i<NUMBER; ++i) {
	  ucl_vector_pop_front(V);
	}
	assert(ucl_vector_size(V) == 0);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_block_swallowing (void)
{
  const size_t NUMBER		= 1000;
  const size_t SIZE		= 8;
  const size_t STEP_UP		= 8;
  const size_t STEP_DOWN	= 10;
  const size_t DIM		= sizeof(size_t);
  mcl_test_begin("vector-1.5", "block swallowing") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_block_t		block;
    ucl_vector_initialise_config(C, DIM, SIZE);
    C->step_up			= STEP_UP;
    C->step_down		= STEP_DOWN;
    block = ucl_block_alloc(ucl_memory_allocator, DIM * NUMBER);
    {
      size_t *	p = (size_t *)block.ptr;
      for (size_t i=0; i<NUMBER; ++i) {
	p[i] = i;
      }
      ucl_vector_swallow_block(V, C, block);
      {
	assert(ucl_vector_number_of_free_slots(V) == 0);
	assert(ucl_vector_size(V) == NUMBER);
	for (size_t i=0; i<NUMBER; ++i) {
	  p = ucl_vector_index_to_slot(V, i);
	  assert(*p == i);
	}
      }
      ucl_vector_free(V);
    }
  }
  mcl_test_end();
}

static void
test_sub_index_functions (ucl_vector_t V, ucl_index_t idx)
{
  uint8_t *	p;
  ucl_index_t	i;

  p = ucl_vector_index_to_slot(V, idx);
  assert( V->first_used_slot+sizeof(int)*idx == p );
  i = ucl_vector_slot_to_index(V, p);
  assert( i == idx );
}
static void
test_index_functions (void)
{
  mcl_test_begin("vector-2.1", "index functions") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(int), 16);
    ucl_vector_alloc(V, C);
    {
      fill(V, 100, 0);

      test_sub_index_functions(V, 0);
      test_sub_index_functions(V, 10);
      test_sub_index_functions(V, 99);

      assert( ucl_vector_last_index(V) == 99 );
      assert( ucl_vector_pointer_is_valid_slot(V, ucl_vector_index_to_slot(V,0)) );
      assert( ucl_vector_pointer_is_valid_slot(V, ucl_vector_index_to_slot(V,10)) );
      assert( ucl_vector_pointer_is_valid_slot(V, ucl_vector_index_to_slot(V,99)) );
      assert( ! ucl_vector_pointer_is_valid_slot(V, ucl_vector_index_to_new_slot(V,100)) );

      assert( ucl_vector_index_is_valid(V, 0) );
      assert( ucl_vector_index_is_valid(V, 10) );
      assert( ucl_vector_index_is_valid(V, 99) );
      assert( ! ucl_vector_index_is_valid(V, 100) );
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_back_element (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-2.2", "back element") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    ucl_vector_alloc(V, C);
    {
      size_t *	P;
      for (ucl_index_t i=0; i < NUMBER; ++i) {
	ucl_vector_enlarge(V);
	P = ucl_vector_index_to_new_slot(V, ucl_vector_size(V));
	assert(P != NULL);
	P = ucl_vector_insert(V, P);
	*P = i;
	assert(P == ucl_vector_back(V));
	assert(P != NULL);
	assert(ucl_vector_size(V) == i+1);
	P = ucl_vector_back(V);
	assert(P != NULL);
	assert(*P == i);
      }
      assert( ucl_vector_size(V) == NUMBER );
      for (ucl_index_t i=0; i < NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(P != NULL);
	assert(*P == i);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_back_element_more (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-2.3", "more back element") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up		= 6;
    C->step_down	= 10;
    ucl_vector_alloc(V, C);
    {
      size_t *	P;
      fill(V, NUMBER, DELTA);
      for (ucl_index_t i=0; i<NUMBER; ++i) {
	P = ucl_vector_back(V);
	assert( *P == (NUMBER-1+DELTA)-i );
	ucl_vector_erase(V, P);
	ucl_vector_restrict(V);
	assert(ucl_vector_size(V) == (NUMBER-1)-i);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_front_element (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-2.4", "front element") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    ucl_vector_alloc(V, C);
    {
      unsigned *	P;
      for (ucl_index_t i=0; i < NUMBER; ++i) {
	ucl_vector_enlarge(V);
	P = ucl_vector_index_to_new_slot(V, 0);
	assert(P != NULL);
	assert(P == (unsigned *) V->first_used_slot);
	P = ucl_vector_insert(V, P);
	assert(P != NULL);
	assert(P == (unsigned *) V->first_used_slot);
	*P = i;
	assert(ucl_vector_size(V) == i+1);
	P = ucl_vector_front(V);
	assert(P != NULL);
	assert(*P == i);
      }
      assert( ucl_vector_size(V) == NUMBER );
      for (ucl_index_t i=0; i < NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(*P == (NUMBER - i - 1));
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_front_element_more (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-2.5", "more front element") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up		= 6;
    C->step_down	= 10;
    ucl_vector_alloc(V, C);
    {
      size_t *	P;
      fill(V, NUMBER, DELTA);
      for (ucl_index_t i=0; i<NUMBER; ++i) {
	P = ucl_vector_front(V);
	assert( *P == i + DELTA );
	ucl_vector_erase(V, P);
	ucl_vector_restrict(V);
	assert(ucl_vector_size(V) == (NUMBER-1)-i);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_front_and_back_elements (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-2.6", "front and back element") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up		= 6;
    C->step_down	= 10;
    ucl_vector_alloc(V, C);
    {
      int i=NUMBER-1;
      fill(V, NUMBER, DELTA);
      for (size_t * P = ucl_vector_back(V); P >= (size_t *) ucl_vector_front(V); --P) {
	assert(P != NULL);
	assert(*P == i+DELTA);
	--i;
      }
      assert(i == -1);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_elements_insertion (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-3.1", "elements insertion") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up		= 6;
    C->step_down	= 10;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i;
      size_t *		P;
      ucl_vector_enlarge(V);
      P = ucl_vector_index_to_new_slot(V, 0);
      assert(P != NULL);
      P = ucl_vector_insert(V, P);
      assert(P != NULL);
      *P = NUMBER-1;

      assert(ucl_vector_size(V) == 1);

      ucl_vector_enlarge(V);
      P = ucl_vector_index_to_new_slot(V, 1);
      assert(P != NULL);
      P = ucl_vector_insert(V, P);
      assert(P != NULL);
      *P = 1;

      assert(ucl_vector_size(V) == 2);

      ucl_vector_enlarge(V);
      P = ucl_vector_index_to_new_slot(V, 2);
      assert(P != NULL);
      P = ucl_vector_insert(V, P);
      assert(P != NULL);
      *P = 0;

      assert(ucl_vector_size(V) == 3);

      for (i=3; i < NUMBER; ++i) {
	ucl_vector_enlarge(V);

	P = ucl_vector_index_to_new_slot(V, 1);
	assert(P != NULL);
	P = ucl_vector_insert(V, P);
	assert(P != NULL);
	*P = i-1;

	assert(ucl_vector_size(V) == (size_t)(i+1));
      }

      assert(ucl_vector_size(V) == NUMBER);

      for (i=0; i < NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(P != NULL);
	assert(*P == NUMBER-1-i);
      }

      assert(ucl_vector_size(V) == NUMBER);

      const size_t BIGSIZE = 10000;
      ucl_vector_enlarge_for_slots(V, BIGSIZE);
      assert(ucl_vector_size(V) == NUMBER); /* the new slots are unused */

      for (i=NUMBER; i < BIGSIZE; ++i) {
	P = ucl_vector_index_to_new_slot(V, i);
	assert(P != NULL);
	P = ucl_vector_insert(V, P);
	assert(P != NULL);
	*P = i-1;
	assert(ucl_vector_size(V) == (size_t)(i+1));
      }

      assert(ucl_vector_size(V) == BIGSIZE);

      for (i=0; i < NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(P != NULL);
	assert(*P == NUMBER-1-i);
      }
      for (i=NUMBER; i < BIGSIZE; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(P != NULL);
	assert(*P == i-1);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_insert_sort (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-3.2", "insert sort") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_index_t		i=0, j;
    ucl_value_t		data = { .pointer = &i };
    size_t *		P;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up		= 6;
    C->step_down	= 10;
    C->compar		= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      for (i=0; i<NUMBER; ++i) {
	ucl_vector_enlarge(V);
	P = ucl_vector_insert_sort(V, data);
	assert(P != NULL);
	*P = i;
	for (j=0; j<i; ++j) {
	  P = ucl_vector_index_to_slot(V, j);
	  assert(P != NULL);
	  assert(*P == j);
	}
      }
      for (i=0; i<NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(P != NULL);
	assert(*P == i);
      }
    }
    ucl_vector_free(V);

    ucl_vector_alloc(V, C);	/* the data area is set to zero */
    {
      i = 4;
      data.pointer = &i;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 8;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 6;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      assert(3 == ucl_vector_size(V));

      P = ucl_vector_index_to_slot(V, 0);
      assert(P != NULL);
      assert(*P == 4);

      P = ucl_vector_index_to_slot(V, 1);
      assert(P != NULL);
      assert(*P == 6);

      P = ucl_vector_index_to_slot(V, 2);
      assert(P != NULL);
      assert(*P == 8);
    }
    ucl_vector_free(V);

    ucl_vector_alloc(V, C);	/* the data area is set to zero */
    {
      i = 4;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 6;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 8;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      assert(3 == ucl_vector_size(V));

      P = ucl_vector_index_to_slot(V, 0);
      assert(P != NULL);
      assert(*P == 4);

      P = ucl_vector_index_to_slot(V, 1);
      assert(P != NULL);
      assert(*P == 6);

      P = ucl_vector_index_to_slot(V, 2);
      assert(P != NULL);
      assert(*P == 8);
    }
    ucl_vector_free(V);

    ucl_vector_alloc(V, C);	/* the data area is set to zero */
    {
      i = 6;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 4;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 8;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      assert(3 == ucl_vector_size(V));

      P = ucl_vector_index_to_slot(V, 0);
      assert(P != NULL);
      assert(*P == 4);

      P = ucl_vector_index_to_slot(V, 1);
      assert(P != NULL);
      assert(*P == 6);

      P = ucl_vector_index_to_slot(V, 2);
      assert(P != NULL);
      assert(*P == 8);
    }
    ucl_vector_free(V);

    ucl_vector_alloc(V, C);	/* the data area is set to zero */
    {
      i = 8;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 6;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      i = 4;
      P = ucl_vector_insert_sort(V, data);
      *P = i;

      assert(3 == ucl_vector_size(V));

      P = ucl_vector_index_to_slot(V, 0);
      assert(P != NULL);
      assert(*P == 4);

      P = ucl_vector_index_to_slot(V, 1);
      assert(P != NULL);
      assert(*P == 6);

      P = ucl_vector_index_to_slot(V, 2);
      assert(P != NULL);
      assert(*P == 8);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_more_insert_sort (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-3.3", "more insert sort") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up		= 6;
    C->step_down	= 10;
    C->compar		= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i, j, k, l;
      ucl_value_t	data = { .pointer = &i };
      size_t *		P;
      for (j=0; j<3; ++j) {
	for (i=j; i<NUMBER; i+=3) {
	  ucl_vector_enlarge(V);
	  P = ucl_vector_insert_sort(V, data);
	  assert(P != NULL);
	  *P = i;

	  P = ucl_vector_index_to_slot(V, 0);
	  l = *P;
	  for (k=1; (size_t)k<ucl_vector_size(V); ++k) {
	    P = ucl_vector_index_to_slot(V, k);
	    assert(P != NULL);
	    assert(l < *P);
	    l = *P;
	  }
	}
      }
      for (i=0; i<NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(P != NULL);
	assert(*P == i);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_vector_insertion (void)
{
  mcl_test_begin("vector-3.4", "vector insertion") {
    ucl_vector_config_t	C;
    ucl_vector_t	SRC, DST;
    size_t *		P;
    ucl_index_t		i, j;
    ucl_vector_initialise_config(C, sizeof(size_t), 16);
    ucl_vector_alloc(SRC, C);
    ucl_vector_alloc(DST, C);
    {
      fill(DST, 10, 0);
      fill(SRC, 10, 20);
      ucl_vector_insert_vector(DST, 5, SRC);
      assert(ucl_vector_size(DST) == 20);
      for (i=0, j=0; i<5; ++i, ++j) {
	P = ucl_vector_index_to_slot(DST, i);
	assert(*P == j);
      }
      for (j=20; i<15; ++i, ++j) {
	P = ucl_vector_index_to_slot(DST, i);
	assert(*P == j);
      }
      for (j=5; i<20; ++i, ++j) {
	P = ucl_vector_index_to_slot(DST, i);
	assert(*P == j);
      }
    }
    ucl_vector_free(SRC);
    ucl_vector_free(DST);
  }
  mcl_test_end();
}

const size_t SIZE_OF_VECTOR	= 10;
static int
vector_ref (ucl_vector_t V, ucl_index_t position)
{
  return *((int *) ucl_vector_index_to_slot(V, position));
}
static void
test_block_insertion_initialise (ucl_vector_t V)
{
  ucl_vector_config_t	C;
  ucl_vector_initialise_config(C, sizeof(size_t), 15);
  C->size_of_padding_area	= 5;
  ucl_vector_alloc(V, C);
  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (ucl_index_t i=0; i<SIZE_OF_VECTOR; ++i) {
    assert(i == vector_ref(V, i));
  }
}
static void
test_block_insertion_initialise2 (ucl_vector_t V)
{
  ucl_vector_config_t	C;
  ucl_vector_initialise_config(C, sizeof(size_t), 15);
  C->size_of_padding_area	= 2;
  ucl_vector_alloc(V, C);
  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (ucl_index_t i=0; i<SIZE_OF_VECTOR; ++i) {
    assert(i == vector_ref(V, i));
  }
}
static void
test_block_insertion_initialise3 (ucl_vector_t V)
{
  ucl_vector_config_t	C;
  ucl_vector_initialise_config(C, sizeof(size_t), 15);
  C->size_of_padding_area	= 5;
  ucl_vector_alloc(V, C);
  fill(V, SIZE_OF_VECTOR, 0);
  assert(ucl_vector_size(V) == SIZE_OF_VECTOR);
  for (ucl_index_t i=0; i<SIZE_OF_VECTOR; ++i) {
    assert(i == vector_ref(V, i));
  }
}
static void
test_block_insertion (void)
{
  mcl_test_begin("vector-3.5", "block insertion") {
#undef 	NUMBER_OF_NUMBERS
#define	NUMBER_OF_NUMBERS	5
    const size_t	numbers[NUMBER_OF_NUMBERS] = { 11, 12, 13, 14, 15 };
    ucl_block_t		block = {
      .ptr = (uint8_t *)numbers,
      .len = sizeof(size_t) * NUMBER_OF_NUMBERS };
    ucl_vector_t	V;
    ucl_index_t		i, j;
    /* inserting at beginning */
    test_block_insertion_initialise(V);
    {
      ucl_vector_insert_block(V, 0, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<NUMBER_OF_NUMBERS; ++i) {
	assert(numbers[i] == vector_ref(V, i));
      }
      for (i=NUMBER_OF_NUMBERS, j=0; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(j == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block in the middle */
    test_block_insertion_initialise(V);
    {
      ucl_vector_insert_block(V, 7, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<7; ++i) {
	assert(i == vector_ref(V, i));
      }
      for (i=7, j=0; i<(7+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(numbers[j] == vector_ref(V, i));
      }
      for (i=7+NUMBER_OF_NUMBERS, j=7; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(j == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block at end */
    test_block_insertion_initialise(V);
    {
      ucl_vector_insert_block(V, SIZE_OF_VECTOR, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<SIZE_OF_VECTOR; ++i) {
	assert(i == vector_ref(V, i));
      }
      for (i=SIZE_OF_VECTOR, j=0; i<NUMBER_OF_NUMBERS; ++i, ++j) {
	assert(numbers[j] == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block at beginning */
    test_block_insertion_initialise2(V);
    {
      ucl_vector_insert_block(V, 0, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<NUMBER_OF_NUMBERS; ++i) {
	assert(numbers[i] == vector_ref(V, i));
      }
      for (i=NUMBER_OF_NUMBERS, j=0; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(j == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block in the middle */
    test_block_insertion_initialise2(V);
    {
      ucl_vector_insert_block(V, 7, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<7; ++i) {
	assert(i == vector_ref(V, i));
      }
      for (i=7, j=0; i<(7+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(numbers[j] == vector_ref(V, i));
      }
      for (i=7+NUMBER_OF_NUMBERS, j=7; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(j == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block at end */
    test_block_insertion_initialise2(V);
    {
      ucl_vector_insert_block(V, SIZE_OF_VECTOR, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<SIZE_OF_VECTOR; ++i) {
	assert(i == vector_ref(V, i));
      }
      for (i=SIZE_OF_VECTOR, j=0; i<NUMBER_OF_NUMBERS; ++i, ++j) {
	assert(numbers[j] == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block at beginning */
    test_block_insertion_initialise3(V);
    {
      ucl_vector_insert_block(V, 0, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<NUMBER_OF_NUMBERS; ++i) {
	assert(numbers[i] == vector_ref(V, i));
      }
      for (i=NUMBER_OF_NUMBERS, j=0; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(j == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block in the middle */
    test_block_insertion_initialise3(V);
    {
      ucl_vector_insert_block(V, 7, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<7; ++i) {
	assert(i == vector_ref(V, i));
      }
      for (i=7, j=0; i<(7+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(numbers[j] == vector_ref(V, i));
      }
      for (i=7+NUMBER_OF_NUMBERS, j=7; i<(SIZE_OF_VECTOR+NUMBER_OF_NUMBERS); ++i, ++j) {
	assert(j == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
    /* inserting block at end */
    test_block_insertion_initialise3(V);
    {
      ucl_vector_insert_block(V, SIZE_OF_VECTOR, block);
      assert(ucl_vector_size(V) == SIZE_OF_VECTOR+NUMBER_OF_NUMBERS);
      for (i=0; i<SIZE_OF_VECTOR; ++i) {
	assert(i == vector_ref(V, i));
      }
      for (i=SIZE_OF_VECTOR, j=0; i<NUMBER_OF_NUMBERS; ++i, ++j) {
	assert(numbers[j] == vector_ref(V, i));
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_range_insertion (void)
{
  const size_t ACTUAL_VECTOR	= 10;
  mcl_test_begin("vector-3.6", "range insertion") {
    ucl_vector_config_t	C;
    ucl_vector_t	SRC, DST;
    ucl_index_t		i, j;
    ucl_range_t		R;
    ucl_vector_initialise_config(C, sizeof(size_t), 15);
    C->size_of_padding_area	= 5;
    ucl_vector_alloc(SRC, C);
    ucl_vector_alloc(DST, C);
    {
      fill(DST, ACTUAL_VECTOR, 0);
      fill(SRC, ACTUAL_VECTOR, 0);
      assert(ucl_vector_size(SRC) == ACTUAL_VECTOR);
      assert(ucl_vector_size(DST) == ACTUAL_VECTOR);
      for (i=0; i<ACTUAL_VECTOR; ++i) {
	assert(i == vector_ref(SRC, i));
	assert(i == vector_ref(DST, i));
      }
      ucl_range_set_min_size(R, 3, 5);
      ucl_vector_insert_range(DST, 5, SRC, R);
      assert(ucl_vector_size(DST) == 15);
      for (i=0; i<5; ++i) {
	assert(i == vector_ref(DST, i));
      }
      for (i=5, j=3; i<10; ++i, ++j) {
	assert(j == vector_ref(DST, i));
      }
      for (i=10, j=5; i<15; ++i, ++j) {
	assert(j == vector_ref(DST, i));
      }
    }
    ucl_vector_free(DST);
    ucl_vector_free(SRC);
  }
  mcl_test_end();
}

static void
test_erasing (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-4.1", "erasing elements") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up			= 6;
    C->step_down		= 10;
    C->size_of_padding_area	= 0;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i;
      size_t *		P;
      fill(V, NUMBER, DELTA);
      for (i=3; i<NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, 3);
	assert(NULL != P);
	ucl_vector_erase(V, P);
	assert(ucl_vector_size(V) == (size_t)((NUMBER - 1) + 3 - i));
      }

      P = ucl_vector_index_to_slot(V, 2);
      assert(P != NULL);
      ucl_vector_erase(V, P);
      P = ucl_vector_index_to_slot(V, 1);
      assert(P != NULL);
      ucl_vector_erase(V, P);
      P = ucl_vector_index_to_slot(V, 0);
      assert(P != NULL);
      ucl_vector_erase(V, P);

      assert(ucl_vector_size(V) == 0);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_range_erasure (void)
{
  mcl_test_begin("vector-4.2", "range erasure") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_range_t		R;
    ucl_index_t		i, j;
    size_t *		P;
    ucl_vector_initialise_config(C, sizeof(size_t), 50);
    C->size_of_padding_area	= 35;
    ucl_vector_alloc(V, C);
    {
      fill(V, 10, 0);
      ucl_range_set_min_max(R, 2, 4); /* from "2" to "4", inclusive */
      assert( ucl_vector_range_is_valid(V, R) );
      ucl_vector_erase_range(V, R);
      assert( ucl_vector_size(V) == 7 );
      for (i=0; i<2; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(*P == i);
      }
      for (j=5; (size_t)i<ucl_vector_size(V); ++i, ++j) {
	P = ucl_vector_index_to_slot(V, i);
	assert(*P == j);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_finding (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-5.1", "finding elements") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up			= 6;
    C->step_down		= 10;
    C->size_of_padding_area	= 0;
    C->compar			= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i, j;
      int *		P;
      ucl_value_t	D = { .pointer = &j };
      fill(V, NUMBER, DELTA);
      for (i=0; i<NUMBER; ++i) {
	j = i + DELTA;
	P = ucl_vector_find(V, D);
	assert(NULL != P);
	/* mcl_debug("i=%u, j=%u, *P=%u, %d\n", i, j, *P, (*P == j)); */
	assert(*P == j);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_sort_finding (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-5.2", "finding elements in sorted vector") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up			= 6;
    C->step_down		= 10;
    C->size_of_padding_area	= 0;
    C->compar			= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i, j;
      int *		P;
      ucl_value_t	D = { .pointer = &j };
      fill(V, NUMBER, DELTA);
      for (i=0; i<NUMBER; ++i) {
	j = i + DELTA;
	P = ucl_vector_sort_find(V, D);
	assert(P != NULL);
	assert(*P == j);
      }
    }
    ucl_vector_free(V);
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i, j;
      int *		P;
      ucl_value_t	D = { .pointer = &j };
      fill(V, NUMBER+1, DELTA);
      for (i=0; i<NUMBER+1; ++i) {
	j = i + DELTA;
	P = ucl_vector_sort_find(V, D);
	assert(P != NULL);
	assert(*P == j);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_binary_search (void)
{
  const size_t	NUMBER	= 1000;
  const size_t	DELTA	= 234;
  mcl_test_begin("vector-5.3", "binary search") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up			= 6;
    C->step_down		= 10;
    C->size_of_padding_area	= 0;
    C->compar			= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i, j;
      int *		P;
      ucl_value_t	D = { .pointer = &j };
      fill(V, NUMBER, DELTA);

      for (i=0; i<NUMBER; ++i) {
	j = i + DELTA;
	P = ucl_vector_binary_search(V, D);
	assert(P != NULL);
	assert(*P == j);
      }

      j=5*NUMBER;
      P = ucl_vector_binary_search(V, D);
      assert(P == NULL);

      j=-1;
      P = ucl_vector_binary_search(V, D);
      assert(P == NULL);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_quick_sorting (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-6.1", "quick sorting") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    C->step_up			= 6;
    C->step_down		= 10;
    C->size_of_padding_area	= 0;
    C->compar			= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i;
      int *		P;

      for (i=0; i<NUMBER; ++i) {
	ucl_vector_enlarge(V);
	P = ucl_vector_insert(V, ucl_vector_index_to_slot(V, 0));
	assert(NULL != P);
	*P = i;
      }

      ucl_vector_quick_sort(V);

      for (i=0; i<NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(NULL != P);
	assert(*P == i);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_sorted_predicate (void)
{
  const size_t SIZE		= 966;
  const size_t STEP_UP		= 884;
  const size_t STEP_DOWN	= 834;
  const size_t PAD		= 74;
  const size_t NUMBER		= 10000;
  mcl_test_begin("vector-6.2", "sorted predicate") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_vector_initialise_config(C, sizeof(size_t), SIZE);
    C->step_up			= STEP_UP;
    C->step_down		= STEP_DOWN;
    C->size_of_padding_area	= PAD;
    C->compar			= ucl_compare_int_pointer;
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i;
      int *		P;
      /* Push numbers [(NUMBER-1), 0]. */
      for (i=0; i<NUMBER; ++i) {
	P = ucl_vector_push_back(V);
	*P = i;
      }
      /* Assert numbers [(NUMBER-1), 0]. */
      for (i=0; i<NUMBER; ++i) {
	P = ucl_vector_index_to_slot(V, i);
	assert(i == *P);
      }
      assert(ucl_vector_sorted(V));
      P = ucl_vector_index_to_slot(V, 100);
      *P = 90;
      assert(0 == ucl_vector_sorted(V));
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_iterating (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-7.1", "iterators") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_iterator_t	I;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i;
      int *		P;
      fill(V, NUMBER, 0);
      for (ucl_vector_iterator_forward(V, I), i=0; ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	P = ucl_iterator_ptr(I);
	assert(i == *P);
      }
      for (ucl_vector_iterator_backward(V, I), i=NUMBER-1; ucl_iterator_more(I);
	   ucl_iterator_next(I), --i) {
	P = ucl_iterator_ptr(I);
	assert(i == *P);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_range_iteration (void)
{
  const size_t	NUMBER	= 1000;
  mcl_test_begin("vector-7.2", "range iterators") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_iterator_t	I;
    ucl_range_t		R;
    ucl_vector_initialise_config(C, sizeof(size_t), 8);
    ucl_vector_alloc(V, C);
    {
      ucl_index_t	i;
      int *		P;
      fill(V, NUMBER, 0);
      {
	static const size_t	results[] = { 1, 2, 3, 4 };
	ucl_range_set_min_max(R, 1, 4);
	for (ucl_vector_iterator_range_forward(V, R, I), i=0;
	     ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	  P = ucl_iterator_ptr(I);
	  assert(results[i] == *P);
	}
      }
      {
	static const size_t	results[] = { 4, 3, 2, 1 };
	ucl_range_set_min_max(R, 1, 4);
	for (ucl_vector_iterator_range_backward(V, R, I), i=0;
	     ucl_iterator_more(I); ucl_iterator_next(I), ++i) {
	  P = ucl_iterator_ptr(I);
	  assert(results[i] == *P);
	}
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_appending (void)
{
  mcl_test_begin("vector-8.1", "appending") {
    ucl_vector_config_t	C;
    ucl_vector_t	DST, SRC;
    ucl_range_t		R;
    ucl_index_t		i;
    int *		P;
    /* whole */
    {
      ucl_vector_initialise_config(C, sizeof(size_t), 8);
      ucl_vector_alloc(DST, C);
      ucl_vector_alloc(SRC, C);
      {
	fill(DST, 10, 0);
	fill(SRC, 10, 10);
	ucl_vector_append(DST, SRC);
	assert(ucl_vector_size(DST) == 20);
	for (i=0; i<20; ++i) {
	  P = ucl_vector_index_to_slot(DST, i);
	  assert(*P == i);
	}
      }
      ucl_vector_free(SRC);
      ucl_vector_free(DST);
    }
    /* range */
    {
      const size_t DST_SIZE	= 10;
      const size_t SRC_SIZE	= 10;
      const size_t DST_FIRST	= 0;
      const size_t SRC_FIRST	= 8;
      const size_t RANGE_SIZE	= (SRC_SIZE-5);
      ucl_vector_initialise_config(C, sizeof(size_t), DST_SIZE);
      ucl_vector_alloc(DST, C);
      ucl_vector_initialise_config(C, sizeof(size_t), SRC_SIZE);
      ucl_vector_alloc(SRC, C);
      {
	fill(DST, DST_SIZE, DST_FIRST);
	fill(SRC, SRC_SIZE, SRC_FIRST);
	ucl_range_set_min_size(R, (DST_SIZE-SRC_FIRST), RANGE_SIZE);
	assert(ucl_vector_range_is_valid(SRC, R));
#if (DEBUGGING == 1)
	debug("dst vector:");
	for (i=0; i<(int)ucl_vector_size(DST); ++i) {
	  P = ucl_vector_index_to_slot(DST, i);
	  debug("%d %d", *P, i);
	}
#endif
#if (DEBUGGING == 1)
	debug("src vector:");
	for (i=0; i<(int)ucl_vector_size(SRC); ++i) {
	  P = ucl_vector_index_to_slot(SRC, i);
	  debug("%d %d", *P, i);
	}
#endif
	ucl_vector_append_range(DST, SRC, R);
	assert(ucl_vector_size(DST) == (DST_SIZE+RANGE_SIZE));
#if (DEBUGGING == 1)
	debug("dst vector:");
	for (i=0; i<(int)ucl_vector_size(DST); ++i) {
	  P = ucl_vector_index_to_slot(DST, i);
	  debug("%d %d", *P, i);
	}
#endif
	for (i=0; i<15; ++i) {
	  P = ucl_vector_index_to_slot(DST, i);
	  debug("%d %d", *P, i);
	  assert(*P == i);
	}
      }
      ucl_vector_free(SRC);
      ucl_vector_free(DST);
    }
  }
  mcl_test_end();
}

static void
test_appending_more (void)
{
  mcl_test_begin("vector-8.2", "appending more") {
    const size_t VECTOR_SIZE	= 10;
    const size_t starts[4] = {
      VECTOR_SIZE, 2*VECTOR_SIZE, 3*VECTOR_SIZE, 4*VECTOR_SIZE
    };
    ucl_vector_config_t	C;
    ucl_vector_t	SRC[4];
    ucl_vector_t	DST;
    ucl_index_t		i;
    int *		P;
    ucl_vector_initialise_config(C, sizeof(size_t), 16);
    for (i=0; i<4; ++i)
      ucl_vector_alloc(SRC[i], C);
    ucl_vector_alloc(DST, C);
    {
      for (i=0; i<4; ++i)
	fill(SRC[i], VECTOR_SIZE, starts[i]);
      fill(DST, VECTOR_SIZE, 0);
      ucl_vector_append_more(DST, SRC[0], SRC[1], SRC[2], SRC[3], NULL);
      assert(ucl_vector_size(DST) == 50);
      for (i=0; i<50; ++i) {
	P = ucl_vector_index_to_slot(DST, i);
	assert(*P == i);
      }
    }
    for (i=0; i<4; ++i) {
      ucl_vector_free(SRC[i]);
    }
    ucl_vector_free(DST);
  }
  mcl_test_end();
}

static void
test_comparison (void)
{
  mcl_test_begin("vector-10.1", "comparison") {
    ucl_vector_config_t	C;
    ucl_vector_t	A, B;
    ucl_range_t		RA, RB;
    ucl_vector_initialise_config(C, sizeof(size_t), 16);
    C->compar	= ucl_compare_int_pointer;
    /* equal */
    {
      ucl_vector_alloc(A, C);
      ucl_vector_alloc(B, C);
      {
	fill(A, 10, 0);
	fill(B, 10, 0);
	assert( ucl_vector_compare(A, B) == 0);
	assert( ucl_vector_equal(A, B) );
      }
      ucl_vector_free(B);
      ucl_vector_free(A);
    }
    /* lesser */
    {
      ucl_vector_alloc(A, C);
      ucl_vector_alloc(B, C);
      {
	fill(A, 10, 0);
	fill(B, 10, 1);
	assert( ucl_vector_compare(A, B) == -1);
      }
      ucl_vector_free(B);
      ucl_vector_free(A);
    }
    /* greater */
    {
      ucl_vector_alloc(A, C);
      ucl_vector_alloc(B, C);
      {
	fill(A, 10, 1);
	fill(B, 10, 0);
	assert( ucl_vector_compare(A, B) == 1);
      }
      ucl_vector_free(B);
      ucl_vector_free(A);
    }
    /* range equal */
    {
      ucl_vector_alloc(A, C);
      ucl_vector_alloc(B, C);
      {
	ucl_range_set_min_size(RA, 5, 5);
	ucl_range_set_min_size(RB, 2, 5);
	fill(A, 10, 0);
	fill(B, 10, 3);
	assert( ucl_vector_range_is_valid(A, RA) );
	assert( ucl_vector_range_is_valid(B, RB) );
	assert( ucl_vector_compare_range(A, RA, B, RB) == 0);
      }
      ucl_vector_free(B);
      ucl_vector_free(A);
    }
    /* range greater */
    {
      ucl_vector_alloc(A, C);
      ucl_vector_alloc(B, C);
      {
	ucl_range_set_min_size(RA, 5, 5); /* the first is 5*/
	ucl_range_set_min_size(RB, 1, 5); /* the first is 4 */
	fill(A, 10, 0);
	fill(B, 10, 3);
	assert( ucl_vector_range_is_valid(A, RA) );
	assert( ucl_vector_range_is_valid(B, RB) );
	assert( ucl_vector_compare_range(A, RA, B, RB) == 1);
      }
      ucl_vector_free(B);
      ucl_vector_free(A);
    }
    /* range lesser */
    {
      ucl_vector_alloc(A, C);
      ucl_vector_alloc(B, C);
      {
	ucl_range_set_min_size(RA, 5, 5); /* the first is 5*/
	ucl_range_set_min_size(RB, 3, 5); /* the first is 6 */
	fill(A, 10, 0);
	fill(B, 10, 3);
	assert( ucl_vector_range_is_valid(A, RA) );
	assert( ucl_vector_range_is_valid(B, RB) );
	assert( ucl_vector_compare_range(A, RA, B, RB) == -1);
      }
      ucl_vector_free(B);
      ucl_vector_free(A);
    }
  }
  mcl_test_end();
}

static void
test_inspection (void)
{
  mcl_test_begin("vector-11.1", "buffer configuration") {
    const size_t	NUMBER	= (10*UCL_VECTOR_BUFFER_PAGE_SIZE);
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_block_t		allocated_memory;
    ucl_vector_initialise_config_buffer(C);
    ucl_vector_alloc(V, C);
    {
      allocated_memory = ucl_vector_get_memory_block(V);
      assert(allocated_memory.ptr);
      assert( ucl_vector_number_of_free_slots(V) == UCL_VECTOR_BUFFER_PAGE_SIZE );

      assert( ucl_vector_number_of_step_up_slots(V) == UCL_VECTOR_BUFFER_PAGE_SIZE );
      assert( ucl_vector_number_of_step_down_slots(V) == UCL_VECTOR_BUFFER_PAGE_SIZE+1);
      assert( ucl_vector_number_of_padding_slots(V) == 0);

      assert( ucl_vector_slot_dimension(V) == sizeof(uint8_t) );

      fill(V, NUMBER, 0);
      assert(ucl_vector_size(V) == NUMBER);
      clean(V);
      fill(V, NUMBER, 0); clean(V);
      fill(V, NUMBER, 0); clean(V);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_accessors_range_block (void)
{
  const size_t SIZE		= 8;
  const size_t STEP_UP		= 6;
  const size_t STEP_DOWN	= 10;
  const size_t PAD		= 0;
  const size_t DIM		= sizeof(int);
  mcl_test_begin("vector-12.1", "range/block conversion") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_block_t		allocated_memory;
    ucl_block_t		B;
    ucl_range_t		R;
    ucl_vector_initialise_config(C, DIM, SIZE);
    C->step_up			= STEP_UP;
    C->step_down		= STEP_DOWN;
    C->size_of_padding_area	= PAD;
    ucl_vector_alloc(V, C);
    {
      allocated_memory = ucl_vector_get_memory_block(V);
      assert(allocated_memory.ptr);
      assert(allocated_memory.len == SIZE * DIM);
      assert( ucl_vector_number_of_free_slots(V)      == SIZE );
      assert( ucl_vector_number_of_step_up_slots(V)   == STEP_UP );
      assert( ucl_vector_number_of_step_down_slots(V) == STEP_DOWN );
      assert( ucl_vector_number_of_padding_slots(V)   == PAD );
      assert( ucl_vector_slot_dimension(V)            == DIM );

      fill(V, 100, 0);

      ucl_range_set_min_max(R, 0, 99);
      B = ucl_vector_block_from_range(V, R);
      for (ucl_index_t i=0; i<100; ++i) {
	/* mcl_debug("i=%d, B.ptr[i]=%d", i, ((size_t *)B.ptr)[i]); */
	assert(i == ((int *)B.ptr)[i]);
      }

      ucl_range_set_min_max(R, 10, 20);
      B = ucl_vector_block_from_range(V, R);
      for (ucl_index_t i=0; i<=10; ++i) {
	assert(i+10 == ((int *)B.ptr)[i]);
      }

      ucl_range_set_min_max(R, 10, 10);
      B = ucl_vector_block_from_range(V, R);
      for (ucl_index_t i=0; i<=10; ++i)
	assert(i+10 == ((int *)B.ptr)[i]);

      ucl_range_set_min_max(R, 0, 0);
      B = ucl_vector_block_from_range(V, R);
      for (ucl_index_t i=0; i<=0; ++i)
	assert(i == ((int *)B.ptr)[i]);

      ucl_range_set_min_max(R, 99, 99);
      B = ucl_vector_block_from_range(V, R);
      for (ucl_index_t i=0; i<=0; ++i)
	assert(i+99 == ((int *)B.ptr)[i]);

      /* ------------------------------------------------------------ */

      B.ptr = ucl_vector_index_to_slot(V, 0);
      B.len = ucl_vector_slot_dimension(V) * ucl_vector_size(V);
      R = ucl_vector_range_from_block(V, B);
      /*   fprintf(stderr, "%d %d\n", ucl_range_min(R), ucl_range_max(R)); */
      assert(0  == ucl_range_min(R));
      assert(99 == ucl_range_max(R));

      B.ptr = ucl_vector_index_to_slot(V, 10);
      B.len = ucl_vector_slot_dimension(V) * 10;
      R = ucl_vector_range_from_block(V, B);
      /*   fprintf(stderr, "%d %d\n", ucl_range_min(R), ucl_range_max(R)); */
      /* Ranges  are  inclusive,  and in  the  range  [0,  9] there  are  10
	 slots. */
      assert(10  == ucl_range_min(R));
      assert(19 == ucl_range_max(R));

      B.ptr = ucl_vector_index_to_slot(V, 12);
      B.len = ucl_vector_slot_dimension(V);
      R = ucl_vector_range_from_block(V, B);
      assert(12 == ucl_range_min(R));
      assert(12 == ucl_range_max(R));

      B.ptr = ucl_vector_index_to_slot(V, 0);
      B.len = ucl_vector_slot_dimension(V);
      R = ucl_vector_range_from_block(V, B);
      assert(0 == ucl_range_min(R));
      assert(0 == ucl_range_max(R));

      B.ptr = ucl_vector_index_to_slot(V, 99);
      B.len = ucl_vector_slot_dimension(V);
      R = ucl_vector_range_from_block(V, B);
      assert(99 == ucl_range_min(R));
      assert(99 == ucl_range_max(R));
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_block_access (void)
{
  const size_t SIZE		= 16;
  const size_t DIM		= sizeof(size_t);
  const size_t BLOCK_SIZE	= 5;
  mcl_test_begin("vector-12.2", "block access and mutation") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    size_t		numbers[BLOCK_SIZE];
    ucl_block_t		B = { .len = DIM * BLOCK_SIZE, .ptr = (void *)numbers };
    ucl_index_t		i, j;
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(V, C);
    {
      fill(V, 10, 1);
      ucl_vector_get_block(B, 3, V);
      for (i=0, j=4; i<BLOCK_SIZE; ++i, ++j)
	assert(j == numbers[i]);
    }
    ucl_vector_free(V);

    ucl_vector_alloc(V, C);
    {
      fill(V, 10, 1);
      ucl_vector_get_block(B, 0, V);
      for (i=0, j=1; i<BLOCK_SIZE; ++i, ++j)
	assert(j == numbers[i]);
    }
    ucl_vector_free(V);

    ucl_vector_alloc(V, C);
    {
      fill(V, 10, 1);
      ucl_vector_get_block(B, BLOCK_SIZE, V);
      for (i=0, j=6; i<BLOCK_SIZE; ++i, ++j)
	assert(j == numbers[i]);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_block_access_and_mutation (void)
{
  const size_t SIZE		= 8;
  const size_t STEP_UP		= 6;
  const size_t STEP_DOWN	= 10;
  const size_t PAD		= 0;
  const size_t DIM		= sizeof(int);
  mcl_test_begin("vector-12.3", "block access and mutation") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    ucl_block_t		allocated_memory;
    ucl_block_t		B;
    int *		p;
    ucl_vector_initialise_config(C, DIM, SIZE);
    C->step_up			= STEP_UP;
    C->step_down		= STEP_DOWN;
    C->size_of_padding_area	= PAD;
    ucl_vector_alloc(V, C);
    {
      allocated_memory = ucl_vector_get_memory_block(V);
      assert(allocated_memory.ptr);
      assert(ucl_vector_number_of_free_slots(V) == SIZE);
      assert( ucl_vector_number_of_step_up_slots(V) == STEP_UP );
      assert( ucl_vector_number_of_step_down_slots(V) == STEP_DOWN );
      assert( ucl_vector_number_of_padding_slots(V) == PAD );
      assert( ucl_vector_slot_dimension(V) == sizeof(int) );

      fill(V, 100, 0);

      ucl_vector_enlarge_for_slots(V, 10);
      B = ucl_vector_get_free_block_at_end(V, 10);
      assert(100 == ucl_vector_size(V));
      for (ucl_index_t i=0; i<10; ++i) {
	((int *)B.ptr)[i] = i + 100;
      }
      ucl_vector_mark_as_used(V, B);
      assert(110 == ucl_vector_size(V));
      for (ucl_index_t i=0; i<110; ++i) {
	p = ucl_vector_index_to_slot(V, i);
	assert(i == *p);
      }

      ucl_vector_enlarge_for_slots(V, 10);
      B = ucl_vector_get_free_block_at_beginning(V, 10);
      assert(110 == ucl_vector_size(V));
      for (ucl_index_t i=0; i<10; ++i) {
	((int *)B.ptr)[i] = i - 10;
      }
      ucl_vector_mark_as_used(V, B);
      assert(120 == ucl_vector_size(V));
      for (ucl_index_t i=0; i<120; ++i) {
	p = ucl_vector_index_to_slot(V, i);
	assert(i-10 == *p);
      }
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static ucl_value_t
test_for_each_callback (ucl_value_t state, va_list ap)
{
  ucl_value_t	custom		= va_arg(ap, ucl_value_t);
  size_t *	accumulator	= state.ptr;
  size_t *	slot		= custom.ptr;
  *accumulator += *slot;
  return ucl_value_null;
}
static void
test_for_each (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(size_t);
  mcl_test_begin("vector-13.1", "simple for each") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    int			accumulator = 0;
    ucl_callback_t	cb = {
      .func = test_for_each_callback,
      .data = { .ptr = &accumulator }
    };
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(V, C);
    {
      fill(V, 5, 1);
      ucl_vector_for_each(cb, V);
      assert((1+2+3+4+5) == accumulator);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static void
test_for_each_in_range (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(size_t);
  mcl_test_begin("vector-13.2", "for each in range") {
    ucl_vector_config_t	C;
    ucl_vector_t	V;
    int			accumulator = 0;
    ucl_range_t		R;
    ucl_callback_t	cb = {
      .func = test_for_each_callback,
      .data = { .ptr = &accumulator }
    };
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(V, C);
    {
      fill(V, 10, 1);

      ucl_range_set_min_max(R, 0, 4);
      ucl_vector_for_each_in_range(cb, R, V);
      assert((1+2+3+4+5) == accumulator);

      accumulator = 0;
      ucl_range_set_min_max(R, 6, 9);
      ucl_vector_for_each_in_range(cb, R, V);
      assert((7+8+9+10) == accumulator);

      accumulator = 0;
      ucl_range_set_min_max(R, 6, 6);
      ucl_vector_for_each_in_range(cb, R, V);
      assert(7 == accumulator);
    }
    ucl_vector_free(V);
  }
  mcl_test_end();
}

static ucl_value_t
test_for_each_multiple_callback (ucl_value_t state, va_list ap)
{
  ucl_value_t			custom  = va_arg(ap,ucl_value_t);
  int *				sums	= state.ptr;
  ucl_array_of_pointers_t *	slots	= custom.ptr;
  int **			values	= (int **)slots->slots;

  for (size_t i=0; i<slots->number_of_slots; ++i)
    sums[slots->data.t_size] += *values[i];
  return ucl_value_null;
}
static void
test_for_each_multiple (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(size_t);
  mcl_test_begin("vector-13.3", "for each multiple") {
    ucl_vector_config_t	C;
    ucl_vector_t	X, Y, Z;
    int			sums[5] = { 0, 0, 0, 0, 0 };
    ucl_callback_t	cb = {
      .func = test_for_each_multiple_callback,
      .data = { .ptr = &sums }
    };
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(X, C);
    ucl_vector_alloc(Y, C);
    ucl_vector_alloc(Z, C);
    {
      fill(X, 5, 1);
      fill(Y, 5, 6);
      fill(Z, 5, 11);
      ucl_vector_for_each_multiple(cb, X, Y, Z, NULL);

      assert((1+ 6+11) == sums[0]);
      assert((2+ 7+12) == sums[1]);
      assert((3+ 8+13) == sums[2]);
      assert((4+ 9+14) == sums[3]);
      assert((5+10+15) == sums[4]);
    }
    ucl_vector_free(X);
    ucl_vector_free(Y);
    ucl_vector_free(Z);
  }
  mcl_test_end();
}

static ucl_value_t
test_map_callback (ucl_value_t state UCL_UNUSED, va_list ap)
{
  ucl_value_t custom = va_arg(ap,ucl_value_t);
  ucl_array_of_pointers_t * slots   = custom.ptr;
  int *                     result  = slots->slots[0];
  int *                     operand = slots->slots[1];

  *result = - *operand;
  return ucl_value_null;
}
static void
test_map (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(int);
  mcl_test_begin("vector-13.4", "simple map") {
    ucl_vector_config_t	C;
    ucl_vector_t	result, operand;
    ucl_callback_t	cb = {
      .func = test_map_callback,
      .data = ucl_value_null
    };
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(result, C);
    ucl_vector_alloc(operand, C);
    {
      fill(operand, 5, 1);
      ucl_vector_map(result, cb, operand);
      for (ucl_index_t i=0; i<5; ++i) {
	int *	slot = ucl_vector_index_to_slot(result, i);
	assert(-(i+1) == *slot);
      }
    }
    ucl_vector_free(result);
    ucl_vector_free(operand);
  }
  mcl_test_end();
}

static void
test_map_in_range (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(int);
  mcl_test_begin("vector-13.5", "map in range") {
    ucl_vector_config_t	C;
    ucl_vector_t	result, operand;
    ucl_range_t		R;
    ucl_callback_t	cb = {
      .func = test_map_callback,
      .data = ucl_value_null
    };
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(result, C);
    ucl_vector_alloc(operand, C);
    {
      fill(operand, 10, 1);
      ucl_range_set_min_max(R, 3, 8);
      ucl_vector_map_range(result, cb, R, operand);
      for (ucl_index_t i=0; i<ucl_range_size(R); ++i) {
	int *	slot = ucl_vector_index_to_slot(result, i);
	assert(-(i+4) == *slot);
      }
    }
    ucl_vector_free(result);
    ucl_vector_free(operand);
  }
  mcl_test_end();
}

static ucl_value_t
test_map_multiple_callback (ucl_value_t state UCL_UNUSED, va_list ap)
{
  ucl_value_t			custom = va_arg(ap,ucl_value_t);
  ucl_array_of_pointers_t *	slots	= custom.ptr;
  size_t **			values	= (size_t **)slots->slots;

  *values[0] = 0;
  for (size_t i=1; i<slots->number_of_slots; ++i) {
    *values[0] += *values[i];
  }
  return ucl_value_null;
}
static void
test_map_multiple (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(size_t);
  mcl_test_begin("vector-13.6", "for each multiple") {
    ucl_vector_config_t	C;
    ucl_vector_t	R, X, Y, Z;
    ucl_callback_t	cb = {
      .func = test_map_multiple_callback,
      .data = ucl_value_null
    };
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(R, C);
    ucl_vector_alloc(X, C);
    ucl_vector_alloc(Y, C);
    ucl_vector_alloc(Z, C);
    {
      fill(X, 5, 1);
      fill(Y, 5, 6);
      fill(Z, 5, 11);

      debug("mapping");
      ucl_vector_map_multiple(R, cb, X, Y, Z, NULL);

      assert((1+ 6+11) == vector_ref(R, 0));
      assert((2+ 7+12) == vector_ref(R, 1));
      assert((3+ 8+13) == vector_ref(R, 2));
      assert((4+ 9+14) == vector_ref(R, 3));
      assert((5+10+15) == vector_ref(R, 4));
    }
    ucl_vector_free(R);
    ucl_vector_free(X);
    ucl_vector_free(Y);
    ucl_vector_free(Z);
  }
  mcl_test_end();
}

static void
test_copying (void)
{
  const size_t SIZE	= 8;
  const size_t DIM	= sizeof(int);
  mcl_test_begin("vector-14.1", "copying") {
    ucl_vector_config_t	C;
    ucl_vector_t	DST, SRC;
    ucl_range_t		R;
    ucl_index_t		i;
    int			j;
    ucl_vector_initialise_config(C, DIM, SIZE);
    ucl_vector_alloc(DST, C);
    ucl_vector_alloc(SRC, C);
    {
      fill(SRC, 10, 11);
      fill(DST, 15, 1);
      ucl_range_set_min_size(R, 4, 5);
      ucl_vector_copy_range(DST, 3, SRC, R);
      for (i=0, j=1; i<3; ++i, ++j)
	assert(j == vector_ref(DST, i));
      for (i=3, j=(11+4); i<(3+5); ++i, ++j) {
	assert(j == vector_ref(DST, i));
      }
      for (i=(3+5), j=(3+5+1); i<15; ++i, ++j)
	assert(j == vector_ref(DST, i));
    }
    ucl_vector_free(DST);
    ucl_vector_free(SRC);

    ucl_vector_alloc(DST, C);
    ucl_vector_alloc(SRC, C);
    {
      fill(SRC, 10, 11);
      fill(DST, 15, 1);
      ucl_range_set_min_size(R, 0, 5);
      ucl_vector_copy_range(DST, 3, SRC, R);
      for (i=0, j=1; i<3; ++i, ++j)
	assert(j == vector_ref(DST, i));
      for (i=3, j=11; i<(3+5); ++i, ++j) {
	assert(j == vector_ref(DST, i));
      }
      for (i=(3+5), j=(3+5+1); i<15; ++i, ++j)
	assert(j == vector_ref(DST, i));
    }
    ucl_vector_free(DST);
    ucl_vector_free(SRC);

    ucl_vector_alloc(DST, C);
    ucl_vector_alloc(SRC, C);
    {
      fill(SRC, 10, 11);
      fill(DST, 15, 1);
      ucl_range_set_min_size(R, 5, 5);
      ucl_vector_copy_range(DST, 10, SRC, R);
      for (i=0, j=1; i<10; ++i, ++j)
	assert(j == vector_ref(DST, i));
      for (i=10, j=(11+5); i<(10+5); ++i, ++j) {
	assert(j == vector_ref(DST, i));
      }
    }
    ucl_vector_free(DST);
    ucl_vector_free(SRC);
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
  test_reallocation();
  test_pushing_and_popping();
  test_block_swallowing();

  mcl_test_subtitle("index functions");
  test_index_functions();
  test_back_element();
  test_back_element_more ();
  test_front_element ();
  test_front_element_more ();
  test_front_and_back_elements ();

  mcl_test_subtitle("insertions");
  test_elements_insertion ();
  test_insert_sort ();
  test_more_insert_sort ();
  test_vector_insertion ();
  test_block_insertion ();
  test_range_insertion ();

  mcl_test_subtitle("removal");
  test_erasing ();
  test_range_erasure ();

  mcl_test_subtitle("searching");
  test_finding ();
  test_sort_finding ();
  test_binary_search ();

  mcl_test_subtitle("sorting");
  test_quick_sorting ();
  test_sorted_predicate ();

  mcl_test_subtitle("iterating");
  test_iterating ();
  test_range_iteration ();

  mcl_test_subtitle("appending");
  test_appending ();
  test_appending_more ();

  mcl_test_subtitle("comparison");
  test_comparison ();

  mcl_test_subtitle("custom configuration");
  test_inspection ();

  mcl_test_subtitle("accessors and mutators");
  test_accessors_range_block ();
  test_block_access ();
  test_block_access_and_mutation ();

  mcl_test_subtitle("map and for each");
  test_for_each ();
  test_for_each_in_range ();
  test_for_each_multiple ();
  test_map ();
  test_map_in_range ();
  test_map_multiple ();

  mcl_test_subtitle("copying");
  test_copying ();

  exit(EXIT_SUCCESS);
}

/* end of file */
