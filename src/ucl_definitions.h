/*
   Part of: Useless Containers Library
   Contents: basic definitions for the UCL
   Date: Thu Sep 25, 2008
   
   Abstract
   
   
   
   Copyright (c) 2001-2007, 2008 Marco Maggi <marcomaggi@gna.org>
   
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


#ifndef UCL_DEFINITIONS_H
#define UCL_DEFINITIONS_H 1


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Basic definitions.
 ** ----------------------------------------------------------*/

#undef UCL_BEGIN_C_DECL
#undef UCL_END_C_DECL
#ifdef __cplusplus
#  define UCL_BEGIN_C_DECL		extern "C" {
#  define UCL_END_C_DECL		}
#else
#  define UCL_BEGIN_C_DECL		/* empty */
#  define UCL_END_C_DECL		/* empty */
#endif

UCL_BEGIN_C_DECL

/* ------------------------------------------------------------ */

#ifndef ucl_decl
#  define ucl_decl		extern
#endif

/* The macro UCL_UNUSED indicates  that a function, function argument or
   variable may potentially be unused.  Examples:

	static int unused_function (char arg) UCL_UNUSED;
	int foo (char unused_argument UCL_UNUSED);
	int unused_variable UCL_UNUSED;
*/
#ifdef __GNUC__
#  define UCL_UNUSED		__attribute__((unused))
#else
#  define UCL_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

#ifdef __GNUC__
#  define UCL_LONG_BIGGER_THAN_POINTER	(__SIZEOF_LONG__ > __SIZEOF_POINTER__)
#else
/* this is probably wrong */
#  define UCL_LONG_BIGGER_THAN_POINTER	(sizeof(long) > sizeof(void *))
#endif

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Common declarations for inline functions.
 ** ----------------------------------------------------------*/

#define ucl_inline		static __inline__ __attribute__((__always_inline__,__nonnull__))
#define ucl_inline_nonnull	static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
#define ucl_inline_pure		static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__))

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Common type definitions.
 ** ----------------------------------------------------------*/

typedef _Bool			ucl_bool_t;
typedef size_t			ucl_index_t;
typedef ucl_index_t		ucl_vector_index_t;
typedef void *			ucl_data_t;
typedef void * ucl_pointer_map_fun_t (void *);

/* ------------------------------------------------------------ */

typedef union ucl_value_t {
  char		t_char;
  unsigned char	t_unsigned_char;
  int		t_int;
  unsigned int	t_unsigned_int;
  long		t_long;
  unsigned long	t_unsigned_long;

  int8_t	t_int8;
  uint8_t	t_uint8;
  int16_t	t_int16;
  uint16_t	t_uint16;
  int32_t	t_int32;
  uint32_t	t_uint32;

  void *	pointer;
  uint8_t *	bytes;
  char *	string;

  /* to be removed */
  unsigned	unum;
  int		num;
  int		integer;
  char *	str;
  void *	ptr;
  /* end to be removed */

  /* The following are used by the UCL. */
  ucl_pointer_map_fun_t *	pointer_map_function;
  struct ucl_value_flags_t {
    unsigned int avl_status: 1;
    unsigned int marked:     1;
  }				flags;
} ucl_value_t;

#if (UCL_LONG_BIGGER_THAN_POINTER)
#  define UCL_VALUE_NULL_VALUE		{ .t_long = 0L }
#else
#  define UCL_VALUE_NULL_VALUE		{ .pointer = NULL }
#endif

/* ------------------------------------------------------------ */

typedef int ucl_comparison_fun_t(ucl_value_t data, const ucl_value_t a, const ucl_value_t b);

typedef struct ucl_comparison_t {
  ucl_value_t			data;
  ucl_comparison_fun_t *	func;
} ucl_comparison_t;

/* ------------------------------------------------------------ */

typedef ucl_index_t ucl_hash_fun_t (ucl_value_t data, ucl_value_t key);

typedef struct ucl_hash_t {
  ucl_value_t		data;
  ucl_hash_fun_t *	func;
} ucl_hash_t;

/* ------------------------------------------------------------ */

typedef struct ucl_link_t {
  struct ucl_link_t *	next_p;
} ucl_link_t;

