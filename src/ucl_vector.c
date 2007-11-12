/* ucl_vector.c  --

   Part of: Useless Container Library (UCL)
   Author: Marco Maggi
   Content: the code for the vector container.
   Creation date: sometime in 2001

   Abstract:

	The  UCL vector container  is an  implementation of  the classic
	array, with  hysteresis in  memory allocation.  This  module was
	inspired by the book on C++  by Bjarne Stroustrup and by the STL
	C++ (Standard Template Library).

   Copyright (c) 2001, 2002, 2003, 2004, 2005, 2007 Marco Maggi

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


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#define UCL_DEBUGGING		0
#include "ucl_defines.h"
#include <limits.h>

/* stub module name vector */

#define SLOT_NUMBER_THAT_TRIGGERS_BINARY_OVER_LINEAR_SEARCH	8

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Assertion macros.
 ** ----------------------------------------------------------*/

/*
  V stands for a pointer to the vector struct, P stands for a pointer to
  a slot.
*/

#define ASSERT_VECTOR_IS_EMPTY(V)		assert(vector_is_empty(V));

#define ASSERT_VECTOR_POINTER(V)		assert((V))
#define ASSERT_MEMORY_ALLOCATED(V)		assert((V)->first_allocated_slot)

#define ASSERT_FRONT_MIN_FRONT_RELATION(V)	\
		assert((V)->first_allocated_slot <= (V)->first_used_slot)
#define ASSERT_BACK_MAX_BACK_RELATION(V)		\
		assert((V)->last_used_slot       <= (V)->last_allocated_slot)
#define ASSERT_FRONT_BACK_RELATION(V)		\
	assert(((V)->first_used_slot <= (V)->last_used_slot) || \
	       ((V)->first_used_slot - (V)->slot_dimension == (V)->last_used_slot))

#define ASSERT_FRONT_BACK_OFFSET_CORRECTNESS(V)			\
	assert(ucl_vector_size(V) * (V)->slot_dimension == \
	       (V)->last_used_slot - (V)->first_used_slot + (V)->slot_dimension)

#define ASSERT_NOT_UNDERFLOW_POINTER(V, P)	assert((V)->first_used_slot <= (P))
#define ASSERT_NOT_OVERFLOW_POINTER(V, P)	assert((V)->last_used_slot  >= (P))
#define ASSERT_NOT_NEW_OVERFLOW_POINTER(V, P)	\
		assert((V)->last_allocated_slot + (V)->slot_dimension >= (P));	\
		assert((V)->last_used_slot      + (V)->slot_dimension >= (P));

#define ASSERT_IS_POINTER_TO_USED_SLOT(V, P)		\
		assert(P);				\
		ASSERT_NOT_UNDERFLOW_POINTER(V, P);	\
		ASSERT_NOT_OVERFLOW_POINTER(V, P);
#define ASSERT_IS_POINTER_TO_NEW_SLOT(V, P)		\
		assert(P);				\
		ASSERT_NOT_UNDERFLOW_POINTER(V, P);	\
		ASSERT_NOT_NEW_OVERFLOW_POINTER(V, P);

#define ASSERT_COMPAR_FUNCTION_IS_SET(V)		assert((V)->compar.func)

#define ASSERT_FIRST_SLOT_IS_AT_PAD_OFFSET(V)		\
		assert((V)->first_used_slot ==		\
		       (V)->first_allocated_slot + (V)->size_of_padding_area)

#define ASSERT_THERE_IS_ROOM_AT_BEGINNING(V)		\
		assert(there_is_room_at_the_beginning(V));
#define ASSERT_THERE_IS_ROOM_AT_END(V)			\
		assert(there_is_room_at_the_end(V));
#define ASSERT_THERE_IS_ROOM(V)				\
		assert(there_is_room_at_the_beginning(V) || \
		       there_is_room_at_the_end(V));

#define ASSERT_INITIALISE_CONDITIONS(V)			\
	assert(V);					\
	assert((V)->slot_dimension);

#define ASSERT_INVARIANTS(V)				\
	ASSERT_VECTOR_POINTER(V);			\
	ASSERT_MEMORY_ALLOCATED(V);			\
	ASSERT_FRONT_MIN_FRONT_RELATION(V);		\
	ASSERT_FRONT_BACK_RELATION(V);			\
	ASSERT_BACK_MAX_BACK_RELATION(V);		\
	ASSERT_FRONT_BACK_OFFSET_CORRECTNESS(V);	\
	ASSERT_SLOTS(V);

#define ASSERT_SLOTS(V)								\
	assert((compute_allocated_slots(V) - compute_used_slots(V)) ==		\
	       (compute_slots_at_beginning(V) + compute_slots_at_end(V)));

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Module's function prototypes.
 ** ----------------------------------------------------------*/

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
static void *		pointer_to_slot_or_null	(const ucl_vector_t this,
						 ucl_vector_index_t index,
						 size_t slot_dimension_or_zero);

/* (Re)Allocation of the memory block. */
static void	allocate_block		(ucl_vector_t this, size_t size_in_slots);
static void	reallocate_block	(ucl_vector_t this, size_t new_size_in_bytes);

/* Moving used bytes. */
static inline void move_used_bytes_at_beginning		(ucl_vector_t this);
static inline void move_used_bytes_at_end		(ucl_vector_t this);
static inline void move_used_bytes_at_byte_pointer	(ucl_vector_t this,
							 ucl_byte_t *p);
static inline void move_used_bytes_at_index_offset	(ucl_vector_t this,
							 ucl_vector_index_t offset);

/* Move used bytes around to preserve padding and free memory when reallcoating. */
static void	move_used_bytes_at_pad_area_beginning (ucl_vector_t this);
static void	move_used_bytes_leave_requested_at_end (ucl_vector_t this,
						   size_t bytes_requested_at_end);

/* Iterators advancing functions. */
static ucl_iterator_next_t	iterator_forward_next;
static ucl_iterator_next_t	iterator_backward_next;
static ucl_iterator_next_t	iterator_range_forward_next;
static ucl_iterator_next_t	iterator_range_backward_next;
static void iterator_next	(ucl_iterator_t iterator, ucl_bool_t forward);
static void iterator_range_next	(ucl_iterator_t iterator, ucl_bool_t forward);


/* Helper functions for "ucl_vector_insert()". */
static ucl_byte_t *	\
   insert_slot_moving_data_toward_the_beginning	(ucl_vector_t this,
						 ucl_byte_t * pointer_to_slot_p);
static ucl_byte_t *	\
   insert_slot_moving_data_toward_the_end	(ucl_vector_t this,
						 ucl_byte_t * pointer_to_slot_p);
/* Helper functions for "ucl_vector_erase()". */
static inline void \
erase_range_moving_tail_data_toward_the_beginning (ucl_vector_t this,
						   ucl_byte_pointer_range_t range);
static inline void \
erase_range_moving_head_data_toward_the_end	(ucl_vector_t this,
						 ucl_byte_pointer_range_t range);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Inspection functions.
 ** ----------------------------------------------------------*/

#if (UCL_DEBUGGING == 1)
static void
PRINT_SLOT_INSPECTION (ucl_vector_t self, const char *title)
{
  ucl_debug("-------- %s --------\n\
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
  ucl_debug("-------- %s --------\n\
\t\tfirst_allocated\t%p\n\
\t\tfirst_used\t%p\n\
\t\tlast_allocated\t%p\n\
\t\tlast_used\t%p\n\
\t\t(first_used < last_used)? = %s", title,
	    self->first_allocated_slot, self->first_used_slot,
	    self->last_allocated_slot, self->last_used_slot,
	    ucl_question(self->first_used_slot < self->last_used_slot));
}
#else
#  define PRINT_SLOT_INSPECTION(VECTOR, TITLE)		/* empty */
#  define PRINT_POINTER_INSPECTION(VECTOR, TITLE)	/* empty */
#endif  

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Initialisation functions.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-16-16) void
ucl_vector_initialise (ucl_vector_t self, size_t slot_dimension)
{
  assert(self); assert(slot_dimension);
  ucl_struct_clean(self, ucl_vector_struct_t);
  self->step_up			= UCL_VECTOR_DEFAULT_STEP_UP;
  self->step_down		= UCL_VECTOR_DEFAULT_STEP_DOWN;
  self->size_of_padding_area	= UCL_VECTOR_DEFAULT_PAD;
  self->slot_dimension		= slot_dimension;

  /* This  is  a weird  setting,  but  it  is correctly  interpreted  by
     'ucl_vector_constructor()'. */
  self->first_used_slot		= NULL;
  self->last_used_slot		= self->first_used_slot + UCL_VECTOR_DEFAULT_SIZE;

  /* 'self->allocator.alloc' is set to NULL by 'ucl_struct_clean()' */
  self->allocator.alloc		= ucl_memory_alloc;
  ASSERT_INITIALISE_CONDITIONS(self);
}
stub(2005-09-23-18-16-19) void
ucl_vector_initialise_size (ucl_vector_t self, size_t number_of_initial_slots)
{
  /* This  is  a weird  setting,  but  it  is correctly  interpreted  by
     'ucl_vector_constructor()'. */
  self->last_used_slot	= self->first_used_slot + number_of_initial_slots;
}
stub(2005-09-23-18-16-21) void
ucl_vector_initialise_pad (ucl_vector_t self, size_t size_of_padding_area)
{
  self->size_of_padding_area = size_of_padding_area;
}
stub(2005-09-23-18-16-24) void
ucl_vector_initialise_step_up (ucl_vector_t self, size_t step_up)
{
  self->step_up = step_up;
}
stub(2005-09-23-18-16-26) void
ucl_vector_initialise_step_down (ucl_vector_t self, size_t step_down)
{
  self->step_down = step_down;
}

