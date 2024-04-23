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

