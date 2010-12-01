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

#define next_entry_in_list	bro

static ucl_iterator_next_t	iterator_next;


/** --------------------------------------------------------------------
 ** Small helpers.
 ** ----------------------------------------------------------------- */

static __inline__ __attribute__((__always_inline__,__nonnull__))
void
hash_table_size_incr (ucl_hash_table_t H)
{
  ++(H->size);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
hash_table_size_decr (ucl_hash_table_t H)
{
  --(H->size);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_value_t
entry_key (const ucl_hash_table_t H, void * E)
{
  return H->getkey.func(H->getkey.data, E);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
int
compare_key_entry (const ucl_hash_table_t H, ucl_value_t K, ucl_node_t E)
{
  return H->compar.func(H->compar.data, K, entry_key(H, E));
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_index_t
compute_key_hash (const ucl_hash_table_t H, ucl_value_t K)
{
  return H->hash.func(H->hash.data, K);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_index_t
compute_entry_hash (const ucl_hash_table_t H, ucl_node_t E)
{
  return H->hash.func(H->hash.data, entry_key(H, E));
}


void
ucl_hash_initialise (ucl_hash_table_t H, ucl_vector_t buckets, ucl_comparison_t compar,
		     ucl_hash_t hash, ucl_node_getkey_t getkey)
{
  assert(H);
  assert(hash.func);
  assert(compar.func);
  assert(getkey.func);
  H->buckets			= buckets;
  H->size			= 0;
  H->number_of_used_buckets	= 0;
  H->hash			= hash;
  H->getkey			= getkey;
  H->compar			= compar;
  ucl_vector_mark_all_slots_as_used(buckets);
  ucl_vector_set_memory_to_zero(buckets);
}

/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

static ucl_node_t *
compute_bucket_pointer_for_key (const ucl_hash_table_t H, const ucl_value_t key)
{
  ucl_index_t	hash_value   = compute_key_hash(H, key);
  ucl_index_t	bucket_index = hash_value % ucl_vector_size(H->buckets);
  return ucl_vector_index_to_slot(H->buckets, bucket_index);
}
static void
append_entry_to_the_end(ucl_node_t * bucket, ucl_node_t entry)
/* This function is used only while resizing the vector of buckets. */
{
  ucl_node_t	entry_in_list = *bucket;
  if (entry_in_list) { /* the bucket is used */
    ucl_node_t  next_entry_in_list = entry_in_list->next_entry_in_list;
    while (next_entry_in_list && next_entry_in_list->meta.to_be_processed_during_rehashing) {
      entry_in_list      = next_entry_in_list;
      next_entry_in_list = entry_in_list->next_entry_in_list;
    }
    /* Here "entry_in_list" references the last to-be-processed entry in
       the   linked   list.   Its   next   field   may   be  the   first
       already-processed entry or NULL. */
    entry->next_entry_in_list = entry_in_list->next_entry_in_list;
    entry_in_list->next_entry_in_list = entry;
  } else { /* the bucket is empty */
    *bucket = entry;
    entry->next_entry_in_list = NULL;
  }
}


void
ucl_hash_insert (ucl_hash_table_t H, void * E_)
{
  ucl_node_t	E = E_, first_entry_in_list, second_entry_in_list;
  ucl_node_t *	bucket;
  assert(H);
  assert(E);
  bucket = compute_bucket_pointer_for_key(H, entry_key(H, E));
  first_entry_in_list = *bucket;
  if (first_entry_in_list) {
    second_entry_in_list = first_entry_in_list;
  } else {
    second_entry_in_list = NULL;
    ++(H->number_of_used_buckets);
  }
  *bucket = E;
  E->next_entry_in_list = second_entry_in_list;
  E->meta.to_be_processed_during_rehashing = false;
  hash_table_size_incr(H);
}

void
ucl_hash_extract (ucl_hash_table_t H, void * E_)
{
  ucl_node_t	E = E_;
  ucl_node_t *	bucket;
  ucl_node_t 	entry_in_list;
  assert(H);
  assert(E);
  bucket = compute_bucket_pointer_for_key(H, entry_key(H, E));
  entry_in_list = *bucket;
  if (entry_in_list == E) {
    *bucket = (entry_in_list->next_entry_in_list)? entry_in_list->next_entry_in_list : NULL;
    --(H->number_of_used_buckets);
  } else { /* first entry != E */
    while (entry_in_list->next_entry_in_list != E)
      entry_in_list = entry_in_list->next_entry_in_list;
    entry_in_list->next_entry_in_list = E->next_entry_in_list;
  }
  hash_table_size_decr(H);
}

void *
ucl_hash_find (const ucl_hash_table_t H, const ucl_value_t key)
{
  ucl_node_t *	bucket;
  ucl_node_t 	entry_in_list;
  assert(H);
  if (0 == ucl_hash_size(H)) { return NULL; }
  bucket = compute_bucket_pointer_for_key(H, key);
  entry_in_list = *bucket;
  while (entry_in_list) {
    if (0 == compare_key_entry(H, key, entry_in_list)) break;
    entry_in_list = entry_in_list->next_entry_in_list;
  }
  return entry_in_list;
}
void *
ucl_hash_first (const ucl_hash_table_t H)
{
  ucl_iterator_t	I;
  ucl_node_t		slot;
  assert(H);
  if (ucl_hash_size(H))
    for (ucl_vector_iterator_forward(H->buckets, I); ucl_iterator_more(I); ucl_iterator_next(I)) {
      slot = ucl_iterator_ptr(I);
      if (slot) return *((ucl_node_t *)slot);
    }
  return NULL;
}

static void
rehash (ucl_hash_table_t H, size_t rehash_number_of_buckets)
{
  /* Mark all the entries to be processed while rehashing. */
  {
    ucl_iterator_t	I;
    ucl_node_t	 	entry;
    for (ucl_hash_iterator(H, I); ucl_iterator_more(I); ucl_iterator_next(I)) {
      entry = ucl_iterator_ptr(I);
      entry->meta.to_be_processed_during_rehashing = true;
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
    ucl_iterator_t	I;
    size_t		bucket_index;
    ucl_node_t *	bucket;
    ucl_node_t *	new_bucket;
    ucl_node_t 		E;
    for (ucl_vector_iterator_forward(H->buckets, I); ucl_iterator_more(I); ucl_iterator_next(I)) {
      bucket = ucl_iterator_ptr(I);
      while ((E = *bucket) && (E->meta.to_be_processed_during_rehashing)) {
	*bucket = E->next_entry_in_list;
	E->meta.to_be_processed_during_rehashing = false;
	bucket_index = compute_entry_hash(H, E) % rehash_number_of_buckets;
	new_bucket = ucl_vector_index_to_slot(H->buckets, bucket_index);
	append_entry_to_the_end(new_bucket, E);
      }
    }
  }
}

void
ucl_hash_enlarge (ucl_hash_table_t H)
{
  assert(H);
  ucl_vector_enlarge_for_slots(H->buckets, ucl_vector_number_of_step_up_slots(H->buckets));
  /* Set to NULL all the new buckets.  Here we rely on the fact that the
     vector module does not move around used slots when reallocating. So
     the new slots are appended at the end. */
  {
    ucl_node_t *	bucket = ucl_vector_back(H->buckets);
    memset(++bucket, '\0', sizeof(void *) * ucl_vector_number_of_free_slots(H->buckets));
    ucl_vector_mark_all_slots_as_used(H->buckets);
  }
  rehash(H, ucl_vector_size(H->buckets));
}
void
ucl_hash_restrict (ucl_hash_table_t H)
{
  assert(H);
  if (ucl_vector_size(H->buckets) > ucl_vector_number_of_step_down_slots(H->buckets)) {
    size_t	new_number_of_buckets;
    /* Compute the new number of buckets. */
    {
      size_t	number_of_step_down_buckets = H->buckets->step_down / H->buckets->slot_dimension;
      new_number_of_buckets = ucl_vector_size(H->buckets) - number_of_step_down_buckets;
      assert(0 < new_number_of_buckets);
      if (new_number_of_buckets < UCL_HASH_MINIMUM_SIZE)
	new_number_of_buckets = UCL_HASH_MINIMUM_SIZE;
    }
    debug("selected new size %u", new_number_of_buckets);
    rehash(H, new_number_of_buckets);
    {
      ucl_range_t	range;
      /* ranges are inclusive                            !!vv!! */
      ucl_range_set_min_max(range, 0, new_number_of_buckets-1);
      ucl_vector_mark_allocated_range_as_used(H->buckets, range);
      ucl_vector_restrict(H->buckets);
    }
  }
}

void
ucl_hash_iterator (const ucl_hash_table_t H, ucl_iterator_t iterator)
{
  ucl_iterator_t	vector_iterator;
  ucl_node_t *		bucket;	/* Pointer to the slot in the vector memory. */
  assert(H);
  assert(iterator);
  if (ucl_hash_size(H)) {
    iterator->container	= H;
    iterator->next	= iterator_next;
    for (ucl_vector_iterator_forward(H->buckets, vector_iterator);
	 ucl_iterator_more(vector_iterator);ucl_iterator_next(vector_iterator)) {
      bucket = ucl_iterator_ptr(vector_iterator);
      if (*bucket) {
	iterator->iterator = *bucket;
	iterator->internal1.pointer = bucket;
	return;
      }
    }
  }
  iterator->iterator = NULL;
}
static void
iterator_next (ucl_iterator_t iterator)
{
  const ucl_hash_table_tag_t *	H;
  ucl_node_t 	current_entry;
  ucl_node_t *	bucket;	/* Pointer to the slot in the vector memory. */
  assert(iterator);
  current_entry = iterator->iterator;
  current_entry = current_entry->next_entry_in_list;
  if (current_entry) {
    iterator->iterator = current_entry;
  } else {
    bucket = iterator->internal1.pointer;
    H     = (const ucl_hash_table_tag_t *) iterator->container;
    /* advance to the next bucket; if it is not the last examine it */
    for (++bucket; bucket <= (ucl_node_t *)ucl_vector_back(H->buckets); ++bucket) {
      if (*bucket) {
	iterator->iterator = *bucket;
	iterator->internal1.pointer = bucket;
	return;
      }
    }
    iterator->iterator = NULL;
  }
}

size_t
ucl_hash_number_of_used_buckets (const ucl_hash_table_t this)
{
  ucl_iterator_t	iterator;
  size_t		number_of_used_buckets = 0;
  ucl_node_t *		bucket_p;
  for (ucl_vector_iterator_forward(this->buckets, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator)) {
    bucket_p = ucl_iterator_ptr(iterator);
    if (NULL != *bucket_p)
      ++number_of_used_buckets;
  }
  return number_of_used_buckets;
}
size_t
ucl_hash_bucket_chain_length (const ucl_hash_table_t this, ucl_index_t position)
{
  ucl_node_t *	bucket_p;
  size_t	number_of_entries_in_chain = 0;
  bucket_p = ucl_vector_index_to_slot(this->buckets, position);
  if (*bucket_p) {
    ucl_node_t 	entry = *bucket_p;
    ++number_of_entries_in_chain;
    while (entry->next_entry_in_list) {
      ++number_of_entries_in_chain;
      entry = entry->next_entry_in_list;
    }
  }
  return number_of_entries_in_chain;
}
double
ucl_hash_average_search_distance (const ucl_hash_table_t this)
{
  size_t	number_of_buckets = ucl_hash_number_of_buckets(this);
  double	average_search_distance = 0.0;
  for (size_t i=0; i<number_of_buckets; ++i) {
    average_search_distance += (double)ucl_hash_bucket_chain_length(this, i);
  }
  return average_search_distance/((double)number_of_buckets);
}

/* end of file */
