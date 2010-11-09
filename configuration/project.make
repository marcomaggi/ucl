# @configure_input@
#

#page
## ---------------------------------------------------------------------
## Global variables.
## ---------------------------------------------------------------------

CONTAINERS	= btree
#circular hash heap list map tree vector graph

C_WARNINGS := $(filter-out -Wcast-qual,$(C_WARNINGS))

ucl_PREREQUISITES	= $(wildcard $(addprefix $(ucl_SRCDIR)/,\
				internal.h debug.h ucl.h))

$(eval $(call ds-c-library,ucl))
$(eval $(call ds-h-files-installer,ucl_headers,$(ucl_SRCDIR),ucl_*.h ucl-*.h))


#page
## ---------------------------------------------------------------------
## Tests.
## ---------------------------------------------------------------------

PATTERNS	?= *.c

ucl_tests_MAIN_SECTION		= shared-tests
ucl_tests_programs_MAIN_SECTION	= shared-tests

ucl_tests_PATTERNS		= $(PATTERNS)
ucl_tests_BUILDDIR		= $(builddir)/shared-tests.d
ucl_tests_PREREQUISITES		= $(ucl_SRCDIR)/ucl.h $(srcdir)/tests/testmain.h
ucl_tests_CC_COMPILE_MORE_FLAGS	= -I$(ucl_SRCDIR)
ucl_tests_programs_PREFIX	= shared-
ucl_tests_programs_ENV		= LD_LIBRARY_PATH=$(ucl_library_BUILDDIR)
ucl_tests_programs_CC_BUILD_PROGRAM_PRE = \
	-L$(ucl_library_BUILDDIR) -l$(ucl_LIBRARY_ID)

$(eval $(call ds-c-test-programs,ucl_shared))

test:		shared-tests
test-clean:	shared-tests-clean
test-realclean:	shared-tests-realclean

test-btree:	; $(MAKE) test PATTERNS=btree.c
test-circular:	; $(MAKE) test PATTERNS=circular.c
test-hash:	; $(MAKE) test PATTERNS=hash.c
test-heap:	; $(MAKE) test PATTERNS=heap.c
test-list:	; $(MAKE) test PATTERNS=list.c
test-map:	; $(MAKE) test PATTERNS=map.c
test-tree:	; $(MAKE) test PATTERNS=tree.c
test-vector:	; $(MAKE) test PATTERNS=vector.c
test-graph:	; $(MAKE) test PATTERNS=graph.c

#page
## ---------------------------------------------------------------------
## Miscelleous stuff.
## ---------------------------------------------------------------------

ucl_examples_PREREQUISITES		= $(ucl_SRCDIR)/ucl.h
ucl_examples_CC_COMPILE_MORE_FLAGS	= -I$(ucl_SRCDIR) -I$(ucl_stubsrc_BUILDDIR)
ucl_examples_programs_ENV		= LD_LIBRARY_PATH=$(ucl_shared_library_BUILDDIR)

$(eval $(call ds-c-example-programs,ucl))


### end of file
# Local Variables:
# mode: makefile
# End:
