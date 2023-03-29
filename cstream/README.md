# cstream
A minimalist c99 data-stream library by Remi Nolan.

As of right now, it only supports reading from a memory buffer or a file, however support for writing to both is in development.

Currently it supports the C-Standard Library for file i/o however platform-specific alternatives are in development

## Adding to Your Project

Simply add `cstream.h` to you include path and `cstream.c` to your build, and you're off to the races.

## Testing

Running `make` from the directory containing cstream & its makefile will compile `cstream_test.c` and run the resulting executable.

## License:

cstream is licensed under the anti-capitalist software license (v 1.4), seee LICENSE for more details

