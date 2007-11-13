/* ucl_hash.c --
   
   Part of: Useless Containers Library
   Contents: hash table implementation
   Date: Thu Mar  6, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005, 2007 Marco Maggi
   
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
 ** Headers
 ** ----------------------------------------------------------*/

#define UCL_DEBUGGING		0
#include "ucl_defines.h"

/* stub module name hash */

/* Values   for    the   "to_be_processed_during_rehashing"   field   of
   "ucl_entry_t". */
#define YES			0xff
#define NO			0x00

/* ------------------------------------------------------------ */

typedef ucl_hash_t		hash_t;
typedef ucl_hash_entry_t	entry_t;
typedef ucl_value_t		value_t;
typedef ucl_iterator_t		iterator_t;

typedef ucl_valcmp_t		valcmp_t;
typedef ucl_hashfun_t		hashfun_t;

/* ------------------------------------------------------------ */

static ucl_iterator_next_t	iterator_next;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Construction.
 ** ----------------------------------------------------------*/

stub(2007-11-12-10-03-54) void
ucl_hash_initialise (ucl_hash_t this, ucl_valcmp_t compar, ucl_hashcmp_t hash)
{
  assert(this);
  assert(hash.func);
  assert(compar.func);

  ucl_vector_initialise(this->buckets, sizeof(entry_t **));
  ucl_vector_initialise_size	 (this->buckets, UCL_HASH_DEFAULT_SIZE);
  ucl_vector_initialise_pad	 (this->buckets, 0);
  ucl_vector_initialise_step_up	 (this->buckets, UCL_HASH_DEFAULT_STEP_UP);
  ucl_vector_initialise_step_down(this->buckets, UCL_HASH_DEFAULT_STEP_DOWN);

  this->hash	= hash;
  this->compar	= compar;
}
stub(2005-09-23-18-10-40) void
ucl_hash_constructor (ucl_hash_t this)
{
  assert(this);

  ucl_vector_initialise_pad(this->buckets, 0);
  ucl_vector_constructor(this->buckets);
  ucl_vector_mark_all_slots_as_used(this->buckets);
  ucl_vector_set_memory_to_zero(this->buckets);
  this->size	= 0;
  this->number_of_used_buckets = 0;
}
stub(2005-09-23-19-07-39) void
ucl_hash_destructor (ucl_hash_t this)
{
  assert(this);
  ucl_vector_destructor(this->buckets);
  ucl_struct_clean(this, ucl_hash_struct_t);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static entry_t **
compute_bucket_pointer_for_key (const ucl_hash_t this, const ucl_value_t key)
{
  size_t	bucket_index;
  ucl_hashcmp_t	hash = this->hash;
  
  bucket_index = (hash.func(hash.data, key)) % ucl_vector_size(this->buckets);
  return ucl_vector_index_to_slot(this->buckets, bucket_index);
}
static void
append_entry_to_the_end(entry_t ** bucket_p, entry_t *entry_p)
{
  entry_t *	entry_in_list_p;
  entry_t *	next_entry_in_list_p;


  entry_in_list_p = *bucket_p;
  if (entry_in_list_p)
    {
      next_entry_in_list_p = entry_in_list_p->next_entry_in_list_p;
      while (next_entry_in_list_p &&
	     next_entry_in_list_p->to_be_processed_during_rehashing)
	{
	  entry_in_list_p = next_entry_in_list_p;
	  next_entry_in_list_p = entry_in_list_p->next_entry_in_list_p;
	}
      /*
	Here "entry_in_list_p" references the last to-be-processed entry
	in  the   linked  list.  Its   next  field  may  be   the  first
	already-processed entry or NULL.
      */
      entry_p->next_entry_in_list_p = entry_in_list_p->next_entry_in_list_p;
      entry_in_list_p->next_entry_in_list_p = entry_p;
    }
  else
    {
      *bucket_p = entry_p;
      entry_p->next_entry_in_list_p = NULL;
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Insertion.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-10-46) void
ucl_hash_insert (ucl_hash_t this, ucl_hash_entry_t *entry_p)
{
  entry_t **	bucket_p;
  entry_t *	first_entry_in_list_p;
  entry_t *	second_entry_in_list_p;
  

  assert(this); assert(entry_p);
  
  bucket_p = compute_bucket_pointer_for_key(this, entry_p->key);
#if 0
  ucl_debug("bucket index %d, %p", ucl_vector_slot_to_index(this->buckets, bucket_p), bucket_p);
#endif
  first_entry_in_list_p = *bucket_p;
  if (first_entry_in_list_p)
    {
      second_entry_in_list_p = first_entry_in_list_p;
    }
  else
    {
      second_entry_in_list_p = NULL;
      ++(this->number_of_used_buckets);
    }

  *bucket_p = entry_p;
  entry_p->next_entry_in_list_p = second_entry_in_list_p;
  entry_p->to_be_processed_during_rehashing = NO;
  ++(this->size);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Extraction.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-10-48) void
ucl_hash_extract (ucl_hash_t this, ucl_hash_entry_t *entry_p)
{
  entry_t **	bucket_p;
  entry_t *	entry_in_list_p;


  assert(this); assert(entry_p);

  bucket_p = compute_bucket_pointer_for_key(this, entry_p->key);
  entry_in_list_p = *bucket_p;
  if (entry_in_list_p == entry_p)
    {
      if (entry_in_list_p->next_entry_in_list_p)
	{
	  *bucket_p = entry_in_list_p->next_entry_in_list_p;
	}
      else
	{
	  *bucket_p = NULL;
	  --(this->number_of_used_buckets);
	}
    }
  else /* first entry != entry_p */
    {
      while (entry_in_list_p->next_entry_in_list_p != entry_p)
	{
	  entry_in_list_p = entry_in_list_p->next_entry_in_list_p;
	}
      entry_in_list_p->next_entry_in_list_p = entry_p->next_entry_in_list_p;
    }
  
  --(this->size);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Search.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-10-50) ucl_hash_entry_t *
ucl_hash_find (const ucl_hash_t this, const ucl_value_t key)
{
  entry_t **	bucket_p;
  entry_t *	entry_in_list_p;
  ucl_valcmp_t	compar;


  assert(this);
  if (0 == ucl_hash_size(this)) { return NULL; }

  compar = this->compar;

  bucket_p = compute_bucket_pointer_for_key(this, key);
  entry_in_list_p = *bucket_p;
  while (entry_in_list_p)
    {
      if (0 == compar.func(compar.data, key, entry_in_list_p->key)) { break; }
      entry_in_list_p = entry_in_list_p->next_entry_in_list_p;
    }
  return entry_in_list_p;
}
stub(2007-11-12-10-33-25) ucl_hash_entry_t *
ucl_hash_first (const ucl_hash_t this)
{
  ucl_iterator_t	I;
  entry_t *		slot;

  assert(this);
  if (0 == ucl_hash_size(this))
    return NULL;

  for (ucl_vector_iterator_forward(this->buckets, I); ucl_iterator_more(I); ucl_iterator_next(I))
    {
      slot = ucl_iterator_ptr(I);
      if (slot) return *((entry_t **)slot);
    }
  return NULL;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Reallocation: enlarging.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-10-51) void
ucl_hash_enlarge (ucl_hash_t this)
{
  assert(this);
  ucl_debug("enter: old size %u", ucl_vector_size(this->buckets));


  ucl_vector_enlarge_for_slots(this->buckets, ucl_vector_number_of_step_up_slots(this->buckets));

  /* Set to NULL all the new buckets.  Here we rely on the fact that the
     vector module does not move around used slots when reallocating. So
     the new slots are appended at the end. */
  {
    entry_t **	bucket_p = ucl_vector_back(this->buckets);

    memset(++bucket_p, '\0', sizeof(void *) * ucl_vector_number_of_free_slots(this->buckets));
    ucl_vector_mark_all_slots_as_used(this->buckets);
  }
      
  /* Mark all the entries to be processed while rehashing. */
  {
    iterator_t	iterator;
    entry_t *	entry_p;

    for (ucl_hash_iterator(this, iterator);
	 ucl_iterator_more(iterator);
	 ucl_iterator_next(iterator))
      {
	entry_p = ucl_iterator_ptr(iterator);
	entry_p->to_be_processed_during_rehashing = YES;
      }
  }
      
  /* Traverse the vector of buckets and the linked lists of entries,
     rehashing each of them.

     For each bucket: extract each entry one after the other and:
	 
     - mark it as already processed in the rehashing operation;
     - compute the new bucket index;
     - append the  entry to the  linked list, after all  the entries
     that have to be processed.

     That  way,   while  rehashing,  each  linked   list  will  have
     already-processed  entries  at  the  end,  and  to-be-processed
     entries at  the beginning; when traversing such  a linked list:
     the first  entry marked as  already processed marks the  end of
     to-be-processed entries.

     *  ------
     * |bucket| 
     *  ------    -----------    -----------    -------------    -------------
     * |bucket|->|entry TO BE|->|entry TO BE|->|entry ALREADY|->|entry ALREADY|->NULL
     *  ------   |rehashed   |  |rehashed   |  |rehashed     |  |rehashed     |
     * |bucket|   -----------    ----------- ^  -------------    -------------
     *  ------                               |
     *                                       | the next rehashed entry goes here
     *                                 --------------
     *                                |rehashed entry|
     *                                 --------------
     */
  {
    iterator_t	iterator;
    ucl_hashcmp_t	hash = this->hash;
    size_t		bucket_index;
    entry_t **	bucket_p;
    entry_t **	new_bucket_p;
    entry_t *	entry_p;


    for (ucl_vector_iterator_forward(this->buckets, iterator);
	 ucl_iterator_more(iterator);
	 ucl_iterator_next(iterator))
      {
	bucket_p = ucl_iterator_ptr(iterator);
	while ((entry_p = *bucket_p) && (entry_p->to_be_processed_during_rehashing))
	  {
	    *bucket_p = entry_p->next_entry_in_list_p;

	    entry_p->to_be_processed_during_rehashing = NO;

	    bucket_index = hash.func(hash.data, entry_p->key) % ucl_vector_size(this->buckets);
	    new_bucket_p = ucl_vector_index_to_slot(this->buckets, bucket_index);
	    append_entry_to_the_end(new_bucket_p, entry_p);
	  }
      }
  }
  ucl_debug("leaving: new size %u\n", ucl_vector_size(this->buckets));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Reallocation: restricting.
 ** ----------------------------------------------------------*/

stub(2007-11-12-18-19-55) void
ucl_hash_restrict (ucl_hash_t this)
{

  assert(this);
  ucl_debug("enter: old size %u", ucl_vector_size(this->buckets));

  if (ucl_vector_size(this->buckets) > ucl_vector_number_of_step_down_slots(this->buckets))
    {
      size_t	new_number_of_buckets;

      /* Compute the new number of buckets. */
      {
	size_t	number_of_step_down_buckets = this->buckets->step_down / this->buckets->slot_dimension;	

	new_number_of_buckets = ucl_vector_size(this->buckets) - number_of_step_down_buckets;
	assert(0 < new_number_of_buckets);

	if (new_number_of_buckets < UCL_HASH_MINIMUM_SIZE)
	  {
	    ucl_debug("normalising because new size %u smaller than limit %u",
		      new_number_of_buckets, UCL_HASH_MINIMUM_SIZE);
	    new_number_of_buckets = UCL_HASH_MINIMUM_SIZE;
	  }
      }
      ucl_debug("selected new size %u", new_number_of_buckets);

      /* Mark all the entries to be processed while rehashing. */
      {
	iterator_t	iterator;
	entry_t *	entry_p;

	for (ucl_hash_iterator(this, iterator);
	     ucl_iterator_more(iterator);
	     ucl_iterator_next(iterator))
	  {
	    entry_p = ucl_iterator_ptr(iterator);
	    entry_p->to_be_processed_during_rehashing = YES;
	  }
      }
      
      /* Traverse the vector of buckets and the linked lists of entries,
	 rehashing each of them.

	 For each bucket: extract each entry one after the other and:
	 
	 - mark it as already processed in the rehashing operation;
	 - compute the new bucket index;
	 - append the  entry to the  linked list, after all  the entries
	   that have to be processed.

	 That  way,   while  rehashing,  each  linked   list  will  have
	 already-processed  entries  at  the  end,  and  to-be-processed
	 entries at  the beginning; when traversing such  a linked list:
	 the first entry marked as  already processed marks the end of p
	 to-be-processed entries.

	 *  ------
	 * |bucket| 
	 *  ------    -----------    -----------    -------------    -------------
	 * |bucket|->|entry TO BE|->|entry TO BE|->|entry ALREADY|->|entry ALREADY|->NULL
	 *  ------   |rehashed   |  |rehashed   |  |rehashed     |  |rehashed     |
	 * |bucket|   -----------    ----------- ^  -------------    -------------
	 *  ------                               |
	 *                                       | the next rehashed entry goes here
	 *                                 --------------
	 *                                |rehashed entry|
	 *                                 --------------
	 */
  
      {
	ucl_hashcmp_t	hash = this->hash;
	iterator_t		iterator;
	size_t		bucket_index;
	entry_t **		bucket_p;
	entry_t **		new_bucket_p;
	entry_t *		entry_p;


	for (ucl_vector_iterator_forward(this->buckets, iterator);
	     ucl_iterator_more(iterator);
	     ucl_iterator_next(iterator))
	  {
	    bucket_p = ucl_iterator_ptr(iterator);
	    while ((entry_p = *bucket_p) && (entry_p->to_be_processed_during_rehashing))
	      {
		*bucket_p = entry_p->next_entry_in_list_p;

		entry_p->to_be_processed_during_rehashing = NO;

		bucket_index = hash.func(hash.data, entry_p->key) % new_number_of_buckets;
		new_bucket_p = ucl_vector_index_to_slot(this->buckets, bucket_index);
		append_entry_to_the_end(new_bucket_p, entry_p);
	      }
	  }
      }
  
      {
	ucl_range_t	range;

	/* ranges are inclusive                            !!vv!! */
	ucl_range_set_min_max(range, 0, new_number_of_buckets-1);
	ucl_vector_mark_allocated_range_as_used(this->buckets, range);
	ucl_vector_restrict(this->buckets);
      }
    }
  ucl_debug("leaving: new size %u\n", ucl_vector_size(this->buckets));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Iteration.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-10-55) void
ucl_hash_iterator (const ucl_hash_t this, ucl_iterator_t iterator)
{
  ucl_iterator_t	vector_iterator;
  entry_t **		bucket_p;	/* Pointer to the slot in the vector memory. */

  assert(this); assert(iterator);
  
  if (! ucl_hash_size(this))
    {
      iterator->iterator = NULL;
    }
  else
    {
      iterator->container = this;
      iterator->next	  = iterator_next;

      for (ucl_vector_iterator_forward(this->buckets, vector_iterator);
	   ucl_iterator_more(vector_iterator);
	   ucl_iterator_next(vector_iterator))
	{
	  bucket_p = ucl_iterator_ptr(vector_iterator);
#if 0
	  ucl_debug("bucket index %d",
		    ucl_vector_slot_to_index(this->buckets, bucket_p));
#endif
	  if (*bucket_p)
	    {
	      iterator->iterator = *bucket_p;
	      iterator->ptr1     = bucket_p;
	      return;
	    }
	}
      iterator->iterator = NULL;
    }
}

/* ------------------------------------------------------------ */

static void
iterator_next (ucl_iterator_t iterator)
{
  const ucl_hash_struct_t *	this;
  entry_t *		current_entry_p;
  entry_t **		bucket_p;	/* Pointer to the slot in the vector memory. */


  assert(iterator);

  current_entry_p = (entry_t *) iterator->iterator;
  current_entry_p = current_entry_p->next_entry_in_list_p;
  if (current_entry_p)
    {
      iterator->iterator = current_entry_p;
    }
  else
    {
      bucket_p = iterator->ptr1;
      this     = (const ucl_hash_struct_t *) iterator->container;

      /* advance to the next bucket; if it is not the last examine it */
#if 0
      ucl_debug("old bucket index %d",  ucl_vector_slot_to_index(this->buckets, bucket_p));
      ucl_debug("last bucket index %d", ucl_vector_slot_to_index(this->buckets, ucl_vector_back(this->buckets)));
#endif
      for (++bucket_p;
	   bucket_p <= (entry_t **)ucl_vector_back(this->buckets);
	   ++bucket_p)
	{
#if 0
	  ucl_debug("bucket index %d %p",
		    ucl_vector_slot_to_index(this->buckets, bucket_p), bucket_p);
#endif
	  if (*bucket_p)
	    {
	      iterator->iterator = *bucket_p;
	      iterator->ptr1     = bucket_p;
	      return;
	    }
	}
      iterator->iterator = NULL;
    }
}


/** ------------------------------------------------------------
 ** Statistics.
 ** ----------------------------------------------------------*/

stub(2007-11-13-12-44-55) size_t
ucl_hash_number_of_used_buckets (const ucl_hash_t this)
{
  ucl_iterator_t	iterator;
  size_t		number_of_used_buckets = 0;
  entry_t **		bucket_p;


  for (ucl_vector_iterator_forward(this->buckets, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator))
    {
      bucket_p = ucl_iterator_ptr(iterator);
      if (NULL != *bucket_p)
	++number_of_used_buckets;
    }
  return number_of_used_buckets;
}
stub(2007-11-13-13-16-16) size_t
ucl_hash_bucket_chain_length (const ucl_hash_t this, ucl_vector_index_t position)
{
  entry_t **	bucket_p;
  size_t	number_of_entries_in_chain = 0;


  bucket_p = ucl_vector_index_to_slot(this->buckets, position);
  if (*bucket_p)
    {
      entry_t *		entry_p = *bucket_p;

      ++number_of_entries_in_chain;
      while (entry_p->next_entry_in_list_p)
	{
	  ++number_of_entries_in_chain;
	  entry_p = entry_p->next_entry_in_list_p;
	}
    }
  
  return number_of_entries_in_chain;
}
stub(2007-11-13-13-27-52) double
ucl_hash_average_search_distance (const ucl_hash_t this)
{
  size_t	number_of_buckets = ucl_hash_number_of_buckets(this);
  double	average_search_distance = 0.0;

  
  for (size_t i=0; i<number_of_buckets; ++i)
    {
      average_search_distance += (double)ucl_hash_bucket_chain_length(this, i);
    }
  return average_search_distance/((double)number_of_buckets);
}


/* end of file */
