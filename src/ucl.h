/*
  Part of: Useless Containers Library
  Contents: main header file
  Date: late 2001

  Abstract

	This file must be included in every module that makes use of the
	UCL containers.

  Copyright (c)  2001-2010, 2019  Marco Maggi <marco.maggi-ipsu@poste.it>

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

#ifndef UCL_H
#define UCL_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The macro UCL_UNUSED indicates that a  function, function argument or variable may
   potentially be unused. Usage examples:

   static int unused_function (char arg) UCL_UNUSED;
   int foo (char unused_argument UCL_UNUSED);
   int unused_variable UCL_UNUSED;
*/
#ifdef __GNUC__
#  define UCL_UNUSED		__attribute__((__unused__))
#else
#  define UCL_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ucl_decl		__attribute__((__dllexport__)) extern
#    else
#      define ucl_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ucl_decl		__attribute__((__dllimport__)) extern
#    else
#      define ucl_decl		__declspec(dllimport) extern
#    endif
#  endif
#  define ucl_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ucl_decl		__attribute__((__visibility__("default"))) extern
#    define ucl_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define ucl_decl		extern
#    define ucl_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** -----------------------------------------------------------------*/

#include <ccexceptions.h>
#include <ccmemory.h>
#include <ccstructs.h>

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define UCL_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ucl_decl char const *	ucl_version_string		(void);
ucl_decl int		ucl_version_interface_current	(void);
ucl_decl int		ucl_version_interface_revision	(void);
ucl_decl int		ucl_version_interface_age	(void);


/** --------------------------------------------------------------------
 ** Common type definitions.
 ** -----------------------------------------------------------------*/

typedef _Bool		ucl_bool_t;
typedef size_t		ucl_index_t;
typedef void *		ucl_pointer_t;
typedef void *		ucl_pointer_map_fun_t (void *);

typedef enum {
  UCL_SON_DEEPER=-1, UCL_EQUAL_DEPTH=0, UCL_BRO_DEEPER=+1
} ucl_avl_status_t;

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

  intptr_t	t_intptr;
  uintptr_t	t_uintptr;
  size_t	t_size;
  ssize_t	t_ssize;

  void *	ptr;
  void *	pointer;
  uint8_t *	bytes;
  char *	chars;

  /* The following are used by the UCL. */
  ucl_pointer_map_fun_t *	pointer_map_function;
  ucl_avl_status_t		avl_status;
  ucl_bool_t			to_be_processed_during_rehashing;
  struct ucl_value_flags_t {
    unsigned int marked:     1;
  } flags;
} ucl_value_t;

ucl_decl const ucl_value_t ucl_value_null;

typedef struct ucl_array_of_pointers_t {
  ucl_value_t	data;
  size_t	number_of_slots;
  void **	slots;
} ucl_array_of_pointers_t;

typedef int ucl_comparison_fun_t (ucl_value_t data, ucl_value_t a, ucl_value_t b);

typedef struct ucl_comparison_t {
  ucl_value_t			data;
  ucl_comparison_fun_t *	func;
} ucl_comparison_t;

typedef ucl_index_t ucl_hash_fun_t (ucl_value_t data, ucl_value_t key);

typedef struct ucl_hash_t {
  ucl_value_t		data;
  ucl_hash_fun_t *	func;
} ucl_hash_t;

typedef struct ucl_link_t {
  struct ucl_link_t *	next_p;
} ucl_link_t;


/** --------------------------------------------------------------------
 ** Memory handling: allocation.
 ** -----------------------------------------------------------------*/

typedef void ucl_memory_alloc_fun_t (void * dummy, void * pp, size_t dim);

typedef struct ucl_memory_allocator_t {
  void *			data;
  ucl_memory_alloc_fun_t *	alloc;
} ucl_memory_allocator_t;

ucl_decl ucl_memory_alloc_fun_t ucl_memory_alloc;
ucl_decl const ucl_memory_allocator_t ucl_memory_allocator;

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
 ** Memory handling: blocks.
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
 ** Memory handling: ASCII-coded, zero-terminated strings.
 ** ----------------------------------------------------------*/

typedef struct ucl_ascii_t {
  size_t	len;
  char *	ptr;
} ucl_ascii_t;

typedef struct ucl_ascii_list_t {
  size_t	len;
  char **	ptr;
} ucl_ascii_list_t;

