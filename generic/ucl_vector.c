/* ucl_vector.c  --

   Part of: Useless Container Library (UCL)
   Content: the code for the vector container.

   Abstract:

        This file must  be included in all the modules  that make use of
        the vector container.

	  The UCL  vector container is an implementation  of the classic
	array with redundant memory allocation.

	  This  module  was  inspired  by  the book  on  C++  by  Bjarne
	Stroustrup and by the STL C++ (Standard Template Library).

   Copyright (c) 2001, 2002, 2003, 2004 Marco Maggi

   This library is free software;  you can redistribute it and/or modify
   it  under the  terms  of the  GNU  Lesser General  Public License  as
   published by the Free Software  Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   This library is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   Lesser General Public License for more details.

   You  should have received  a copy  of the  GNU Lesser  General Public
   License along with  this library; if not, write  to the Free Software
   Foundation, Inc.,  59 Temple Place, Suite 330,  Boston, MA 02111-1307
   USA

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

typedef ucl_vector_t		vector_t;


/* ucl_vector_constructor1 --

	Initialises an already allocated vector struct.

   Arguments:

	this -		ptr to the base struct
	size -		initialise number of slots to alloc
	step_up -	select the number of slots to add when
			reallocating a full array
	step_down -	the number of free slots that will cause the
			array to be reallocated and restricted
	pad -		the numer of free slots to keep at the beginning
			of the array when moving elements
	dim -		the size of the contained element

   Results:

	Returns a  pointer to  the first allocated  slot, or NULL  if an
	error  allocating memory occurs  or if  the selected  values are
	incorrect.

   Side effects:

        none

*/

void *
ucl_vector_constructor1(this, size, step_up, step_down, pad, dim)
     vector_t *	this;
     size_t		size;
     size_t		step_up;
     size_t		step_down;
     size_t		pad;
     size_t		dim;
{
  unsigned char *	ptr;
  

  assert(this != NULL);

  if ((pad >= size) || (dim == 0))
    {
      return NULL;
    }

  ptr = ucl_malloc(dim * size);
  if (ptr == NULL)
    {
      return NULL;
    }

  this->size		= 0;
  this->step_up		= step_up   * dim;
  this->step_down	= step_down * dim;
  this->pad		= pad       * dim;
  this->dim		= dim;

  this->min_front	= ptr;
  this->front		= \
  this->back		= ptr + (dim * pad);
  this->max_back	= ptr + (dim * size);

  return this->front;
}


/* ucl_vector_destructor --

	Destroys a vector struct.

   Arguments:

	this -		pointer to the base structure

   Results:

	The  dynamically allocated  array  is freed.  All the  structure
	fields are reset to zero.

   Side effects:

        If  the  contained values  nees  some  sort  of destruction,  it
        responsibility of the user  code to extract them before invoking
        this function.

*/

void
ucl_vector_destructor (vector_t *this)
{
  assert(this != 0);

  if (this->min_front != 0)
    {
      ucl_free(this->min_front);
    }
  memset(this, '\0', sizeof(vector_t));
}



/* ucl_vector_insert --

	Inserts an empty slot at a selected position.

	  This function assumes  that the array has a  free slot to hold
	the new element:  to make sure that this is true  a call to this
	function must be preceeded by a call to "ucl_vector_enlarge()".

	  The pointer to the slot must be the return value of a previous
	invocation to "ucl_vector_newindex()".

   Arguments:

	this -		pointer to the base structure
	_target -	pointer to the slot in which the new element
			must be inserted

   Results:

	This  function tries  to move  as less  elements as  possible to
	create an empty slot at  the selected position. The return value
	is a pointer to the empty slot: this could be different from the
	value of the "target" argument.

   Side effects:

        The correct sequence of function  calls required to insert a new
        element is: enlarge the vector, acquire the pointer, make a free
        slot, copy the value. Example:

	  vector_t *	vectPtr;
	  DataType *	ptr;
	  size_t	index;

	  ...
	  ptr = ucl_vector_enlarge(vectPtr);
	  if (ptr == NULL)
	    {
	      handle_the_error();
            }
          ptr = ucl_vector_newindex(vectPtr, index);
	  ptr = ucl_vector_insert(vectPtr, ptr);
	  *ptr = data_value;

*/

void *
ucl_vector_insert (vector_t *this, void *_target)
{
  ptrdiff_t		dim;
  unsigned char *	target;


  target = _target;
  assert(this != NULL);
  assert(target != NULL);
  assert(target >= this->front);
  assert(target <= this->back);


  dim = this->dim;

  /*
    If the vector is empty...
  */

  if (this->front == this->back)
    {
      if (this->back < this->max_back)
	{
	  this->back += dim;
	}
      else
	{
	  assert(this->front > this->min_front);
	  target = this->front -= dim;
	}
    }

  /*
    If the  elements are  attached to the  beginning of the  array, make
    room by moving elements toward the end.
  */

  else if (this->front == this->min_front)
    {
      assert(this->back < this->max_back);

      if (target < this->back)
	{
	  memmove(target+dim, target, (this->back - target));
	  /* now target points to an empty slot */
	}
      this->back += dim;
    }

  /*
    If the elements  are attached to the end of the  array, make room by
    moving elements toward the beginning.
  */

  else if (this->back == this->max_back)
    {
      assert(this->front > this->min_front);

      if (target != this->front)
	{
	  memmove(this->front-dim, this->front, (target - this->front));
	  /* now target-dim points to an empty slot */
	}
      this->front -= dim;
      target -= dim;
    }

  /*
    If there is room both at the beginning and at the end of the buffer:
    handle the simple cases of new first element and new last element.
  */

  else if (target == this->back)
    {
      assert(this->back < this->max_back);
      this->back += dim;
    }
  else if (target == this->front)
    {
      assert(this->front > this->min_front);
      this->front -= dim;
      target -= dim;
    }

  /*
    If  there's room  both  at the  beginning  and at  the  end: if  the
    insertion position  is closer  to the end  move the elements  at the
    end, else move the elements at the beginning.
  */

  else if ((target - this->front) > (this->back - target))
    {
      assert(this->back < this->max_back);

      memmove(target+dim, target, (this->back - target));
      this->back += dim;
    }
  else
    {
      assert(this->front > this->min_front);

      memmove(this->front-dim, this->front, (target - this->front));
      this->front -= dim;
      target -= dim;
    }

  assert(target >= this->front);
  assert(target < this->back);

  ++(this->size);
  return target;
}


/* ucl_vector_erase --

	Erases  an   element  at  a  selected  position:   the  slot  is
	overwritten by moving as less elements as possible to cover it.

   Arguments:

	this -		pointer to the base structure
	target -	pointer to the slot that must be erased

   Results:

	Returns nothing.

   Side effects:

        After  the invocation  the pointer  represented by  the "target"
        argument value, it's  not guaranteed to be a  pointer to a valid
        slot in the vector.

*/

void
ucl_vector_erase (vector_t *this, void *_target)
{
  size_t		dim;
  unsigned char *	target;


  target = _target;
  assert(this != 0);
  assert(target != NULL);
  assert(target >= this->front);
  assert(target <  this->back);


  dim = this->dim;


  /*
    If  the target  is  the  first (last)  element  we simply  increment
    (decrement) the ptr to the first (last) element and forget about it.
  */

  if (target == this->front)
    {
      this->front += dim;
    }
  else if (target == this->back-dim)
    {
      this->back -= dim;
    }

  /*
    If  the target is  close to  the end  we move  the last  elements to
    overwrite the slot, else we move the first elements.
  */

  else if ((target - this->front) > (this->back - target))
    {
      /* delete an element in the middle, but near the end */
      memmove(target, target+dim, (this->back - target));
      this->back -= dim;
    }
  else
    {
      /* delete an element in the middle, but near the beginning */
      memmove(this->front+dim, this->front, (target - this->front));
      this->front += dim;
    }

  --(this->size);
}




/* ucl_vector_index --

	Converts an index in the  vector to the corresponding pointer to
	a slot.

   Arguments:

	this -		pointer to the base structure
	index -		the selected index

   Results:

        Returns a pointer to the  selected slot, or NULL if the selected
        index is out of range.

   Side effects:

        None.

*/

void *
ucl_vector_index (vector_t *this, size_t index)
{
  unsigned char *	target;


  assert(this != NULL);

  target = this->front + (this->dim * index);
  return (target < this->back)? target : NULL;
}



/* ucl_vector_newindex --

	Converts an index in the  vector to the corresponding pointer to
	a slot.

	  This  is  different  from  "ucl_vector_index()"  in  that  the
	requested  position  can  be  one  unit greater  than  the  last
	position in the vector: that  way the returned value can be used
	with "ucl_vector_insert()"  to append an  element to the  end of
	the vector.

   Arguments:

	this -		pointer to the base structure
	index -		the selected index

   Results:

        Returns a pointer to the  selected slot, or NULL if the selected
        index is out of range.

   Side effects:

        None.

*/

void *
ucl_vector_newindex (vector_t *this, size_t index)
{
  unsigned char *	target;


  assert(this != NULL);

  target = this->front + (this->dim * index);
  return (target <= this->back)? target : NULL;
}

/* ucl_vector_qsort --

	Quick sort the vector.

   Arguments:

	this -		pointer to the base struct
	compar -	pointer to the comparative function

   Results:

        Nothing.

   Side effects:

        none

*/

void
ucl_vector_qsort (vector_t *this, ucl_valcmp_t *compar)
{
  assert(this   != NULL);
  assert(compar != NULL);
  
  qsort(this->front, this->size, this->dim, (ucl_voidcmp_t *)compar);
}



/* ucl_vector_enlarge --

	The caller  is thinking: "I'm  sure that, if there's  room, it's
	enough for me. So realloc() only if there's no left space."

   Arguments:

	this -		pointer to the base struct

   Results:

        Returns a pointer to the first element in the vector, or NULL if
        the reallocation failed.

   Side effects:

        none

*/

void *
ucl_vector_enlarge (vector_t *this)
{
  size_t		new_size;
  ptrdiff_t		length;
  unsigned char *	tmp;


  assert(this != NULL);
  assert(this->min_front != NULL);
  assert(this->min_front <= this->front);
  assert(this->front     <= this->back);
  assert(this->back      <= this->max_back);

  if ((this->front == this->min_front) && (this->back == this->max_back))
    {
      length	= this->max_back - this->min_front;
      new_size	= length + this->step_up;

#ifdef DEBUGGING
      fprintf(stderr, "enlarged: old size %d, new size %d\n", length, new_size);
      fflush(stderr);
#endif

      tmp = ucl_realloc(this->min_front, new_size);
      if (tmp == NULL)
	{
	  return tmp;
	}

      this->min_front	= tmp;
      this->front	= tmp;
      this->back	= tmp + length;
      this->max_back	= tmp + new_size;
    }

  return this->front;
}

/* ucl_vector_restrict --

	The  caller is  thinking: "Hey,  we don't  want to  waste space,
	don't we?  So why don't you check if we can free some memory?"

   Arguments:

	this -		pointer to the base struct

   Results:

        Returns a pointer to the first element in the vector, or NULL if
        the reallocation failed.

   Side effects:

        none

*/

void *
ucl_vector_restrict (vector_t *this)
{
  size_t		new_size;
  ptrdiff_t		size;
  ptrdiff_t		used;
  unsigned char *	tmp;


  assert(this != NULL);
  assert(this->min_front != NULL);
  assert(this->min_front <= this->front);
  assert(this->front     <= this->back);
  assert(this->back      <= this->max_back);

  size = this->max_back - this->min_front;
  used = this->back     - this->front;

  assert(this != 0);

  if ((size - used) > this->step_down)
    {
      memmove((this->min_front + this->pad), this->front, used);
      this->front     = this->min_front + this->pad;
      this->back      = this->front + used;
      this->max_back  = this->min_front + size;

      if (used + this->pad > this->step_up)
	{
	  new_size = this->pad + used + this->step_up - (used % this->step_up);
	}
      else
	{
	  new_size = this->step_up;
	}

      if (new_size < size)
	{
#ifdef DEBUGGING
      fprintf(stderr, "enlarged: old size %d, new size %d\n", used, new_size);
      fflush(stderr);
#endif

	  tmp = ucl_realloc(this->min_front, new_size);
	  if (tmp == NULL)
	    {
	      return tmp;
	    }

	  this->min_front = tmp;
	  this->front	  = tmp + this->pad;
	  this->back      = tmp + this->pad + used;
	  this->max_back  = tmp + new_size;
	}
    }

  return this->front;
}

/* ucl_vector_find --

	Finds an element in the array with a linear search.

   Arguments:

	this -		pointer to the base structure
	elmPtr -	pointer to the element to be found, it's used
			as first argument to the "compar" function
	compar -	pointer to a strcmp()-like function that's used
			to compare elements

   Results:

        Returns a pointer to the found  element in the array, or NULL if
	the element is not present.

   Side effects:

        None.

*/

void *
ucl_vector_find (this, elmPtr, compar)
     vector_t *	this;
     void *		elmPtr;
     int		(*compar)(const void *a, const void *b);
{
  size_t		dim;
  unsigned char *	ptr;

  assert(this != NULL);

  assert(elmPtr != NULL);
  assert(compar != NULL);

  if (this->size == 0)
    {
      return NULL;
    }

  dim = this->dim;

  for (ptr = this->front; ptr < this->back; ptr += dim)
    {
      if (compar(elmPtr, ptr) == 0)
	{
	  return ptr;
	}
    }

  return NULL;
}

