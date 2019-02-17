/*
  Part of: Useless Containers Library
  Contents: linked list

  Abstract:

	Lisp-like lists using the "ucl_node_tag_t" structure.

  Copyright (c) 2008-2010, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#define DEBUGGING	0
#include "ucl-internals.h"

#define SWAP(APTR, BPTR)	\
  do {				\
    void * tmp=(APTR);		\
    (APTR)=(BPTR);		\
    (BPTR)=tmp;			\
  } while (0);


/** ------------------------------------------------------------
 ** Inspection.
 ** ----------------------------------------------------------*/

size_t
ucl_list_length (void * N_)
{
  ucl_node_t	N = N_;
  size_t	length = 1;
  while ((N = N->bro))
    ++length;
  return length;
}
void *
ucl_list_ref (void * N_, ucl_index_t position)
{
  ucl_node_t	N = N_;
  for (ucl_index_t j=0; j < position; ++j) {
    if (N) N = N->bro;
  }
  return N;
}


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

void *
ucl_list_caar (void * N_)
{
  ucl_node_t	N = N_, P = NULL;
  if (N->son)
    P = N->son->son;
  return P;
}
void *
ucl_list_cadr (void * N_)
{
  ucl_node_t	N = N_, P = NULL;
  if (N->son)
    P = N->son->bro;
  return P;
}
void *
ucl_list_cdar (void * N_)
{
  ucl_node_t	N = N_, P = NULL;
  if (N->bro)
    P = N->bro->son;
  return P;
}
void *
ucl_list_cddr (void * N_)
{
  ucl_node_t	N = N_, P = NULL;
  if (N->bro)
    P = N->bro->bro;
  return P;
}

/* ------------------------------------------------------------ */

void *
ucl_list_caaar (void * N_)
{
  ucl_node_t	N = N_, P = NULL;
  if (N->son && N->son->son)
    P = N->son->son->son;
  return P;
}
void *
ucl_list_caadr (void * N_)
{
  ucl_node_t	N = N_, P = NULL;
  if (N->son && N->son->son)
    P = N->son->son->bro;
  return P;
}
void *
ucl_list_cadar (void * N_)
{
  ucl_node_t	N = N_ , P = NULL;
  if (N->son && N->son->bro)
    P = N->son->bro->son;
  return P;
}
void *
ucl_list_caddr (void * N_)
{
  ucl_node_t	N = N_ , P = NULL;
  if (N->son && N->son->bro)
    P = N->son->bro->bro;
  return P;
}
void *
ucl_list_cdaar (void * N_)
{
  ucl_node_t	N = N_ , P = NULL;
  if (N->bro && N->bro->son)
    P = N->bro->son->son;
  return P;
}
void *
ucl_list_cdadr (void * N_)
{
  ucl_node_t	N = N_ , P = NULL;
  if (N->bro && N->bro->son)
    P = N->bro->son->bro;
  return P;
}
void *
ucl_list_cddar (void * N_)
{
  ucl_node_t	N = N_ , P = NULL;
  if (N->bro && N->bro->bro)
    P = N->bro->bro->son;
  return P;
}
void *
ucl_list_cdddr (void * N_)
{
  ucl_node_t	N = N_ , P = NULL;
  if (N->bro && N->bro->bro)
    P = N->bro->bro->bro;
  return P;
}


/** ------------------------------------------------------------
 ** Removal.
 ** ----------------------------------------------------------*/

void *
ucl_list_remove (void * N_)
{
  ucl_node_t	N = N_;
  ucl_list_set_cdr(N->dad, N->bro);
  return N;
}
void *
ucl_list_popfront (void * N_, void ** new_first_p)
{
  ucl_node_t	N = ucl_tree_ref_first(N_), new_first = NULL;
  if (N->bro) {
    new_first  = N->bro;
    N->bro = NULL;
    if (new_first)
      new_first->dad = NULL;
  }
  *new_first_p = new_first;
  return N;
}
void *
ucl_list_popback (void * N_)
{
  ucl_node_t	N = ucl_tree_ref_last(N_), new_last = NULL;
  if (N->dad) {
    new_last = N->dad;
    N->dad = NULL;
    if (new_last)
      new_last->bro = NULL;
  }
  return N;
}


/** --------------------------------------------------------------------
 ** Mapping and for each.
 ** ----------------------------------------------------------------- */

void
ucl_list_for_each (ucl_callback_t cb, void * N_)
{
  for (ucl_node_t N = N_; N; N = N->bro) {
    ucl_callback_apply(cb, N);
  }
}
void
ucl_list_map (void * _P, ucl_callback_t cb, void * Q_)
{
  for (ucl_node_t P = _P, Q = Q_; P; P = P->bro, Q = Q->bro) {
    ucl_callback_apply(cb, P, Q);
  }
}


/** --------------------------------------------------------------------
 ** Other operations.
 ** ----------------------------------------------------------------- */

void *
ucl_list_reverse (void * N_)
{
  ucl_node_t	N, P;
  for (N = N_, P = N_; N; P = N, N = N->dad) {
    SWAP(N->dad, N->bro);
  }
  return P;
}

/* end of file */
