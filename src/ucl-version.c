/*
  Part of: Useless Containers Library
  Contents: version functions
  Date: Sun Jun 16, 2013

  Abstract



  Copyright (C) 2013, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ucl-internals.h"


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

char const *
ucl_version_string (void)
{
  return ucl_VERSION_INTERFACE_STRING;
}
int
ucl_version_interface_current (void)
{
  return ucl_VERSION_INTERFACE_CURRENT;
}
int
ucl_version_interface_revision (void)
{
  return ucl_VERSION_INTERFACE_REVISION;
}
int
ucl_version_interface_age (void)
{
  return ucl_VERSION_INTERFACE_AGE;
}

/* end of file */
