#include "cstream.h"

#include <stdlib.h>

bool cstream_output_memory_init(cstream_output_memory_t* memory_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(memory_stream && memory_length && memory)
   {
      memory_stream->position = 0;
      memory_stream->length = memory_length;
      memory_stream->memory = memory;

      result = true;
   }

   return(result);
}

bool cstream_output_memory_init_allocate(cstream_output_memory_t* memory_stream, uint64_t memory_length)
{
   bool result = false;

   if(memory_stream && memory_length)
   {
      memory_stream->memory = malloc(memory_length);

      if(memory_stream->memory)
      {
         memory_stream->position = 0;
         memory_stream->length = memory_length;

         result = true;
      }
   }

   return(result);
}

void cstream_output_memory_quit(cstream_output_memory_t* memory_stream)
{
   if(memory_stream && memory_stream->memory)
   {
      free(memory_stream->memory);
      *memory_stream = (cstream_output_memory_t){0};
   }
}

bool cstream_output_memory_valid(cstream_output_memory_t memory_stream)
{
   return(memory_stream.memory && memory_stream.length);
}

uint64_t cstream_output_memory_write(cstream_output_memory_t* memory_stream, uint64_t desired_byte_count, void* value)
{
   uint64_t bytes_written = 0;

   if(memory_stream && memory_stream->memory)
   {
      uint8_t* memory_bytes = ((uint8_t*)memory_stream->memory) + memory_stream->position;
      uint8_t* value_bytes = (uint8_t*)value;

      if((memory_stream->position + desired_byte_count) >= memory_stream->length)
      {
         desired_byte_count = memory_stream->length - memory_stream->position;
      }

      for(uint64_t iter = 0; iter < desired_byte_count; ++iter)
      {
         memory_bytes[iter] = value_bytes[iter];
         memory_stream->position++;
         bytes_written++;
      }
   }

   return(bytes_written);
}

