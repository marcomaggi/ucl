/* 
   Part of: Useless Container Library
   Contents: main header file
   Date: late 2001

   Abstract

	This file must be included in every module that makes use of the
	UCL containers.

   Copyright (c)  2001-2007, 2008  Marco Maggi
   
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

#ifndef __UCL_H
#define __UCL_H 1


/** ------------------------------------------------------------
 ** Header files.
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
 ** Definitions.
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

/*
  This is adapted from Guile 1.8, file "__scm.h".
  
  The macro  UCL_UNUSED indicates that a function,  function argument or
  variable may potentially be unused.

  Examples:

	static int unused_function (char arg) UCL_UNUSED;
	int foo (char unused_argument UCL_UNUSED);
	int unused_variable UCL_UNUSED;
*/

#ifdef __GNUC__
#define UCL_UNUSED __attribute__ ((unused))
#else
#define UCL_UNUSED
#endif

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Common type declarations.
 ** ----------------------------------------------------------*/

typedef _Bool			ucl_bool_t;
typedef size_t			ucl_index_t;
typedef ucl_index_t		ucl_vector_index_t;
typedef void *			ucl_data_t;

typedef union ucl_value_t {
  unsigned	unum;
  int		num;
  int		integer;
  void *	ptr;
  char *	str;
  uint8_t *	bytes;
} ucl_value_t;

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

static __inline__ void
ucl_p_struct_alloc (ucl_memory_allocator_t * allocator, void ** buffer_pp, size_t len)
{
  void *	p = NULL;

  allocator->alloc(allocator->data, &p, len);
  *buffer_pp = p;
}
#define ucl_struct_alloc(ALLOCATOR,P,TYPE_T)	ucl_p_struct_alloc(ALLOCATOR,&(P),sizeof(TYPE_T))

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

static __inline__ void
ucl_block_set (ucl_block_t * block, void * p, size_t len)
{
  block->ptr = p;
  block->len = len;
}
static __inline__ void
ucl_block_reset (ucl_block_t * block)
{
  block->ptr = NULL;
  block->len = 0;
}
static __inline__ ucl_bool_t
ucl_block_is_null (ucl_block_t block)
{
  return (NULL == block.ptr);
}
static __inline__ void
ucl_block_clean_memory (ucl_block_t block)
{
  memset(block.ptr, '\0', block.len);
}

/* ------------------------------------------------------------ */

static __inline__ ucl_block_t
ucl_block_alloc (ucl_memory_allocator_t allocator, size_t dim)
{
  ucl_block_t	block = UCL_EMPTY_BLOCK;

  allocator.alloc(allocator.data, &(block.ptr), dim);
  block.len = dim;
  return block;
}
static __inline__ ucl_block_t
ucl_block_realloc (ucl_memory_allocator_t allocator, ucl_block_t block, size_t new_dim)
{
  allocator.alloc(allocator.data, &(block.ptr), new_dim);
  block.len = new_dim;
  return block;
}
static __inline__ void
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

static __inline__ void
ucl_ascii_set (ucl_ascii_t * ascii, void * ptr, size_t len)
{
  ascii->ptr = ptr;
  ascii->len = len;
}
static __inline__ void
ucl_ascii_reset (ucl_ascii_t * ascii)
{
  ascii->ptr = NULL;
  ascii->len = 0;
}
static __inline__ ucl_ascii_t
ucl_ascii_const (const char * string)
{
  ucl_ascii_t	ascii;

  ascii.ptr = (void *)string;
  ascii.len = strlen(string);
  return ascii;
}
static __inline__ ucl_bool_t
ucl_ascii_is_null (ucl_ascii_t ascii)
{
  return (NULL == ascii.ptr);
}
static __inline__ void
ucl_ascii_clean_memory (ucl_ascii_t ascii)
{
  memset(ascii.ptr,'\0',ascii.len);
}
static __inline__ ucl_ascii_t
ucl_ascii_from_block (ucl_block_t block)
{
  ucl_ascii_t	ascii;

  ascii.len = block.len - 1;
  ascii.ptr = (void *)block.ptr;
  ascii.ptr[block.len] = '\0';
  return ascii;
}
static __inline__ ucl_block_t
ucl_block_from_ascii (ucl_ascii_t ascii)
{
  ucl_block_t	block;

  block.len = 1 + ascii.len;
  block.ptr = (void *)ascii.ptr;
  return block;
}

/* ------------------------------------------------------------ */

static __inline__ ucl_ascii_t
ucl_ascii_alloc (ucl_memory_allocator_t allocator, size_t dim)
{
  ucl_ascii_t	ascii = UCL_ASCII_NULL_VALUE;

  allocator.alloc(allocator.data, &(ascii.ptr), 1+dim);
  ascii.len = dim;
  ascii.ptr[dim]='\0';
  return ascii;
}
static __inline__ ucl_ascii_t
ucl_ascii_realloc (ucl_memory_allocator_t allocator, ucl_ascii_t ascii, size_t new_dim)
{
  allocator.alloc(allocator.data, &(ascii.ptr), 1+new_dim);
  ascii.len = new_dim;
  ascii.ptr[new_dim]='\0';
  return ascii;
}
static __inline__ void
ucl_ascii_free (ucl_memory_allocator_t allocator, ucl_ascii_t ascii)
{
  if (ascii.ptr)
    allocator.alloc(allocator.data, &(ascii.ptr), 0);
}

/* ------------------------------------------------------------ */

/** ------------------------------------------------------------
 ** Callback.
 ** ----------------------------------------------------------*/

typedef void ucl_callback_fun_t (ucl_value_t callback_data, va_list ap);

typedef struct ucl_callback_t {
  ucl_callback_fun_t *	func;
  ucl_value_t		data;
} ucl_callback_t;

#define UCL_CALLBACK_NULL_VALUE		{ .func = NULL, .data = { .ptr = 0 } }

typedef void ucl_callback_apply_fun_t (ucl_callback_t callback, ...);

/* ------------------------------------------------------------ */

