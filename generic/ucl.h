/* ucl.h  --

   Part of: Useless Container Library
   Contents: common types and symbol definitions.
   Date: late 2001

   Abstract

	This file must be included in every module that makes use of the
	UCL containers.

   Copyright (c) 2001, 2002, 2003 Marco Maggi

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


#ifndef __UCL_H
#define __UCL_H 1


/** ------------------------------------------------------------
 ** Common declarations.
 ** ----------------------------------------------------------*/

/*
  Begin  and  end  C  declarations.  Useful when  building  with  a  C++
  compiler.
*/

#undef UCL_BEGIN_C_DECL
#undef UCL_END_C_DECL
#ifdef __cplusplus
#  define UCL_BEGIN_C_DECL		extern "C" {
#  define UCL_END_C_DECL		}
#else
#  define UCL_BEGIN_C_DECL		/* empty */
#  define UCL_END_C_DECL		/* empty */
#endif


/*
  Removes arguments  from prototypes for  compilers that do  not support
  them.

  Usage example:

		extern void fun UCL_ARGS((int a, int b));
*/

#undef UCL_ARGS
#if defined (__STDC__) || defined (_AIX) \
             || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
             || defined(WIN32) || defined(__cplusplus)
# define UCL_ARGS(protos)	protos
#else
# define UCL_ARGS(protos)	()
#endif


/*
  Some compilers do not support the "void *" type.
*/

#ifdef UCL_CONFIG_NO_VOID_P
#  define ucl_voidptr_t		unsigned char *
#else
#  define ucl_voidptr_t		void *
#endif


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

UCL_BEGIN_C_DECL


/** ------------------------------------------------------------
 ** Defines.
 ** ----------------------------------------------------------*/

/* UCL_CUSTOM_ALLOCATOR --
 
   Control  memory  allocation:  if  it's  defined we  have  to  provide
   definitions for the macros/functions:
 
	void *ucl_malloc (Index size);
 	void ucl_free (void *buf);
 	void *ucl_realloc (void *buf, Index size);
*/

#ifndef UCL_CUSTOM_ALLOCATOR
#  include <stdlib.h>
#  define ucl_malloc(block)		malloc(block)
#  define ucl_free(block)		free(block)
#  define ucl_realloc(block,size)	realloc(block,size)
#endif



/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef union ucl_value_t {
  unsigned	unum;
  int		num;
  int		integer;
  void *	ptr;
  char *	str;
} ucl_value_t;

typedef int	ucl_valcmp_t	UCL_ARGS((const ucl_value_t a,
					  const ucl_value_t b));
typedef int	ucl_voidcmp_t	UCL_ARGS((const ucl_voidptr_t a,
					  const ucl_voidptr_t b));
typedef size_t	ucl_hashfun_t	UCL_ARGS((ucl_value_t key));

extern ucl_valcmp_t	ucl_intcmp;


typedef struct ucl_iterator_t {
  ucl_voidptr_t		iterator;
  const ucl_voidptr_t	container;
  void			(*next)	UCL_ARGS((struct ucl_iterator_t *iterator));

  ucl_voidptr_t	ptr1;
  ucl_voidptr_t	ptr2;
} ucl_iterator_t;


typedef struct ucl_btree_node_t {
  struct ucl_btree_node_t *	dadPtr;
  struct ucl_btree_node_t *	broPtr;
  struct ucl_btree_node_t *	sonPtr;
} ucl_btree_node_t;


typedef struct ucl_circular_link_t {
  struct ucl_circular_link_t *	next;
  struct ucl_circular_link_t *	prev;
  ucl_value_t			val;
} ucl_circular_link_t;


typedef struct ucl_circular_t {
  ucl_circular_link_t *		current_link;
  size_t			size;
} ucl_circular_t;


typedef struct ucl_hash_entry_t {
  struct ucl_hash_entry_t *	nxtPtr;
  ucl_value_t			key;
  ucl_value_t			val;
  unsigned char			erased;
} ucl_hash_entry_t;


typedef struct ucl_hash_t {
  ucl_hash_entry_t **		buckets;
  ucl_hashfun_t *		hash;
  ucl_valcmp_t *		compar;
  size_t			size;
  size_t			dim;
  size_t			used;
} ucl_hash_t;


