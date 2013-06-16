## Process this file with automake to produce Makefile.in

AM_CFLAGS		= -Wall

ucl_CURRENT		= @ucl_VERSION_INTERFACE_CURRENT@
ucl_REVISION		= @ucl_VERSION_INTERFACE_REVISION@
ucl_AGE			= @ucl_VERSION_INTERFACE_AGE@

include_HEADERS		= ucl.h

lib_LTLIBRARIES		= libucl.la
libucl_la_LDFLAGS	= -version-info $(ucl_CURRENT):$(ucl_REVISION):$(ucl_AGE)
libucl_la_SOURCES	= \
	debug.h			\
	internal.h		\
	ucl-version.c		\
	ucl.c			\
	ucl.h			\
	ucl-bst.c		\
	ucl-btree.c		\
	ucl-circular.c		\
	ucl-graph.c		\
	ucl-hash.c		\
	ucl-heap.c		\
	ucl-list.c		\
	ucl-map.c		\
	ucl-memory.c		\
	ucl-tree.c		\
	ucl-vector.c

### end of file
