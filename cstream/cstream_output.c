/*
 * file: cstream_output.c
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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "stb_sprintf.h"

static cstream_output_file_t cstream_internal_stdout = {0};
cstream_output_t cstream_standard_out = { .source_type = CSTREAM_STDOUT };

static cstream_output_file_t cstream_stderr = {0};
cstream_output_t cstream_standard_error = { .source_type = CSTREAM_STDERR };

bool cstream_output_init_memory(cstream_output_t* output_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   output_stream->source_type = CSTREAM_MEMORY;
   result = cstream_output_memory_init(&output_stream->source.memory, memory_length, memory);

   return(result);
}

bool cstream_output_init_memory_allocate(cstream_output_t* output_stream, uint64_t memory_length)
{
   bool result = false;

   output_stream->source_type = CSTREAM_MEMORY;
   result = cstream_output_memory_init_allocate(&output_stream->source.memory, memory_length);

   return(result);
}

bool cstream_output_init_file(cstream_output_t* output_stream, char* file_name)
{
   bool result = false;

   output_stream->source_type = CSTREAM_FILE;
   result = cstream_output_file_init(&output_stream->source.file, file_name);

   return(result);
}

bool cstream_output_init_stdout(cstream_output_t* output_stream)
{
   bool result = false;

   if(output_stream && !cstream_output_valid(*output_stream))
   {
      output_stream->source_type = CSTREAM_STDOUT;
      output_stream->source = (cstream_output_source_t){0};
   }

   return(result);
}

bool cstream_output_init_stderr(cstream_output_t* output_stream)
{
   bool result = false;

   if(output_stream)
   {
      output_stream->source_type = CSTREAM_STDERR;
      output_stream->source = (cstream_output_source_t){0};
   }

   return(result);
}

void cstream_output_quit(cstream_output_t* output_stream)
{
   if(output_stream)
   {
      switch(output_stream->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_quit(&output_stream->source.memory);
         break;
      case CSTREAM_FILE:
         cstream_output_file_quit(&output_stream->source.file);
         break;
      }

      output_stream->source_type = CSTREAM_UNKNOWN;
      output_stream->source = (cstream_output_source_t){0};
   }
}

bool cstream_output_valid(cstream_output_t output_stream)
{
   bool result = false;

   switch(output_stream.source_type)
   {
   default:
   case CSTREAM_UNKNOWN:
      result = false;
      break;

   case CSTREAM_MEMORY:
      result = cstream_output_memory_valid(output_stream.source.memory);
      break;

   case CSTREAM_FILE:
      result = cstream_output_file_valid(output_stream.source.file);
      break;

   case CSTREAM_STDOUT:
      result = true;
      break;
   case CSTREAM_STDERR:
      result = true;
      break;
   }

   return(result);
}

uint64_t cstream_output_write(cstream_output_t* output_stream, uint64_t byte_count, void* source)
{
   uint64_t result = 0;

   if(output_stream && cstream_output_valid(*output_stream))
   {
      switch(output_stream->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_output_memory_write(&output_stream->source.memory, byte_count, source);
         break;
      case CSTREAM_FILE:
         result = cstream_output_file_write(&output_stream->source.file, byte_count, source);
         break;

      case CSTREAM_STDOUT:
         if(cstream_internal_stdout.handle == 0)
         {
            cstream_internal_stdout.handle = stdout;
         }
         cstream_output_file_write(&cstream_internal_stdout, byte_count, source);
         break;
      case CSTREAM_STDERR:
         if(cstream_stderr.handle == 0)
         {
            cstream_stderr.handle = stderr;
         }
         cstream_output_file_write(&cstream_stderr, byte_count, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_int8(cstream_output_t* output_stream, int8_t source)
{
   return(cstream_output_write(output_stream, 1, &source) == 1);
}

bool cstream_output_write_uint8(cstream_output_t* output_stream, uint8_t source)
{
   return(cstream_output_write(output_stream, 1, &source) == 1);
}

bool cstream_output_write_int16(cstream_output_t* output_stream, int16_t source)
{
   return(cstream_output_write(output_stream, 2, &source) == 2);
}

bool cstream_output_write_uint16(cstream_output_t* output_stream, uint16_t source)
{
   return(cstream_output_write(output_stream, 2, &source) == 2);
}

bool cstream_output_write_int32(cstream_output_t* output_stream, int32_t source)
{
   return(cstream_output_write(output_stream, 4, &source) == 4);
}

bool cstream_output_write_uint32(cstream_output_t* output_stream, uint32_t source)
{
   return(cstream_output_write(output_stream, 4, &source) == 4);
}

bool cstream_output_write_int64(cstream_output_t* output_stream, int64_t source)
{
   return(cstream_output_write(output_stream, 8, &source) == 8);
}

bool cstream_output_write_uint64(cstream_output_t* output_stream, uint64_t source)
{
   return(cstream_output_write(output_stream, 8, &source) == 8);
}

static char CSTREAM_SPRINTF_BUFFER[STB_SPRINTF_MIN];

static char* cstream_output_string_sprintf_callback(const char* buffer, void* user, int byte_count)
{
   cstream_output_write((cstream_output_t*)user, byte_count, (void*)buffer);
   return(CSTREAM_SPRINTF_BUFFER);
}

uint64_t cstream_output_write_string(cstream_output_t* output_stream, const char* format_string, ...)
{
   va_list variadic_arguments;
   uint64_t result = 0;

   if(output_stream && format_string)
   {
      va_start(variadic_arguments, format_string);
      result = stbsp_vsprintfcb(cstream_output_string_sprintf_callback, (void*)output_stream, CSTREAM_SPRINTF_BUFFER, format_string, variadic_arguments);
      va_end(variadic_arguments);
   }

   return(result);
}

