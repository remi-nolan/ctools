
# ctools

A collection of minimalist helper libraries written in pure c99 by Remi Nolan, inspired by Sean Barrett's [stb libraries](https://github.com/nothings/stb).

Essentially just a collection of useful little libraries that don't necessitate a larger project of their own.

All of the libraries can be compiled without linking against the standard library. However -- the libraries do use standard ints, however these can be disabled by using `CTOOLS_NO_STANDARD_INCLUDE`.

## in this repository

Each library is in its own directory (i.e. `ctalk` is in `<ctools>/ctalk/`), containing a header file and a source file.
The header file contains the entire library and is all you need to include the library in your project.
The source file is a small program designed to test each feature of the library, and functions as an example use of the library.

Here is a list of the libraries and a short description:

library | description
------: | :----------
ctalk   | a file i/o api and data stream api
cbug    | a debug-break & assert toolkit

## testing ctools

There is a (platform-independent) makefile in the root of the repository that will compile & run each of the test programs, which outputs to stdout and reports and reports errors verbosely.

