## Process this file with automake to produce Makefile.in

EXTRA_DIST	= mcl-common.h mcl-debug.h mcl-test.h

check_PROGRAMS	= \
	bst		\
	btree		\
	circular	\
	graph		\
	hash		\
	heap		\
	list		\
	map		\
	tree		\
	vector

# mcl-common.h
# mcl-debug.h
# mcl-test.h

AM_CPPFLAGS	= -I$(top_srcdir)/src
LDADD		= ../src/libucl.la
TESTS		= $(check_PROGRAMS)

### end of file
