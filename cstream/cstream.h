/*
 * cstream - cstream.h
 * a minimalist c99 data-stream library by Remi Nolan
 *
 * License:
 * ANTI-CAPITALIST SOFTWARE LICENSE (v 1.4)
 *
 * Copyright © 2023 Remi Nolan Eidahl
 * This is anti-capitalist software, released for free use by individuals and organizations that do not operate by capitalist principles.
 *
 * Permission is hereby granted, free of charge, to any person or organization (the "User") obtaining a copy of this software and associated documentation files (the "Software"), to use, copy, modify, merge, distribute, and/or sell copies of the Software, subject to the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be included in all copies or modified versions of the Software.
 * 2. The User is one of the following: a. An individual person, laboring for themselves
 * b. A non-profit organization
 * c. An educational institution
 * d. An organization that seeks shared profit for all of its members, and allows non-members to set the cost of their labor
 *
 * 3. If the User is an organization with owners, then all owners are workers and all workers are owners with equal equity and/or equal vote.
 *
 * 4. If the User is an organization, then the User is not law enforcement or military, or working for or under either.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CSTREAM_INCLUDED
#define CSTREAM_INCLUDED

#ifndef CTOOLS_NO_STANDARD_INCLUDE
#include <stdint.h>
#else
#define int8_t    signed char
#define int16_t   signed short
#define int32_t   signed long
#define int64_t   signed long long
#define uint8_t   unsigned char
#define uint16_t  unsigned short
#define uint32_t  unsigned long
#define uint64_t  unsigned long long
#endif

enum cstream_error {
   CStreamError_None          = 0,
   CStreamError_EndOfStream   = 1,
   CStreamError_Unspecified   = 5, //NOTE: really only used with the C-Standard Library, which doesn't give good errors

   CStreamError_NullPointer   = 2,
   CStreamError_InvalidArg    = 3,
   CStreamError_FileNotFound  = 4,
};
int cstream_error();

enum cstream_type {
   CStreamType_Unknown   = 0,

   CStreamType_Memory    = 1,
   CStreamType_File      = 2,
};

enum cstream_file_flags {
   CStream_TextFile   = (1<<0),
   CStream_SwapBytes  = (1<<1),
   CStream_IsWrite    = (1<<2),
};

typedef struct cstream_s {
   int type;
   int flags;

   union {
      struct {
         uint32_t position;
         uint32_t length;
      };
      struct {
         uint32_t bytes_written;
         uint32_t bytes_max;
      };
   };

   void* handle;
} cstream_t;


int cstream_read_memory(cstream_t* stream, void* memory, uint32_t num_bytes, int flags);
int cstream_read_file(cstream_t* stream, char* filename, int flags);

int cstream_read_8bits(cstream_t* stream, int8_t* out);
int cstream_read_16bits(cstream_t* stream, int16_t* out);
int cstream_read_32bits(cstream_t* stream, int32_t* out);
int cstream_read_64bits(cstream_t* stream, int64_t* out);


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t max_bytes, int flags);
int cstream_write_file(cstream_t* stream, char* filename, uint32_t max_bytes, int flags);

int cstream_write_8bits(cstream_t* stream, int8_t val);
int cstream_write_16bits(cstream_t* stream, int16_t val);
int cstream_write_32bits(cstream_t* stream, int32_t val);
int cstream_write_64bits(cstream_t* stream, int64_t val);


int cstream_readable(cstream_t* stream);
int cstream_writable(cstream_t* stream);

int cstream_quit(cstream_t* stream);
int cstream_valid(cstream_t* stream);
int cstream_rewind(cstream_t* stream, uint32_t amount);
int cstream_fast_forward(cstream_t* stream, uint32_t amount);

#endif//CSTREAM_INCLUDED
