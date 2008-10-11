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

#include <ucl_definitions.h>

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

typedef struct ucl_heap_tag_t {
  size_t		size;
  ucl_node_t		root_p;
  ucl_node_t		next_p;
  int			state;
  ucl_comparison_t	compar;
} ucl_heap_tag_t;

typedef ucl_heap_tag_t *	ucl_heap_t;

/* ------------------------------------------------------------ */

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


/** ------------------------------------------------------------
 ** Map container type definitions.
 ** ----------------------------------------------------------*/

typedef struct ucl_map_link_t {
  ucl_node_tag_t	node;
  char			avl_status;
  ucl_value_t		key;
  ucl_value_t		val;
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
 ** Binary tree functions: setters.
 ** ----------------------------------------------------------*/

ucl_inline_nonnull void
ucl_btree_set_bro (void * self, void * bro)
{
  ((ucl_node_t)self)->bro = bro;
}
ucl_inline_nonnull void
ucl_btree_set_son (void * self, void * son)
{
  ((ucl_node_t)self)->son = son;
}
ucl_inline_nonnull void
ucl_btree_set_dad (void * self, void * dad)
{
  ((ucl_node_t)self)->dad = dad;
}

/* ------------------------------------------------------------ */

ucl_inline void
ucl_btree_set_dadbro (void * dad, void * bro)
{
  ucl_btree_set_dad(bro, dad);
  ucl_btree_set_bro(dad, bro);
}
ucl_inline void
ucl_btree_set_dadson (void * dad, void * son)
{
  ucl_btree_set_dad(son, dad);
  ucl_btree_set_son(dad, son);
}

/* ------------------------------------------------------------ */

ucl_inline void
ucl_btree_set_dadsonbro (void * dad, void * son, void * bro)
{
  ucl_btree_set_dadbro(dad, bro);
  ucl_btree_set_dadson(dad, son);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Binary tree functions: getters.
 ** ----------------------------------------------------------*/

ucl_inline_pure void *
ucl_btree_ref_bro (void * self)
{
  return ((ucl_node_t)self)->bro;
}
ucl_inline_pure void *
ucl_btree_ref_son (void * self)
{
  return ((ucl_node_t)self)->son;
}
ucl_inline_pure void *
ucl_btree_ref_dad (void * self)
{
  return ((ucl_node_t)self)->dad;
}

/* ------------------------------------------------------------ */

ucl_inline_pure void *
ucl_btree_data (void * self)
{
  return ((ucl_embedded_node_t)self)->data;
}
ucl_inline_pure ucl_bool_t
ucl_btree_is_leaf (void * _self)
{
  ucl_node_t	self = _self;

  return ((NULL == self->bro) %% (NULL == self->son));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Binary tree functions: detaching nodes.
 ** ----------------------------------------------------------*/

ucl_inline void *
ucl_btree_detach_son (void * self)
{
  ucl_node_t	son = ucl_btree_ref_son(self);

  ucl_btree_set_son(self, NULL);
  ucl_btree_set_dad(son,  NULL);
  return son;
}
ucl_inline void *
ucl_btree_detach_bro (void * self)
{
  ucl_node_t	bro = ucl_btree_ref_bro(self);

  ucl_btree_set_bro(self, NULL);
  ucl_btree_set_dad(bro,  NULL);
  return bro;
}
ucl_inline void *
ucl_btree_detach_dad (void * self)
{
  ucl_node_t	dad = ucl_btree_ref_dad(self);

  ucl_btree_set_dad(self, NULL);
  ucl_btree_set_dad(dad,  NULL);
  return dad;
}

/* ------------------------------------------------------------ */

ucl_inline void
ucl_btree_clean (void * self)
{
  ucl_struct_clean(self,ucl_node_tag_t);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Binary tree functions: iteration.
 ** ----------------------------------------------------------*/

ucl_inline_pure void *
ucl_btree_first_inorder (void * root_node)
{
  return ucl_btree_find_leftmost(root_node);
}
ucl_inline_pure void *
ucl_btree_first_inorder_backward (void * root_node)
{
  return ucl_btree_find_rightmost(root_node);
}

/* ------------------------------------------------------------ */

ucl_inline_pure void *
ucl_btree_first_preorder (void * root_node)
{
  return root_node;
}
ucl_inline_pure void *
ucl_btree_first_preorder_backward (void * root_node)
{
  return root_node;
}

/* ------------------------------------------------------------ */

ucl_inline_pure void *
ucl_btree_first_postorder (void * root_node)
{
  return ucl_btree_find_deepest_son(root_node);
}
ucl_inline_pure void *
ucl_btree_first_postorder_backward (void * root_node)
{
  return ucl_btree_find_deepest_bro(root_node);
}

/* ------------------------------------------------------------ */

ucl_inline_pure void *
ucl_btree_first_levelorder (void * root_node)
{
  return root_node;
}
ucl_inline_pure void *
ucl_btree_first_levelorder_backward (void * root_node)
{
  return root_node;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Tree functions: setters.
 ** ----------------------------------------------------------*/

ucl_inline_nonnull void
ucl_tree_set_next (void * self, void * next)
{
  ucl_btree_set_bro(self,next);
}
ucl_inline_nonnull void
ucl_tree_set_son (void * self, void * son)
{
  ucl_btree_set_son(self,son);
}
ucl_inline_nonnull void
ucl_tree_set_prev (void * self, void * prev)
{
  ucl_btree_set_dad(self,prev);
}

/* ------------------------------------------------------------ */

ucl_inline void
ucl_tree_set_nextprev(void * prev, void * next)
{
  ucl_tree_set_next(prev,next);
  ucl_tree_set_prev(next,prev);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** List container types definitions.
 ** ----------------------------------------------------------*/

ucl_inline_pure ucl_node_t
ucl_list_car (ucl_node_t node)
{
  return node->son;
}
ucl_inline_pure ucl_node_t
ucl_list_cdr (ucl_node_t node)
{
  return node->bro;
}
ucl_inline_pure ucl_node_t
ucl_list_first (ucl_node_t node)
{
  return ucl_tree_get_first(node);
}
ucl_inline_pure ucl_node_t
ucl_list_last (ucl_node_t node)
{
  return ucl_tree_get_last(node);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Heap functions.
 ** ----------------------------------------------------------*/

ucl_inline_pure size_t
ucl_heap_size (const ucl_heap_t this)
{
  return this->size;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Circular list functions.
 ** ----------------------------------------------------------*/

ucl_inline void
ucl_circular_backward (ucl_circular_t this, int times)
{
  ucl_circular_forward(this, -times);
}
ucl_inline void
ucl_circular_constructor (ucl_circular_t this)
{
  ucl_struct_clean(this, ucl_circular_t);
}
ucl_inline void
ucl_circular_destructor (ucl_circular_t this)
{
  ucl_struct_clean(this, ucl_circular_t);
}
ucl_inline size_t
ucl_circular_size (ucl_circular_t this)
{
  return this->size;
}
ucl_inline ucl_circular_link_t *
ucl_circular_current (ucl_circular_t this)
{
  return this->current_link;
}

/* ------------------------------------------------------------ */

ucl_inline ucl_value_t
ucl_circular_data (ucl_circular_link_t * link_p)
{
  return link_p->val;
}
ucl_inline ucl_value_t
ucl_circular_getval (ucl_circular_link_t * link_p)
{
  return link_p->val;
}
ucl_inline void
ucl_circular_setval (ucl_circular_link_t * link_p, ucl_value_t newval)
{
  link_p->val = newval;
}
ucl_inline void
ucl_circular_set_compar (ucl_circular_t this, ucl_comparison_t compar)
{
  this->compar = compar;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Map functions.
 ** ----------------------------------------------------------*/

ucl_inline void
ucl_map_extract (ucl_map_t this, ucl_map_link_t * _link)
{
  ucl_map_remove(this, _link);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Vector functions.
 ** ----------------------------------------------------------*/

ucl_inline_pure void *
ucl_vector_front (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, 0);
}
ucl_inline void *
ucl_vector_back (const ucl_vector_t vector)
{
  return ucl_vector_index_to_slot(vector, ucl_vector_last_index(vector));
}
ucl_inline_pure void *
ucl_vector_increment_slot (const ucl_vector_t self, void * slot)
{
  uint8_t *	p = slot;

  return p + self->slot_dimension;
}
ucl_inline_pure void *
ucl_vector_decrement_slot (const ucl_vector_t self, void * slot)
{
  uint8_t *	p = slot;

  return p - self->slot_dimension;
}
ucl_inline_pure int
ucl_vector_equal_range (const ucl_vector_t a, ucl_range_t ra, const ucl_vector_t b, ucl_range_t rb)
{
  return (0 == ucl_vector_compare_range(a, ra, b, rb));
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Hash table functions.
 ** ----------------------------------------------------------*/

ucl_inline_pure size_t
ucl_hash_number_of_buckets (const ucl_hash_table_t self)
{
  return ucl_vector_size(self->buckets);
}
ucl_inline_pure size_t
ucl_hash_size (const ucl_hash_table_t this)
{
  return this->size;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** End.
 ** ----------------------------------------------------------*/

UCL_END_C_DECL

#endif /* __UCL_H */

/* end of file */
