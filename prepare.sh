# prepare.sh --

set -x

(cd ..
    if test ! -f config.h.in ; then
        autoheader
    fi)
(cd ..
    if test configure.ac -nt configure -o \
        ../configuration/configure.ds -nt configure -o \
        ../infrastructure/acmacros.m4 -nt configure
        then
        autoconf
        autoheader
    fi)

../configure "$@"

### end of file