typedef struct ucl_heap_node_t {
  struct ucl_heap_node_t *	dadPtr;
  struct ucl_heap_node_t *	broPtr;
  struct ucl_heap_node_t *	sonPtr;
  ucl_value_t			val;
} ucl_heap_node_t;


typedef struct ucl_heap_t {
  size_t			size;
  ucl_heap_node_t *		rootPtr;
  ucl_heap_node_t *		nextPtr;
  int				state;
  ucl_valcmp_t *		valcmp;
} ucl_heap_t;


typedef struct ucl_list_link_t {
  struct ucl_list_link_t *	next;
  struct ucl_list_link_t *	prev;
  struct ucl_list_link_t *	dummy;
  ucl_value_t			val;
} ucl_list_link_t;


typedef struct ucl_list_t {
  size_t		size;	/* number of allocated nodes */

  ucl_list_link_t *	front;	/* pointer to the first node */
  ucl_list_link_t *	back;	/* pointer to the last node */

  size_t		li;	/* the index of the last accessed node */
  ucl_list_link_t *	ll;	/* pointer to the last accessed node */
} ucl_list_t;


typedef struct ucl_map_link_t {
  ucl_btree_node_t 	node;
  char			avl_status;
  ucl_value_t		key;
  ucl_value_t		val;
} ucl_map_link_t;


typedef struct ucl_map_t {
  size_t			size;
  unsigned int			flags;
  ucl_valcmp_t *		keycmp;
  ucl_map_link_t *		root;
} ucl_map_t;


typedef struct ucl_string_t {
  size_t	len;
  size_t	size;
  char *	ptr;
  char		data[1];
} ucl_string_t;


typedef struct ucl_strtok_t {
  size_t	len;
  size_t	size;
  char *	ptr;
} ucl_strtok_t;


typedef struct ucl_tree_node_t {
  struct ucl_tree_node_t *	dadPtr;
  struct ucl_tree_node_t *	broPtr;
  struct ucl_tree_node_t *	sonPtr;
} ucl_tree_node_t;


typedef struct ucl_vector_t {
  size_t	size;
  size_t	step_up;
  size_t	step_down;
  size_t	pad;
  size_t	dim;

  unsigned char *	min_front;	/* ptr to data area */
  unsigned char *	front;		/* ptr to the first used slot */
  unsigned char *	back;		/* ptr to the last used slot + 1 */
  unsigned char *	max_back;	/* ptr to the last allocated slot + 1 */
} ucl_vector_t;


/** ------------------------------------------------------------
 ** Function prototypes.
 ** ----------------------------------------------------------*/

extern const char *	ucl_version	UCL_ARGS((void));



/** ------------------------------------------------------------
 ** Binary tree container prototypes.
 ** ----------------------------------------------------------*/

extern void ucl_btree_constructor UCL_ARGS((ucl_btree_node_t *nodePtr,
					    ucl_btree_node_t *dadPtr));
#define	ucl_btree_destructor(this)

#define ucl_btree_init(node)	(memset(node,'\0',sizeof(ucl_btree_node_t)))


#define ucl_btree_setbro(this,newBro)		((this)->broPtr)=(newBro)
#define ucl_btree_setson(this,newSon)		((this)->sonPtr)=(newSon)
#define ucl_btree_setdad(this,newDad)		((this)->dadPtr)=(newDad)

#define ucl_btree_getbro(this)			((this)->broPtr)
#define ucl_btree_getson(this)			((this)->sonPtr)
#define ucl_btree_getdad(this)			((this)->dadPtr)

#define ucl_btree_dadbro(dad,bro) (dad)->broPtr=(bro);(bro)->dadPtr=(dad)
#define ucl_btree_dadson(dad,son) (dad)->sonPtr=(son);(son)->dadPtr=(dad)


extern ucl_btree_node_t *ucl_btree_find_leftmost	\
				UCL_ARGS((ucl_btree_node_t *nodePtr));
extern ucl_btree_node_t *ucl_btree_find_rightmost	\
				UCL_ARGS((ucl_btree_node_t *nodePtr));
extern ucl_btree_node_t *ucl_btree_find_deepest_son	\
				UCL_ARGS((ucl_btree_node_t *nodePtr));


extern ucl_btree_node_t *ucl_btree_step_inorder		\
				UCL_ARGS((ucl_btree_node_t *nodePtr));
