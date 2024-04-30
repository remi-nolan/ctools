/*
 * file: cstream_input.h
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

#ifndef __CSTREAM_INPUT__
#define __CSTREAM_INPUT__

#include "cstream_input_memory.h"
#include "cstream_input_file.h"

#include <stdbool.h>

typedef union cstream_input_source_t
{
   cstream_input_memory_t memory;
   cstream_input_file_t file;
} cstream_input_source_t;

typedef struct cstream_input_t
{
   uint8_t source_type;
   cstream_input_source_t source;
} cstream_input_t;

extern cstream_input_t cstream_standard_in;

bool cstream_input_init_memory(cstream_input_t* stream_input, uint64_t memory_length, void* memory);
bool cstream_input_init_memory_no_copy(cstream_input_t* stream_input, uint64_t memory_length, void* memory);

bool cstream_input_init_file(cstream_input_t* input_stream, char* filename);
bool cstream_input_init_stdin(cstream_input_t* input_stream);

void cstream_input_quit(cstream_input_t* stream_input);

bool cstream_input_valid(cstream_input_t stream_input);

uint64_t cstream_input_read(cstream_input_t* stream_input, uint64_t desired_byte_count, void* destination);

bool cstream_input_read_int8(cstream_input_t* stream_input, int8_t* destination);
bool cstream_input_read_uint8(cstream_input_t* stream_input, uint8_t* destination);

bool cstream_input_read_int16(cstream_input_t* stream_input, int16_t* destination);
bool cstream_input_read_uint16(cstream_input_t* stream_input, uint16_t* destination);

bool cstream_input_read_int32(cstream_input_t* stream_input, int32_t* destination);
bool cstream_input_read_uint32(cstream_input_t* stream_input, uint32_t* destination);

bool cstream_input_read_int64(cstream_input_t* stream_input, int64_t* destination);
bool cstream_input_read_uint64(cstream_input_t* stream_input, uint64_t* destination);

#endif//__CSTREAM_INPUT__
