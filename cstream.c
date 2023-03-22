/*
 * cstream - cstream.c
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
 *
 * 2. The User is one of the following:
 * a. An individual person, laboring for themselves
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

#ifndef CSTREAM_IMPLEMENTED
#define CSTREAM_IMPLEMENTED

#ifndef CSTREAM_HEADER_ONLY
#include "cstream.h"
#endif//CSTREAM_HEADER_ONLY

static int cstream_last_error;
int cstream_error() {
   return(cstream_last_error);
}

#define ERROR(error_code) return(cstream_last_error = error_code)

#include <stdio.h>
int cstream_read_memory(cstream* stream, void* memory, unsigned int num_bytes, int flags) {
   if (!stream || !memory) ERROR(CStreamError_NullPointer);
   if (cstream_valid(stream)) ERROR(CStreamError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags & ~CStream_IsWrite;
   stream->position = 0;
   stream->length = num_bytes;
   stream->handle = memory;

   return(CStreamError_None);
}

int cstream_read_file(cstream* stream, char* filename, int flags) {
   if (!stream || !filename) ERROR(CStreamError_NullPointer);
   if (cstream_valid(stream)) ERROR(CStreamError_InvalidArg);

   char file_args[] = "rb";
   if ((flags & CStream_TextFile) == CStream_TextFile) {
      file_args[2] = 0;
   }

   stream->type = CStreamType_File;
   stream->handle = (void*)fopen(filename, file_args);

   if (stream->handle) {
      fseek(stream->handle, 0, SEEK_END);
      stream->length = ftell(stream->handle);
      fseek(stream->handle, 0, SEEK_SET);

      return(CStreamError_None);
   } else {
      ERROR(CStreamError_FileNotFound);
   }
}


int cstream_read_8bits(cstream* stream, char* out) {
   if (!stream || !out) ERROR(CStreamError_NullPointer);
   if (!cstream_valid(stream) || (stream->type & CStream_IsWrite)) ERROR(CStreamError_InvalidArg);
   *out = 0;

   switch (stream->type) {
      default: ERROR(CStreamError_InvalidArg);

      case CStreamType_Memory:
         if (stream->position < stream->length) {
            *out |= ((char*)stream->handle)[stream->position];
         } else ERROR(CStreamError_EndOfStream);
         break;

      case CStreamType_File: {
            size_t bytes_read = fread(out, 1, 1, (FILE*)stream->handle);
            if (bytes_read == 0 || *out == EOF) {
               *out |= 0;
               ERROR(CStreamError_EndOfStream);
            }
         } break;
   }

   ++stream->position;
   return(CStreamError_None);
}

int cstream_read_16bits(cstream* stream, short* out) {
   char byte;
   *out = 0;

   /*
    * here we are reading the hi-order byte from the buffer.
    * however if the user has requested that we swap the hi- and lo- order bytes in this stream,
    * we want to treat it as the lo-order byte
    */
   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= byte;
   } else {
      *out |= byte << 8;
   }

   /*
    * we're just doing the inverse of above, because we read the lo-order byte
    */
   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= byte << 8;
   } else {
      *out |= byte;
   }

   return(CStreamError_None);
}

int cstream_read_32bits(cstream* stream, long* out) {
   char byte;
   *out = 0;

   /*
    * see `cstream_read_16bits` for an explanation of how this function works
    * albeit with a 16-bit value instead of a 32-bit one
    */
   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= byte;
   } else {
      *out |= byte << 24;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= byte << 16;
   } else {
      *out |= byte << 8;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= byte << 8;
   } else {
      *out |= byte << 16;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= byte << 24;
   } else {
      *out |= byte << 0;
   }

   return(CStreamError_None);
}

int cstream_read_64bits(cstream* stream, long long* out) {
   char byte;
   *out = 0;

   //  56 48 40 32 24 16  8  0
   //0xFF FF FF FF FF FF FF FF

   /*
    * see `cstream_read_16bits` for an explanation of how this function works
    * albeit with a 16-bit value instead of a 64-bit one
    */
   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 0;
   } else {
      *out |= (long long)byte << 56;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 8;
   } else {
      *out |= (long long)byte << 48;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 16;
   } else {
      *out |= (long long)byte << 40; 
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 24;
   } else {
      *out |= (long long)byte << 32;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 32;
   } else {
      *out |= (long long)byte << 24;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 40;
   } else {
      *out |= (long long)byte << 16;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 48;
   } else {
      *out |= (long long)byte << 8;
   }

   if (cstream_read_8bits(stream, &byte) != CStreamError_None) return(cstream_last_error);
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      *out |= (long long)byte << 56;
   } else {
      *out |= (long long)byte << 0;
   }

   return(CStreamError_None);
}

int cstream_quit(cstream* stream) {
   if (cstream_valid(stream)) {
      switch (stream->type) {
         default:
            return(CStreamError_InvalidArg);

         case CStreamType_Memory:
            *stream = (cstream){.handle=stream->handle};
            break;

         case CStreamType_File:
            fclose(stream->handle);
            *stream = (cstream){0};
            break;
      }
   }

   return(CStreamError_None);
}

int cstream_readable(cstream* stream) {
   if (stream) {
      return((stream->flags & CStream_IsWrite) != CStream_IsWrite);
   } else {
      return(0);
   }
}

int cstream_writable(cstream* stream) {
   if (stream) {
      return((stream->flags & CStream_IsWrite) == CStream_IsWrite);
   } else {
      return(0);
   }
}

int cstream_valid(cstream* stream) {
   /*
    * as of right now, this is the valid for *both* file and memory streams;
    * this might change later if more `stream_type`s are added,
    * at which time this function will need to be in the standard discriminated union form
    *  - remi, 22 march 2023
    */
   return(stream->position <= stream->length && stream->length > 0 && stream->handle != 0);
}

int cstream_rewind(cstream* stream, int amount) {
   if (!stream) ERROR(CStreamError_NullPointer);
   if (!cstream_valid(stream)) ERROR(CStreamError_InvalidArg);

   switch (stream->type) {
      default: ERROR(CStreamError_InvalidArg);

      case CStreamType_Memory:
         if (amount <= 0 || amount > stream->position) {
            stream->position = 0;
         } else {
            stream->position -= amount;
         }
         break;

      case CStreamType_File:
         if (amount <= 0) {
            amount = stream->position * -1;
         } else {
            amount *= -1;
         }

         fseek(stream->handle, amount, SEEK_CUR);
         break;
   }
   
   return(CStreamError_None);
}

int cstream_fast_forward(cstream* stream, int amount) {
   if (!stream) ERROR(CStreamError_NullPointer);
   if (!cstream_valid(stream)) ERROR(CStreamError_InvalidArg);

   switch (stream->type) {
      default: ERROR(CStreamError_InvalidArg);

      case CStreamType_Memory:
         if (amount <= 0 || amount > stream->position) {
            stream->position = 0;
         } else {
            stream->position -= amount;
         }
         break;

      case CStreamType_File:
         if (amount <= 0) {
            amount = stream->length - stream->position;
         }

         fseek(stream->handle, amount, SEEK_CUR);
         break;
   }

   return(CStreamError_None);
}

#undef ERROR

#endif//CSTREAM_IMPLEMENTED
