/* ucl_strtok.c --
   
   Part of: Useless Container Library (UCL)
   Contents: strtok implementation
   Date: Mon Dec 16, 2002
   
   Abstract
   
   
   
   Copyright (c) 2002, 2003 Marco Maggi
   
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
   
   $Id: ucl_strtok.c,v 1.1.1.13 2003/12/09 14:08:50 marco Exp $
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_strtok_t		strtok_t;


/* ucl_strtok_constructor --

	Allocates and initialises a new string token.

   Arguments:

	this -	a pointer to the string structure
	size -		the initial size

   Results:

        None.

   Side effects:

        None.

*/

char *
ucl_strtok_constructor	(strtok_t *this, size_t size)
{
  char *	ptr;
  

  ptr  = (char *) ucl_malloc(sizeof(char) * size);
  if (ptr)
    {
      this->size = size;
      this->len  = 0;
      this->ptr  = ptr;
      memset(ptr, '\0', sizeof(char) * size);
    }
  return ptr;
}



/* ucl_strtok_destructor --

	Destroys the  string token structure.  The dynamically allocated
	memory is freed.

   Arguments:

	this -		a pointer to the string token structure

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_strtok_destructor (strtok_t *this)
{
  if (this->ptr)
    {
      ucl_free(this->ptr);
    }
  this->size = this->len = 0;
}


/* ucl_strtok_duplicate --

	Duplicates a string.

   Arguments:

	this -	a pointer to the string structure

   Results:

        The buffer  pointer of the duplicate  token is NULL  if an error
        allocating memory occurred.

   Side effects:

        None.

*/

char *
ucl_strtok_duplicate (strtok_t *dst, strtok_t *src)
{
  char *	ptr;

  ptr = ucl_strtok_constructor(dst, src->len);
  if (ptr)
    {
      dst->len = src->len;
      strncpy(dst->ptr, src->ptr, dst->len);
    }
  return ptr;
}



/* ucl_strtok_insert --

	Insert a character at a selected position. This function assumes
	that the string buffer is  not full and the selected position is
	a valid index: it must be an integer between zero and the string
	length.

   Arguments:

	this -	a pointer to the strtok structure
	index -		the position of the new char
	c -		the new char

   Results:

        None

   Side effects:

        None.

*/

void
ucl_strtok_insert (strtok_t *this, size_t index, char c)
{
  char *	ptr;


  assert(this->len < this->size);
  assert(index <= this->len);

  if (index == this->len)
    {
      this->ptr[this->len] = c;
    }
  else
    {
      ptr = this->ptr + index;
      memmove(ptr+1, ptr, this->len-index);
      *ptr = c;
    }
  ++(this->len);
}



/* ucl_strtok_erase --

	Erase a character at  a selected position. This function assumes
	that the selected index is a  valid index: it must be an integer
	between zero and the strtok length decremented by one.

   Arguments:

	this -	a pointer to the string structure
	index -		the position of the character to erase

   Results:

        Returns ...

   Side effects:

        None.

*/

void
ucl_strtok_erase (strtok_t *this, size_t index)
{
  char *	ptr;


  assert(index < this->len);

  if (index+1 != this->len)
    {
      ptr = this->ptr + index;
      memmove(ptr, ptr+1, this->len - index - 1);
    }
  --(this->len);
  this->ptr[this->len] = '\0';
}



/* ucl_strtok_pushfront --

	Append  a character  to the  tail  of the  string buffer.   This
	function assumes that the strtok is not full.

   Arguments:

	this -	a pointer to the strtok structure
	c -		the character to append

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_strtok_pushfront (strtok_t *this, char c)
{
  char *	ptr;

  assert(this->len < this->size);

  ptr = this->ptr;
  memmove(ptr+1, ptr, this->len);
  ptr[0] = c;
  ++(this->len);
}


/* ucl_strtok_popfront --

	Extract  the first  character.  This function  assumes that  the
	strtok is not empty.

   Arguments:

	this -		a pointer to the strtok structure

   Results:

        Returns the character.

   Side effects:

        None.

*/

char
ucl_strtok_popfront (strtok_t *this)
{
  char *	ptr;
  char		c;

  assert(this->len > 0);

  ptr = this->ptr;
  c   = ptr[0];
  --(this->len);
  memmove(ptr, ptr+1, this->len);
  return c;
}



/* ucl_strtok_pushback --

	Append  a  character at  the  end  of  the string  buffer.  This
	function assumes that the strtok is not full.

   Arguments:

	this -	a pointer to the strtok structure
	c -		the new character

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_strtok_pushback (strtok_t *this, char c)
{
  assert(this->len < this->size);

  this->ptr[this->len] = c;
  ++(this->len);
}



/* ucl_strtok_popback --

	Extract  a character  from the  end  of the  string buffer.  The
	strtok length is reduced by  one, and the extracted character is
	overwritten with a null char.

	  This function assumes that the strtok is not empty.

   Arguments:

	this -		a pointer to the strtok structure

   Results:

        Returns the last character in the strtok.

   Side effects:

        None.

*/

char
ucl_strtok_popback (strtok_t *this)
{
  char	c;
  
  assert(this->len > 0);

  --(this->len);
  c = this->ptr[this->len];
  this->ptr[this->len] = '\0';
  return c;
}



/* ucl_strtok_randomise --

	Randomise a string.

	  An original version of this  code was found on the comp.lang.c
	newsgroup.

   Arguments:

	this -		a pointer to the string structure

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_strtok_randomise (strtok_t *this)
{
  size_t i = 0, j, len;
  char	 tmp;


  len = this->len;

  if (len)
    {
      while (i < len) {
	j = rand() % len;
	tmp = this->ptr[j];
	this->ptr[j] = this->ptr[i];
	this->ptr[i] = tmp;
	++i;
      }
    }
}



/* ucl_strtok_swap --

	Swap two characters. This  function assumes that the two indexes
	are valid.

   Arguments:

	this -	a pointer to the strtok structure
	a -		the index of the first char
	b -		the index of the last char

   Results:

	None.

   Side effects:

        None.

*/

void
ucl_strtok_swap (strtok_t *this, size_t a, size_t b)
{
  char tmp;


  assert((a >= this->len) || (b >= this->len));

  tmp = this->ptr[a];
  this->ptr[a] = this->ptr[b];
  this->ptr[b] = tmp;
}



/* ucl_strtok_resize --

	Reallocates and resize  the buffer.  If the new  size is greater
	than the length of the string, the tail is truncated.

   Arguments:

	this -	a pointer to the string structure
	size -		the new size

   Results:

        Returns  a pointer  to the  string buffer  or NULL  if  an error
        allocating memory occurs.

   Side effects:

        None.

*/

char *
ucl_strtok_resize (strtok_t *this, size_t size)
{
  char *	ptr;


  ptr = ucl_realloc(this->ptr, size);
  if (ptr)
    {
      this->size = size;
      this->ptr  = ptr;

      if (this->len >= size)
	{
	  --size;
	  this->len = size;
	  this->ptr[size] = '\0';
	}
    }
  return ptr;
}



/* ucl_strtok_enlarge --

	Enlarge  the  string  buffer   size  to  the  next  multiple  of
	UCL_STRTOK_STEP_UP.

   Arguments:

	this -		a pointer to the strtok structure

   Results:

        Returns a pointer to the reallocated string buffer or NULL if an
        error allocating memory occurred.

   Side effects:

        None.

*/

char *
ucl_strtok_enlarge (strtok_t *this)
{
  size_t	size;

  
  if (ucl_strtok_isfull(this))
    {
      size = this->size;
      size = size + UCL_STRTOK_STEP_UP - (size % UCL_STRTOK_STEP_UP);
      return ucl_strtok_resize(this, size);
    }
  else
    {
      return this->ptr;
    }
}



/* ucl_strtok_restrict --

	Restrict  the  string  buffer  size  to  the  next  multiple  of
	UCL_STRTOK_STEP_UP.

   Arguments:

	this -		a pointer to the strtok structure

   Results:

        Returns a pointer to the reallocated string buffer or NULL if an
        error allocating memory occurred.

   Side effects:

        None.

*/

char *
ucl_strtok_restrict (strtok_t *this)
{
  size_t	len, newsize;


  len	= this->len;
  if (len+1 <= UCL_STRTOK_STEP_UP)
    {
      newsize = UCL_STRTOK_STEP_UP;
    }
  else
    {
      newsize = len + 1 + UCL_STRTOK_STEP_UP - (len % UCL_STRTOK_STEP_UP);
    }

  if (this->size <= newsize)
    {
      return this->ptr;
    }
  else
    {
      return ucl_strtok_resize(this, newsize);
    }
}



/* end of file */