/* ------------------------------------------------------------ */

stub(2006-03-02-22-48-28) void
ucl_vector_initialise_buffer (ucl_vector_t self)
{
  ucl_vector_initialise(self, sizeof(ucl_byte_t));
  ucl_vector_initialise_size(self, UCL_VECTOR_BUFFER_PAGE_SIZE);
  ucl_vector_initialise_pad(self, 0);
  ucl_vector_initialise_step_up(self,   UCL_VECTOR_BUFFER_PAGE_SIZE);
  ucl_vector_initialise_step_down(self, UCL_VECTOR_BUFFER_PAGE_SIZE);
  ASSERT_INITIALISE_CONDITIONS(self);
}

/* ------------------------------------------------------------ */

stub(2006-10-05-14-28-25) void
ucl_vector_register_allocator (ucl_vector_t self, ucl_memory_allocator_t allocator)
{
  self->allocator = allocator;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Construction/destruction.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-16-28) void
ucl_vector_constructor (ucl_vector_t self)
{
  ASSERT_INITIALISE_CONDITIONS(self);

  if (self->step_up >= self->step_down)
    self->step_down = self->step_up + 1;

  self->step_up			*= self->slot_dimension;
  self->step_down		*= self->slot_dimension;
  self->size_of_padding_area	*= self->slot_dimension;

  allocate_block(self,
		 /* This is weird, but it is a correct
		    interpretation of what 'ucl_vector_initialise()'
		    and 'ucl_vector_initialise_size()' do. */
		 (self->last_used_slot - self->first_used_slot));
  set_pointers_for_empty_vector(self);
  PRINT_SLOT_INSPECTION(self, "construction");
  PRINT_POINTER_INSPECTION(self, "construction");
  ASSERT_INVARIANTS(self);
}
stub(2005-09-23-18-16-32) void
ucl_vector_destructor (ucl_vector_t self)
{
  if (NULL != self->first_allocated_slot)
    {
      ASSERT_INVARIANTS(self);
      self->allocator.alloc(self->allocator.data,
			    &(self->first_allocated_slot), 0);
      ucl_struct_clean(self, ucl_vector_struct_t);
    }
}

/* ------------------------------------------------------------ */

stub(2006-03-03-10-30-32) void
ucl_vector_swallow_block (ucl_vector_t self, ucl_block_t block)
{
  ASSERT_INITIALISE_CONDITIONS(self);

  self->step_up			*= self->slot_dimension;
  self->step_down		*= self->slot_dimension;
  self->size_of_padding_area	*= self->slot_dimension;

  assert(0 == (block.len % self->slot_dimension));
  self->first_allocated_slot = self->first_used_slot = block.ptr;
  self->last_allocated_slot  = self->last_used_slot  =
    block.ptr + block.len - self->slot_dimension;

  ASSERT_INVARIANTS(self);
  PRINT_SLOT_INSPECTION(self, "construction");
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Inspection and configuration.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-16-34) size_t
ucl_vector_number_of_step_up_slots (const ucl_vector_t self)
{
  return (self->step_up)? (self->step_up / self->slot_dimension) : 0;
}
stub(2005-09-23-18-16-36) size_t
ucl_vector_number_of_step_down_slots (const ucl_vector_t self)
{
  return (self->step_down)? (self->step_down / self->slot_dimension) : 0;
}
stub(2005-09-23-18-16-39) size_t
ucl_vector_number_of_padding_slots (const ucl_vector_t self)
{
  return ((self->size_of_padding_area)?
	  (self->size_of_padding_area / self->slot_dimension) : 0);
}
stub(2006-03-03-20-59-23) ucl_bool_t
ucl_vector_running (const ucl_vector_t self)
{
  return (NULL != self->first_allocated_slot);
}

/* ------------------------------------------------------------ */

stub(2007-10-22-13-09-11) void
ucl_vector_update_number_of_step_up_slots (ucl_vector_t self, size_t step_up)
{
  self->step_up = step_up * self->slot_dimension;
}
stub(2007-10-22-13-09-17) void
ucl_vector_update_number_of_step_down_slots (ucl_vector_t self, size_t step_down)
{
  self->step_down = step_down * self->slot_dimension;
}
stub(2007-10-22-13-09-22) void
ucl_vector_update_number_of_padding_slots (ucl_vector_t self, size_t padding)
{
  self->size_of_padding_area = padding * self->slot_dimension;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Special slots functions.
 ** ----------------------------------------------------------*/

stub(2005-10-03-11-06-52) void
ucl_vector_mark_all_slots_as_used (ucl_vector_t self)
{
  self->first_used_slot = self->first_allocated_slot;
  self->last_used_slot  = self->last_allocated_slot;
}
stub(2006-03-04-08-08-21) void
ucl_vector_reset (ucl_vector_t self)
{
  set_pointers_for_empty_vector(self);
}
stub(2006-03-04-08-11-16) void
ucl_vector_clean (ucl_vector_t self)
{
  ucl_block_t	slots;

  slots = ucl_vector_get_memory_block(self);
  ucl_block_clean_memory(slots);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Converting indexes to pointers.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-16-41) void *
ucl_vector_index_to_slot (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return pointer_to_slot_or_null(self, idx, 0);
}
stub(2005-09-23-18-16-43) void *
ucl_vector_index_to_new_slot (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return pointer_to_slot_or_null(self, idx, self->slot_dimension);
}
static void *
pointer_to_slot_or_null (self, idx, slot_dimension_or_zero)
     const ucl_vector_t	self;
     ucl_vector_index_t idx;
     size_t		slot_dimension_or_zero;
{
  ucl_byte_t *	pointer_to_slot_p;
  ucl_byte_t *	max_allowed_value_p;


  ASSERT_INVARIANTS(self);
  pointer_to_slot_p   = self->first_used_slot     + self->slot_dimension * idx;
  max_allowed_value_p = self->last_allocated_slot + slot_dimension_or_zero;
  return (pointer_to_slot_p <= max_allowed_value_p)? pointer_to_slot_p : NULL;
}
stub(2005-09-23-18-16-46) ucl_vector_index_t
ucl_vector_last_index (const ucl_vector_t self)
{
  ucl_vector_index_t	last = compute_used_slots(self);
  
  return (last > 0)? (last - 1) : 0;
}
stub(2005-09-23-18-16-49) ucl_vector_index_t
ucl_vector_slot_to_index (const ucl_vector_t self, const void *_pointer_to_slot_p)
{
  const ucl_byte_t * pointer_to_slot_p = _pointer_to_slot_p;
  return (pointer_to_slot_p - self->first_used_slot)/self->slot_dimension;
}

/* ------------------------------------------------------------ */

stub(2005-09-23-18-16-51) ucl_bool_t
ucl_vector_pointer_is_valid_slot (const ucl_vector_t self, const void *_pointer_to_slot_p)
{
  const ucl_byte_t *	pointer_to_slot_p = _pointer_to_slot_p;

  return ((pointer_to_slot_p >= self->first_used_slot) &&
	  (pointer_to_slot_p <= self->last_used_slot)  &&
	  (((pointer_to_slot_p - self->first_used_slot)%self->slot_dimension)==0));
}
stub(2005-09-23-18-16-54) ucl_bool_t
ucl_vector_index_is_valid (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return (idx < ucl_vector_size(self));
}
stub(2007-10-30-13-52-30) ucl_bool_t
ucl_vector_index_is_valid_new (const ucl_vector_t self, ucl_vector_index_t idx)
{
  return (idx <= ucl_vector_size(self));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Range functions.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-16-56) ucl_bool_t
ucl_vector_range_is_valid (const ucl_vector_t self, ucl_range_t range)
{
  return (ucl_vector_index_is_valid(self, ucl_range_min(range)) &&
	  ucl_vector_index_is_valid(self, ucl_range_max(range)));
}
stub(2007-10-26-11-54-39) ucl_range_t
ucl_vector_range (const ucl_vector_t self)
{
  ucl_range_t	range;

  ucl_range_set_min_size(range, 0, ucl_vector_size(self));
  return range;
}
stub(2007-10-26-11-56-36) ucl_range_t
ucl_vector_range_from_position_to_end (const ucl_vector_t self, ucl_vector_index_t position)
{
  ucl_range_t	range;

  assert(ucl_vector_index_is_valid(self, position));
  ucl_range_set_min_size(range, position, ucl_vector_size(self)-position);
  return range;
}
stub(2007-10-26-12-00-34) ucl_range_t
ucl_vector_range_from_end_to_position (const ucl_vector_t self, ucl_vector_index_t position)
{
  ucl_range_t	range;

  assert(ucl_vector_size(self) <= position);
  ucl_range_set_min_size(range, ucl_vector_size(self), position);
  return range;
}
stub(2007-10-26-12-08-18) ucl_range_t
ucl_vector_range_from_end_with_span (const ucl_vector_t self, size_t span)
{
  ucl_range_t	range;
  size_t	size = ucl_vector_size(self);

  ucl_range_set_min_size(range, size, span);
  return range;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Insert functions.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-17-01) void *
ucl_vector_insert (ucl_vector_t self, void *_pointer_to_slot_p)
{
  ucl_bool_t	room_at_the_end, room_at_the_beginning;
   ucl_byte_t *	pointer_to_slot_p = _pointer_to_slot_p;


  ucl_debug("making room at pointer %p, first %p, index %d",
	    pointer_to_slot_p, self->first_used_slot,
	    ucl_vector_slot_to_index(self, pointer_to_slot_p));
  ASSERT_INVARIANTS(self);
  ASSERT_IS_POINTER_TO_NEW_SLOT(self, pointer_to_slot_p);
  ucl_debug("room at beg: %s, room at end: %s",
	    ucl_question(there_is_room_at_the_beginning(self)),
	    ucl_question(there_is_room_at_the_end(self)));
  PRINT_SLOT_INSPECTION(self, "before insertion");
  ASSERT_THERE_IS_ROOM(self);
  if (vector_is_empty(self))
    {
      assert(self->first_used_slot == pointer_to_slot_p);
      self->last_used_slot = self->first_used_slot;
    }
  else if (pointer_to_slot_p == self->last_allocated_slot + self->slot_dimension)
    {
      assert(self->last_used_slot == self->last_allocated_slot);
      memmove(self->first_used_slot - self->slot_dimension,
	      self->first_used_slot,
	      self->slot_dimension * compute_used_slots(self));
      self->first_used_slot -= self->slot_dimension;
      pointer_to_slot_p      = self->last_used_slot;
    }
  else
    {
      room_at_the_beginning	= there_is_room_at_the_beginning(self);
      room_at_the_end		= there_is_room_at_the_end(self);
      if (room_at_the_beginning && (pointer_to_slot_p == self->first_used_slot))
	{
	  ucl_debug("mark 1 %p", pointer_to_slot_p);
	  self->first_used_slot -= self->slot_dimension;
	  pointer_to_slot_p = self->first_used_slot;
	}
      else if (room_at_the_end &&
	       (pointer_to_slot_p == self->last_used_slot + self->slot_dimension))
	{
	  self->last_used_slot = pointer_to_slot_p;
	}
      else if (room_at_the_beginning && room_at_the_end)
	{
	  ucl_debug("mark 2 %p", pointer_to_slot_p);
	  if (compute_bytes_at_beginning(self) < compute_bytes_at_end(self))
	    {
	      ucl_debug("mark 21 %p", pointer_to_slot_p);
	      pointer_to_slot_p = \
		insert_slot_moving_data_toward_the_end(self, pointer_to_slot_p);
	    }
	  else
	    {
	      ucl_debug("mark 22 %p", pointer_to_slot_p);
	      pointer_to_slot_p = \
		insert_slot_moving_data_toward_the_beginning(self, pointer_to_slot_p);
	    }
	}
      else if (room_at_the_beginning)
	{
	  ucl_debug("mark 3 %p", pointer_to_slot_p);
	  assert(self->first_allocated_slot < self->first_used_slot);
	  pointer_to_slot_p = \
	    insert_slot_moving_data_toward_the_beginning(self, pointer_to_slot_p);
	}
      else 
	{
	  ucl_debug("mark 4 %p", pointer_to_slot_p);
	  assert(room_at_the_end);
	  pointer_to_slot_p = \
	    insert_slot_moving_data_toward_the_end(self, pointer_to_slot_p);
	}
    }
  ucl_debug("exit %p", pointer_to_slot_p);

  ASSERT_INVARIANTS(self);
  return pointer_to_slot_p;
}
static ucl_byte_t *
insert_slot_moving_data_toward_the_beginning (self, pointer_to_slot_p)
     ucl_vector_t	self;
     ucl_byte_t *	pointer_to_slot_p;
{
  assert(there_is_room_at_the_beginning(self));
  ucl_debug("here %p", pointer_to_slot_p);
  if (pointer_to_slot_p != self->first_used_slot)
    {
      memmove(self->first_used_slot - self->slot_dimension,
	      self->first_used_slot, pointer_to_slot_p - self->first_used_slot);
    }
  pointer_to_slot_p -= self->slot_dimension;
  self->first_used_slot -= self->slot_dimension;
  ucl_debug("there %p", pointer_to_slot_p);
  return pointer_to_slot_p;
}
static ucl_byte_t *
insert_slot_moving_data_toward_the_end (self, pointer_to_slot_p)
     ucl_vector_t	self;
     ucl_byte_t *	pointer_to_slot_p;
{
  assert(there_is_room_at_the_end(self));
  if (pointer_to_slot_p != self->last_used_slot)
    {
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
      ucl_debug("pointer to slot NOT equal to last used");
      memmove(pointer_to_slot_p + self->slot_dimension, pointer_to_slot_p,
	      self->last_used_slot - pointer_to_slot_p + self->slot_dimension);
    }
  else
    {
      ucl_debug("pointer to slot equal to last used");
      memmove(self->last_used_slot + self->slot_dimension,
	      self->last_used_slot, self->slot_dimension);
    }
  self->last_used_slot += self->slot_dimension;
  return pointer_to_slot_p;
}
stub(2005-09-23-18-17-07) void *
ucl_vector_insert_sort (ucl_vector_t self, void *data_p)
{
  ucl_byte_t *		p = NULL;
  ucl_vector_index_t	size;


  ASSERT_INVARIANTS(self); ASSERT_COMPAR_FUNCTION_IS_SET(self); assert(data_p);

  size = ucl_vector_size(self);
  if (! size)
    {
      p = ucl_vector_index_to_new_slot(self, 0);
    }
  else if (size <= SLOT_NUMBER_THAT_TRIGGERS_BINARY_OVER_LINEAR_SEARCH)
    {
      ucl_valcmp_t	compar = self->compar;
      ucl_value_t	a, b;


      ucl_debug("linear search (size %u)", size);
      for (p = self->first_used_slot; p <= self->last_used_slot;
	   p += self->slot_dimension)
	{
	  ucl_debug("current %d", *(int *)p);
	  a.ptr = data_p;
	  b.ptr = p;
	  if (compar.func(compar.data, a, b) <= 0)
	    {
	      ucl_debug("it's it!!");
	      break;
	    }
	}
    }
  else
    {
      size_t		i, lower_index_limit, upper_index_limit;
      size_t		current_index, new_current_index;
      int		match;
      ucl_valcmp_t	compar = self->compar;
      ucl_value_t	a, b;


      ucl_debug("binary search (size %u)", size);

      lower_index_limit	= 0;
      upper_index_limit	= size-1;
      /* Doing shift-right-1 on  2*N will return N, on  2*N+1 will return N,
	 too (tested with the [expr] command of TCL). */
      current_index		= size >> 1;

      for (i=0; i<size; ++i)
	{
	  p = ucl_vector_index_to_slot(self, current_index);
	  a.ptr = data_p;
	  b.ptr = p;
	  match = compar.func(compar.data, a, b);
	  if (0 == match)
	    break;
	  else if (match < 0) /* the new item is less */
	    {
	      upper_index_limit = current_index - 1;
	      new_current_index = lower_index_limit +
		((current_index - lower_index_limit) >> 1);
	      current_index     = (new_current_index < current_index)?
		new_current_index : (new_current_index-1);
	      if (current_index < lower_index_limit)
		{
		  ucl_debug("lower %d", current_index+1);
		  p = ucl_vector_index_to_new_slot(self, current_index+1);
		  break;
		}
	    }
	  else /* match > 0 */ /* the new item is greater */
	    {
	      lower_index_limit = current_index + 1;
	      new_current_index = current_index +
		((upper_index_limit - current_index) >> 1);
	      current_index     = (new_current_index > current_index)?
		new_current_index : (new_current_index+1);
	      if (current_index > upper_index_limit)
		{
		  p = ucl_vector_index_to_new_slot(self, current_index);
		  ucl_debug("upper %d", current_index);
		  break;
		}
	    }
	}
    }
  ucl_debug("exiting %p (%d)", p, *((int *)p));
  return ucl_vector_insert(self, p);
}

/** ------------------------------------------------------------
 ** Erase functions.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-17-11) void
ucl_vector_erase (ucl_vector_t self, void *_pointer_to_slot_p)
{
  ucl_bool_t			room_at_the_end, room_at_the_beginning;
  ucl_byte_t *			pointer_to_slot_p = _pointer_to_slot_p;
  ucl_byte_pointer_range_t	range;


  ASSERT_INVARIANTS(self);
  ASSERT_IS_POINTER_TO_USED_SLOT(self, pointer_to_slot_p);
  assert(ucl_vector_size(self));

  if (1 == ucl_vector_size(self))
    { set_pointers_for_empty_vector(self); }
  else if (pointer_to_slot_p == self->first_used_slot)
    { self->first_used_slot += self->slot_dimension; }
  else if (pointer_to_slot_p == self->last_used_slot)
    { self->last_used_slot -= self->slot_dimension; }
  else
    {
      /* the range is inclusive !!! */
      ucl_range_set_min_max(range, pointer_to_slot_p, pointer_to_slot_p);
      room_at_the_beginning	= there_is_room_at_the_beginning(self);
      room_at_the_end		= there_is_room_at_the_end(self);
      if (room_at_the_beginning && room_at_the_end)
	{
	  if (compute_bytes_at_beginning(self) < compute_bytes_at_end(self))
	    { goto move_head_data_toward_the_end; }
	  else
	    { goto move_tail_data_toward_the_beginning; }
	}
      else if (room_at_the_beginning)
	{ goto move_tail_data_toward_the_beginning; }
      else
	{ goto move_head_data_toward_the_end; }
    }
  return;

 move_tail_data_toward_the_beginning:
  erase_range_moving_tail_data_toward_the_beginning(self, range); 
  return;
 move_head_data_toward_the_end:
  erase_range_moving_head_data_toward_the_end(self, range);
}
static inline void 
erase_range_moving_tail_data_toward_the_beginning (self, range)
     ucl_vector_t		self;
     ucl_byte_pointer_range_t	range;
{
  ucl_debug("move tail data toward the beg");
  /*
    Notice:

    (last_used_slot + slot_dimension) - (range.max + slot_dimension)
    = (last_used_slot - range.max)
  */
  memmove(range.min, (range.max + self->slot_dimension),
	  (self->last_used_slot - (range.max)));
  self->last_used_slot -= (range.max + self->slot_dimension) - range.min;
}
static inline void
erase_range_moving_head_data_toward_the_end (self, range)
     ucl_vector_t		self;
     ucl_byte_pointer_range_t	range;
{
  ucl_byte_t *	new_first;

  ucl_debug("move head data toward the end");
  new_first = self->first_used_slot + (range.max - range.min + self->slot_dimension);
  memmove(new_first, self->first_used_slot,
	  (range.min - self->first_used_slot));
  self->first_used_slot = new_first;
}

