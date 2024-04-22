#include "cstream.h"

#include <stdio.h>

bool cstream_input_file_init(cstream_input_file_t* file_stream, char* file_name)
{
   bool result = false;

   if(file_stream && file_name)
   {
      FILE* file_handle = fopen(file_name, "rb");

      if(file_handle)
      {
         file_stream->position = 0;

         fseek(file_stream->handle, 0L, SEEK_END);
         file_stream->length = ftell(file_stream->handle);
         rewind(file_stream->handle);

         file_stream->handle = (void*)file_handle;

         result = true;
      }
   }

   return(result);
}

void cstream_input_file_quit(cstream_input_file_t* file_stream)
{
   if(file_stream && cstream_input_file_valid(*file_stream))
   {
      fclose((FILE*)file_stream->handle);
      *file_stream = (cstream_input_file_t){0};
   }
}

bool cstream_input_file_valid(cstream_input_file_t file_stream)
{
   return(file_stream.length && file_stream.handle);
}

uint32_t cstream_input_file_read(cstream_input_file_t* file_stream, uint32_t desired_byte_count, void* destination)
{
   uint32_t bytes_read = 0;

   if(file_stream && cstream_input_file_valid(*file_stream) && destination)
   {
      if((file_stream->position + desired_byte_count) >= file_stream->length)
      {
         desired_byte_count = file_stream->length - file_stream->position;
      }

      bytes_read = fread(destination, 1, desired_byte_count, (FILE*)file_stream->handle);
   }

   return(bytes_read);
}

