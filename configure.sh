# configure.sh --
#

set -ex

prefix=/usr/local

../configure \
    --config-cache				\
    --cache-file=../config.cache		\
    --enable-maintainer-mode                    \
    --disable-static --enable-shared            \
    --prefix="${prefix}"			\
    CFLAGS='-O3'				\
    "$@"

### end of file