/* ------------------------------------------------------------ */

typedef struct ucl_array_of_pointers_t {
  ucl_value_t	data;
  size_t	number_of_slots;
  void **	slots;
} ucl_array_of_pointers_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Memory allocation.
 ** ----------------------------------------------------------*/

typedef void ucl_memory_alloc_fun_t (void * dummy, void * pp, size_t dim);

typedef struct ucl_memory_allocator_t {
  void *			data;
  ucl_memory_alloc_fun_t *	alloc;
} ucl_memory_allocator_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Memory block macros.
 ** ----------------------------------------------------------*/

typedef struct ucl_block_t {
  size_t	len;
  uint8_t *	ptr;
} ucl_block_t;

#define UCL_BLOCK_NULL_VALUE	{ 0, NULL }
#define UCL_BLOCK_EMPTY		UCL_BLOCK_NULL_VALUE
#define UCL_EMPTY_BLOCK		UCL_BLOCK_NULL_VALUE

/* ------------------------------------------------------------ */

ucl_inline void
ucl_block_set (ucl_block_t * block, void * p, size_t len)
{
  block->ptr = p;
  block->len = len;
}
ucl_inline void
ucl_block_reset (ucl_block_t * block)
{
  block->ptr = NULL;
  block->len = 0;
}
ucl_inline ucl_bool_t
ucl_block_is_null (ucl_block_t block)
{
  return (NULL == block.ptr);
}
ucl_inline void
ucl_block_clean_memory (ucl_block_t block)
{
  memset(block.ptr, '\0', block.len);
}

/* ------------------------------------------------------------ */

ucl_inline ucl_block_t
ucl_block_alloc (ucl_memory_allocator_t allocator, size_t dim)
{
  ucl_block_t	block = UCL_EMPTY_BLOCK;

  allocator.alloc(allocator.data, &(block.ptr), dim);
  block.len = dim;
  return block;
}
ucl_inline ucl_block_t
ucl_block_realloc (ucl_memory_allocator_t allocator, ucl_block_t block, size_t new_dim)
{
  allocator.alloc(allocator.data, &(block.ptr), new_dim);
  block.len = new_dim;
  return block;
}
ucl_inline void
ucl_block_free (ucl_memory_allocator_t allocator, ucl_block_t block)
{
  if (block.ptr)
    allocator.alloc(allocator.data, &(block.ptr), 0);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** ASCII string macros.
 ** ----------------------------------------------------------*/

typedef struct ucl_ascii_t {
  size_t	len;
  char *	ptr;
} ucl_ascii_t;

typedef struct ucl_ascii_list_struct_t {
  size_t	len;
  char **	ptr;
} ucl_ascii_list_t;

#define UCL_ASCII_NULL_VALUE	{ 0, NULL }
#define UCL_ASCII_EMPTY		UCL_ASCII_NULL_VALUE
#define UCL_EMPTY_ASCII		UCL_ASCII_NULL_VALUE

/* ------------------------------------------------------------ */

ucl_inline void
ucl_ascii_set (ucl_ascii_t * ascii, void * ptr, size_t len)
{
  ascii->ptr = ptr;
  ascii->len = len;
}
ucl_inline void
ucl_ascii_reset (ucl_ascii_t * ascii)
{
  ascii->ptr = NULL;
  ascii->len = 0;
}
ucl_inline ucl_ascii_t
ucl_ascii_const (const char * string)
{
  ucl_ascii_t	ascii;

  ascii.ptr = (void *)string;
  ascii.len = strlen(string);
  return ascii;
}
ucl_inline ucl_bool_t
ucl_ascii_is_null (ucl_ascii_t ascii)
{
  return (NULL == ascii.ptr);
}
ucl_inline void
ucl_ascii_clean_memory (ucl_ascii_t ascii)
{
  memset(ascii.ptr,'\0',ascii.len);
}
ucl_inline ucl_ascii_t
ucl_ascii_from_block (ucl_block_t block)
{
  ucl_ascii_t	ascii;

  ascii.len = block.len - 1;
  ascii.ptr = (void *)block.ptr;
  ascii.ptr[block.len] = '\0';
  return ascii;
}
ucl_inline ucl_block_t
ucl_block_from_ascii (ucl_ascii_t ascii)
{
  ucl_block_t	block;

  block.len = 1 + ascii.len;
  block.ptr = (void *)ascii.ptr;
  return block;
}

/* ------------------------------------------------------------ */

ucl_inline ucl_ascii_t
ucl_ascii_alloc (ucl_memory_allocator_t allocator, size_t dim)
{
  ucl_ascii_t	ascii = UCL_ASCII_NULL_VALUE;

  allocator.alloc(allocator.data, &(ascii.ptr), 1+dim);
  ascii.len = dim;
  ascii.ptr[dim]='\0';
  return ascii;
}
ucl_inline ucl_ascii_t
ucl_ascii_realloc (ucl_memory_allocator_t allocator, ucl_ascii_t ascii, size_t new_dim)
{
  allocator.alloc(allocator.data, &(ascii.ptr), 1+new_dim);
  ascii.len = new_dim;
  ascii.ptr[new_dim]='\0';
  return ascii;
}
ucl_inline void
ucl_ascii_free (ucl_memory_allocator_t allocator, ucl_ascii_t ascii)
{
  if (ascii.ptr)
    allocator.alloc(allocator.data, &(ascii.ptr), 0);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Ranges.
 ** ----------------------------------------------------------*/

/* Ranges are  inclusive. Range stuff  is written in macros,  not inline
   functions, so  that they  work with all  the type  structures defined
   below. */

#define ucl_range_set_min_max(RANGE,MIN,MAX)	\
		((RANGE).min = (MIN), (RANGE).max = (MAX))

#define ucl_range_set_min_size(RANGE,MIN,SIZE)	\
		((RANGE).min = (MIN), (RANGE).max = ((MIN)+(SIZE)-1))

#define ucl_range_set_max_size(RANGE,MAX,SIZE)	\
		((RANGE).min = ((MAX)-(SIZE)+1), (RANGE).max = (MAX))

#define ucl_range_set_size_on_min(RANGE, SIZE)	\
		((RANGE).max = (RANGE).min+(SIZE)-1)

#define ucl_range_set_size_on_max(RANGE, SIZE)	\
		((RANGE).min = (RANGE).max-(SIZE)+1)

#define ucl_range_is_empty(RANGE)	((RANGE).min > (RANGE).max)
#define ucl_range_size(RANGE)		((RANGE).max - (RANGE).min + 1)
#define ucl_range_min(RANGE)		((RANGE).min)
#define ucl_range_max(RANGE)		((RANGE).max)

#define ucl_range_equal(RA, RB)		((RA) == (RB))

#define ucl_range_value_is_in(RANGE,VALUE)	\
		(((RANGE).min <= (VALUE)) && ((VALUE) <= (RANGE).max))

#define ucl_range_value_is_out(RANGE,VALUE)	\
		(((VALUE) < (RANGE).min) || ((RANGE).max < (VALUE)))

/* ------------------------------------------------------------ */

typedef struct ucl_range_t {
  size_t	min;
  size_t	max;
} ucl_range_t;

typedef struct ucl_char_range_t {
  char		min;
  char		max;
} ucl_char_range_t;

typedef struct ucl_unsigned_char_range_t {
  unsigned char	min;
  unsigned char	max;
} ucl_unsigned_char_range_t;

typedef struct ucl_int_range_t {
  int		min;
  int		max;
} ucl_int_range_t;

typedef struct ucl_unsigned_range_t {
  unsigned	min;
  unsigned	max;
} ucl_unsigned_range_t;

typedef struct ucl_long_range_t {
  long		min;
  long		max;
} ucl_long_range_t;

typedef struct ucl_unsigned_long_range_t {
  unsigned long	min;
  unsigned long	max;
} ucl_unsigned_long_range_t;

typedef struct ucl_size_t_range_t {
  size_t		min;
  size_t		max;
} ucl_size_t_range_t;

typedef struct ucl_float_range_t {
  float		min;
  float		max;
} ucl_float_range_t;

typedef struct ucl_double_range_t {
  double	min;
  double	max;
} ucl_double_range_t;

typedef struct ucl_byte_pointer_range_t {
  uint8_t *	min;
  uint8_t *	max;
} ucl_byte_pointer_range_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Struct macros.
 ** ----------------------------------------------------------*/

#define ucl_struct_clean(B,TYPE_T)	memset((B),'\0',sizeof(TYPE_T))
#define ucl_struct_reset(B,TYPE_T)	ucl_struct_clean((B),(TYPE_T))

ucl_inline void
ucl_p_struct_alloc (ucl_memory_allocator_t * allocator, void ** buffer_pp, size_t len)
{
  void *	p = NULL;

  allocator->alloc(allocator->data, &p, len);
  *buffer_pp = p;
}
#define ucl_struct_alloc(ALLOCATOR,P,TYPE_T)	ucl_p_struct_alloc(ALLOCATOR,&(P),sizeof(TYPE_T))

/* ------------------------------------------------------------ */



/** ------------------------------------------------------------
 ** Callback.
 ** ----------------------------------------------------------*/

typedef ucl_value_t ucl_callback_fun_t (ucl_value_t callback_data, va_list ap);

typedef struct ucl_callback_t {
  ucl_callback_fun_t *	func;
  ucl_value_t		data;
} ucl_callback_t;

#define UCL_CALLBACK_NULL_VALUE		{ .func = NULL, .data = { .ptr = 0 } }

typedef ucl_value_t ucl_callback_apply_fun_t (ucl_callback_t callback, ...);

/* ------------------------------------------------------------ */

ucl_inline_pure ucl_bool_t
ucl_callback_is_present (ucl_callback_t callback)
{
  return (NULL != callback.func);
}
static __inline__ ucl_value_t
ucl_callback_apply (ucl_callback_t callback, ...)
{
  ucl_value_t	retval = UCL_VALUE_NULL_VALUE;

  if (ucl_callback_is_present(callback))
    {
      va_list		ap;


      va_start(ap,callback);
      retval = callback.func(callback.data, ap);

      /* This  does nothing  with  the GNU  C  Library; it  is here  for
	 compatibility.  If  the callback function  raises an exception:
	 nothing bad happend with the GNU C Library. */
      va_end(ap);
    }
  return retval;
}
static __inline__ ucl_value_t
ucl_callback_eval_thunk (ucl_callback_t callback)
{
  ucl_value_t	retval = UCL_VALUE_NULL_VALUE;

  if (ucl_callback_is_present(callback))
    retval = callback.func(callback.data,NULL);
  return retval;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Generic iterator.
 ** ----------------------------------------------------------*/

typedef struct ucl_iterator_tag_t	ucl_iterator_tag_t;
typedef void ucl_iterator_next_t (ucl_iterator_tag_t *iterator);

struct ucl_iterator_tag_t {
  void *		iterator;
  const void *		container;
  ucl_iterator_next_t *	next;
  ucl_value_t		internal1;
  ucl_value_t		internal2;
};

typedef ucl_iterator_tag_t		ucl_iterator_t[1];

/* ------------------------------------------------------------ */

ucl_inline_pure ucl_bool_t	
ucl_iterator_more (ucl_iterator_t iterator)
{
  return (iterator->iterator != 0);
}
ucl_inline void
ucl_iterator_next (ucl_iterator_t iterator)
{
  iterator->next(iterator);
}
ucl_inline_pure void *
ucl_iterator_ptr (ucl_iterator_t iterator)
{
  return iterator->iterator;
}

/* ------------------------------------------------------------ */

static __inline__ void
ucl_iterator_for_each (ucl_iterator_t iterator, ucl_callback_t F)
{
  for (; ucl_iterator_more(iterator); ucl_iterator_next(iterator))
    {
      ucl_callback_apply(F,ucl_iterator_ptr(iterator));
    }
}
static __inline__ void
ucl_iterator_map (ucl_iterator_t output, ucl_iterator_t input, ucl_callback_t F)
{
  for (;
       ucl_iterator_more(output) && ucl_iterator_more(input);
       ucl_iterator_next(output), ucl_iterator_next(input))
    {
      ucl_callback_apply(F,ucl_iterator_ptr(output),ucl_iterator_ptr(input));
    }
}

#endif /* UCL_DEFINITIONS_H */

/* end of file */