ucl_decl const ucl_ascii_t ucl_ascii_empty;

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
ucl_bool_t
ucl_ascii_is_terminated (ucl_ascii_t ascii)
{
  return ('\0' == ascii.ptr[ascii.len]);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_ascii_clean_memory (ucl_ascii_t ascii)
{
  memset(ascii.ptr,'\0',ascii.len);
  ascii.ptr[ascii.len] = '\0';
}
static __inline__ __attribute__((__always_inline__,__nonnull__))
void
ucl_ascii_terminate (ucl_ascii_t ascii)
{
  ascii.ptr[ascii.len] = '\0';
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
 ** Memory handling: structures.
 ** ----------------------------------------------------------*/

#define ucl_struct_clean(B,TYPE_T)	memset((B),'\0',sizeof(TYPE_T))
#define ucl_struct_reset(B,TYPE_T)	ucl_struct_clean((B),(TYPE_T))

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_p_struct_alloc (ucl_memory_allocator_t * allocator, void ** buffer_pp, size_t len)
{
  void *	p = NULL;
  allocator->alloc(allocator->data, &p, len);
  *buffer_pp = p;
}
#define ucl_struct_alloc(ALLOCATOR,P,TYPE_T)	ucl_p_struct_alloc(ALLOCATOR,&(P),sizeof(TYPE_T))


/** ------------------------------------------------------------
 ** Ranges.
 ** ----------------------------------------------------------*/

/* Ranges are inclusive.   Range stuff is written in  macros, not inline
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
 ** Callback.
 ** ----------------------------------------------------------*/

typedef ucl_value_t ucl_callback_fun_t	(ucl_value_t callback_data, va_list ap);

typedef struct ucl_callback_t {
  ucl_callback_fun_t *	func;
  ucl_value_t		data;
} ucl_callback_t;

ucl_decl const ucl_callback_t	ucl_callback_null;

typedef ucl_value_t ucl_callback_apply_fun_t (ucl_callback_t callback, ...);

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_bool_t
ucl_callback_is_present (ucl_callback_t callback)
{
  return (NULL != callback.func);
}
static __inline__ ucl_value_t
ucl_callback_apply (ucl_callback_t callback, ...)
{
  ucl_value_t	retval = ucl_value_null;
  if (ucl_callback_is_present(callback)) {
    va_list	ap;
    va_start(ap,callback);
    retval = callback.func(callback.data, ap);
    /* The following does nothing with the GNU C Library; it is here for
       compatibility.  If  the callback function  longjumps: nothing bad
       happend with the GNU C Library. */
    va_end(ap);
  }
  return retval;
}
static __inline__ ucl_value_t
ucl_callback_eval_thunk (ucl_callback_t callback)
{
  ucl_value_t	retval = ucl_value_null;
  if (ucl_callback_is_present(callback))
    retval = callback.func(callback.data,NULL);
  return retval;
}

ucl_decl void ucl_callback_set_application_function (ucl_callback_apply_fun_t * f);


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

typedef ucl_iterator_tag_t	ucl_iterator_t[1];

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) ucl_bool_t
ucl_iterator_more (ucl_iterator_t iterator)
{
  return (NULL != iterator->iterator);
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
static __inline__ void
ucl_iterator_for_each (ucl_iterator_t iterator, ucl_callback_t F)
{
  for (; ucl_iterator_more(iterator); ucl_iterator_next(iterator))
    ucl_callback_apply(F,ucl_iterator_ptr(iterator));
}
static __inline__ void
ucl_iterator_map (ucl_iterator_t output, ucl_iterator_t input, ucl_callback_t F)
{
  for (; ucl_iterator_more(output) && ucl_iterator_more(input);
       ucl_iterator_next(output), ucl_iterator_next(input))
    ucl_callback_apply(F,ucl_iterator_ptr(output),ucl_iterator_ptr(input));
}


/** ------------------------------------------------------------
 ** Container configuration symbols.
 ** ----------------------------------------------------------*/

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

#define UCL_NODE_SIZE		sizeof(ucl_node_tag_t)

/* BEGIN: RESERVED FOR INTERNAL USAGE */
typedef struct ucl_embedded_node_tag_t {
  ucl_node_tag_t	node;
  uint8_t		data[];
} ucl_embedded_node_tag_t;
typedef ucl_embedded_node_tag_t *	ucl_embedded_node_t;
/* END: RESERVED FOR INTERNAL USAGE */

typedef ucl_value_t ucl_node_getkey_fun_t (ucl_value_t context, void * node);

typedef struct ucl_node_getkey_t {
  ucl_value_t			data;
  ucl_node_getkey_fun_t *	func;
} ucl_node_getkey_t;

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

typedef struct ucl_vector_array_t {
  size_t		number_of_vectors;
  ucl_vector_tag_t **	vectors;
} ucl_vector_array_t;


/** ------------------------------------------------------------
 ** Binary tree prototypes.
 ** ----------------------------------------------------------*/

ucl_decl void * ucl_btree_find_value (void * node, ucl_value_t value, ucl_comparison_t compar)
  __attribute__((__nonnull__,__pure__));

/* ------------------------------------------------------------ */

ucl_decl void * ucl_btree_find_leftmost (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_find_rightmost (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_find_deepest_son (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_find_deepest_bro (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_find_root (void * node)
  __attribute__((__nonnull__,__pure__));

/* ------------------------------------------------------------ */

ucl_decl void ucl_btree_swap	(void * A, void * B);
ucl_decl void ucl_btree_swap_out	(void * A, void * B);
ucl_decl void ucl_btree_swap_no_meta (void * A_, void * B_);

ucl_decl int ucl_btree_depth	(void * N);

/* ------------------------------------------------------------ */

ucl_decl void * ucl_btree_avl_rot_left		(void * old_dad);
ucl_decl void * ucl_btree_avl_rot_left_right	(void * old_dad);
ucl_decl void * ucl_btree_avl_rot_right		(void * old_dad);
ucl_decl void * ucl_btree_avl_rot_right_left	(void * old_dad);
ucl_decl int    ucl_btree_avl_depth		(void * N_);
ucl_decl int    ucl_btree_avl_factor		(void * N_);
ucl_decl ucl_bool_t ucl_btree_avl_is_balanced	(void * N_);
ucl_decl ucl_bool_t ucl_btree_avl_is_correct	(void * N_);

ucl_decl void ucl_bst_insert  (void * root, void * N, ucl_comparison_t keycmp, ucl_node_getkey_t getkey);
ucl_decl void * ucl_bst_find  (void * N, ucl_value_t K, ucl_comparison_t keycmp, ucl_node_getkey_t getkey);
ucl_decl void* ucl_bst_delete (void * root, void * N);

/* ------------------------------------------------------------ */

ucl_decl void * ucl_btree_step_inorder (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_inorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_preorder (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_preorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_postorder (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_postorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_levelorder (void * node)
  __attribute__((__nonnull__,__pure__));

ucl_decl void * ucl_btree_step_levelorder_backward (void * node)
  __attribute__((__nonnull__,__pure__));

/* ------------------------------------------------------------ */

ucl_decl void ucl_btree_iterator_inorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_inorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_preorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_preorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_postorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_postorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_levelorder (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_iterator_levelorder_backward (ucl_iterator_t iterator, void * root_node)
  __attribute__((__nonnull__));

/* ------------------------------------------------------------ */

ucl_decl void ucl_btree_subtree_iterator_inorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_subtree_iterator_inorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_subtree_iterator_preorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_subtree_iterator_preorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_subtree_iterator_postorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_subtree_iterator_postorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));

ucl_decl void ucl_btree_subtree_iterator_levelorder (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));
ucl_decl void ucl_btree_subtree_iterator_levelorder_backward (ucl_iterator_t iterator, void * node)
  __attribute__((__nonnull__));


/** ------------------------------------------------------------
 ** Graph container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_graph_node_tag_t	ucl_graph_node_tag_t;
typedef struct ucl_graph_link_tag_t	ucl_graph_link_tag_t;

typedef ucl_graph_node_tag_t *		ucl_graph_node_t;
typedef ucl_graph_link_tag_t *		ucl_graph_link_t;

typedef struct ucl_graph_link_list_base_t ucl_graph_link_list_base_t;
typedef struct ucl_graph_link_list_node_t ucl_graph_link_list_node_t;

struct ucl_graph_link_list_base_t {
  size_t		number_of_links;
  ucl_graph_link_t	first_link;
};

struct ucl_graph_link_list_node_t {
  ucl_graph_link_t	prev_link;
  ucl_graph_link_t	next_link;
};

struct ucl_graph_node_tag_t {
  ucl_graph_link_list_base_t	input;
  ucl_graph_link_list_base_t	output;
  ucl_value_t			mark;
  ucl_value_t			val;
};

struct ucl_graph_link_tag_t {
  ucl_graph_node_t		src_node;
  ucl_graph_node_t		dst_node;
  ucl_graph_link_list_node_t	input;
  ucl_graph_link_list_node_t	output;
  ucl_value_t			val;
};

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
size_t
ucl_graph_number_of_output_links (ucl_graph_node_t N)
{
  return N->output.number_of_links;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
size_t
ucl_graph_number_of_input_links (ucl_graph_node_t N)
{
  return N->input.number_of_links;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_graph_link_t
ucl_graph_input_link (const ucl_graph_node_t N)
{
  return N->input.first_link;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_graph_link_t
ucl_graph_output_link (const ucl_graph_node_t N)
{
  return N->output.first_link;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_graph_link_t
ucl_graph_prev_input_link (const ucl_graph_link_t L)
{
  return L->input.prev_link;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_graph_link_t
ucl_graph_next_input_link (const ucl_graph_link_t L)
{
  return L->input.next_link;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_graph_link_t
ucl_graph_prev_output_link (const ucl_graph_link_t L)
{
  return L->output.prev_link;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_graph_link_t
ucl_graph_next_output_link (const ucl_graph_link_t L)
{
  return L->output.next_link;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_value_t
ucl_graph_node_get_value (const ucl_graph_node_t N)
{
  return N->val;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
void
ucl_graph_node_set_value (ucl_graph_node_t N, const ucl_value_t value)
{
  N->val = value;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_value_t
ucl_graph_link_get_value (const ucl_graph_link_t L)
{
  return L->val;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
void
ucl_graph_link_set_value (ucl_graph_link_t L, const ucl_value_t value)
{
  L->val = value;
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
ucl_value_t
ucl_graph_node_get_mark (const ucl_graph_node_t N)
{
  return N->mark;
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1)))
void
ucl_graph_node_set_mark (ucl_graph_node_t N, const ucl_value_t mark)
{
  N->mark = mark;
}

/* ------------------------------------------------------------ */

#define UCL_GRAPH_INPUT_LINKS_LOOP(NODE,LINK)		\
							\
  for ((LINK) = ucl_graph_input_link(NODE); (LINK);	\
       (LINK) = ucl_graph_next_input_link(LINK))

#define UCL_GRAPH_OUTPUT_LINKS_LOOP(NODE,LINK)		\
							\
  for ((LINK) = ucl_graph_output_link(NODE); (LINK);	\
       (LINK) = ucl_graph_next_output_link(LINK))

/* ------------------------------------------------------------ */

#define UCL_GRAPH_FIRST_INPUT_LINK_LOOP(NODE,LINK)	\
							\
  for ((LINK) = ucl_graph_input_link((NODE)); (LINK);	\
       (LINK) = ucl_graph_input_link((NODE)))

#define UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(NODE,LINK)	\
							\
  for ((LINK) = ucl_graph_output_link((NODE)); (LINK);	\
       (LINK) = ucl_graph_output_link((NODE)))

/* ------------------------------------------------------------ */

ucl_decl ucl_graph_link_t ucl_graph_last_output_link  (ucl_graph_link_t  link_p);
ucl_decl ucl_graph_link_t ucl_graph_last_input_link   (ucl_graph_link_t  link_p);
ucl_decl ucl_graph_link_t ucl_graph_first_output_link (ucl_graph_link_t  link_p);
ucl_decl ucl_graph_link_t ucl_graph_first_input_link  (ucl_graph_link_t  link_p);

ucl_decl void ucl_graph_link (ucl_graph_node_t s, ucl_graph_link_t l, ucl_graph_node_t d);
ucl_decl void ucl_graph_unlink (ucl_graph_link_t link_p);
ucl_decl void ucl_graph_erase_node_destroy_links (ucl_graph_node_t N, ucl_callback_t destructor);

ucl_decl ucl_bool_t ucl_graph_nodes_are_linked        (ucl_graph_node_t src, ucl_graph_node_t dst);
ucl_decl ucl_bool_t ucl_graph_nodes_are_doubly_linked (ucl_graph_node_t src, ucl_graph_node_t dst);
ucl_decl ucl_bool_t ucl_graph_nodes_are_connected     (ucl_graph_node_t a,   ucl_graph_node_t b);

ucl_decl void ucl_graph_merge_upon_input_link  (ucl_graph_link_t in, ucl_graph_link_t out);
ucl_decl void ucl_graph_merge_upon_output_link (ucl_graph_link_t in, ucl_graph_link_t out);


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

typedef struct ucl_graph_dfs_tag_t {
  ucl_vector_tag_t *	visited_nodes;
  size_t		counter;
  ucl_graph_node_t	max_node;
  ucl_graph_node_t	min_node;
} ucl_graph_dfs_tag_t;

typedef ucl_graph_dfs_tag_t ucl_graph_dfs_t[1];

typedef struct ucl_graph_dfs_item_t {
  ucl_graph_node_t	node;
  size_t		in_counter;
  size_t		out_counter;
} ucl_graph_dfs_item_t;

ucl_decl void ucl_graph_dfs_initialise_handle (ucl_graph_dfs_t S, ucl_vector_t visited_nodes);
ucl_decl void ucl_graph_dfs_directed	    (ucl_graph_dfs_t S, ucl_graph_node_t root);
ucl_decl void ucl_graph_dfs		    (ucl_graph_dfs_t S, ucl_graph_node_t root);


/** ------------------------------------------------------------
 ** Binary tree functions: setters.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
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


/** ------------------------------------------------------------
 ** Tree functions.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_tree_set_next (void * self, void * next)
{
  ucl_btree_set_bro(self,next);
}
static __inline__ __attribute__((__always_inline__,__nonnull__(1))) void
ucl_tree_set_prev (void * self, void * prev)
{
  ucl_btree_set_dad(self,prev);
}

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_tree_set_dadson (void * dad, void * son)
{
  ucl_btree_set_dadson(dad,son);
}
static __inline__ __attribute__((__always_inline__,__nonnull__))void
ucl_tree_set_prevnext (void * prev, void * next)
{
  ucl_btree_set_dadbro(prev,next);
}

/* ------------------------------------------------------------ */

ucl_decl ucl_bool_t ucl_tree_is_dad (void * nod_p, void * cld_p)
  __attribute__((__nonnull__));
ucl_decl ucl_bool_t ucl_tree_is_bro (void * nod_p, void * bro)
  __attribute__((__nonnull__,__pure__));
ucl_decl ucl_bool_t ucl_tree_has_dad (void * nod_p)
  __attribute__((__nonnull__,__pure__));
ucl_decl ucl_bool_t ucl_tree_has_prev (void * nod_p)
  __attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_tree_ref_dad (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_tree_ref_prev (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_tree_ref_first (void * _nod_p)
  __attribute__((__nonnull__,__pure__));
ucl_decl void * ucl_tree_ref_last (void * _nod_p)
  __attribute__((__nonnull__,__pure__));

static __inline__ __attribute__((__always_inline__,__nonnull__)) void *
ucl_tree_ref_son (void * node)
{
  return ucl_btree_ref_son(node);
}
static __inline__ __attribute__((__always_inline__,__nonnull__)) void *
ucl_tree_ref_next (void * node)
{
  return ucl_btree_ref_bro(node);
}

ucl_decl void ucl_tree_insert_dad (void * nod_p, void * dad);
ucl_decl void ucl_tree_insert_son (void *  nod_p, void *  son);
ucl_decl void ucl_tree_insert_next (void *  nod_p, void *  nxt_p);
ucl_decl void ucl_tree_insert_prev (void *  nod_p, void *  prv_p);
ucl_decl void * ucl_tree_extract_dad (void * nod_p);
ucl_decl void * ucl_tree_extract_son (void * nod_p);
ucl_decl void * ucl_tree_extract_prev (void * nod_p);
ucl_decl void * ucl_tree_extract_next (void * nod_p);
ucl_decl void ucl_tree_iterator_inorder (void *  nod_p, ucl_iterator_t iterator);
ucl_decl void ucl_tree_iterator_preorder (void * nod_p, ucl_iterator_t iterator);
ucl_decl void ucl_tree_iterator_postorder (void * nod_p, ucl_iterator_t iterator);


/** ------------------------------------------------------------
 ** List container types definitions.
 ** ----------------------------------------------------------*/

static __inline__ __attribute__((__always_inline__,__pure__))
void
ucl_list_set_car (void * N_, void * M_)
{
  ucl_node_t	N = N_, M = M_;
  if (N) N->son = M;
  if (M) M->dad = N;
}
static __inline__ __attribute__((__always_inline__,__pure__))
void
ucl_list_set_cdr (void * N_, void * M_)
{
  ucl_node_t	N = N_, M = M_;
  if (N) N->bro = M;
  if (M) M->dad = N;
}

/* ------------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__))
void *
ucl_list_car (void * N_)
{
  ucl_node_t	N = N_;
  return N->son;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__))
void *
ucl_list_cdr (void * N_)
{
  ucl_node_t	N = N_;
  return N->bro;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__))
void *
ucl_list_prev (void * N_)
{
  ucl_node_t	N = N_;
  return N->dad;
}

/* ------------------------------------------------------------------ */

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__))
void *
ucl_list_first (void * N)
{
  return ucl_tree_ref_first(N);
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__))
void *
ucl_list_last (void * N)
{
  return ucl_tree_ref_last(N);
}

/* ------------------------------------------------------------------ */

ucl_decl void * ucl_list_caar (void * node);
ucl_decl void * ucl_list_cadr (void * node);
ucl_decl void * ucl_list_cdar (void * node);
ucl_decl void * ucl_list_cddr (void * node);
ucl_decl void * ucl_list_caaar (void * node);
ucl_decl void * ucl_list_caadr (void * node);
ucl_decl void * ucl_list_cadar (void * node);
ucl_decl void * ucl_list_caddr (void * node);
ucl_decl void * ucl_list_cdaar (void * node);
ucl_decl void * ucl_list_cdadr (void * node);
ucl_decl void * ucl_list_cddar (void * node);
ucl_decl void * ucl_list_cdddr (void * node);

ucl_decl void * ucl_list_remove (void * node);
ucl_decl void * ucl_list_popfront (void * node, void ** new_first_p);
ucl_decl void * ucl_list_popback (void * node);

ucl_decl void ucl_list_for_each (ucl_callback_t cb, void * N);
ucl_decl void ucl_list_map (void * P, ucl_callback_t cb, void * Q);
ucl_decl void * ucl_list_reverse (void * N);

ucl_decl size_t ucl_list_length (void * node);
ucl_decl void * ucl_list_ref (void * N_, ucl_index_t position);


/** ------------------------------------------------------------
 ** Heap functions.
 ** ----------------------------------------------------------*/

typedef struct ucl_heap_tag_t {
  size_t		size;
  ucl_node_t		root;
  ucl_node_t		next;
  ucl_bool_t		append_to_bro_of_next;
  ucl_comparison_t	keycmp;
  ucl_node_getkey_t	getkey;
} ucl_heap_tag_t;

typedef ucl_heap_tag_t	ucl_heap_t[1];

ucl_decl void ucl_heap_initialise	(ucl_heap_t H, ucl_comparison_t compar, ucl_node_getkey_t getkey);
ucl_decl void ucl_heap_insert	(ucl_heap_t H, void * node);
ucl_decl void * ucl_heap_extract	(ucl_heap_t H);
ucl_decl void ucl_heap_merge	(ucl_heap_t H1, ucl_heap_t H2);

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) size_t
ucl_heap_size (const ucl_heap_t H)
{
  return H->size;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_heap_root (const ucl_heap_t H)
{
  return H->root;
}


/** ------------------------------------------------------------
 ** Map container.
 ** ----------------------------------------------------------*/

typedef struct ucl_map_tag_t {
  size_t		size;
  unsigned int		flags;
  ucl_comparison_t	keycmp;
  ucl_node_getkey_t	getkey;
  ucl_node_t		root;
} ucl_map_tag_t;

typedef ucl_map_tag_t ucl_map_t[1];

#define UCL_ALLOW_MULTIPLE_OBJECTS	0x10

static __inline__ __attribute__((__always_inline__,__nonnull__,__pure__))
size_t
ucl_map_size (const ucl_map_t M)
{
  return M->size;
}
static __inline__ __attribute__((__always_inline__,__nonnull__,__pure__))
void *
ucl_map_root (const ucl_map_t M)
{
  return M->root;
}

ucl_decl void ucl_map_initialise (ucl_map_t M, unsigned int flags,
				ucl_comparison_t keycmp, ucl_node_getkey_t getkey);
ucl_decl ucl_bool_t ucl_map_insert (ucl_map_t M, void * L);
ucl_decl void ucl_map_delete (ucl_map_t M, void * L);
ucl_decl void * ucl_map_find (const ucl_map_t M, const ucl_value_t key);
ucl_decl ucl_bool_t ucl_map_find_node (const ucl_map_t M, void * N_);
ucl_decl size_t ucl_map_depth (const ucl_map_t M);
ucl_decl void * ucl_map_first (const ucl_map_t M);
ucl_decl void * ucl_map_last (const ucl_map_t M);
ucl_decl void * ucl_map_next (void * L);
ucl_decl void * ucl_map_prev (void * L);
ucl_decl void * ucl_map_find_or_next (const ucl_map_t M, const ucl_value_t key);
ucl_decl void * ucl_map_find_or_prev (const ucl_map_t M, const ucl_value_t key);
ucl_decl size_t ucl_map_count (const ucl_map_t M, const ucl_value_t key);
ucl_decl void ucl_map_iterator_inorder (const ucl_map_t M, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_inorder_backward (const ucl_map_t M, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_preorder (const ucl_map_t M, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_postorder (const ucl_map_t M, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_levelorder (const ucl_map_t M, ucl_iterator_t I);
ucl_decl void ucl_map_lower_bound (const ucl_map_t M, ucl_iterator_t I, const ucl_value_t key);
ucl_decl void ucl_map_upper_bound (const ucl_map_t M, ucl_iterator_t I, const ucl_value_t key);
ucl_decl void ucl_map_iterator_union (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_intersection (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_complintersect (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I);
ucl_decl void ucl_map_iterator_subtraction (ucl_iterator_t I1, ucl_iterator_t I2, ucl_iterator_t I);


/** ------------------------------------------------------------
 ** Vector functions.
 ** ----------------------------------------------------------*/

ucl_decl void ucl_vector_initialise_config (ucl_vector_config_t config,
					  size_t slot_dimension,
					  size_t number_of_slots);
ucl_decl void ucl_vector_initialise_config_buffer (ucl_vector_config_t config);
ucl_decl void ucl_vector_initialise_config_hash   (ucl_vector_config_t config);
ucl_decl void ucl_vector_initialise_config_dfs    (ucl_vector_config_t config);
ucl_decl void ucl_vector_alloc (ucl_vector_t V, ucl_vector_config_t config);
ucl_decl void ucl_vector_free (ucl_vector_t V);

ucl_decl void ucl_vector_swallow_block (ucl_vector_t V, ucl_vector_config_t config, ucl_block_t block);

ucl_decl size_t ucl_vector_number_of_step_up_slots (const ucl_vector_t V);
ucl_decl size_t ucl_vector_number_of_step_down_slots (const ucl_vector_t V);
ucl_decl size_t ucl_vector_number_of_padding_slots (const ucl_vector_t V);
ucl_decl ucl_bool_t ucl_vector_running (const ucl_vector_t V);
ucl_decl void ucl_vector_update_number_of_step_up_slots (ucl_vector_t V, size_t step_up);
ucl_decl void ucl_vector_update_number_of_step_down_slots (ucl_vector_t V, size_t step_down);
ucl_decl void ucl_vector_update_number_of_padding_slots (ucl_vector_t V, size_t padding);
ucl_decl void ucl_vector_mark_all_slots_as_used (ucl_vector_t V);
ucl_decl void ucl_vector_mark_allocated_range_as_used (ucl_vector_t V, ucl_range_t range);
ucl_decl void ucl_vector_reset (ucl_vector_t V);
ucl_decl void ucl_vector_clean (ucl_vector_t V);
ucl_decl void * ucl_vector_index_to_slot (const ucl_vector_t V, ucl_index_t idx);
ucl_decl void * ucl_vector_index_to_new_slot (const ucl_vector_t V, ucl_index_t idx);
ucl_decl ucl_index_t ucl_vector_last_index (const ucl_vector_t V);
ucl_decl ucl_index_t ucl_vector_slot_to_index (const ucl_vector_t V, const void *_pointer_to_slot_p);
ucl_decl ucl_bool_t ucl_vector_pointer_is_valid_slot (const ucl_vector_t V, const void *pointer_to_slot_p);
ucl_decl ucl_bool_t ucl_vector_index_is_valid (const ucl_vector_t V, ucl_index_t idx);
ucl_decl ucl_bool_t ucl_vector_index_is_valid_new (const ucl_vector_t V, ucl_index_t idx);
ucl_decl ucl_bool_t ucl_vector_range_is_valid (const ucl_vector_t V, ucl_range_t range);
ucl_decl ucl_range_t ucl_vector_range (const ucl_vector_t V);
ucl_decl ucl_range_t ucl_vector_range_from_position_to_end (const ucl_vector_t V, ucl_index_t position);
ucl_decl ucl_range_t ucl_vector_range_from_end_to_position (const ucl_vector_t V, ucl_index_t position);
ucl_decl ucl_range_t ucl_vector_range_from_end_with_span (const ucl_vector_t V, size_t span);
ucl_decl void * ucl_vector_insert (ucl_vector_t V, void *_pointer_to_slot_p);
ucl_decl void * ucl_vector_insert_sort (ucl_vector_t V, ucl_value_t data);
ucl_decl void ucl_vector_erase (ucl_vector_t V, void *_pointer_to_slot_p);
ucl_decl size_t ucl_vector_slot_dimension (const ucl_vector_t V);
ucl_decl size_t ucl_vector_size (const ucl_vector_t V);
ucl_decl void ucl_vector_set_compar (ucl_vector_t V, ucl_comparison_t compar);
ucl_decl ucl_block_t ucl_vector_get_memory_block (const ucl_vector_t V);
ucl_decl ucl_block_t ucl_vector_get_data_block (const ucl_vector_t V);
ucl_decl ucl_block_t ucl_vector_get_free_block_at_end (ucl_vector_t V, size_t requested_slots);
ucl_decl ucl_block_t ucl_vector_get_free_block_at_beginning (ucl_vector_t V, size_t requested_slots);
ucl_decl void ucl_vector_mark_as_used (ucl_vector_t V, ucl_block_t used_block);
ucl_decl void ucl_vector_quick_sort (ucl_vector_t V);
ucl_decl ucl_bool_t ucl_vector_sorted (ucl_vector_t V);
ucl_decl ucl_block_t ucl_vector_block_from_range (const ucl_vector_t V, const ucl_range_t range);
ucl_decl ucl_range_t ucl_vector_range_from_block (const ucl_vector_t V, const ucl_block_t block);
ucl_decl void ucl_vector_iterator_forward (const ucl_vector_t V, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_backward (const ucl_vector_t V, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_range_forward (const ucl_vector_t V, ucl_range_t range, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_range_backward (const ucl_vector_t V, ucl_range_t range, ucl_iterator_t iterator);
ucl_decl ucl_bool_t ucl_vector_will_enlarge (ucl_vector_t V);
ucl_decl size_t ucl_vector_enlarged_size (ucl_vector_t V);
ucl_decl void ucl_vector_enlarge (ucl_vector_t V);
ucl_decl void ucl_vector_enlarge_for_slots (ucl_vector_t V, size_t required_free_slots);
ucl_decl void ucl_vector_enlarge_for_range (ucl_vector_t V, ucl_range_t range);
ucl_decl ucl_bool_t ucl_vector_will_restrict (ucl_vector_t V);
ucl_decl size_t ucl_vector_restricted_size (ucl_vector_t V);
ucl_decl void ucl_vector_restrict (ucl_vector_t V);
ucl_decl void ucl_vector_set_memory_to_zero (ucl_vector_t V);
ucl_decl size_t ucl_vector_number_of_free_slots (const ucl_vector_t V);
ucl_decl void * ucl_vector_find (const ucl_vector_t V, const ucl_value_t data);
ucl_decl void * ucl_vector_sort_find (const ucl_vector_t V, const ucl_value_t data);
ucl_decl void * ucl_vector_binary_search (const ucl_vector_t V, const ucl_value_t data);
ucl_decl void * ucl_vector_push_back (ucl_vector_t V);
ucl_decl void * ucl_vector_push_front (ucl_vector_t V);
ucl_decl void ucl_vector_pop_back (ucl_vector_t V);
ucl_decl void ucl_vector_pop_front (ucl_vector_t V);
ucl_decl void ucl_vector_append_block (ucl_vector_t V, const ucl_block_t block);
ucl_decl void ucl_vector_append (ucl_vector_t target, const ucl_vector_t source);
ucl_decl void ucl_vector_append_range (ucl_vector_t target, const ucl_vector_t source, ucl_range_t range);
ucl_decl void ucl_vector_append_more (ucl_vector_t target, const ucl_vector_t source, ...);
ucl_decl void ucl_vector_append_more_from_array (ucl_vector_t target, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_insert_block (ucl_vector_t target, ucl_index_t offset, const ucl_block_t block);
ucl_decl void ucl_vector_insert_range (ucl_vector_t V, ucl_index_t offset, const ucl_vector_t A, ucl_range_t range);
ucl_decl void ucl_vector_insert_vector (ucl_vector_t T, ucl_index_t offset, const ucl_vector_t S);
ucl_decl void ucl_vector_erase_range (ucl_vector_t V, ucl_range_t index_range);
ucl_decl void ucl_vector_copy_range (ucl_vector_t target, ucl_index_t position, ucl_vector_t source, ucl_range_t source_range);
ucl_decl void ucl_vector_set_block (ucl_vector_t target, ucl_index_t position, ucl_block_t source);
ucl_decl void ucl_vector_get_block (ucl_block_t target, ucl_index_t position, ucl_vector_t source);
ucl_decl int ucl_vector_compare_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb);
ucl_decl int ucl_vector_compare (const ucl_vector_t a, const ucl_vector_t b);
ucl_decl ucl_bool_t ucl_vector_equal (const ucl_vector_t a, const ucl_vector_t b);
ucl_decl void ucl_vector_for_each (ucl_callback_t callback, const ucl_vector_t V);
ucl_decl void ucl_vector_for_each_in_range (ucl_callback_t callback, ucl_range_t range, const ucl_vector_t V);
ucl_decl void ucl_vector_for_each_multiple_from_array (ucl_callback_t callback, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_for_each_multiple (ucl_callback_t callback, const ucl_vector_t first, ...);
ucl_decl void ucl_vector_map (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t V);
ucl_decl void ucl_vector_map_range (ucl_vector_t result, ucl_callback_t callback, ucl_range_t range, const ucl_vector_t V);
ucl_decl void ucl_vector_map_multiple_from_array (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_map_multiple (ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t first, ...);

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
ucl_vector_increment_slot (const ucl_vector_t V, void * slot)
{
  uint8_t *	p = slot;

  return p + V->slot_dimension;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) void *
ucl_vector_decrement_slot (const ucl_vector_t V, void * slot)
{
  uint8_t *	p = slot;

  return p - V->slot_dimension;
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) int
ucl_vector_equal_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  return (0 == ucl_vector_compare_range(a, ra, b, rb));
}


/** --------------------------------------------------------------------
 ** Hash table functions.
 ** ----------------------------------------------------------------- */

typedef struct ucl_hash_table_tag_t {
  ucl_vector_tag_t *	buckets;
  ucl_hash_t		hash;
  ucl_comparison_t	compar;
  ucl_node_getkey_t	getkey;
  size_t		size;
  size_t		number_of_used_buckets;
} ucl_hash_table_tag_t;

typedef ucl_hash_table_tag_t ucl_hash_table_t[1];

ucl_decl void ucl_hash_initialise (ucl_hash_table_t H, ucl_vector_t buckets,
				 ucl_comparison_t compar, ucl_hash_t hash,
				 ucl_node_getkey_t getkey);

ucl_decl void ucl_hash_insert  (ucl_hash_table_t H, void * entry);
ucl_decl void ucl_hash_extract (ucl_hash_table_t H, void * entry);

ucl_decl void * ucl_hash_find  (const ucl_hash_table_t H, const ucl_value_t key);
ucl_decl void * ucl_hash_first (const ucl_hash_table_t H);

ucl_decl void ucl_hash_enlarge  (ucl_hash_table_t H);
ucl_decl void ucl_hash_restrict (ucl_hash_table_t H);

ucl_decl void ucl_hash_iterator (const ucl_hash_table_t H, ucl_iterator_t iterator);

ucl_decl size_t ucl_hash_number_of_used_buckets  (const ucl_hash_table_t H);
ucl_decl size_t ucl_hash_bucket_chain_length     (const ucl_hash_table_t H, ucl_index_t position);
ucl_decl double ucl_hash_average_search_distance (const ucl_hash_table_t H);

static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) size_t
ucl_hash_number_of_buckets (const ucl_hash_table_t H)
{
  return ucl_vector_size(H->buckets);
}
static __inline__ __attribute__((__always_inline__,__pure__,__nonnull__)) size_t
ucl_hash_size (const ucl_hash_table_t H)
{
  return H->size;
}


/** --------------------------------------------------------------------
 ** Circular list.
 ** ----------------------------------------------------------------- */

typedef struct ucl_circular_tag_t {
  ucl_node_t		current;
  size_t		size;
  ucl_comparison_t	compar;
} ucl_circular_tag_t;

typedef ucl_circular_tag_t ucl_circular_t[1];

ucl_decl void ucl_circular_insert (ucl_circular_t self, ucl_node_t new);
ucl_decl ucl_node_t  ucl_circular_extract (ucl_circular_t self);
ucl_decl void ucl_circular_forward (ucl_circular_t self, int times);
ucl_decl ucl_node_t  ucl_circular_find (ucl_circular_t self, ucl_value_t val);

static __inline__ void
ucl_circular_constructor (ucl_circular_t self)
{
  ucl_struct_clean(self, ucl_circular_t);
}
static __inline__ void
ucl_circular_destructor (ucl_circular_t self)
{
  ucl_struct_clean(self, ucl_circular_t);
}
static __inline__ size_t
ucl_circular_size (ucl_circular_t self)
{
  return self->size;
}
static __inline__ ucl_node_t
ucl_circular_current (ucl_circular_t self)
{
  return self->current;
}
static __inline__ void
ucl_circular_set_compar (ucl_circular_t self, ucl_comparison_t compar)
{
  self->compar = compar;
}
static __inline__ void
ucl_circular_backward (ucl_circular_t self, int times)
{
  ucl_circular_forward(self, -times);
}


/** --------------------------------------------------------------------
 ** Miscellaneous functions.
 ** -----------------------------------------------------------------*/

ucl_decl ucl_comparison_fun_t	ucl_compare_int_fun;
ucl_decl ucl_comparison_fun_t	ucl_compare_unsigned_int_fun;
ucl_decl ucl_comparison_fun_t	ucl_compare_string_fun;
ucl_decl ucl_comparison_fun_t	ucl_compare_int_pointer_fun;

ucl_decl const ucl_comparison_t	ucl_compare_int;
ucl_decl const ucl_comparison_t	ucl_compare_unsigned_int;
ucl_decl const ucl_comparison_t	ucl_compare_string;
ucl_decl const ucl_comparison_t	ucl_compare_int_pointer;

ucl_decl const ucl_hash_t		ucl_hash_string;
ucl_decl ucl_hash_fun_t		ucl_hash_string_fun;

ucl_decl void ucl_quicksort (void *const pbase, size_t total_elems, size_t size, ucl_comparison_t cmp);


/** ------------------------------------------------------------
 ** End.
 ** ----------------------------------------------------------*/

UCL_END_C_DECL

#endif /* UCL_H */

/* end of file */
