/*
   Part of: Useless Containers Library
   Contents: linked list

   Abstract:

	Lisp-like lists using the "ucl_node_t" structure.

   Copyright (c) 2001-2005, 2008-2010 Marco Maggi <marcomaggi@gna.org>

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



/** ------------------------------------------------------------
 ** Inspection.
 ** ----------------------------------------------------------*/

size_t
ucl_list_length (ucl_node_t node)
{
  size_t	length = 0;

  while ((node = node->bro))
    ++length;
  return length;
}


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

ucl_node_t
ucl_list_caar (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->son)
    N = node->son->son;
  return N;
}
ucl_node_t
ucl_list_cadr (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->son)
    N = node->son->bro;
  return N;
}

/* ------------------------------------------------------------ */

ucl_node_t
ucl_list_cdar (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->bro)
    N = node->bro->son;
  return N;
}
ucl_node_t
ucl_list_cddr (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->bro)
    N = node->bro->bro;
  return N;
}

/* ------------------------------------------------------------ */

ucl_node_t
ucl_list_caaar (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->son && node->son->son)
    N = node->son->son->son;
  return N;
}
ucl_node_t
ucl_list_caadr (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->son && node->son->son)
    N = node->son->son->bro;
  return N;
}
ucl_node_t
ucl_list_cadar (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->son && node->son->bro)
    N = node->son->bro->son;
  return N;
}
ucl_node_t
ucl_list_caddr (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->son && node->son->bro)
    N = node->son->bro->bro;
  return N;
}
ucl_node_t
ucl_list_cdaar (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->bro && node->bro->son)
    N = node->bro->son->son;
  return N;
}
ucl_node_t
ucl_list_cdadr (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->bro && node->bro->son)
    N = node->bro->son->bro;
  return N;
}
ucl_node_t
ucl_list_cddar (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->bro && node->bro->bro)
    N = node->bro->bro->son;
  return N;
}
ucl_node_t
ucl_list_cdddr (ucl_node_t node)
{
  ucl_node_t	N = NULL;

  if (node->bro && node->bro->bro)
    N = node->bro->bro->bro;
  return N;
}


/** ------------------------------------------------------------
 ** Removal.
 ** ----------------------------------------------------------*/

ucl_node_t
ucl_list_remove (ucl_node_t node)
{
  ucl_node_t	dad = node->dad;
  ucl_node_t	bro = node->bro;

  if (dad && bro)
    {
      dad->bro = bro;
      bro->dad = dad;
    }
  else if (dad)
    {
      assert(NULL == node->bro);
      dad->bro  = NULL;
      node->dad = NULL;
    }
  else if (bro)
    {
      assert(NULL == node->dad);
      bro->dad  = NULL;
      node->bro = NULL;
    }
  return node;
}
ucl_node_t
ucl_list_popfront (ucl_node_t node)
{
  ucl_node_t	first = ucl_tree_get_first(node);

  if (first->bro)
    {
      first->bro->dad = NULL;
      first->bro      = NULL;
    }
  return first;
}
ucl_node_t
ucl_list_popback (ucl_node_t node)
{
  ucl_node_t	last = ucl_tree_get_last(node);

  if (last->dad)
    {
      last->dad->bro = NULL;
      last->dad      = NULL;
    }
  return last;
}


/* end of file */