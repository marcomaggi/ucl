/* vectortest.h --
   
   Part of: Useless Containers Library
   Contents: main header file for vector tests
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004 Marco Maggi
   
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


#ifndef __VECTORTEST_H
#define __VECTORTEST_H 1

#include <stdio.h>
#include "ucl.h"
#include "debug.h"

#define NUMBER	1000
#define DELTA	234

UCL_BEGIN_C_DECL

extern void test (void);
extern void fill (ucl_vector_t vectptr, int number, int first);
extern void clean (ucl_vector_t vectPtr);
extern int intcmp (const int *a, const int *b);

/* ------------------------------------------------------------ */

void
fill (ucl_vector_t vect_p, int number, int first)
{
  int	i;
  int *	ptr;

  for (i=0; i < number; ++i)
    {
      ucl_vector_enlarge(vect_p);

      ptr = ucl_vector_index_to_new_slot(vect_p, ucl_vector_size(vect_p));
      ptr = ucl_vector_insert(vect_p, ptr);
      *ptr = i + first;
    }
}
void
clean (ucl_vector_t vect_p)
{
  int *	ptr;

  while(ucl_vector_size(vect_p))
    {
      ptr = ucl_vector_index_to_slot(vect_p, 0);
      ucl_vector_erase(vect_p, ptr);
      ucl_vector_restrict(vect_p);
    }
}
int
intcmp (const int *a, const int *b)
{
  return ((*a == *b)? 0 : ((*a > *b)? 1 : -1));
}

UCL_END_C_DECL

#endif /* __VECTORTEST_H */

/* end of file */