static __inline__ __attribute__((__pure__)) ucl_bool_t
ucl_callback_is_present (ucl_callback_t callback)
{
  return (NULL != callback.func);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Generic iterator.
 ** ----------------------------------------------------------*/

typedef struct ucl_iterator_struct_t	ucl_iterator_struct_t;
typedef void ucl_iterator_next_t	(ucl_iterator_struct_t *iterator);

struct ucl_iterator_struct_t {
  void *		iterator;
  const void *		container;
  ucl_iterator_next_t *	next;

  void *		ptr1;
  void *		ptr2;
};

typedef ucl_iterator_struct_t		ucl_iterator_t[1];

/* ------------------------------------------------------------ */

static __inline__ ucl_bool_t	
ucl_iterator_more (ucl_iterator_t iterator_p)
{
  return (iterator_p->iterator != 0);
}
static __inline__ void
ucl_iterator_next (ucl_iterator_t iterator_p)
{
  iterator_p->next(iterator_p);
}
static __inline__ void *
ucl_iterator_ptr (ucl_iterator_t iterator_p)
{
  return iterator_p->iterator;
}

/* ------------------------------------------------------------ */


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
#  define UCL_HASH_DEFAULT_SIZE		16
#endif

#ifndef UCL_HASH_MINIMUM_SIZE
#  define UCL_HASH_MINIMUM_SIZE		16
#endif

#ifndef UCL_HASH_DEFAULT_STEP_UP
#  define UCL_HASH_DEFAULT_STEP_UP	16
#endif

#ifndef UCL_HASH_DEFAULT_STEP_DOWN
#  define UCL_HASH_DEFAULT_STEP_DOWN	((UCL_HASH_DEFAULT_STEP_UP)+1)
#endif

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Binary tree type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_btree_node_tag_t {
  struct ucl_btree_node_tag_t *	dad_p;
  struct ucl_btree_node_tag_t *	bro_p;
  struct ucl_btree_node_tag_t *	son_p;
} ucl_btree_node_tag_t;

typedef ucl_btree_node_tag_t *	ucl_btree_node_t;

/* ------------------------------------------------------------ */

#if (defined __GNUC__)
#  if !(defined ucl_btree_decl)
#    define ucl_btree_decl		\
  __attribute__((__always_inline__,__nonnull__))
#    define ucl_btree_decl_null		\
  __attribute__((__always_inline__,__nonnull__(1)))
#    define ucl_btree_decl_pure		\
  __attribute__((__always_inline__,__pure__,__nonnull__))
#  else
#    define ucl_btree_decl		/* empty */
#    define ucl_btree_decl		/* empty */
#  endif /* not defined ucl_btree_decl */
#endif /* defined __GNUC__ */

/* ------------------------------------------------------------ */
/* setters */

static __inline__ ucl_btree_decl_null void
ucl_btree_setbro (void * self, void * bro_p)
{
  ((ucl_btree_node_t)self)->bro_p = bro_p;
}
static __inline__ ucl_btree_decl_null void
ucl_btree_setson (void * self, void * son_p)
{
  ((ucl_btree_node_t)self)->son_p = son_p;
}
static __inline__ ucl_btree_decl_null void
ucl_btree_setdad (void * self, void * dad_p)
{
  ((ucl_btree_node_t)self)->dad_p = dad_p;
}
static __inline__ ucl_btree_decl void
ucl_btree_dadbro (void * dad_p, void * bro_p)
{
  ucl_btree_setdad(bro_p, dad_p);
  ucl_btree_setbro(dad_p, bro_p);
}
static __inline__ ucl_btree_decl void
ucl_btree_dadson (void * dad_p, void * son_p)
{
  ucl_btree_setdad(son_p, dad_p);
  ucl_btree_setson(dad_p, son_p);
}
static __inline__ ucl_btree_decl void
ucl_btree_dadsonbro (void * dad_p, void * son_p, void * bro_p)
{
  ucl_btree_setdad(son_p, dad_p);
  ucl_btree_setdad(bro_p, dad_p);
  ucl_btree_setson(dad_p, son_p);
  ucl_btree_setbro(dad_p, bro_p);
}

/* ------------------------------------------------------------ */
/* getters */

static __inline__ ucl_btree_decl_pure void *
ucl_btree_getbro (void * self)
{
  return ((ucl_btree_node_t)self)->bro_p;
}
static __inline__ ucl_btree_decl_pure void *
ucl_btree_getson (void * self)
{
  return ((ucl_btree_node_t)self)->son_p;
}
static __inline__ ucl_btree_decl_pure void *
ucl_btree_getdad (void * self)
{
  return ((ucl_btree_node_t)self)->dad_p;
}
static __inline__ ucl_btree_decl_pure void *
ucl_btree_data (void * self)
{
  typedef struct {
    ucl_btree_node_t	dad_p;
    ucl_btree_node_t	bro_p;
    ucl_btree_node_t	son_p;
    uint8_t		data[];
  } ucl_btree_embedded_node_tag_t;

  return ((ucl_btree_embedded_node_tag_t *)self)->data;
}

/* ------------------------------------------------------------ */
/* detaching */

static __inline__ ucl_btree_decl void *
ucl_btree_detach_son (void * self)
{
  ucl_btree_node_t	son_p = ucl_btree_getson(self);

  ucl_btree_setson(self, NULL);
  ucl_btree_setdad(son_p, NULL);
  return son_p;
}
static __inline__ ucl_btree_decl void *
ucl_btree_detach_bro (void * self)
{
  ucl_btree_node_t	bro_p = ucl_btree_getbro(self);

  ucl_btree_setbro(self, NULL);
  ucl_btree_setdad(bro_p, NULL);
  return bro_p;
}
static __inline__ ucl_btree_decl void *
ucl_btree_detach_dad (void * self)
{
  ucl_btree_node_t	dad_p = ucl_btree_getdad(self);

  ucl_btree_setdad(self, NULL);
  ucl_btree_setdad(dad_p, NULL);
  return dad_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Circular list type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_circular_link_t {
  struct ucl_circular_link_t *	next;
  struct ucl_circular_link_t *	prev;
  ucl_value_t			val;
} ucl_circular_link_t;

typedef struct ucl_circular_struct_t {
  ucl_circular_link_t *	current_link;
  size_t		size;
  ucl_comparison_t	compar;
} ucl_circular_struct_t;

typedef ucl_circular_struct_t ucl_circular_t[1];

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
ucl_circular_set_compar (ucl_circular_t this, ucl_comparison_t compar)
{
  this->compar = compar;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Heap container types definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_heap_node_t {
  struct ucl_heap_node_t *	dad_p;
  struct ucl_heap_node_t *	bro_p;
  struct ucl_heap_node_t *	son_p;
  ucl_value_t			val;
} ucl_heap_node_t;

typedef struct ucl_heap_struct_t {
  size_t		size;
  ucl_heap_node_t *	root_p;
  ucl_heap_node_t *	next_p;
  int			state;
  ucl_comparison_t	compar;
} ucl_heap_struct_t;

typedef ucl_heap_struct_t ucl_heap_t[1];

/* ------------------------------------------------------------ */

static __inline__ void
ucl_heap_destructor (ucl_heap_t this)
{
  ucl_struct_clean(this, ucl_heap_struct_t);
}
static __inline__ size_t
ucl_heap_size (const ucl_heap_t this)
{
  return this->size;
}
static __inline__ void
ucl_heap_setval (ucl_heap_node_t * node_p, const ucl_value_t newval)
{
  node_p->val = newval;
}
static __inline__ ucl_value_t
ucl_heap_getval (const ucl_heap_node_t * node_p)
{
  return node_p->val;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** List container types definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_list_link_t {
  struct ucl_list_link_t *	next;
  struct ucl_list_link_t *	prev;
  struct ucl_list_link_t *	dummy;
  ucl_value_t			val;
} ucl_list_link_t;

typedef struct ucl_list_struct_t {
  size_t		size;	/* number of allocated nodes */

  ucl_list_link_t *	front;	/* pointer to the first node */
  ucl_list_link_t *	back;	/* pointer to the last node */

  size_t		li;	/* the index of the last accessed node */
  ucl_list_link_t *	ll;	/* pointer to the last accessed node */
} ucl_list_struct_t;

typedef ucl_list_struct_t ucl_list_t[1];

/* ------------------------------------------------------------ */

static __inline__ void
ucl_list_constructor (ucl_list_t this)
{
  ucl_struct_clean(this, ucl_list_struct_t);
}
static __inline__ void
ucl_list_destructor (ucl_list_t this)
{
  ucl_struct_clean(this, ucl_list_struct_t);
}

/* ------------------------------------------------------------ */

static __inline__ ucl_list_link_t *
ucl_list_front (ucl_list_t this)
{
  return this->front;
}
static __inline__ ucl_list_link_t *
ucl_list_back (ucl_list_t this)
{
  return this->back;
}
static __inline__ ucl_list_link_t *
ucl_list_next (ucl_list_link_t * link_p)
{
  return link_p->next;
}
static __inline__ ucl_list_link_t *
ucl_list_prev (ucl_list_link_t * link_p)
{
  return link_p->prev;
}

/* ------------------------------------------------------------ */

static __inline__ size_t
ucl_list_size (ucl_list_t this)
{
  return this->size;
}
static __inline__ ucl_value_t
ucl_list_getval (ucl_list_link_t * link_p)
{
  return link_p->val;
}
static __inline__ void
ucl_list_setval (ucl_list_link_t * link_p, ucl_value_t newval)
{
  link_p->val = newval;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Map container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_map_link_t {
  ucl_btree_node_tag_t	node;
  char		avl_status;
  ucl_value_t	key;
  ucl_value_t	val;
} ucl_map_link_t;

typedef struct ucl_map_struct_t {
  size_t		size;
  unsigned int		flags;
  ucl_comparison_t	keycmp;
  ucl_map_link_t *	root;
} ucl_map_struct_t;

typedef ucl_map_struct_t ucl_map_t[1];

#define UCL_ALLOW_MULTIPLE_OBJECTS	0x10

/* ------------------------------------------------------------ */

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

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Tree container types definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_tree_node_t {
  struct ucl_tree_node_t *	dad_p;
  struct ucl_tree_node_t *	bro_p;
  struct ucl_tree_node_t *	son_p;
} ucl_tree_node_t;

/* ------------------------------------------------------------ */

static __inline__ void
ucl_tree_constructor (ucl_tree_node_t * this)
{
  ucl_struct_clean(this, ucl_tree_node_t);
}

/* ------------------------------------------------------------ */

static __inline__ ucl_bool_t
ucl_tree_has_next (const ucl_tree_node_t * this)
{
  return (NULL != this->bro_p);
}
static __inline__ ucl_bool_t
ucl_tree_has_son (const ucl_tree_node_t * this)
{
  return (NULL != this->son_p);
}

/* ------------------------------------------------------------ */

static __inline__ ucl_tree_node_t *
ucl_tree_get_next (const ucl_tree_node_t * nod_p)
{
  return nod_p->bro_p;
}
static __inline__ ucl_tree_node_t *
ucl_tree_get_son (const ucl_tree_node_t * nod_p)
{
  return nod_p->son_p;
}

/* ------------------------------------------------------------ */

static __inline__ void
ucl_tree_set_son (ucl_tree_node_t * nod_p, ucl_tree_node_t * son_p)
{
  nod_p->son_p = son_p;
  son_p->dad_p = nod_p;
}
static __inline__ void
ucl_tree_set_bro (ucl_tree_node_t * nod_p, ucl_tree_node_t * bro_p)
{
  nod_p->bro_p = bro_p;
  bro_p->dad_p = nod_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Vector container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_vector_struct_t {
  ucl_index_t	step_up;
  ucl_index_t	step_down;
  ucl_index_t	size_of_padding_area;
  ucl_index_t	slot_dimension;

  ucl_comparison_t	compar;

  uint8_t *	first_allocated_slot;
  uint8_t *	first_used_slot;
  uint8_t *	last_used_slot;
  uint8_t *	last_allocated_slot;

  ucl_memory_allocator_t	allocator;
} ucl_vector_struct_t;

typedef ucl_vector_struct_t ucl_vector_t[1];

typedef struct ucl_vector_array_t {
  size_t			number_of_vectors;
  ucl_vector_struct_t **	vectors;
} ucl_vector_array_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Hash table container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_hash_entry_t {
  struct ucl_hash_entry_t *	next_entry_in_list_p;
  ucl_value_t			key;
  ucl_value_t			val;
  ucl_bool_t			to_be_processed_during_rehashing;
} ucl_hash_entry_t;

typedef struct ucl_hash_table_tag_t {
  ucl_vector_t		buckets;
  ucl_hash_t		hash;
  ucl_comparison_t	compar;
  size_t		size;
  size_t		number_of_used_buckets;
} ucl_hash_table_tag_t;

typedef ucl_hash_table_tag_t *	ucl_hash_table_t;

/* ------------------------------------------------------------ */

static __inline__ size_t
ucl_hash_size (const ucl_hash_table_t this)
{
  return this->size;
}
static __inline__ void
ucl_hash_setkey (ucl_hash_entry_t * entry_p, const ucl_value_t newkey)
{
  entry_p->key = newkey;
}
static __inline__ ucl_value_t
ucl_hash_getkey (const ucl_hash_entry_t * entry_p)
{
  return entry_p->key;
}
static __inline__ void
ucl_hash_setval (ucl_hash_entry_t * entry_p, const ucl_value_t newval)
{
  entry_p->val = newval;
}
static __inline__ ucl_value_t
ucl_hash_getval (const ucl_hash_entry_t * entry_p)
{
  return entry_p->val;
}

/* ------------------------------------------------------------ */


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
	 NULL  != (NEXT);			\
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

#define UCL_GRAPH_INPUT_LINKS_LOOP(NODE,ITER)	\
						\
  for ((ITER) = ucl_graph_input_link(NODE);	\
       NULL  != (ITER);				\
       (ITER) = ucl_graph_next_input_link(ITER))

#define UCL_GRAPH_OUTPUT_LINKS_LOOP(NODE,ITER)	\
						\
  for ((ITER) = ucl_graph_output_link(NODE);	\
       NULL  != (ITER);				\
       (ITER) = ucl_graph_next_output_link(ITER))

/* ------------------------------------------------------------ */

#define UCL_GRAPH_FIRST_INPUT_LINK_LOOP(NODE,LINK)	\
							\
  for ((LINK) = ucl_graph_input_link((NODE));	\
       NULL != (LINK);					\
       (LINK) = ucl_graph_input_link((NODE)))

#define UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(NODE,LINK)	\
							\
  for ((LINK) = ucl_graph_output_link((NODE));	\
       NULL != (LINK);					\
       (LINK) = ucl_graph_output_link((NODE)))

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Graph algorithms type definitions.
 ** ----------------------------------------------------------*/

#ifndef UCL_GRAPH_DFS_INITIAL_VECTOR_SIZE
#  define UCL_GRAPH_DFS_INITIAL_VECTOR_SIZE	64
#endif

#ifndef UCL_GRAPH_DFS_STEP_UP
#  define UCL_GRAPH_DFS_STEP_UP			64
#endif

/* ------------------------------------------------------------ */

typedef struct ucl_graph_dfs_t {
  ucl_vector_t		visited_nodes;
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

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Stub interface.
 ** ----------------------------------------------------------*/

#include "ucl_stub.h"

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Other functions.
 ** ----------------------------------------------------------*/

/* The  following  __inline__ functions  require  the  declaration of  other
   functions, so they must be  defined after the stub interface has been
   loaded. */

static __inline__ void
ucl_circular_backward (ucl_circular_t this, int times)
{
  ucl_circular_forward(this, -times);
}

/* ------------------------------------------------------------ */

static __inline__ void
ucl_list_popfront (ucl_list_t this)
{
  ucl_list_extract(this, this->front);
}
static __inline__ void
ucl_list_popback (ucl_list_t this)
{
  ucl_list_extract(this, this->back);
}

/* ------------------------------------------------------------ */

static __inline__ void
ucl_map_extract (ucl_map_t this, ucl_map_link_t * _link)
{
  ucl_map_remove(this, _link);
}

/* ------------------------------------------------------------ */

static __inline__ void *
ucl_vector_front (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, 0);
}
static __inline__ void *
ucl_vector_back (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, ucl_vector_last_index(vector));
}
static __inline__ void *
ucl_vector_increment_slot (const ucl_vector_t self, void * slot)
{
  uint8_t *	p = slot;

  return p + self->slot_dimension;
}
static __inline__ void *
ucl_vector_decrement_slot (const ucl_vector_t self, void * slot)
{
  uint8_t *	p = slot;

  return p - self->slot_dimension;
}
static __inline__ int
ucl_vector_equal_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  return (0 == ucl_vector_compare_range(a, ra, b, rb));
}

/* ------------------------------------------------------------ */

static __inline__ size_t
ucl_hash_number_of_buckets (const ucl_hash_table_t self)
{
  return ucl_vector_size(self->buckets);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** End.
 ** ----------------------------------------------------------*/

UCL_END_C_DECL

#endif /* __UCL_H */

/* end of file */
