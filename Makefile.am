## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL
dist_doc_DATA		= README COPYING
AM_CFLAGS		= $(MMUX_CFLAGS)
AM_CPPFLAGS		= -I$(srcdir)/src

## --------------------------------------------------------------------

nodist_pkgconfig_DATA	= meta/scripts/ucl.pc


#### documentation

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/ucl.texi
doc_ucl_TEXINFOS	= \
	doc/macros.texi		\
	doc/gpl-3.0.texi	\
	doc/fdl-1.3.texi


#### library

ucl_CURRENT		= @ucl_VERSION_INTERFACE_CURRENT@
ucl_REVISION		= @ucl_VERSION_INTERFACE_REVISION@
ucl_AGE			= @ucl_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ucl.h

lib_LTLIBRARIES		= libucl.la
libucl_la_CPPFLAGS	= $(AM_CPPFLAGS)
libucl_la_CFLAGS	= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
libucl_la_LDFLAGS	= -version-info $(ucl_CURRENT):$(ucl_REVISION):$(ucl_AGE) \
	$(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS)
libucl_la_SOURCES	= \
	src/ucl-debug.h		\
	src/ucl-internals.h	\
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
	tests/vector		\
	tests/version

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ucl_tests_cppflags	= $(AM_CPPFLAGS) -I$(srcdir)/tests
ucl_tests_cflags	= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS) $(CCTESTS_CFLAGS)
ucl_tests_ldadd		= libucl.la $(CCTESTS_LIBS)

## --------------------------------------------------------------------

# tests_condition_objects_CPPFLAGS	= $(ucl_tests_cppflags)
# tests_condition_objects_CFLAGS	= $(ucl_tests_cflags)
# tests_condition_objects_LDADD		= $(ucl_tests_ldadd)

tests_bst_CPPFLAGS	= $(ucl_tests_cppflags)
tests_bst_CFLAGS	= $(ucl_tests_cflags)
tests_bst_LDADD		= $(ucl_tests_ldadd)

tests_btree_CPPFLAGS	= $(ucl_tests_cppflags)
tests_btree_CFLAGS	= $(ucl_tests_cflags)
tests_btree_LDADD	= $(ucl_tests_ldadd)

tests_circular_CPPFLAGS	= $(ucl_tests_cppflags)
tests_circular_CFLAGS	= $(ucl_tests_cflags)
tests_circular_LDADD	= $(ucl_tests_ldadd)

tests_graph_CPPFLAGS	= $(ucl_tests_cppflags)
tests_graph_CFLAGS	= $(ucl_tests_cflags)
tests_graph_LDADD	= $(ucl_tests_ldadd)

tests_hash_CPPFLAGS	= $(ucl_tests_cppflags)
tests_hash_CFLAGS	= $(ucl_tests_cflags)
tests_hash_LDADD	= $(ucl_tests_ldadd)

tests_heap_CPPFLAGS	= $(ucl_tests_cppflags)
tests_heap_CFLAGS	= $(ucl_tests_cflags)
tests_heap_LDADD	= $(ucl_tests_ldadd)

tests_list_CPPFLAGS	= $(ucl_tests_cppflags)
tests_list_CFLAGS	= $(ucl_tests_cflags)
tests_list_LDADD	= $(ucl_tests_ldadd)

tests_map_CPPFLAGS	= $(ucl_tests_cppflags)
tests_map_CFLAGS	= $(ucl_tests_cflags)
tests_map_LDADD		= $(ucl_tests_ldadd)

tests_tree_CPPFLAGS	= $(ucl_tests_cppflags)
tests_tree_CFLAGS	= $(ucl_tests_cflags)
tests_tree_LDADD	= $(ucl_tests_ldadd)

tests_vector_CPPFLAGS	= $(ucl_tests_cppflags)
tests_vector_CFLAGS	= $(ucl_tests_cflags)
tests_vector_LDADD	= $(ucl_tests_ldadd)

tests_version_CPPFLAGS	= $(ucl_tests_cppflags)
tests_version_CFLAGS	= $(ucl_tests_cflags)
tests_version_LDADD	= $(ucl_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
