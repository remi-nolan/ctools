/*
 * file: cstream_output.h
 * author: Remi Eidahl
 *
 * license:
 *    ANTI-CAPITALIST SOFTWARE LICENSE (v 1.4)
 *
 *    Copyright © 2024 Remi Eidahl
 *
 *    This is anti-capitalist software, released for free use by individuals and organizations that do not operate by capitalist principles.
 *
 *    Permission is hereby granted, free of charge, to any person or organization (the "User") obtaining a copy of this software and associated documentation files (the "Software"), to use, copy, modify, merge, distribute, and/or sell copies of the Software, subject to the following conditions:
 *
 *    1. The above copyright notice and this permission notice shall be included in all copies or modified versions of the Software.
 *
 *    2. The User is one of the following:
 *    a. An individual person, laboring for themselves
 *    b. A non-profit organization
 *    c. An educational institution
 *    d. An organization that seeks shared profit for all of its members, and allows non-members to set the cost of their labor
 *
 *    3. If the User is an organization with owners, then all owners are workers and all workers are owners with equal equity and/or equal vote.
 *
 *    4. If the User is an organization, then the User is not law enforcement or military, or working for or under either.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __CSTREAM_OUTPUT__
#define __CSTREAM_OUTPUT__

#include <stdint.h>
#include <stdbool.h>

#include "cstream_output_memory.h"
#include "cstream_output_file.h"

typedef union cstream_output_source_t
{
   cstream_output_memory_t memory;
   cstream_output_file_t file;
} cstream_output_source_t;

typedef struct cstream_output_t
{
   uint32_t source_type;
   cstream_output_source_t source;
} cstream_output_t;

extern cstream_output_t cstream_standard_out;
extern cstream_output_t cstream_standard_error;

bool cstream_output_init_memory(cstream_output_t* output_stream, uint64_t memory_length, void* memory);
bool cstream_output_init_memory_allocate(cstream_output_t* output_stream, uint64_t memory_length);

bool cstream_output_init_file(cstream_output_t* output_stream, char* file_name);
bool cstream_output_init_stdout(cstream_output_t* output_stream);
bool cstream_output_init_stderr(cstream_output_t* output_stream);

void cstream_output_quit(cstream_output_t* output_stream);

bool cstream_output_valid(cstream_output_t output_stream);

uint64_t cstream_output_write(cstream_output_t* output_stream, uint64_t byte_count, void* source);

bool cstream_output_write_int8(cstream_output_t* output_stream, int8_t source);
bool cstream_output_write_uint8(cstream_output_t* output_stream, uint8_t source);

bool cstream_output_write_int16(cstream_output_t* output_stream, int16_t source);
bool cstream_output_write_uint16(cstream_output_t* output_stream, uint16_t source);

bool cstream_output_write_int32(cstream_output_t* output_stream, int32_t source);
bool cstream_output_write_uint32(cstream_output_t* output_stream, uint32_t source);

bool cstream_output_write_int64(cstream_output_t* output_stream, int64_t source);
bool cstream_output_write_uint64(cstream_output_t* output_stream, uint64_t source);

uint64_t cstream_output_write_string(cstream_output_t* output_stream, const char* format_string, ...);

#endif//__CSTREAM_OUTPUT__
