#include "cstream.h"

#include <stdlib.h>

bool cstream_output_init_memory(cstream_output_t* output_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   output_stream->source_type = CSTREAM_MEMORY;
   result = cstream_output_memory_init(&output_stream->source.memory, memory_length, memory);

   return(result);
}

bool cstream_output_init_memory_allocate(cstream_output_t* output_stream, uint64_t memory_length)
{
   bool result = false;

   output_stream->source_type = CSTREAM_MEMORY;
   result = cstream_output_memory_init_allocate(&output_stream->source.memory, memory_length);

   return(result);
}

bool cstream_output_init_file(cstream_output_t* output_stream, char* file_name)
{
   bool result = false;

   output_stream->source_type = CSTREAM_FILE;
   result = cstream_output_file_init(&output_stream->source.file, file_name);

   return(result);
}

void cstream_output_quit(cstream_output_t* output_stream)
{
   if(output_stream)
   {
      switch(output_stream->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_quit(&output_stream->source.memory);
         break;
      case CSTREAM_FILE:
         cstream_output_file_quit(&output_stream->source.file);
         break;
      }

      output_stream->source_type = CSTREAM_UNKNOWN;
      output_stream->source = (cstream_output_source_t){0};
   }
}

bool cstream_output_valid(cstream_output_t output_stream)
{
   bool result = false;

   switch(output_stream.source_type)
   {
   default:
   case CSTREAM_UNKNOWN:
      result = false;
      break;

   case CSTREAM_MEMORY:
      result = cstream_output_memory_valid(output_stream.source.memory);
      break;

   case CSTREAM_FILE:
      result = cstream_output_file_valid(output_stream.source.file);
      break;
   }

   return(result);
}

uint64_t cstream_output_write(cstream_output_t* output_stream, uint64_t byte_count, void* source)
{
   uint64_t result = 0;

   if(output_stream && cstream_output_valid(*output_stream))
   {
      switch(output_stream->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_output_memory_write(&output_stream->source.memory, byte_count, source);
         break;
      case CSTREAM_FILE:
         result = cstream_output_file_write(&output_stream->source.file, byte_count, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_int8(cstream_output_t* output_stream, int8_t source)
{
   return(cstream_output_write(output_stream, 1, &source) == 1);
}

bool cstream_output_write_uint8(cstream_output_t* output_stream, uint8_t source)
{
   return(cstream_output_write(output_stream, 1, &source) == 1);
}

bool cstream_output_write_int16(cstream_output_t* output_stream, int16_t source)
{
   return(cstream_output_write(output_stream, 2, &source) == 2);
}

bool cstream_output_write_uint16(cstream_output_t* output_stream, uint16_t source)
{
   return(cstream_output_write(output_stream, 2, &source) == 2);
}

bool cstream_output_write_int32(cstream_output_t* output_stream, int32_t source)
{
   return(cstream_output_write(output_stream, 4, &source) == 4);
}

bool cstream_output_write_uint32(cstream_output_t* output_stream, uint32_t source)
{
   return(cstream_output_write(output_stream, 4, &source) == 4);
}

bool cstream_output_write_int64(cstream_output_t* output_stream, int64_t source)
{
   return(cstream_output_write(output_stream, 8, &source) == 8);
}

bool cstream_output_write_uint64(cstream_output_t* output_stream, uint64_t source)
{
   return(cstream_output_write(output_stream, 8, &source) == 8);
}

