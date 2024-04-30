/*
 * file: cstream_input_memory.c
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

#include "cstream_input_memory.h"

#include <stdlib.h>

bool cstream_input_memory_init(cstream_input_memory_t* memory_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(memory_stream && memory)
   {
      uint8_t* memory_bytes = (uint8_t*)memory;

      if(memory_length == 0)
      {
         while(*memory_bytes++ != 0)
         {
            memory_length++;
         }
         memory_bytes = memory;
      }

      memory_stream->length = memory_length;
      memory_stream->position = 0;

      memory_stream->buffer = malloc(memory_length);

      if(memory_stream->buffer)
      {
         uint8_t* buffer_bytes = (uint8_t*)memory_stream->buffer;
         for(uint64_t iter = 0; iter < memory_length; ++iter)
         {
            *buffer_bytes++ = *memory_bytes++;
         }

         result = true;
      }
   }

   return(result);
}

bool cstream_input_memory_init_no_copy(cstream_input_memory_t* memory_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(memory_stream && memory)
   {
      if(memory_length == 0)
      {
         uint8_t* memory_bytes = (uint8_t*)memory;
         while(*memory_bytes++ != 0)
         {
            memory_length++;
         }
         memory_bytes = memory;
      }

      memory_stream->length = memory_length;
      memory_stream->position = 0;
      memory_stream->buffer = memory;

      result = true;
   }

   return(result);
}

void cstream_input_memory_quit(cstream_input_memory_t* memory_stream)
{
   if(memory_stream && cstream_input_memory_valid(*memory_stream))
   {
      free(memory_stream->buffer);
      *memory_stream = (cstream_input_memory_t){0};
   }
}

bool cstream_input_memory_valid(cstream_input_memory_t memory_stream)
{
   return(memory_stream.length > 0 && memory_stream.buffer != 0);
}

uint64_t cstream_input_memory_read(cstream_input_memory_t* memory_stream, uint64_t desired_byte_count, void* destination)
{
   uint64_t result = 0;

   if(memory_stream && cstream_input_memory_valid(*memory_stream) && destination)
   {
      if((memory_stream->position + desired_byte_count) > memory_stream->length)
      {
         desired_byte_count = memory_stream->length - memory_stream->position;
      }

      if(desired_byte_count > 0)
      {
         uint8_t* destination_bytes = (uint8_t*)destination;
         uint8_t* buffer_bytes = ((uint8_t*)memory_stream->buffer) + memory_stream->position;

         for(uint64_t iter = 0; iter < desired_byte_count; ++iter)
         {
            *destination_bytes++ = *buffer_bytes++;

            result++;
            memory_stream->position++;
         }
      }
   }

   return(result);
}