/** ------------------------------------------------------------
 ** Various operations.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-17-18) size_t
ucl_vector_slot_dimension (const ucl_vector_t self)
{
  return self->slot_dimension;
}
stub(2005-09-23-18-17-20) size_t
ucl_vector_size (const ucl_vector_t self)
{
  return compute_used_slots(self);
}
stub(2005-09-23-18-17-22) void
ucl_vector_set_compar (ucl_vector_t self, ucl_valcmp_t compar)
{
  self->compar = compar;
}

/* ------------------------------------------------------------ */

stub(2005-09-23-18-17-23) ucl_block_t
ucl_vector_get_memory_block (const ucl_vector_t self)
{
  ucl_block_t	allocated_memory;

  allocated_memory.len = compute_allocated_bytes(self);
  allocated_memory.ptr = self->first_allocated_slot;
  return allocated_memory;
}
stub(2005-09-23-18-17-26) ucl_block_t
ucl_vector_get_data_block (const ucl_vector_t self)
{
  ucl_block_t	data;

  data.len = compute_used_bytes(self);
  data.ptr = self->first_used_slot;
  return data;
}
stub(2006-11-20-07-04-49) ucl_block_t
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
stub(2006-11-20-07-15-11) ucl_block_t
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
stub(2006-11-20-07-16-43) void
ucl_vector_mark_as_used (ucl_vector_t self, ucl_block_t used_block)
{
  assert(0 == (used_block.len % self->slot_dimension));
  /* Assert that 'used_block' is adjacent to the block of used slots, at
     the beginning or at the end. */
  assert(((used_block.ptr + used_block.len) == self->first_used_slot) ||
	 ((self->last_used_slot + self->slot_dimension) == used_block.ptr));
  /* Assert that 'used_block' is inside the allocated block. */
  assert(self->first_allocated_slot <= used_block.ptr);
  assert((used_block.ptr + used_block.len) <=
	 (self->last_allocated_slot + self->slot_dimension));
  
  if (used_block.ptr < self->first_used_slot)
    self->first_used_slot = used_block.ptr;
  else
    self->last_used_slot = used_block.ptr + used_block.len - self->slot_dimension;
}

/* ------------------------------------------------------------ */

stub(2005-09-23-18-17-29) void
ucl_vector_quick_sort (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self); ASSERT_COMPAR_FUNCTION_IS_SET(self);

  ucl_quicksort(self->first_used_slot, compute_used_slots(self),
		self->slot_dimension, self->compar);
}
stub(2006-10-06-11-47-11) ucl_bool_t
ucl_vector_sorted (ucl_vector_t self)
{
  size_t	size = ucl_vector_size(self);
  ucl_valcmp_t	compar = self->compar;
  ucl_value_t	a, b;

  for (size_t i=1; i<size; ++i)
    {
      a.ptr = ucl_vector_index_to_slot(self, i-1);
      b.ptr = ucl_vector_index_to_slot(self, i);
      if (0 < compar.func(compar.data, a, b))
	return 0;
    }
  return 1;
}

/* ------------------------------------------------------------ */

stub(2006-11-19-13-08-03) ucl_block_t
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
stub(2006-11-19-13-12-13) ucl_range_t
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
			ucl_vector_slot_to_index(self, (block.ptr + block.len) -
						 self->slot_dimension));
  return range;
}


/** ------------------------------------------------------------
 ** Whole vector iteration functions.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-17-33) void
ucl_vector_iterator_forward (const ucl_vector_t self, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self))? self->first_used_slot : NULL;
  iterator->next	= iterator_forward_next;
}
stub(2005-09-23-18-17-35) void
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
  const ucl_vector_struct_t *	self = iterator->container;
  ucl_byte_t *			current_p = iterator->iterator;

  ASSERT_INVARIANTS(self);
  if (forward)
    {
      if (current_p < self->last_used_slot)
	{
	  iterator->iterator = current_p + self->slot_dimension;
	  goto end;
	}
    }
  else
    {
      if (current_p > self->first_used_slot)
	{
	  iterator->iterator = current_p - self->slot_dimension;
	  goto end;
	}
    }
  iterator->iterator = NULL;
 end:
  ASSERT_INVARIANTS(self);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Range iteration functions.
 ** ----------------------------------------------------------*/

