/* hashtest.h --
   
   Part of: Useless Containers Library
   Contents: header file for hash table tests
   Date: Wed Dec 10, 2003
   
   Abstract
   
   
   
   Copyright (c) 2003, 2004, 2005 Marco Maggi
   
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


#ifndef __HASHTEST_H
#define __HASHTEST_H 1

#include <stdio.h>
#include "ucl.h"
#include "debug.h"
	
#define BUCKETS	100
#define NUMBER 1000
#define DELTA	234

UCL_BEGIN_C_DECL

extern void test (void);
extern ucl_hash_entry_t * alloc_link (void);
extern size_t hash_num (ucl_value_t data, ucl_value_t val);

/* ------------------------------------------------------------ */

ucl_hash_entry_t *
alloc_link(void)
{
  ucl_hash_entry_t *		ent_p;

  
  ent_p = (ucl_hash_entry_t *) malloc(sizeof(ucl_hash_entry_t));
  if (ent_p == NULL)
    {
      printf("error allocating memoryn");
      exit(EXIT_FAILURE);
    }
  return ent_p;
}

size_t
hash_num(ucl_value_t data UCL_UNUSED, ucl_value_t val)
{
  return (size_t) val.integer;
}

UCL_END_C_DECL

#endif /* __HASHTEST_H */


/* end of file */
