#!/bin/bash
#
# Installation  script to  run from  the  Travis CI  config file  before
# attempting a build.
#
# Install CCTests under the directory "/tmp/mine".  We assume the script
# is run from the top directory of the build tree.

PROGNAME=install-cctests.sh
VERSION=0.3.0-devel.5
STEM="cctests-${VERSION}"
ARCHIVE="${STEM}.tar.gz"
SOURCE_URI="https://github.com/marcomaggi/cctests/archive/v${VERSION}.tar.gz"
LOCAL_ARCHIVE="/tmp/${ARCHIVE}"
TOP_SRCDIR="/tmp/${STEM}"

test -d /tmp/mine || {
    if ! mkdir /tmp/mine
    then
	printf '%s: error creating directory for dependency package building and installation\n' "$PROGNAME" >&2
	exit 1
    fi
}

echo "wget \"$SOURCE_URI\" -O \"$LOCAL_ARCHIVE\"" >&2
if ! wget "$SOURCE_URI" -O "$LOCAL_ARCHIVE"
then
    printf '%s: error downloading %s\n' "$PROGNAME" "${ARCHIVE}" >&2
    exit 1
fi

cd /tmp

echo "tar -xzf \"$LOCAL_ARCHIVE\"" >&2
if ! tar -xzf "$LOCAL_ARCHIVE"
then
    printf '%s: error unpacking %s\n' "$PROGNAME" "$LOCAL_ARCHIVE" >&2
    exit 1
fi

if ! cd "$TOP_SRCDIR"
then
    printf '%s: error changing directory to %s\n' "$PROGNAME" "${TOP_SRCDIR}" >&2
    exit 1
fi

echo "sh autogen.sh" >&2
if [[ "$TRAVIS_OS_NAME" == "osx" ]]
then
    if ! LIBTOOLIZE=glibtoolize sh autogen.sh
    then
	printf '%s: error configuring %s\n' "$PROGNAME" "${STEM}" >&2
	exit 1
    fi
else
    if ! sh autogen.sh
    then
	printf '%s: error configuring %s\n' "$PROGNAME" "${STEM}" >&2
	exit 1
    fi
fi

echo "./configure --prefix=/tmp/mine --enable-maintainer-mode" >&2
if ! ./configure --prefix=/tmp/mine --enable-maintainer-mode
then
    printf '%s: error configuring %s\n' "$PROGNAME" "${STEM}" >&2
    exit 1
fi

echo "make -j2 all" >&2
if ! make -j2 all
then
    printf '%s: error configuring %s\n' "$PROGNAME" "${STEM}" >&2
    exit 1
fi

echo "make install" >&2
if ! make install
then
    printf '%s: error configuring %s\n' "$PROGNAME" "${STEM}" >&2
    exit 1
fi

exit 0

# Local Variables:
# mode: sh
# End:
