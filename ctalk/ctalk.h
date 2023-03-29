/*
 * ctalk - ctalk.h
 * a minimalist c99, header-only cross-platform file i/o and data-stream library by Remi Nolan
 *
 * License:
 * ANTI-CAPITALIST SOFTWARE LICENSE(v 1.4)
 *
 * Copyright © 2023 Remi Nolan Eidahl
 *
 * This is anti-capitalist software, released for free use by individuals and organizations that do not operate by capitalist principles.
 *
 * Permission is hereby granted, free of charge, to any person or organization(the "User") obtaining a copy of this software and associated documentation files(the "Software"), to use, copy, modify, merge, distribute, and/or sell copies of the Software, subject to the following conditions:
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

#ifndef CTALK_INCLUDED
#define CTALK_INCLUDED

#ifndef CTOOLS_NO_STANDARD_INCLUDE
#define CTOOLS_STDINT
#include<stdint.h>
#endif

#ifndef CTOOLS_STDINT
#define CTOOLS_STDINT

#define int8_t    signed char
#define int16_t   signed short
#define int32_t   signed long
#define int64_t   signed long long
#define uint8_t   unsigned char
#define uint16_t  unsigned short
#define uint32_t  unsigned long
#define uint64_t  unsigned long long
#endif

enum ctalk_error {
   CTalkError_None          = 0,
   CTalkError_EndOfStream   = 1,
   CTalkError_Unspecified   = 5, //NOTE: really only used with the C-Standard Library, which doesn't give good errors

   CTalkError_NullPointer   = 2,
   CTalkError_InvalidArg    = 3,

   CTalkError_FileNotFound  = 4,
};
int ctalk_error();


enum cstream_type {
   CStreamType_Unknown   = 0,

   CStreamType_Memory    = 1,
   CStreamType_File      = 2,
};

enum cstream_file_flags {
   CStream_TextFile   = (1<<0),
   CStream_SwapBytes  = (1<<1),
   CStream_IsWrite    = (1<<2),
};

typedef struct cstream_s {
   int type;
   int flags;

   union {
      struct {
         uint32_t position;
         uint32_t length;
      };
      struct {
         uint32_t bytes_written;
         uint32_t bytes_max;
      };
   };

   void* handle;
} cstream_t;

int cstream_read_memory(cstream_t* stream, void* memory, uint32_t num_bytes, int flags);
int cstream_read_file(cstream_t* stream, char* filename, int flags);

uint32_t cstream_read(cstream_t* stream, uint32_t count, int8_t* buffer);

int cstream_read_8bits(cstream_t* stream, int8_t* out);
int cstream_read_16bits(cstream_t* stream, int16_t* out);
int cstream_read_32bits(cstream_t* stream, int32_t* out);
int cstream_read_64bits(cstream_t* stream, int64_t* out);


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t max_bytes, int flags);
int cstream_write_file(cstream_t* stream, char* filename, uint32_t max_bytes, int flags);

uint32_t cstream_write(cstream_t* stream, uint32_t count, int8_t* buffer);

int cstream_write_8bits(cstream_t* stream, int8_t val);
int cstream_write_16bits(cstream_t* stream, int16_t val);
int cstream_write_32bits(cstream_t* stream, int32_t val);
int cstream_write_64bits(cstream_t* stream, int64_t val);


int cstream_readable(cstream_t* stream);
int cstream_writable(cstream_t* stream);

int cstream_quit(cstream_t* stream);
int cstream_valid(cstream_t* stream);
int cstream_rewind(cstream_t* stream, uint32_t amount);
int cstream_fast_forward(cstream_t* stream, uint32_t amount);

#endif//CTALK_INCLUDED

#if !defined(CTOOLS_IMPLEMENTATION) || !defined(CTALK_IMPLEMENTATION)

//todo: remove this in favor of an implementation of a cfile api.
#include <stdio.h>

static int ctalk_last_error;
int ctalk_error() {
   int result = ctalk_last_error;
   ctalk_last_error = CTalkError_None;
   return(result);
}

#define CTalk_ErrorOut(error_code) return(ctalk_last_error = error_code)

int cstream_read_memory(cstream_t* stream, void* memory, uint32_t num_bytes, int flags) {
   if(!stream || !memory) CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(stream)) CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags & ~CStream_IsWrite;
   stream->position = 0;
   stream->length = num_bytes;
   stream->handle = memory;

   return(CTalkError_None);
}

int cstream_read_file(cstream_t* stream, char* filename, int flags) {
   if(!stream || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   char file_args[] = "rb";
   if((flags & CStream_TextFile) == CStream_TextFile) {
      file_args[2] = 0;
   }

   stream->type = CStreamType_File;
   stream->handle = (void*)fopen(filename, file_args);

   if(stream->handle) {
      fseek(stream->handle, 0, SEEK_END);
      stream->length = ftell(stream->handle);
      fseek(stream->handle, 0, SEEK_SET);

      return(CTalkError_None);
   } else {
      CTalk_ErrorOut(CTalkError_FileNotFound);
   }
}

uint32_t cstream_read(cstream_t* stream, uint32_t count, int8_t* buffer) {
   uint32_t bytes_read = 0;

   if(!stream || !buffer) {
      ctalk_last_error = CTalkError_NullPointer;
   } else if(!cstream_valid(stream) || !cstream_readable(stream) || count == 0) {
      ctalk_last_error = CTalkError_InvalidArg;
   } else if(stream->position >= stream->length) {
      ctalk_last_error = CTalkError_EndOfStream;
   } else {
      switch(stream->type) {
         default: {
            ctalk_last_error = CTalkError_InvalidArg;
         } break;

         case CStreamType_Memory: {
            if(stream->position + count >= stream->length)
               count = stream->length - stream->position;

            for(bytes_read = 0; bytes_read < count; ++bytes_read)
               *buffer++ = ((int8_t*)stream->handle)[stream->position++];
         } break;

         case CStreamType_File: {
            uint32_t remaining_length = stream->length - stream->position;
            if(count > remaining_length)
               count = remaining_length;

            bytes_read = (uint32_t)fread(buffer, 1, count, (FILE*)stream->handle);

            stream->position += bytes_read;
         } break;
      }
   }

   return(bytes_read);
}

int cstream_read_8bits(cstream_t* stream, int8_t* out) {
   //note: for the cstream_read_Xbits functions, we don't need to verify stream is non-null and valid.
   //this is because cstream_read (which is called before anything else happens) will check that for us.

   if(cstream_read(stream, 1, (int8_t*)out) != 1)
      CTalk_ErrorOut(ctalk_last_error);

   return(CTalkError_None);
}

int cstream_read_16bits(cstream_t* stream, int16_t* out) {
   if(cstream_read(stream, 2, (int8_t*)out) != 2)
      CTalk_ErrorOut(ctalk_last_error);

   if(stream->flags & CStream_SwapBytes) {
      int16_t tmp = *out;
      *out = ((tmp & 0x00FF) << 8)|((tmp & 0xFF00) >> 8);
   }

   return(CTalkError_None);
}

int cstream_read_32bits(cstream_t* stream, int32_t* out) {
   if(cstream_read(stream, 4, (int8_t*)out) != 4)
      CTalk_ErrorOut(ctalk_last_error);

   if(stream->flags & CStream_SwapBytes) {
      int32_t tmp = *out;
      *out = ((tmp & 0x000000FF) << 24)|((tmp & 0x0000FF00) << 8)|((tmp & 0x00FF0000) >> 8)|((tmp & 0xFF000000) >> 24);
   }

   return(CTalkError_None);
}

int cstream_read_64bits(cstream_t* stream, int64_t* out) {
   if(cstream_read(stream, 8, (int8_t*)out) != 8)
      CTalk_ErrorOut(ctalk_last_error);

   if(stream->flags & CStream_SwapBytes) {
      int64_t tmp = *out;
      *out = ((tmp & 0x00000000000000FF) << 56)|((tmp & 0x000000000000FF00) << 40)|((tmp & 0x0000000000FF0000) << 24)|((tmp & 0x00000000FF000000) << 8)|
             ((tmp & 0xFF00000000000000) >> 56)|((tmp & 0x00FF000000000000) >> 40)|((tmp & 0x0000FF0000000000) >> 24)|((tmp & 0x000000FF00000000) >> 8);
   }

   return(CTalkError_None);
}


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t max_bytes, int flags) {
   if(!stream || !memory)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags|CStream_IsWrite;
   stream->bytes_written = 0;
   stream->bytes_max = max_bytes;
   stream->handle = memory;

   return(CTalkError_None);
}

int cstream_write_file(cstream_t* stream, char* filename, uint32_t max_bytes, int flags) {
   if(!stream || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   char file_args[] = "wb";
   if((flags & CStream_TextFile) == CStream_TextFile)
      file_args[2] = 0;

   if(max_bytes == 0)
      max_bytes = 0xFFFFFFFF;

   stream->type = CStreamType_File;
   stream->flags = flags|CStream_IsWrite;
   stream->bytes_written = 0;
   stream->bytes_max = max_bytes;
   stream->handle = fopen(filename, file_args);

   if(!stream->handle) {
      *stream = (cstream_t){0};
      CTalk_ErrorOut(CTalkError_FileNotFound);
   }

   return(CTalkError_None);
}

uint32_t cstream_write(cstream_t* stream, uint32_t count, int8_t* buffer) {
   uint32_t bytes_written = 0;

   if(!stream || !buffer) {
      ctalk_last_error = CTalkError_NullPointer;
   } else if(!cstream_valid(stream) || !cstream_writable(stream) || count == 0) {
      ctalk_last_error = CTalkError_InvalidArg;
   } else if(stream->position >= stream->length) {
      ctalk_last_error = CTalkError_EndOfStream;
   } else {
       switch(stream->type) {
         default: {
            ctalk_last_error = CTalkError_InvalidArg;
         } break;

         case CStreamType_Memory: {
            if(stream->bytes_written + count >= stream->bytes_max)
               count = stream->bytes_max - stream->position;

            for(bytes_written = 0; bytes_written < count; ++bytes_written)
               ((int8_t*)stream->handle)[stream->position++] = *buffer++;
         } break;

         case CStreamType_File: {
            uint32_t remaining_length = stream->length - stream->position;
            if (count > remaining_length)
               count = remaining_length;

            bytes_written = (uint32_t)fwrite((const void*)buffer, (size_t)1, (size_t)count, (FILE*)stream->handle);

            stream->position += bytes_written;
         } break;
      }
   }

   return(bytes_written);
}

int cstream_write_8bits(cstream_t* stream, int8_t val) {
   if(cstream_write(stream, 1, &val) != 1)
      CTalk_ErrorOut(ctalk_last_error);

   return(CTalkError_None);
}

int cstream_write_16bits(cstream_t* stream, int16_t val) {
   if(!stream)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cstream_valid(stream) || !cstream_writable(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   if((stream->flags & CStream_SwapBytes) == CStream_SwapBytes)
      val = ((val & 0xFF00) >> 8)|((val & 0x00FF) << 8);

   if(cstream_write(stream, 2, (int8_t*)&val) != 2)
      CTalk_ErrorOut(ctalk_last_error);

   return(CTalkError_None);
}

int cstream_write_32bits(cstream_t* stream, int32_t val) {
   if(!stream)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cstream_valid(stream) || !cstream_writable(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   if((stream->flags & CStream_SwapBytes) == CStream_SwapBytes)
      val = ((val & 0xFF000000) >> 24)|((val & 0x00FF0000) >> 8)|
            ((val & 0x000000FF) << 24)|((val & 0x0000FF00) << 8);

   if(cstream_write(stream, 4, (int8_t*)&val) != 4)
      CTalk_ErrorOut(ctalk_last_error);

   return(CTalkError_None);
}

int cstream_write_64bits(cstream_t* stream, int64_t val) {
   if(!stream)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cstream_valid(stream) || !cstream_writable(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   if((stream->flags & CStream_SwapBytes) == CStream_SwapBytes)
      val = ((val & 0xFF00000000000000) >> 56)|((val & 0x00FF000000000000) >> 40)|((val & 0x0000FF0000000000) >> 24)|((val & 0x000000FF00000000) >> 8)|
            ((val & 0x00000000000000FF) << 56)|((val & 0x000000000000FF00) << 40)|((val & 0x0000000000FF0000) << 24)|((val & 0x00000000FF000000) << 8);

   if(cstream_write(stream, 4, (int8_t*)&val) != 4)
      CTalk_ErrorOut(ctalk_last_error);

   return(CTalkError_None);
}


int cstream_readable(cstream_t* stream) {
   if(stream) {
      return((stream->flags & CStream_IsWrite) != CStream_IsWrite);
   } else {
      return(0);
   }
}

int cstream_writable(cstream_t* stream) {
   if(stream) {
      return((stream->flags & CStream_IsWrite) == CStream_IsWrite);
   } else {
      return(0);
   }
}

int cstream_quit(cstream_t* stream) {
   if(cstream_valid(stream)) {
      switch(stream->type) {
         default:
            CTalk_ErrorOut(CTalkError_InvalidArg);

         case CStreamType_Memory:
            *stream = (cstream_t){.handle=stream->handle};
            break;

         case CStreamType_File:
            fclose(stream->handle);
            *stream = (cstream_t){0};
            break;
      }

      ctalk_last_error = CTalkError_None;
   }

   return(CTalkError_None);
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
   if(!stream)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cstream_valid(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   switch(stream->type) {
      default:
         CTalk_ErrorOut(CTalkError_InvalidArg);

      case CStreamType_Memory:
         if(amount == 0 || amount > stream->position)
            stream->position = 0;
         else
            stream->position -= amount;
         break;

      case CStreamType_File:
         if(amount <= 0)
            amount = stream->position * -1;
         else
            amount *= -1;

         fseek(stream->handle, amount, SEEK_CUR);
         break;
   }

   return(CTalkError_None);
}

int cstream_fast_forward(cstream_t* stream, uint32_t amount) {
   if(!stream)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cstream_valid(stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   switch(stream->type) {
      default:
         CTalk_ErrorOut(CTalkError_InvalidArg);

      case CStreamType_Memory:
         if(amount == 0 || amount > stream->position)
            stream->position = 0;
         else
            stream->position -= amount;
         break;

      case CStreamType_File:
         if(amount <= 0)
            amount = stream->length - stream->position;

         fseek(stream->handle, amount, SEEK_CUR);
         break;
   }

   return(CTalkError_None);
}

#endif//!defined(CTOOLS_IMPLEMENTED) || !defined(CTALK_IMPLEMENTATION)