extern ucl_btree_node_t *ucl_btree_step_inorder_backward	\
				UCL_ARGS((ucl_btree_node_t *nodePtr));
extern ucl_btree_node_t *ucl_btree_step_preorder		\
				UCL_ARGS((ucl_btree_node_t *nodePtr));
extern ucl_btree_node_t *ucl_btree_step_postorder		\
				UCL_ARGS((ucl_btree_node_t *nodePtr));
extern ucl_btree_node_t *ucl_btree_step_levelorder		\
				UCL_ARGS((ucl_btree_node_t *nodePtr));


/** ------------------------------------------------------------
 ** Circular list container prototypes.
 ** ----------------------------------------------------------*/

extern void	ucl_circular_constructor UCL_ARGS((ucl_circular_t *this));
extern void	ucl_circular_destructor  UCL_ARGS((ucl_circular_t *this));


extern void ucl_circular_insert	UCL_ARGS((ucl_circular_t *this,
					  ucl_circular_link_t *link_p));
extern ucl_circular_link_t *ucl_circular_extract	\
				 UCL_ARGS((ucl_circular_t *this));

#define   ucl_circular_size(this)		((this)->size)
#define	  ucl_circular_current(this)		((this)->current_link)
#define   ucl_circular_data(link_p)		((link_p)->val)
#define   ucl_circular_getval(link_p)		((link_p)->val)
#define   ucl_circular_setval(link_p, newval)	((link_p)->val)=(newval)

extern void ucl_circular_forward  UCL_ARGS((ucl_circular_t *this,
					    size_t times));
extern void ucl_circular_backward UCL_ARGS((ucl_circular_t *this,
					    size_t times));

#define ucl_circular_stepforw(this)	\
			((this)->current_link)=((this)->current_link->next)
#define ucl_circular_stepback(this)	\
			((this)->current_link)=((this)->current_link->prev)

extern ucl_circular_link_t * ucl_circular_find		\
				UCL_ARGS((ucl_circular_t *this,
					  ucl_value_t val,
					  ucl_valcmp_t *valcmp));


/** ------------------------------------------------------------
 ** Hash table container prototypes.
 ** ----------------------------------------------------------*/

extern ucl_voidptr_t ucl_hash_constructor	\
				UCL_ARGS((ucl_hash_t *this,
					  size_t dim,
					  ucl_valcmp_t *keycmp,
					  ucl_hashfun_t *hash));

extern void ucl_hash_destructor	UCL_ARGS((ucl_hash_t *this));

extern void ucl_hash_insert	UCL_ARGS((ucl_hash_t *this,
					  ucl_hash_entry_t *entPtr));
extern void ucl_hash_extract	UCL_ARGS((ucl_hash_t *this,
					  ucl_hash_entry_t *entPtr));

extern ucl_hash_entry_t * ucl_hash_find	\
				UCL_ARGS((ucl_hash_t *this, ucl_value_t key));

#define ucl_hash_size(this)	((this)->size)

extern ucl_voidptr_t ucl_hash_enlarge	\
				UCL_ARGS((ucl_hash_t *this, size_t newsize));

#define ucl_hash_setkey(entPtr, newkey)	((entPtr)->key)=(newkey)
#define ucl_hash_getkey(entPtr)		((entPtr)->key)
#define ucl_hash_setval(entPtr, newval)	((entPtr)->val)=(newval)
#define ucl_hash_getval(entPtr)		((entPtr)->val)

extern void ucl_hash_iterator	UCL_ARGS((ucl_hash_t *this,
					  ucl_iterator_t *iterator));

extern ucl_hashfun_t	ucl_hash_string;


/** ------------------------------------------------------------
 ** Heap container prototypes.
 ** ----------------------------------------------------------*/

extern void ucl_heap_constructor	UCL_ARGS((ucl_heap_t *this,
						  ucl_valcmp_t *valcmp));
extern void ucl_heap_destructor		UCL_ARGS((ucl_heap_t *this));

extern void ucl_heap_insert		UCL_ARGS((ucl_heap_t *this,
						  ucl_heap_node_t *node));

extern ucl_heap_node_t * ucl_heap_extract	\
					 UCL_ARGS((ucl_heap_t *this));

#define ucl_heap_size(this)			((this)->size)
#define ucl_heap_setval(nodePtr, newval)	((nodePtr)->val)=(newval)
#define ucl_heap_getval(nodePtr)		((nodePtr)->val)


