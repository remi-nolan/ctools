/*
 * cstream - cstream_test.h
 * a minimalist c99 data-stream library by Remi Nolan
 *
 * License:
 * ANTI-CAPITALIST SOFTWARE LICENSE (v 1.4)
 *
 * Copyright © 2023 Remi Nolan Eidahl
 *
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

#include "cstream.c"

static int buffer_length = 50;
static char* buffer = "a minimalist c99 data-stream library by Remi Nolan";

int main(int arg_count, char** args) {
   cstream stream;

   printf("read_memory:\n   "); {
      if (cstream_read_memory(&stream, (void*)buffer, buffer_length, 0) == CStreamError_None) {
         char byte = 0;
         int error = 0;
         do {
            error = cstream_read_8bits(&stream, &byte);

            if (error == CStreamError_None) {
               printf("%c", byte);
            } else if (error == CStreamError_EndOfStream) {
               printf("\n"); break; } else {
               printf("error occurred!!! %d\n", cstream_error());
               return(-1);
            }
         } while (error == CStreamError_None);

         cstream_quit(&stream);
      } else {
         printf("error occurred!!!!!!!! %d\n", cstream_error());
         return(-2);
      }
   }

   printf("read_file:\n   "); {
      if (cstream_read_file(&stream, "cstream.h", CStream_TextFile) == CStreamError_None) {
         cstream_fast_forward(&stream, 29);

         char byte = 0;
         for (int iter = 0; iter < 51; ++iter) {
            if (cstream_read_8bits(&stream, &byte) == CStreamError_None) {
               printf("%c", byte);
            } else {
               printf("\nerror occurred!!! %d\n", cstream_error());
               return(-3);
            }
         }

         cstream_quit(&stream);
      } else {
         printf("\nerror occurred!!!! %d\n", cstream_error());
         return(-4);
      }
   }

   return(0);
}

