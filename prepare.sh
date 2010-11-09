# prepare.sh --

set -x

(cd ..
    test -f config.h.in || autoheader
    if test \
        configure -ot configure.ac -o \
        configure -ot aclocal.m4   -o \
        configure -ot infrastructure/develstuff.autoconf
    then
        autoconf
        autoheader
    fi)

../configure \
    --with-abi=local-slackware                          \
    --enable-static                                     \
    --enable-doc-html                                   \
    --enable-doc-pdf                                    \
    --enable-doc-dvi                                    \
    --enable-doc-ps                                     \
    CFLAGS="-O3 -g -march=i686 -mtune=i686" "$@"

### end of file
