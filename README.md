
# ctools

A collection of minimalist helper libraries written in pure c99 by Remi Nolan, inspired by Sean Barrett's [stb libraries](https://github.com/nothings/stb).

Essentially just a collection of useful little libraries that don't necessitate a larger project of their own.

All of the libraries can be compiled without linking against the standard library. However -- the libraries do use standard ints, however these can be disabled by using `CTOOLS_NO_STANDARD_INCLUDE`.

## in this repository

Each library is in its own directory (i.e. `cstream` is in `<ctools>/cstream/`), containing a header file and two source files (the library's source file and a test program's source file).

A list of the libraries and a short description is as follows:

library | description
------: | :----------
cstream | a cross-platform file i/o api and file/memory stream api

## testing ctools

There is a (platform-independent) makefile in the root of the repository that will compile & run each of the test programs, which outputs to stdout and reports and reports errors verbosely.

