# configure.ds --
#






#PAGE
## ------------------------------------------------------------
## Initialisation.
## ------------------------------------------------------------

AC_INIT([ucl], [0.7.1], [marcomaggi@tiscali.it])
AC_PREREQ(2.57)
AC_COPYRIGHT([Copyright (C) 2002, 2003, 2004 by Marco Maggi])
AC_REVISION([$Revision: 1.1.1.131 $])
AC_CONFIG_SRCDIR([./generic/ucl.h])
AC_CONFIG_AUX_DIR([infrastructure])

AC_SUBST([PACKAGE_XVERSION], [0.7.1])
AC_DEFINE([PACKAGE_XVERSION], ["0.7.1"], [full version])
AC_SUBST([PACKAGE_NODOT_VERSION], [07])
AC_SUBST([PACKAGE_NAME_UPPERCASE], [UCL])
AC_SUBST([PACKAGE_NAME_LOWERCASE], [ucl])

AC_SUBST([PACKAGE_VERSION_MAJMIN], [0.7])
AC_SUBST([PACKAGE_VERSION_MAJOR], [0])
AC_SUBST([PACKAGE_VERSION_MINOR], [7])
AC_SUBST([PACKAGE_VERSION_PATCH], [1])


#PAGE
## ------------------------------------------------------------
## Common programs.
## ------------------------------------------------------------

AC_PROG_INSTALL
AC_PROG_MAKE_SET


AC_PATH_PROG([BASH], [bash], :)
AC_ARG_VAR([BASH], [the GNU bash shell])

AC_PATH_PROG([BZIP], [bzip2], :)
AC_ARG_VAR([BZIP], [the bzip2 compressor program])

AC_PATH_PROG([CAT], [cat], :)
AC_ARG_VAR([CAT], [the GNU cat program])

AC_PATH_PROG([CP], [cp], :)
AC_ARG_VAR([CP], [copies files])

AC_PATH_PROG([DATE], [date], :)
AC_ARG_VAR([DATE], [a program that prints the current date])

AC_PATH_PROG([GREP], [grep], :)
AC_ARG_VAR([GREP], [the GNU grep program])

AC_PATH_PROG([GZIP], [gzip], :)
AC_ARG_VAR([GZIP], [the gzip compressor program])

AC_PATH_PROG([M4], [m4], :)
AC_ARG_VAR([M4], [the GNU m4 preprocessor])

AC_PATH_PROG([MAKEINFO], [makeinfo], :)
AC_ARG_VAR([MAKEINFO], [builds docs from Texinfo source])

AC_PATH_PROG([MKDIR], [mkdir], :)
AC_ARG_VAR([MKDIR], [creates directories recursively])

AC_PATH_PROG([RM], [rm], :)
AC_ARG_VAR([RM], [deletes files and directories recursively])

AC_PATH_PROG([SED], [sed], :)
AC_ARG_VAR([SED], [the GNU sed program])

AC_PATH_PROG([SYMLINK], [ln], :)
AC_ARG_VAR([SYMLINK], [program used create symbolic links])

AC_PATH_PROG([TAR], [tar], :)
AC_ARG_VAR([TAR], [the GNU tar program])

AC_PATH_PROG([TCLSH], [tclsh], :)
AC_ARG_VAR([TCLSH], [the TCL shell])

CURRENT_DATE=$($DATE)
AC_SUBST(CURRENT_DATE)


#PAGE
## ------------------------------------------------------------
## C language environment.
## ------------------------------------------------------------

AC_ARG_ENABLE([assert],
    AC_HELP_STRING([--disable-assert], [Disable assertions.]),
    [AC_DEFINE([NDEBUG])])

AC_PROG_CC
AC_PROG_RANLIB
AC_CHECK_PROG([AR], [ar], [ar], :)
AC_CHECK_PROG([STRIP], [strip], [strip], :)

if test "${GCC}" = "yes"; then
    ALL_CFLAGS="${ALL_CFLAGS} -Wall -pipe -ansi -pedantic"
fi	
AC_SUBST(ALL_CFLAGS)
AC_ARG_VAR([ALL_CFLAGS], [fixed C compiler flags])

INCLUDES="-I. -I\$(top_srcdir) -I\$(srcdir) -I\$(builddir)"
for ds_tmp in generic compat unix win mac; do
    ds_tmp="${top_srcdir}/src/${ds_tmp}"
    if test -d "${ds_tmp}"; then
	INCLUDES="${INCLUDES} -I${ds_tmp}"
    fi
done
for ds_tmp in generic compat unix win mac; do
    ds_tmp="${top_srcdir}/${ds_tmp}"
    if test -d "${ds_tmp}"; then
	INCLUDES="${INCLUDES} -I${ds_tmp}"
    fi
done

AC_SUBST(INCLUDES)
AC_SUBST(LIBS)

AC_HEADER_STDC
AC_C_CONST
AC_TYPE_SIZE_T
AC_FUNC_MALLOC

AC_CHECK_HEADERS([limits.h stddef.h stdlib.h string.h])
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_FUNCS([memmove memset strerror])

#PAGE
## ------------------------------------------------------------
## Finalisation.
## ------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT


### end of file
# Local Variables:
# mode: sh
# End:
