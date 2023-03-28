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

#include "cstream.h"

static int cstream_last_error;
int cstream_error() {
   int result = cstream_last_error;
   cstream_last_error = CStreamError_None;
   return(result);
}

#define CStream_ErrorOut(error_code) return(cstream_last_error = error_code)

#include <stdio.h>
int cstream_read_memory(cstream_t* stream, void* memory, uint32_t num_bytes, int flags) {
   if (!stream || !memory) CStream_ErrorOut(CStreamError_NullPointer);
   if (cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags & ~CStream_IsWrite;
   stream->position = 0;
   stream->length = num_bytes;
   stream->handle = memory;

   return(CStreamError_None);
}

int cstream_read_file(cstream_t* stream, char* filename, int flags) {
   if (!stream || !filename) CStream_ErrorOut(CStreamError_NullPointer);
   if (cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

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
      CStream_ErrorOut(CStreamError_FileNotFound);
   }
}

uint32_t cstream_read(cstream_t* stream, uint32_t count, int8_t* buffer) {
   uint32_t bytes_read = 0;

   if (!stream || !buffer) {
      cstream_last_error = CStreamError_NullPointer;
   } else if (!cstream_valid(stream) || !cstream_readable(stream) || count == 0) {
      cstream_last_error = CStreamError_InvalidArg;
   } else if (stream->position >= stream->length) {
      cstream_last_error = CStreamError_EndOfStream;
   } else {
      switch (stream->type) {
         default: {
            cstream_last_error = CStreamError_InvalidArg;
         } break;

         case CStreamType_Memory: {
            if (stream->position + count >= stream->length) {
               count = stream->length - stream->position;
            }

            for (bytes_read = 0; bytes_read < count; ++bytes_read)
               *buffer++ = ((int8_t*)stream->handle)[stream->position++];
         } break;

         case CStreamType_File: {
            uint32_t remaining_length = stream->length - stream->position;
            if (count > remaining_length)
               count = remaining_length;

            size_t bytes_read = fread(buffer, 1, count, (FILE*)stream->handle);

            if (bytes_read < count)
               cstream_last_error = CStreamError_EndOfStream;

            stream->position += bytes_read;
         } break;
      }
   }

   return(bytes_read);
}

int cstream_read_8bits(cstream_t* stream, int8_t* out) {
   if (cstream_read(stream, 1, (int8_t*)out) != 1 || cstream_last_error == CStreamError_EndOfStream)
      CStream_ErrorOut(cstream_last_error);

   return(CStreamError_None);
}

int cstream_read_16bits(cstream_t* stream, int16_t* out) {
   if (cstream_read(stream, 2, (int8_t*)out) != 2)
      CStream_ErrorOut(cstream_last_error);

   if (stream->flags & CStream_SwapBytes) {
      int16_t tmp = *out;
      *out = ((tmp & 0x00FF) << 8)|((tmp & 0xFF00) >> 8);
   }

   return(CStreamError_None);
}

int cstream_read_32bits(cstream_t* stream, int32_t* out) {
   if (cstream_read(stream, 4, (int8_t*)out) != 4)
      CStream_ErrorOut(cstream_last_error);

   if (stream->flags & CStream_SwapBytes) {
      int32_t tmp = *out;
      *out = ((tmp & 0x000000FF) << 24)|((tmp & 0x0000FF00) << 8)|((tmp & 0x00FF0000) >> 8)|((tmp & 0xFF000000) >> 24);
   }

   return(CStreamError_None);
}

int cstream_read_64bits(cstream_t* stream, int64_t* out) {
   if (cstream_read(stream, 8, (int8_t*)out) != 8)
      return(cstream_last_error);

   if (stream->flags & CStream_SwapBytes) {
      int64_t tmp = *out;
      *out = ((tmp & 0x00000000000000FF) << 56)|((tmp & 0x000000000000FF00) << 40)|((tmp & 0x0000000000FF0000) << 24)|((tmp & 0x00000000FF000000) << 8)|
             ((tmp & 0xFF00000000000000) >> 56)|((tmp & 0x00FF000000000000) >> 40)|((tmp & 0x0000FF0000000000) >> 24)|((tmp & 0x000000FF00000000) >> 8);
   }

   return(CStreamError_None);
}


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t max_bytes, int flags) {
   if (!stream || !memory) CStream_ErrorOut(CStreamError_NullPointer);
   if (cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags|CStream_IsWrite;
   stream->bytes_written = 0;
   stream->bytes_max = max_bytes;
   stream->handle = memory;

   return(CStreamError_None);
}

int cstream_write_file(cstream_t* stream, char* filename, uint32_t max_bytes, int flags) {
   if (!stream || !filename) CStream_ErrorOut(CStreamError_NullPointer);
   if (cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

   char file_args[] = "wb";
   if ((flags & CStream_TextFile) == CStream_TextFile) {
      file_args[2] = 0;
   }

   if (max_bytes == 0) {
      max_bytes = 0xFFFFFFFF;
   }

   stream->type = CStreamType_File;
   stream->flags = flags|CStream_IsWrite;
   stream->bytes_written = 0;
   stream->bytes_max = max_bytes;
   stream->handle = fopen(filename, file_args);

   if (!stream->handle) {
      *stream = (cstream_t){0};
      CStream_ErrorOut(CStreamError_FileNotFound);
   }

   return(CStreamError_None);
}

int cstream_write_8bits(cstream_t* stream, int8_t val) {
   if (!stream) CStream_ErrorOut(CStreamError_NullPointer);
   if (!cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

   if (stream->bytes_written <= stream->bytes_max) {
      switch (stream->type) {
         case CStreamType_Memory: {
            ((char*)stream->handle)[stream->position] = val;
         } break;
         case CStreamType_File: {
            char buf[2] = {val, 0};
            if (fputs(buf, stream->handle) == EOF) {
               CStream_ErrorOut(CStreamError_Unspecified);
            }
         } break;
      }

      ++stream->bytes_written;
   } else {
      CStream_ErrorOut(CStreamError_EndOfStream);
   }

   return(CStreamError_None);
}

int cstream_write_16bits(cstream_t* stream, int16_t val) {
   int8_t byte;

   /*
    * we're doing the same thing as cstream_read_16bits, but writing the bytes out instead of reading them in
    */
   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0xFF) >> 8);
   } else {
      byte = (int8_t)(val & 0xFF);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)(val & 0x00FF);
   } else {
      byte = (int8_t)((val & 0xFF00) >> 8);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   return(CStreamError_None);
}

