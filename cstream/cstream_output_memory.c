/*
 * file: cstream_output_memory.c
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

#include <stdlib.h>

bool cstream_output_memory_init(cstream_output_memory_t* memory_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(memory_stream && memory_length && memory)
   {
      memory_stream->position = 0;
      memory_stream->length = memory_length;
      memory_stream->memory = memory;

      result = true;
   }

   return(result);
}

bool cstream_output_memory_init_allocate(cstream_output_memory_t* memory_stream, uint64_t memory_length)
{
   bool result = false;

   if(memory_stream && memory_length)
   {
      memory_stream->memory = malloc(memory_length);

      if(memory_stream->memory)
      {
         memory_stream->position = 0;
         memory_stream->length = memory_length;

         result = true;
      }
   }

   return(result);
}

void cstream_output_memory_quit(cstream_output_memory_t* memory_stream)
{
   if(memory_stream && memory_stream->memory)
   {
      free(memory_stream->memory);
      *memory_stream = (cstream_output_memory_t){0};
   }
}

bool cstream_output_memory_valid(cstream_output_memory_t memory_stream)
{
   return(memory_stream.memory && memory_stream.length);
}

uint64_t cstream_output_memory_write(cstream_output_memory_t* memory_stream, uint64_t desired_byte_count, void* value)
{
   uint64_t bytes_written = 0;

   if(memory_stream && memory_stream->memory)
   {
      uint8_t* memory_bytes = ((uint8_t*)memory_stream->memory) + memory_stream->position;
      uint8_t* value_bytes = (uint8_t*)value;

      if((memory_stream->position + desired_byte_count) >= memory_stream->length)
      {
         desired_byte_count = memory_stream->length - memory_stream->position;
      }

      for(uint64_t iter = 0; iter < desired_byte_count; ++iter)
      {
         memory_bytes[iter] = value_bytes[iter];
         memory_stream->position++;
         bytes_written++;
      }
   }

   return(bytes_written);
}

