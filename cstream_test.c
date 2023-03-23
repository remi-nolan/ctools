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

#include "cstream.c"

#include <stdio.h>
#include <stdlib.h>

int main(int arg_count, char** args) {
   cstream_t stream;
   int buff_length = 50;
   char* buff = (char*)malloc(buff_length + 1); if (!buff) return(-100);

   printf("write_memory:\n   "); {
      if (cstream_write_memory(&stream, (void*)buff, buff_length + 1, 0) == CStreamError_None) {
         for (int iter = 0; iter <= buff_length; ++iter ) {
            char byte = 0;
            switch (iter & 0xF) {
               case 0x0: byte = '0'; break;
               case 0x1: byte = '1'; break;
               case 0x2: byte = '2'; break;
               case 0x3: byte = '3'; break;
               case 0x4: byte = '4'; break;
               case 0x5: byte = '5'; break;
               case 0x6: byte = '6'; break;
               case 0x7: byte = '7'; break;
               case 0x8: byte = '8'; break;
               case 0x9: byte = '9'; break;
               case 0xA: byte = 'A'; break;
               case 0xB: byte = 'B'; break;
               case 0xC: byte = 'C'; break;
               case 0xD: byte = 'D'; break;
               case 0xE: byte = 'E'; break;
               case 0xF: byte = 'F'; break;
            }
            if (cstream_write_8bits(&stream, byte) != CStreamError_None) {
               printf("\nerror ocurrrred!!!!!!!!!! %d\n", cstream_error());
               return(-5);
            }
         }
         if (cstream_write_8bits(&stream, 0) != CStreamError_None) {
            printf("\nerrrrrrror!!!!! %d\n", cstream_error());
            return(-6);
         }
         cstream_quit(&stream);

         printf("%s\n", buff);
      } else {
         printf("\nerror!!! %d\n", cstream_error());
      }
   }

   printf("read_memory:\n   "); {
      if (cstream_read_memory(&stream, buff, buff_length + 1, 0) == CStreamError_None) {
         char byte = 0;
         int error = 0;
         do { error = cstream_read_8bits(&stream, &byte);

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

   printf("write_file:\n   "); {
      if (cstream_write_file(&stream, "cstest.txt", 0, CStream_TextFile) == CStreamError_None) {
         for (char* c = buff; *c != 0; ++c) {
            if (cstream_write_8bits(&stream, *c) != CStreamError_None) {
               return(-7);
            }
         }

         printf("%s\n", buff);
         cstream_quit(&stream);
      } else {
         printf("\nerrrrorrrrr!!!!!! %d\n", cstream_error());
      }
   }

   printf("read_file:\n   "); {
      if (cstream_read_file(&stream, "cstest.txt", CStream_TextFile) == CStreamError_None) {
         char byte = 0;
         int error = CStreamError_None;
         do {
            error = cstream_read_8bits(&stream, &byte);
            switch (error) {
               case CStreamError_None:
                  printf("%c", byte);
                  break;
               case CStreamError_EndOfStream:
                  break;
               default:
                  printf("\nerror occurred!!! %d\n", cstream_error());
                  return(-3);
            }
         } while (error == CStreamError_None);

         printf("\n");
         cstream_quit(&stream);
      } else {
         printf("\nerror occurred!!!! %d\n", cstream_error());
         return(-4);
      }
   }

   free(buff);
   return(0);
}