stub(2007-10-25-07-27-53) void
ucl_vector_iterator_range_forward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  assert(ucl_vector_range_is_valid(self, range));
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self) && ucl_range_size(range))?
    ucl_vector_index_to_slot(self, ucl_range_min(range)) : NULL;
  iterator->ptr1	= ucl_vector_index_to_slot(self, ucl_range_max(range));
  iterator->next	= iterator_range_forward_next;
}
stub(2007-10-25-07-29-52) void
ucl_vector_iterator_range_backward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator)
{
  ASSERT_INVARIANTS(self);
  assert(ucl_vector_range_is_valid(self, range));
  iterator->container	= self;
  iterator->iterator	= (ucl_vector_size(self) && ucl_range_size(range))?
    ucl_vector_index_to_slot(self, ucl_range_max(range)) : NULL;
  iterator->ptr1	= ucl_vector_index_to_slot(self, ucl_range_min(range));
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
  const ucl_vector_struct_t *	self = iterator->container;
  ucl_byte_t *			current_p = iterator->iterator;
  ucl_byte_t *			last_p = iterator->ptr1;

  ASSERT_INVARIANTS(self);
  if (forward)
    {
      if (current_p < last_p)
	{
	  iterator->iterator = current_p + self->slot_dimension;
	  goto end;
	}
    }
  else
    {
      if (current_p > last_p)
	{
	  iterator->iterator = current_p - self->slot_dimension;
	  goto end;
	}
    }
  iterator->iterator = NULL;
 end:
  ASSERT_INVARIANTS(self);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Memory functions: enlarging.
 ** ----------------------------------------------------------*/

stub(2007-11-12-13-16-37) ucl_bool_t
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
  ucl_debug("old size: %d", number_of_allocated_bytes);

  rest = number_of_allocated_bytes % self->step_up;
  number_of_allocated_bytes += (rest)? rest : self->step_up;
  ucl_debug("new size: %d", number_of_allocated_bytes);
  return number_of_allocated_bytes;
}
stub(2007-11-12-13-26-53) size_t
ucl_vector_enlarged_size (ucl_vector_t self)
{
  return (compute_enlarged_size_in_bytes(self) / self->slot_dimension);
}
stub(2005-09-23-18-17-40) void
ucl_vector_enlarge (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);

  if (ucl_vector_will_enlarge(self))
    {
      reallocate_block(self, compute_enlarged_size_in_bytes(self));
      ASSERT_INVARIANTS(self);
    }
}
stub(2006-03-01-09-51-14) void
ucl_vector_enlarge_for_slots (ucl_vector_t self, size_t required_free_slots)
{
  ASSERT_INVARIANTS(self);
  size_t	number_of_allocated_bytes, rest;
  size_t	number_of_free_slots, number_of_slots_to_allocate;
  

  ucl_debug("requested slots: %d", required_free_slots); 
  PRINT_SLOT_INSPECTION(self, "before reallocation");

  assert(ucl_vector_number_of_free_slots(self) ==
	 ((compute_bytes_at_beginning(self) + compute_bytes_at_end(self))
	  / self->slot_dimension));

  number_of_free_slots = ucl_vector_number_of_free_slots(self);
  ucl_debug("reallocating? %s", ucl_question(number_of_free_slots < required_free_slots));


  if (number_of_free_slots < required_free_slots)
    {
      number_of_slots_to_allocate = required_free_slots - number_of_free_slots;
      ucl_debug("slots to be allocated: %d", number_of_slots_to_allocate); 
      number_of_allocated_bytes   = compute_allocated_bytes(self);
      ucl_debug("old byte size: %d", number_of_allocated_bytes); 

      number_of_allocated_bytes += number_of_slots_to_allocate * self->slot_dimension;
      rest = number_of_allocated_bytes % self->step_up;
      number_of_allocated_bytes += (rest)? rest : self->step_up;
      ucl_debug("new byte size (adjusted to step_up): %d", number_of_allocated_bytes);
      
      reallocate_block(self, number_of_allocated_bytes);
    }

  PRINT_SLOT_INSPECTION(self, "after reallocation");

  assert(ucl_vector_number_of_free_slots(self) ==
	 ((compute_bytes_at_beginning(self) + compute_bytes_at_end(self))
	  / self->slot_dimension));
  ASSERT_INVARIANTS(self);
  ucl_debug("done");
}
stub(2007-10-26-12-20-00) void
ucl_vector_enlarge_for_range (ucl_vector_t self, ucl_range_t range)
{
  size_t		size = ucl_vector_size(self);
  ucl_vector_index_t	max  = ucl_range_max(range);


  if (size <= max)
    {
      size_t	required_free_slots = max - size + 1;

      ucl_vector_enlarge_for_slots(self, required_free_slots);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Memory functions: restricting.
 ** ----------------------------------------------------------*/

stub(2007-11-12-13-18-22) ucl_bool_t
ucl_vector_will_restrict (ucl_vector_t self)
{
  return (compute_free_bytes(self) >= self->step_down);
}
static size_t
compute_restricted_size_in_bytes (ucl_vector_t self)
{
  size_t	number_of_allocated_bytes, new_number_of_allocated_bytes;
  size_t	number_of_used_bytes, rest;


  number_of_allocated_bytes	= compute_allocated_bytes(self);
  number_of_used_bytes		= compute_used_bytes(self);

  new_number_of_allocated_bytes = number_of_allocated_bytes - self->step_down;
  rest = number_of_allocated_bytes % self->step_up;
  new_number_of_allocated_bytes += (rest)? rest : self->step_up;

  /* The following  condition can be false when:  'step_up > step_down'.
     This  is why  'step_down' is  normalised  to a  value greater  than
     'step_up' by the constructor. */
  assert(new_number_of_allocated_bytes < number_of_allocated_bytes);
  return new_number_of_allocated_bytes;
}
stub(2007-11-12-13-34-28) size_t
ucl_vector_restricted_size (ucl_vector_t self)
{
  return (compute_restricted_size_in_bytes(self) / self->slot_dimension);
}
stub(2005-09-23-18-17-43) void
ucl_vector_restrict (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);


  if (ucl_vector_will_restrict(self))
    {
      size_t	number_of_allocated_bytes	= compute_allocated_bytes(self);
      size_t	new_number_of_allocated_bytes	= compute_restricted_size_in_bytes(self);


      move_used_bytes_leave_requested_at_end(self, number_of_allocated_bytes - new_number_of_allocated_bytes);
      reallocate_block(self, new_number_of_allocated_bytes);
      move_used_bytes_at_pad_area_beginning(self);

      ASSERT_INVARIANTS(self);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Memory functions: miscellaneous operations.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-17-47) void
ucl_vector_set_memory_to_zero (ucl_vector_t self)
{
  memset(self->first_allocated_slot, '\0', compute_allocated_bytes(self));
}
stub(2005-09-23-18-17-49) size_t
ucl_vector_number_of_free_slots (const ucl_vector_t self)
{
  return (compute_allocated_bytes(self) - \
	  compute_used_bytes(self)) / self->slot_dimension;
}
static void
allocate_block (ucl_vector_t self, size_t number_of_slots)
{
  size_t	number_of_bytes;
  void *	p = NULL;
  

  number_of_bytes = self->slot_dimension * number_of_slots;
  self->allocator.alloc(self->allocator.data, &p, number_of_bytes);
  assert(p);
  self->first_allocated_slot	= p;
  self->last_allocated_slot	=
    ((ucl_byte_t *)p) + number_of_bytes - self->slot_dimension;
}
static void
reallocate_block (ucl_vector_t self, size_t new_number_of_allocated_bytes)
{
  size_t	offset_of_first_used_slot, offset_of_last_used_slot;
  ucl_byte_t *	p;
  

  assert(0 == (new_number_of_allocated_bytes % self->slot_dimension));

  p = self->first_allocated_slot;
  offset_of_first_used_slot = self->first_used_slot - p;
  offset_of_last_used_slot  = self->last_used_slot  - p;

  self->allocator.alloc(self->allocator.data, &p, new_number_of_allocated_bytes);

  assert(self->first_allocated_slot);
  self->first_allocated_slot	= p;
  self->first_used_slot		= p + offset_of_first_used_slot;
  self->last_used_slot		= p + offset_of_last_used_slot;
  self->last_allocated_slot	= p + \
    new_number_of_allocated_bytes - self->slot_dimension;
  ASSERT_INVARIANTS(self);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Find functions.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-17-56) void *
ucl_vector_find (const ucl_vector_t self, const void * data_p)
{
  ucl_byte_t *	p;


  ASSERT_INVARIANTS(self); ASSERT_COMPAR_FUNCTION_IS_SET(self); assert(data_p);

  if (ucl_vector_size(self))
    {
      ucl_valcmp_t	compar = self->compar;
      ucl_value_t	a, b;

      for (p = self->first_used_slot; p <= self->last_used_slot;
	   p += self->slot_dimension)
	{
	  a.ptr = (void *)data_p;
	  b.ptr = p;
	  if (compar.func(compar.data, a, b) == 0) { return p; }	  
	}
    }
  return NULL;
}
stub(2005-09-23-18-17-58) void *
ucl_vector_sort_find (const ucl_vector_t self, const void * data_p)
{
  ucl_vector_index_t	size;


  ASSERT_INVARIANTS(self); ASSERT_COMPAR_FUNCTION_IS_SET(self); assert(data_p);

  size = ucl_vector_size(self);
  if (! size) { return NULL; }
  return (size <= SLOT_NUMBER_THAT_TRIGGERS_BINARY_OVER_LINEAR_SEARCH)? \
    ucl_vector_find(self, data_p) : ucl_vector_binary_search(self, data_p);
}
stub(2005-09-23-18-18-00) void *
ucl_vector_binary_search (const ucl_vector_t self, const void * data_p)
{
  size_t	size, i, lower_index_limit, upper_index_limit;
  size_t	current_index, new_current_index;
  ucl_byte_t *	p;
  int		match;
  ucl_valcmp_t	compar = self->compar;
  ucl_value_t	a, b;


  size = ucl_vector_size(self);
  lower_index_limit	= 0;
  upper_index_limit	= size-1;
  /* Doing shift-right-1 on  2*N will return N, on  2*N+1 will return N,
     too (tested with the [expr] command of TCL). */
  current_index		= size >> 1;

  for (i=0; i<size; ++i)
    {
      p = ucl_vector_index_to_slot(self, current_index);
      a.ptr = (void *)data_p;
      b.ptr = p;
      match = compar.func(compar.data, a, b);
      if (0 == match) { return p; }
      else if (match < 0)
	{
	  if (0 == current_index) return NULL;
	  upper_index_limit = current_index - 1;
	  new_current_index = lower_index_limit +
	    ((current_index - lower_index_limit) >> 1);
	  current_index     = (new_current_index < current_index)?
	    new_current_index : (new_current_index-1);
	  if (current_index < lower_index_limit) { return NULL; }
	}
      else /* match > 0 */
	{
	  if (UINT_MAX == current_index) return NULL;
	  lower_index_limit = current_index + 1;
	  new_current_index = current_index +
	    ((upper_index_limit - current_index) >> 1);
	  current_index     = (new_current_index > current_index)?
	    new_current_index : (new_current_index+1);
	  if (current_index > upper_index_limit) { return NULL; }
	}
    }
  return NULL;
}

/** ------------------------------------------------------------
 ** High level functions: stack/queue.
 ** ----------------------------------------------------------*/

stub(2006-03-04-07-12-03) ucl_data_t
ucl_vector_push_back (ucl_vector_t self)
{
  ucl_data_t	p;

  ucl_vector_enlarge(self);
  p = ucl_vector_index_to_new_slot(self, ucl_vector_size(self));
  p = ucl_vector_insert(self, p);
  return p;
}
stub(2006-03-04-07-12-03) ucl_data_t
ucl_vector_push_front (ucl_vector_t self)
{
  ucl_data_t	p;

  ucl_vector_enlarge(self);
  p = ucl_vector_index_to_new_slot(self, 0);
  p = ucl_vector_insert(self, p);
  return p;
}
stub(2006-03-04-07-12-03) void
ucl_vector_pop_back (ucl_vector_t self)
{
  if (ucl_vector_size(self))
    {
      ucl_vector_erase(self, ucl_vector_back(self));
      ucl_vector_restrict(self);
    }
}
stub(2006-03-04-07-25-03) void
ucl_vector_pop_front (ucl_vector_t self)
{
  if (ucl_vector_size(self))
    {
      ucl_vector_erase(self, ucl_vector_front(self));
      ucl_vector_restrict(self);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: appending stuff.
 ** ----------------------------------------------------------*/

stub(2006-03-02-17-14-35) void
ucl_vector_append_block (ucl_vector_t self, const ucl_block_t block)
{
  ucl_debug("number of used slots %d", ucl_vector_size(self));
  ucl_debug("request to append a block of len %d", block.len);
  ucl_debug("number of slots to append %d", block.len/self->slot_dimension);
  ucl_debug("vector slot dimension %d, old vector size in slots %d",
	    self->slot_dimension, compute_used_slots(self));
  ucl_debug("len %% slot_dimension = %d", (block.len % self->slot_dimension));

  assert(0 == (block.len % self->slot_dimension));
  ASSERT_INVARIANTS(self);

  if (block.len > 0)
    {
      ucl_byte_t *	first_new_slot;
      size_t	number_of_new_slots = block.len / self->slot_dimension;

      ucl_debug("enlarging for %d new slots", number_of_new_slots);
      ucl_vector_enlarge_for_slots(self, number_of_new_slots);
      assert(compute_free_bytes(self) >= block.len);
      ucl_debug("number of used slots %d", ucl_vector_size(self));

      ucl_debug("making space at end");
      move_used_bytes_leave_requested_at_end(self, block.len);
      ucl_debug("number of free slots %d",
		ucl_vector_number_of_free_slots(self));
      ucl_debug("number of slots at beginning %d",
		compute_bytes_at_beginning(self) / self->slot_dimension);
      ucl_debug("number of slots at end %d",
		compute_bytes_at_end(self) / self->slot_dimension);
      assert(compute_bytes_at_end(self) >= block.len);

      first_new_slot = self->last_used_slot + self->slot_dimension;
      memcpy(first_new_slot, block.ptr, block.len);

      ucl_debug("number of new bytes %d", number_of_new_slots * self->slot_dimension);
      ucl_debug("old offset of last used %d",
		(self->last_used_slot - self->first_used_slot)
		/ self->slot_dimension);
      self->last_used_slot += number_of_new_slots * self->slot_dimension;
      ucl_debug("new offset of last used %d",
		(self->last_used_slot - self->first_used_slot)
		/ self->slot_dimension);
    }

  ucl_debug("completed append");
  PRINT_SLOT_INSPECTION(self,"after append block");
  PRINT_POINTER_INSPECTION(self,"after append block");
  ASSERT_INVARIANTS(self);
}
stub(2005-09-23-18-18-08) void
ucl_vector_append (ucl_vector_t target, const ucl_vector_t source)
{
  assert(ucl_vector_slot_dimension(target) ==
	 ucl_vector_slot_dimension(source));
  ucl_vector_append_block(target, ucl_vector_get_data_block(source));
}
stub(2005-09-23-18-18-05) void
ucl_vector_append_range (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range)
{
  ucl_block_t		block;


  assert(ucl_vector_slot_dimension(target) == ucl_vector_slot_dimension(source));
  assert(ucl_vector_range_is_valid(source, range));

  ucl_block_set(block,
		ucl_vector_index_to_slot(source, ucl_range_min(range)),
		(ucl_range_size(range) * ucl_vector_slot_dimension(source)));

  ucl_vector_append_block(target, block);
}
stub(2005-09-23-18-18-10) void
ucl_vector_append_more (ucl_vector_t target, const ucl_vector_t source, ...)
{
  const ucl_vector_struct_t *	other_source;
  va_list			ap;


  ucl_vector_append(target, source);

  va_start(ap, source);
  for (other_source = va_arg(ap, ucl_vector_struct_t *);
       (NULL != other_source);
       other_source = va_arg(ap, ucl_vector_struct_t *))
    {
      ucl_vector_append(target, other_source);
    }
  va_end(ap);
}
stub(2007-10-26-13-32-10) void
ucl_vector_append_more_from_array (ucl_vector_t target, const ucl_vector_array_t * vectors)
{
  size_t	total_number_of_new_slots = 0;


  for (size_t i=0; i<vectors->number_of_vectors; ++i)
    {
      total_number_of_new_slots += ucl_vector_size(vectors->vectors[i]);
    }
  ucl_vector_enlarge_for_slots(target, total_number_of_new_slots);

  for (size_t i=0; i<vectors->number_of_vectors; ++i)
    ucl_vector_append(target, vectors->vectors[i]);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: insertions.
 ** ----------------------------------------------------------*/

stub(2007-10-26-17-40-01) void
ucl_vector_insert_block	(ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block)
{
  ucl_byte_t *	insertion_slot;
  size_t	bytes_at_beginning	= compute_bytes_at_beginning(target);
  size_t	bytes_at_end		= compute_bytes_at_end(target);


  ucl_vector_enlarge_for_slots(target, block.len / target->slot_dimension);
  insertion_slot = ucl_vector_index_to_new_slot(target, offset);
  assert(insertion_slot);

  if (block.len <= bytes_at_beginning)
    {
      ucl_byte_t *	p = target->first_used_slot - block.len;
      size_t		size_of_block_to_move = insertion_slot - target->first_used_slot;

      memmove(p, target->first_used_slot, size_of_block_to_move);
      target->first_used_slot = p;
      insertion_slot = p + size_of_block_to_move;
    }
  else if (block.len <= bytes_at_end)
    {
      ucl_byte_t *	p = insertion_slot + block.len;
      size_t		size_of_block_to_move = target->last_used_slot + target->slot_dimension - insertion_slot;

      memmove(p, insertion_slot, size_of_block_to_move);
      target->last_used_slot += size_of_block_to_move;
    }
  else
    {
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
stub(2007-10-26-17-31-42) void
ucl_vector_insert_range (ucl_vector_t V, ucl_vector_index_t offset, const ucl_vector_t A, ucl_range_t range)
{
  ucl_block_t	block;


  assert(V->slot_dimension == A->slot_dimension);
  assert(ucl_vector_range_is_valid(A, range));

  block.ptr = ucl_vector_index_to_slot(A, ucl_range_min(range));
  block.len = A->slot_dimension * ucl_range_size(range);
  ucl_vector_insert_block(V, offset, block);
}
stub(2005-09-23-18-18-14) void
ucl_vector_insert_vector (ucl_vector_t T, ucl_vector_index_t offset, const ucl_vector_t S)
{
  ucl_block_t	block;

  assert(T->slot_dimension == S->slot_dimension);
  block = ucl_vector_get_data_block(S);
  ucl_vector_insert_block(T, offset, block);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: erasures.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-18-16) void
ucl_vector_erase_range (ucl_vector_t self, ucl_range_t index_range)
{
  ucl_byte_pointer_range_t	slot_range;
  ucl_bool_t			room_at_the_end, room_at_the_beginning;


  ASSERT_INVARIANTS(self);
  assert(ucl_vector_range_is_valid(self, index_range));

  if (0 == ucl_range_size(index_range))
    {
      return;
    }
  ucl_debug("range first index %d, range last index %d",
	    ucl_range_min(index_range), ucl_range_max(index_range));
  ucl_range_set_min_max(slot_range,
			ucl_vector_index_to_slot(self,ucl_range_min(index_range)),
			ucl_vector_index_to_slot(self,ucl_range_max(index_range)));
  ucl_debug("range byte size %d",
	    ucl_range_size(slot_range) + self->slot_dimension - 1);

  room_at_the_beginning	= there_is_room_at_the_beginning(self);
  room_at_the_end	= there_is_room_at_the_end(self);
  if (room_at_the_beginning && room_at_the_end)
    {
      ucl_debug("there is room at the beginning and at the end");
      ucl_debug("bytes at beg %d, bytes at end %d",
		compute_bytes_at_beginning(self), compute_bytes_at_end(self));
      if (compute_bytes_at_beginning(self) < compute_bytes_at_end(self))
	{
	  goto move_head_data_toward_the_end;
	}
      else
	{ goto move_tail_data_toward_the_beginning; }
    }
  else if (room_at_the_beginning)
    {
      ucl_debug("there is room at the beginning, bytes %d",
		compute_bytes_at_beginning(self));
      goto move_tail_data_toward_the_beginning;
    }
  else
    {
      ucl_debug("there is room at the end, bytes %d",
		compute_bytes_at_end(self));
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

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: accessors.
 ** ----------------------------------------------------------*/

#undef NDEBUG
stub(2007-10-26-21-32-41) void
ucl_vector_copy_range (ucl_vector_t target, ucl_vector_index_t position, ucl_vector_t source, ucl_range_t source_range)
{
  ucl_block_t	block;


  assert(target->slot_dimension == target->slot_dimension);
  assert(ucl_vector_index_is_valid(target, position));
  assert(ucl_vector_range_is_valid(source, source_range));
  assert((ucl_vector_size(target) - position) >= ucl_range_size(source_range));

  block = ucl_vector_block_from_range(source, source_range);
  ucl_vector_set_block(target, position, block);
}
stub(2007-10-26-21-32-52) void
ucl_vector_set_block (ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source)
{
  void *	slot;


  assert(ucl_vector_index_is_valid(target, position));
  assert((source.len / target->slot_dimension) <= (ucl_vector_size(target) - position));

  slot = ucl_vector_index_to_slot(target, position);
  memcpy(slot, source.ptr, source.len);
}
stub(2007-10-26-21-33-04) void
ucl_vector_get_block (ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source)
{
  void *	slot;


  assert(ucl_vector_index_is_valid(source, position));
  assert((target.len / source->slot_dimension) <= (ucl_vector_size(source) - position));

  slot = ucl_vector_index_to_slot(source, position);
  memcpy(target.ptr, slot, target.len);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: comparing.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-18-19) int
ucl_vector_compare_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  ucl_byte_t *		pa;
  ucl_byte_t *		pb;
  ucl_vector_index_t	i;
  ucl_valcmp_t		compar;
  ucl_value_t		va, vb;
  int			result=0;
  

  if (ucl_range_size(ra) > ucl_range_size(rb))
    {
      ucl_range_set_size_on_min(ra, ucl_range_size(rb));
    }
  compar = a->compar;

  for (i=0; i<ucl_range_size(ra); ++i)
    {
      pa = ucl_vector_index_to_slot(a, ucl_range_min(ra)+i);
      pb = ucl_vector_index_to_slot(b, ucl_range_min(rb)+i);
      ucl_debug("%d %d", *pa, *pb);
      va.ptr = pa;
      vb.ptr = pb;
      result = compar.func(compar.data, va, vb);
      if (result != 0)
	{
	  return result;
	}
    }
  return result;
}
stub(2005-09-23-18-18-23) int
ucl_vector_compare (const ucl_vector_t a, const ucl_vector_t b)
{
  ucl_range_t	range_a, range_b;

  ucl_range_set_min_size(range_a, 0, ucl_vector_size(a));
  ucl_range_set_min_size(range_b, 0, ucl_vector_size(b));
  return ucl_vector_compare_range(a, range_a, b, range_b);
}
stub(2005-09-23-18-18-25) ucl_bool_t
ucl_vector_equal (const ucl_vector_t a, const ucl_vector_t b)
{
  return ((ucl_vector_size(a) == ucl_vector_size(b)) &&
	  (ucl_vector_compare(a, b) == 0));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: for each.
 ** ----------------------------------------------------------*/

stub(2007-10-24-13-25-44) void
ucl_vector_for_each (ucl_callback_t callback, const ucl_vector_t self)
{
  ucl_iterator_t	I;
  ucl_value_t		slot;


  for (ucl_vector_iterator_forward(self, I); ucl_iterator_more(I); ucl_iterator_next(I))
    {
      slot.ptr = ucl_iterator_ptr(I);;
      ucl_callback_invoke(callback, slot);
    }
}
stub(2007-10-24-13-26-16) void
ucl_vector_for_each_in_range (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self)
{
  ucl_value_t	data;
  void *	slot;


  assert(ucl_vector_range_is_valid(self, range));
  
  slot = ucl_vector_index_to_slot(self, ucl_range_min(range));
  for (size_t i = ucl_range_min(range);
       i <= ucl_range_max(range);
       ++i, slot = ucl_vector_increment_slot(self, slot))
    {
      data.ptr = slot;
      ucl_callback_invoke(callback, data);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: multiple for each.
 ** ----------------------------------------------------------*/

stub(2007-10-24-13-25-52) void
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

  for (slots_array.data.unum = 0;; ++slots_array.data.unum)
    {
      for (i=0; i<vectors->number_of_vectors; ++i)
	if (! ucl_iterator_more(iterators[i]))
	  goto exit;

      for (i=0; i<slots_array.number_of_slots; ++i)
	slots[i] = ucl_iterator_ptr(iterators[i]);

      ucl_callback_invoke(callback, data);

      for (i=0; i<vectors->number_of_vectors; ++i)
	ucl_iterator_next(iterators[i]);
    }
 exit:
  return;
}
stub(2007-10-24-13-26-01) void
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
    ucl_vector_struct_t *	V;
    
    va_start(save, first);
    for (V = va_arg(save, void *); V; V = va_arg(save, void *))
      ++counter;
    va_end(save);
  }
  
  /* Store the vectors in an array, then do the iteration. */
  {
    const ucl_vector_struct_t *	vectors[counter];  
    ucl_vector_array_t		array = {
      .number_of_vectors = counter,
      .vectors		 = (ucl_vector_struct_t **)vectors
    };


    vectors[0] = first;
    va_start(ap, first);
    for (size_t i=1; i<counter; ++i)
      vectors[i] = va_arg(ap, void *);
    va_end(ap);

    ucl_vector_for_each_multiple_from_array(callback, &array);
  }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: mapping.
 ** ----------------------------------------------------------*/

stub(2007-10-24-13-26-07) void
ucl_vector_map (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self)
{
  void *			slots[2];
  ucl_array_of_pointers_t	slots_array = {
    .number_of_slots	= 2,
    .slots		= slots
  };
  ucl_value_t			data = { .ptr = &slots_array };
  ucl_iterator_t		I;


  for (ucl_vector_iterator_forward(self, I), slots_array.data.unum = 0;
       ucl_iterator_more(I);
       ucl_iterator_next(I), ++slots_array.data.unum)
    {
      slots[0] = ucl_vector_push_back(result);
      slots[1] = ucl_iterator_ptr(I);
      ucl_callback_invoke(callback, data);
    }
}
stub(2007-10-24-13-26-25) void
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
       ucl_iterator_more(I);
       ucl_iterator_next(I), ++slots_array.data.unum)
    {
      slots[0] = ucl_vector_push_back(result);
      slots[1] = ucl_iterator_ptr(I);
      ucl_callback_invoke(callback, data);
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** High level functions: multiple map.
 ** ----------------------------------------------------------*/

stub(2007-10-26-07-45-30) void
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

  for (slots_array.data.unum = 0;; ++slots_array.data.unum)
    {
      for (i=0; i<vectors->number_of_vectors; ++i)
	if (! ucl_iterator_more(iterators[i]))
	  goto exit;

      slots[0] = ucl_vector_push_back(result);
      for (i=0; i<vectors->number_of_vectors; ++i)
	slots[1+i] = ucl_iterator_ptr(iterators[i]);

      ucl_callback_invoke(callback, data);

      for (i=0; i<vectors->number_of_vectors; ++i)
	ucl_iterator_next(iterators[i]);
    }
 exit:
  return;
}
stub(2007-10-26-07-45-36) void
ucl_vector_map_multiple (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...)
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
    ucl_vector_struct_t *	V;
    
    va_start(save, first);
    for (V = va_arg(save, void *); V; V = va_arg(save, void *))
      ++counter;
    va_end(save);
  }
  
  /* Store the vectors in an array, then do the iteration. */
  {
    const ucl_vector_struct_t *	vectors[counter];  
    const ucl_vector_array_t	array = {
      .number_of_vectors = counter,
      .vectors		 = (ucl_vector_struct_t **)vectors
    };


    vectors[0] = first;
    va_start(ap, first);
    for (size_t i=1; i<counter; ++i)
      vectors[i] = va_arg(ap, void *);
    va_end(ap);

    ucl_vector_map_multiple_from_array(result, callback, &array);
  }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Private module functions.
 ** ----------------------------------------------------------*/

static inline void
set_pointers_for_empty_vector	(ucl_vector_t self)
{
  ucl_byte_t *	potential_first_used_slot;

  /* do not assert emptyness here,  because this function may be used to
     force the emptyness state */
  potential_first_used_slot = self->first_allocated_slot + self->size_of_padding_area;
  if (potential_first_used_slot < self->last_allocated_slot)
    {
      self->first_used_slot = potential_first_used_slot;
      self->last_used_slot  = self->first_used_slot - self->slot_dimension;
      ASSERT_FIRST_SLOT_IS_AT_PAD_OFFSET(self);
    }
  else
    {
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
  if (! vector_is_empty(self))
    { return (self->last_used_slot < self->last_allocated_slot); }
  else /* vector is empty */
    { return (self->first_used_slot <= self->last_allocated_slot); }
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
  if (! vector_is_empty(self))
    {
      /*
	(last_allocated_slot + slot_dimension) - (last_used_slot + slot_dimension) =
	(last_allocated_slot - last_used_slot)
       */
      return (self->last_allocated_slot - self->last_used_slot);
    }
  else /* vector is empty */
    {
      /*
	(last_allocated_slot + slot_dimension) - (first_used_slot + slot_dimension)
	+ slot_dimension =
	(last_allocated_slot - first_used_slot + slot_dimension)
       */
      ucl_debug("there");
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

/* ------------------------------------------------------------ */


static void
move_used_bytes_at_pad_area_beginning (ucl_vector_t self)
{
  ASSERT_INVARIANTS(self);
  if (vector_is_empty(self))
    {
      set_pointers_for_empty_vector(self);
      ASSERT_INVARIANTS(self);
    }
  else
    {
      ucl_byte_t *	p;


      if (self->size_of_padding_area < compute_free_bytes(self))
	{
	  p = self->first_allocated_slot + self->size_of_padding_area;
	}
      else
	{
	  p = self->first_allocated_slot + \
	    (self->slot_dimension * (compute_free_slots(self) >> 1));
	}
      move_used_bytes_at_byte_pointer(self, p);
      ASSERT_INVARIANTS(self);
    }
}

/* ------------------------------------------------------------ */


static void
move_used_bytes_leave_requested_at_end (ucl_vector_t self, size_t bytes_requested_at_end)
{
  assert(bytes_requested_at_end > 0);
  if (vector_is_empty(self))
    {
      ASSERT_INVARIANTS(self);
      set_pointers_for_empty_vector(self);
      if (bytes_requested_at_end > compute_bytes_at_end(self))
	{
	  self->first_used_slot = self->first_allocated_slot;
	  self->last_used_slot  = self->first_used_slot - self->slot_dimension;
	}
    }
  else
    {
      size_t	number_of_free_bytes = compute_free_bytes(self);

      ASSERT_INVARIANTS(self);
      assert(bytes_requested_at_end <= number_of_free_bytes);

      ucl_debug("moving? %s",
		ucl_question(bytes_requested_at_end > compute_bytes_at_end(self)));
      if (bytes_requested_at_end > compute_bytes_at_end(self))
	{
	  size_t	bytes_at_end_after_moving_on_padding;


	  bytes_at_end_after_moving_on_padding = (number_of_free_bytes > self->size_of_padding_area)?
	    (number_of_free_bytes - self->size_of_padding_area) : number_of_free_bytes;
	  
	  if (bytes_at_end_after_moving_on_padding > bytes_requested_at_end)
	    {
	      ucl_debug("moving bytes at pad area");
	      move_used_bytes_at_pad_area_beginning(self);
	      ASSERT_INVARIANTS(self);
	    }
	  else
	    {
	      ucl_byte_t *	p;
	      size_t		slots_requested_at_end = \
		bytes_requested_at_end / self->slot_dimension;


	      ucl_debug("leaving half slots at beginning and half at end");
	      p = self->first_allocated_slot + self->slot_dimension * \
		((compute_free_slots(self) - slots_requested_at_end) >> 1);
	      ucl_debug("moving bytes at %p", p);
	      move_used_bytes_at_byte_pointer(self, p);
	      PRINT_SLOT_INSPECTION(self,"after moving bytes");
	      ASSERT_INVARIANTS(self);
	    }
	}
    }
}

/* ------------------------------------------------------------ */


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
  ucl_byte_t *	p = self->last_allocated_slot -
    compute_used_bytes(self) - self->slot_dimension;
  move_used_bytes_at_byte_pointer(self, p);
}
static inline void
move_used_bytes_at_index_offset (ucl_vector_t self, ucl_vector_index_t offset)
{
  ucl_byte_t *	p = ucl_vector_index_to_slot(self, offset);
  move_used_bytes_at_byte_pointer(self, p);
}
static inline void
move_used_bytes_at_byte_pointer (ucl_vector_t self, ucl_byte_t *p)
{
  if (p != self->first_used_slot)
    {
      memmove(p, self->first_used_slot, compute_used_bytes(self));
      self->last_used_slot += p - self->first_used_slot;
      self->first_used_slot = p;
      PRINT_POINTER_INSPECTION(self,"after moving bytes");
    }
}


/* end of file */
