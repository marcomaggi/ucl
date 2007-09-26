# Part of: Useless Containers Library
# Contents: GNU Autoconf macros for UCL loading
# Date: Sun Sep 16, 2007
# 
# Abstract
# 
# 
# 
# Copyright (c) 2007 Marco Maggi
# 
# This is free  software you can redistribute it  and/or modify it under
# the terms of  the GNU General Public License as  published by the Free
# Software Foundation; either  version 2, or (at your  option) any later
# version.
# 
# This  file is  distributed in  the hope  that it  will be  useful, but
# WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
# MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
# General Public License for more details.
# 
# You  should have received  a copy  of the  GNU General  Public License
# along with this file; see the file COPYING.  If not, write to the Free
# Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
# 02111-1307, USA.



AC_DEFUN([USELESS_CONTAINERS_LIBRARY], [
AC_PATH_PROG([UCL_CONFIG],[ucl-config], :)
if test "${UCL_CONFIG}" = ':' ; then
    AC_MSG_ERROR([missing Useless Containers Library (cannot find ucl-config)],1)
fi

UCL_INCLUDEDIR=$("${UCL_CONFIG}" --pkgincludedir)
AC_SUBST([UCL_INCLUDEDIR],[${UCL_INCLUDEDIR}])
AC_MSG_NOTICE([UCL include directory ${UCL_INCLUDEDIR}])

UCL_LIBDIR=$("${UCL_CONFIG}" --libdir)
AC_MSG_NOTICE([UCL library directory ${UCL_LIBDIR}])
UCL_LDFLAGS="-L${UCL_LIBDIR}"
AC_SUBST([UCL_LDFLAGS],[${UCL_LDFLAGS}])

UCL_LIBRARY_ID=$("${UCL_CONFIG}" --library-id)
AC_SUBST([UCL_LIBRARY_ID],[${UCL_LIBRARY_ID}])
AC_MSG_NOTICE([UCL library identifier ${UCL_LIBRARY_ID}])

UCL_STUB_STATIC_LIBRARY_ID=$("${UCL_CONFIG}" --stub-static-library-id)
AC_SUBST([UCL_STUB_STATIC_LIBRARY_ID],[${UCL_STUB_STATIC_LIBRARY_ID}])
AC_MSG_NOTICE([UCL stub static library identifier ${UCL_STUB_STATIC_LIBRARY_ID}])

# Verify the header file.
INCLUDES="${INCLUDES} -I${UCL_INCLUDEDIR}"
ucl_saved_CPPFLAGS="${CPPFLAGS}"
CPPFLAGS="${CPPFLAGS} ${INCLUDES}"
AC_CHECK_HEADERS([ucl.h])
CPPFLAGS="${ucl_saved_CPPFLAGS}"

# Verify the static/shared library.
UCL_LIBS=
AC_CHECK_LIB(${UCL_LIBRARY_ID},ucl_version,[UCL_LIBS="-l${UCL_LIBRARY_ID}"],[
AC_MSG_ERROR([cannot link to Useless Containers Library])
])
AC_SUBST([UCL_LIBS],[${UCL_LIBS}])

# Verify the static/shared library.
UCL_STUB_LIBS=
AC_CHECK_LIB(${UCL_STUB_STATIC_LIBRARY_ID},ucl_init_stub_table,
    [UCL_STUB_LIBS="-l${UCL_STUB_STATIC_LIBRARY_ID}"],[
AC_MSG_WARN([cannot link to stub static Useless Containers Library])
],[-ldl])

AC_SUBST([UCL_STUB_LIBS],[${UCL_STUB_LIBS}])

])

### end of file
# Local Variables:
# mode: sh
# End:
