dnl @configure_input@
dnl

AC_PREREQ([2.69])
AC_INIT([Useless Containers Library],[2.0d1],[marco.maggi-ipsu@poste.it],
  [ucl],[http://github.com/marcomaggi/ucl])
AC_SUBST([MY_PKG_CONFIG_VERSION],[2.0])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([meta/autotools])
AC_CONFIG_AUX_DIR([meta/autotools])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall foreign dist-xz no-dist-gzip subdir-objects])
AM_MAINTAINER_MODE

AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4])
LT_INIT

#page
#### interface version

ucl_VERSION_INTERFACE_CURRENT=3
ucl_VERSION_INTERFACE_REVISION=0
ucl_VERSION_INTERFACE_AGE=0
AC_DEFINE_UNQUOTED([ucl_VERSION_INTERFACE_CURRENT],
  [$ucl_VERSION_INTERFACE_CURRENT],
  [current interface number])
AC_DEFINE_UNQUOTED([ucl_VERSION_INTERFACE_REVISION],
  [$ucl_VERSION_INTERFACE_REVISION],
  [current interface implementation number])
AC_DEFINE_UNQUOTED([ucl_VERSION_INTERFACE_AGE],
  [$ucl_VERSION_INTERFACE_AGE],
  [current interface age number])
AC_DEFINE_UNQUOTED([ucl_VERSION_INTERFACE_STRING],
  ["$ucl_VERSION_INTERFACE_CURRENT.$ucl_VERSION_INTERFACE_REVISION"],
  [library interface version])
AC_SUBST([ucl_VERSION_INTERFACE_CURRENT])
AC_SUBST([ucl_VERSION_INTERFACE_REVISION])
AC_SUBST([ucl_VERSION_INTERFACE_AGE])

#page
#### basic system inspection

AC_LANG([C])
dnl AC_DEFINE([_GNU_SOURCE],[1],[enable everything GNU])
AC_PROG_CC_C99
AM_PROG_AS
AM_PROG_CC_C_O
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([assert.h stdint.h strings.h errno.h stddef.h sys/mman.h sys/resource.h unistd.h])
AC_CHECK_SIZEOF(void *)
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_LONG_LONG_INT
AC_TYPE_UNSIGNED_LONG_LONG_INT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

dnl UCL_CHECK_PAGESIZE
dnl AC_DEFINE_UNQUOTED([CCG_PAGESIZE],[$ucl_cv_pagesize],[Platform page size.])
dnl UCL_CHECK_PAGESHIFT($ucl_cv_pagesize)
dnl AC_DEFINE_UNQUOTED([CCG_PAGESHIFT],[$ucl_cv_pageshift],[Page shift bit count.])

AC_CACHE_SAVE

#page
#### checks for library functions

AC_FUNC_MALLOC
AC_FUNC_MEMCMP
dnl AC_CHECK_FUNCS([memmove memset strerror])
dnl AC_CHECK_FUNCS([mmap munmap msync mremap madvise mlock munlock mlockall munlockall mprotect])
dnl AC_CHECK_FUNCS([getenv])

AC_CACHE_SAVE

#page
#### finish

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile]
  [meta/slackware/slack-desc]
  [meta/scripts/ucl.pc])
AC_OUTPUT

### end of file
# Local Variables:
# mode: autoconf
# page-delimiter: "^#page"
# End:
