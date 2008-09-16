/*    
   Part of: Useless Containers Library (UCL)
   Contents: binary tree functions
   Date: Fri Oct 11, 2002
   
   Abstract
   
	The btree container is built as a chain of structures; each link
	has a brother, a son and a parent.

                 -----    bro     -----
	        | no1 |--------->| no2 |
	         ----- <--------- -----
	         |  ^     dad
             son |  | dad
                 v  |
                 ----- 
                | no3 |
                 ----- 
   
   
   Copyright (c) 2002, 2003, 2004, 2005, 2008 Marco Maggi
   
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

#define stubmodule		btree

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Traversing.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-13) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_find_leftmost (void * _node_p)
{
  ucl_btree_node_t	node_p = _node_p;

  while (node_p->son_p)
    {
      node_p = node_p->son_p;
    }
  return node_p;
}
stub(2005-09-23-19-07-07) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_find_rightmost (void * _node_p)
{
  ucl_btree_node_t	node_p = _node_p;

  while (node_p->bro_p)
    {
      node_p = node_p->bro_p;
    }
  return node_p;
}
stub(2005-09-23-19-07-13) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_find_deepest_son (void * _node_p)
{
  ucl_btree_node_t	node_p = _node_p;

  do
    {
      while (node_p->son_p)
	{
	  node_p = node_p->son_p;
	}
      if (node_p->bro_p)
	{
	  node_p = node_p->bro_p;
	}
    }
  while (node_p->son_p || node_p->bro_p);
  return node_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Inorder forward iteration step.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-15) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_step_inorder (void * _cur_p)
{
  ucl_btree_node_t	cur_p = _cur_p;
  ucl_btree_node_t	dad_p;


  if (cur_p->bro_p)
    {
      /* There's  a brother, so go to  it and find the  leftmost node in
         the subtree. */
      cur_p = cur_p->bro_p;
      while (cur_p->son_p)
	{
	  cur_p = cur_p->son_p;
	}
      return cur_p;
    }
  else
    {
      /* If  there's no brother step  up to the father;  if we're coming
	 from the brother of the father we have to step up again because
	 this  father has  been already  visited.

	 If the node  has NULL father or we're coming  from the son, the
	 step is complete:  we return the father. The  iteration is over
	 if the father is NULL. */

      dad_p = cur_p->dad_p;
      while ((dad_p) && (dad_p->bro_p == cur_p))
	{
	  cur_p	= dad_p;
	  dad_p	= cur_p->dad_p;
	}
      return dad_p;
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Inorder backward iteration step.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-19) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_step_inorder_backward (void * _cur_p)
{
  ucl_btree_node_t	cur_p = _cur_p;
  ucl_btree_node_t	dad_p;


  if (cur_p->son_p)
    {
      /* There is  a son: go  to it and  find the rightmost node  in the
	 subtree. */
      cur_p = cur_p->son_p;
      while (cur_p->bro_p)
	{
	  cur_p = cur_p->bro_p;
	}
      return cur_p;
    }
  else  
    {
      /* There's no son: step up to the father; if we're coming from the
	 son of the father we have  to step up again because this father
	 has been already visited.
     
	 If the node  has NULL father or we're  coming from the brother,
	 the step  is complete: we  return the father. The  iteration is
	 over if the father is NULL. */

      dad_p = cur_p->dad_p;
      while ((dad_p) && (dad_p->son_p == cur_p))
	{
	  cur_p	= dad_p;
	  dad_p	= cur_p->dad_p;
	}
      return dad_p;
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Preorder forward iteration step.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-23) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_step_preorder (void * _cur_p)
{
  ucl_btree_node_t	cur_p = _cur_p;
  ucl_btree_node_t	dad_p;


  /* Visit the son, if any. If no son: visit the brother, if any. */
  if (cur_p->son_p)
    {
      return cur_p->son_p;
    }
  else if (cur_p->bro_p)
    {
      return cur_p->bro_p;
    }
  /* No  son and no  brother so  step up  to the  father: if  there's no
     father, ends the  iteration; if coming from the  father's son visit
     the father's brother,  if any; if coming from  the father's brother
     step up and loop.
  */
  else
    {
      for (;;)
	{
	  dad_p = cur_p->dad_p;

	  if (! dad_p)
	    {
	      break;
	    }
	  else if ((dad_p->son_p == cur_p) && (dad_p->bro_p))
	    {
	      return dad_p->bro_p;
	    }
	  else
	    {
	      cur_p = dad_p;
	    }
	}
    }
  return dad_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Postorder forward iteration step.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-27) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_step_postorder (void * _cur_p)
{
  ucl_btree_node_t	cur_p = _cur_p;
  ucl_btree_node_t	dad_p;


  dad_p = cur_p->dad_p;
  if (! dad_p)
    {
      return dad_p;
    }
  else if ((dad_p->bro_p) && (dad_p->bro_p != cur_p))
    {
      return ucl_btree_find_deepest_son(dad_p->bro_p);
    }
  else
    {
      return dad_p;
    }
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Levelorder forward iteration step.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-30) __attribute__((__nonnull__,__pure__)) void *
ucl_btree_step_levelorder (void * _cur_p)
{
  ucl_btree_node_t	cur_p = _cur_p;
  ucl_btree_node_t	org_p;
  ucl_btree_node_t	last_p;
  int i=0;


  if ((! cur_p->dad_p) && (! cur_p->son_p) && (! cur_p->bro_p))
    {
      return NULL;
    }

  org_p = cur_p;

  for (;;)
    {
      if (cur_p->dad_p)
	{
	  last_p = cur_p;
	  cur_p = cur_p->dad_p;
	  ++i;
	  if (cur_p->bro_p && cur_p->bro_p != last_p)
	    {
	      last_p = cur_p;
	      cur_p = cur_p->bro_p;
	      --i;
	      if (i == 0)
		{
		  return cur_p;
		}
	      if (cur_p == org_p && !cur_p->son_p && !cur_p->bro_p)
		{
		  return NULL;
		}
	      while (cur_p->son_p || cur_p->bro_p)
		{
		  if (cur_p->son_p)
		    {
		      last_p = cur_p;
		      cur_p = cur_p->son_p;
		    }
		  else if (cur_p->bro_p)
		    {
		      last_p = cur_p;
		      cur_p = cur_p->bro_p;
		    }
		  --i;
		  if (i == 0)
		    {
		      return cur_p;
		    }
		  if (cur_p == org_p && !cur_p->son_p && !cur_p->bro_p)
		    {
		      return NULL;
		    }
		}
	    }
	}
      else
	{
	  ++i;
	  while (cur_p->son_p || cur_p->bro_p)
	    {
	      if (cur_p->son_p)
		{
		  last_p = cur_p;
		  cur_p = cur_p->son_p;
		}
	      else if (cur_p->bro_p)
		{
		  last_p = cur_p;
		  cur_p = cur_p->bro_p;
		}
	      --i;
	      if (i == 0)
		{
		  return cur_p;
		}
	      if ((cur_p == org_p) && (!cur_p->son_p) && (!cur_p->bro_p))
		{
		  return NULL;
		}
	    }
	}
    }
  return cur_p;
}

/* end of file */
