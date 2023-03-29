# ctalk
A minimalist c99 data-stream library by Remi Nolan.

As of right now, it only supports reading from a memory buffer or a file, however support for writing to both is in development.

Currently it supports the C-Standard Library for file i/o however platform-specific alternatives are in development

## Adding to Your Project

CTalk is implemented as a header-only library. That means you use `#include <ctalk.h>` as you would normally.

However in *one* translation unit, you to put `#define CTALK_IMPLEMENTATION` (or `#define CTOOLS_IMPLEMENTATION` if you're implementing multi ctools projects in the same translation unit) before `#include <ctalk.h>` which will tell the header file to act like a source file that has been included in the translation unit.

## Testing

Running `make` from the directory containing ctalk & its makefile will compile `ctalk_test.c` and run the resulting executable.

## License:

ctalk is licensed under the anti-capitalist software license (v 1.4), seee LICENSE for more details

