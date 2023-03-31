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
 * 4. If the User is an organization, then the User is not law enforcement or military, or working for or under either.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CTALK_INCLUDED
#define CTALK_INCLUDED

#ifndef CTOOLS_NO_STANDARD_INCLUDE
#define CTOOLS_STDINT
#include<stdint.h>
#include<stdbool.h>
#endif

#ifndef CTOOLS_STDINC
#define CTOOLS_STDINC

#define int8_t    signed char
#define int16_t   signed short
#define int32_t   signed long
#define int64_t   signed long long
#define uint8_t   unsigned char
#define uint16_t  unsigned short
#define uint32_t  unsigned long
#define uint64_t  unsigned long long

#ifndef bool
#define bool _Bool
#endif//bool

#ifndef true
#define true (1)
#endif//true

#ifndef false
#define false (0)
#endif//false

#endif//CTOOLS_STDINC

enum ctalk_error_e {
   CTalkError_None               = 0,
   CTalkError_EndOfStream        = 1,
   CTalkError_Unspecified        = 5,

   CTalkError_NullPointer        = 2,
   CTalkError_InvalidArg         = 3,

   CTalkError_FileNotFound       = 4,
   CTalkError_FileExists         = 5,
   CTalkError_AccessDenied       = 6,
   CTalkError_WriteDir           = 7,
   CTalkError_TooManyFilesOpen   = 8,
   CTalkError_PathExceedsLimit   = 9,
   CTalkError_SymlinkLoop        = 10,
   CTalkError_FileTooBig         = 11,
};
int ctalk_error();


enum cfile_args_e {
   CFile_Read         = (1<<0),
   CFile_Write        = (1<<1),
   CFile_AccessBits   = CFile_Read|CFile_Write,

   CFile_Create       = (1<<2),
   CFile_Overwrite    = (1<<3),
   CFile_Append       = (1<<4),
   CFile_OpenBits     = CFile_Create|CFile_Overwrite|CFile_Append,
};

enum cfile_from_e {
   CFile_FromBeginning = 1,
   CFile_FromCurrent   = 0,
   CFile_FromEnd       = 2,
};

typedef struct cfile_s {
   uint8_t flags;
   uint32_t size;
   void* handle;
} cfile_t;

int cfile_init(cfile_t* file, char* filename, uint8_t flags);
int cfile_quit(cfile_t* file);

cfile_t* cfile_stdout();
cfile_t* cfile_stderr();
cfile_t* cfile_stdin();

bool cfile_valid(cfile_t file);
int cfile_move(cfile_t file, int32_t factor, int from);

bool cfile_readable(cfile_t file);
uint32_t cfile_read(cfile_t file, uint32_t num_bytes, int8_t* bytes);

bool cfile_writable(cfile_t file);
uint32_t cfile_write(cfile_t file, uint32_t num_bytes, int8_t* bytes);

bool cfile_exists(char* filename);


enum cstream_type_e {
   CStreamType_Unknown   = 0,

   CStreamType_Memory    = 1,
   CStreamType_File      = 2,
};

enum cstream_flags_e {
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

   union {
      void* memory;
      cfile_t file;
   };
} cstream_t;

int cstream_read_memory(cstream_t* stream, void* memory, uint32_t num_bytes, int flags);
int cstream_read_file(cstream_t* stream, char* filename, int file_flags, int stream_flags);

uint32_t cstream_read(cstream_t* stream, uint32_t count, int8_t* buffer);

int cstream_read_8bits(cstream_t* stream, int8_t* out);
int cstream_read_16bits(cstream_t* stream, int16_t* out);
int cstream_read_32bits(cstream_t* stream, int32_t* out);
int cstream_read_64bits(cstream_t* stream, int64_t* out);


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t max_bytes, int flags);
int cstream_write_file(cstream_t* stream, char* filename, uint32_t max_bytes, int file_flags, int stream_flags);

uint32_t cstream_write(cstream_t* stream, uint32_t count, int8_t* buffer);

int cstream_write_8bits(cstream_t* stream, int8_t val);
int cstream_write_16bits(cstream_t* stream, int16_t val);
int cstream_write_32bits(cstream_t* stream, int32_t val);
int cstream_write_64bits(cstream_t* stream, int64_t val);

int cstream_quit(cstream_t* stream);
bool cstream_readable(cstream_t stream);
bool cstream_writable(cstream_t stream);

int cstream_move(cstream_t* stream, int32_t factor, int from);

int cstream_valid(cstream_t stream);

#endif//CTALK_INCLUDED

#if !defined(CTOOLS_IMPLEMENTATION) || !defined(CTALK_IMPLEMENTATION)

