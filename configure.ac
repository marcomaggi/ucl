dnl @configure_input@
dnl
dnl Part of: Useless Containers Library
dnl Contents: configuration script
dnl Date: Mon Feb  2, 2009
dnl
dnl Abstract
dnl
dnl
dnl
dnl Copyright (c) 2009, 2010 Marco Maggi <marco.maggi-ipsu@poste.it>
dnl
dnl This  program  is free  software:  you  can redistribute  it
dnl and/or modify it under the terms of the GNU General Public
dnl License as published by the Free Software Foundation, either
dnl version  3 of  the License,  or (at  your option)  any later
dnl version.
dnl
dnl This  program is  distributed in  the hope  that it  will be
dnl useful, but  WITHOUT ANY WARRANTY; without  even the implied
dnl warranty  of  MERCHANTABILITY or  FITNESS  FOR A  PARTICULAR
dnl PURPOSE.   See  the  GNU  General Public  License  for  more
dnl details.
dnl
dnl You should  have received a  copy of the GNU  General Public
dnl License   along   with    this   program.    If   not,   see
dnl <http://www.gnu.org/licenses/>.
dnl

dnl page
dnl --------------------------------------------------------------------
dnl Setup.
dnl --------------------------------------------------------------------

AC_INIT(ucl, 1.0d6)
AC_PREREQ(2.63)
AC_CONFIG_AUX_DIR(infrastructure)
AC_CONFIG_SRCDIR(./src/ucl.c)

DS_INCLUDES
DS_OPTIONS
DS_PROGRAMS
DS_PACKAGING

DS_INCLUDE_DEV_RULES=yes

DS_C_LANGUAGE_SYSTEM_SETUP
DS_C_LANGUAGE
DS_C_LANGUAGE_LIBRARIES
DS_C_LANGUAGE_LIBRARY(ucl,2,0,ucl)

dnl page
dnl --------------------------------------------------------------------
dnl Done.
dnl --------------------------------------------------------------------

AC_CONFIG_FILES([meta.d/ucl-config:meta/ucl-config.in])
AC_CONFIG_FILES([meta.d/ucl.pc:meta/ucl.pc.in])
AC_CONFIG_FILES([meta.d/slackware/bin/slack-desc:meta/slackware/bin/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/doc/slack-desc:meta/slackware/doc/slack-desc.in])
AC_CONFIG_FILES([meta.d/slackware/dev/slack-desc:meta/slackware/dev/slack-desc.in])
DS_CONFIG_FILES
AC_OUTPUT


### end of file
