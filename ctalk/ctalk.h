/*
 * ctalk - ctalk.h
 * a minimalist c99, header-only, cross-platform file i/o and data-stream library by Remi Nolan
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

#ifndef CTOOLS_STDINT
#define CTOOLS_STDINT

#ifndef CTOOLS_NO_STANDARD_INCLUDE
#include <stdint.h>
#include <stdbool.h>
#else
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
#endif//CTOOLS_NO_STANDARD_INCLUDE
#endif//CTOOLS_STDINT


#ifndef CTALK_INCLUDED
#define CTALK_INCLUDED


enum ctalk_error_e {
   CTalkError_None               = 0,
   CTalkError_EndOfStream        = 1,
   CTalkError_Unspecified        = 99,
   CTalkError_Unsupported        = 98,

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
   CTalkError_SignalInterrupt    = 12,
   CTalkError_NotEnoughSpace     = 13,
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
   uint64_t size;
   void* handle;
} cfile_t;

int cfile_init(cfile_t* file, char* filename, uint8_t flags);
int cfile_quit(cfile_t* file);

bool cfile_valid(cfile_t file);
int cfile_move(cfile_t file, int64_t factor, int from);

bool cfile_readable(cfile_t file);
uint32_t cfile_read(cfile_t file, uint32_t num_bytes, int8_t* bytes);

bool cfile_writable(cfile_t file);
uint32_t cfile_write(cfile_t file, uint32_t num_bytes, int8_t* bytes);

bool cfile_exists(char* filename);

inline cfile_t ctalk_stdout();
inline cfile_t ctalk_stderr();
inline cfile_t ctalk_stdin();

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

   struct {
      uint32_t position;
      uint32_t length;
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


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t length, int flags);
int cstream_write_file(cstream_t* stream, char* filename, uint32_t length, int file_flags, int stream_flags);

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

#if !defined(CTOOLS_FORCE_STDIO) && defined(_WIN32)
#define CFILE_IMPLEMENTATION
#include <Windows.h>

int cfile_init(cfile_t* file, char* filename, uint8_t flags) {
   if(!file || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cfile_valid(*file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   bool seek_end = false;
   DWORD desired_access = 0;
   DWORD creation_disposition = 0;

   if(flags & CFile_Read)
      desired_access |= GENERIC_READ;
   if(flags & CFile_Write)
      desired_access |= GENERIC_WRITE;
   if(desired_access == 0)
      CTalk_ErrorOut(CTalkError_InvalidArg);

   if(flags & CFile_Write) {
      switch(flags & CFile_OpenBits) {
         default:
            CTalk_ErrorOut(CTalkError_InvalidArg);

         case CFile_Create:
            creation_disposition = CREATE_NEW;
            break;
         case CFile_Overwrite:
            creation_disposition = TRUNCATE_EXISTING;
            break;
         case CFile_Append:
            creation_disposition = OPEN_EXISTING;
            seek_end = true;
            break;

         case CFile_Create|CFile_Overwrite:
            creation_disposition = CREATE_ALWAYS;
            break;
         case CFile_Create|CFile_Append:
            creation_disposition = OPEN_ALWAYS;
            seek_end = true;
            break;
      }
   } else {
      creation_disposition = OPEN_EXISTING;
   }

   /*
    * note: right now we hardcode dwShareMode to zero (lock all access if you're not the us).
    * potentially in the future we might want to expose this to the user.
    * that way we are making as few decisions in library-code as possible.
    *  -remi 01 April 2023
    */
   HANDLE handle = CreateFile(filename, desired_access, 0, 0, creation_disposition, FILE_ATTRIBUTE_NORMAL, 0);

   if(handle != INVALID_HANDLE_VALUE) {
      uint64_t file_size = 0;

      {
         LARGE_INTEGER quad_file_size;
         if(GetFileSizeEx(handle, &quad_file_size)) {
            file_size = quad_file_size.QuadPart;
         } else {
            //CTalk_ErrorOut(cfile_intern_parse_win32_last_error());
         }
      }

      if(seek_end)
         SetFilePointerEx(handle, (LARGE_INTEGER){0}, 0, FILE_END); //we're appending, which windows doesn't native support, so we just seek to the end of the file

      *file = (cfile_t){
         .handle = (void*)handle,
         .flags = flags,
         .size = file_size,
      };
   } else {
      //CTalk_ErrorOut(cfile_intern_parse_win32_last_error());
   }

   return(CTalkError_None);
}

int cfile_quit(cfile_t* file) {
   if(!file)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cfile_valid(*file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   CloseHandle(file->handle);
   *file = (cfile_t){0};

   return(CTalkError_None);
}

bool cfile_valid(cfile_t file) {
   return(file.flags != 0 && file.handle != INVALID_HANDLE_VALUE);
}

int cfile_move(cfile_t file, int64_t factor, int from) {
   if(!cfile_valid(file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   DWORD move_method;
   switch(from) {
      case CFile_FromBeginning:
         move_method = FILE_BEGIN;
         break;
      case CFile_FromCurrent:
         move_method = FILE_CURRENT;
         break;
      case CFile_FromEnd:
         move_method = FILE_END;
         break;
   }

   LARGE_INTEGER quad_factor = { .QuadPart = factor };
   SetFilePointerEx((HANDLE)file.handle, quad_factor, 0, move_method);

   //todo: handle SetFilePointer failure

   return(CTalkError_None);
}

bool cfile_readable(cfile_t file) {
   return(cfile_valid(file) && file.flags & CFile_Read);
}

uint32_t cfile_read(cfile_t file, uint32_t num_bytes, int8_t* bytes) {
   uint32_t result = -1;

   if(cfile_valid(file))
      ReadFile((HANDLE)file.handle, (LPVOID)bytes, (DWORD)num_bytes, (LPDWORD)&result, 0);
   else
      CTalk_Error(CTalkError_InvalidArg);

   return(result);
}

bool cfile_writable(cfile_t file) {
   return(cfile_valid(file) && file.flags & CFile_Write);
}

uint32_t cfile_write(cfile_t file, uint32_t num_bytes, int8_t* bytes) {
   uint32_t result = -1;

   if(cfile_valid(file)) {
      WriteFile((HANDLE)file.handle, (LPCVOID)bytes, (DWORD)num_bytes, (LPDWORD)&result, 0);
   } else {
      CTalk_Error(CTalkError_InvalidArg);
   }

   return(result);
}

bool cfile_exists(char* filename) {
   DWORD attribs = GetFileAttributes(filename);
   bool invalid = attribs != INVALID_FILE_ATTRIBUTES;
   bool directory = attribs & FILE_ATTRIBUTE_DIRECTORY;

   return(!(invalid || directory));
}


#elif !defined(CTOOLS_FORCE_STDIO) && defined(__linux__)
#define CFILE_IMPLEMENTATION

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static inline int cfile_intern_translate_errno();
#define PLATFORM_NEEDS_ERRNO_TRANSLATION

int cfile_init(cfile_t* file, char* filename, uint8_t flags) {
   int open_flags = 0;

   switch(flags & CFile_AccessBits) {
      case CFile_Read|CFile_Write:
         open_flags |= O_RDWR;
         break;
      case CFile_Read:
         open_flags |= O_RDONLY;
         break;
      case CFile_Write:
         open_flags |= O_WRONLY;
         break;
      default:
         CTalk_ErrorOut(CTalkError_InvalidArg);
   }

   if(flags & CFile_Write) {
      switch(flags & CFile_OpenBits) {
         case CFile_Create:
            open_flags |= O_CREAT|O_EXCL;
            break;
         case CFile_Overwrite:
            open_flags |= O_TRUNC|O_EXCL;
            break;
         case CFile_Append:
            open_flags |= O_APPEND;
            break;

         case CFile_Create|CFile_Overwrite:
            open_flags |= O_CREAT|O_TRUNC;
            break;
         case CFile_Create|CFile_Append:
            open_flags |= O_CREAT|O_APPEND;
            break;
      }
   }

   int handle = open(filename, open_flags);
   if(handle != -1) {
      //since we compile with warnings as errors (to avoid flooding the builds of people who compile with warnings)
      //this will throw warnings if we just raw cast the handle to a void*, so we have to or it with a blank long.
      //I'd imagine this is optimized out (or otherwise minimized); however it should be investigated
      // -remi (01 April 2023)

      off_t size = lseek(handle, 0, SEEK_END);
      lseek(handle, 0, SEEK_SET);

      if(size != -1) {
         *file = (cfile_t){
            .handle = (void*)(0L|handle),
            .flags = flags,
            .size = size,
         };
      } else {
         CTalk_ErrorOut(cfile_intern_translate_errno());
      }
   } else {
      CTalk_ErrorOut(cfile_intern_translate_errno());
   }

   return(CTalkError_None);
}

int cfile_quit(cfile_t* file) {
   if(!file)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(!cfile_valid(*file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   int handle = (int)(int64_t)file->handle;

   *file = (cfile_t){0};

   if(close(handle) == -1)
      CTalk_ErrorOut(cfile_intern_translate_errno());

   return(CTalkError_None);
}

bool cfile_valid(cfile_t file) {
   int handle = (int)(int64_t)file.handle;
   return(handle != -1);
}

/*
 * note:
 * we take in `factor` as an int64_t here, even though it is immediately cast to an int32_t.
 * Why? Because on Windows we can support Large Files, however the Linux/CRT libraries do not have support this.
 * I'm sure there is a way to support it, but as of right now I don't need it.
 * If it comes time that I need 4GiB+ files (or if there is enough support for it), I'll add it.
 *  -remi (5 April 2023)
 */
int cfile_move(cfile_t file, int64_t factor64, int from) {
   int32_t factor = (int32_t)factor64;

   if(cfile_valid(file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   int whence = 0;
   switch(from) {
      case CFile_FromBeginning:
         whence = SEEK_SET;
         break;
      case CFile_FromCurrent:
         whence = SEEK_CUR;
         break;
      case CFile_FromEnd:
         factor *= -1;
         whence = SEEK_END;
         break;
   }

   int handle = (int)(int64_t)file.handle;
   lseek(handle, factor, whence);

   return(CTalkError_None);
}

bool cfile_readable(cfile_t file) {
   return(cfile_valid(file) && file.flags & CFile_Read);
}

uint32_t cfile_read(cfile_t file, uint32_t num_bytes, int8_t* bytes) {
   if(!cfile_valid(file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   int32_t bytes_read = 0;
   int handle = (int)(int64_t)file.handle;

   bytes_read = (int32_t)read(handle, bytes, num_bytes);
   if(bytes_read == -1)
      CTalk_ErrorOut(cfile_intern_translate_errno());

   return((uint32_t)bytes_read);
}

bool cfile_writable(cfile_t file) {
   return(cfile_valid(file) && file.flags & CFile_Write);
}

uint32_t cfile_write(cfile_t file, uint32_t num_bytes, int8_t* bytes) {
   if(!cfile_valid(file))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   int32_t bytes_read = 0;
   int handle = (int)(int64_t)file.handle;

   bytes_read = (int32_t)write(handle, bytes, num_bytes);
   if(bytes_read == -1)
      CTalk_ErrorOut(cfile_intern_translate_errno());

   return((uint32_t)bytes_read);
}

bool cfile_exists(char* filename) {
   bool result = false;

   if(filename)
      result = (access(filename, F_OK) == 0);
   else
      CTalk_Error(CTalkError_NullPointer);

   return(result);
}

#endif

#if defined(CTOOLS_FORCE_STDIO) || !defined(CFILE_IMPLEMENTATION)

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif//_MSC_VER

#include <stdio.h>
#include <errno.h>

static inline int cfile_intern_translate_errno();
#define PLATFORM_NEEDS_ERRNO_TRANSLATION

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
      CTalk_ErrorOut(cfile_intern_translate_errno());
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


bool cfile_valid(cfile_t file) {
   return(file.handle != NULL);
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
INVALID_HANDLE_VALUE
   ctalk_last_error = CTalkError_None;

   cfile_t file;
   cfile_init(&file, filename, CFile_Read);

   if (ctalk_error() != CTalkError_FileNotFound)
      result = true;

   if(cfile_valid(file))
      cfile_quit(&file);

   return(result);
}

#endif//defined(CTOOLS_FORCE_STDIO) || !defined(CTOOLS_CFILE_IMPLEMENTATION)

#ifdef PLATFORM_NEEDS_ERRNO_TRANSLATION
#undef PLATFORM_NEEDS_ERRNO_TRANSLATION
static inline int cfile_intern_translate_errno() {
   switch(errno) {
      default:
         return(CTalkError_Unspecified);
      case EBADF:
         return(CTalkError_InvalidArg);
      case EINVAL:
         return(CTalkError_InvalidArg);
      case EOVERFLOW:
         return(CTalkError_FileTooBig);
      case EACCES:
         return(CTalkError_AccessDenied);
      case EISDIR:
         return(CTalkError_WriteDir);
      case ENFILE:
         return(CTalkError_TooManyFilesOpen);
      case EMFILE:
         return(CTalkError_TooManyFilesOpen);
      case ENAMETOOLONG:
         return(CTalkError_PathExceedsLimit);
      case ELOOP:
         return(CTalkError_SymlinkLoop);
      case ESPIPE:
         return(CTalkError_Unsupported);
      case ENXIO:
         return(CTalkError_EndOfStream);
      case EINTR:
         return(CTalkError_SignalInterrupt);
      case ENOSPC:
         return(CTalkError_NotEnoughSpace);
  }
}
#endif//PLATFORM_NEEDS_ERRNO_TRANSLATION



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


int cstream_write_memory(cstream_t* stream, void* memory, uint32_t length, int flags) {
   if(!stream || !memory)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_Memory;
   stream->flags = flags|CStream_IsWrite;
   stream->position = 0;
   stream->length = length;
   stream->memory = memory;

   return(CTalkError_None);
}

int cstream_write_file(cstream_t* stream, char* filename, uint32_t length, int file_flags, int flags) {
   if(!stream || !filename)
      CTalk_ErrorOut(CTalkError_NullPointer);
   if(cstream_valid(*stream))
      CTalk_ErrorOut(CTalkError_InvalidArg);

   stream->type = CStreamType_File;
   stream->flags = flags|CStream_IsWrite;
   stream->position = 0;
   stream->length = length;

   cfile_init(&stream->file, filename, file_flags);

   if(cfile_valid(stream->file)) {
      if(stream->length == 0) {
         if(stream->file.size > 0)
            stream->length = stream->file.size;
         else
            stream->length = 0xFFFFFFFF;
      }
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
            if(stream->position + count >= stream->length)
               count = stream->length - stream->position;

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

