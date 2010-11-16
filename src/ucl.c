/*
  Part of: Useless Containers Library
  Contents: miscellaneous functions

  Abstract:

	The   quicksort  implementation   is  a   modified   version  of
	"stdlib/qsort.c" from the GNU C Library version 2.4.  Written by
	Douglas  C. Schmidt  (schmidt@ics.uci.edu).   Modified by  Marco
	Maggi (marcomaggi@gna.org).

  Copyright (c) 2001-2005, 2007-2010 Marco Maggi <marco.maggi-ipsu@poste.it>
  Copyright (c) 1991,1992,1996,1997,1999,2004 Free Software Foundation,
  Inc.

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


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include "internal.h"

const ucl_ascii_t	ucl_ascii_empty		= { .ptr = (char *)"", .len = 0 };
const ucl_value_t	ucl_value_null		= { .t_unsigned_long = 0 };
const ucl_callback_t	ucl_callback_null	= { .func = NULL, .data = { .ptr = 0 } };

ucl_callback_apply_fun_t * ucl_callback_application_function = ucl_callback_apply;

const ucl_comparison_t ucl_compare_int = {
  .data = { .t_unsigned_long = 0 }, .func = ucl_compare_int_fun
};
const ucl_comparison_t ucl_compare_unsigned_int = {
  .data = { .t_unsigned_long = 0 }, .func = ucl_compare_unsigned_int_fun
};
const ucl_comparison_t ucl_compare_string = {
  .data = { .t_unsigned_long = 0 }, .func = ucl_compare_string_fun
};
const ucl_comparison_t ucl_compare_int_pointer = {
  .data = { .t_unsigned_long = 0 }, .func = ucl_compare_int_pointer_fun
};


/** --------------------------------------------------------------------
 ** Callback functions.
 ** -----------------------------------------------------------------*/

void
ucl_callback_set_application_function (ucl_callback_apply_fun_t * f)
{
  ucl_callback_application_function = f;
}


/** ------------------------------------------------------------
 ** Version number functions.
 ** ----------------------------------------------------------*/

const char *
ucl_version (void)
{
  return PACKAGE_VERSION;
}
unsigned
ucl_interface_major_version (void)
{
  return ucl_INTERFACE_MAJOR_VERSION;
}
unsigned
ucl_interface_minor_version (void)
{
  return ucl_INTERFACE_MINOR_VERSION;
}


/** ------------------------------------------------------------
 ** Comparison functions.
 ** ----------------------------------------------------------*/

int
ucl_compare_int_fun (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b)
{
  return ((a.t_int == b.t_int)? 0 : ((a.t_int > b.t_int)? 1 : -1));
}
int
ucl_compare_unsigned_int_fun (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b)
{
  return ((a.t_unsigned_int == b.t_unsigned_int)? 0 : ((a.t_unsigned_int > b.t_unsigned_int)? 1 : -1));
}
int
ucl_compare_string_fun (ucl_value_t data UCL_UNUSED, const ucl_value_t a, const ucl_value_t b)
{
  return strcmp(a.chars, b.chars);
}
int
ucl_compare_int_pointer_fun (ucl_value_t data, const ucl_value_t a, const ucl_value_t b)
{
  ucl_value_t	A = { .t_int = *((const int *) a.ptr) };
  ucl_value_t	B = { .t_int = *((const int *) b.ptr) };
  return ucl_compare_int_fun(data, A, B);
}


/** --------------------------------------------------------------------
 ** Hash functions.
 ** ----------------------------------------------------------------- */

size_t
ucl_hash_string (ucl_value_t data UCL_UNUSED, const ucl_value_t val)
{ /* This comes from the C++ book of Bjarne Srtoustrup. */
  size_t	num=0, len;
  const char *	p = val.chars;
  len = strlen(p);
  while (len--)
    num = (num << 1)^*p++;
  return num;
}


/** --------------------------------------------------------------------
 ** Quick sort.
 ** ----------------------------------------------------------------- */

/* If  you consider  tuning this  algorithm, you  should  consult first:
   Engineering  a sort function;  Jon Bentley  and M.   Douglas McIlroy;
   Software  -  Practice  and  Experience;  Vol.   23  (11),  1249-1265,
   1993.  */

