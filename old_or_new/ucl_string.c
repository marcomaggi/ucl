/* ucl_string.c --
   
   Part of: Useless Container Library (UCL)
   Contents: string implementation
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
   
   $Id: ucl_string.c,v 1.1.1.15 2003/12/09 14:07:46 marco Exp $
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl.h"


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_string_t		string_t;

#define TOTALSIZE(size)		sizeof(size_t) + sizeof(size_t) + \
				sizeof(char *) + sizeof(char) * (size);


/* ucl_string_constructor --

	Allocates and initialises a new string.

   Arguments:

	size -		the initial size

   Results:

        Returns a pointer to the string structure or NULL if an
	error occurred allocating memory.

   Side effects:

        None.

*/

string_t *
ucl_string_constructor	(size_t size)
{
  string_t *	ptr;
  size_t	totsize;
  

  totsize = TOTALSIZE(size);

  ptr  = (string_t *) ucl_malloc(totsize);
  if (ptr)
    {
      ptr->size = size;
      ptr->len	= 0;
      ptr->ptr  = &(ptr->data[0]);
      memset(ptr->ptr, '\0', sizeof(char) * size);
    }
  return ptr;
}



/* ucl_string_duplicate --

	Duplicates a string.

   Arguments:

	this -		a pointer to the string structure

   Results:

        Returns a pointer to the duplicate or NULL if an error
	allocating memory occurred.

   Side effects:

        None.

*/

string_t *
ucl_string_duplicate (string_t *this)
{
  string_t *	ptr;
  

  ptr = ucl_string_constructor(this->size);
  if (ptr)
    {
      ptr->len	= this->len;
      strncpy(ptr->ptr, this->ptr, this->len);
    }
  return ptr;
}



/* ucl_string_insert --

	Insert a character at a selected position. This function assumes
	that the string is not full and the selected position is a valid
	index: it must be an integer between zero and the string length.

   Arguments:

	this -		a pointer to the string structure
	index -		the position of the new char
	c -		the new char

   Results:

        None

   Side effects:

        None.

*/

void
ucl_string_insert (string_t *this, size_t index, const char c)
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



/* ucl_string_erase --

	Erase a character at  a selected position. This function assumes
	that the selected index is a  valid index: it must be an integer
	between zero and the string length decremented by one.

   Arguments:

	this -		a pointer to the string structure
	index -		the index of the character to erase

   Results:

        Returns ...

   Side effects:

        None.

*/

void
ucl_string_erase (string_t *this, size_t index)
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



/* ucl_string_pushfront --

	Append  a character  to the  tail of  the string.  This function
	assumes that the string is not full.

   Arguments:

	this -		a pointer to the string function
	c -		the character to append

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_string_pushfront (string_t *this, char c)
{
  assert(this->len < this->size);
  
  memmove(this->ptr+1, this->ptr, this->len);
  this->ptr[0] = c;
  ++(this->len);
}


/* ucl_string_popfront --

	Extract  the first  character.  This function  assumes that  the
	string is not empty.

   Arguments:

	this -		a pointer to the string structure

   Results:

        Returns the character.

   Side effects:

        None.

*/

char
ucl_string_popfront (string_t *this)
{
  char c;

  assert(this->len > 0);

  c = this->ptr[0];
  --(this->len);
  memmove(this->ptr, this->ptr+1, this->len);
  return c;
}



/* ucl_string_pushback --

	Append  a character  at the  end  of the  string. This  function
	assumes that the string is not full.

   Arguments:

	this -		a pointer to the string structure
	c -		the new character

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_string_pushback (string_t *this, char c)
{
  assert(this->len < this->size);

  this->ptr[this->len] = c;
  ++(this->len);
}



/* ucl_string_popback --

	Extract  a character  from the  end  of the  string. The  string
	length  is  reduced  by  one,  and the  extracted  character  is
	overwritten with a null char. The string is not reallocated.

	  This function assumes that the string is not empty.

   Arguments:

	this -		a pointer to the string structure

   Results:

        Returns the last character in the string.

   Side effects:

        None.

*/

char
ucl_string_popback (string_t *this)
{
  char	c;
  
  assert(this->len > 0);

  --(this->len);
  c = this->ptr[this->len];
  this->ptr[this->len] = '\0';
  return c;
}




/* ucl_string_randomise --

	Randomises a string.

   Arguments:

	this -		a pointer to the source string

   Results:

        None.

   Side effects:

        None.

*/

void
ucl_string_randomise (string_t *strPtr)
{
  size_t i = 0, j, len;
  char tmp;
  

  len = strPtr->len;

  if (len)
    {
      while (i < len) {
	j = rand() % len;
	tmp = strPtr->ptr[j];
	strPtr->ptr[j] = strPtr->ptr[i];
	strPtr->ptr[i] = tmp;
	++i;
      }
    }
}



/* ucl_string_swap --

	Swap two characters. This  function assumes that the indexes are
	not out of range.

   Arguments:

	this -		a pointer to the string structure
	a -		the index of the first char
	b -		the index of the last char

   Results:

        The characters are swapped.

   Side effects:

        None.

*/

void
ucl_string_swap (string_t *this, size_t a, size_t b)
{
  char tmp;


  tmp = this->ptr[a];
  this->ptr[a] = this->ptr[b];
  this->ptr[b] = tmp;
}



/* ucl_string_resize --

	Reallocates a  string to resize the  buffer. If the  new size is
	greater than the length of the string, the tail is truncated.

   Arguments:

	this -		a pointer to the string
	size -		the new size

   Results:

        Returns a pointer  to the string or NULL  if an error allocating
        memory occurs.

   Side effects:

        None.

*/

string_t *
ucl_string_resize (string_t *this, size_t size)
{
  string_t *	new;
  size_t	totsize;


  totsize = TOTALSIZE(size);
  new = ucl_realloc(this, totsize);
  if (new)
    {
      new->size = size;
      new->ptr  = &(new->data[0]);

      if (new->len >= size)
	{
	  --size;
	  new->len = size;
	}
      new->ptr[new->len] = '\0';
    }
  return new;
}



/* ucl_string_enlarge --

	If  the string  is full,  enlarge the  buffer size  to  the next
	multiple of UCL_STRING_STEP_UP.

   Arguments:

	this -		a pointer to the string structure

   Results:

        Returns a pointer to the  reallocated string or NULL if an error
        allocating memory occurred.

   Side effects:

        None.

*/

string_t *
ucl_string_enlarge (string_t *this)
{
  size_t	size;

  
  if (ucl_string_isfull(this))
    {
      size = this->size;
      size = size + UCL_STRING_STEP_UP - (size % UCL_STRING_STEP_UP);
      return ucl_string_resize(this, size);
    }
  else
    {
      return this;
    }
}



/* ucl_string_restrict --

	Restrict the string buffer size to the next multiple of 16.

   Arguments:

	this -		a pointer to the string structure

   Results:

        Returns a pointer to the  reallocated string or NULL if an error
        allocating memory occurred.

   Side effects:

        None.

*/

string_t *
ucl_string_restrict (string_t *this)
{
  size_t	len, newsize;


  len	= this->len;
  if (len+1 <= UCL_STRING_STEP_UP)
    {
      newsize = UCL_STRING_STEP_UP;
    }
  else
    {
      newsize = len + 1 + UCL_STRING_STEP_UP - (len % UCL_STRING_STEP_UP);
    }

  if (this->size <= newsize)
    {
      return this;
    }
  else
    {
      return ucl_string_resize(this, newsize);
    }
}


/* end of file */
