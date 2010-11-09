/*
  Part of: Useless Containers Library
  Contents: main header file
  Date: late 2001

  Abstract

	This file must be included in every module that makes use of the
	UCL containers.

  Copyright (c)  2001-2010  Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this  program.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef __UCL_H
#define __UCL_H 1


/** --------------------------------------------------------------------
 ** Headers.
 ** -----------------------------------------------------------------*/

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Basic definitions.
 ** -----------------------------------------------------------------*/

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

/* ------------------------------------------------------------------ */

/* The macro UCL_UNUSED indicates  that a function, function argument or
   variable may potentially be unused.  Usage examples:

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


/** --------------------------------------------------------------------
 ** Common type definitions.
 ** -----------------------------------------------------------------*/

typedef _Bool			ucl_bool_t;
typedef size_t			ucl_index_t;
typedef ucl_index_t		ucl_vector_index_t;
typedef void *			ucl_data_t;
typedef void *			ucl_pointer_t;
typedef void * ucl_pointer_map_fun_t (void *);

typedef union ucl_value_t {
  char		t_char;
  unsigned char	t_unsigned_char;
  int			t_int;
  unsigned int	t_unsigned_int;
  long		t_long;
  unsigned long	t_unsigned_long;

  int8_t	t_int8;
  uint8_t	t_uint8;
  int16_t	t_int16;
  uint16_t	t_uint16;
  int32_t	t_int32;
  uint32_t	t_uint32;

  intptr_t	t_intptr;
  uintptr_t	t_uintptr;

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

#define UCL_VALUE_NULL_VALUE	{ .t_uintptr = 0 }

/* ------------------------------------------------------------ */

typedef int ucl_value_comparison_fun_t	(ucl_value_t data,
					 const ucl_value_t a,
					 const ucl_value_t b);

typedef struct ucl_value_comparison_t {
  ucl_value_t				data;
  ucl_value_comparison_fun_t *	func;
} ucl_value_comparison_t;

typedef int ucl_comparison_fun_t	(ucl_value_t data,
					 const void * a,
					 const void * b);

typedef struct ucl_comparison_t {
  ucl_value_t			data;
  ucl_comparison_fun_t *	func;
} ucl_comparison_t;

/* ------------------------------------------------------------ */

typedef ucl_index_t ucl_hash_fun_t	(ucl_value_t data,
					 ucl_value_t key);

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



/** --------------------------------------------------------------------
 ** Miscellaneous functions.
 ** -----------------------------------------------------------------*/

extern const char * ucl_version (void);
extern unsigned	ucl_interface_major_version (void);
extern unsigned	ucl_interface_minor_version (void);

extern ucl_value_comparison_fun_t	ucl_intcmp;
extern ucl_value_comparison_fun_t	ucl_uintcmp;
extern ucl_value_comparison_fun_t	ucl_strcmp;
extern ucl_value_comparison_fun_t	ucl_ptrintcmp;

extern ucl_hash_fun_t			ucl_hash_string;

extern void	ucl_quicksort (void *const pbase, size_t total_elems,
			       size_t size, ucl_comparison_t cmp);


/** --------------------------------------------------------------------
 ** Memory allocation.
 ** -----------------------------------------------------------------*/

typedef void ucl_memory_alloc_fun_t (void * dummy, void * pp, size_t dim);

typedef struct ucl_memory_allocator_t {
  void *			data;
  ucl_memory_alloc_fun_t *	alloc;
} ucl_memory_allocator_t;

extern ucl_memory_alloc_fun_t ucl_memory_alloc;

static __inline__ __attribute__((__always_inline__,__pure__))
void *
ucl_malloc (ucl_memory_allocator_t allocator, size_t number_of_bytes)
{
  void *	p = NULL;
  allocator.alloc(allocator.data,&p,number_of_bytes);
  return p;
}
static __inline__ __attribute__((__always_inline__,__pure__))
void *
ucl_realloc (ucl_memory_allocator_t allocator, void * p, size_t number_of_bytes)
{
  allocator.alloc(allocator.data,&p,number_of_bytes);
  return p;
}
static __inline__ __attribute__((__always_inline__,__pure__))
void
ucl_free (ucl_memory_allocator_t allocator, void * p)
{
  allocator.alloc(allocator.data,&p,0);
}


/** --------------------------------------------------------------------
 ** Memory block macros.
 ** -----------------------------------------------------------------*/

typedef struct ucl_block_t {
  size_t	len;
  uint8_t *	ptr;
} ucl_block_t;

static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_block_set (ucl_block_t * block, void * p, size_t len)
{
  block->ptr = p;
  block->len = len;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_block_reset (ucl_block_t * block)
{
  block->ptr = NULL;
  block->len = 0;
}
static __inline__ __attribute__((__always_inline__))
ucl_bool_t
ucl_block_is_null (ucl_block_t block)
{
  return (NULL == block.ptr);
}
static __inline__ __attribute__((__always_inline__))
void
ucl_block_clean_memory (ucl_block_t block)
{
  memset(block.ptr, '\0', block.len);
}
static __inline__ __attribute__((__always_inline__))
ucl_block_t
ucl_block_alloc (ucl_memory_allocator_t allocator, size_t dim)
{
  ucl_block_t	block = { .ptr = NULL, .len = dim };
  allocator.alloc(allocator.data, &(block.ptr), dim);
  return block;
}
static __inline__ __attribute__((__always_inline__))
ucl_block_t
ucl_block_realloc (ucl_memory_allocator_t allocator, ucl_block_t block, size_t new_dim)
{
  allocator.alloc(allocator.data, &(block.ptr), new_dim);
  block.len = new_dim;
  return block;
}
static __inline__ __attribute__((__always_inline__))
void
ucl_block_free (ucl_memory_allocator_t allocator, ucl_block_t block)
{
  if (block.ptr)
    allocator.alloc(allocator.data, &(block.ptr), 0);
}
static __inline__ void
ucl_block_shift_x (ucl_block_t * block, ssize_t offset, size_t dim)
{
  if (dim) {
    if (1 != dim) {
      block->ptr += offset;
      block->len -= offset;
    } else {
      block->ptr += offset * dim;
      block->len -= offset * dim;
    }
  }
}
static __inline__ ucl_block_t
ucl_block_shift (ucl_block_t A, ssize_t offset, size_t dim)
{
  ucl_block_t	B;
  if (dim) {
    if (1 != dim) {
      B.ptr = A.ptr + offset;
      B.len = A.len + offset;
    } else {
      B.ptr = A.ptr + offset * dim;
      B.len = A.len + offset * dim;
    }
  } else {
    B = A;
  }
  return B;
}
static __inline__ __attribute__((__always_inline__))
ucl_block_t
ucl_block_difference (ucl_block_t A, ucl_block_t B)
{
  ucl_block_t	C = { .ptr = A.ptr, .len = B.ptr - A.ptr };
  return C;
}


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

static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_ascii_set (ucl_ascii_t * ascii, void * ptr, size_t len)
{
  ascii->ptr = ptr;
  ascii->len = len;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_ascii_reset (ucl_ascii_t * ascii)
{
  ascii->ptr = NULL;
  ascii->len = 0;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_ascii_t
ucl_ascii_const (char * string)
{
  ucl_ascii_t	ascii;
  ascii.ptr = (void *)string;
  ascii.len = strlen(string);
  return ascii;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_bool_t
ucl_ascii_is_null (ucl_ascii_t ascii)
{
  return (NULL == ascii.ptr);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_ascii_clean_memory (ucl_ascii_t ascii)
{
  memset(ascii.ptr,'\0',ascii.len);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_ascii_t
ucl_ascii_from_block (ucl_block_t block)
{
  ucl_ascii_t	ascii;

  ascii.len = block.len - 1;
  ascii.ptr = (void *)block.ptr;
  ascii.ptr[block.len] = '\0';
  return ascii;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_block_t
ucl_block_from_ascii (ucl_ascii_t ascii)
{
  ucl_block_t	block;

  block.len = 1 + ascii.len;
  block.ptr = (void *)ascii.ptr;
  return block;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_ascii_t
ucl_ascii_alloc (ucl_memory_allocator_t allocator, size_t dim)
{
  ucl_ascii_t	ascii = { .ptr = NULL };

  allocator.alloc(allocator.data, &(ascii.ptr), 1+dim);
  ascii.len = dim;
  ascii.ptr[dim]='\0';
  return ascii;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
ucl_ascii_t
ucl_ascii_realloc (ucl_memory_allocator_t allocator,
		   ucl_ascii_t ascii, size_t new_dim)
{
  allocator.alloc(allocator.data, &(ascii.ptr), 1+new_dim);
  ascii.len = new_dim;
  ascii.ptr[new_dim]='\0';
  return ascii;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_ascii_free (ucl_memory_allocator_t allocator, ucl_ascii_t ascii)
{
  if (ascii.ptr)
    allocator.alloc(allocator.data, &(ascii.ptr), 0);
}


/** ------------------------------------------------------------
 ** Ranges.
 ** ----------------------------------------------------------*/

/* Ranges are  inclusive. Range stuff  is written in macros,  not inline
   functions, so  that they  work with all  the type  structures defined
   below. */

#define ucl_range_set_min_max(RANGE,MIN,MAX)	\
  ((RANGE).min = (MIN), (RANGE).max = (MAX))

#define ucl_range_set_min_size(RANGE,MIN,SIZE)		\
  ((RANGE).min = (MIN), (RANGE).max = ((MIN)+(SIZE)-1))

#define ucl_range_set_max_size(RANGE,MAX,SIZE)		\
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

#define ucl_range_value_is_in(RANGE,VALUE)			\
  (((RANGE).min <= (VALUE)) && ((VALUE) <= (RANGE).max))

#define ucl_range_value_is_out(RANGE,VALUE)		\
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



/** ------------------------------------------------------------
 ** Struct macros.
 ** ----------------------------------------------------------*/

#define ucl_struct_clean(B,TYPE_T)	memset((B),'\0',sizeof(TYPE_T))
#define ucl_struct_reset(B,TYPE_T)	ucl_struct_clean((B),(TYPE_T))

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_p_struct_alloc (ucl_memory_allocator_t * allocator,
		    void ** buffer_pp,
		    size_t len)
{
  void *	p = NULL;

  allocator->alloc(allocator->data, &p, len);
  *buffer_pp = p;
}
#define ucl_struct_alloc(ALLOCATOR,P,TYPE_T)		\
  ucl_p_struct_alloc(ALLOCATOR,&(P),sizeof(TYPE_T))



/** ------------------------------------------------------------
 ** Callback.
 ** ----------------------------------------------------------*/

typedef ucl_value_t ucl_callback_fun_t	(ucl_value_t callback_data,
					 va_list ap);

typedef struct ucl_callback_t {
  ucl_callback_fun_t *	func;
  ucl_value_t		data;
} ucl_callback_t;

#define UCL_CALLBACK_NULL_VALUE	{ .func = NULL, .data = { .ptr = 0 } }

typedef ucl_value_t ucl_callback_apply_fun_t (ucl_callback_t callback, ...);

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_bool_t
ucl_callback_is_present (ucl_callback_t callback)
{
  return (NULL != callback.func);
}
static __inline__ ucl_value_t
ucl_callback_apply (ucl_callback_t callback, ...)
{
  ucl_value_t	retval = UCL_VALUE_NULL_VALUE;
  if (ucl_callback_is_present(callback)) {
    va_list	ap;
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

extern void ucl_callback_set_application_function (ucl_callback_apply_fun_t * f);


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

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_bool_t
ucl_iterator_more (ucl_iterator_t iterator)
{
  return (iterator->iterator != 0);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_iterator_next (ucl_iterator_t iterator)
{
  iterator->next(iterator);
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
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


/** ------------------------------------------------------------
 ** Container configuration symbols.
 ** ----------------------------------------------------------*/

#ifndef UCL_VECTOR_DEFAULT_SIZE
#  define UCL_VECTOR_DEFAULT_SIZE	16
#endif
#ifndef UCL_VECTOR_DEFAULT_STEP_UP
#  define UCL_VECTOR_DEFAULT_STEP_UP	 8
#endif
#ifndef UCL_VECTOR_DEFAULT_STEP_DOWN
#  define UCL_VECTOR_DEFAULT_STEP_DOWN	10
#endif
#ifndef UCL_VECTOR_DEFAULT_PAD
#  define UCL_VECTOR_DEFAULT_PAD	 3
#endif

#ifndef UCL_VECTOR_BUFFER_PAGE_SIZE
#  define UCL_VECTOR_BUFFER_PAGE_SIZE	4096
#endif

/* ------------------------------------------------------------ */

#ifndef UCL_HASH_DEFAULT_SIZE
#  define UCL_HASH_DEFAULT_SIZE		37
#endif

#ifndef UCL_HASH_MINIMUM_SIZE
#  define UCL_HASH_MINIMUM_SIZE		13
#endif

#ifndef UCL_HASH_DEFAULT_STEP_UP
#  define UCL_HASH_DEFAULT_STEP_UP	16
#endif

#ifndef UCL_HASH_DEFAULT_STEP_DOWN
#  define UCL_HASH_DEFAULT_STEP_DOWN	((UCL_HASH_DEFAULT_STEP_UP)+1)
#endif


/** ------------------------------------------------------------
 ** Container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_node_tag_t {
  struct ucl_node_tag_t *	dad;
  struct ucl_node_tag_t *	bro;
  struct ucl_node_tag_t *	son;
  ucl_value_t			meta;
} ucl_node_tag_t;

typedef ucl_node_tag_t *	ucl_node_t;

/* FIXME: one day this will disappear */
typedef ucl_node_t		ucl_btree_node_t;

/* BEGIN: RESERVED FOR INTERNAL USAGE */
typedef struct ucl_embedded_node_tag_t {
  ucl_node_tag_t	node;
  uint8_t		data[];
} ucl_embedded_node_tag_t;
typedef ucl_embedded_node_tag_t *	ucl_embedded_node_t;
/* END: RESERVED FOR INTERNAL USAGE */

/* ------------------------------------------------------------ */

typedef struct ucl_heap_tag_t {
  size_t		size;
  ucl_node_t		root;
  ucl_node_t		next;
  int			state;
  ucl_comparison_t	compar;
} ucl_heap_tag_t;

typedef ucl_heap_tag_t *	ucl_heap_t;

/* ------------------------------------------------------------ */

typedef struct ucl_vector_config_tag_t {
  ucl_index_t			step_up;
  ucl_index_t			step_down;
  ucl_index_t			size_of_padding_area;
  size_t			slot_dimension;
  size_t			number_of_slots;
  ucl_comparison_t		compar;
  ucl_memory_allocator_t	allocator;
} ucl_vector_config_tag_t;

typedef ucl_vector_config_tag_t	ucl_vector_config_t[1];

typedef struct ucl_vector_tag_t {
  ucl_index_t	step_up;
  ucl_index_t	step_down;
  ucl_index_t	size_of_padding_area;
  ucl_index_t	slot_dimension;

  ucl_comparison_t		compar;
  ucl_memory_allocator_t	allocator;

  uint8_t *	first_allocated_slot;
  uint8_t *	first_used_slot;
  uint8_t *	last_used_slot;
  uint8_t *	last_allocated_slot;
} ucl_vector_tag_t;

typedef ucl_vector_tag_t ucl_vector_t[1];

typedef struct ucl_vector_view_tag_t {
  ucl_index_t	step_up;
  ucl_index_t	step_down;
  ucl_index_t	size_of_padding_area;
  ucl_index_t	slot_dimension;

  ucl_comparison_t		compar;
  ucl_memory_allocator_t	allocator;

  uint8_t *	first_used_slot;
  uint8_t *	last_used_slot;
  uint8_t *	last_allocated_slot;
  uint8_t *	first_allocated_slot;
} ucl_vector_view_tag_t;

typedef ucl_vector_view_tag_t *		ucl_vector_view_t;

typedef struct ucl_vector_array_t {
  size_t		number_of_vectors;
  ucl_vector_tag_t **	vectors;
} ucl_vector_array_t;

/* ------------------------------------------------------------ */

typedef struct ucl_hash_entry_t {
  struct ucl_hash_entry_t *	next_entry_in_list_p;
  ucl_value_t			key;
  ucl_value_t			val;
  ucl_bool_t			to_be_processed_during_rehashing;
} ucl_hash_entry_t;

typedef struct ucl_hash_table_tag_t {
  ucl_vector_tag_t *		buckets;
  ucl_hash_t			hash;
  ucl_value_comparison_t	compar;
  size_t			size;
  size_t			number_of_used_buckets;
} ucl_hash_table_tag_t;

typedef ucl_hash_table_tag_t ucl_hash_table_t[1];


/** ------------------------------------------------------------
 ** Binary tree prototypes.
 ** ----------------------------------------------------------*/

extern void * ucl_btree_find_value (void * node, void * value, ucl_comparison_t compar)
  __attribute__((__nonnull__,__pure__));

/* ------------------------------------------------------------ */

extern void * ucl_btree_find_leftmost (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_find_rightmost (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_find_deepest_son (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_find_deepest_bro (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_find_root (void * node)
  __attribute__((__nonnull__,__pure__));

/* ------------------------------------------------------------ */

extern void * ucl_btree_step_inorder (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_inorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_preorder (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_preorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_postorder (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_postorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_levelorder (void * node)
  __attribute__((__nonnull__,__pure__));

extern void * ucl_btree_step_levelorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

/* ------------------------------------------------------------ */

extern void ucl_btree_iterator_inorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_inorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_preorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_preorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_postorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_postorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_levelorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

extern void ucl_btree_iterator_levelorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

/* ------------------------------------------------------------ */

extern void ucl_btree_subtree_iterator_inorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

extern void ucl_btree_subtree_iterator_inorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

extern void ucl_btree_subtree_iterator_preorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

extern void ucl_btree_subtree_iterator_preorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

extern void ucl_btree_subtree_iterator_postorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

extern void ucl_btree_subtree_iterator_postorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

#if 0
extern void ucl_btree_subtree_iterator_levelorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_levelorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
#endif

extern void * ucl_btree_find_value (void * node, void * value, ucl_comparison_t compar)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_find_leftmost (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_find_rightmost (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_find_deepest_son (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_find_deepest_bro (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_find_root (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_inorder (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_inorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_preorder (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_preorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_postorder (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_postorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_levelorder (void * node)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_btree_step_levelorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));
extern void ucl_btree_iterator_inorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_inorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_preorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_preorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_postorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_postorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_levelorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_iterator_levelorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_inorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_inorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_preorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_preorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_postorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_postorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_levelorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
extern void ucl_btree_subtree_iterator_levelorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));


/** ------------------------------------------------------------
 ** Map container type definitions.
 ** ----------------------------------------------------------*/

typedef enum {
  LEFT_HIGHER, BALANCED, RIGHT_HIGHER
} ucl_avl_status_t;

typedef struct ucl_map_link_t {
  ucl_node_tag_t	node;
  ucl_avl_status_t	avl_status;
  ucl_value_t		key;
  ucl_value_t		val;
} ucl_map_link_t;

typedef struct ucl_map_struct_t {
  size_t			size;
  unsigned int			flags;
  ucl_value_comparison_t	keycmp;
  ucl_map_link_t *		root;
} ucl_map_struct_t;

typedef ucl_map_struct_t ucl_map_t[1];

#define UCL_ALLOW_MULTIPLE_OBJECTS	0x10

/* ------------------------------------------------------------------ */

static __inline__ size_t
ucl_map_size (const ucl_map_t this)
{
  return this->size;
}
static __inline__ ucl_value_t
ucl_map_getkey (const ucl_map_link_t * link_p)
{
  return link_p->key;
}
static __inline__ ucl_value_t
ucl_map_getval (const ucl_map_link_t * link_p)
{
  return link_p->val;
}
static __inline__ void
ucl_map_setkey (ucl_map_link_t * link_p, ucl_value_t newkey)
{
  link_p->key = newkey;
}
static __inline__ void
ucl_map_setval (ucl_map_link_t * link_p, ucl_value_t newval)
{
  link_p->val = newval;
}


/** ------------------------------------------------------------
 ** Graph container type definitions.
 ** ----------------------------------------------------------*/

struct ucl_graph_node_t;
struct ucl_graph_link_t;

typedef struct ucl_graph_node_t		ucl_graph_node_t;
typedef struct ucl_graph_link_t		ucl_graph_link_t;

typedef struct ucl_graph_link_list_base_t {
  size_t			number_of_links;
  ucl_graph_link_t *		first_link_p;
} ucl_graph_link_list_base_t;

typedef struct ucl_graph_link_list_node_t {
  ucl_graph_link_t *		prev_link_p;
  ucl_graph_link_t *		next_link_p;
} ucl_graph_link_list_node_t;

struct ucl_graph_node_t {
  ucl_graph_link_list_base_t	input;
  ucl_graph_link_list_base_t	output;
  ucl_graph_node_t *		next_node_p;
  ucl_value_t			mark;
  ucl_value_t			val;
};

struct ucl_graph_link_t {
  ucl_graph_node_t *		src_node_p;
  ucl_graph_node_t *		dst_node_p;
  ucl_graph_link_list_node_t	input;
  ucl_graph_link_list_node_t	output;
  ucl_value_t			val;
};

/* ------------------------------------------------------------ */

static __inline__ size_t
ucl_graph_number_of_output_links (ucl_graph_node_t * node_p)
{
  return node_p->output.number_of_links;
}
static __inline__ size_t
ucl_graph_number_of_input_links (ucl_graph_node_t * node_p)
{
  return node_p->input.number_of_links;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_graph_link_t *
ucl_graph_input_link (const ucl_graph_node_t * node_p)
{
  return node_p->input.first_link_p;
}
static __inline__ ucl_graph_link_t *
ucl_graph_output_link (const ucl_graph_node_t * node_p)
{
  return node_p->output.first_link_p;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_graph_link_t *
ucl_graph_prev_input_link (const ucl_graph_link_t * link_p)
{
  return link_p->input.prev_link_p;
}
static __inline__ ucl_graph_link_t *
ucl_graph_next_input_link (const ucl_graph_link_t * link_p)
{
  return link_p->input.next_link_p;
}
static __inline__ ucl_graph_link_t *
ucl_graph_prev_output_link (const ucl_graph_link_t * link_p)
{
  return link_p->output.prev_link_p;
}
static __inline__ ucl_graph_link_t *
ucl_graph_next_output_link (const ucl_graph_link_t * link_p)
{
  return link_p->output.next_link_p;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_value_t
ucl_graph_node_get_value (const ucl_graph_node_t * node_p)
{
  return node_p->val;
}
static __inline__ void
ucl_graph_node_set_value (ucl_graph_node_t * node_p, const ucl_value_t value)
{
  node_p->val = value;
}
static __inline__ ucl_value_t
ucl_graph_link_get_value (const ucl_graph_link_t * link_p)
{
  return link_p->val;
}
static __inline__ void
ucl_graph_link_set_value (ucl_graph_link_t * link_p, const ucl_value_t value)
{
  link_p->val = value;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_value_t
ucl_graph_node_get_mark (const ucl_graph_node_t * node_p)
{
  return node_p->mark;
}
static __inline__ void
ucl_graph_node_set_mark (ucl_graph_node_t * node_p, const ucl_value_t mark)
{
  node_p->mark = mark;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_graph_node_t *
ucl_graph_get_next_node (const ucl_graph_node_t * node_p)
{
  return node_p->next_node_p;
}

/* ------------------------------------------------------------ */

#define UCL_GRAPH_NODE_LIST_LOOP(FIRST,NEXT)	\
						\
  for ((NEXT) = (FIRST);			\
       NULL  != (NEXT);				\
       (NEXT) = ucl_graph_get_next_node(NEXT))

/* The condition  expression in the  'for' statement is composed  of two
   parts:

   1 - if ITER_NODE is non-NULL, then assign the next node to TMP_NODE;

   2 - ITER_NODE is used as conditional expression for the 'for'.

   This usage of  the conditional expression is "special"  and it causes
   GCC to raise a 'computed value not used' warning because, indeed, the
   value computed at point 1 is not used. */
#define UCL_GRAPH_NODE_LIST_DELETE_LOOP(FIRST,ITER_NODE,TMP_NODE)	\
									\
  for ((ITER_NODE) = (FIRST);						\
       (((ITER_NODE) && ((TMP_NODE) = (ITER_NODE)->next_node_p)), (ITER_NODE)); \
       (ITER_NODE) = (TMP_NODE))

/* ------------------------------------------------------------ */

#define UCL_GRAPH_INPUT_LINKS_LOOP(NODE,ITER)		\
							\
  for ((ITER) = ucl_graph_input_link(NODE);		\
       NULL  != (ITER);					\
       (ITER) = ucl_graph_next_input_link(ITER))

#define UCL_GRAPH_OUTPUT_LINKS_LOOP(NODE,ITER)		\
							\
  for ((ITER) = ucl_graph_output_link(NODE);		\
       NULL  != (ITER);					\
       (ITER) = ucl_graph_next_output_link(ITER))

/* ------------------------------------------------------------ */

#define UCL_GRAPH_FIRST_INPUT_LINK_LOOP(NODE,LINK)	\
							\
  for ((LINK) = ucl_graph_input_link((NODE));		\
       NULL != (LINK);					\
       (LINK) = ucl_graph_input_link((NODE)))

#define UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(NODE,LINK)	\
							\
  for ((LINK) = ucl_graph_output_link((NODE));		\
       NULL != (LINK);					\
       (LINK) = ucl_graph_output_link((NODE)))

/* ------------------------------------------------------------ */

extern ucl_graph_link_t * ucl_graph_last_output_link (ucl_graph_link_t * link_p);
extern ucl_graph_link_t * ucl_graph_last_input_link (ucl_graph_link_t * link_p);
extern ucl_graph_link_t * ucl_graph_first_output_link (ucl_graph_link_t * link_p);
extern ucl_graph_link_t * ucl_graph_first_input_link (ucl_graph_link_t * link_p);
extern void ucl_graph_link (ucl_graph_node_t * s, ucl_graph_link_t * l, ucl_graph_node_t * d);
extern ucl_bool_t ucl_graph_nodes_are_linked (ucl_graph_node_t * src_p, ucl_graph_node_t * dst_p);
extern ucl_bool_t ucl_graph_nodes_are_connected (ucl_graph_node_t * a, ucl_graph_node_t * b);
extern void ucl_graph_set_next_node (ucl_graph_node_t * node_p, const ucl_graph_node_t * next_p);
extern void ucl_graph_unlink (ucl_graph_link_t *link_p);
extern void ucl_graph_erase_node_free_links (ucl_graph_node_t * node_p, void (* destructor)(void *));
extern ucl_link_t * ucl_graph_erase_node_return_links (ucl_graph_node_t * node_p);
extern ucl_graph_node_t * ucl_graph_remove_next_node (ucl_graph_node_t * node_p);
extern void ucl_graph_insert_next_node (ucl_graph_node_t * node_p, ucl_graph_node_t * next_p);
extern void ucl_graph_merge_upon_input_link (ucl_graph_link_t * in, ucl_graph_link_t * out);
extern void ucl_graph_merge_upon_output_link (ucl_graph_link_t * in, ucl_graph_link_t * out);


/** ------------------------------------------------------------
 ** Graph algorithms type definitions.
 ** ----------------------------------------------------------*/

#ifndef UCL_GRAPH_DFS_INITIAL_VECTOR_SIZE
#  define UCL_GRAPH_DFS_INITIAL_VECTOR_SIZE	64
#endif

#ifndef UCL_GRAPH_DFS_STEP_UP
#  define UCL_GRAPH_DFS_STEP_UP			64
#endif

#ifndef UCL_GRAPH_DFS_STEP_DOWN
#  define UCL_GRAPH_DFS_STEP_DOWN		64
#endif

/* ------------------------------------------------------------ */

typedef struct ucl_graph_dfs_t {
  ucl_vector_tag_t *	visited_nodes;
  size_t		counter;
  ucl_graph_node_t *	max_node;
  ucl_graph_node_t *	min_node;
} ucl_graph_dfs_t;

typedef struct ucl_graph_dfs_item_t {
  ucl_graph_node_t *	node_p;
  size_t		in_counter;
  size_t		out_counter;
} ucl_graph_dfs_item_t;

#define ucl_graph_dfs_items(SEARCH_HANDLE_P)	(&((SEARCH_HANDLE_P)->visited_nodes[0]))

extern void ucl_graph_initialise_dfs_handle (ucl_graph_dfs_t * search_handle, ucl_vector_t visited_nodes);
extern void ucl_graph_finalise_dfs_handle (ucl_graph_dfs_t * search_handle);
extern void ucl_graph_directed_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);
extern void ucl_graph_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);


/** ------------------------------------------------------------
 ** Binary tree functions: setters.
 ** ----------------------------------------------------------*/

static __inline__
__attribute__((__always_inline__,__nonnull__(1)))
void
ucl_btree_set_bro (void * self, void * bro)
{
  ((ucl_node_t)self)->bro = bro;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_btree_set_son (void * self, void * son)
{
  ((ucl_node_t)self)->son = son;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_btree_set_dad (void * self, void * dad)
{
  ((ucl_node_t)self)->dad = dad;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_btree_set_dadbro (void * dad, void * bro)
{
  ucl_btree_set_dad(bro, dad);
  ucl_btree_set_bro(dad, bro);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_btree_set_dadson (void * dad, void * son)
{
  ucl_btree_set_dad(son, dad);
  ucl_btree_set_son(dad, son);
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_btree_set_dadsonbro (void * dad, void * son, void * bro)
{
  ucl_btree_set_dadbro(dad, bro);
  ucl_btree_set_dadson(dad, son);
}


/** ------------------------------------------------------------
 ** Binary tree functions: getters.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_ref_bro (void * self)
{
  return ((ucl_node_t)self)->bro;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_ref_son (void * self)
{
  return ((ucl_node_t)self)->son;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_ref_dad (void * self)
{
  return ((ucl_node_t)self)->dad;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_data (void * self)
{
  return ((ucl_embedded_node_t)self)->data;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_bool_t
ucl_btree_is_leaf (void * _self)
{
  ucl_node_t	self = _self;

  return ((NULL == self->bro) && (NULL == self->son));
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_bool_t
ucl_btree_is_root (void * _self)
{
  ucl_node_t	self = _self;

  return (NULL == self->dad);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Binary tree functions: detaching nodes.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__nonnull__))void *
ucl_btree_detach_son (void * self)
{
  ucl_node_t	son = ucl_btree_ref_son(self);

  ucl_btree_set_son(self, NULL);
  ucl_btree_set_dad(son,  NULL);
  return son;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))void *
ucl_btree_detach_bro (void * self)
{
  ucl_node_t	bro = ucl_btree_ref_bro(self);

  ucl_btree_set_bro(self, NULL);
  ucl_btree_set_dad(bro,  NULL);
  return bro;
}
static __inline__ __attribute__((__always_inline__,__nonnull__))void *
ucl_btree_detach_dad (void * self)
{
  ucl_node_t	dad = ucl_btree_ref_dad(self);

  ucl_btree_set_dad(self, NULL);
  ucl_btree_set_dad(dad,  NULL);
  return dad;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_btree_clean (void * self)
{
  ucl_struct_clean(self,ucl_node_tag_t);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Binary tree functions: iteration.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_inorder (void * root_node)
{
  return ucl_btree_find_leftmost(root_node);
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_inorder_backward (void * root_node)
{
  return ucl_btree_find_rightmost(root_node);
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_preorder (void * root_node)
{
  return root_node;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_preorder_backward (void * root_node)
{
  return root_node;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_postorder (void * root_node)
{
  return ucl_btree_find_deepest_son(root_node);
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_postorder_backward (void * root_node)
{
  return ucl_btree_find_deepest_bro(root_node);
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_levelorder (void * root_node)
{
  return root_node;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_btree_first_levelorder_backward (void * root_node)
{
  return root_node;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Tree functions: setters.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_tree_set_next (void * self, void * next)
{
  ucl_btree_set_bro(self,next);
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_tree_set_son (void * self, void * son)
{
  ucl_btree_set_son(self,son);
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_tree_set_prev (void * self, void * prev)
{
  ucl_btree_set_dad(self,prev);
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_tree_set_nextprev(void * prev, void * next)
{
  ucl_tree_set_next(prev,next);
  ucl_tree_set_prev(next,prev);
}

/* ------------------------------------------------------------ */

extern ucl_bool_t ucl_tree_is_dad (ucl_node_t nod_p, ucl_node_t cld_p)
  __attribute__((__nonnull__));
extern ucl_bool_t ucl_tree_is_bro (ucl_node_t nod_p, ucl_node_t bro)
  __attribute__((__nonnull__,__pure__));
extern ucl_bool_t ucl_tree_has_dad (ucl_node_t nod_p)
  __attribute__((__nonnull__,__pure__));
extern ucl_bool_t ucl_tree_has_prev (ucl_node_t nod_p)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_tree_get_dad (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_tree_get_prev (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_tree_get_first (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
extern void * ucl_tree_get_last (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
extern void ucl_tree_insert_dad (ucl_node_t nod_p, ucl_node_t dad);
extern void ucl_tree_insert_son (ucl_node_t  nod_p, ucl_node_t  son);
extern void ucl_tree_insert_next (ucl_node_t  nod_p, ucl_node_t  nxt_p);
extern void ucl_tree_insert_prev (ucl_node_t  nod_p, ucl_node_t  prv_p);
extern ucl_node_t  ucl_tree_extract_dad (ucl_node_t nod_p);
extern ucl_node_t  ucl_tree_extract_son (ucl_node_t nod_p);
extern ucl_node_t  ucl_tree_extract_prev (ucl_node_t nod_p);
extern ucl_node_t  ucl_tree_extract_next (ucl_node_t nod_p);
extern void ucl_tree_iterator_inorder (ucl_node_t  nod_p, ucl_iterator_t iterator);
extern void ucl_tree_iterator_preorder (ucl_node_t nod_p, ucl_iterator_t iterator);
extern void ucl_tree_iterator_postorder (ucl_node_t nod_p, ucl_iterator_t iterator);


/** ------------------------------------------------------------
 ** List container types definitions.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_node_t
ucl_list_car (ucl_node_t node)
{
  return node->son;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_node_t
ucl_list_cdr (ucl_node_t node)
{
  return node->bro;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_node_t
ucl_list_first (ucl_node_t node)
{
  return ucl_tree_get_first(node);
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_node_t
ucl_list_last (ucl_node_t node)
{
  return ucl_tree_get_last(node);
}

/* ------------------------------------------------------------ */

extern size_t ucl_list_length (ucl_node_t node);
extern ucl_node_t ucl_list_caar (ucl_node_t node);
extern ucl_node_t ucl_list_cadr (ucl_node_t node);
extern ucl_node_t ucl_list_cdar (ucl_node_t node);
extern ucl_node_t ucl_list_cddr (ucl_node_t node);
extern ucl_node_t ucl_list_caaar (ucl_node_t node);
extern ucl_node_t ucl_list_caadr (ucl_node_t node);
extern ucl_node_t ucl_list_cadar (ucl_node_t node);
extern ucl_node_t ucl_list_caddr (ucl_node_t node);
extern ucl_node_t ucl_list_cdaar (ucl_node_t node);
extern ucl_node_t ucl_list_cdadr (ucl_node_t node);
extern ucl_node_t ucl_list_cddar (ucl_node_t node);
extern ucl_node_t ucl_list_cdddr (ucl_node_t node);
extern ucl_node_t ucl_list_remove (ucl_node_t node);
extern ucl_node_t ucl_list_popfront (ucl_node_t node);
extern ucl_node_t ucl_list_popback (ucl_node_t node);


/** ------------------------------------------------------------
 ** Heap functions.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) size_t
ucl_heap_size (const ucl_heap_t this)
{
  return this->size;
}

/* ------------------------------------------------------------ */

extern void ucl_heap_insert (ucl_heap_t this, void * _node_p);
extern ucl_node_t  ucl_heap_extract (ucl_heap_t this);
extern void ucl_heap_merge (ucl_heap_t this, ucl_heap_t other);


/** ------------------------------------------------------------
 ** Map functions.
 ** ----------------------------------------------------------*/

extern void ucl_map_constructor (ucl_map_t this, unsigned int flags, ucl_value_comparison_t keycmp);
extern void ucl_map_destructor (ucl_map_t this);
extern void ucl_map_insert (ucl_map_t this, ucl_map_link_t *link_p)
  __attribute__((__nonnull__,__pure__));
extern ucl_map_link_t * ucl_map_remove (ucl_map_t this, ucl_map_link_t *cur_p);
extern ucl_map_link_t * ucl_map_find (const ucl_map_t this, const ucl_value_t key);
extern ucl_map_link_t * ucl_map_first (const ucl_map_t this);
extern ucl_map_link_t * ucl_map_last (const ucl_map_t this);
extern ucl_map_link_t * ucl_map_next (ucl_map_link_t *link_p);
extern ucl_map_link_t * ucl_map_prev (ucl_map_link_t *link_p);
extern ucl_map_link_t * ucl_map_find_or_next (const ucl_map_t this, const ucl_value_t key);
extern ucl_map_link_t * ucl_map_find_or_prev (const ucl_map_t this, const ucl_value_t key);
extern size_t ucl_map_count (const ucl_map_t this, const ucl_value_t key);
extern void ucl_map_iterator_inorder (const ucl_map_t this, ucl_iterator_t iterator);
extern void ucl_map_iterator_inorder_backward (const ucl_map_t this, ucl_iterator_t iterator);
extern void ucl_map_iterator_preorder (const ucl_map_t this, ucl_iterator_t iterator);
extern void ucl_map_iterator_postorder (const ucl_map_t this, ucl_iterator_t iterator);
extern void ucl_map_iterator_levelorder (const ucl_map_t this, ucl_iterator_t iterator);
extern void ucl_map_lower_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
extern void ucl_map_upper_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
extern void ucl_map_iterator_union (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
extern void ucl_map_iterator_intersection (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
extern void ucl_map_iterator_complintersect (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t 	iterator);
extern void ucl_map_iterator_subtraction (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);

static __inline__ __attribute__((__always_inline__,__nonnull__)) void
ucl_map_extract (ucl_map_t this, ucl_map_link_t * _link)
{
  ucl_map_remove(this, _link);
}


/** ------------------------------------------------------------
 ** Vector functions.
 ** ----------------------------------------------------------*/

extern void ucl_vector_initialise_config (ucl_vector_config_t config,
					  size_t slot_dimension,
					  size_t number_of_slots);
extern void ucl_vector_initialise_config_buffer (ucl_vector_config_t config);
extern void ucl_vector_initialise_config_hash   (ucl_vector_config_t config);
extern void ucl_vector_initialise_config_dfs    (ucl_vector_config_t config);
extern void ucl_vector_alloc (ucl_vector_t self, ucl_vector_config_t config);
extern void ucl_vector_free (ucl_vector_t self);

extern void ucl_vector_swallow_block (ucl_vector_t self, ucl_block_t block);

extern size_t ucl_vector_number_of_step_up_slots (const ucl_vector_t self);
extern size_t ucl_vector_number_of_step_down_slots (const ucl_vector_t self);
extern size_t ucl_vector_number_of_padding_slots (const ucl_vector_t self);
extern ucl_bool_t ucl_vector_running (const ucl_vector_t self);
extern void ucl_vector_update_number_of_step_up_slots (ucl_vector_t self, size_t step_up);
extern void ucl_vector_update_number_of_step_down_slots (ucl_vector_t self, size_t step_down);
extern void ucl_vector_update_number_of_padding_slots (ucl_vector_t self, size_t padding);
extern void ucl_vector_mark_all_slots_as_used (ucl_vector_t self);
extern void ucl_vector_mark_allocated_range_as_used (ucl_vector_t self, ucl_range_t range);
extern void ucl_vector_reset (ucl_vector_t self);
extern void ucl_vector_clean (ucl_vector_t self);
extern void * ucl_vector_index_to_slot (const ucl_vector_t self, ucl_vector_index_t idx);
extern void * ucl_vector_index_to_new_slot (const ucl_vector_t self, ucl_vector_index_t idx);
extern ucl_vector_index_t ucl_vector_last_index (const ucl_vector_t self);
extern ucl_vector_index_t ucl_vector_slot_to_index (const ucl_vector_t self, const void *_pointer_to_slot_p);
extern ucl_bool_t ucl_vector_pointer_is_valid_slot (const ucl_vector_t self, const void *_pointer_to_slot_p);
extern ucl_bool_t ucl_vector_index_is_valid (const ucl_vector_t self, ucl_vector_index_t idx);
extern ucl_bool_t ucl_vector_index_is_valid_new (const ucl_vector_t self, ucl_vector_index_t idx);
extern ucl_bool_t ucl_vector_range_is_valid (const ucl_vector_t self, ucl_range_t range);
extern ucl_range_t ucl_vector_range (const ucl_vector_t self);
extern ucl_range_t ucl_vector_range_from_position_to_end (const ucl_vector_t self, ucl_vector_index_t position);
extern ucl_range_t ucl_vector_range_from_end_to_position (const ucl_vector_t self, ucl_vector_index_t position);
extern ucl_range_t ucl_vector_range_from_end_with_span (const ucl_vector_t self, size_t span);
extern void * ucl_vector_insert (ucl_vector_t self, void *_pointer_to_slot_p);
extern void * ucl_vector_insert_sort (ucl_vector_t self, void *data_p);
extern void ucl_vector_erase (ucl_vector_t self, void *_pointer_to_slot_p);
extern size_t ucl_vector_slot_dimension (const ucl_vector_t self);
extern size_t ucl_vector_size (const ucl_vector_t self);
extern void ucl_vector_set_compar (ucl_vector_t self, ucl_comparison_t compar);
extern ucl_block_t ucl_vector_get_memory_block (const ucl_vector_t self);
extern ucl_block_t ucl_vector_get_data_block (const ucl_vector_t self);
extern ucl_block_t ucl_vector_get_free_block_at_end (ucl_vector_t self, size_t requested_slots);
extern ucl_block_t ucl_vector_get_free_block_at_beginning (ucl_vector_t self, size_t requested_slots);
extern void ucl_vector_mark_as_used (ucl_vector_t self, ucl_block_t used_block);
extern void ucl_vector_quick_sort (ucl_vector_t self);
extern ucl_bool_t ucl_vector_sorted (ucl_vector_t self);
extern ucl_block_t ucl_vector_block_from_range (const ucl_vector_t self, const ucl_range_t range);
extern ucl_range_t ucl_vector_range_from_block (const ucl_vector_t self, const ucl_block_t block);
extern void ucl_vector_iterator_forward (const ucl_vector_t self, ucl_iterator_t iterator);
extern void ucl_vector_iterator_backward (const ucl_vector_t self, ucl_iterator_t iterator);
extern void ucl_vector_iterator_range_forward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
extern void ucl_vector_iterator_range_backward (const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
extern ucl_bool_t ucl_vector_will_enlarge (ucl_vector_t self);
extern size_t ucl_vector_enlarged_size (ucl_vector_t self);
extern void ucl_vector_enlarge (ucl_vector_t self);
extern void ucl_vector_enlarge_for_slots (ucl_vector_t self, size_t required_free_slots);
extern void ucl_vector_enlarge_for_range (ucl_vector_t self, ucl_range_t range);
extern ucl_bool_t ucl_vector_will_restrict (ucl_vector_t self);
extern size_t ucl_vector_restricted_size (ucl_vector_t self);
extern void ucl_vector_restrict (ucl_vector_t self);
extern void ucl_vector_set_memory_to_zero (ucl_vector_t self);
extern size_t ucl_vector_number_of_free_slots (const ucl_vector_t self);
extern void * ucl_vector_find (const ucl_vector_t self, const void * data_p);
extern void * ucl_vector_sort_find (const ucl_vector_t self, const void * data_p);
extern void * ucl_vector_binary_search (const ucl_vector_t self, const void * data_p);
extern ucl_data_t ucl_vector_push_back (ucl_vector_t self);
extern ucl_data_t ucl_vector_push_front (ucl_vector_t self);
extern void ucl_vector_pop_back (ucl_vector_t self);
extern void ucl_vector_pop_front (ucl_vector_t self);
extern void ucl_vector_append_block (ucl_vector_t self, const ucl_block_t block);
extern void ucl_vector_append (ucl_vector_t target, const ucl_vector_t source);
extern void ucl_vector_append_range (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range);
extern void ucl_vector_append_more (ucl_vector_t target, const ucl_vector_t source, ...);
extern void ucl_vector_append_more_from_array (ucl_vector_t target, const ucl_vector_array_t * vectors);
extern void ucl_vector_insert_block (ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block);
extern void ucl_vector_insert_range (ucl_vector_t V, ucl_vector_index_t offset, const ucl_vector_t A, ucl_range_t range);
extern void ucl_vector_insert_vector (ucl_vector_t T, ucl_vector_index_t offset, const ucl_vector_t S);
extern void ucl_vector_erase_range (ucl_vector_t self, ucl_range_t index_range);
extern void ucl_vector_copy_range (ucl_vector_t target, ucl_vector_index_t position, ucl_vector_t source, ucl_range_t source_range);
extern void ucl_vector_set_block (ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source);
extern void ucl_vector_get_block (ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source);
extern int ucl_vector_compare_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb);
extern int ucl_vector_compare (const ucl_vector_t a, const ucl_vector_t b);
extern ucl_bool_t ucl_vector_equal (const ucl_vector_t a, const ucl_vector_t b);
extern void ucl_vector_for_each (ucl_callback_t callback, const ucl_vector_t self);
extern void ucl_vector_for_each_in_range (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
extern void ucl_vector_for_each_multiple_from_array (ucl_callback_t callback, const ucl_vector_array_t * vectors);
extern void ucl_vector_for_each_multiple (ucl_callback_t callback, const ucl_vector_t first, ...);
extern void ucl_vector_map (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self);
extern void ucl_vector_map_range (ucl_vector_t result, ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
extern void ucl_vector_map_multiple_from_array (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_array_t * vectors);
extern void ucl_vector_map_multiple (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...);

/* ------------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_vector_front (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, 0);
}
static __inline__ __attribute__((__always_inline__,__nonnull__)) void *
ucl_vector_back (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, ucl_vector_last_index(vector));
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_vector_increment_slot (const ucl_vector_t self, void * slot)
{
  uint8_t *	p = slot;

  return p + self->slot_dimension;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_vector_decrement_slot (const ucl_vector_t self, void * slot)
{
  uint8_t *	p = slot;

  return p - self->slot_dimension;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) int
ucl_vector_equal_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  return (0 == ucl_vector_compare_range(a, ra, b, rb));
}


/** --------------------------------------------------------------------
 ** Hash table functions.
 ** ----------------------------------------------------------------- */

static __inline__
__attribute__((__always_inline__,__pure__,__nonnull__))
size_t
ucl_hash_number_of_buckets (const ucl_hash_table_t self)
{
  return ucl_vector_size(self->buckets);
}
static __inline__
__attribute__((__always_inline__,__pure__,__nonnull__))
size_t
ucl_hash_size (const ucl_hash_table_t this)
{
  return this->size;
}

/* ------------------------------------------------------------ */

extern void ucl_hash_initialise (ucl_hash_table_t this, ucl_vector_t buckets,
				 ucl_value_comparison_t compar, ucl_hash_t hash);
extern void ucl_hash_insert (ucl_hash_table_t this, ucl_hash_entry_t *entry_p);
extern void ucl_hash_extract (ucl_hash_table_t this, ucl_hash_entry_t *entry_p);

extern ucl_hash_entry_t * ucl_hash_find (const ucl_hash_table_t this, const ucl_value_t key);
extern ucl_hash_entry_t * ucl_hash_first (const ucl_hash_table_t this);

extern void ucl_hash_enlarge (ucl_hash_table_t this);
extern void ucl_hash_restrict (ucl_hash_table_t this);

extern void ucl_hash_iterator (const ucl_hash_table_t this, ucl_iterator_t iterator);

extern size_t ucl_hash_number_of_used_buckets (const ucl_hash_table_t this);
extern size_t ucl_hash_bucket_chain_length (const ucl_hash_table_t this, ucl_vector_index_t position);
extern double ucl_hash_average_search_distance (const ucl_hash_table_t this);


/** --------------------------------------------------------------------
 ** Circular list.
 ** ----------------------------------------------------------------- */

typedef struct ucl_circular_link_t {
  struct ucl_circular_link_t *	next;
  struct ucl_circular_link_t *	prev;
  ucl_value_t			val;
} ucl_circular_link_t;

typedef struct ucl_circular_tag_t {
  ucl_circular_link_t *		current_link;
  size_t			size;
  ucl_value_comparison_t	compar;
} ucl_circular_tag_t;

typedef ucl_circular_tag_t ucl_circular_t[1];

/* ------------------------------------------------------------ */

extern void ucl_circular_insert (ucl_circular_t this, ucl_circular_link_t *new);
extern ucl_circular_link_t * ucl_circular_extract (ucl_circular_t this);
extern void ucl_circular_forward (ucl_circular_t this, int times);
extern ucl_circular_link_t * ucl_circular_find (ucl_circular_t this, ucl_value_t val);

/* ------------------------------------------------------------ */

static __inline__ void
ucl_circular_constructor (ucl_circular_t this)
{
  ucl_struct_clean(this, ucl_circular_t);
}
static __inline__ void
ucl_circular_destructor (ucl_circular_t this)
{
  ucl_struct_clean(this, ucl_circular_t);
}
static __inline__ size_t
ucl_circular_size (ucl_circular_t this)
{
  return this->size;
}
static __inline__ ucl_circular_link_t *
ucl_circular_current (ucl_circular_t this)
{
  return this->current_link;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_value_t
ucl_circular_data (ucl_circular_link_t * link_p)
{
  return link_p->val;
}
static __inline__ ucl_value_t
ucl_circular_getval (ucl_circular_link_t * link_p)
{
  return link_p->val;
}
static __inline__ void
ucl_circular_setval (ucl_circular_link_t * link_p, ucl_value_t newval)
{
  link_p->val = newval;
}
static __inline__ void
ucl_circular_set_compar (ucl_circular_t this, ucl_value_comparison_t compar)
{
  this->compar = compar;
}


/** ------------------------------------------------------------
 ** End.
 ** ----------------------------------------------------------*/

UCL_END_C_DECL

#endif /* __UCL_H */

/* end of file */