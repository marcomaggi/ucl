/* ucl_graph.c --
   
   Part of: Useless Containers Library
   Contents: graph implementation
   Date: Sat Mar  1, 2003
   
   Abstract
   
   
   
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
   
   $Id: ucl_graph.c,v 1.1.1.5 2003/12/09 12:43:07 marco Exp $
*/


/** ------------------------------------------------------------
 ** Header files.
 ** ----------------------------------------------------------*/

#include <assert.h>
#include "ucl_graph.h"

#ifdef DEBUGGING
#  include <stdio.h>
#endif


/** ------------------------------------------------------------
 ** Type declarations.
 ** ----------------------------------------------------------*/

typedef ucl_graph_node_t		node_t;
typedef ucl_graph_link_t		link_t;



/* ucl_graph_link --

	Links two nodes.

   Arguments:

	lnkPtr -	pointer to the link structure
	fromPtr -	pointer to the structure of the source node
	toPtr -		pointer to the structure of the target node

   Results:

        The  link structure is  appended to  the chain  of links  of the
        source  node. The  target node  pointer  is stored  in the  link
        structure.

   Side effects:

        None.

*/

void
ucl_graph_link (link *lnkPtr, node_t *fromPtr, node_t *toPtr)
{
  Link *	tmpPtr;


  assert(lnkPtr);
  assert(fromPtr);
  assert(toPtr);

  tmpPtr = ucl_graph_getlink(fromPtr);
  while (ucl_graph_getnext(tmpPtr))
    {
      tmpPtr = ucl_graph_getnext(tmpPtr);
    }

  ucl_graph_setnext(tmpPtr, lnkPtr);
  ucl_graph_setnode(lnkPtr, toPtr);
}




/* end of file */
