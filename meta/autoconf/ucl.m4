# Part of: Useless Containers Library
# Contents: GNU Autoconf macros for UCL loading
# Date: Sun Sep 16, 2007
# 
# Abstract
# 
#       This file must be included in your "configure.ac" by
#       putting:
#
#               m4_include(ucl.m4)
#
#       in your "aclocal.m4" file.  It defines the single
#       macro 'USELESS_CONTAINERS_LIBRARY', which must be
#       invoked with the requested major and minor version
#       numbers as arguments:
#
#               USELESS_CONTAINERS_LIBRARY(1,0)
#
#       for interface version "1.0".
# 
# Copyright (c) 2007, 2008 Marco Maggi
# 
# This  program  is free  software:  you  can redistribute  it
# and/or modify it  under the terms of the  GNU General Public
# License as published by the Free Software Foundation, either
# version  3 of  the License,  or (at  your option)  any later
# version.
# 
# This  program is  distributed in  the hope  that it  will be
# useful, but  WITHOUT ANY WARRANTY; without  even the implied
# warranty  of  MERCHANTABILITY or  FITNESS  FOR A  PARTICULAR
# PURPOSE.   See  the  GNU  General Public  License  for  more
# details.
# 
# You should  have received a  copy of the GNU  General Public
# License   along   with    this   program.    If   not,   see
# <http://www.gnu.org/licenses/>.

AC_DEFUN([USELESS_CONTAINERS_LIBRARY], [
ucl_REQUESTED_MAJOR_VERSION=$1
ucl_REQUESTED_MINOR_VERSION=$2

AC_PATH_PROG([UCL_CONFIG],[ucl-config], :)
if test "${UCL_CONFIG}" = ':' ; then
    AC_MSG_ERROR([missing Useless Containers Library (cannot find ucl-config)],1)
fi

ucl_AVAILABLE_MAJOR_VERSION=$("${UCL_CONFIG}" --library-interface-major-version)
ucl_AVAILABLE_MINOR_VERSION=$("${UCL_CONFIG}" --library-interface-minor-version)

if test -n "${ucl_REQUESTED_MAJOR_VERSION}" ; then
    AC_MSG_NOTICE([requested UCL major version ${ucl_REQUESTED_MAJOR_VERSION}])

    if test "${ucl_REQUESTED_MAJOR_VERSION}" -ne "${ucl_AVAILABLE_MAJOR_VERSION}" ; then
        AC_MSG_ERROR([requested UCL major version number ${ucl_REQUESTED_MAJOR_VERSION} while available is ${ucl_AVAILABLE_MAJOR_VERSION}],[1])
    fi
    if test -n "${ucl_REQUESTED_MINOR_VERSION}" ; then
        AC_MSG_NOTICE([requested UCL minor version ${ucl_REQUESTED_MINOR_VERSION}])
        if test "${ucl_REQUESTED_MINOR_VERSION}" -gt "${ucl_AVAILABLE_MINOR_VERSION}" ; then
            AC_MSG_ERROR([requested UCL minor version number ${ucl_REQUESTED_MINOR_VERSION} while available is ${ucl_AVAILABLE_MINOR_VERSION}],[1])
        fi
    fi
fi

UCL_INCLUDEDIR=$("${UCL_CONFIG}" --pkgincludedir)
AC_SUBST([UCL_INCLUDEDIR],[${UCL_INCLUDEDIR}])
AC_MSG_NOTICE([UCL include directory ${UCL_INCLUDEDIR}])

UCL_CPPFLAGS="-I${UCL_INCLUDEDIR}"
AC_SUBST([UCL_CPPFLAGS],[${UCL_CPPFLAGS}])

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

AC_ARG_ENABLE([ucl-stub],
    AC_HELP_STRING([--disable-ucl-stub], [Disable UCL linking with stub mechanism (default: ENabled).]), [
        if test "$enableval" = yes ; then
            UCL_ENABLE_STUB=yes
        else
            UCL_ENABLE_STUB=no
        fi
],[UCL_ENABLE_STUB=yes])

AC_SUBST([UCL_ENABLE_STUB])

if test "${UCL_ENABLE_STUB}" = 'yes' ; then
    AC_MSG_NOTICE([enabled UCL linking with the stub mechanism])
    AC_DEFINE([UCL_ENABLE_STUB],[1],[enables UCL linking with the stub mechanism])
else
    AC_MSG_NOTICE([Disabled UCL linking with the stub mechanism])
fi

])

### end of file
# Local Variables:
# mode: sh
# End:
