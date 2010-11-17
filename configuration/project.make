# @configure_input@
#

#page
## ---------------------------------------------------------------------
## Global variables.
## ---------------------------------------------------------------------

GCC_WARNINGS := $(filter-out -Wcast-qual,$(GCC_WARNINGS))

ucl_PREREQUISITES	= $(wildcard $(addprefix $(ucl_SRCDIR)/, internal.h debug.h ucl.h))

$(eval $(call ds-c-library,ucl))
$(eval $(call ds-h-files-installer,ucl_headers,$(ucl_SRCDIR),ucl*.h))

#page
## ---------------------------------------------------------------------
## Tests.
## ---------------------------------------------------------------------

PATTERNS	?= test-*.c

ucl_tests_PATTERNS		= $(PATTERNS)
ucl_tests_PREREQUISITES		= $(ucl_PREREQUISITES)
ucl_tests_CC_COMPILE_MORE	= -I$(ucl_SRCDIR)
ucl_tests_programs_ENV		= LD_LIBRARY_PATH=$(ucl_shlib_BUILDDIR)
ucl_tests_programs_CC_PROGRAM_LIBS = -L$(ucl_shlib_BUILDDIR) -l$(ucl_LIBRARY_ID)

$(eval $(call ds-c-test-programs,ucl))

test-circular:	; $(MAKE) tests PATTERNS=circular.c
test-hash:	; $(MAKE) tests PATTERNS=hash.c
test-heap:	; $(MAKE) tests PATTERNS=heap.c
test-list:	; $(MAKE) tests PATTERNS=list.c
test-map:	; $(MAKE) tests PATTERNS=map.c
test-graph:	; $(MAKE) tests PATTERNS=graph.c

test-btree: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-btree.c
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-btree

test-tree: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-tree.c
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-tree

test-vector: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-vector.c
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-vector

#page
## ---------------------------------------------------------------------
## Miscelleous stuff.
## ---------------------------------------------------------------------

ucl_examples_PREREQUISITES	= $(ucl_SRCDIR)/ucl.h
ucl_examples_CC_COMPILE_MORE	= -I$(ucl_SRCDIR)
ucl_examples_programs_ENV	= LD_LIBRARY_PATH=$(ucl_shared_library_BUILDDIR)

$(eval $(call ds-c-example-programs,ucl))


### end of file
# Local Variables:
# mode: makefile
# End:
