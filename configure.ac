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

AC_INIT([ucl],[1.0a2],[marco.maggi-ipsu@poste.it])
DS_BEGIN([./src/ucl.c])

DS_C99_LANGUAGE_COMMON_SETUP
dnl DS_EXCLUDE_SECTION([DOC_RULES])
dnl DS_EXCLUDE_SECTION([DEV_RULES])
dnl DS_EXCLUDE_SECTION([CONFIG_INSPECTION_SCRIPT])
dnl DS_EXCLUDE_SECTION([AUTOCONF])
dnl DS_EXCLUDE_SECTION([PKGCONFIG])
DS_C_LANGUAGE_LIBRARY([ucl],3,0,[ucl])

dnl page
dnl --------------------------------------------------------------------
dnl Done.
dnl --------------------------------------------------------------------

DS_CONFIG_FILE_PKGCONFIG
DS_CONFIG_FILE_INSPECTION_SCRIPT
DS_CONFIG_FILE_SLACKWARE_BIN_DESC
DS_CONFIG_FILE_SLACKWARE_DOC_DESC
DS_CONFIG_FILE_SLACKWARE_DEV_DESC
DS_END

### end of file