/** ------------------------------------------------------------
 ** List container prototypes.
 ** ----------------------------------------------------------*/

extern void ucl_list_constructor	UCL_ARGS((ucl_list_t *this));
extern void ucl_list_destructor		UCL_ARGS((ucl_list_t *this));

extern ucl_list_link_t * ucl_list_index	UCL_ARGS((ucl_list_t *this,
						  size_t index));

#define		ucl_list_front(this)	((this)->front)
#define		ucl_list_back(this)	((this)->back)

extern void ucl_list_insertbefore	UCL_ARGS((ucl_list_t *this,
						  ucl_list_link_t *linkPtr,
						  ucl_list_link_t *newPtr));
extern void ucl_list_insertafter	UCL_ARGS((ucl_list_t *this,
						 ucl_list_link_t *linkPtr,
						 ucl_list_link_t *newPtr));
extern void ucl_list_pushfront	UCL_ARGS((ucl_list_t *this,
					  ucl_list_link_t *linkPtr));
extern void ucl_list_pushback	UCL_ARGS((ucl_list_t *this,
					  ucl_list_link_t *linkPtr));
extern void ucl_list_extract	UCL_ARGS((ucl_list_t *this,
					  ucl_list_link_t *linkPtr));

#define ucl_list_popfront(this)	ucl_list_extract((this),(this)->front)
#define ucl_list_popback(this)	ucl_list_extract((this),(this)->back)

#define ucl_list_next(linkPtr)	((linkPtr)->next)
#define ucl_list_prev(linkPtr)	((linkPtr)->prev)

#define ucl_list_size(this)			((this)->size)
#define	ucl_list_getval(linkPtr)		((linkPtr)->val)
#define	ucl_list_setval(linkPtr, newval)	((linkPtr)->val)=(newval)



/** ------------------------------------------------------------
 ** Map container prototypes.
 ** ----------------------------------------------------------*/

#define UCL_ALLOW_MULTIPLE_OBJECTS	0x10

extern void ucl_map_constructor	UCL_ARGS((ucl_map_t *this, unsigned int flags,
					  ucl_valcmp_t *keycmp));
extern void ucl_map_destructor	UCL_ARGS((ucl_map_t *this));

extern void ucl_map_insert	UCL_ARGS((ucl_map_t *this,
					  ucl_map_link_t *linkPtr));

extern ucl_map_link_t *ucl_map_remove	UCL_ARGS((ucl_map_t *this,
						  ucl_map_link_t *linkPtr));

#define ucl_map_extract(this,link)  ucl_map_remove(this,link)

extern size_t ucl_map_count		UCL_ARGS((const ucl_map_t *this,
						  const ucl_value_t key));
extern ucl_map_link_t *	ucl_map_find	UCL_ARGS((const ucl_map_t *this,
						  const ucl_value_t key));

extern ucl_map_link_t *	ucl_map_first	UCL_ARGS((const ucl_map_t *this));
extern ucl_map_link_t *	ucl_map_last	UCL_ARGS((const ucl_map_t *this));

extern ucl_map_link_t *	ucl_map_next	UCL_ARGS((const ucl_map_link_t *linkPtr));
extern ucl_map_link_t *	ucl_map_prev	UCL_ARGS((const ucl_map_link_t *linkPtr));

extern ucl_map_link_t *	ucl_map_find_or_next UCL_ARGS((const ucl_map_t *this,
						       const ucl_value_t key));
extern ucl_map_link_t *	ucl_map_find_or_prev UCL_ARGS((const ucl_map_t *this,
						       const ucl_value_t key));

#define ucl_map_size(this)		((this)->size)

#define ucl_map_getkey(linkPtr)		((linkPtr)->key)
#define ucl_map_getval(linkPtr)		((linkPtr)->val)

#define ucl_map_setkey(linkPtr, newkey)	((linkPtr)->key)=(newkey)
#define ucl_map_setval(linkPtr, newval)	((linkPtr)->val)=(newval)


extern void ucl_map_iterator_inorder	UCL_ARGS((const ucl_map_t *this,
						  ucl_iterator_t *iterator));
extern void ucl_map_iterator_preorder	UCL_ARGS((const ucl_map_t *this,
						  ucl_iterator_t *iterator));
extern void ucl_map_iterator_postorder	UCL_ARGS((const ucl_map_t *this,
						  ucl_iterator_t *iterator));
extern void ucl_map_iterator_levelorder UCL_ARGS((const ucl_map_t *this,
						  ucl_iterator_t *iterator));
extern void ucl_map_upper_bound	UCL_ARGS((const ucl_map_t *this,
					  ucl_iterator_t *iterator,
					  const ucl_value_t key));
extern void ucl_map_lower_bound	UCL_ARGS((const ucl_map_t *this,
					  ucl_iterator_t *iterator,
					  const ucl_value_t key));

extern void ucl_map_iterator_inorder_backward	\
					 UCL_ARGS((const ucl_map_t *this,
						   ucl_iterator_t *iterator));

extern void ucl_map_iterator_union	UCL_ARGS((ucl_iterator_t *iter1,
						  ucl_iterator_t *iter2,
						  ucl_iterator_t *iterator));
extern void ucl_map_iterator_intersection	\
					 UCL_ARGS((ucl_iterator_t *iter1,
						   ucl_iterator_t *iter2,
						   ucl_iterator_t *iterator));
extern void ucl_map_iterator_complintersect	\
					 UCL_ARGS((ucl_iterator_t *iter1,
						   ucl_iterator_t *iter2,
						   ucl_iterator_t *iterator));
extern void ucl_map_iterator_subtraction	\
					 UCL_ARGS((ucl_iterator_t *iter1,
						   ucl_iterator_t *iter2,
						   ucl_iterator_t *iterator));


/** ------------------------------------------------------------
 ** String container prototypes.
 ** ----------------------------------------------------------*/

#define	UCL_STRING_STEP_UP		16

extern ucl_string_t *	ucl_string_constructor	UCL_ARGS((size_t size));
extern ucl_string_t *	ucl_string_duplicate	UCL_ARGS((ucl_string_t *this));
#define ucl_string_destructor(this)	ucl_free(this)

extern void ucl_string_insert	UCL_ARGS((ucl_string_t *this,
					  size_t index,
					  const char c));
extern void ucl_string_erase	UCL_ARGS((ucl_string_t *this,
					  size_t index));

extern void ucl_string_pushback	UCL_ARGS((ucl_string_t *this, char c));
extern void ucl_string_pushfront UCL_ARGS((ucl_string_t *this, char c));
extern char ucl_string_popback	UCL_ARGS((ucl_string_t *this));
extern char ucl_string_popfront	UCL_ARGS((ucl_string_t *this));

#define ucl_string_len(this)		((this)->len)
#define ucl_string_size(this)		((this)->size)
#define ucl_string_data(this)		((this)->ptr)

#define ucl_string_setval(this, index, c) ((this)->ptr[index])=(c)
#define ucl_string_getval(this, index)	((this)->ptr[index])
#define ucl_string_front(this)		((this)->ptr[0])
#define ucl_string_back(this)		((this)->ptr[((this)->len)-1])

extern void ucl_string_randomise	UCL_ARGS((ucl_string_t *this));
extern void ucl_string_swap		UCL_ARGS((ucl_string_t *this,
						  size_t a, size_t b));
#define ucl_string_compare(one, two)	(strcmp((one)->ptr, (two)->ptr))

/*
extern void		ucl_string_concat	(ucl_string_t *this, ...);
extern ucl_string_t *	ucl_string_strcat	(ucl_string_t *this, ...);
extern ucl_string_t *	ucl_string_prepare	(ucl_string_t *this, ucl_string_t **str);
*/

#define ucl_string_isfull(this)		((this)->len == (this)->size-1)
extern ucl_string_t *ucl_string_resize	UCL_ARGS((ucl_string_t *this, size_t size));
extern ucl_string_t *ucl_string_enlarge	UCL_ARGS((ucl_string_t *this));
extern ucl_string_t *ucl_string_restrict UCL_ARGS((ucl_string_t *this));


/** ------------------------------------------------------------
 ** String token container prototypes.
 ** ----------------------------------------------------------*/

#ifndef UCL_STRTOK_STEP_UP
#  define UCL_STRTOK_STEP_UP		16
#endif

extern char *ucl_strtok_constructor	UCL_ARGS((ucl_strtok_t *this,
						  size_t size));
extern void ucl_strtok_destructor	UCL_ARGS((ucl_strtok_t *this));
extern char *ucl_strtok_duplicate	UCL_ARGS((ucl_strtok_t *dst,
						  ucl_strtok_t *src));

extern void ucl_strtok_insert	UCL_ARGS((ucl_strtok_t *this,
					  size_t index, char c));
extern void ucl_strtok_erase	UCL_ARGS((ucl_strtok_t *this,
					  size_t index));

extern void ucl_strtok_pushback	UCL_ARGS((ucl_strtok_t *this, char c));
extern void ucl_strtok_pushfront UCL_ARGS((ucl_strtok_t *this, char c));
extern char ucl_strtok_popback	UCL_ARGS((ucl_strtok_t *this));
extern char ucl_strtok_popfront	UCL_ARGS((ucl_strtok_t *this));

#define ucl_strtok_len(this)		((this)->len)
#define ucl_strtok_size(this)		((this)->size)
#define ucl_strtok_data(this)		((this)->ptr)

#define ucl_strtok_setval(this, index, c) ((this)->ptr[index])=(c)
#define ucl_strtok_getval(this, index)	((this)->ptr[index])
#define ucl_strtok_front(this)		((this)->ptr[0])
#define ucl_strtok_back(this)		((this)->ptr[(this)->len-1])

#define ucl_strtok_compare(one, two) (strcmp(one->ptr, two->ptr))
extern void ucl_strtok_randomise	UCL_ARGS((ucl_strtok_t *this));
extern void ucl_strtok_swap		UCL_ARGS((ucl_strtok_t *this,
						  size_t a, size_t b));

#define ucl_strtok_isfull(this)	((this)->len == (this)->size)
extern char *ucl_strtok_resize		UCL_ARGS((ucl_strtok_t *this,
						  size_t size));
extern char *ucl_strtok_enlarge		UCL_ARGS((ucl_strtok_t *this));
extern char *ucl_strtok_restrict	UCL_ARGS((ucl_strtok_t *this));

/*
char *	ucl_strtok_concatva	(ucl_strtok_t *this, ...);
void	ucl_strtok_strcatva	(ucl_strtok_t *this, ...);
void	ucl_strtok_strcatnil	(ucl_strtok_t *this, char *str[]);
*/


/** ------------------------------------------------------------
 ** Tree container prototypes.
 ** ----------------------------------------------------------*/

#define ucl_tree_constructor(node)  memset(node,'\0',sizeof(ucl_tree_node_t))

extern int ucl_tree_is_dad	UCL_ARGS((ucl_tree_node_t *dadPtr,
					  ucl_tree_node_t *cldPtr));
extern int ucl_tree_is_bro	UCL_ARGS((ucl_tree_node_t *nodPtr,
					  ucl_tree_node_t *broPtr));

extern int ucl_tree_has_dad	UCL_ARGS((ucl_tree_node_t *this));
extern int ucl_tree_has_prev	UCL_ARGS((ucl_tree_node_t *this));
#define ucl_tree_has_next(this)	((this)->broPtr)
#define ucl_tree_has_son(this)	((this)->sonPtr)

extern ucl_tree_node_t * ucl_tree_get_dad UCL_ARGS((ucl_tree_node_t *this));
extern ucl_tree_node_t * ucl_tree_get_prev UCL_ARGS((ucl_tree_node_t *this));
#define	ucl_tree_get_next(nodPtr)	((nodPtr)->broPtr)
#define ucl_tree_get_son(nodPtr)	((nodPtr)->sonPtr)
extern ucl_tree_node_t * ucl_tree_get_first UCL_ARGS((ucl_tree_node_t *nodPtr));
extern ucl_tree_node_t * ucl_tree_get_last UCL_ARGS((ucl_tree_node_t *nodPtr));

#define ucl_tree_set_son(nod,son) ((nod)->sonPtr)=(son);((son)->dadPtr)=(nod)
#define ucl_tree_set_bro(nod,bro) ((nod)->broPtr)=(bro);((bro)->dadPtr)=(nod)

extern void ucl_tree_insert_dad	UCL_ARGS((ucl_tree_node_t *nodPtr,
					  ucl_tree_node_t *dadPtr));
extern void ucl_tree_insert_son	UCL_ARGS((ucl_tree_node_t *nodPtr,
					  ucl_tree_node_t *sonPtr));
extern void ucl_tree_insert_prev UCL_ARGS((ucl_tree_node_t *nodPtr,
					   ucl_tree_node_t *broPtr));
extern void ucl_tree_insert_next UCL_ARGS((ucl_tree_node_t *nodPtr,
					   ucl_tree_node_t *broPtr));

extern ucl_tree_node_t * ucl_tree_extract_dad	\
					UCL_ARGS((ucl_tree_node_t *nodPtr));
extern ucl_tree_node_t * ucl_tree_extract_son	\
					UCL_ARGS((ucl_tree_node_t *nodPtr));
extern ucl_tree_node_t * ucl_tree_extract_prev	\
					UCL_ARGS((ucl_tree_node_t *nodPtr));
extern ucl_tree_node_t * ucl_tree_extract_next	\
					UCL_ARGS((ucl_tree_node_t *nodPtr));

extern void ucl_tree_iterator_inorder   UCL_ARGS((ucl_tree_node_t *nodPtr,
						  ucl_iterator_t *iter));
extern void ucl_tree_iterator_preorder	UCL_ARGS((ucl_tree_node_t *nodPtr,
						  ucl_iterator_t *iter));
extern void ucl_tree_iterator_postorder UCL_ARGS((ucl_tree_node_t *nodPtr,
						  ucl_iterator_t *iter));


/** ------------------------------------------------------------
 ** Vector container prototypes.
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


#define ucl_vector_constructor(this, dim) \
   ucl_vector_constructor1 ((this), \
			    UCL_VECTOR_DEFAULT_SIZE, \
			    UCL_VECTOR_DEFAULT_STEP_UP, \
                            UCL_VECTOR_DEFAULT_STEP_DOWN, \
                            UCL_VECTOR_DEFAULT_PAD, (dim))

extern ucl_voidptr_t	ucl_vector_constructor1		\
				UCL_ARGS((ucl_vector_t *this,
					  size_t size,
					  size_t step_up,
					  size_t step_down,
					  size_t pad,
					  size_t dim));

extern void ucl_vector_destructor	UCL_ARGS((ucl_vector_t *this));

extern ucl_voidptr_t ucl_vector_insert	UCL_ARGS((ucl_vector_t *this,
						  ucl_voidptr_t target));
extern void ucl_vector_erase		UCL_ARGS((ucl_vector_t *this,
						  ucl_voidptr_t target));

extern ucl_voidptr_t ucl_vector_insertsort	\
					UCL_ARGS((ucl_vector_t *this,
						  ucl_voidptr_t elmPtr,
						  ucl_voidcmp_t *compar));

extern ucl_voidptr_t ucl_vector_index	UCL_ARGS((ucl_vector_t *this,
						  size_t index));
extern ucl_voidptr_t ucl_vector_newindex UCL_ARGS((ucl_vector_t *this,
						   size_t index));

#define ucl_vector_front(this)	ucl_vector_index((this), 0)
#define ucl_vector_back(this)	ucl_vector_index((this), (this)->size-1)


extern void ucl_vector_qsort	UCL_ARGS((ucl_vector_t *this,
					  ucl_voidcmp_t compar));
#define ucl_vector_size(this)	((this)->size)

extern ucl_voidptr_t ucl_vector_enlarge	UCL_ARGS((ucl_vector_t *this));
extern ucl_voidptr_t ucl_vector_restrict UCL_ARGS((ucl_vector_t *this));

extern ucl_voidptr_t ucl_vector_find	UCL_ARGS((ucl_vector_t *this,
						  ucl_voidptr_t elmPtr,
						  ucl_voidcmp_t *compar));

extern ucl_voidptr_t ucl_vector_sortfind UCL_ARGS((ucl_vector_t *this,
						   ucl_voidptr_t elmPtr,
						   ucl_voidcmp_t *compar));


/** ------------------------------------------------------------
 ** Generic iteration macros.
 ** ----------------------------------------------------------*/

#define ucl_iterator_more(iterPtr)	((iterPtr)->iterator != 0)
#define ucl_iterator_next(iterPtr)	((iterPtr)->next(iterPtr))
#define ucl_iterator_ptr(iterPtr)	((iterPtr)->iterator)


UCL_END_C_DECL
#endif /* __UCL_H */


/* end of file */
