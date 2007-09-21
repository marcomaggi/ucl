/* ucl_graph.h --
   
   Part of: Useless Containers Library
   Contents: graph header file
   Date: Sat Mar  1, 2003
   
   Abstract
   
	    ----     ----     ----     ----
           |nod1|-->|lnk1|-->|nod2|-->|lnk4|
            ----     ----     ----     ----
                      |
                      v
                     ----     ----
                    |lnk2|-->|nod3|
                     ----     ----
                      |
                      v 
                     ----     ----
                    |lnk3|-->|nod4|
                     ----     ----
   
   Copyright (c) 2003 Marco Maggi
   
   This is free software; you  can redistribute it and/or modify it under
   the terms of the GNU Lesser General Public License as published by the
   Free Software  Foundation; either version  2.1 of the License,  or (at
   your option) any later version.
   
   This library  is distributed in the  hope that it will  be useful, but
   WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
   MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
   Lesser General Public License for more details.
   
   You  should have  received a  copy of  the GNU  Lesser  General Public
   License along  with this library; if  not, write to  the Free Software
   Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
   USA
   
   $Id: ucl_graph.h,v 1.1.1.5 2003/12/09 12:42:04 marco Exp $
*/


#ifndef __UCL_GRAPH_H
#define __UCL_GRAPH_H 1

#include <string.h>
#include "ucl.h"


struct ucl_graph_node_t;
struct ucl_graph_link_t;

typedef struct ucl_graph_node_t {
  struct ucl_graph_link_t *	lnkPtr;
  ucl_value_t			val;
} ucl_graph_node_t;

typedef struct ucl_graph_link_t {
  struct ucl_graph_node_t *	nodPtr;
  struct ucl_graph_link_t *	nxtPtr;
  ucl_value_t			val;
} ucl_graph_link_t;


#define ucl_graph_initnode(nodPtr) memset(nodPtr, '\0', sizeof(ucl_graph_node_t))
#define ucl_graph_initlink(nodPtr) memset(nodPtr, '\0', sizeof(ucl_graph_link_t))

void	ucl_graph_link	(ucl_graph_link_t *lnkPtr,
			 ucl_graph_node_t *fromPtr,
			 ucl_graph_node_t *toPtr);

#define ucl_graph_setlink(nodPtr, newPtr)	((nodPtr)->lnkPtr)=(newPtr)
#define ucl_graph_getlink(nodPtr)		((nodPtr)->lnkPtr)

#define ucl_graph_setnext(lnkPtr, newPtr)	((lnkPtr)->nxtPtr)=(newPtr)
#define ucl_graph_getnext(lnkPtr)		((lnkPtr)->nxtPtr)

#define ucl_graph_setnode(lnkPtr, newPtr)	((lnkPtr)->nodPtr)=(newPtr)
#define ucl_graph_getnode(lnkPtr)		((lnkPtr)->nodPtr)

#define ucl_graph_setval(ptr,newval)	((ptr)->val)=(newval)
#define ucl_graph_getval(ptr)		((ptr)->val)

#endif /* __UCL_GRAPH_H */


/* end of file */
