# prepare.sh --

set -x

(cd ..
    if test ! -f config.h.in ; then
        autoheader
    fi)
(cd ..
    if test configure.ac -nt configure -o configure.ds -nt configure ; then
        autoconf
        autoheader
    fi)

../configure CFLAGS='-march=i686 -g -O3' --enable-static "$@"

### end of file
