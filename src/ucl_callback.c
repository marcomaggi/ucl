/*
   Part of: Useless Containers Library
   Contents: callback handling functions
   Date: Mon Sep 15, 2008
   
   Abstract
   
   
   
   Copyright (c) 2008 Marco Maggi <marcomaggi@gna.org>
   
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
 ** Headers.
 ** ----------------------------------------------------------*/

#define DEBUGGING		0
#include "internal.h"

#define stubmodule		callback

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Functions.
 ** ----------------------------------------------------------*/

stub(2008-09-15-13-34-00) void
ucl_callback_apply (ucl_callback_t callback, ...)
{
  if (ucl_callback_is_present(callback))
    {
      va_list	ap;

      va_start(ap,callback);
      callback.func(callback.data, ap);
      va_end(ap);
    }
}
stub(2008-09-15-13-43-41) void
ucl_callback_eval_thunk (ucl_callback_t callback)
{
  if (ucl_callback_is_present(callback))
    callback.func(callback.data,NULL);
}


/* end of file */
