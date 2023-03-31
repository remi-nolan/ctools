# ctalk
A minimalist c99 data-stream library by Remi Nolan.

Currently it supports the C-Standard Library for file i/o however platform-specific alternatives are currently in development

The C Standard Library (and, C Standard includes) will be entirely optional once the custom api is done (stdlib will not used by default, though it will be an option to be enabled with `#define CTOOLS_FORCE_STDIO`), and the Standard Include can be disabled by using `#define CTOOLS_NO_STDINC` (in which case it will define its own versions (using `#define` no they can be `undef`'d if needed).

## Adding to Your Project

CTalk is implemented as a header-only library. That means you use `#include <ctalk.h>` as you would normally.

However in *one* translation unit, you to put `#define CTALK_IMPLEMENTATION` (or `#define CTOOLS_IMPLEMENTATION` if you're implementing multi ctools projects in the same translation unit) before `#include <ctalk.h>` which will tell the header file to act like a source file that has been included in the translation unit.

## Testing

Running `make` from the directory containing ctalk & its makefile will compile `ctalk_test.c` and run the resulting executable.

## License:

ctalk is licensed under the anti-capitalist software license (v 1.4), seee LICENSE for more details

