# @configure_input@
#

#page
## ---------------------------------------------------------------------
## Global variables.
## ---------------------------------------------------------------------

CONTAINERS	= btree
#circular hash heap list map tree vector graph

C_WARNINGS := $(filter-out -Wcast-qual,$(C_WARNINGS))

ucl_PREREQUISITES	= $(wildcard $(addprefix $(ucl_SRCDIR)/, internal.h debug.h ucl.h))

$(eval $(call ds-c-library,ucl))
$(eval $(call ds-h-files-installer,ucl_headers,$(ucl_SRCDIR),ucl*.h))

#page
## ---------------------------------------------------------------------
## Tests.
## ---------------------------------------------------------------------

PATTERNS	?= *.c

ucl_tests_PATTERNS		= $(PATTERNS)
ucl_tests_PREREQUISITES		= $(ucl_SRCDIR)/ucl.h $(srcdir)/tests/testmain.h
ucl_tests_CC_COMPILE_MORE	= -I$(ucl_SRCDIR)
ucl_tests_programs_ENV		= LD_LIBRARY_PATH=$(ucl_library_BUILDDIR)
ucl_tests_programs_CC_BUILD_PROGRAM_PRE = -L$(ucl_library_BUILDDIR) -l$(ucl_LIBRARY_ID)

$(eval $(call ds-c-test-programs,ucl))

test-btree:	; $(MAKE) tests PATTERNS=btree.c
test-circular:	; $(MAKE) tests PATTERNS=circular.c
test-hash:	; $(MAKE) tests PATTERNS=hash.c
test-heap:	; $(MAKE) tests PATTERNS=heap.c
test-list:	; $(MAKE) tests PATTERNS=list.c
test-map:	; $(MAKE) tests PATTERNS=map.c
test-tree:	; $(MAKE) tests PATTERNS=tree.c
test-vector:	; $(MAKE) tests PATTERNS=vector.c
test-graph:	; $(MAKE) tests PATTERNS=graph.c

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
