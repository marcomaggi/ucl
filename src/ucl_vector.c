/*
  Part of: Useless Containers Library
  Author: Marco Maggi
  Content: the code for the vector container.
  Creation date: sometime in 2001

  Abstract:

	The  UCL vector container  is an  implementation of  the classic
	array, with  hysteresis in  memory allocation.  This  module was
	inspired by the book on C++  by Bjarne Stroustrup and by the STL
	C++ (Standard Template Library).

  Copyright (c) 2001-2005, 2007-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#ifndef DEBUGGING
#  define DEBUGGING		0
#endif
#include "internal.h"

#define SLOT_NUMBER_THAT_TRIGGERS_BINARY_OVER_LINEAR_SEARCH	8


/** --------------------------------------------------------------------
 ** Assertion macros.
 ** ----------------------------------------------------------------- */

/* V stands for a pointer to  the vector struct.  P stands for a pointer
   to a slot.  Q stands for a generic pointer. */

#define ASSERT_VECTOR_IS_EMPTY(V)		assert(vector_is_empty(V));

#define ASSERT_VECTOR_POINTER(V)		assert((V))
#define ASSERT_MEMORY_ALLOCATED(V)		assert((V)->first_allocated_slot)

#define ASSERT_FRONT_MIN_FRONT_RELATION(V)			\
  assert((V)->first_allocated_slot <= (V)->first_used_slot)
#define ASSERT_BACK_MAX_BACK_RELATION(V)			\
  assert((V)->last_used_slot       <= (V)->last_allocated_slot)
#define ASSERT_FRONT_BACK_RELATION(V)					\
  assert(((V)->first_used_slot <= (V)->last_used_slot) ||		\
	 ((V)->first_used_slot - (V)->slot_dimension == (V)->last_used_slot))

#define ASSERT_FRONT_BACK_OFFSET_CORRECTNESS(V)				\
  assert(ucl_vector_size(V) * (V)->slot_dimension ==			\
	 (V)->last_used_slot - (V)->first_used_slot + (V)->slot_dimension)

#define ASSERT_NOT_UNDERFLOW_POINTER(V, P)	assert((V)->first_used_slot <= (P))
#define ASSERT_NOT_OVERFLOW_POINTER(V, P)	assert((V)->last_used_slot  >= (P))
#define ASSERT_NOT_NEW_OVERFLOW_POINTER(V, P)				\
  assert((V)->last_allocated_slot + (V)->slot_dimension >= (P));	\
  assert((V)->last_used_slot      + (V)->slot_dimension >= (P));

#define ASSERT_IS_POINTER_TO_USED_SLOT(V, P)	\
  assert(P);					\
  ASSERT_NOT_UNDERFLOW_POINTER(V, P);		\
  ASSERT_NOT_OVERFLOW_POINTER(V, P);
#define ASSERT_IS_POINTER_TO_NEW_SLOT(V, P)	\
  assert(P);					\
  ASSERT_NOT_UNDERFLOW_POINTER(V, P);		\
  ASSERT_NOT_NEW_OVERFLOW_POINTER(V, P);

/* Assert that  pointer P is  a valid value  for a pointer to  the first
   used slots; it means  that the block of used bytes can  be moved to P
   without going out of the allocated block. */
#define ASSERT_VALID_POINTER_AS_FIRST_USED(V,Q)					\
  do {										\
    size_t	used_bytes = compute_used_bytes((V));				\
    assert(Q);									\
    assert((V)->first_allocated_slot <= (Q));					\
    assert((Q) <= (V)->last_allocated_slot + (V)->slot_dimension - used_bytes); \
  } while (0);

#define ASSERT_COMPAR_FUNCTION_IS_SET(V)		assert((V)->compar.func)

#define ASSERT_FIRST_SLOT_IS_AT_PAD_OFFSET(V)			\
  assert((V)->first_used_slot ==				\
	 (V)->first_allocated_slot + (V)->size_of_padding_area)

#define ASSERT_THERE_IS_ROOM_AT_BEGINNING(V)	\
  assert(there_is_room_at_the_beginning(V));
#define ASSERT_THERE_IS_ROOM_AT_END(V)		\
  assert(there_is_room_at_the_end(V));
#define ASSERT_THERE_IS_ROOM(V)			\
  assert(there_is_room_at_the_beginning(V) ||	\
	 there_is_room_at_the_end(V));

#define ASSERT_INITIALISE_CONDITIONS(V)		\
  assert(V);					\
  assert((V)->slot_dimension);

#define ASSERT_INVARIANTS(V)			\
  ASSERT_VECTOR_POINTER(V);			\
  ASSERT_MEMORY_ALLOCATED(V);			\
  ASSERT_FRONT_MIN_FRONT_RELATION(V);		\
  ASSERT_FRONT_BACK_RELATION(V);		\
  ASSERT_BACK_MAX_BACK_RELATION(V);		\
  ASSERT_FRONT_BACK_OFFSET_CORRECTNESS(V);	\
  ASSERT_SLOTS(V);

#define ASSERT_SLOTS(V)							\
  assert((compute_allocated_slots(V) - compute_used_slots(V)) ==	\
	 (compute_slots_at_beginning(V) + compute_slots_at_end(V)));


/** --------------------------------------------------------------------
 ** Module's function prototypes.
 ** ----------------------------------------------------------------- */

/* Number of bytes computations. */
static inline size_t	compute_allocated_bytes		(const ucl_vector_t this);
static inline size_t	compute_used_bytes		(const ucl_vector_t this);
static inline size_t	compute_free_bytes		(const ucl_vector_t this);
static inline size_t    compute_bytes_at_beginning	(const ucl_vector_t this);
static inline size_t    compute_bytes_at_end		(const ucl_vector_t this);

static inline size_t	compute_allocated_slots		(const ucl_vector_t self);
static inline size_t	compute_used_slots		(const ucl_vector_t self);
static inline size_t	compute_free_slots		(const ucl_vector_t self);
static inline size_t	compute_slots_at_end		(const ucl_vector_t self);
static inline size_t	compute_slots_at_beginning	(const ucl_vector_t self);

static inline ucl_bool_t there_is_room_at_the_beginning	(const ucl_vector_t this);
static inline ucl_bool_t there_is_room_at_the_end	(const ucl_vector_t this);
static inline ucl_bool_t vector_is_empty		(const ucl_vector_t this);

/* Update pointers after moving/inserting/erasing used slots. */
static inline void	set_pointers_for_empty_vector(ucl_vector_t this);

/* Helper function for "ucl_vector_index()" and  "ucl_vector_newindex()". */
static void *	pointer_to_slot_or_null	(const ucl_vector_t this, ucl_vector_index_t index,
					 size_t slot_dimension_or_zero);

/* (Re)Allocation of the memory block. */
static void	reallocate_block	(ucl_vector_t this, size_t new_size_in_bytes);

/* Moving used bytes. */
static inline void move_used_bytes_at_beginning		(ucl_vector_t this);
static inline void move_used_bytes_at_end		(ucl_vector_t this);
static inline void move_used_bytes_at_byte_pointer	(ucl_vector_t this, uint8_t *p);
static inline void move_used_bytes_at_index_offset	(ucl_vector_t this, ucl_vector_index_t offset);

/* Move used bytes around to preserve padding and free memory when reallcoating. */
static void move_used_bytes_at_pad_area_beginning (ucl_vector_t this);
static void move_used_bytes_leave_requested_at_end (ucl_vector_t this, size_t bytes_requested_at_end);

/* Iterators advancing functions. */
static ucl_iterator_next_t	iterator_forward_next;
static ucl_iterator_next_t	iterator_backward_next;
static ucl_iterator_next_t	iterator_range_forward_next;
static ucl_iterator_next_t	iterator_range_backward_next;
static void iterator_next	(ucl_iterator_t iterator, ucl_bool_t forward);
static void iterator_range_next	(ucl_iterator_t iterator, ucl_bool_t forward);

/* Helper functions for "ucl_vector_insert()". */
static uint8_t *	\
insert_slot_moving_data_toward_the_beginning	(ucl_vector_t this, uint8_t * pointer_to_slot_p);
static uint8_t *	\
insert_slot_moving_data_toward_the_end	(ucl_vector_t this, uint8_t * pointer_to_slot_p);

/* Helper functions for "ucl_vector_erase()". */
static inline void \
erase_range_moving_tail_data_toward_the_beginning (ucl_vector_t this, ucl_byte_pointer_range_t range);
static inline void \
erase_range_moving_head_data_toward_the_end (ucl_vector_t this, ucl_byte_pointer_range_t range);


/** ------------------------------------------------------------
 ** Inspection functions.
 ** ----------------------------------------------------------*/

#if (DEBUGGING == 1)
static void
PRINT_SLOT_INSPECTION (ucl_vector_t self, const char *title)
{
  debug("-------- %s --------\n\
\t\tnumber of allocated slots %d (bytes %d)\n\
\t\tnumber of used slots %d (bytes %d)\n\
\t\tnumber of free slots %d (bytes %d)\n\
\t\tnumber of slots for padding %d (bytes %d)\n\
\t\tnumber of slots at beginning %d (bytes %d)\n\
\t\tnumber of slots at end %d (bytes %d)",
	title,
	compute_allocated_slots(self), compute_allocated_bytes(self),
	compute_used_slots(self), compute_used_bytes(self),
	compute_free_slots(self), compute_free_bytes(self),
	self->size_of_padding_area/self->slot_dimension,
	self->size_of_padding_area,
	compute_slots_at_beginning(self), compute_bytes_at_beginning(self),
	compute_slots_at_end(self), compute_bytes_at_end(self));
}
static void
PRINT_POINTER_INSPECTION (ucl_vector_t self, const char *title)
{
  debug("-------- %s --------\n\
\t\tfirst_allocated\t%p\n\
\t\tfirst_used\t%p\n\
\t\tlast_allocated\t%p\n\
\t\tlast_used\t%p\n\
\t\t(first_used < last_used)? = %s", title,
	self->first_allocated_slot, self->first_used_slot,
	self->last_allocated_slot, self->last_used_slot,
	debug_question(self->first_used_slot < self->last_used_slot));
}
#else
#  define PRINT_SLOT_INSPECTION(VECTOR, TITLE)		/* empty */
#  define PRINT_POINTER_INSPECTION(VECTOR, TITLE)	/* empty */
#endif


void
ucl_vector_initialise_config (ucl_vector_config_t config, size_t slot_dimension, size_t number_of_slots)
{
  assert(config);
  assert(slot_dimension);
  config->step_up		= UCL_VECTOR_DEFAULT_STEP_UP;
  config->step_down		= UCL_VECTOR_DEFAULT_STEP_DOWN;
  config->size_of_padding_area	= UCL_VECTOR_DEFAULT_PAD;
  config->slot_dimension	= slot_dimension;
  config->number_of_slots	= number_of_slots;
  config->allocator.data	= NULL;
  config->allocator.alloc	= ucl_memory_alloc;
}
void
ucl_vector_initialise_config_buffer (ucl_vector_config_t config)
{
  assert(config);
  config->step_up		= UCL_VECTOR_BUFFER_PAGE_SIZE;
  config->step_down		= UCL_VECTOR_BUFFER_PAGE_SIZE;
  config->size_of_padding_area	= 0;
  config->slot_dimension	= sizeof(uint8_t);
  config->number_of_slots	= UCL_VECTOR_BUFFER_PAGE_SIZE;
  config->allocator.data	= NULL;
  config->allocator.alloc	= ucl_memory_alloc;
}
void
ucl_vector_initialise_config_hash (ucl_vector_config_t config)
{
  assert(config);
  config->step_up		= UCL_HASH_DEFAULT_STEP_UP;
  config->step_down		= UCL_HASH_DEFAULT_STEP_DOWN;
  config->size_of_padding_area	= 0;
  config->slot_dimension	= sizeof(void *);
  config->number_of_slots	= UCL_HASH_DEFAULT_SIZE;
  config->allocator.data	= NULL;
  config->allocator.alloc	= ucl_memory_alloc;
}
void
ucl_vector_initialise_config_dfs (ucl_vector_config_t config)
{
  assert(config);
  config->step_up		= UCL_GRAPH_DFS_STEP_UP;
  config->step_down		= UCL_GRAPH_DFS_STEP_DOWN;
  config->size_of_padding_area	= 0;
  config->slot_dimension	= sizeof(ucl_graph_dfs_item_t);
  config->number_of_slots	= UCL_GRAPH_DFS_INITIAL_VECTOR_SIZE;
  config->allocator.data	= NULL;
  config->allocator.alloc	= ucl_memory_alloc;
}
void
ucl_vector_alloc (ucl_vector_t self, ucl_vector_config_t config)
{
  assert(self);
  assert(config);
  size_t	dim  = config->slot_dimension;
  size_t	number_of_bytes = dim * config->number_of_slots;
  uint8_t *	p;
  ASSERT_INITIALISE_CONDITIONS(config);
  config->allocator.alloc(config->allocator.data, &p, number_of_bytes);
  assert(p);
  self->slot_dimension		= dim;
  self->step_up			= dim * config->step_up;
  self->step_down		= dim * config->step_down;
  if (self->step_up >= self->step_down)
    self->step_down = self->step_up + dim;
  self->size_of_padding_area	= dim * config->size_of_padding_area;
  self->allocator		= config->allocator;
  self->compar			= config->compar;
  self->first_allocated_slot	= p;
  self->last_allocated_slot	= self->first_allocated_slot + number_of_bytes - dim;
  set_pointers_for_empty_vector(self);
  PRINT_SLOT_INSPECTION(self, "construction");
  PRINT_POINTER_INSPECTION(self, "construction");
  ASSERT_INVARIANTS(self);
}
void
ucl_vector_free (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);
  self->allocator.alloc(self->allocator.data, &(self->first_allocated_slot), 0);
  self->last_allocated_slot	= NULL;
  self->first_used_slot		= NULL;
  self->last_used_slot		= NULL;
}
void
ucl_vector_swallow_block (ucl_vector_t self, ucl_block_t block)
{
  ASSERT_INITIALISE_CONDITIONS(self);
  self->step_up			*= self->slot_dimension;
  self->step_down		*= self->slot_dimension;
  if (self->step_up >= self->step_down)
    self->step_down = self->step_up + self->slot_dimension;
  self->size_of_padding_area	*= self->slot_dimension;
  assert(0 == (block.len % self->slot_dimension));
  self->first_allocated_slot = self->first_used_slot = block.ptr;
  self->last_allocated_slot  = self->last_used_slot  = block.ptr + block.len - self->slot_dimension;
  ASSERT_INVARIANTS(self);
  PRINT_SLOT_INSPECTION(self, "construction");
}


/** ------------------------------------------------------------
 ** Inspection and configuration.
 ** ----------------------------------------------------------*/

size_t
ucl_vector_number_of_step_up_slots (const ucl_vector_t self)
{
  return (self->step_up)? (self->step_up / self->slot_dimension) : 0;
}
size_t
ucl_vector_number_of_step_down_slots (const ucl_vector_t self)
{
  return (self->step_down)? (self->step_down / self->slot_dimension) : 0;
}
size_t
ucl_vector_number_of_padding_slots (const ucl_vector_t self)
{
  return ((self->size_of_padding_area)?
	  (self->size_of_padding_area / self->slot_dimension) : 0);
}
ucl_bool_t
ucl_vector_running (const ucl_vector_t self)
{
  return (NULL != self->first_allocated_slot);
}
void
ucl_vector_update_number_of_step_up_slots (ucl_vector_t self, size_t step_up)
{
  self->step_up = step_up * self->slot_dimension;
  if (self->step_up >= self->step_down)
    self->step_down = self->step_up + self->slot_dimension;
}
void
ucl_vector_update_number_of_step_down_slots (ucl_vector_t self, size_t step_down)
{
  self->step_down = step_down * self->slot_dimension;
  if (self->step_up >= self->step_down)
    self->step_down = self->step_up + self->slot_dimension;
}
void
ucl_vector_update_number_of_padding_slots (ucl_vector_t self, size_t padding)
{
  self->size_of_padding_area = padding * self->slot_dimension;
}


/** ------------------------------------------------------------
 ** Special slots functions.
 ** ----------------------------------------------------------*/

void
ucl_vector_mark_all_slots_as_used (ucl_vector_t self)
{
  self->first_used_slot = self->first_allocated_slot;
  self->last_used_slot  = self->last_allocated_slot;
}
void
ucl_vector_mark_allocated_range_as_used (ucl_vector_t self, ucl_range_t range)
{
  void *	potential_first_used_slot;
  void *	potential_last_used_slot;
  potential_first_used_slot = self->first_allocated_slot + ucl_range_min(range) * self->slot_dimension;
  potential_last_used_slot  = self->first_allocated_slot + ucl_range_max(range) * self->slot_dimension;
  assert((uint8_t *)potential_first_used_slot <= (uint8_t *)potential_last_used_slot);
  assert((uint8_t *)potential_first_used_slot >= (uint8_t *)self->first_allocated_slot);
  assert((uint8_t *)potential_first_used_slot <= (uint8_t *)self->last_allocated_slot);
  assert((uint8_t *)potential_last_used_slot  >= (uint8_t *)self->first_allocated_slot);
  assert((uint8_t *)potential_last_used_slot  <= (uint8_t *)self->last_allocated_slot);
  self->first_used_slot = potential_first_used_slot;
  self->last_used_slot  = potential_last_used_slot;
}
void
ucl_vector_reset (ucl_vector_t self)
{
  set_pointers_for_empty_vector(self);
}
void
ucl_vector_clean (ucl_vector_t self)
{
  ucl_block_t	slots;

  slots = ucl_vector_get_memory_block(self);
  ucl_block_clean_memory(slots);
}


/** ------------------------------------------------------------
 ** Converting indexes to pointers.
 ** ----------------------------------------------------------*/

void *
ucl_vector_index_to_slot (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return pointer_to_slot_or_null(self, idx, 0);
}
void *
ucl_vector_index_to_new_slot (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return pointer_to_slot_or_null(self, idx, self->slot_dimension);
}
static void *
pointer_to_slot_or_null (const ucl_vector_t self, ucl_vector_index_t idx, size_t slot_dimension_or_zero)
{
  ASSERT_INVARIANTS(self);
  uint8_t * pointer_to_slot_p   = self->first_used_slot     + self->slot_dimension * idx;
  uint8_t * max_allowed_value_p = self->last_allocated_slot + slot_dimension_or_zero;
  return (pointer_to_slot_p <= max_allowed_value_p)? pointer_to_slot_p : NULL;
}
ucl_vector_index_t
ucl_vector_last_index (const ucl_vector_t self)
{
  ucl_vector_index_t	last = compute_used_slots(self);
  return (last > 0)? (last - 1) : 0;
}
ucl_vector_index_t
ucl_vector_slot_to_index (const ucl_vector_t self, const void *_pointer_to_slot_p)
{
  const uint8_t * pointer_to_slot_p = _pointer_to_slot_p;
  return (pointer_to_slot_p - self->first_used_slot)/self->slot_dimension;
}
ucl_bool_t
ucl_vector_pointer_is_valid_slot (const ucl_vector_t self, const void *_pointer_to_slot_p)
{
  const uint8_t *	pointer_to_slot_p = _pointer_to_slot_p;
  return ((pointer_to_slot_p >= self->first_used_slot) &&
	  (pointer_to_slot_p <= self->last_used_slot)  &&
	  (((pointer_to_slot_p - self->first_used_slot)%self->slot_dimension)==0));
}
ucl_bool_t
ucl_vector_index_is_valid (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return (idx < ucl_vector_size(self));
}
ucl_bool_t
ucl_vector_index_is_valid_new (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return (idx <= ucl_vector_size(self));
}


/** ------------------------------------------------------------
 ** Range functions.
 ** ----------------------------------------------------------*/

ucl_bool_t
ucl_vector_range_is_valid (const ucl_vector_t self, ucl_range_t range)
{
  return (ucl_vector_index_is_valid(self, ucl_range_min(range)) &&
	  ucl_vector_index_is_valid(self, ucl_range_max(range)));
}
ucl_range_t
ucl_vector_range (const ucl_vector_t self)
{
  ucl_range_t	range;
  ucl_range_set_min_size(range, 0, ucl_vector_size(self));
  return range;
}
ucl_range_t
ucl_vector_range_from_position_to_end (const ucl_vector_t self, ucl_vector_index_t position)
{
  ucl_range_t	range;
  assert(ucl_vector_index_is_valid(self, position));
  ucl_range_set_min_size(range, position, ucl_vector_size(self)-position);
  return range;
}
ucl_range_t
ucl_vector_range_from_end_to_position (const ucl_vector_t self, ucl_vector_index_t position)
{
  ucl_range_t	range;
  assert(ucl_vector_size(self) <= position);
  ucl_range_set_min_size(range, ucl_vector_size(self), position);
  return range;
}
ucl_range_t
ucl_vector_range_from_end_with_span (const ucl_vector_t self, size_t span)
{
  ucl_range_t	range;
  size_t	size = ucl_vector_size(self);
  ucl_range_set_min_size(range, size, span);
  return range;
}


/** ------------------------------------------------------------
 ** Insert functions.
 ** ----------------------------------------------------------*/

void *
ucl_vector_insert (ucl_vector_t self, void *_pointer_to_slot_p)
{
  ucl_bool_t	room_at_the_end, room_at_the_beginning;
  uint8_t *	pointer_to_slot_p = _pointer_to_slot_p;
  debug("making room at pointer %p, first %p, index %d",
	pointer_to_slot_p, self->first_used_slot,
	ucl_vector_slot_to_index(self, pointer_to_slot_p));
  ASSERT_INVARIANTS(self);
  ASSERT_IS_POINTER_TO_NEW_SLOT(self, pointer_to_slot_p);
  debug("room at beg: %s, room at end: %s",
	debug_question(there_is_room_at_the_beginning(self)),
	debug_question(there_is_room_at_the_end(self)));
  PRINT_SLOT_INSPECTION(self, "before insertion");
  ASSERT_THERE_IS_ROOM(self);
  if (vector_is_empty(self)) {
    assert(self->first_used_slot == pointer_to_slot_p);
    self->last_used_slot = self->first_used_slot;
  } else if (pointer_to_slot_p == self->last_allocated_slot + self->slot_dimension) {
    assert(self->last_used_slot == self->last_allocated_slot);
    memmove(self->first_used_slot - self->slot_dimension,
	    self->first_used_slot,
	    self->slot_dimension * compute_used_slots(self));
    self->first_used_slot -= self->slot_dimension;
    pointer_to_slot_p      = self->last_used_slot;
  } else {
    room_at_the_beginning	= there_is_room_at_the_beginning(self);
    room_at_the_end		= there_is_room_at_the_end(self);
    if (room_at_the_beginning && (pointer_to_slot_p == self->first_used_slot)) {
      debug("mark 1 %p", pointer_to_slot_p);
      self->first_used_slot -= self->slot_dimension;
      pointer_to_slot_p = self->first_used_slot;
    } else if (room_at_the_end && (pointer_to_slot_p == self->last_used_slot + self->slot_dimension)) {
      self->last_used_slot = pointer_to_slot_p;
    } else if (room_at_the_beginning && room_at_the_end) {
      debug("mark 2 %p", pointer_to_slot_p);
      if (compute_bytes_at_beginning(self) < compute_bytes_at_end(self)) {
	debug("mark 21 %p", pointer_to_slot_p);
	pointer_to_slot_p = insert_slot_moving_data_toward_the_end(self, pointer_to_slot_p);
      } else {
	debug("mark 22 %p", pointer_to_slot_p);
	pointer_to_slot_p = insert_slot_moving_data_toward_the_beginning(self, pointer_to_slot_p);
      }
    } else if (room_at_the_beginning) {
      debug("mark 3 %p", pointer_to_slot_p);
      assert(self->first_allocated_slot < self->first_used_slot);
      pointer_to_slot_p = \
	insert_slot_moving_data_toward_the_beginning(self, pointer_to_slot_p);
    } else {
      debug("mark 4 %p", pointer_to_slot_p);
      assert(room_at_the_end);
      pointer_to_slot_p = insert_slot_moving_data_toward_the_end(self, pointer_to_slot_p);
    }
  }
  debug("exit %p", pointer_to_slot_p);
  ASSERT_INVARIANTS(self);
  return pointer_to_slot_p;
}
static uint8_t *
insert_slot_moving_data_toward_the_beginning (ucl_vector_t self, uint8_t * pointer_to_slot_p)
{
  assert(there_is_room_at_the_beginning(self));
  debug("here %p", pointer_to_slot_p);
  if (pointer_to_slot_p != self->first_used_slot)
    memmove(self->first_used_slot - self->slot_dimension,
	    self->first_used_slot,
	    pointer_to_slot_p - self->first_used_slot);
  pointer_to_slot_p -= self->slot_dimension;
  self->first_used_slot -= self->slot_dimension;
  debug("there %p", pointer_to_slot_p);
  return pointer_to_slot_p;
}
static uint8_t *
insert_slot_moving_data_toward_the_end (ucl_vector_t self, uint8_t * pointer_to_slot_p)
{
  assert(there_is_room_at_the_end(self));
  if (pointer_to_slot_p != self->last_used_slot) {
    /*
      |--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
      ^  ^                    ^           ^
      |  |                    |           last_allocated_slot
      |  |                    last_used_slot
      |  pointer_to_slot_p + slot_dimension
      pointer_to_slot_p

      |..........................|
      last_used_slot - pointer_to_slot + slot_dimension
    */
    debug("pointer to slot NOT equal to last used");
    memmove(pointer_to_slot_p + self->slot_dimension, pointer_to_slot_p,
	    self->last_used_slot - pointer_to_slot_p + self->slot_dimension);
  } else {
    debug("pointer to slot equal to last used");
    memmove(self->last_used_slot + self->slot_dimension, self->last_used_slot, self->slot_dimension);
  }
  self->last_used_slot += self->slot_dimension;
  return pointer_to_slot_p;
}
void *
ucl_vector_insert_sort (ucl_vector_t self, void *data_p)
{
  ucl_value_t		inner = { .bytes = NULL };
  ucl_vector_index_t	size;
  ASSERT_INVARIANTS(self);
  ASSERT_COMPAR_FUNCTION_IS_SET(self);
  assert(data_p);
  size = ucl_vector_size(self);
  if (! size)
    inner.bytes = ucl_vector_index_to_new_slot(self, 0);
  else {
    ucl_value_t	outer = { .bytes = data_p };
    if (size <= SLOT_NUMBER_THAT_TRIGGERS_BINARY_OVER_LINEAR_SEARCH) {
      ucl_value_comparison_t	compar = self->compar;
      debug("linear search (size %u)", size);
      for (inner.bytes = self->first_used_slot;
	   inner.bytes <= self->last_used_slot;
	   inner.bytes += self->slot_dimension) {
	debug("current %d", *(int *)inner.bytes);
	if (compar.func(compar.data, outer, inner) <= 0) {
	  debug("it's it!!");
	  break;
	}
      }
    } else {
      size_t			i, lower_index_limit, upper_index_limit;
      size_t			current_index, new_current_index;
      int			match;
      ucl_value_comparison_t	compar = self->compar;
      debug("binary search (size %u)", size);
      lower_index_limit	= 0;
      upper_index_limit	= size-1;
      /* Doing shift-right-1 on  2*N will return N, on  2*N+1 will return N,
	 too (tested with the [expr] command of TCL). */
      current_index	= size >> 1;
      for (i=0; i<size; ++i) {
	inner.bytes = ucl_vector_index_to_slot(self, current_index);
	match = compar.func(compar.data, outer, inner);
	if (0 == match)
	  break;
	else if (match < 0) { /* the new item is less */
	  upper_index_limit = current_index - 1;
	  new_current_index = lower_index_limit + ((current_index - lower_index_limit) >> 1);
	  current_index = (new_current_index < current_index)? new_current_index : (new_current_index-1);
	  if (current_index < lower_index_limit) {
	    debug("lower %d", current_index+1);
	    inner.bytes = ucl_vector_index_to_new_slot(self, current_index+1);
	    break;
	  }
	} else { /* match > 0 */ /* the new item is greater */
	  lower_index_limit = current_index + 1;
	  new_current_index = current_index + ((upper_index_limit - current_index) >> 1);
	  current_index = (new_current_index > current_index)? new_current_index : (new_current_index+1);
	  if (current_index > upper_index_limit) {
	    inner.bytes = ucl_vector_index_to_new_slot(self, current_index);
	    debug("upper %d", current_index);
	    break;
	  }
	}
      }
    }
  }
  debug("exiting %p (%d)", inner.bytes, *((int *)inner.bytes));
  return ucl_vector_insert(self, inner.bytes);
}

/** ------------------------------------------------------------
 ** Erase functions.
 ** ----------------------------------------------------------*/

void
ucl_vector_erase (ucl_vector_t self, void *_pointer_to_slot_p)
{
  ucl_bool_t			room_at_the_end, room_at_the_beginning;
  uint8_t *			pointer_to_slot_p = _pointer_to_slot_p;
  ucl_byte_pointer_range_t	range;
  ASSERT_INVARIANTS(self);
  ASSERT_IS_POINTER_TO_USED_SLOT(self, pointer_to_slot_p);
  assert(ucl_vector_size(self));
  if (1 == ucl_vector_size(self)) {
    set_pointers_for_empty_vector(self);
  } else if (pointer_to_slot_p == self->first_used_slot) {
    self->first_used_slot += self->slot_dimension;
  } else if (pointer_to_slot_p == self->last_used_slot) {
    self->last_used_slot -= self->slot_dimension;
  } else {
    /* the range is inclusive !!! */
    ucl_range_set_min_max(range, pointer_to_slot_p, pointer_to_slot_p);
    room_at_the_beginning	= there_is_room_at_the_beginning(self);
    room_at_the_end		= there_is_room_at_the_end(self);
    if (room_at_the_beginning && room_at_the_end) {
      if (compute_bytes_at_beginning(self) < compute_bytes_at_end(self)) {
	goto move_head_data_toward_the_end;
      } else {
	goto move_tail_data_toward_the_beginning;
      }
    } else if (room_at_the_beginning) {
      goto move_tail_data_toward_the_beginning;
    } else {
      goto move_head_data_toward_the_end;
    }
  }
  return;
 move_tail_data_toward_the_beginning:
  erase_range_moving_tail_data_toward_the_beginning(self, range);
  return;
 move_head_data_toward_the_end:
  erase_range_moving_head_data_toward_the_end(self, range);
}
static inline void
erase_range_moving_tail_data_toward_the_beginning (ucl_vector_t self, ucl_byte_pointer_range_t range)
{
  debug("move tail data toward the beg");
  /* Notice:
     (last_used_slot + slot_dimension) - (range.max + slot_dimension) = (last_used_slot - range.max)
  */
  memmove(range.min, (range.max + self->slot_dimension), (self->last_used_slot - (range.max)));
  self->last_used_slot -= (range.max + self->slot_dimension) - range.min;
}
static inline void
erase_range_moving_head_data_toward_the_end (ucl_vector_t self, ucl_byte_pointer_range_t range)
{
  uint8_t *	new_first;
  debug("move head data toward the end");
  new_first = self->first_used_slot + (range.max - range.min + self->slot_dimension);
  memmove(new_first, self->first_used_slot, (range.min - self->first_used_slot));
  self->first_used_slot = new_first;
}


/** ------------------------------------------------------------
 ** Various operations.
 ** ----------------------------------------------------------*/

size_t
ucl_vector_slot_dimension (const ucl_vector_t self)
{
  return self->slot_dimension;
}
size_t
ucl_vector_size (const ucl_vector_t self)
{
  return compute_used_slots(self);
}
void
ucl_vector_set_compar (ucl_vector_t self, ucl_value_comparison_t compar)
{
  self->compar = compar;
}
ucl_block_t
ucl_vector_get_memory_block (const ucl_vector_t self)
{
  ucl_block_t	allocated_memory;
  allocated_memory.len = compute_allocated_bytes(self);
  allocated_memory.ptr = self->first_allocated_slot;
  return allocated_memory;
}
ucl_block_t
ucl_vector_get_data_block (const ucl_vector_t self)
{
  ucl_block_t	data;
  data.len = compute_used_bytes(self);
  data.ptr = self->first_used_slot;
  return data;
}
ucl_block_t
ucl_vector_get_free_block_at_end (ucl_vector_t self, size_t requested_slots)
{
  ucl_block_t	free_at_end;
  assert(compute_free_slots(self) >= requested_slots);
  if (compute_slots_at_end(self) < requested_slots)
    move_used_bytes_at_beginning(self);
  free_at_end.ptr = self->last_used_slot + self->slot_dimension;
  free_at_end.len = requested_slots * self->slot_dimension;
  return free_at_end;
}
ucl_block_t
ucl_vector_get_free_block_at_beginning (ucl_vector_t self, size_t requested_slots)
{
  ucl_block_t	free_at_beg;
  assert(compute_free_slots(self) >= requested_slots);
  if (compute_slots_at_beginning(self) < requested_slots)
    move_used_bytes_at_end(self);
  free_at_beg.len = requested_slots * self->slot_dimension;
  free_at_beg.ptr = self->first_used_slot - free_at_beg.len;
  return free_at_beg;
}
void
ucl_vector_mark_as_used (ucl_vector_t self, ucl_block_t used_block)
{
  assert(0 == (used_block.len % self->slot_dimension));
  /* Assert that 'used_block' is adjacent to the block of used slots, at
     the beginning or at the end. */
  assert(((used_block.ptr + used_block.len) == self->first_used_slot) ||
	 ((self->last_used_slot + self->slot_dimension) == used_block.ptr));
  /* Assert that 'used_block' is inside the allocated block. */
  assert(self->first_allocated_slot <= used_block.ptr);
  assert((used_block.ptr + used_block.len) <= (self->last_allocated_slot + self->slot_dimension));
  if (used_block.ptr < self->first_used_slot)
    self->first_used_slot = used_block.ptr;
  else
    self->last_used_slot = used_block.ptr + used_block.len - self->slot_dimension;
}
void
ucl_vector_quick_sort (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);
  ASSERT_COMPAR_FUNCTION_IS_SET(self);
  ucl_quicksort(self->first_used_slot, compute_used_slots(self), self->slot_dimension, self->compar);
}
ucl_bool_t
ucl_vector_sorted (ucl_vector_t self)
{
  size_t		size   = ucl_vector_size(self);
  ucl_value_comparison_t	compar = self->compar;
  for (size_t i=1; i<size; ++i) {
    ucl_value_t	a = { .bytes = ucl_vector_index_to_slot(self, i-1) };
    ucl_value_t	b = { .bytes = ucl_vector_index_to_slot(self, i)   };
    if (0 < compar.func(compar.data, a, b))
      return 0;
  }
  return 1;
}
ucl_block_t
ucl_vector_block_from_range (const ucl_vector_t self, const ucl_range_t range)
{
  ucl_block_t	block;
  assert(ucl_range_min(range) <= ucl_range_max(range));
  assert(ucl_range_min(range) < ucl_vector_size(self));
  assert(ucl_range_max(range) < ucl_vector_size(self));
  block.ptr = ucl_vector_index_to_slot(self, ucl_range_min(range));
  block.len = ucl_range_size(range) * self->slot_dimension;
  return block;
}
ucl_range_t
ucl_vector_range_from_block(const ucl_vector_t self, const ucl_block_t block)
{
  ucl_range_t	range;
  assert(block.ptr >= self->first_used_slot);
  assert(block.len <= (self->slot_dimension * ucl_vector_size(self)));
  assert((block.ptr + block.len) <= (self->last_used_slot + self->slot_dimension));
  assert(0 == ((block.ptr - self->first_used_slot) % self->slot_dimension));
  assert(0 == ((block.ptr + block.len - self->first_used_slot) % self->slot_dimension));
  /* Ranges are inclusive!!! */
  ucl_range_set_min_max(range,
			ucl_vector_slot_to_index(self, block.ptr),
			ucl_vector_slot_to_index(self, (block.ptr + block.len) - self->slot_dimension));
  return range;
}


/** ------------------------------------------------------------
 ** Whole vector iteration functions.
 ** ----------------------------------------------------------*/

void
ucl_vector_iterator_forward (const ucl_vector_t self, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self))? self->first_used_slot : NULL;
  iterator->next	= iterator_forward_next;
}
void
ucl_vector_iterator_backward (const ucl_vector_t self, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self))? self->last_used_slot : NULL;
  iterator->next	= iterator_backward_next;
}
static void
iterator_forward_next (ucl_iterator_t iterator)
{
  iterator_next(iterator, 1);
}
static void
iterator_backward_next (ucl_iterator_t iterator)
{
  iterator_next(iterator, 0);
}
static void
iterator_next (ucl_iterator_t iterator, ucl_bool_t forward)
{
  const ucl_vector_tag_t * self = iterator->container;
  uint8_t *		current_p = iterator->iterator;
  ASSERT_INVARIANTS(self);
  if (forward) {
    if (current_p < self->last_used_slot) {
      iterator->iterator = current_p + self->slot_dimension;
      goto end;
    }
  } else {
    if (current_p > self->first_used_slot) {
      iterator->iterator = current_p - self->slot_dimension;
      goto end;
    }
  }
  iterator->iterator = NULL;
 end:
  ASSERT_INVARIANTS(self);
}


/** ------------------------------------------------------------
 ** Range iteration functions.
 ** ----------------------------------------------------------*/

void
ucl_vector_iterator_range_forward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  assert(ucl_vector_range_is_valid(self, range));
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self) && ucl_range_size(range))?
    ucl_vector_index_to_slot(self, ucl_range_min(range)) : NULL;
  iterator->internal1.pointer	= ucl_vector_index_to_slot(self, ucl_range_max(range));
  iterator->next	= iterator_range_forward_next;
}
void
ucl_vector_iterator_range_backward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  assert(ucl_vector_range_is_valid(self, range));
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self) && ucl_range_size(range))?
    ucl_vector_index_to_slot(self, ucl_range_max(range)) : NULL;
  iterator->internal1.pointer	= ucl_vector_index_to_slot(self, ucl_range_min(range));
  iterator->next	= iterator_range_backward_next;
}
static void
iterator_range_forward_next (ucl_iterator_t iterator)
{
  iterator_range_next(iterator, 1);
}
static void
iterator_range_backward_next (ucl_iterator_t iterator)
{
  iterator_range_next(iterator, 0);
}
static void
iterator_range_next (ucl_iterator_t iterator, ucl_bool_t forward)
{
  const ucl_vector_tag_t * 	self = iterator->container;
  uint8_t *			current_p = iterator->iterator;
  uint8_t *			last_p = iterator->internal1.pointer;
  ASSERT_INVARIANTS(self);
  if (forward) {
    if (current_p < last_p) {
      iterator->iterator = current_p + self->slot_dimension;
      goto end;
    }
  } else {
    if (current_p > last_p) {
      iterator->iterator = current_p - self->slot_dimension;
      goto end;
    }
  }
  iterator->iterator = NULL;
 end:
  ASSERT_INVARIANTS(self);
}


/** ------------------------------------------------------------
 ** Memory functions: enlarging.
 ** ----------------------------------------------------------*/

ucl_bool_t
ucl_vector_will_enlarge (ucl_vector_t self)
{
  return
    (self->first_used_slot == self->first_allocated_slot) &&
    (self->last_used_slot  == self->last_allocated_slot);
}
static size_t
compute_enlarged_size_in_bytes (ucl_vector_t self)
{
  size_t	number_of_allocated_bytes, rest;
  number_of_allocated_bytes = compute_allocated_bytes(self);
  debug("old size: %d", number_of_allocated_bytes);
  rest = number_of_allocated_bytes % self->step_up;
  number_of_allocated_bytes += (rest)? rest : self->step_up;
  debug("new size: %d", number_of_allocated_bytes);
  return number_of_allocated_bytes;
}
size_t
ucl_vector_enlarged_size (ucl_vector_t self)
{
  return (compute_enlarged_size_in_bytes(self) / self->slot_dimension);
}
void
ucl_vector_enlarge (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);
  if (ucl_vector_will_enlarge(self)) {
    /* NOTE: the hash table expects  that the used bytes are not moved
       around when reallocating. */
    reallocate_block(self, compute_enlarged_size_in_bytes(self));
    ASSERT_INVARIANTS(self);
  }
}
void
ucl_vector_enlarge_for_slots (ucl_vector_t self, size_t required_free_slots)
{
  ASSERT_INVARIANTS(self);
  size_t	number_of_allocated_bytes, rest;
  size_t	number_of_free_slots, number_of_slots_to_allocate;
  debug("requested slots: %d", required_free_slots);
  PRINT_SLOT_INSPECTION(self, "before reallocation");
  assert(ucl_vector_number_of_free_slots(self) ==
	 ((compute_bytes_at_beginning(self) + compute_bytes_at_end(self))
	  / self->slot_dimension));
  number_of_free_slots = ucl_vector_number_of_free_slots(self);
  debug("reallocating? %s", debug_question(number_of_free_slots < required_free_slots));
  if (number_of_free_slots < required_free_slots) {
    number_of_slots_to_allocate = required_free_slots - number_of_free_slots;
    debug("slots to be allocated: %d", number_of_slots_to_allocate);
    number_of_allocated_bytes   = compute_allocated_bytes(self);
    debug("old byte size: %d", number_of_allocated_bytes);

    number_of_allocated_bytes += number_of_slots_to_allocate * self->slot_dimension;
    rest = number_of_allocated_bytes % self->step_up;
    number_of_allocated_bytes += (rest)? rest : self->step_up;
    debug("new byte size (adjusted to step_up): %d", number_of_allocated_bytes);

    reallocate_block(self, number_of_allocated_bytes);
  }
  PRINT_SLOT_INSPECTION(self, "after reallocation");
  assert(ucl_vector_number_of_free_slots(self) ==
	 ((compute_bytes_at_beginning(self) + compute_bytes_at_end(self))
	  / self->slot_dimension));
  ASSERT_INVARIANTS(self);
  debug("done");
}
void
ucl_vector_enlarge_for_range (ucl_vector_t self, ucl_range_t range)
{
  size_t		size = ucl_vector_size(self);
  ucl_vector_index_t	max  = ucl_range_max(range);
  if (size <= max) {
    size_t	required_free_slots = max - size + 1;
    ucl_vector_enlarge_for_slots(self, required_free_slots);
  }
}


/** ------------------------------------------------------------
 ** Memory functions: restricting.
 ** ----------------------------------------------------------*/

ucl_bool_t
ucl_vector_will_restrict (ucl_vector_t self)
{
  return (compute_free_bytes(self) >= self->step_down);
}
static size_t
compute_restricted_size_in_bytes (ucl_vector_t self)
{
  size_t number_of_allocated_bytes	= compute_allocated_bytes(self);
  size_t new_number_of_allocated_bytes	= number_of_allocated_bytes - self->step_down;
  size_t rest = number_of_allocated_bytes % self->step_up;
  new_number_of_allocated_bytes += (rest)? rest : self->step_up;
  /* The following condition can  be false when: 'step_up >= step_down'.
     This  is why  'step_down' is  normalised  to a  value greater  than
     'step_up' by the constructor. */
  assert(new_number_of_allocated_bytes < number_of_allocated_bytes);
  return new_number_of_allocated_bytes;
}
size_t
ucl_vector_restricted_size (ucl_vector_t self)
{
  return (compute_restricted_size_in_bytes(self) / self->slot_dimension);
}
void
ucl_vector_restrict (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);
  if (ucl_vector_will_restrict(self)) {
    size_t	number_of_allocated_bytes	= compute_allocated_bytes(self);
    size_t	new_number_of_allocated_bytes	= compute_restricted_size_in_bytes(self);
    /* NOTE: the  hash table relies  on this operation; it  expects that
       the used bytes are placed at the beginning when reallocating. */
    move_used_bytes_leave_requested_at_end(self,
					   number_of_allocated_bytes - new_number_of_allocated_bytes);
    reallocate_block(self, new_number_of_allocated_bytes);
    move_used_bytes_at_pad_area_beginning(self);
    ASSERT_INVARIANTS(self);
  }
}


/** ------------------------------------------------------------
 ** Memory functions: miscellaneous operations.
 ** ----------------------------------------------------------*/

void
ucl_vector_set_memory_to_zero (ucl_vector_t self)
{
  memset(self->first_allocated_slot, '\0', compute_allocated_bytes(self));
}
size_t
ucl_vector_number_of_free_slots (const ucl_vector_t self)
{
  return (compute_allocated_bytes(self) - \
	  compute_used_bytes(self)) / self->slot_dimension;
}
static void
reallocate_block (ucl_vector_t self, size_t new_number_of_allocated_bytes)
{
  size_t	offset_of_first_used_slot, offset_of_last_used_slot;
  uint8_t *	p;
  assert(0 == (new_number_of_allocated_bytes % self->slot_dimension));
  p = self->first_allocated_slot;
  offset_of_first_used_slot = self->first_used_slot - p;
  offset_of_last_used_slot  = self->last_used_slot  - p;
  self->allocator.alloc(self->allocator.data, &p, new_number_of_allocated_bytes);
  assert(self->first_allocated_slot);
  self->first_allocated_slot	= p;
  self->first_used_slot		= p + offset_of_first_used_slot;
  self->last_used_slot		= p + offset_of_last_used_slot;
  self->last_allocated_slot	= p + new_number_of_allocated_bytes - self->slot_dimension;
  ASSERT_INVARIANTS(self);
}


/** ------------------------------------------------------------
 ** Find functions.
 ** ----------------------------------------------------------*/

void *
ucl_vector_find (const ucl_vector_t self, const void * data_p)
{
  ucl_value_t	inner;
  ucl_value_t	outer = { .bytes = (uint8_t *)data_p };
  ASSERT_INVARIANTS(self);
  ASSERT_COMPAR_FUNCTION_IS_SET(self);
  assert(data_p);
  if (ucl_vector_size(self)) {
    ucl_value_comparison_t	compar = self->compar;
    for (inner.bytes = self->first_used_slot;
	 inner.bytes <= self->last_used_slot;
	 inner.bytes += self->slot_dimension)
      if (0 == compar.func(compar.data, outer, inner))
	return inner.bytes;
  }
  return NULL;
}
void *
ucl_vector_sort_find (const ucl_vector_t self, const void * data_p)
{
  ucl_vector_index_t	size;
  ASSERT_INVARIANTS(self); ASSERT_COMPAR_FUNCTION_IS_SET(self); assert(data_p);
  size = ucl_vector_size(self);
  if (! size) { return NULL; }
  return (size <= SLOT_NUMBER_THAT_TRIGGERS_BINARY_OVER_LINEAR_SEARCH)? \
    ucl_vector_find(self, data_p) : ucl_vector_binary_search(self, data_p);
}
void *
ucl_vector_binary_search (const ucl_vector_t self, const void * data_p)
{
  size_t	size, i, lower_index_limit, upper_index_limit;
  size_t	current_index, new_current_index;
  ucl_value_t	inner;
  ucl_value_t	outer = { .ptr = (uint8_t *)data_p };
  int		match;
  ucl_value_comparison_t	compar = self->compar;
  size = ucl_vector_size(self);
  lower_index_limit	= 0;
  upper_index_limit	= size-1;
  /* Doing shift-right-1 on 2*N will return N, on 2*N+1 will return N,
     too (tested with the [expr] command of TCL). */
  current_index = size >> 1;
  for (i=0; i<size; ++i)    {
    inner.bytes = ucl_vector_index_to_slot(self, current_index);
    match = compar.func(compar.data, outer, inner);
    if (0 == match)
      return inner.bytes;
    else if (match < 0) {
      if (0 == current_index) return NULL;
      upper_index_limit = current_index - 1;
      new_current_index = lower_index_limit + ((current_index - lower_index_limit) >> 1);
      current_index     = (new_current_index < current_index)? new_current_index : (new_current_index-1);
      if (current_index < lower_index_limit) return NULL;
    } else { /* match > 0 */
      if (UINT_MAX == current_index) return NULL;
      lower_index_limit = current_index + 1;
      new_current_index = current_index + ((upper_index_limit - current_index) >> 1);
      current_index     = (new_current_index > current_index)? new_current_index : (new_current_index+1);
      if (current_index > upper_index_limit) return NULL;
    }
  }
  return NULL;
}

/** ------------------------------------------------------------
 ** High level functions: stack/queue.
 ** ----------------------------------------------------------*/

ucl_data_t
ucl_vector_push_back (ucl_vector_t self)
{
  ucl_data_t	p;
  ucl_vector_enlarge(self);
  p = ucl_vector_index_to_new_slot(self, ucl_vector_size(self));
  p = ucl_vector_insert(self, p);
  return p;
}
ucl_data_t
ucl_vector_push_front (ucl_vector_t self)
{
  ucl_data_t	p;
  ucl_vector_enlarge(self);
  p = ucl_vector_index_to_new_slot(self, 0);
  p = ucl_vector_insert(self, p);
  return p;
}
void
ucl_vector_pop_back (ucl_vector_t self)
{
  if (ucl_vector_size(self)) {
    ucl_vector_erase(self, ucl_vector_back(self));
    ucl_vector_restrict(self);
  }
}
void
ucl_vector_pop_front (ucl_vector_t self)
{
  if (ucl_vector_size(self)) {
    ucl_vector_erase(self, ucl_vector_front(self));
    ucl_vector_restrict(self);
  }
}


/** ------------------------------------------------------------
 ** High level functions: appending stuff.
 ** ----------------------------------------------------------*/

void
ucl_vector_append_block (ucl_vector_t self, const ucl_block_t block)
{
  debug("number of used slots %d", ucl_vector_size(self));
  debug("request to append a block of len %d", block.len);
  debug("number of slots to append %d", block.len/self->slot_dimension);
  debug("vector slot dimension %d, old vector size in slots %d",
	self->slot_dimension, compute_used_slots(self));
  debug("len %% slot_dimension = %d", (block.len % self->slot_dimension));
  assert(0 == (block.len % self->slot_dimension));
  ASSERT_INVARIANTS(self);
  if (block.len > 0) {
    uint8_t *	first_new_slot;
    size_t	number_of_new_slots = block.len / self->slot_dimension;
    debug("enlarging for %d new slots", number_of_new_slots);
    ucl_vector_enlarge_for_slots(self, number_of_new_slots);
    assert(compute_free_bytes(self) >= block.len);
    debug("number of used slots %d", ucl_vector_size(self));
    debug("making space at end");
    move_used_bytes_leave_requested_at_end(self, block.len);
    debug("number of free slots %d", ucl_vector_number_of_free_slots(self));
    debug("number of slots at beginning %d", compute_bytes_at_beginning(self) / self->slot_dimension);
    debug("number of slots at end %d", compute_bytes_at_end(self) / self->slot_dimension);
    assert(compute_bytes_at_end(self) >= block.len);
    first_new_slot = self->last_used_slot + self->slot_dimension;
    memcpy(first_new_slot, block.ptr, block.len);
    debug("number of new bytes %d", number_of_new_slots * self->slot_dimension);
    debug("old offset of last used %d",
	  (self->last_used_slot - self->first_used_slot) / self->slot_dimension);
    self->last_used_slot += number_of_new_slots * self->slot_dimension;
    debug("new offset of last used %d",
	  (self->last_used_slot - self->first_used_slot) / self->slot_dimension);
  }
  debug("completed append");
  PRINT_SLOT_INSPECTION(self,"after append block");
  PRINT_POINTER_INSPECTION(self,"after append block");
  ASSERT_INVARIANTS(self);
}
void
ucl_vector_append (ucl_vector_t target, const ucl_vector_t source)
{
  assert(ucl_vector_slot_dimension(target) == ucl_vector_slot_dimension(source));
  ucl_vector_append_block(target, ucl_vector_get_data_block(source));
}
void
ucl_vector_append_range (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range)
{
  ucl_block_t		block;
  assert(ucl_vector_slot_dimension(target) == ucl_vector_slot_dimension(source));
  assert(ucl_vector_range_is_valid(source, range));
  ucl_block_set(&block,
		ucl_vector_index_to_slot(source, ucl_range_min(range)),
		(ucl_range_size(range) * ucl_vector_slot_dimension(source)));
  ucl_vector_append_block(target, block);
}
void
ucl_vector_append_more (ucl_vector_t target, const ucl_vector_t source, ...)
{
  ucl_vector_tag_t * 	other_source;
  va_list		ap;
  ucl_vector_append(target, source);
  va_start(ap, source);
  for (other_source = va_arg(ap, ucl_vector_t); (NULL != other_source);
       other_source = va_arg(ap, ucl_vector_t))
    ucl_vector_append(target, other_source);
  va_end(ap);
}
void
ucl_vector_append_more_from_array (ucl_vector_t target, const ucl_vector_array_t * vectors)
{
  size_t	total_number_of_new_slots = 0;
  for (size_t i=0; i<vectors->number_of_vectors; ++i)
    total_number_of_new_slots += ucl_vector_size(vectors->vectors[i]);
  ucl_vector_enlarge_for_slots(target, total_number_of_new_slots);
  for (size_t i=0; i<vectors->number_of_vectors; ++i)
    ucl_vector_append(target, vectors->vectors[i]);
}


/** ------------------------------------------------------------
 ** High level functions: insertions.
 ** ----------------------------------------------------------*/

void
ucl_vector_insert_block	(ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block)
{
  uint8_t *	insertion_slot;
  size_t	bytes_at_beginning	= compute_bytes_at_beginning(target);
  size_t	bytes_at_end		= compute_bytes_at_end(target);
  ucl_vector_enlarge_for_slots(target, block.len / target->slot_dimension);
  insertion_slot = ucl_vector_index_to_new_slot(target, offset);
  assert(insertion_slot);
  if (block.len <= bytes_at_beginning) {
    uint8_t *	p = target->first_used_slot - block.len;
    size_t	size_of_block_to_move = insertion_slot - target->first_used_slot;
    memmove(p, target->first_used_slot, size_of_block_to_move);
    target->first_used_slot = p;
    insertion_slot = p + size_of_block_to_move;
  } else if (block.len <= bytes_at_end) {
    uint8_t *	p = insertion_slot + block.len;
    size_t	size_of_block_to_move = target->last_used_slot + target->slot_dimension - insertion_slot;
    memmove(p, insertion_slot, size_of_block_to_move);
    target->last_used_slot += size_of_block_to_move;
  } else {
    size_t	delta = block.len - bytes_at_beginning;
    memmove(target->first_allocated_slot, target->first_used_slot,
	    insertion_slot - target->first_used_slot);
    target->first_used_slot = target->first_allocated_slot;
    {
      size_t	size_of_block_to_move = target->last_used_slot + target->slot_dimension - insertion_slot;
      memmove(insertion_slot + delta, insertion_slot, size_of_block_to_move);
      target->last_used_slot += delta;
    }
    insertion_slot -= bytes_at_beginning;
  }
  memcpy(insertion_slot, block.ptr, block.len);
}
void
ucl_vector_insert_range (ucl_vector_t V, ucl_vector_index_t offset,
			 const ucl_vector_t A, ucl_range_t range)
{
  ucl_block_t	block;
  assert(V->slot_dimension == A->slot_dimension);
  assert(ucl_vector_range_is_valid(A, range));
  block.ptr = ucl_vector_index_to_slot(A, ucl_range_min(range));
  block.len = A->slot_dimension * ucl_range_size(range);
  ucl_vector_insert_block(V, offset, block);
}
void
ucl_vector_insert_vector (ucl_vector_t T, ucl_vector_index_t offset, const ucl_vector_t S)
{
  ucl_block_t	block;
  assert(T->slot_dimension == S->slot_dimension);
  block = ucl_vector_get_data_block(S);
  ucl_vector_insert_block(T, offset, block);
}


/** ------------------------------------------------------------
 ** High level functions: erasures.
 ** ----------------------------------------------------------*/

void
ucl_vector_erase_range (ucl_vector_t self, ucl_range_t index_range)
{
  ucl_byte_pointer_range_t	slot_range;
  ucl_bool_t			room_at_the_end, room_at_the_beginning;
  ASSERT_INVARIANTS(self);
  assert(ucl_vector_range_is_valid(self, index_range));
  if (0 == ucl_range_size(index_range))
    return;
  debug("range first index %d, range last index %d",
	ucl_range_min(index_range), ucl_range_max(index_range));
  ucl_range_set_min_max(slot_range,
			ucl_vector_index_to_slot(self,ucl_range_min(index_range)),
			ucl_vector_index_to_slot(self,ucl_range_max(index_range)));
  debug("range byte size %d", ucl_range_size(slot_range) + self->slot_dimension - 1);
  room_at_the_beginning	= there_is_room_at_the_beginning(self);
  room_at_the_end	= there_is_room_at_the_end(self);
  if (room_at_the_beginning && room_at_the_end) {
    debug("there is room at the beginning and at the end");
    debug("bytes at beg %d, bytes at end %d",
	  compute_bytes_at_beginning(self), compute_bytes_at_end(self));
    if (compute_bytes_at_beginning(self) < compute_bytes_at_end(self)) {
      goto move_head_data_toward_the_end;
    } else {
      goto move_tail_data_toward_the_beginning;
    }
  } else if (room_at_the_beginning) {
    debug("there is room at the beginning, bytes %d", compute_bytes_at_beginning(self));
    goto move_tail_data_toward_the_beginning;
  } else {
    debug("there is room at the end, bytes %d", compute_bytes_at_end(self));
    goto move_head_data_toward_the_end;
  }
 exit:
  ucl_vector_restrict(self);
  return;
 move_tail_data_toward_the_beginning:
  erase_range_moving_tail_data_toward_the_beginning(self, slot_range);
  goto exit;
 move_head_data_toward_the_end:
  erase_range_moving_head_data_toward_the_end(self, slot_range);
  goto exit;
}


/** ------------------------------------------------------------
 ** High level functions: accessors.
 ** ----------------------------------------------------------*/

void
ucl_vector_copy_range (ucl_vector_t target, ucl_vector_index_t position,
		       ucl_vector_t source, ucl_range_t source_range)
{
  ucl_block_t	block;
  assert(target->slot_dimension == target->slot_dimension);
  assert(ucl_vector_index_is_valid(target, position));
  assert(ucl_vector_range_is_valid(source, source_range));
  assert((ucl_vector_size(target) - position) >= ucl_range_size(source_range));
  block = ucl_vector_block_from_range(source, source_range);
  ucl_vector_set_block(target, position, block);
}
void
ucl_vector_set_block (ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source)
{
  void *	slot;
  assert(ucl_vector_index_is_valid(target, position));
  assert((source.len / target->slot_dimension) <= (ucl_vector_size(target) - position));
  slot = ucl_vector_index_to_slot(target, position);
  memcpy(slot, source.ptr, source.len);
}
void
ucl_vector_get_block (ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source)
{
  void *	slot;
  assert(ucl_vector_index_is_valid(source, position));
  assert((target.len / source->slot_dimension) <= (ucl_vector_size(source) - position));
  slot = ucl_vector_index_to_slot(source, position);
  memcpy(target.ptr, slot, target.len);
}


/** ------------------------------------------------------------
 ** High level functions: comparing.
 ** ----------------------------------------------------------*/

int
ucl_vector_compare_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  ucl_vector_index_t	i;
  ucl_value_comparison_t	compar;
  int			result=0;
  if (ucl_range_size(ra) > ucl_range_size(rb))
    ucl_range_set_size_on_min(ra, ucl_range_size(rb));
  compar = a->compar;
  for (i=0; i<ucl_range_size(ra); ++i) {
    ucl_value_t pa = { .ptr = ucl_vector_index_to_slot(a, ucl_range_min(ra)+i) };
    ucl_value_t pb = { .ptr = ucl_vector_index_to_slot(b, ucl_range_min(rb)+i) };
    debug("%d %d", *(int *)pa.ptr, *(int *)pb.ptr);
    result = compar.func(compar.data, pa, pb);
    if (result != 0)
      return result;
  }
  return result;
}
int
ucl_vector_compare (const ucl_vector_t a, const ucl_vector_t b)
{
  ucl_range_t	range_a, range_b;
  ucl_range_set_min_size(range_a, 0, ucl_vector_size(a));
  ucl_range_set_min_size(range_b, 0, ucl_vector_size(b));
  return ucl_vector_compare_range(a, range_a, b, range_b);
}
ucl_bool_t
ucl_vector_equal (const ucl_vector_t a, const ucl_vector_t b)
{
  return ((ucl_vector_size(a) == ucl_vector_size(b)) && (ucl_vector_compare(a, b) == 0));
}


/** ------------------------------------------------------------
 ** High level functions: for each.
 ** ----------------------------------------------------------*/

void
ucl_vector_for_each (ucl_callback_t callback, const ucl_vector_t self)
{
  ucl_iterator_t	I;
  ucl_value_t		slot;
  for (ucl_vector_iterator_forward(self, I); ucl_iterator_more(I); ucl_iterator_next(I)) {
    slot.ptr = ucl_iterator_ptr(I);;
    ucl_callback_application_function(callback, slot);
  }
}
void
ucl_vector_for_each_in_range (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self)
{
  ucl_value_t	data;
  void *	slot;
  assert(ucl_vector_range_is_valid(self, range));
  slot = ucl_vector_index_to_slot(self, ucl_range_min(range));
  for (size_t i = ucl_range_min(range); i <= ucl_range_max(range);
       ++i, slot = ucl_vector_increment_slot(self, slot)) {
    data.ptr = slot;
    ucl_callback_application_function(callback, data);
  }
}


/** ------------------------------------------------------------
 ** High level functions: multiple for each.
 ** ----------------------------------------------------------*/

void
ucl_vector_for_each_multiple_from_array (ucl_callback_t callback, const ucl_vector_array_t * vectors)
{
  void *		slots[vectors->number_of_vectors];
  ucl_iterator_t	iterators[vectors->number_of_vectors];
  ucl_array_of_pointers_t slots_array = {
    .number_of_slots	= vectors->number_of_vectors,
    .slots		= slots
  };
  ucl_value_t		data = { .ptr = &slots_array };
  size_t		i;
  for (i=0; i<vectors->number_of_vectors; ++i)
    ucl_vector_iterator_forward(vectors->vectors[i], iterators[i]);
  for (slots_array.data.unum = 0;; ++slots_array.data.unum) {
    for (i=0; i<vectors->number_of_vectors; ++i)
      if (! ucl_iterator_more(iterators[i]))
	goto exit;
    for (i=0; i<slots_array.number_of_slots; ++i)
      slots[i] = ucl_iterator_ptr(iterators[i]);
    ucl_callback_apply(callback, data);
    for (i=0; i<vectors->number_of_vectors; ++i)
      ucl_iterator_next(iterators[i]);
  }
 exit:
  return;
}
void
ucl_vector_for_each_multiple (ucl_callback_t callback, const ucl_vector_t first, ...)
{
  size_t	counter = 1;
  va_list	ap, save;
#ifdef __va_copy
  __va_copy(save, ap);
#else
  save = ap;
#endif
  /* Count how many vectors are there. */
  {
    ucl_vector_tag_t * 	V;
    va_start(save, first);
    for (V = va_arg(save, void *); V; V = va_arg(save, void *))
      ++counter;
    va_end(save);
  }
  /* Store the vectors in an array, then do the iteration. */
  {
    ucl_vector_tag_t * 	vectors[counter];
    ucl_vector_array_t	array = {
      .number_of_vectors	= counter,
      .vectors			= vectors
    };
    vectors[0] = (ucl_vector_tag_t *)first;
    va_start(ap, first);
    for (size_t i=1; i<counter; ++i)
      vectors[i] = va_arg(ap, void *);
    va_end(ap);
    ucl_vector_for_each_multiple_from_array(callback, &array);
  }
}


/** ------------------------------------------------------------
 ** High level functions: mapping.
 ** ----------------------------------------------------------*/

void
ucl_vector_map (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self)
{
  void *			slots[2];
  ucl_array_of_pointers_t	slots_array = {
    .number_of_slots	= 2,
    .slots		= slots
  };
  ucl_value_t			data = { .ptr = &slots_array };
  ucl_iterator_t		I;
  for (ucl_vector_iterator_forward(self, I), slots_array.data.unum = 0; ucl_iterator_more(I);
       ucl_iterator_next(I), ++slots_array.data.unum) {
    slots[0] = ucl_vector_push_back(result);
    slots[1] = ucl_iterator_ptr(I);
    ucl_callback_apply(callback, data);
  }
}
void
ucl_vector_map_range (ucl_vector_t result, ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self)
{
  void *			slots[2];
  ucl_array_of_pointers_t	slots_array = {
    .number_of_slots	= 2,
    .slots		= slots
  };
  ucl_value_t			data = { .ptr = &slots_array };
  ucl_iterator_t		I;
  assert(ucl_vector_range_is_valid(self, range));
  for (ucl_vector_iterator_range_forward(self, range, I), slots_array.data.unum = ucl_range_min(range);
       ucl_iterator_more(I); ucl_iterator_next(I), ++slots_array.data.unum) {
    slots[0] = ucl_vector_push_back(result);
    slots[1] = ucl_iterator_ptr(I);
    ucl_callback_apply(callback, data);
  }
}


/** ------------------------------------------------------------
 ** High level functions: multiple map.
 ** ----------------------------------------------------------*/

void
ucl_vector_map_multiple_from_array (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_array_t * vectors)
{
  size_t		number_of_slots = 1+vectors->number_of_vectors;
  void *		slots[number_of_slots];
  ucl_iterator_t	iterators[vectors->number_of_vectors];
  ucl_array_of_pointers_t slots_array = {
    .number_of_slots	= number_of_slots,
    .slots		= slots
  };
  ucl_value_t		data = { .ptr = &slots_array };
  size_t		i;
  for (i=0; i<vectors->number_of_vectors; ++i)
    ucl_vector_iterator_forward(vectors->vectors[i], iterators[i]);
  for (slots_array.data.unum = 0;; ++slots_array.data.unum) {
    for (i=0; i<vectors->number_of_vectors; ++i)
      if (! ucl_iterator_more(iterators[i]))
	goto exit;
    slots[0] = ucl_vector_push_back(result);
    for (i=0; i<vectors->number_of_vectors; ++i)
      slots[1+i] = ucl_iterator_ptr(iterators[i]);
    ucl_callback_apply(callback, data);
    for (i=0; i<vectors->number_of_vectors; ++i)
      ucl_iterator_next(iterators[i]);
  }
 exit:
  return;
}
void
ucl_vector_map_multiple (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...)
{
  size_t	counter = 1;
  va_list	ap, save;
#ifdef __va_copy
  __va_copy(save, ap);
#else
  save = ap;
#endif
  { /* Count how many vectors are there. */
    va_start(save, first);
    for (ucl_vector_tag_t * V = va_arg(save, void *); V; V = va_arg(save, void *))
      ++counter;
    va_end(save);
  }
  { /* Store the vectors in an array, then do the iteration. */
    ucl_vector_tag_t *		vectors[counter];
    const ucl_vector_array_t	array = {
      .number_of_vectors	= counter,
      .vectors			= vectors
    };
    vectors[0] = (ucl_vector_tag_t *)first;
    va_start(ap, first);
    for (size_t i=1; i<counter; ++i)
      vectors[i] = va_arg(ap, void *);
    va_end(ap);
    ucl_vector_map_multiple_from_array(result, callback, &array);
  }
}


/** ------------------------------------------------------------
 ** Private module functions.
 ** ----------------------------------------------------------*/

static inline void
set_pointers_for_empty_vector	(ucl_vector_t self)
{
  uint8_t *	potential_first_used_slot;

  /* do not assert emptyness here,  because this function may be used to
     force the emptyness state */
  potential_first_used_slot = self->first_allocated_slot + self->size_of_padding_area;
  if (potential_first_used_slot < self->last_allocated_slot) {
    self->first_used_slot = potential_first_used_slot;
    self->last_used_slot  = self->first_used_slot - self->slot_dimension;
    ASSERT_FIRST_SLOT_IS_AT_PAD_OFFSET(self);
  } else {
    self->first_used_slot = self->first_allocated_slot;
    self->last_used_slot  = self->first_used_slot - self->slot_dimension;
  }
  ASSERT_VECTOR_IS_EMPTY(self);
}
static inline ucl_bool_t
vector_is_empty (const ucl_vector_t self)
{
  return (self->first_used_slot > self->last_used_slot);
}
static inline ucl_bool_t
there_is_room_at_the_end (const ucl_vector_t self)
{
  if (! vector_is_empty(self)) {
    return (self->last_used_slot < self->last_allocated_slot);
  } else { /* vector is empty */
    return (self->first_used_slot <= self->last_allocated_slot);
  }
}
static inline ucl_bool_t
there_is_room_at_the_beginning (const ucl_vector_t self)
{
  return (self->first_allocated_slot < self->first_used_slot);
}
static inline size_t
compute_bytes_at_beginning (const ucl_vector_t self)
{
  return (self->first_used_slot - self->first_allocated_slot);
}
static inline size_t
compute_bytes_at_end (const ucl_vector_t self)
{
  if (! vector_is_empty(self)) {
    /*
      (last_allocated_slot + slot_dimension) - (last_used_slot + slot_dimension) =
      (last_allocated_slot - last_used_slot)
    */
    return (self->last_allocated_slot - self->last_used_slot);
  } else { /* vector is empty */
    /*
      (last_allocated_slot + slot_dimension) - (first_used_slot + slot_dimension)
      + slot_dimension =
      (last_allocated_slot - first_used_slot + slot_dimension)
    */
    return (self->last_allocated_slot - self->first_used_slot + self->slot_dimension);
  }
}
static inline size_t
compute_used_bytes (const ucl_vector_t self)
{
  return ((self->last_used_slot + self->slot_dimension) - self->first_used_slot);
}
static inline size_t
compute_allocated_bytes (const ucl_vector_t self)
{
  return ((self->last_allocated_slot + self->slot_dimension) \
	  - self->first_allocated_slot);
}
static inline size_t
compute_free_bytes (const ucl_vector_t self)
{
  return (compute_allocated_bytes(self) - compute_used_bytes(self));
}
static inline size_t
compute_allocated_slots (const ucl_vector_t self)
{
  return (compute_allocated_bytes(self) / self->slot_dimension);
}
static inline size_t
compute_used_slots (const ucl_vector_t self)
{
  return (compute_used_bytes(self) / self->slot_dimension);
}
static inline size_t
compute_free_slots (const ucl_vector_t self)
{
  return (compute_free_bytes(self) / self->slot_dimension);
}
static inline size_t
compute_slots_at_end (const ucl_vector_t self)
{
  return (compute_bytes_at_end(self) / self->slot_dimension);
}
static inline size_t
compute_slots_at_beginning (const ucl_vector_t self)
{
  return (compute_bytes_at_beginning(self) / self->slot_dimension);
}


static void
move_used_bytes_at_pad_area_beginning (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);
  if (vector_is_empty(self)) {
    set_pointers_for_empty_vector(self);
    ASSERT_INVARIANTS(self);
  } else {
    uint8_t *	p;
    size_t	number_of_free_bytes = compute_free_bytes(self);
    if (0 < number_of_free_bytes) {
      if (self->size_of_padding_area < number_of_free_bytes) {
	p = self->first_allocated_slot + self->size_of_padding_area;
      } else {
	/* Leave half of the free slots at the beginning and half at the
	   end. */
	p = self->first_allocated_slot +
	  (self->slot_dimension * (compute_free_slots(self) >> 1));
      }
      move_used_bytes_at_byte_pointer(self, p);
    }
    ASSERT_INVARIANTS(self);
  }
}


static void
move_used_bytes_leave_requested_at_end (ucl_vector_t self, size_t bytes_requested_at_end)
{
  assert(bytes_requested_at_end > 0);
  if (vector_is_empty(self)) {
    ASSERT_INVARIANTS(self);
    set_pointers_for_empty_vector(self);
    if (bytes_requested_at_end > compute_bytes_at_end(self)) {
      self->first_used_slot = self->first_allocated_slot;
      self->last_used_slot  = self->first_used_slot - self->slot_dimension;
    }
  } else {
    size_t	number_of_free_bytes = compute_free_bytes(self);
    ASSERT_INVARIANTS(self);
    assert(bytes_requested_at_end <= number_of_free_bytes);
    debug("moving? %s",debug_question(bytes_requested_at_end > compute_bytes_at_end(self)));
    if (bytes_requested_at_end > compute_bytes_at_end(self)) {
      size_t	bytes_at_end_after_moving_on_padding;
      bytes_at_end_after_moving_on_padding = (number_of_free_bytes > self->size_of_padding_area)?
	(number_of_free_bytes - self->size_of_padding_area) : number_of_free_bytes;
      if (bytes_at_end_after_moving_on_padding > bytes_requested_at_end) {
	debug("moving bytes at pad area");
	move_used_bytes_at_pad_area_beginning(self);
	ASSERT_INVARIANTS(self);
      } else {
	uint8_t *	p;
	size_t		slots_requested_at_end = bytes_requested_at_end / self->slot_dimension;
	debug("leaving half slots at beginning and half at end");
	p = self->first_allocated_slot + self->slot_dimension * \
	  ((compute_free_slots(self) - slots_requested_at_end) >> 1);
	debug("moving bytes at %p", p);
	move_used_bytes_at_byte_pointer(self, p);
	PRINT_SLOT_INSPECTION(self,"after moving bytes");
	ASSERT_INVARIANTS(self);
      }
    }
  }
}


/** ------------------------------------------------------------
 ** Moving used slots.
 ** ----------------------------------------------------------*/

static inline void
move_used_bytes_at_beginning (ucl_vector_t self)
{
  move_used_bytes_at_byte_pointer(self, self->first_allocated_slot);
}
static inline void
move_used_bytes_at_end (ucl_vector_t self)
{
  uint8_t *	p = self->last_allocated_slot - compute_used_bytes(self) - self->slot_dimension;
  ASSERT_VALID_POINTER_AS_FIRST_USED(self, p);
  move_used_bytes_at_byte_pointer(self, p);
}
static inline void
move_used_bytes_at_index_offset (ucl_vector_t self, ucl_vector_index_t offset)
{
  uint8_t *	p = ucl_vector_index_to_slot(self, offset);
  ASSERT_VALID_POINTER_AS_FIRST_USED(self, p);
  move_used_bytes_at_byte_pointer(self, p);
}
static inline void
move_used_bytes_at_byte_pointer (ucl_vector_t self, uint8_t *p)
{
  if (p != self->first_used_slot) {
    ASSERT_VALID_POINTER_AS_FIRST_USED(self, p);
    memmove(p, self->first_used_slot, compute_used_bytes(self));
    self->last_used_slot += p - self->first_used_slot;
    self->first_used_slot = p;
    PRINT_POINTER_INSPECTION(self,"after moving bytes");
  }
}

/* end of file */
