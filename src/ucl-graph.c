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

#define DEBUGGING		0
#include "internal.h"


/** --------------------------------------------------------------------
 ** Link doubly-linked list iterators.
 ** -----------------------------------------------------------------*/

ucl_graph_link_t
ucl_graph_last_output_link (ucl_graph_link_t  link)
{
  while (NULL != link->output.next_link)
    link = link->output.next_link;
  return link;
}
ucl_graph_link_t
ucl_graph_last_input_link (ucl_graph_link_t  link)
{
  while (NULL != link->input.next_link)
    link = link->input.next_link;
  return link;
}
ucl_graph_link_t
ucl_graph_first_output_link (ucl_graph_link_t  link)
{
  while (NULL != link->output.prev_link)
    link = link->output.prev_link;
  return link;
}
ucl_graph_link_t
ucl_graph_first_input_link (ucl_graph_link_t  link)
{
  while (NULL != link->input.prev_link)
    link = link->input.prev_link;
  return link;
}


/** ------------------------------------------------------------
 ** Link two nodes.
 ** ----------------------------------------------------------*/

void
ucl_graph_link (ucl_graph_node_t src, ucl_graph_link_t lnk, ucl_graph_node_t dst)
{
  ucl_graph_link_t 	L;
  lnk->src_node	= src;
  lnk->dst_node	= dst;
  ++(src->output.number_of_links);
  ++(dst->input.number_of_links);
  L = src->output.first_link;
  if (NULL == L) {
    src->output.first_link = lnk;
  } else {
    L = ucl_graph_last_output_link(L);
    L->output.next_link = lnk;
    lnk->output.prev_link = L;
    lnk->output.next_link = NULL;
  }
  L = dst->input.first_link;
  if (NULL == L) {
    dst->input.first_link = lnk;
  } else {
    L = ucl_graph_last_input_link(L);
    L->input.next_link = lnk;
    lnk->input.prev_link = L;
    lnk->input.next_link = NULL;
  }
}


/** --------------------------------------------------------------------
 ** Inspect linked nodes.
 ** ----------------------------------------------------------------- */

ucl_bool_t
ucl_graph_nodes_are_linked (ucl_graph_node_t src, ucl_graph_node_t dst)
{
  ucl_graph_link_t 	link;
  UCL_GRAPH_OUTPUT_LINKS_LOOP(src, link)
    if (link->dst_node == dst)
      return 1;
  return 0;
}
ucl_bool_t
ucl_graph_nodes_are_doubly_linked (ucl_graph_node_t a, ucl_graph_node_t b)
{
  return ucl_graph_nodes_are_linked(a, b) && ucl_graph_nodes_are_linked(b, a);
}
ucl_bool_t
ucl_graph_nodes_are_connected (ucl_graph_node_t a, ucl_graph_node_t b)
{
  return ucl_graph_nodes_are_linked(a, b) || ucl_graph_nodes_are_linked(b, a);
}


/** ------------------------------------------------------------
 ** Unlink two nodes.
 ** ----------------------------------------------------------*/

void
ucl_graph_unlink (ucl_graph_link_t link)
{
  ucl_graph_node_t 	node;
  ucl_graph_link_t 	prev;
  ucl_graph_link_t 	next;
  /* Remove the link from the chain of ingoing links. */
  prev = link->input.prev_link;
  next = link->input.next_link;
  if (prev) prev->input.next_link = next;
  if (next) next->input.prev_link = prev;

  /* If  this  link  was  the  first  in the  list  of  ingoing  of  the
     destination node: register the new first. */
  node = link->dst_node;
  if (node->input.first_link == link)
    node->input.first_link = next;
  --(node->input.number_of_links);

  /* Remove the link from the chain of outgoing links. */
  prev = link->output.prev_link;
  next = link->output.next_link;
  if (prev) prev->output.next_link = next;
  if (next) next->output.prev_link = prev;

  /* If this  node was the first in  the list of outgoing  of the source
     node: register the new first in the node. */
  node = link->src_node;
  if (node->output.first_link == link)
    node->output.first_link = next;
  --(node->output.number_of_links);
}


/** ------------------------------------------------------------
 ** Erase node.
 ** ----------------------------------------------------------*/

static void
ucl_graph_node_reset_link_fields (ucl_graph_node_t N)
{
  memset(&(N->input),  '\0', sizeof(ucl_graph_link_list_base_t));
  memset(&(N->output), '\0', sizeof(ucl_graph_link_list_base_t));
}
void
ucl_graph_erase_node_destroy_links (ucl_graph_node_t N, ucl_callback_t destructor)
{
  ucl_graph_link_t  L;
  UCL_GRAPH_FIRST_INPUT_LINK_LOOP(N, L) {
    ucl_graph_unlink(L);
    ucl_callback_apply(destructor, L);
  }
  UCL_GRAPH_FIRST_OUTPUT_LINK_LOOP(N, L) {
    ucl_graph_unlink(L);
    ucl_callback_apply(destructor, L);
  }
  ucl_graph_node_reset_link_fields(N);
}


/** ------------------------------------------------------------
 ** Merging links.
 ** ----------------------------------------------------------*/

void
ucl_graph_merge_upon_input_link (ucl_graph_link_t in, ucl_graph_link_t out)
{
  ucl_graph_link_t 	prev;
  ucl_graph_link_t 	next;
  /* Remove 'in' from the list of ingoing links of the middle node. */
  prev = in->input.prev_link;
  next = in->input.prev_link;
  if (prev) prev->input.next_link = next;
  if (next) next->input.prev_link = prev;

  /* Remove 'out' from the list of outgoing links of the middle node. */
  prev = out->output.prev_link;
  next = out->output.next_link;
  if (prev) prev->output.next_link = next;
  if (next) next->output.prev_link = prev;

  /* Merge upon 'in'. */
  in->dst_node = out->dst_node;
  in->input.prev_link = out->input.prev_link;
  in->input.next_link = out->input.next_link;
}

void
ucl_graph_merge_upon_output_link (ucl_graph_link_t  in, ucl_graph_link_t  out)
{
  ucl_graph_link_t 	prev;
  ucl_graph_link_t 	next;
  /* Remove 'in' from the list of ingoing links of the middle node. */
  prev = in->input.prev_link;
  next = in->input.prev_link;
  if (prev) prev->input.next_link = next;
  if (next) next->input.prev_link = prev;

  /* Remove 'out' from the list of outgoing links of the middle node. */
  prev = out->output.prev_link;
  next = out->output.next_link;
  if (prev) prev->output.next_link = next;
  if (next) next->output.prev_link = prev;

  /* Merge upon 'out'. */
  out->dst_node = in->dst_node;
  out->input.prev_link = in->input.prev_link;
  out->input.next_link = in->input.next_link;
}


/** ------------------------------------------------------------
 ** Depth first search: helper functions.
 ** ----------------------------------------------------------*/

static ucl_graph_dfs_item_t *
dfs_enter_node (ucl_graph_dfs_t S, ucl_graph_node_t N)
{
  ucl_graph_dfs_item_t *	dfs_node;
  N->mark.t_int = 1;
  /* Remember  that  'ucl_vector_push_back()'  is  a high  level  vector
     function that calls 'ucl_vector_enlarge()'. */
  dfs_node = ucl_vector_push_back(S->visited_nodes);
  dfs_node->node = N;
  dfs_node->in_counter = (S->counter)++;
  return dfs_node;
}
static void
dfs_leave_node (ucl_graph_dfs_t S, ucl_graph_dfs_item_t * dfs_node)
{
  dfs_node->out_counter = (S->counter)++;
}


/** --------------------------------------------------------------------
 ** Depth first search: public API.
 ** ----------------------------------------------------------------- */

void
ucl_graph_dfs_initialise_handle (ucl_graph_dfs_t S, ucl_vector_t visited_nodes)
{
  S->counter		= 0;
  S->visited_nodes	= visited_nodes;
}
void
ucl_graph_dfs_directed (ucl_graph_dfs_t S, ucl_graph_node_t root)
{
  ucl_graph_link_t 	L;
  ucl_graph_dfs_item_t *dfs_node;
  /* Test  if  the  node  is  marked,  that  is:  it  has  been  already
     visited. */
  if (ucl_graph_node_get_mark(root).t_int) return;
  dfs_node = dfs_enter_node(S, root);
  {
    UCL_GRAPH_OUTPUT_LINKS_LOOP(root, L)
      ucl_graph_dfs_directed(S, L->dst_node);
  }
  dfs_leave_node(S, dfs_node);
}
void
ucl_graph_dfs (ucl_graph_dfs_t S, ucl_graph_node_t root)
{
  ucl_graph_link_t 	L;
  ucl_graph_dfs_item_t *dfs_node;
  /* Test  if  the  node  is  marked,  that  is:  it  has  been  already
     visited. */
  if (ucl_graph_node_get_mark(root).t_int) return;
  dfs_node = dfs_enter_node(S, root);
  {
    UCL_GRAPH_OUTPUT_LINKS_LOOP(root, L)
      ucl_graph_dfs(S, L->dst_node);
    UCL_GRAPH_INPUT_LINKS_LOOP(root, L)
      ucl_graph_dfs(S, L->src_node);
  }
  dfs_leave_node(S, dfs_node);
}

/* end of file */
