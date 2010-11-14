/*
   Part of: Useless Containers Library
   Contents: graph container
   Date: Sat Apr 16, 2005

   Abstract

      Graph container.

   Copyright (c) 2005-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** --------------------------------------------------------------------
 ** Headers.
 ** -----------------------------------------------------------------*/

#include "internal.h"

/* Prototype of Depth First Search functions. */
typedef void dfs_recurse_t (ucl_graph_dfs_t * search_handle,
			    ucl_graph_node_t * root_p);

/* Registers a node in the DFS. */
static ucl_graph_dfs_item_t * dfs_enter_node (ucl_graph_dfs_t * search_handle,
					      ucl_graph_node_t * node_p);

static void dfs_leave_node (ucl_graph_dfs_t * search_handle,
			    ucl_graph_dfs_item_t * dfs_node);


/** --------------------------------------------------------------------
 ** Link doubly-linked list iterators.
 ** -----------------------------------------------------------------*/

ucl_graph_link_t *
ucl_graph_last_output_link (ucl_graph_link_t * link_p)
{
  while (NULL != link_p->output.next_link_p)
    link_p = link_p->output.next_link_p;
  return link_p;
}
ucl_graph_link_t *
ucl_graph_last_input_link (ucl_graph_link_t * link_p)
{
  while (NULL != link_p->input.next_link_p)
    link_p = link_p->input.next_link_p;
  return link_p;
}

/* ------------------------------------------------------------ */