static int ctalk_last_error;
int ctalk_error() {
   int result = ctalk_last_error;
   ctalk_last_error = CTalkError_None;
   return(result);
}

#define CTalk_Error(error_code) ctalk_last_error = error_code
#define CTalk_ErrorOut(error_code) return(CTalk_Error(error_code))

#if defined(CTOOLS_FORCE_STDIO) || !defined(CTOOLS_CFILE_IMPLEMENTATION)
#include <stdio.h>
#include <errno.h>

int cfile_init(cfile_t* file, char* filename, uint8_t flags) {
   if(!file || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);

   file->flags = flags;
   file->size = 0;

   bool get_size = false;
   char open_string[4] = {
      [1] = 'b',
   };

   if(flags & CFile_Write) {
      //note: Read & Write flags are the same as just write, with an append '+', which is handled down below
      switch(flags & CFile_OpenBits) {
         default:
            CTalk_ErrorOut(CTalkError_InvalidArg);

         case CFile_Create:
            if(cfile_exists(filename))
               CTalk_ErrorOut(CTalkError_FileExists);
            open_string[0] = 'w';
            break;

         case CFile_Overwrite:
            if(!cfile_exists(filename))
               open_string[0] = 'w';
            else
               CTalk_ErrorOut(CTalkError_FileNotFound);
            break;

         case CFile_Append:
            if(cfile_exists(filename))
               open_string[0] = 'a';
            else
               CTalk_ErrorOut(CTalkError_FileNotFound);
            break;

         case CFile_Create|CFile_Overwrite:
            open_string[0] = 'w';
            break;

         case CFile_Create|CFile_Append:
            open_string[0] = 'a';
            break;

         //note: `CFile_Append|CFile_Overwrite` doesn't really make sense, so we don't implement it.
         //We leave it undefined, which will cause it to error as `CTalkError_InvalidArg` (in default).
         //Potentially in the future, this could be changed so the Overwrite overrides Append, (or vice versa).
         //However this adds a case where the library has to make a decision for the user, which I would rather avoid.
         // -remi(31 March 2023)
      }

      if(flags & CFile_Read) {
         open_string[3] = '+';
      }
   } else if(flags & CFile_Read) {
      get_size = true;
      open_string[0] = 'r';
   }

   file->handle = (void*)fopen(filename, open_string);

   if(file->handle) {
      if(get_size) {
         fseek((FILE*)file->handle, 0, SEEK_END);
         file->size = ftell((FILE*)file->handle);
         fseek((FILE*)file->handle, 0, SEEK_SET);
      } else {
         file->size = -1;
      }
   } else {
      *file = (cfile_t){0};

      switch(errno) {
         default:
            CTalk_ErrorOut(CTalkError_Unspecified);

         case EACCES:
            CTalk_ErrorOut(CTalkError_AccessDenied);

         case EISDIR:
            CTalk_ErrorOut(CTalkError_WriteDir);

         case ENFILE:
         case EMFILE:
            CTalk_ErrorOut(CTalkError_TooManyFilesOpen);

         case ENAMETOOLONG:
            CTalk_ErrorOut(CTalkError_PathExceedsLimit);

         case ELOOP:
            CTalk_ErrorOut(CTalkError_SymlinkLoop);

         case EOVERFLOW:
            CTalk_ErrorOut(CTalkError_FileTooBig);
      }
   }

   return(CTalkError_None);
}

int cfile_quit(cfile_t* file) {
   if(!file)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cfile_valid(*file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   fclose(file->handle);
   *file = (cfile_t){0};

   return(CTalkError_None);
}

static cfile_t cfile_stdout_handle;
cfile_t* cfile_stdout() {
   if(!cfile_stdout_handle.handle) {
      cfile_stdout_handle = (cfile_t){
         .handle = stdout,
         .flags = CFile_Write,
      };
   }

   return(&cfile_stdout_handle);
}

static cfile_t cfile_stderr_handle;
cfile_t* cfile_stderr() {
   if(!cfile_stderr_handle.handle) {
      cfile_stderr_handle = (cfile_t){
         .handle = stderr,
         .flags = CFile_Write,
      };
   }

   return(&cfile_stderr_handle);
}

static cfile_t cfile_stdin_handle;
cfile_t* cfile_stdin() {
   if(!cfile_stdin_handle.handle) {
      cfile_stdin_handle = (cfile_t){
         .handle = stdin,
         .flags = CFile_Read,
      };
   }

   return(&cfile_stdin_handle);
}

bool cfile_valid(cfile_t file) {
   return(file.handle);
}

int cfile_move(cfile_t file, int32_t factor, int from) {
   if(!cfile_valid(file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   int whence = 0;
   switch(from) {
      default:
         CTalk_ErrorOut(CTalkError_InvalidArg);
      case CFile_FromBeginning:
         whence = SEEK_SET;
         break;
      case CFile_FromCurrent:
         whence = SEEK_CUR;
         break;
      case CFile_FromEnd:
         whence = SEEK_END;
         break;
   }

   fseek(file.handle, factor, whence);

   return(CTalkError_None);
}

bool cfile_readable(cfile_t file) {
   bool result = false;

   if(cfile_valid(file)) {
      result = (file.flags & CFile_Read);
   } else {
      CTalk_Error(CTalkError_InvalidArg);
   }

   return(result);
}

uint32_t cfile_read(cfile_t file, uint32_t num_bytes, int8_t* bytes) {
   uint32_t bytes_read = 0;

   if(cfile_valid(file)) {
      bytes_read = (uint32_t)fread(bytes, 1, num_bytes, (FILE*)file.handle);
   } else {
      CTalk_Error(CTalkError_InvalidArg);
   }

   return(bytes_read);
}

bool cfile_writable(cfile_t file) {
   bool result = false;

   if(cfile_valid(file)) {
      result = (file.flags & CFile_Write);
   } else {
      CTalk_Error(CTalkError_InvalidArg);
   }

   return(result);
}

uint32_t cfile_write(cfile_t file, uint32_t num_bytes, int8_t* bytes) {
   uint32_t bytes_written = 0;

   if(cfile_valid(file)) {
      bytes_written = (uint32_t)fwrite(bytes, 1, num_bytes, (FILE*)file.handle);
   } else {
      CTalk_Error(CTalkError_InvalidArg);
   }

   return(bytes_written);
}

bool cfile_exists(char* filename) {
   bool result = false;

   FILE* handle = fopen(filename, "r");
   if (handle) {
      result = true;
      fclose(handle);
   }

   return(result);
}

#endif//defined(CTOOLS_FORCE_STDIO) || !defined(CTOOLS_CFILE_IMPLEMENTATION)



int cstream_read_memory(cstream_t* stream, void* memory, uint32_t num_bytes, int flags) {
   if(!stream || !memory) CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(*stream)) CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags & ~CStream_IsWrite;
   stream->position = 0;
   stream->length = num_bytes;
   stream->memory = memory;

   return(CTalkError_None);
}

int cstream_read_file(cstream_t* stream, char* filename, int file_flags, int stream_flags) {
   if(!stream || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_File;
   stream->flags = stream_flags;
   cfile_init(&stream->file, filename, file_flags);

   if(cfile_valid(stream->file)) {
      stream->position = 0;
      stream->length = stream->file.size;
      return(CTalkError_None);
   } else {
      *stream = (cstream_t){0};
      CTalk_ErrorOut(CTalkError_FileNotFound);
   }
}

uint32_t cstream_read(cstream_t* stream, uint32_t count, int8_t* buffer) {
   uint32_t bytes_read = 0;

   if(!stream || !buffer) {
      CTalk_Error(CTalkError_NullPointer);
   } else if(!cstream_valid(*stream) || !cstream_readable(*stream) || count == 0) {
      CTalk_Error(CTalkError_InvalidArg);
   } else if(stream->position >= stream->length) {
      CTalk_Error(CTalkError_EndOfStream);
   } else {
      switch(stream->type) {
         default: {
            CTalk_Error(CTalkError_InvalidArg);
         } break;

         case CStreamType_Memory: {
            if(stream->position + count >= stream->length)
               count = stream->length - stream->position;

            for(bytes_read = 0; bytes_read < count; ++bytes_read)
               *buffer++ = ((int8_t*)stream->memory)[stream->position++];
         } break;

         case CStreamType_File: {
            uint32_t remaining_length = stream->length - stream->position;
            if(count > remaining_length)
               count = remaining_length;

            bytes_read = cfile_read(stream->file, count, buffer);
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
   if(cstream_valid(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags|CStream_IsWrite;
   stream->bytes_written = 0;
   stream->bytes_max = max_bytes;
   stream->memory = memory;

   return(CTalkError_None);
}

int cstream_write_file(cstream_t* stream, char* filename, uint32_t max_bytes, int file_flags, int flags) {
   if(!stream || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_File;
   stream->flags = flags|CStream_IsWrite;
   stream->bytes_written = 0;
   stream->bytes_max = max_bytes;

   cfile_init(&stream->file, filename, file_flags);

   if(cfile_valid(stream->file)) {
      stream->length = stream->file.size;
   } else {
      *stream = (cstream_t){0};
      CTalk_ErrorOut(ctalk_last_error);
   }

   return(CTalkError_None);
}

uint32_t cstream_write(cstream_t* stream, uint32_t count, int8_t* buffer) {
   uint32_t bytes_written = 0;

   if(!stream || !buffer) {
      CTalk_Error(CTalkError_NullPointer);
   } else if(!cstream_valid(*stream) || !cstream_writable(*stream) || count == 0) {
      CTalk_Error(CTalkError_InvalidArg);
   } else if(stream->position >= stream->length) {
      CTalk_Error(CTalkError_EndOfStream);
   } else {
       switch(stream->type) {
         default: {
            CTalk_Error(CTalkError_InvalidArg);
         } break;

         case CStreamType_Memory: {
            if(stream->bytes_written + count >= stream->bytes_max)
               count = stream->bytes_max - stream->position;

            for(bytes_written = 0; bytes_written < count; ++bytes_written)
               ((int8_t*)stream->memory)[stream->position++] = *buffer++;
         } break;

         case CStreamType_File: {
            uint32_t remaining_length = stream->length - stream->position;
            if (count > remaining_length)
               count = remaining_length;

            bytes_written = cfile_write(stream->file, count, buffer);

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
   if(!cstream_valid(*stream) || !cstream_writable(*stream))
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
   if(!cstream_valid(*stream) || !cstream_writable(*stream))
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
   if(!cstream_valid(*stream) || !cstream_writable(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   if((stream->flags & CStream_SwapBytes) == CStream_SwapBytes)
      val = ((val & 0xFF00000000000000) >> 56)|((val & 0x00FF000000000000) >> 40)|((val & 0x0000FF0000000000) >> 24)|((val & 0x000000FF00000000) >> 8)|
            ((val & 0x00000000000000FF) << 56)|((val & 0x000000000000FF00) << 40)|((val & 0x0000000000FF0000) << 24)|((val & 0x00000000FF000000) << 8);

   if(cstream_write(stream, 4, (int8_t*)&val) != 4)
      CTalk_ErrorOut(ctalk_last_error);

   return(CTalkError_None);
}


bool cstream_readable(cstream_t stream) {
   return(cstream_valid(stream) && (stream.flags & CStream_IsWrite) != CStream_IsWrite);
}

bool cstream_writable(cstream_t stream) {
   return(cstream_valid(stream) && (stream.flags & CStream_IsWrite) == CStream_IsWrite);
}

int cstream_quit(cstream_t* stream) {
   if(cstream_valid(*stream)) {
      switch(stream->type) {
         default:
            CTalk_ErrorOut(CTalkError_InvalidArg);

         case CStreamType_Memory:
            *stream = (cstream_t){.memory=stream->memory};
            break;

         case CStreamType_File:
            cfile_quit(&stream->file);
            *stream = (cstream_t){0};
            break;
      }

      ctalk_last_error = CTalkError_None;
   }

   return(CTalkError_None);
}

int cstream_valid(cstream_t stream) {
   /*
    * as of right now, this is the valid for *both* file and memory streams;
    * this might change later if more `stream_type`s are added,
    * at which time this function will need to be in the standard discriminated union form
    *  -remi, 22 march 2023
    */
   if(stream.type == CStreamType_File)
      return(cfile_valid(stream.file));

   return(stream.memory && stream.length > 0);
}

int cstream_move(cstream_t* stream, int32_t factor, int from) {
   if(!stream)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cstream_valid(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   uint32_t new_position = 0; 
   switch(from) {
      default:
         CTalk_ErrorOut(CTalkError_InvalidArg);

      case CFile_FromBeginning:
         if(factor < 0)
            factor = 0;
         new_position = factor;
         break;
      case CFile_FromCurrent:
         new_position = stream->position + factor;
         break;
      case CFile_FromEnd:
         if(factor < 0)
            factor = 0;
         new_position = stream->length - factor;
         break;
   }

   switch(stream->type) {
      default:
         CTalk_ErrorOut(CTalkError_InvalidArg);

      case CStreamType_Memory:
         stream->position = new_position;
         break;

      case CStreamType_File:
         if (cfile_move(stream->file, factor, from) == CTalkError_None)
            stream->position = new_position;
         else
            CTalk_ErrorOut(ctalk_last_error);
         break;
   }

   return(CTalkError_None);
}

#endif//!defined(CTOOLS_IMPLEMENTED) || !defined(CTALK_IMPLEMENTATION)

