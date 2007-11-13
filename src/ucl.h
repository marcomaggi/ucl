/* ucl.h  --

   Part of: Useless Container Library
   Contents: main header file
   Date: late 2001

   Abstract

	This file must be included in every module that makes use of the
	UCL containers.

   Copyright (c)  2001-2007  Marco Maggi

   This is free  software you can redistribute it  and/or modify it under
   the terms of  the GNU General Public License as  published by the Free
   Software Foundation; either  version 2, or (at your  option) any later
   version.
   
   This  file is  distributed in  the hope  that it  will be  useful, but
   WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   General Public License for more details.
   
   You  should have received  a copy  of the  GNU General  Public License
   along with this file; see the file COPYING.  If not, write to the Free
   Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
   02111-1307, USA.

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

typedef unsigned short int	ucl_bool_t;
typedef unsigned char		ucl_byte_t;
typedef size_t			ucl_index_t;
typedef void *			ucl_data_t;
typedef const char *		ucl_cstr_t;

typedef union ucl_value_t {
  unsigned	unum;
  int		num;
  int		integer;
  void *	ptr;
  char *	str;
  ucl_byte_t *	bytes;
} ucl_value_t;

/* ------------------------------------------------------------ */

typedef int ucl_valcmp_fun_t(ucl_value_t data, const ucl_value_t a, const ucl_value_t b);

typedef struct ucl_valcmp_t {
  ucl_value_t		data;
  ucl_valcmp_fun_t *	func;
} ucl_valcmp_t;

typedef ucl_valcmp_t	ucl_keycmp_t;

/* ------------------------------------------------------------ */

typedef size_t	ucl_hashfun_t	(ucl_value_t data, ucl_value_t key);

typedef struct ucl_hashcmp_t {
  ucl_value_t		data;
  ucl_hashfun_t *	func;
} ucl_hashcmp_t;

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
 ** Base functions.
 ** ----------------------------------------------------------*/

#ifndef UCL_ENABLE_STUB

ucl_decl const char * ucl_version      (void);
ucl_decl const char * ucl_major_version (void);
ucl_decl const char * ucl_minor_version (void);
ucl_decl const char * ucl_patch_version (void);
ucl_decl ucl_valcmp_fun_t	ucl_intcmp;
ucl_decl ucl_valcmp_fun_t	ucl_uintcmp;
ucl_decl ucl_valcmp_fun_t	ucl_strcmp;
ucl_decl ucl_valcmp_fun_t	ucl_ptrintcmp;
ucl_decl ucl_hashfun_t		ucl_hash_string;
ucl_decl unsigned   ucl_interface_major_version (void);
ucl_decl unsigned   ucl_interface_minor_version (void);

ucl_decl void ucl_quicksort (void *const pbase, size_t total_elems, size_t size, ucl_valcmp_t cmp);

#endif

/* ------------------------------------------------------------ */



/** ------------------------------------------------------------
 ** Memory allocation.
 ** ----------------------------------------------------------*/

typedef void ucl_memory_alloc_fun_t (void * dummy, void * pp, size_t dim);

typedef struct ucl_memory_allocator_t {
  void *			data;
  ucl_memory_alloc_fun_t *	alloc;
} ucl_memory_allocator_t;

#ifndef UCL_ENABLE_STUB

ucl_decl ucl_memory_alloc_fun_t ucl_memory_alloc;

#endif

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
  ucl_byte_t *	min;
  ucl_byte_t *	max;
} ucl_byte_pointer_range_t;

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Struct macros.
 ** ----------------------------------------------------------*/

static inline void
ucl_p_struct_clean (void * buffer_p, size_t len)
{
  memset(buffer_p, '\0', len);
}
#define ucl_struct_clean(B,TYPE_T)	ucl_p_struct_clean((B),sizeof(TYPE_T))
#define ucl_struct_reset(B,TYPE_T)	ucl_struct_clean((B),(TYPE_T))

static inline void
ucl_p_struct_alloc (ucl_memory_allocator_t * allocator, void ** buffer_pp, size_t len)
{
  allocator->alloc(allocator->data, buffer_pp, len);
}
#define ucl_struct_alloc(A,B,TYPE_T)	ucl_p_struct_alloc((A),&(B),sizeof(TYPE_T))

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Memory block macros.
 ** ----------------------------------------------------------*/

typedef struct ucl_block_struct_t {
  size_t	len;
  ucl_byte_t *	ptr;
} ucl_block_t;

typedef struct ucl_const_block_struct_t {
  size_t		len;
  const ucl_byte_t *	ptr;
} ucl_const_block_t;

#define UCL_BLOCK_NULL_VALUE	{ 0, NULL }

/* ------------------------------------------------------------ */

#define ucl_block_set(BLOCK, PTR, LEN)	((BLOCK).ptr = (ucl_byte_t *)(PTR), (BLOCK).len = LEN)
#define ucl_block_reset(BLOCK)		ucl_block_set((BLOCK),NULL,0)

#define ucl_block_alloc(ALLOCATOR, BLOCK, DIM)	\
	((ALLOCATOR).alloc((ALLOCATOR).data, &((BLOCK).ptr), (DIM)), \
	 (BLOCK).len = (DIM))

#define ucl_block_is_null(BLOCK)	(! (BLOCK).ptr)
#define ucl_block_clean_memory(BLOCK)	memset((BLOCK).ptr, '\0', (BLOCK).len)

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** ASCII string macros.
 ** ----------------------------------------------------------*/

typedef struct ucl_ascii_struct_t {
  size_t	len;
  char *	ptr;
} ucl_ascii_t;

typedef struct ucl_const_ascii_struct_t {
  size_t	len;
  const char *	ptr;
} ucl_const_ascii_t;

typedef struct ucl_ascii_list_struct_t {
  size_t	len;
  char **	ptr;
} ucl_ascii_list_t;

#define UCL_ASCII_NULL_VALUE	{ 0, NULL }

/* ------------------------------------------------------------ */

#define ucl_ascii_alloc(ALLOCATOR, ASCII, DIM)				\
	(((ASCII).ptr = NULL),						\
	 (ALLOCATOR).alloc((ALLOCATOR).data, &((ASCII).ptr), (DIM)+1),	\
	 ((ASCII).len = (DIM)),						\
	 ((ASCII).ptr[(DIM)]='\0'))

#define ucl_ascii_set(ASCII, PTR, LEN)	((ASCII).ptr = (PTR), (ASCII).len = LEN)
#define ucl_ascii_reset(ASCII)		((ASCII).ptr = NULL, (ASCII).len = 0)
#define ucl_ascii_const(ASCII, STRING)				\
	ucl_ascii_set((ASCII), (char *)(STRING), strlen(STRING))

#define ucl_ascii_is_null(ASCII)	(! (ASCII).ptr)
#define ucl_ascii_clean_memory(ASCII)	memset((ASCII).ptr, '\0', (ASCII).len)


#define mcl_block_from_ascii(BLOCK,ASCII)			\
		((BLOCK).len=((ASCII).len+1),			\
                 (BLOCK).ptr=(ucl_byte_t *)((ASCII).ptr))

#define ucl_ascii_from_block(ASCII,BLOCK)			\
		((ASCII).len=((BLOCK).len-1),			\
		 (ASCII).ptr=(char *)((BLOCK).ptr),		\
                 (ASCII).ptr[(BLOCK).len]='\0')

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Callback.
 ** ----------------------------------------------------------*/

#define UCL_CALLBACK_NULL_VALUE		{ .func = NULL, .data = { .ptr = 0 } }

typedef void ucl_callback_fun_t (ucl_value_t callback_data, ucl_value_t custom_data);

typedef struct ucl_callback_t {
  ucl_callback_fun_t *	func;
  ucl_value_t		data;
} ucl_callback_t;

static inline ucl_bool_t
ucl_callback_is_present (ucl_callback_t callback)
{
  return (NULL != (callback.func));
}

static inline void
ucl_callback_invoke_alone (ucl_callback_t callback)
{
  static const ucl_value_t	custom_data = { .ptr = NULL };

  if (ucl_callback_is_present(callback))
    callback.func(callback.data, custom_data);
}
static inline void
ucl_callback_invoke (ucl_callback_t callback, ucl_value_t custom_data)
{
  if (ucl_callback_is_present(callback))
    callback.func(callback.data, custom_data);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Debug module.
 ** ----------------------------------------------------------*/

#ifndef UCL_DEBUGGING
#  define UCL_DEBUGGING		0
#endif

#if (UCL_DEBUGGING == 1)
#  warning UCL debugging is on
#  define ucl_debug(...)	ucl_debug_print(__func__,__VA_ARGS__)
#else
#  define ucl_debug(...)	/* empty */
#endif

#ifndef UCL_ENABLE_STUB

ucl_decl void       ucl_debug_on       (void);
ucl_decl void       ucl_debug_print    (const char *function, const char *template, ...);
ucl_decl void       ucl_debug_off      (void);
ucl_decl const char * ucl_question     (ucl_bool_t expr);

#endif

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

static inline ucl_bool_t	
ucl_iterator_more (ucl_iterator_t iterator_p)
{
  return (iterator_p->iterator != 0);
}
static inline void
ucl_iterator_next (ucl_iterator_t iterator_p)
{
  iterator_p->next(iterator_p);
}
static inline void *
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

typedef struct ucl_btree_node_t {
  struct ucl_btree_node_t *	dad_p;
  struct ucl_btree_node_t *	bro_p;
  struct ucl_btree_node_t *	son_p;
} ucl_btree_node_t;

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

/* init/final */
ucl_decl void ucl_btree_constructor	(ucl_btree_node_t * node_p, const ucl_btree_node_t *dad_p);
ucl_decl void ucl_btree_destructor	(ucl_btree_node_t * node_p);

/* setters */
ucl_decl void ucl_btree_setbro (ucl_btree_node_t * this, const ucl_btree_node_t * newBro);
ucl_decl void ucl_btree_setson (ucl_btree_node_t * this, const ucl_btree_node_t * newSon);
ucl_decl void ucl_btree_setdad (ucl_btree_node_t * this, const ucl_btree_node_t * newDad);

ucl_decl void ucl_btree_dadbro (ucl_btree_node_t * dad, ucl_btree_node_t * bro);
ucl_decl void ucl_btree_dadson (ucl_btree_node_t * dad, ucl_btree_node_t * son);

/* getters */
ucl_decl ucl_btree_node_t * ucl_btree_getbro (const ucl_btree_node_t * this);
ucl_decl ucl_btree_node_t * ucl_btree_getson (const ucl_btree_node_t * this);
ucl_decl ucl_btree_node_t * ucl_btree_getdad (const ucl_btree_node_t * this);

/* step */
ucl_decl ucl_btree_node_t * ucl_btree_step_inorder	(const ucl_btree_node_t * cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_inorder_backward (const ucl_btree_node_t * cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_preorder	(const ucl_btree_node_t * cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_postorder	(const ucl_btree_node_t * cur_p);
ucl_decl ucl_btree_node_t * ucl_btree_step_levelorder	(const ucl_btree_node_t * cur_p);

/* search */
ucl_decl ucl_btree_node_t * ucl_btree_find_leftmost	(const ucl_btree_node_t * node_p);
ucl_decl ucl_btree_node_t * ucl_btree_find_rightmost	(const ucl_btree_node_t * node_p);
ucl_decl ucl_btree_node_t * ucl_btree_find_deepest_son	(const ucl_btree_node_t * node_p);

#endif

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
  ucl_valcmp_t		compar;
} ucl_circular_struct_t;

typedef ucl_circular_struct_t ucl_circular_t[1];

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

/* insertion and extraction */
ucl_decl void			ucl_circular_insert	(ucl_circular_t this, ucl_circular_link_t *new);
ucl_decl ucl_circular_link_t *	ucl_circular_extract	(ucl_circular_t this);

/* stepping */
ucl_decl void ucl_circular_forward (ucl_circular_t this, int times);

/* searching */
ucl_decl ucl_circular_link_t * ucl_circular_find (ucl_circular_t this, ucl_value_t val);

#endif

/* ------------------------------------------------------------ */

static inline void
ucl_circular_constructor (ucl_circular_t this)
{
  ucl_struct_clean(this, ucl_circular_t);
}
static inline void
ucl_circular_destructor (ucl_circular_t this)
{
  ucl_struct_clean(this, ucl_circular_t);
}
static inline size_t
ucl_circular_size (ucl_circular_t this)
{
  return this->size;
}
static inline ucl_circular_link_t *
ucl_circular_current (ucl_circular_t this)
{
  return this->current_link;
}

/* ------------------------------------------------------------ */

static inline ucl_value_t
ucl_circular_data (ucl_circular_link_t * link_p)
{
  return link_p->val;
}
static inline ucl_value_t
ucl_circular_getval (ucl_circular_link_t * link_p)
{
  return link_p->val;
}
static inline void
ucl_circular_setval (ucl_circular_link_t * link_p, ucl_value_t newval)
{
  link_p->val = newval;
}
static inline void
ucl_circular_set_compar (ucl_circular_t this, ucl_valcmp_t compar)
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
  ucl_valcmp_t		valcmp;
} ucl_heap_struct_t;

typedef ucl_heap_struct_t ucl_heap_t[1];

/* ------------------------------------------------------------ */

static inline void
ucl_heap_destructor (ucl_heap_t this)
{
  ucl_struct_clean(this, ucl_heap_struct_t);
}
static inline size_t
ucl_heap_size (const ucl_heap_t this)
{
  return this->size;
}
static inline void
ucl_heap_setval (ucl_heap_node_t * node_p, const ucl_value_t newval)
{
  node_p->val = newval;
}
static inline ucl_value_t
ucl_heap_getval (const ucl_heap_node_t * node_p)
{
  return node_p->val;
}

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_heap_constructor (ucl_heap_t this, ucl_valcmp_t compar);
ucl_decl void ucl_heap_insert (ucl_heap_t this, ucl_heap_node_t *node_p);
ucl_decl ucl_heap_node_t * ucl_heap_extract (ucl_heap_t this);
ucl_decl void ucl_heap_merge (ucl_heap_t this, ucl_heap_t other);

#endif

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

#ifndef UCL_ENABLE_STUB

ucl_decl ucl_list_link_t * ucl_list_index (ucl_list_t this, size_t idx);
ucl_decl void ucl_list_insertbefore (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
ucl_decl void ucl_list_insertafter (ucl_list_t this, ucl_list_link_t *link_p, ucl_list_link_t *new_p);
ucl_decl void ucl_list_pushfront (ucl_list_t this, ucl_list_link_t *link_p);
ucl_decl void ucl_list_pushback  (ucl_list_t this, ucl_list_link_t *link_p);
ucl_decl void ucl_list_extract   (ucl_list_t this, ucl_list_link_t *link_p);

#endif

/* ------------------------------------------------------------ */

static inline void
ucl_list_constructor (ucl_list_t this)
{
  ucl_struct_clean(this, ucl_list_struct_t);
}
static inline void
ucl_list_destructor (ucl_list_t this)
{
  ucl_struct_clean(this, ucl_list_struct_t);
}

/* ------------------------------------------------------------ */

static inline ucl_list_link_t *
ucl_list_front (ucl_list_t this)
{
  return this->front;
}
static inline ucl_list_link_t *
ucl_list_back (ucl_list_t this)
{
  return this->back;
}
static inline ucl_list_link_t *
ucl_list_next (ucl_list_link_t * link_p)
{
  return link_p->next;
}
static inline ucl_list_link_t *
ucl_list_prev (ucl_list_link_t * link_p)
{
  return link_p->prev;
}

/* ------------------------------------------------------------ */

static inline size_t
ucl_list_size (ucl_list_t this)
{
  return this->size;
}
static inline ucl_value_t
ucl_list_getval (ucl_list_link_t * link_p)
{
  return link_p->val;
}
static inline void
ucl_list_setval (ucl_list_link_t * link_p, ucl_value_t newval)
{
  link_p->val = newval;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Map container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_map_link_t {
  ucl_btree_node_t 	node;
  char			avl_status;
  ucl_value_t		key;
  ucl_value_t		val;
} ucl_map_link_t;

typedef struct ucl_map_struct_t {
  size_t		size;
  unsigned int		flags;
  ucl_valcmp_t		keycmp;
  ucl_map_link_t *	root;
} ucl_map_struct_t;

typedef ucl_map_struct_t ucl_map_t[1];

#define UCL_ALLOW_MULTIPLE_OBJECTS	0x10

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_map_constructor (ucl_map_t this, unsigned int flags, ucl_keycmp_t keycmp);
ucl_decl void ucl_map_destructor (ucl_map_t this);
ucl_decl void ucl_map_insert     (ucl_map_t this, ucl_map_link_t *link_p);
ucl_decl ucl_map_link_t * ucl_map_remove (ucl_map_t this, ucl_map_link_t *cur_p);
ucl_decl ucl_map_link_t * ucl_map_find (const ucl_map_t this, const ucl_value_t key);
ucl_decl ucl_map_link_t * ucl_map_first (const ucl_map_t this);
ucl_decl ucl_map_link_t * ucl_map_last (const ucl_map_t this);
ucl_decl ucl_map_link_t * ucl_map_next (ucl_map_link_t *link_p);
ucl_decl ucl_map_link_t * ucl_map_prev (ucl_map_link_t *link_p);
ucl_decl ucl_map_link_t * ucl_map_find_or_next (const ucl_map_t this, const ucl_value_t key);
ucl_decl ucl_map_link_t * ucl_map_find_or_prev (const ucl_map_t this, const ucl_value_t key);
ucl_decl size_t ucl_map_count      (const ucl_map_t this, const ucl_value_t key);

ucl_decl void ucl_map_iterator_inorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_inorder_backward (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_preorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_postorder (const ucl_map_t this, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_levelorder (const ucl_map_t this, ucl_iterator_t iterator);

ucl_decl void ucl_map_lower_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);
ucl_decl void ucl_map_upper_bound (const ucl_map_t this, ucl_iterator_t iterator, const ucl_value_t key);

ucl_decl void ucl_map_iterator_union (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_intersection (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);
ucl_decl void ucl_map_iterator_complintersect (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t 	iterator);
ucl_decl void ucl_map_iterator_subtraction (ucl_iterator_t iter1, ucl_iterator_t iter2, ucl_iterator_t iterator);

#endif /* not defined UCL_ENABLE_STUB */

/* ------------------------------------------------------------ */

static inline size_t
ucl_map_size (const ucl_map_t this)
{
  return this->size;
}
static inline ucl_value_t
ucl_map_getkey (const ucl_map_link_t * link_p)
{
  return link_p->key;
}
static inline ucl_value_t
ucl_map_getval (const ucl_map_link_t * link_p)
{
  return link_p->val;
}
static inline void
ucl_map_setkey (ucl_map_link_t * link_p, ucl_value_t newkey)
{
  link_p->key = newkey;
}
static inline void
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

static inline void
ucl_tree_constructor (ucl_tree_node_t * this)
{
  ucl_struct_clean(this, ucl_tree_node_t);
}

/* ------------------------------------------------------------ */

static inline ucl_bool_t
ucl_tree_has_next (const ucl_tree_node_t * this)
{
  return (ucl_bool_t)(NULL != this->bro_p);
}
static inline ucl_bool_t
ucl_tree_has_son (const ucl_tree_node_t * this)
{
  return (ucl_bool_t)(NULL != this->son_p);
}

/* ------------------------------------------------------------ */

static inline ucl_tree_node_t *
ucl_tree_get_next (const ucl_tree_node_t * nod_p)
{
  return nod_p->bro_p;
}
static inline ucl_tree_node_t *
ucl_tree_get_son (const ucl_tree_node_t * nod_p)
{
  return nod_p->son_p;
}

/* ------------------------------------------------------------ */

static inline void
ucl_tree_set_son (ucl_tree_node_t * nod_p, ucl_tree_node_t * son_p)
{
  nod_p->son_p = son_p;
  son_p->dad_p = nod_p;
}
static inline void
ucl_tree_set_bro (ucl_tree_node_t * nod_p, ucl_tree_node_t * bro_p)
{
  nod_p->bro_p = bro_p;
  bro_p->dad_p = nod_p;
}

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

ucl_decl ucl_bool_t ucl_tree_is_dad   (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *cld_p);
ucl_decl ucl_bool_t ucl_tree_is_bro   (const ucl_tree_node_t *nod_p, const ucl_tree_node_t *bro_p);
ucl_decl ucl_bool_t ucl_tree_has_dad  (const ucl_tree_node_t *nod_p);
ucl_decl ucl_bool_t ucl_tree_has_prev (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_dad   (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_prev  (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_first (const ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_get_last  (const ucl_tree_node_t *nod_p);
ucl_decl void ucl_tree_insert_dad (ucl_tree_node_t *nod_p, ucl_tree_node_t *dad_p);
ucl_decl void ucl_tree_insert_son (ucl_tree_node_t * nod_p, ucl_tree_node_t * son_p);
ucl_decl void ucl_tree_insert_next (ucl_tree_node_t * nod_p, ucl_tree_node_t * nxt_p);
ucl_decl void ucl_tree_insert_prev (ucl_tree_node_t * nod_p, ucl_tree_node_t * prv_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_dad (ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_son (ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_prev (ucl_tree_node_t *nod_p);
ucl_decl ucl_tree_node_t * ucl_tree_extract_next (ucl_tree_node_t *nod_p);
ucl_decl void ucl_tree_iterator_inorder (const ucl_tree_node_t * nod_p, ucl_iterator_t iterator);
ucl_decl void ucl_tree_iterator_preorder (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);
ucl_decl void ucl_tree_iterator_postorder (const ucl_tree_node_t *nod_p, ucl_iterator_t iterator);

#endif

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Vector container type definitions.
 ** ----------------------------------------------------------*/

typedef size_t		ucl_vector_index_t;

typedef struct ucl_vector_struct_t {
  ucl_vector_index_t	step_up;
  ucl_vector_index_t	step_down;
  ucl_vector_index_t	size_of_padding_area;
  ucl_vector_index_t	slot_dimension;

  ucl_valcmp_t		compar;

  ucl_byte_t *		first_allocated_slot;
  ucl_byte_t *		first_used_slot;
  ucl_byte_t *		last_used_slot;
  ucl_byte_t *		last_allocated_slot;

  ucl_memory_allocator_t	allocator;
} ucl_vector_struct_t;

typedef ucl_vector_struct_t ucl_vector_t[1];

typedef struct ucl_vector_array_t {
  size_t			number_of_vectors;
  ucl_vector_struct_t **	vectors;
} ucl_vector_array_t;

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

/* initialisation */
ucl_decl void ucl_vector_initialise		(ucl_vector_t self, size_t slot_dimension);
ucl_decl void ucl_vector_initialise_size	(ucl_vector_t self, size_t number_of_initial_slots);
ucl_decl void ucl_vector_initialise_pad		(ucl_vector_t self, size_t size_of_padding_area);
ucl_decl void ucl_vector_initialise_step_up	(ucl_vector_t self, size_t step_up);
ucl_decl void ucl_vector_initialise_step_down	(ucl_vector_t self, size_t step_down);
ucl_decl void ucl_vector_set_compar		(ucl_vector_t self, ucl_valcmp_t compar);

ucl_decl void ucl_vector_initialise_buffer	(ucl_vector_t self);

/* construction and destruction */
ucl_decl void ucl_vector_constructor	(ucl_vector_t self);
ucl_decl void ucl_vector_destructor	(ucl_vector_t self);

/* inspection */
ucl_decl size_t ucl_vector_number_of_step_up_slots	(const ucl_vector_t self);
ucl_decl size_t ucl_vector_number_of_step_down_slots	(const ucl_vector_t self);
ucl_decl size_t ucl_vector_number_of_padding_slots	(const ucl_vector_t self);
ucl_decl size_t ucl_vector_number_of_free_slots		(const ucl_vector_t self);
ucl_decl size_t ucl_vector_slot_dimension		(const ucl_vector_t self);
ucl_decl size_t ucl_vector_size				(const ucl_vector_t self);
ucl_decl ucl_bool_t ucl_vector_running			(const ucl_vector_t self);

/* configuration update */
ucl_decl void ucl_vector_update_number_of_step_up_slots		(ucl_vector_t self, size_t step_up);
ucl_decl void ucl_vector_update_number_of_step_down_slots	(ucl_vector_t self, size_t step_down);
ucl_decl void ucl_vector_update_number_of_padding_slots		(ucl_vector_t self, size_t padding);

/* index conversion */
ucl_decl void * ucl_vector_index_to_slot	(const ucl_vector_t self, ucl_vector_index_t idx);
ucl_decl void * ucl_vector_index_to_new_slot	(const ucl_vector_t self, ucl_vector_index_t idx);

/* index getters */
ucl_decl ucl_vector_index_t ucl_vector_last_index	(const ucl_vector_t self);
ucl_decl ucl_vector_index_t ucl_vector_slot_to_index	(const ucl_vector_t self, const void *_pointer_to_slot_p);

/* index predicates */
ucl_decl ucl_bool_t ucl_vector_pointer_is_valid_slot	(const ucl_vector_t self, const void *_pointer_to_slot_p);
ucl_decl ucl_bool_t ucl_vector_index_is_valid		(const ucl_vector_t self, ucl_vector_index_t idx);
ucl_decl ucl_bool_t ucl_vector_index_is_valid_new	(const ucl_vector_t self, ucl_vector_index_t idx);

/* range of indexes */
ucl_decl ucl_bool_t ucl_vector_range_is_valid		(const ucl_vector_t self, ucl_range_t range);
ucl_decl ucl_range_t ucl_vector_range			(const ucl_vector_t self);
ucl_decl ucl_range_t ucl_vector_range_from_position_to_end(const ucl_vector_t self, ucl_vector_index_t position);
ucl_decl ucl_range_t ucl_vector_range_from_end_to_position(const ucl_vector_t self, ucl_vector_index_t position);
ucl_decl ucl_range_t ucl_vector_range_from_end_with_span  (const ucl_vector_t self, size_t span);

/* data getters and setters */
ucl_decl ucl_block_t ucl_vector_get_memory_block	(const ucl_vector_t self);
ucl_decl ucl_block_t ucl_vector_get_data_block		(const ucl_vector_t self);
ucl_decl ucl_block_t ucl_vector_block_from_range	(const ucl_vector_t self, const ucl_range_t range);
ucl_decl ucl_range_t ucl_vector_range_from_block	(const ucl_vector_t self, const ucl_block_t block);
ucl_decl ucl_block_t ucl_vector_get_free_block_at_end		(ucl_vector_t self, size_t requested_slots);
ucl_decl ucl_block_t ucl_vector_get_free_block_at_beginning	(ucl_vector_t self, size_t requested_slots);

/* inserting */
ucl_decl void *		ucl_vector_insert	(ucl_vector_t self, void *_pointer_to_slot_p);
ucl_decl ucl_data_t	ucl_vector_push_back	(ucl_vector_t self);
ucl_decl ucl_data_t	ucl_vector_push_front	(ucl_vector_t self);
ucl_decl void *		ucl_vector_insert_sort	(ucl_vector_t self, void *data_p);

/* appending */
ucl_decl void ucl_vector_append_range	(ucl_vector_t target, const ucl_vector_t source, ucl_range_t range);
ucl_decl void ucl_vector_append		(ucl_vector_t target, const ucl_vector_t source);
ucl_decl void ucl_vector_append_more	(ucl_vector_t target, const ucl_vector_t source, ...);
ucl_decl void ucl_vector_append_more_from_array (ucl_vector_t target, const ucl_vector_array_t * vectors);
ucl_decl void ucl_vector_append_block	(ucl_vector_t self, const ucl_block_t block);

/* high level insert */
ucl_decl void ucl_vector_insert_vector	(ucl_vector_t target, ucl_vector_index_t offset, const ucl_vector_t source);
ucl_decl void ucl_vector_insert_block	(ucl_vector_t target, ucl_vector_index_t offset, const ucl_block_t block);
ucl_decl void ucl_vector_insert_range	(ucl_vector_t target, ucl_vector_index_t offset,
					 const ucl_vector_t source, ucl_range_t range);

/* erasing */
ucl_decl void ucl_vector_erase		(ucl_vector_t self, void *_pointer_to_slot_p);
ucl_decl void ucl_vector_erase_range	(ucl_vector_t self, ucl_range_t index_range);
ucl_decl void ucl_vector_pop_back	(ucl_vector_t self);
ucl_decl void ucl_vector_pop_front	(ucl_vector_t self);
ucl_decl void ucl_vector_reset		(ucl_vector_t self);
ucl_decl void ucl_vector_clean		(ucl_vector_t self);

/* accessing */
ucl_decl void ucl_vector_copy_range	(ucl_vector_t target, ucl_vector_index_t position,
					 ucl_vector_t source, ucl_range_t source_range);
ucl_decl void ucl_vector_set_block	(ucl_vector_t target, ucl_vector_index_t position, ucl_block_t source);
ucl_decl void ucl_vector_get_block	(ucl_block_t target, ucl_vector_index_t position, ucl_vector_t source);

/* sorting */
ucl_decl void ucl_vector_quick_sort	(ucl_vector_t self);
ucl_decl ucl_bool_t ucl_vector_sorted	(ucl_vector_t self);

/* iterating */
ucl_decl void ucl_vector_iterator_forward	(const ucl_vector_t self, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_backward	(const ucl_vector_t self, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_range_forward	(const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);
ucl_decl void ucl_vector_iterator_range_backward(const ucl_vector_t self, ucl_range_t range, ucl_iterator_t iterator);

/* memory allocation */
ucl_decl ucl_bool_t ucl_vector_will_enlarge	(ucl_vector_t self);
ucl_decl ucl_bool_t ucl_vector_will_restrict	(ucl_vector_t self);
ucl_decl size_t     ucl_vector_enlarged_size	(ucl_vector_t self);
ucl_decl size_t     ucl_vector_restricted_size	(ucl_vector_t self);
ucl_decl void ucl_vector_enlarge		(ucl_vector_t self);
ucl_decl void ucl_vector_restrict		(ucl_vector_t self);
ucl_decl void ucl_vector_enlarge_for_slots	(ucl_vector_t self, size_t required_free_slots);
ucl_decl void ucl_vector_enlarge_for_range	(ucl_vector_t self, ucl_range_t range);

/* miscellaneous memory related */
ucl_decl void ucl_vector_register_allocator	(ucl_vector_t self, ucl_memory_allocator_t allocator);
ucl_decl void ucl_vector_set_memory_to_zero	(ucl_vector_t self);
ucl_decl void ucl_vector_mark_all_slots_as_used	(ucl_vector_t self);
ucl_decl void ucl_vector_mark_as_used		(ucl_vector_t self, ucl_block_t used_block);
ucl_decl void ucl_vector_mark_allocated_range_as_used (ucl_vector_t self, ucl_range_t range);
ucl_decl void ucl_vector_swallow_block		(ucl_vector_t self, ucl_block_t block);

/* searching */
ucl_decl void * ucl_vector_find			(const ucl_vector_t self, const void * data_p);
ucl_decl void * ucl_vector_sort_find		(const ucl_vector_t self, const void * data_p);
ucl_decl void * ucl_vector_binary_search	(const ucl_vector_t self, const void * data_p);

/* comparing */
ucl_decl int ucl_vector_compare_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb);
ucl_decl int ucl_vector_compare (const ucl_vector_t a, const ucl_vector_t b);
ucl_decl ucl_bool_t ucl_vector_equal   (const ucl_vector_t a, const ucl_vector_t b);

/* function application */
ucl_decl void ucl_vector_for_each		(ucl_callback_t callback, const ucl_vector_t self);
ucl_decl void ucl_vector_for_each_in_range	(ucl_callback_t callback, ucl_range_t range, const ucl_vector_t self);
ucl_decl void ucl_vector_for_each_multiple	(ucl_callback_t callback, const ucl_vector_t first, ...);
ucl_decl void ucl_vector_for_each_multiple_from_array	(ucl_callback_t callback, const ucl_vector_array_t * vectors);

ucl_decl void ucl_vector_map		(ucl_vector_t result, ucl_callback_t callback, const ucl_vector_t self);
ucl_decl void ucl_vector_map_range	(ucl_vector_t result, ucl_callback_t callback,
					 ucl_range_t range, const ucl_vector_t self);
ucl_decl void ucl_vector_map_multiple	(ucl_vector_t result, ucl_callback_t callback,
					 const ucl_vector_t first, ...);
ucl_decl void ucl_vector_map_multiple_from_array	(ucl_vector_t result, ucl_callback_t callback,
							 const ucl_vector_array_t * vectors);

#endif

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

typedef struct ucl_hash_struct_t {
  ucl_vector_t		buckets;
  ucl_hashcmp_t		hash;
  ucl_valcmp_t		compar;
  size_t		size;
  size_t		number_of_used_buckets;
} ucl_hash_struct_t;

typedef ucl_hash_struct_t	ucl_hash_t[1];

/* ------------------------------------------------------------ */

static inline size_t
ucl_hash_size (const ucl_hash_t this)
{
  return this->size;
}
static inline void
ucl_hash_setkey (ucl_hash_entry_t * entry_p, const ucl_value_t newkey)
{
  entry_p->key = newkey;
}
static inline ucl_value_t
ucl_hash_getkey (const ucl_hash_entry_t * entry_p)
{
  return entry_p->key;
}
static inline void
ucl_hash_setval (ucl_hash_entry_t * entry_p, const ucl_value_t newval)
{
  entry_p->val = newval;
}
static inline ucl_value_t
ucl_hash_getval (const ucl_hash_entry_t * entry_p)
{
  return entry_p->val;
}

/* ------------------------------------------------------------ */

#ifndef UCL_ENABLE_STUB

/* construction and destruction */
ucl_decl void ucl_hash_initialise  (ucl_hash_t this, ucl_valcmp_t compar, ucl_hashcmp_t hash);
ucl_decl void ucl_hash_constructor (ucl_hash_t this);
ucl_decl void ucl_hash_destructor  (ucl_hash_t this);

/* insertion and removal */
ucl_decl void ucl_hash_insert    (ucl_hash_t this, ucl_hash_entry_t *entry_p);
ucl_decl void ucl_hash_extract   (ucl_hash_t this, ucl_hash_entry_t *entry_p);

/* searching */
ucl_decl ucl_hash_entry_t * ucl_hash_find  (const ucl_hash_t this, const ucl_value_t key);
ucl_decl ucl_hash_entry_t * ucl_hash_first (const ucl_hash_t this);

/* memory */
ucl_decl void ucl_hash_enlarge   (ucl_hash_t this);
ucl_decl void ucl_hash_restrict  (ucl_hash_t this);

/* iteration */
ucl_decl void ucl_hash_iterator  (const ucl_hash_t this, ucl_iterator_t iterator);

/* statistics */
ucl_decl size_t ucl_hash_number_of_used_buckets  (const ucl_hash_t self);
ucl_decl size_t ucl_hash_bucket_chain_length	 (const ucl_hash_t this, ucl_vector_index_t position);
ucl_decl double ucl_hash_average_search_distance (const ucl_hash_t this);

#endif

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

static inline size_t
ucl_graph_number_of_output_links (ucl_graph_node_t * node_p)
{
  return node_p->output.number_of_links;
}
static inline size_t
ucl_graph_number_of_input_links (ucl_graph_node_t * node_p)
{
  return node_p->input.number_of_links;
}

/* ------------------------------------------------------------ */

static inline ucl_graph_link_t *
ucl_graph_input_link (const ucl_graph_node_t * node_p)
{
  return node_p->input.first_link_p;
}
static inline ucl_graph_link_t *
ucl_graph_output_link (const ucl_graph_node_t * node_p)
{
  return node_p->output.first_link_p;
}

/* ------------------------------------------------------------ */

static inline ucl_graph_link_t *
ucl_graph_prev_input_link (const ucl_graph_link_t * link_p)
{
  return link_p->input.prev_link_p;
}
static inline ucl_graph_link_t *
ucl_graph_next_input_link (const ucl_graph_link_t * link_p)
{
  return link_p->input.next_link_p;
}
static inline ucl_graph_link_t *
ucl_graph_prev_output_link (const ucl_graph_link_t * link_p)
{
  return link_p->output.prev_link_p;
}
static inline ucl_graph_link_t *
ucl_graph_next_output_link (const ucl_graph_link_t * link_p)
{
  return link_p->output.next_link_p;
}

/* ------------------------------------------------------------ */

static inline ucl_value_t
ucl_graph_node_get_value (const ucl_graph_node_t * node_p)
{
  return node_p->val;
}
static inline void
ucl_graph_node_set_value (ucl_graph_node_t * node_p, const ucl_value_t value)
{
  node_p->val = value;
}
static inline ucl_value_t
ucl_graph_link_get_value (const ucl_graph_link_t * link_p)
{
  return link_p->val;
}
static inline void
ucl_graph_link_set_value (ucl_graph_link_t * link_p, const ucl_value_t value)
{
  link_p->val = value;
}
  
/* ------------------------------------------------------------ */

static inline ucl_value_t
ucl_graph_node_get_mark (const ucl_graph_node_t * node_p)
{
  return node_p->mark;
}
static inline void
ucl_graph_node_set_mark (ucl_graph_node_t * node_p, const ucl_value_t mark)
{
  node_p->mark = mark;
}

/* ------------------------------------------------------------ */

static inline ucl_graph_node_t *
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

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_graph_link     (ucl_graph_node_t * s, ucl_graph_link_t * l, ucl_graph_node_t * d);
ucl_decl void ucl_graph_unlink   (ucl_graph_link_t *link_p);
ucl_decl void ucl_graph_erase_node_free_links (ucl_graph_node_t * node_p, void (* destructor)(void *));
ucl_decl ucl_graph_link_t * ucl_graph_last_output_link (ucl_graph_link_t * link_p);
ucl_decl ucl_graph_link_t * ucl_graph_last_input_link (ucl_graph_link_t * link_p);
ucl_decl ucl_graph_link_t * ucl_graph_first_output_link (ucl_graph_link_t * link_p);
ucl_decl ucl_graph_link_t * ucl_graph_first_input_link (ucl_graph_link_t * link_p);
ucl_decl ucl_link_t * ucl_graph_erase_node_return_links (ucl_graph_node_t * node_p);
ucl_decl ucl_graph_node_t * ucl_graph_remove_next_node (ucl_graph_node_t * node_p);
ucl_decl void ucl_graph_insert_next_node (ucl_graph_node_t * node_p, ucl_graph_node_t * next_p);
ucl_decl void ucl_graph_merge_upon_input_link (ucl_graph_link_t * in, ucl_graph_link_t * out);
ucl_decl void ucl_graph_merge_upon_output_link (ucl_graph_link_t * in, ucl_graph_link_t * out);
ucl_decl ucl_bool_t ucl_graph_nodes_are_linked (ucl_graph_node_t * src_p, ucl_graph_node_t * dst_p);
ucl_decl ucl_bool_t ucl_graph_nodes_are_connected (ucl_graph_node_t * a, ucl_graph_node_t * b);

ucl_decl void ucl_graph_set_next_node (ucl_graph_node_t * node_p, const ucl_graph_node_t * next_p);

#endif

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

#ifndef UCL_ENABLE_STUB

ucl_decl void ucl_graph_initialise_dfs_handle	  (ucl_graph_dfs_t * search_handle);
ucl_decl void ucl_graph_finalise_dfs_handle	  (ucl_graph_dfs_t * search_handle);
ucl_decl void ucl_graph_directed_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);
ucl_decl void ucl_graph_depth_first_search	  (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p);

#endif

/* ------------------------------------------------------------ */



/** ------------------------------------------------------------
 ** Stub interface.
 ** ----------------------------------------------------------*/

#include "ucl_stub.h"


/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Other functions.
 ** ----------------------------------------------------------*/

/* The  following  inline functions  require  the  declaration of  other
   functions, so they must be  defined after the stub interface has been
   loaded. */

static inline void
ucl_circular_backward (ucl_circular_t this, int times)
{
  ucl_circular_forward(this, -times);
}

/* ------------------------------------------------------------ */

static inline void
ucl_list_popfront (ucl_list_t this)
{
  ucl_list_extract(this, this->front);
}
static inline void
ucl_list_popback (ucl_list_t this)
{
  ucl_list_extract(this, this->back);
}

/* ------------------------------------------------------------ */

static inline void
ucl_map_extract (ucl_map_t this, ucl_map_link_t * _link)
{
  ucl_map_remove(this, _link);
}

/* ------------------------------------------------------------ */

static inline void *
ucl_vector_front (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, 0);
}
static inline void *
ucl_vector_back (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, ucl_vector_last_index(vector));
}
static inline void *
ucl_vector_increment_slot (const ucl_vector_t self, void * slot)
{
  ucl_byte_t *	p = slot;

  return p + self->slot_dimension;
}
static inline void *
ucl_vector_decrement_slot (const ucl_vector_t self, void * slot)
{
  ucl_byte_t *	p = slot;

  return p - self->slot_dimension;
}
static inline int
ucl_vector_equal_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  return (0 == ucl_vector_compare_range(a, ra, b, rb));
}

/* ------------------------------------------------------------ */

static inline size_t
ucl_hash_number_of_buckets (const ucl_hash_t self)
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
