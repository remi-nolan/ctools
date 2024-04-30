/*
 * file: cstream_output_file.c
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

bool cstream_output_file_init(cstream_output_file_t* file_stream, char* file_name)
{
   bool result = false;

   if(file_stream && file_name)
   {
      FILE* file_handle = fopen(file_name, "wb");

      if(file_handle)
      {
         file_stream->bytes_written = 0;
         file_stream->handle = (void*)file_handle;

         result = true;
      }
   }

   return(result);
}

void cstream_output_file_quit(cstream_output_file_t* file_stream)
{
   if(file_stream && cstream_output_file_valid(*file_stream))
   {
      fclose(file_stream->handle);
      *file_stream = (cstream_output_file_t){0};
   }
}

bool cstream_output_file_valid(cstream_output_file_t file_stream)
{
   return(file_stream.handle != 0);
}

uint64_t cstream_output_file_write(cstream_output_file_t* file_stream, uint64_t value_length_bytes, void* value)
{
   uint64_t bytes_written = 0;

   if(file_stream && cstream_output_file_valid(*file_stream) && value_length_bytes && value)
   {
      bytes_written = fwrite((const void*)value, 1, value_length_bytes, (FILE*)file_stream->handle);
      file_stream->bytes_written += bytes_written;
   }

   return(bytes_written);
}

