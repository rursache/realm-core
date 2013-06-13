Tightdb
=======

This README file explains how to build and install the Tightdb core
library.


Prerequisites
-------------

To build the Tightdb core library, you need the standard set of build
tools. This includes a C/C++ compiler and GNU make. Tightdb is
thoroughly tested with both GCC and Clang. It is known to work with
GCC 4.2 and newer, as well as with Clang 3.0 and newer.

If you are going to modify the Tightdb core library, you will need
Cheetah for Python (http://www.cheetahtemplate.org). It is needed
because some source files are generated.

To run the test suite, you will need "UnitTest++"
(http://unittest-cpp.sourceforge.net), however, a bundled fallback
version will be used if `pkg-config unittest++ --exists` fails.

Finally, to run the benchmarking suite (make benchmark) on Linux, you
will need the development part of the 'procps' library.

The following is a suggestion of how to install the prerequisites on
each of our major platforms:

### Ubuntu 10.04 and 12.04

    sudo apt-get install build-essential
    sudo apt-get install python-cheetah
    sudo apt-get install libunittest++-dev
    sudo apt-get install libproc-dev

### Ubuntu 13.04

    sudo apt-get install build-essential
    sudo apt-get install python-cheetah
    sudo apt-get install libunittest++-dev
    sudo apt-get install libprocps0-dev

### Fedora 17

    sudo yum install gcc gcc-c++
    sudo yum install python-cheetah
    sudo yum install procps-devel

### OS X 10.7 and 10.8

    Install Xcode
    Install command line tools (via Xcode)

    Download https://pypi.python.org/packages/source/C/Cheetah/Cheetah-2.4.4.tar.gz
    tar xf Cheetah-2.4.4.tar.gz
    cd Cheetah-2.4.4/
    sudo python setup.py install


Building, testing, and installing
---------------------------------

    sh build.sh config
    sh build.sh clean
    sh build.sh build
    sh build.sh test
    sh build.sh test-debug
    sudo sh build.sh install
    sh build.sh test-intalled

Headers are installed in:

    /usr/local/include/tightdb/

Except for `tightdb.hpp` which is installed as:

    /usr/local/include/tightdb.hpp

The following libraries are installed:

    /usr/local/lib/libtightdb.so
    /usr/local/lib/libtightdb-dbg.so
    /usr/local/lib/libtightdb.a

Note: '.so' is replaced by '.dylib' on OS X.

The following programs are installed:

    /usr/local/bin/tightdb-config
    /usr/local/bin/tightdb-config-dbg

These programs provide the necessary compiler flags for an application
that needs to link against TightDB. They work with GCC and other
compilers, such as Clang, that are mostly command line compatible with
GCC. Here is an example:

    g++  my_app.cpp  `tightdb-config --cflags --libs`

After building, you might want to see exactly what will be installed,
without actually instyalling anything. This can be done as follows:

    DESTDIR=/tmp/check sh build.sh install && find /tmp/check -type f


Configuration
-------------

It is possible to install into a non-default location by running the
following command before building and installing:

    sh build.sh config [PREFIX]

Here, `PREFIX` is the installation prefix. If it is not specified, it
defaults to `/usr/local`.

To use a nondefault compiler, or a compiler in a nondefault location,
set the environment variable `CC` before calling `sh build.sh build`
or `sh build.sh bin-dist`, as in the following example:

    CC=clang sh build.sh bin-dist all

There are also a number of environment variables that serve to enable
or disable special features during building:

Set `TIGHTDB_ENABLE_REPLICATION` to a nonempty value to enable
replication. For example:

    TIGHTDB_ENABLE_REPLICATION=1 sh build.sh src-dist all


Packaging
---------

It is possible to create Debian packages (`.deb`) by running the
following command:

    dpkg-buildpackage -rfakeroot

The packages will be signed by the maintainer's signature.


Building a distribution package
-------------------------------

In general, it is necessary (and crucial) to properly update the
versions of the following shared libraries (do this by editing the the
indicated Makefiles):

    libtightdb.so      (/tightdb/src/tightdb/Makefile)
    libtightdb-c.so    (/tightdb_c/src/tightdb/c/Makefile)
    libtightdb-objc.so (/tightdb_objc/src/tightdb/objc/Makefile)

Please note that these versions are completely independent of each
other and of the package version. When the library versions are set
correctly, do one of the following:

    sh build.sh src-dist all   # Source distribution
    sh build.sh bin-dist all   # Prebuilt core library

If everything went well, consider tagging and then rebuilding the
package:

    git tag -a 'bNNN' -m "New tag for 'Build NNN'"
    git push --tags

This will produce a package whose name and whose top-level directory
is named according to the tag.
