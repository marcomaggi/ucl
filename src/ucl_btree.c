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
 ** Constructor.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-10) void
ucl_btree_constructor (ucl_btree_node_t * this_p, const ucl_btree_node_t * dad_p)
{
  assert(this_p);
  ucl_struct_clean(this_p, ucl_btree_node_t);
  this_p->dad_p	= (ucl_btree_node_t *)dad_p;
}
stub(2007-09-26-10-45-05) void
ucl_btree_destructor (ucl_btree_node_t * this)
{
  ucl_struct_clean(this, ucl_btree_node_t);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Setters.
 ** ----------------------------------------------------------*/

stub(2007-09-26-10-48-10) void
ucl_btree_setbro (ucl_btree_node_t * this, const ucl_btree_node_t * newBro)
{
  this->bro_p = (ucl_btree_node_t *)newBro;
}
stub(2007-09-26-10-48-16) void
ucl_btree_setson (ucl_btree_node_t * this, const ucl_btree_node_t * newSon)
{
  this->son_p = (ucl_btree_node_t *)newSon;
}
stub(2007-09-26-10-48-17) void
ucl_btree_setdad (ucl_btree_node_t * this, const ucl_btree_node_t * newDad)
{
  this->dad_p = (ucl_btree_node_t *)newDad;
}

/* ------------------------------------------------------------ */

stub(2007-09-26-10-48-49) void
ucl_btree_dadbro (ucl_btree_node_t * dad, ucl_btree_node_t * bro)
{
  dad->bro_p = bro;
  bro->dad_p = dad;
}
stub(2007-09-26-10-48-50) void
ucl_btree_dadson (ucl_btree_node_t * dad, ucl_btree_node_t * son)
{
  dad->son_p = son;
  son->dad_p = dad;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Getters.
 ** ----------------------------------------------------------*/

stub(2007-09-26-10-48-35) ucl_btree_node_t *
ucl_btree_getbro (const ucl_btree_node_t * this)
{
  return (ucl_btree_node_t *)this->bro_p;
}
stub(2007-09-26-10-48-36) ucl_btree_node_t *
ucl_btree_getson (const ucl_btree_node_t * this)
{
  return (ucl_btree_node_t *)this->son_p;
}
stub(2007-09-26-10-48-37) ucl_btree_node_t *
ucl_btree_getdad (const ucl_btree_node_t * this)
{
  return (ucl_btree_node_t *)this->dad_p;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Traversing.
 ** ----------------------------------------------------------*/

stub(2005-09-23-18-09-13) ucl_btree_node_t *
ucl_btree_find_leftmost (const ucl_btree_node_t *node_p)
{
  assert(node_p);
  while (node_p->son_p)
    {
      node_p = node_p->son_p;
    }
  return (ucl_btree_node_t *)node_p;
}
stub(2005-09-23-19-07-07) ucl_btree_node_t *
ucl_btree_find_rightmost (const ucl_btree_node_t *node_p)
{
  assert(node_p);
  while (node_p->bro_p)
    {
      node_p = node_p->bro_p;
    }
  return (ucl_btree_node_t *)node_p;
}
stub(2005-09-23-19-07-13) ucl_btree_node_t *
ucl_btree_find_deepest_son (const ucl_btree_node_t *node_p)
{
  assert(node_p);
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
  return (ucl_btree_node_t *)node_p;
}

/* ------------------------------------------------------------ */


stub(2005-09-23-18-09-15) ucl_btree_node_t *
ucl_btree_step_inorder (const ucl_btree_node_t *cur_p)
{
  const ucl_btree_node_t *	dad_p;


  assert(cur_p);

  if (cur_p->bro_p)
    {
      /* There's  a brother, so go to  it and find the  leftmost node in
         the subtree. */
      cur_p = cur_p->bro_p;
      while (cur_p->son_p)
	{
	  cur_p = cur_p->son_p;
	}
      return (ucl_btree_node_t *)cur_p;
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
      return (ucl_btree_node_t *)dad_p;
    }
}

stub(2005-09-23-18-09-19) ucl_btree_node_t *
ucl_btree_step_inorder_backward (const ucl_btree_node_t *cur_p)
{
  const ucl_btree_node_t *	dad_p;


  assert(cur_p);

  if (cur_p->son_p)
    {
      /* There is  a son: go  to it and  find the rightmost node  in the
	 subtree. */
      cur_p = cur_p->son_p;
      while (cur_p->bro_p)
	{
	  cur_p = cur_p->bro_p;
	}
      return (ucl_btree_node_t *)cur_p;
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
      return (ucl_btree_node_t *)dad_p;
    }
}

stub(2005-09-23-18-09-23) ucl_btree_node_t *
ucl_btree_step_preorder (const ucl_btree_node_t *cur_p)
{
  const ucl_btree_node_t *	dad_p;


  /* Visit the son, if any. If no son: visit the brother, if any. */
  if (cur_p->son_p)
    {
      return (ucl_btree_node_t *)(cur_p->son_p);
    }
  else if (cur_p->bro_p)
    {
      return (ucl_btree_node_t *)(cur_p->bro_p);
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
	      return (ucl_btree_node_t *)(dad_p->bro_p);
	    }
	  else
	    {
	      cur_p = dad_p;
	    }
	}
    }
  return (ucl_btree_node_t *)dad_p;
}

stub(2005-09-23-18-09-27) ucl_btree_node_t *
ucl_btree_step_postorder (const ucl_btree_node_t *cur_p)
{
  const ucl_btree_node_t *	dad_p;


  dad_p = cur_p->dad_p;
  if (! dad_p)
    {
      return (ucl_btree_node_t *)dad_p;
    }
  else if ((dad_p->bro_p) && (dad_p->bro_p != cur_p))
    {
      return ucl_btree_find_deepest_son(dad_p->bro_p);
    }
  else
    {
      return (ucl_btree_node_t *)dad_p;
    }
}

stub(2005-09-23-18-09-30) ucl_btree_node_t *
ucl_btree_step_levelorder (const ucl_btree_node_t * cur_p)
{
  const ucl_btree_node_t *	org_p;
  const ucl_btree_node_t *	last_p;
  int i=0;


  assert(cur_p != NULL);

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
		  return (ucl_btree_node_t *)cur_p;
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
		      return (ucl_btree_node_t *)cur_p;
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
		  return (ucl_btree_node_t *)cur_p;
		}
	      if ((cur_p == org_p) && (!cur_p->son_p) && (!cur_p->bro_p))
		{
		  return NULL;
		}
	    }
	}
    }
  return (ucl_btree_node_t *)cur_p;
}

/* end of file */
