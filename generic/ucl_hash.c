/* ucl_hash.c --
   
   Part of: Useless Containers Library
   Contents: hash table implementation
   Date: Thu Mar  6, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: ucl_hash.c,v 1.1.1.5 2003/12/09 12:47:58 marco Exp $
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"

#ifdef DEBUGGING
#  include <stdio.h>
#endif


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_hash_t		hash_t;
typedef ucl_hash_entry_t	entry_t;
typedef ucl_value_t		value_t;
typedef ucl_iterator_t		iterator_t;

typedef ucl_valcmp_t		valcmp_t;
typedef ucl_hashfun_t		hashfun_t;


/** ------------------------------------------------------------
 ** Function prototypes.
 ** ----------------------------------------------------------*/

static void	iterator_next	(iterator_t *iterator);



/* ucl_hash_constructor --

	Initialises an already allocated hash structure.

   Arguments:

	this -		pointer to the hash structure
	size -		the initial number of buckets, can't be zero
	compar -	pointer to the strcmp()-like function used
			to compare keys
	hash -		pointer to the hash function

   Results:

        The vector of buckets is allocated.

	  Returns NULL if an error occurred allocating memory.

   Side effects:

        None.

*/

void *
ucl_hash_constructor (this, dim, compar, hash)
     hash_t *		this;
     size_t		dim;
     valcmp_t *		compar;
     hashfun_t *	hash;
{
  void *	ptr;


  assert(this);
  assert(hash);
  assert(compar);
  assert(dim > 0);

  ptr = ucl_malloc(sizeof(entry_t *) * dim);
  if (!ptr)
    {
      return ptr;
    }
  memset(ptr, '\0', (sizeof(entry_t *) * dim));

  this->buckets = ptr;
  this->size	= this->used = 0;
  this->dim	= dim;
  this->hash	= hash;
  this->compar	= compar;
  return ptr;
}

/* ucl_hash_destructor --

	Resets a hash table.

   Arguments:

	this -		pointer to the base structure

   Results:

        The  vector of buckets  is freed.  All the  fields are  reset to
        zero.

   Side effects:

        None.

*/

void
ucl_hash_destructor (hash_t *this)
{
  assert(this);
  
  if (this->buckets)
    {
      ucl_free(this->buckets);
    }
  memset(this, '\0', sizeof(hash_t));
}

/* ucl_hash_insert --

	Insert a new entry in the table.

   Arguments:

	this -		pointer to the base structure
	entPtr -	pointer to the entry structure

   Results:

        The new entry is inserted as first in the chain for the selected
        bucket.

   Side effects:

        None.

*/

void
ucl_hash_insert (hash_t *this, entry_t *entPtr)
{
  entry_t **	buckets;
  entry_t *	nxtPtr;
  size_t	index;
  

  assert(this);
  assert(entPtr);
  
  index = this->hash(entPtr->val);
  index %= this->dim;

  buckets = this->buckets;
  if (buckets[index])
    {
      nxtPtr = buckets[index];
      buckets[index] = entPtr;
      entPtr->nxtPtr = nxtPtr;
    }
  else
    {
      buckets[index] = entPtr;
      entPtr->nxtPtr = NULL;
      ++(this->used);
    }

  entPtr->erased = 0x00;
  ++(this->size);
}

/* ucl_hash_extract --

	Extracts an entry from the table.

   Arguments:

	this -		pointer to the base structure
	entPtr -	pointer to the entry structure

   Results:

        The entry is removed from the chain.

   Side effects:

        None.

*/

void
ucl_hash_extract (hash_t *this, entry_t *entPtr)
{
  size_t	index;
  entry_t *	lnkPtr;
  entry_t **	buckets;


  assert(this);
  assert(this->buckets);
  assert(entPtr);

  index  = this->hash(entPtr->val);
  index %= this->dim;

  buckets = this->buckets;
  assert(buckets[index]);

  lnkPtr = buckets[index];
  if (lnkPtr == entPtr)
    {
      if (lnkPtr->nxtPtr)
	{
	  buckets[index] = lnkPtr->nxtPtr;
	}
      else
	{
	  buckets[index] = NULL;
	  --(this->used);
	}
    }
  else
    {
      while (lnkPtr->nxtPtr != entPtr)
	{
	  lnkPtr = lnkPtr->nxtPtr;
	}
      lnkPtr->nxtPtr = entPtr->nxtPtr;
    }
  
  --(this->size);
}

/* ucl_hash_find --

	Finds an entry associated to a key.

   Arguments:

	this -		pointer to the base structure
	key -		the key value

   Results:

        Returns a pointer to the  entry structure or NULL if the element
        was not found.

   Side effects:

        None.

*/

entry_t *
ucl_hash_find (hash_t *this, value_t key)
{
  int		v;
  size_t	index;
  entry_t *	entPtr;
  valcmp_t *	compar;


  assert(this);
  assert(this->buckets);

  if (this->size == 0)
    {
      return NULL;
    }

  index = this->hash(key);
  index %= this->dim;

  entPtr = this->buckets[index];
  if (entPtr)
    {
      compar = this->compar;

      v = compar(key, entPtr->key);
      while ((v != 0) && (entPtr->nxtPtr))
	{
	  entPtr = entPtr->nxtPtr;
	  v = compar(key, entPtr->key);
	}

      if (v != 0)
	{
	  entPtr = NULL;
	}
    }

  return entPtr;
}

/* ucl_hash_enlarge --

	Resizes the table.

   Arguments:

	this -		pointer to the base structure
	newsize -	the new size

   Results:

        The  vector of  buckets  is reallocated  and  enlarged. All  the
        entries are marked as erased and rehashed.

	  Returns NULL if an error occurred reallocating memory.

   Side effects:

        None.

*/

void *
ucl_hash_enlarge (hash_t *this, size_t newdim)
{
  size_t	i, index;
  void *	ptr;
  entry_t **	buckets;
  entry_t *	entPtr;
  entry_t *	lnkPtr;


  assert(this);
  assert(this->dim < newdim);
  assert(this->buckets);

  ptr = ucl_realloc(this->buckets, sizeof(entry_t *) * newdim);
  if (!ptr)
    {
      return ptr;
    }

  buckets = this->buckets = ptr;

  /*
    Reset to NULL all the new buckets.
  */

  for (i=this->dim; i<newdim; ++i)
    {
      buckets[i] = NULL;
    }

  /*
    Mark erased all the entries.
  */

  for (i=0; i<this->dim; ++i)
    {
      entPtr = buckets[i];
      while (entPtr)
	{
	  entPtr->erased = 0xff;
	  entPtr = entPtr->nxtPtr;
	}
    }

  /*
    Rehash all the entries.
  */

  for (i=0; i<this->dim; ++i)
    {
      if (buckets[i])
	{
	  /*
	    Extract  all the  erased  entries in  this  bucket from  the
	    chain, then  find the new  bucket and insert them  after the
	    last erased entry in the new chain.
	  */

	  while (buckets[i] && buckets[i]->erased)
	    {
	      entPtr = buckets[i];
	      entPtr->erased = 0x00;
	      buckets[i] = entPtr->nxtPtr;

	      index  = this->hash(entPtr->key) % newdim;
	      lnkPtr = buckets[index];
	      if (lnkPtr)
		{
		  while (lnkPtr->nxtPtr && lnkPtr->nxtPtr->erased)
		    {
		      lnkPtr = lnkPtr->nxtPtr;
		    }
		  entPtr->nxtPtr = lnkPtr->nxtPtr;
		  lnkPtr->nxtPtr = entPtr;
		}
	      else
		{
		  buckets[index] = entPtr;
		  entPtr->nxtPtr = NULL;
		}
	    }
	}
    }

  this->dim = newdim;
  return buckets;
}



/* ucl_hash_iterator --

	Initialises the hash table iterator.

   Arguments:

	this -		pointer to the base structure
	iterator -	poitner to the iterator structure

   Results:

	The iterator references the current table entry. If the table is
	empty the iteration is over. The values are visited unordered.

   Side effects:

        None.

*/

void
ucl_hash_iterator (hash_t *this, iterator_t *iterator)
{
  size_t	i;
  

  assert(this);
  assert(iterator);
  
  if (ucl_hash_size(this) == 0)
    {
      iterator->iterator = NULL;
    }
  else
    {
      iterator->container = this;
      iterator->next	  = iterator_next;

      for (i = 0; i<this->dim; ++i)
	{
	  if (this->buckets[i] != NULL)
	    {
	      iterator->iterator = this->buckets[i];
	      iterator->ptr1     = &(this->buckets[i]);
	      return;
	    }
	}
      iterator->iterator = NULL;
    }
}

/* iterator_next --

	Advances the table iterator.

   Arguments:

	iterator -	pointer to the base structure

   Results:

        If the current chain of entries is not over, take the next; else
        move to the next bucket.

   Side effects:

        None.

*/

static void
iterator_next (iterator_t *iterator)
{
  ptrdiff_t		i;
  const hash_t *	hash;
  entry_t *		entPtr;
  entry_t **		buckets;


  assert(iterator);

  entPtr = ((entry_t *) iterator->iterator)->nxtPtr;
  if (entPtr != NULL)
    {
      iterator->iterator = entPtr;
    }
  else
    {
      buckets = iterator->ptr1;
      hash    = iterator->container;
      i = buckets - hash->buckets;

      for (++i; i<hash->dim; ++i)
	{
	  if (hash->buckets[i] != NULL)
	    {
	      iterator->iterator = hash->buckets[i];
	      iterator->ptr1     = &(hash->buckets[i]);
	      return;
	    }
	}
      iterator->iterator = NULL;
    }
}

/* ucl_hash_string --

	A sample hash  function. This comes from the  C++ book of Bjarne
	Srtoustrup.

   Arguments:

	val -		the key value as a string

   Results:

        Returns a number that must be reduced to the interval [0, dim).

   Side effects:

        None.

*/

size_t
ucl_hash_string (value_t val)
{
  size_t	num=0, len;

  len = strlen(val.str);
  while (len--)
    {
      num = (num << 1)^*val.str++;
    }

  return num;
}


/* end of file */
