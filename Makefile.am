## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL configure.sh
dist_doc_DATA		= README COPYING
AM_CFLAGS		= -Wall
AM_CPPFLAGS		= -I$(srcdir)/src -I$(srcdir)/tests

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ucl.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ucl.pc

#page
#### documentation

AM_MAKEINFOFLAGS	= --no-split

info_TEXINFOS		= doc/ucl.texi
doc_ucl_TEXINFOS	= \
	doc/macros.texi		\
	doc/gpl-3.0.texi	\
	doc/fdl-1.3.texi

#page
#### library

ucl_CURRENT		= @ucl_VERSION_INTERFACE_CURRENT@
ucl_REVISION		= @ucl_VERSION_INTERFACE_REVISION@
ucl_AGE			= @ucl_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ucl.h

lib_LTLIBRARIES		= libucl.la
libucl_la_LDFLAGS	= -version-info $(ucl_CURRENT):$(ucl_REVISION):$(ucl_AGE)
libucl_la_SOURCES	= \
	src/debug.h		\
	src/internal.h		\
	src/ucl-version.c	\
	src/ucl.c		\
	src/ucl.h		\
	src/ucl-bst.c		\
	src/ucl-btree.c		\
	src/ucl-circular.c	\
	src/ucl-graph.c		\
	src/ucl-hash.c		\
	src/ucl-heap.c		\
	src/ucl-list.c		\
	src/ucl-map.c		\
	src/ucl-memory.c	\
	src/ucl-tree.c		\
	src/ucl-vector.c

#page
#### tests

EXTRA_DIST	+= \
	tests/mcl-common.h	\
	tests/mcl-debug.h	\
	tests/mcl-test.h

check_PROGRAMS	= \
	tests/bst		\
	tests/btree		\
	tests/circular		\
	tests/graph		\
	tests/hash		\
	tests/heap		\
	tests/list		\
	tests/map		\
	tests/tree		\
	tests/vector

tests_bst_LDADD	= libucl.la
tests_btree_LDADD	= libucl.la
tests_circular_LDADD	= libucl.la
tests_graph_LDADD	= libucl.la
tests_hash_LDADD	= libucl.la
tests_heap_LDADD	= libucl.la
tests_list_LDADD	= libucl.la
tests_map_LDADD		= libucl.la
tests_tree_LDADD	= libucl.la
tests_vector_LDADD	= libucl.la

TESTS		= $(check_PROGRAMS)

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done

### end of file