int cstream_write_32bits(cstream_t* stream, int32_t val) {
   int8_t byte;

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)(val & 0x000000FF);
   } else {
      byte = (int8_t)((val & 0xFF000000) >> 24);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x0000FF00) >> 8);
   } else {
      byte = (int8_t)((val & 0x00FF0000) >> 16);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x00FF0000) >> 16);
   } else {
      byte = (int8_t)((val & 0x0000FF00) >> 8);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0xFF000000) >> 24);
   } else {
      byte = (int8_t)(val & 0x000000FF);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   return(CStreamError_None);
}

int cstream_write_64bits(cstream_t* stream, int64_t val) {
   int8_t byte;

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)(val & 0x00000000000000FF);
   } else {
      byte = (int8_t)((val & 0xFF00000000000000) >> 56);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x000000000000FF00) >> 8);
   } else {
      byte = (int8_t)((val & 0x00FF000000000000) >> 48);   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x0000000000FF0000) >> 16);
   } else {
      byte = (int8_t)((val & 0x0000FF0000000000) >> 40);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x00000000FF000000) >> 24);
   } else {
      byte = (int8_t)((val & 0x000000FF00000000) >> 32);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x000000FF00000000) >> 32);
   } else {
      byte = (int8_t)((val & 0x00000000FF000000) >> 24);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x0000FF0000000000) >> 40);
   } else {
      byte = (int8_t)((val & 0x0000000000FF0000) >> 16);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0x00FF000000000000) >> 48);
   } else {
      byte = (int8_t)((val & 0x000000000000FF00) >> 8);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   if ((stream->flags & CStream_SwapBytes) == CStream_SwapBytes) {
      byte = (int8_t)((val & 0xFF00000000000000) >> 56);
   } else {
      byte = (int8_t)(val & 0x00000000000000FF);
   }
   if (cstream_write_8bits(stream, byte) != CStreamError_None) return(cstream_last_error);

   return(CStreamError_None);
}



int cstream_readable(cstream_t* stream) {
   if (stream) {
      return((stream->flags & CStream_IsWrite) != CStream_IsWrite);
   } else {
      return(0);
   }
}

int cstream_writable(cstream_t* stream) {
   if (stream) {
      return((stream->flags & CStream_IsWrite) == CStream_IsWrite);
   } else {
      return(0);
   }
}


int cstream_quit(cstream_t* stream) {
   if (cstream_valid(stream)) {
      switch (stream->type) {
         default:
            CStream_ErrorOut(CStreamError_InvalidArg);

         case CStreamType_Memory:
            *stream = (cstream_t){.handle=stream->handle};
            break;

         case CStreamType_File:
            fclose(stream->handle);
            *stream = (cstream_t){0};
            break;
      }

      cstream_last_error = CStreamError_None;
   }

   return(CStreamError_None);
}

int cstream_valid(cstream_t* stream) {
   /*
    * as of right now, this is the valid for *both* file and memory streams;
    * this might change later if more `stream_type`s are added,
    * at which time this function will need to be in the standard discriminated union form
    *  - remi, 22 march 2023
    */
   return(stream->length > 0 && stream->handle != 0);
}

int cstream_rewind(cstream_t* stream, uint32_t amount) {
   if (!stream) CStream_ErrorOut(CStreamError_NullPointer);
   if (!cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

   switch (stream->type) {
      default: CStream_ErrorOut(CStreamError_InvalidArg);

      case CStreamType_Memory:
         if (amount == 0 || amount > stream->position) {
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

int cstream_fast_forward(cstream_t* stream, uint32_t amount) {
   if (!stream) CStream_ErrorOut(CStreamError_NullPointer);
   if (!cstream_valid(stream)) CStream_ErrorOut(CStreamError_InvalidArg);

   switch (stream->type) {
      default: CStream_ErrorOut(CStreamError_InvalidArg);

      case CStreamType_Memory:
         if (amount == 0 || amount > stream->position) {
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

#undef CStream_ErrorOut

