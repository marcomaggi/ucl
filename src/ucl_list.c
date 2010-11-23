/*
  Part of: Useless Containers Library
  Contents: linked list

  Abstract:

	Lisp-like lists using the "ucl_node_tag_t" structure.

  Copyright (c) 2008-2010 Marco Maggi <marco.maggi-ipsu@poste.it>

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


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#define DEBUGGING	0
#include "internal.h"


/** ------------------------------------------------------------
 ** Inspection.
 ** ----------------------------------------------------------*/

size_t
ucl_list_length (void * _N)
{
  ucl_node_t	N = _N;
  size_t	length = 1;
  while ((N = N->bro))
    ++length;
  return length;
}
void *
ucl_list_ref (void * _N, ucl_index_t position)
{
  ucl_node_t	N = _N;
  for (ucl_index_t j=0; j < position; ++j) {
    if (N) N = N->bro;
  }
  return N;
}


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

void *
ucl_list_caar (void * _N)
{
  ucl_node_t	N = _N, P = NULL;
  if (N->son)
    P = N->son->son;
  return P;
}
void *
ucl_list_cadr (void * _N)
{
  ucl_node_t	N = _N, P = NULL;
  if (N->son)
    P = N->son->bro;
  return P;
}
void *
ucl_list_cdar (void * _N)
{
  ucl_node_t	N = _N, P = NULL;
  if (N->bro)
    P = N->bro->son;
  return P;
}
void *
ucl_list_cddr (void * _N)
{
  ucl_node_t	N = _N, P = NULL;
  if (N->bro)
    P = N->bro->bro;
  return P;
}

/* ------------------------------------------------------------ */

void *
ucl_list_caaar (void * _N)
{
  ucl_node_t	N = _N, P = NULL;
  if (N->son && N->son->son)
    P = N->son->son->son;
  return P;
}
void *
ucl_list_caadr (void * _N)
{
  ucl_node_t	N = _N, P = NULL;
  if (N->son && N->son->son)
    P = N->son->son->bro;
  return P;
}
void *
ucl_list_cadar (void * _N)
{
  ucl_node_t	N = _N , P = NULL;
  if (N->son && N->son->bro)
    P = N->son->bro->son;
  return P;
}
void *
ucl_list_caddr (void * _N)
{
  ucl_node_t	N = _N , P = NULL;
  if (N->son && N->son->bro)
    P = N->son->bro->bro;
  return P;
}
void *
ucl_list_cdaar (void * _N)
{
  ucl_node_t	N = _N , P = NULL;
  if (N->bro && N->bro->son)
    P = N->bro->son->son;
  return P;
}
void *
ucl_list_cdadr (void * _N)
{
  ucl_node_t	N = _N , P = NULL;
  if (N->bro && N->bro->son)
    P = N->bro->son->bro;
  return P;
}
void *
ucl_list_cddar (void * _N)
{
  ucl_node_t	N = _N , P = NULL;
  if (N->bro && N->bro->bro)
    P = N->bro->bro->son;
  return P;
}
void *
ucl_list_cdddr (void * _N)
{
  ucl_node_t	N = _N , P = NULL;
  if (N->bro && N->bro->bro)
    P = N->bro->bro->bro;
  return P;
}


/** ------------------------------------------------------------
 ** Removal.
 ** ----------------------------------------------------------*/

void *
ucl_list_remove (void * _N)
{
  ucl_node_t	N = _N;
  ucl_list_set_cdr(N->dad, N->bro);
  return N;
}
void *
ucl_list_popfront (void * _N)
{
  ucl_node_t	N = _N, first = ucl_tree_ref_first(N);
  if (first->bro) {
    first->bro->dad = NULL;
    first->bro      = NULL;
  }
  return first;
}
void *
ucl_list_popback (void * _N)
{
  ucl_node_t	N = _N, last = ucl_tree_ref_last(N);
  if (last->dad) {
    last->dad->bro = NULL;
    last->dad      = NULL;
  }
  return last;
}


/** --------------------------------------------------------------------
 ** Mapping and for each.
 ** ----------------------------------------------------------------- */

void
ucl_list_for_each (ucl_callback_t cb, void * _N)
{
  for (ucl_node_t N = _N; N; N = N->bro) {
    ucl_callback_apply(cb, N);
  }
}
void
ucl_list_map (void * _P, ucl_callback_t cb, void * _Q)
{
  for (ucl_node_t P = _P, Q = _Q; P; P = P->bro, Q = Q->bro) {
    ucl_callback_apply(cb, P, Q);
  }
}


/** --------------------------------------------------------------------
 ** Other operations.
 ** ----------------------------------------------------------------- */

void *
ucl_list_reverse (void * _N)
{
  ucl_node_t	N = _N, P;
  while (N) {
    P = N->bro;
    if (P) {
      ucl_list_set_cdr(P, N);
    } else {
      return N;
    }
  }
  return N;
}

/* end of file */
