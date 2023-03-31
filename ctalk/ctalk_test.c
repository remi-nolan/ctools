/*
 * cstream - cstream_test.h
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

#define CTALK_IMPLEMENTATION
#include "ctalk.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRINT_ERROR() printf("\nerror!!!! %d %s (%s:%d)\n", ctalk_error(), __func__, __FILE__, __LINE__)
bool write_memory(char* buff, uint32_t buff_length) {
   cstream_t stream = {0};

   printf("write_memory:\n   ");

   if (cstream_write_memory(&stream, (void*)buff, buff_length, 0) != CTalkError_None) {
      PRINT_ERROR();
      return(false);
   }

   for (int iter = 0; iter < buff_length - 1; ++iter ) {
      char byte = 0;
      if (iter % 2 == 0) {
         byte = 'e';
      } else {
         byte = 'o';
      }

      if (cstream_write_8bits(&stream, byte) != CTalkError_None) {
         PRINT_ERROR();
         return(false);
      }
   }
   if (cstream_write_8bits(&stream, 0) != CTalkError_None) {
      PRINT_ERROR();
      return(false);
   }
   cstream_quit(&stream);

   printf("%s\n", buff);
   return(true);
}

bool read_memory(char* buff, int buff_length) {
   cstream_t stream = {0};

   printf("read_memory:\n   ");
   if (cstream_read_memory(&stream, buff, buff_length, 0) != CTalkError_None) {
      PRINT_ERROR();
      return(false);
   }

   int8_t byte = 0;
   int error = 0;

   do {
      error = cstream_read_8bits(&stream, &byte);

      if (error == CTalkError_None) {
         printf("%c", byte);
      }
   } while (error == CTalkError_None);

   if (error != CTalkError_EndOfStream) {
      PRINT_ERROR();
      return(false);
   }

   printf("\n");
   cstream_quit(&stream);
   return(true);
}

bool write_file(char* buff, uint32_t buff_length) {
   cstream_t stream = {0};

   printf("write_file:\n   ");
   if (cstream_write_file(&stream, "ct_test.txt", 0, CFile_Write|CFile_Create|CFile_Overwrite, 0) == CTalkError_None) {
      for (int8_t* c = (int8_t*)buff; *c != 0; ++c) {
         if (cstream_write_8bits(&stream, *c) != CTalkError_None) {
            PRINT_ERROR();
            return(false);
         }
      }

      printf("%s\n", buff);
      cstream_quit(&stream);
   } else {
      PRINT_ERROR();
      return(false);
   }

   return(true);
}

bool read_file() {
   cstream_t stream = {0};

   printf("read_file:\n   ");
   if (cstream_read_file(&stream, "ct_test.txt", CFile_Read, 0) != CTalkError_None) {
      PRINT_ERROR();
      return(false);
   }

   int8_t byte = 0;
   int error = CTalkError_None;
   do {
      error = cstream_read_8bits(&stream, &byte);
      if (error == CTalkError_None)
         printf("%c", byte);
   } while (error == CTalkError_None);

   if (error != CTalkError_EndOfStream) {
      PRINT_ERROR();
      return(false);
   }

   printf("\n");
   cstream_quit(&stream);

   return(true);
}

int main(int arg_count, char** args) {
   int buff_length = 51;
   char* buff = (char*)malloc(buff_length); if (!buff) return(-100);

   if (!write_memory(buff, buff_length))
      return(-1);
   if (!read_memory(buff, buff_length))
      return(-2);
   if (!write_file(buff, buff_length))
      return(-3);
   if (!read_file())
      return(-4);

   free(buff);
   return(0);
}

