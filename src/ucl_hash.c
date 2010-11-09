/*
   Part of: Useless Containers Library
   Contents: hash table implementation
   Date: Thu Mar  6, 2003

   Abstract



   Copyright (c) 2003-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** --------------------------------------------------------------------
 ** Headers.
 ** -----------------------------------------------------------------*/

#define DEBUGGING		0
#include "internal.h"

#define stubmodule		hash

/* Values   for    the   "to_be_processed_during_rehashing"   field   of
   "ucl_entry_t". */
#define YES			0xff
#define NO			0x00

typedef ucl_hash_table_t	hash_t;
typedef ucl_hash_entry_t	entry_t;
typedef ucl_value_t		value_t;
typedef ucl_iterator_t		iterator_t;
typedef ucl_hash_fun_t		hashfun_t;

static ucl_iterator_next_t	iterator_next;


/** --------------------------------------------------------------------
 ** Construction.
 ** -----------------------------------------------------------------*/

void
ucl_hash_initialise (ucl_hash_table_t self, ucl_vector_t buckets,
		     ucl_value_comparison_t compar, ucl_hash_t hash)
{
  /* ucl_vector_config_t	config; */
  assert(self);
  assert(hash.func);
  assert(compar.func);
  /* ucl_vector_initialise_config_hash(config); */
  self->buckets			= buckets;
  self->size			= 0;
  self->number_of_used_buckets	= 0;
  self->hash			= hash;
  self->compar			= compar;
}


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static entry_t **
compute_bucket_pointer_for_key (const ucl_hash_table_t this,
				const ucl_value_t key)
{
  size_t	bucket_index;
  bucket_index = (this->hash.func(this->hash.data, key)) %
    ucl_vector_size(this->buckets);
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


/** --------------------------------------------------------------------
 ** Insertion.
 ** -----------------------------------------------------------------*/

void
ucl_hash_insert (ucl_hash_table_t this, ucl_hash_entry_t *entry_p)
{
  entry_t **	bucket_p;
  entry_t *	first_entry_in_list_p;
  entry_t *	second_entry_in_list_p;


  assert(this); assert(entry_p);

  bucket_p = compute_bucket_pointer_for_key(this, entry_p->key);
#if 0
  debug("bucket index %d, %p", ucl_vector_slot_to_index(this->buckets, bucket_p), bucket_p);
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


/** ------------------------------------------------------------
 ** Extraction.
 ** ----------------------------------------------------------*/

void
ucl_hash_extract (ucl_hash_table_t this, ucl_hash_entry_t *entry_p)
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


/** ------------------------------------------------------------
 ** Search.
 ** ----------------------------------------------------------*/

ucl_hash_entry_t *
ucl_hash_find (const ucl_hash_table_t this, const ucl_value_t key)
{
  ucl_value_comparison_t	compar;
  entry_t **	bucket_p;
  entry_t *	entry_in_list_p;


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
ucl_hash_entry_t *
ucl_hash_first (const ucl_hash_table_t this)
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


/** ------------------------------------------------------------
 ** Rehashing.
 ** ----------------------------------------------------------*/

static void
rehash (ucl_hash_table_t this, size_t rehash_number_of_buckets)
{
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

  /* Traverse the  vector of  buckets and the  linked lists  of entries,
     rehashing each of them.

     For each bucket: extract each entry one after the other and:

     - mark it as already processed in the rehashing operation;
     - compute the new bucket index;
     - append the entry  to the linked list, after  all the entries that
       have to be processed.

     That   way,   while  rehashing,   each   linked   list  will   have
     already-processed entries  at the end,  and to-be-processed entries
     at the  beginning; when  traversing such a  linked list:  the first
     entry marked as already  processed marks the end of to-be-processed
     entries.

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

	    bucket_index = this->hash.func(this->hash.data, entry_p->key) % rehash_number_of_buckets;
	    new_bucket_p = ucl_vector_index_to_slot(this->buckets, bucket_index);
	    append_entry_to_the_end(new_bucket_p, entry_p);
	  }
      }
  }
}


/** ------------------------------------------------------------
 ** Reallocation: enlarging.
 ** ----------------------------------------------------------*/

void
ucl_hash_enlarge (ucl_hash_table_t this)
{
  assert(this);
  debug("enter: old size %u", ucl_vector_size(this->buckets));

  ucl_vector_enlarge_for_slots(this->buckets, ucl_vector_number_of_step_up_slots(this->buckets));

  /* Set to NULL all the new buckets.  Here we rely on the fact that the
     vector module does not move around used slots when reallocating. So
     the new slots are appended at the end. */
  {
    entry_t **	bucket_p = ucl_vector_back(this->buckets);

    memset(++bucket_p, '\0', sizeof(void *) * ucl_vector_number_of_free_slots(this->buckets));
    ucl_vector_mark_all_slots_as_used(this->buckets);
  }

  rehash(this, ucl_vector_size(this->buckets));
  debug("leaving: new size %u\n", ucl_vector_size(this->buckets));
}


/** ------------------------------------------------------------
 ** Reallocation: restricting.
 ** ----------------------------------------------------------*/

void
ucl_hash_restrict (ucl_hash_table_t this)
{
  assert(this);
  debug("enter: old size %u", ucl_vector_size(this->buckets));

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
	    debug("normalising because new size %u smaller than limit %u",
		      new_number_of_buckets, UCL_HASH_MINIMUM_SIZE);
	    new_number_of_buckets = UCL_HASH_MINIMUM_SIZE;
	  }
      }
      debug("selected new size %u", new_number_of_buckets);

      rehash(this, new_number_of_buckets);

      {
	ucl_range_t	range;

	/* ranges are inclusive                            !!vv!! */
	ucl_range_set_min_max(range, 0, new_number_of_buckets-1);
	ucl_vector_mark_allocated_range_as_used(this->buckets, range);
	ucl_vector_restrict(this->buckets);
      }
    }
  debug("leaving: new size %u\n", ucl_vector_size(this->buckets));
}


/** ------------------------------------------------------------
 ** Iteration.
 ** ----------------------------------------------------------*/

void
ucl_hash_iterator (const ucl_hash_table_t this, ucl_iterator_t iterator)
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
	  debug("bucket index %d",
		    ucl_vector_slot_to_index(this->buckets, bucket_p));
#endif
	  if (*bucket_p)
	    {
	      iterator->iterator = *bucket_p;
	      iterator->internal1.pointer = bucket_p;
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
  const ucl_hash_table_tag_t *	this;
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
      bucket_p = iterator->internal1.pointer;
      this     = (const ucl_hash_table_tag_t *) iterator->container;

      /* advance to the next bucket; if it is not the last examine it */
#if 0
      debug("old bucket index %d",  ucl_vector_slot_to_index(this->buckets, bucket_p));
      debug("last bucket index %d", ucl_vector_slot_to_index(this->buckets, ucl_vector_back(this->buckets)));
#endif
      for (++bucket_p;
	   bucket_p <= (entry_t **)ucl_vector_back(this->buckets);
	   ++bucket_p)
	{
#if 0
	  debug("bucket index %d %p",
		    ucl_vector_slot_to_index(this->buckets, bucket_p), bucket_p);
#endif
	  if (*bucket_p)
	    {
	      iterator->iterator = *bucket_p;
	      iterator->internal1.pointer = bucket_p;
	      return;
	    }
	}
      iterator->iterator = NULL;
    }
}


/** ------------------------------------------------------------
 ** Statistics.
 ** ----------------------------------------------------------*/

size_t
ucl_hash_number_of_used_buckets (const ucl_hash_table_t this)
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
size_t
ucl_hash_bucket_chain_length (const ucl_hash_table_t this, ucl_vector_index_t position)
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
double
ucl_hash_average_search_distance (const ucl_hash_table_t this)
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