/* ucl_vector_sortfind --

	Finds  an element  in  the  array with  a  binary search.   This
	function  assumes that  the array  is sorted.  If there  are few
	elements in the vector, a linear search is performed.

   Arguments:

	this -		pointer to the base structure
	elmPtr -	pointer to the element to be found, it's used
			as first argument to the "compar" function
	compar -	pointer to a strcmp()-like function that's used
			to compare elements

   Results:

        Returns a pointer to the found  element in the array, or NULL if
	the element is not present.

   Side effects:

        None.

*/

void *
ucl_vector_sortfind (this, elmPtr, compar)
     vector_t *		this;
     void *		elmPtr;
     ucl_valcmp_t *	compar;
{
  unsigned char *	ptr;
  ucl_value_t		a, b;


  assert(this != NULL);
  assert(elmPtr != NULL);
  assert(compar != NULL);

  if (this->size == 0)
    {
      return NULL;
    }


  /* few strings, do a linear search */

  if (this->size <= 5)
    {
      size_t	dim = this->dim;

      for (ptr = this->front; ptr < this->back; ptr += dim)
	{
	  a.ptr = elmPtr;
	  b.ptr = ptr;
	  if (compar(a, b) == 0)
	    {
	      return ptr;
	    }
	}
    }
  else /* do a binary search */
    {
      ptrdiff_t		i, cur, newcur, match, uplim, dnlim;


      cur	= this->size >> 1;
      uplim = this->size - 1;
      dnlim = 0;

      for (i=0; i<this->size; ++i)
	{
	  ptr = ucl_vector_index(this, cur);
	  a.ptr = elmPtr;
	  b.ptr = ptr;
	  match = compar(a, b);
	  if (match == 0)
	    {
	      return ptr;
	    }
	  else if (match < 0)
	    {
	      uplim  = cur-1;
	      newcur = dnlim + ((cur-dnlim) >> 1);
	      cur = (newcur < cur)? newcur : newcur-1;
	      if (cur < dnlim)
		{
		  return NULL;
		}
	    }
	  else /* match > 0 */
	    {
	      dnlim  = cur+1;
	      newcur = cur + ((uplim-cur) >> 1);
	      cur = (newcur > cur)? newcur : newcur+1;
	      if (cur > uplim)
		{
		  return NULL;
		}
	    }
	}
    }

  return NULL;
}

/* ucl_vector_insertsort --

	Finds the  position in  the vector in  which an element  must be
	inserted to  keep the array sorted. This  function assumes that:
	there's room in the vector to insert a new element; the array is
	sorted.

   Arguments:

	this -		pointer to the base structure
	elmPtr -	pointer to the element to insert, it's used
			as first argument to the "compar" function
	compar -	pointer to a strcmp()-like function that's used
			to compare elements

   Results:

        The return value is a pointer to the empty slot.

   Side effects:

        None.

*/

void *
ucl_vector_insertsort (this, elmPtr, compar)
     vector_t *		this;
     void *		elmPtr;
     ucl_valcmp_t	*compar;
{
  unsigned char *	ptr = NULL;
  size_t		dim;
  ucl_value_t		a, b;


  assert(this != NULL);
  assert(elmPtr != NULL);
  assert(compar != NULL);


  if (this->size == 0)
    {
      return ucl_vector_insert(this, ucl_vector_newindex(this, 0));
    }

  dim = this->dim;

  /* few strings, do a linear search */

  if (this->size <= 5)
    {
      for (ptr = this->front; ptr < this->back; ptr += dim)
	{
	  a.ptr = elmPtr;
	  b.ptr = ptr;
	  if (compar(a, b) > 0)
	    {
	      break;
	    }
	}

      ptr = this->back;
    }
  else /* do a binary search */
    {
      ptrdiff_t		i, cur, newcur, match, uplim, dnlim;


      cur	= this->size >> 1;
      uplim	= this->size - 1;
      dnlim	= 0;

      for (i=0; i<this->size; ++i)
	{
	  ptr = ucl_vector_index(this, cur);
	  a.ptr = elmPtr;
	  b.ptr = ptr;
	  match = compar(a, b);
	  if (match == 0)
	    {
	      ptr += dim;
	      break;
	    }
	  else if (match < 0)
	    {
	      uplim  = cur-1;
	      newcur = dnlim + ((cur-dnlim) >> 1);
	      cur = (newcur < cur)? newcur : newcur-1;
	      if (cur < dnlim)
		{
		  break;
		}
	    }
	  else /* match > 0 */
	    {
	      dnlim  = cur+1;
	      newcur = cur + ((uplim-cur) >> 1);
	      cur = (newcur > cur)? newcur : newcur+1;
	      if (cur > uplim)
		{
		  ptr += dim;
		  break;
		}
	    }
	}
    }

  return ucl_vector_insert(this, ptr);
}


/* end of file */