ucl_graph_link_t *
ucl_graph_first_output_link (ucl_graph_link_t * link_p)
{
  while (NULL != link_p->output.prev_link_p)
    link_p = link_p->output.prev_link_p;
  return link_p;
}
ucl_graph_link_t *
ucl_graph_first_input_link (ucl_graph_link_t * link_p)
{
  while (NULL != link_p->input.prev_link_p)
    link_p = link_p->input.prev_link_p;
  return link_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Link two nodes.
 ** ----------------------------------------------------------*/

void
ucl_graph_link (ucl_graph_node_t * s, ucl_graph_link_t * l, ucl_graph_node_t * d)
{
  ucl_graph_link_t *	link_p;

  l->src_node_p	= s;
  l->dst_node_p	= d;

  ++(s->output.number_of_links);
  ++(d->input.number_of_links);

  link_p = s->output.first_link_p;
  if (NULL == link_p)
    {
      s->output.first_link_p = l;
    }
  else
    {
      link_p = ucl_graph_last_output_link(link_p);
      link_p->output.next_link_p = l;
      l->output.prev_link_p = link_p;
      l->output.next_link_p = NULL;
    }

  link_p = d->input.first_link_p;
  if (NULL == link_p)
    {
      d->input.first_link_p = l;
    }
  else
    {
      link_p = ucl_graph_last_input_link(link_p);
      link_p->input.next_link_p = l;
      l->input.prev_link_p = link_p;
      l->input.next_link_p = NULL;
    }
}

ucl_bool_t
ucl_graph_nodes_are_linked (ucl_graph_node_t * src_p, ucl_graph_node_t * dst_p)
{
  ucl_graph_link_t *	link_p;

  UCL_GRAPH_OUTPUT_LINKS_LOOP(src_p, link_p)
    if (link_p->dst_node_p == dst_p)
      return 1;
  return 0;
}
ucl_bool_t
ucl_graph_nodes_are_connected (ucl_graph_node_t * a, ucl_graph_node_t * b)
{
  ucl_graph_link_t *	link_p;

  UCL_GRAPH_OUTPUT_LINKS_LOOP(a, link_p)
    if (link_p->dst_node_p == b)
      return 1;
  UCL_GRAPH_INPUT_LINKS_LOOP(a, link_p)
    if (link_p->src_node_p == b)
      return 1;
  return 0;
}

/* ------------------------------------------------------------ */

void
ucl_graph_set_next_node (ucl_graph_node_t * node_p, const ucl_graph_node_t * next_p)
{
  node_p->next_node_p = (ucl_graph_node_t *)next_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Unlink two nodes.
 ** ----------------------------------------------------------*/

void
ucl_graph_unlink (ucl_graph_link_t *link_p)
{
  ucl_graph_node_t *	node_p;
  ucl_graph_link_t *	prev_p;
  ucl_graph_link_t *	next_p;


  /* Remove the link from the chain of ingoing links. */
  prev_p = link_p->input.prev_link_p;
  next_p = link_p->input.next_link_p;
  if (prev_p) prev_p->input.next_link_p = next_p;
  if (next_p) next_p->input.prev_link_p = prev_p;

  /* If  this  link  was  the  first  in the  list  of  ingoing  of  the
     destination node: register the new first. */
  node_p = link_p->dst_node_p;
  if (node_p->input.first_link_p == link_p)
    node_p->input.first_link_p = next_p;
  --(node_p->input.number_of_links);

  /* Remove the link from the chain of outgoing links. */
  prev_p = link_p->output.prev_link_p;
  next_p = link_p->output.next_link_p;
  if (prev_p) prev_p->output.next_link_p = next_p;
  if (next_p) next_p->output.prev_link_p = prev_p;

  /* If this  node was the first in  the list of outgoing  of the source
     node: register the new first in the node. */
  node_p = link_p->src_node_p;
  if (node_p->output.first_link_p == link_p)
    node_p->output.first_link_p = next_p;
  --(node_p->output.number_of_links);
}


/** ------------------------------------------------------------
 ** Erase node.
 ** ----------------------------------------------------------*/

static void
ucl_graph_node_reset_link_fields (ucl_graph_node_t * node_p)
{
  memset(&(node_p->input),  '\0', sizeof(ucl_graph_link_list_base_t));
  memset(&(node_p->output), '\0', sizeof(ucl_graph_link_list_base_t));
}

void
ucl_graph_erase_node_free_links (ucl_graph_node_t * node_p, void (* destructor)(void *))
{
  ucl_graph_link_t * link_p;


  UCL_GRAPH_FIRST_INPUT_LINK_LOOP(node_p, link_p)
    {
      ucl_graph_unlink(link_p);
      destructor(link_p);
    }

  UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(node_p, link_p)
    {
      ucl_graph_unlink(link_p);
      destructor(link_p);
    }

  ucl_graph_node_reset_link_fields(node_p);
}
ucl_link_t *
ucl_graph_erase_node_return_links (ucl_graph_node_t * node_p)
{
  ucl_graph_link_t *	link_p;
  ucl_link_t *		first_list_link_p = NULL;
  ucl_link_t *		list_link_p       = NULL;


  UCL_GRAPH_FIRST_INPUT_LINK_LOOP(node_p, link_p)
    {
      ucl_graph_unlink(link_p);
      if (NULL == first_list_link_p)
	first_list_link_p = (ucl_link_t *)link_p;
      else
	{
	  list_link_p->next_p = (ucl_link_t *)link_p;
	  list_link_p         = (ucl_link_t *)link_p;
	}
    }

  UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(node_p, link_p)
    {
      ucl_graph_unlink(link_p);
      if (NULL == first_list_link_p)
	first_list_link_p = (ucl_link_t *)link_p;
      else
	{
	  list_link_p->next_p = (ucl_link_t *)link_p;
	  list_link_p         = (ucl_link_t *)link_p;
	}
    }

  ucl_graph_node_reset_link_fields(node_p);
  return first_list_link_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Node list.
 ** ----------------------------------------------------------*/

ucl_graph_node_t *
ucl_graph_remove_next_node (ucl_graph_node_t * node_p)
{
  ucl_graph_node_t *	next_p = node_p->next_node_p;

  if (NULL != next_p)
    node_p->next_node_p = next_p->next_node_p;
  return next_p;
}

void
ucl_graph_insert_next_node (ucl_graph_node_t * node_p, ucl_graph_node_t * next_p)
{
  ucl_graph_node_t *	old_next_p = node_p->next_node_p;

  node_p->next_node_p = next_p;
  if (NULL != next_p)
    next_p->next_node_p = old_next_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Merging links.
 ** ----------------------------------------------------------*/

void
ucl_graph_merge_upon_input_link (ucl_graph_link_t * in, ucl_graph_link_t * out)
{
  ucl_graph_link_t *	prev_p;
  ucl_graph_link_t *	next_p;


  /* Remove 'in' from the list of ingoing links of the middle node. */
  prev_p = in->input.prev_link_p;
  next_p = in->input.prev_link_p;
  if (prev_p) prev_p->input.next_link_p = next_p;
  if (next_p) next_p->input.prev_link_p = prev_p;

  /* Remove 'out' from the list of outgoing links of the middle node. */
  prev_p = out->output.prev_link_p;
  next_p = out->output.next_link_p;
  if (prev_p) prev_p->output.next_link_p = next_p;
  if (next_p) next_p->output.prev_link_p = prev_p;

  /* Merge upon 'in'. */
  in->dst_node_p = out->dst_node_p;
  in->input.prev_link_p = out->input.prev_link_p;
  in->input.next_link_p = out->input.next_link_p;
}

void
ucl_graph_merge_upon_output_link (ucl_graph_link_t * in, ucl_graph_link_t * out)
{
  ucl_graph_link_t *	prev_p;
  ucl_graph_link_t *	next_p;


  /* Remove 'in' from the list of ingoing links of the middle node. */
  prev_p = in->input.prev_link_p;
  next_p = in->input.prev_link_p;
  if (prev_p) prev_p->input.next_link_p = next_p;
  if (next_p) next_p->input.prev_link_p = prev_p;

  /* Remove 'out' from the list of outgoing links of the middle node. */
  prev_p = out->output.prev_link_p;
  next_p = out->output.next_link_p;
  if (prev_p) prev_p->output.next_link_p = next_p;
  if (next_p) next_p->output.prev_link_p = prev_p;

  /* Merge upon 'out'. */
  out->dst_node_p = in->dst_node_p;
  out->input.prev_link_p = in->input.prev_link_p;
  out->input.next_link_p = in->input.next_link_p;
}


/** ------------------------------------------------------------
 ** Depth first search.
 ** ----------------------------------------------------------*/

void
ucl_graph_initialise_dfs_handle (ucl_graph_dfs_t * search_handle, ucl_vector_t visited_nodes)
{
  search_handle->counter	= 0;
  search_handle->visited_nodes	= visited_nodes;
}
void
ucl_graph_finalise_dfs_handle (ucl_graph_dfs_t * search_handle)
{
  ucl_vector_tag_t *	items = search_handle->visited_nodes;
  ucl_iterator_t	iterator;
  ucl_graph_dfs_item_t *item;
  ucl_value_t		mark = { .t_int = 0 };
  for (ucl_vector_iterator_forward(items, iterator);
       ucl_iterator_more(iterator);
       ucl_iterator_next(iterator)) {
    item = ucl_iterator_ptr(iterator);
    ucl_graph_node_set_mark(item->node_p, mark);
  }
}
void
ucl_graph_directed_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p)
{
  ucl_graph_link_t *	link_p;
  ucl_graph_dfs_item_t *dfs_node;


  /* Test  if  the  node  is  marked,  that  is:  it  has  been  already
     visited. */
  if (ucl_graph_node_get_mark(root_p).t_int) return;

  dfs_node = dfs_enter_node(search_handle, root_p);
  {
    UCL_GRAPH_OUTPUT_LINKS_LOOP(root_p, link_p)
      ucl_graph_directed_depth_first_search(search_handle, link_p->dst_node_p);
  }
  dfs_leave_node(search_handle, dfs_node);
}
void
ucl_graph_depth_first_search (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * root_p)
{
  ucl_graph_link_t *	link_p;
  ucl_graph_dfs_item_t *dfs_node;


  /* Test  if  the  node  is  marked,  that  is:  it  has  been  already
     visited. */
  if (ucl_graph_node_get_mark(root_p).t_int) return;
  dfs_node = dfs_enter_node(search_handle, root_p);
  {
    UCL_GRAPH_OUTPUT_LINKS_LOOP(root_p, link_p)
      ucl_graph_depth_first_search(search_handle, link_p->dst_node_p);

    UCL_GRAPH_INPUT_LINKS_LOOP(root_p, link_p)
      ucl_graph_depth_first_search(search_handle, link_p->src_node_p);
  }
  dfs_leave_node(search_handle, dfs_node);
}

/* ------------------------------------------------------------ */

static ucl_graph_dfs_item_t *
dfs_enter_node (ucl_graph_dfs_t * search_handle, ucl_graph_node_t * node_p)
{
  ucl_graph_dfs_item_t *	dfs_node;


  node_p->mark.t_int = 1;
  /* Remember  that  'ucl_vector_push_back()'  is  a high  level  vector
     function that calls 'ucl_vector_enlarge()'. */
  dfs_node = ucl_vector_push_back(search_handle->visited_nodes);
  dfs_node->node_p = node_p;
  dfs_node->in_counter = (search_handle->counter)++;
  return dfs_node;
}
static void
dfs_leave_node (ucl_graph_dfs_t * search_handle, ucl_graph_dfs_item_t * dfs_node)
{
  dfs_node->out_counter = (search_handle->counter)++;
}

/* end of file */
