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

## --------------------------------------------------------------------

ucl_container_run_tests	=	\
	run-test-btree		run-test-circular	\
	run-test-hash		run-test-tree		\
	run-test-vector		run-test-heap

.PHONY: $(ucl_container_run_tests)

run-test-btree:
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-btree

run-test-circular:
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-circular

run-test-hash:
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-hash

run-test-tree:
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-tree

run-test-vector:
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-vector

run-test-heap:
	$(ucl_tests_programs_ENV) $(ucl_tests_programs_BUILDDIR)/test-heap

## --------------------------------------------------------------------

ucl_container_tests	=	\
	test-btree		test-circular		\
	test-hash		test-tree		\
	test-vector		test-heap

.PHONY: $(ucl_container_tests)

# test-list:	; $(MAKE) tests PATTERNS=list.c
# test-map:	; $(MAKE) tests PATTERNS=map.c
# test-graph:	; $(MAKE) tests PATTERNS=graph.c

test-btree: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-btree.c
	$(MAKE) run-test-btree

test-circular: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-circular.c
	$(MAKE) run-test-circular

test-hash: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-hash.c
	$(MAKE) run-test-hash

test-tree: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-tree.c
	$(MAKE) run-test-tree

test-vector: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-vector.c
	$(MAKE) run-test-vector

test-heap: ucl_shlib-all
	$(MAKE) tests PATTERNS=test-heap.c
	$(MAKE) run-test-heap

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
