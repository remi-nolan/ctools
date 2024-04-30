/*
 * file: cstream_input.c
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

#include "cstream.h"

#include <stdio.h>

static cstream_input_file_t cstream_internal_stdin = {
   .length = -1,
   .position = 0,
   .handle = 0
};
cstream_input_t cstream_standard_in = { .source_type = CSTREAM_STDIN };

bool cstream_input_init_memory(cstream_input_t* stream_input, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_MEMORY;
      result = cstream_input_memory_init(&stream_input->source.memory, memory_length, memory);
   }

   return(result);
}

bool cstream_input_init_memory_no_copy(cstream_input_t* stream_input, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_MEMORY;
      result = cstream_input_memory_init_no_copy(&stream_input->source.memory, memory_length, memory);
   }

   return(result);
}

bool cstream_input_init_stdin(cstream_input_t* input_stream)
{
   bool result = false;

   if(input_stream && !cstream_input_valid(*input_stream))
   {
      input_stream->source_type = CSTREAM_STDIN;
      input_stream->source = (cstream_input_source_t){0};
   }

   return(result);
}

bool cstream_input_init_file(cstream_input_t* stream_input, char* file_name)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_FILE;
      result = cstream_input_file_init(&stream_input->source.file, file_name);
   }

   return(result);
}

void cstream_input_quit(cstream_input_t* stream_input)
{
   if(stream_input)
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_input_memory_quit(&stream_input->source.memory);
         break;
      case CSTREAM_FILE:
         cstream_input_file_quit(&stream_input->source.file);
         break;
      }

      stream_input->source_type = CSTREAM_UNKNOWN;
      stream_input->source = (cstream_input_source_t){0};
   }
}

bool cstream_input_valid(cstream_input_t stream_input)
{
   return(stream_input.source_type != CSTREAM_UNKNOWN);
}

uint64_t cstream_input_read(cstream_input_t* stream_input, uint64_t desired_byte_count, void* destination)
{
   uint64_t result = 0;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read(&stream_input->source.memory, desired_byte_count, destination);
         break;
      case CSTREAM_FILE:
         result = cstream_input_file_read(&stream_input->source.file, desired_byte_count, destination);
         break;
      case CSTREAM_STDIN:
         if(cstream_internal_stdin.handle == 0)
         {
            cstream_internal_stdin.handle = stdin;
         }

         result = cstream_input_file_read(&cstream_internal_stdin, desired_byte_count, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_int8(cstream_input_t* stream_input, int8_t* destination)
{
   return(cstream_input_read(stream_input, 1, destination) == 1);
}

bool cstream_input_read_uint8(cstream_input_t* stream_input, uint8_t* destination)
{
   return(cstream_input_read(stream_input, 1, destination) == 1);
}

bool cstream_input_read_int16(cstream_input_t* stream_input, int16_t* destination)
{
   return(cstream_input_read(stream_input, 2, destination) == 2);
}

bool cstream_input_read_uint16(cstream_input_t* stream_input, uint16_t* destination)
{
   return(cstream_input_read(stream_input, 2, destination) == 2);
}

bool cstream_input_read_int32(cstream_input_t* stream_input, int32_t* destination)
{
   return(cstream_input_read(stream_input, 4, destination) == 4);
}

bool cstream_input_read_uint32(cstream_input_t* stream_input, uint32_t* destination)
{
   return(cstream_input_read(stream_input, 4, destination) == 4);
}

bool cstream_input_read_int64(cstream_input_t* stream_input, int64_t* destination)
{
   return(cstream_input_read(stream_input, 8, destination) == 8);
}

bool cstream_input_read_uint64(cstream_input_t* stream_input, uint64_t* destination)
{
   return(cstream_input_read(stream_input, 8, destination) == 8);
}