#include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* Byte-wise swap two items of size SIZE. */
#define SWAP(a, b, size)			\
  do {						\
    register size_t __size = (size);		\
    register uint8_t *__a = (a), *__b = (b);	\
    do {					\
      uint8_t __tmp = *__a;			\
      *__a++ = *__b;				\
      *__b++ = __tmp;				\
    } while (--__size > 0);			\
  } while (0)

/* Discontinue quicksort algorithm when  partition gets below this size.
   This  particular  magic number  was  chosen to  work  best  on a  Sun
   4/260. */
#define MAX_THRESH 4

/* Stack   node  declarations  used   to  store   unfulfilled  partition
   obligations. */
typedef struct {
  uint8_t *lo;
  uint8_t *hi;
} stack_node;

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log  (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since  total_elements has  type size_t, we  get as
   upper  bound for  log (total_elements):  bits per  byte  (CHAR_BIT) *
   sizeof(size_t).  */
#define STACK_SIZE	(CHAR_BIT * sizeof(size_t))
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))
#define	STACK_NOT_EMPTY	(stack < top)


/* Order  size using quicksort.   This implementation  incorporates four
   optimizations discussed in Sedgewick:

   1. Non-recursive, using  an explicit stack of pointer  that store the
   next array partition  to sort.  To save time,  this maximum amount of
   space  required to store  an array  of SIZE_MAX  is allocated  on the
   stack.  Assuming  a 32-bit  (64 bit) integer  for size_t,  this needs
   only 32 *  sizeof(stack_node) == 256 bytes (for  64 bit: 1024 bytes).
   Pretty cheap, actually.

   2.  Chose the pivot  element using  a median-of-three  decision tree.
   This  reduces the  probability of  selecting  a bad  pivot value  and
   eliminates certain extraneous comparisons.

   3.  Only  quicksorts  TOTAL_ELEMS  / MAX_THRESH  partitions,  leaving
   insertion sort  to order the MAX_THRESH items  within each partition.
   This is a  big win, since insertion sort is  faster for small, mostly
   sorted array segments.

   4. The  larger of  the two sub-partitions  is always pushed  onto the
   stack  first, with the  algorithm then  concentrating on  the smaller
   partition.  This  *guarantees* no  more than log  (total_elems) stack
   size is needed (actually O(1) in this case)!  */

void
ucl_quicksort (void *const pbase, size_t total_elems, size_t size, ucl_comparison_t cmp)
{
  ucl_value_t	base = { .bytes = (uint8_t *)pbase };
  const size_t	max_thresh = MAX_THRESH * size;
  ucl_value_t	a, b;


  if (total_elems == 0)
    /* Avoid lossage with unsigned arithmetic below.  */
    return;

  if (total_elems > MAX_THRESH) {
    ucl_value_t lo = { .bytes = base.bytes };
    ucl_value_t hi = { .bytes = &lo.bytes[size * (total_elems - 1)] };
    stack_node stack[STACK_SIZE];
    stack_node *top = stack;

    PUSH (NULL, NULL);

    while (STACK_NOT_EMPTY) {
      ucl_value_t left;
      ucl_value_t right;

      /* Select median value  from among LO, MID, and  HI.  Rearrange LO
	 and  HI  so the  three  values  are  sorted.  This  lowers  the
	 probability of  picking a pathological pivot value  and skips a
	 comparison for both the LEFT.BYTES and RIGHT.BYTES in the while
	 loops. */

      ucl_value_t mid = { .bytes = lo.bytes + size * ((hi.bytes - lo.bytes) / size >> 1) };

      if (cmp.func(cmp.data, mid, lo) < 0)
	SWAP (mid.bytes, lo.bytes, size);
      if (cmp.func(cmp.data, hi, mid) < 0)
	SWAP (mid.bytes, hi.bytes, size);
      else
	goto jump_over;
      if (cmp.func(cmp.data, mid, lo) < 0)
	SWAP (mid.bytes, lo.bytes, size);
    jump_over:;
      left.bytes  = lo.bytes + size;
      right.bytes = hi.bytes - size;

      /* Here's the famous ``collapse the walls'' section of quicksort.
	 Gotta like those tight inner loops!  They are the main reason
	 that this algorithm runs much faster than others. */
      do {
	while (cmp.func(cmp.data, left, mid) < 0) {
	  left.bytes += size;
	  a.bytes = left.bytes;
	}
	while (cmp.func(cmp.data, mid, right) < 0) {
	  right.bytes -= size;
	  b.bytes = right.bytes;
	}
	if (left.bytes < right.bytes) {
	  SWAP (left.bytes, right.bytes, size);
	  if (mid.bytes == left.bytes)
	    mid.bytes = right.bytes;
	  else if (mid.bytes == right.bytes)
	    mid.bytes = left.bytes;
	  left.bytes  += size;
	  right.bytes -= size;
	} else if (left.bytes == right.bytes) {
	  left.bytes  += size;
	  right.bytes -= size;
	  break;
	}
      } while (left.bytes <= right.bytes);

      /* Set up  pointers for  next iteration.  First  determine whether
	 left and right partitions are below the threshold size.  If so,
	 ignore  one or  both.  Otherwise,  push the  larger partition's
	 bounds on the stack and continue sorting the smaller one. */

      if ((size_t) (right.bytes - lo.bytes) <= max_thresh) {
	if ((size_t) (hi.bytes - left.bytes) <= max_thresh)
	  /* Ignore both small partitions. */
	  POP (lo.bytes, hi.bytes);
	else
	  /* Ignore small left partition. */
	  lo.bytes = left.bytes;
      } else if ((size_t) (hi.bytes - left.bytes) <= max_thresh)
	/* Ignore small right partition. */
	hi.bytes = right.bytes;
      else if ((right.bytes - lo.bytes) > (hi.bytes - left.bytes)) {
	/* Push larger left partition indices. */
	PUSH (lo.bytes, right.bytes);
	lo.bytes = left.bytes;
      } else {
	/* Push larger right partition indices. */
	PUSH (left.bytes, hi.bytes);
	hi.bytes = right.bytes;
      }
    }
  }

  /* Once the BASE.BYTES array is partially sorted by quicksort the rest
     is completely sorted using  insertion sort, since this is efficient
     for  partitions below  MAX_THRESH  size. BASE.BYTES  points to  the
     beginning of the array to sort, and END_PTR points at the very last
     element in the array (*not* one beyond it!). */

#define min(x, y) ((x) < (y) ? (x) : (y))

  {
    uint8_t *const end_ptr = &base.bytes[size * (total_elems - 1)];
    uint8_t *thresh = min(end_ptr, base.bytes + max_thresh);
    ucl_value_t run, tmp = base;

    /* Find  smallest element  in first  threshold and  place it  at the
       array's beginning.   This is the smallest array  element, and the
       operation speeds up insertion sort's inner loop. */
    for (run.bytes = tmp.bytes + size; run.bytes <= thresh; run.bytes += size) {
      if (cmp.func(cmp.data, run, tmp) < 0)
	tmp.bytes = run.bytes;
    }
    if (tmp.bytes != base.bytes)
      SWAP (tmp.bytes, base.bytes, size);

    /* Insertion    sort,   running    from    left-hand-side   up    to
       right-hand-side.  */
    run.bytes = base.bytes + size;
    while ((run.bytes += size) <= end_ptr) {
      tmp.bytes = run.bytes - size;
      while (cmp.func(cmp.data, run, tmp) < 0) {
	tmp.bytes -= size;
	b.bytes = (void *) tmp.bytes;
      }
      tmp.bytes += size;
      if (tmp.bytes != run.bytes) {
	uint8_t *trav;
	trav = run.bytes + size;
	while (--trav >= run.bytes) {
	  uint8_t c = *trav;
	  uint8_t *hi, *lo;
	  for (hi = lo = trav; (lo -= size) >= tmp.bytes; hi = lo)
	    *hi = *lo;
	  *hi = c;
	}
      }
    }
  }
}

/* end of file */
