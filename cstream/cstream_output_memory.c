#include "cstream.h"

#include <stdlib.h>

bool cstream_output_memory_init(cstream_output_memory_t* memory_stream, void* memory, uint32_t memory_length)
{
   bool result = false;

   if(memory_stream && memory_length && memory)
   {
      memory_stream->position = 0;
      memory_stream->length = memory_length;
      memory_stream->memory = memory;
   }

   return(result);
}

bool cstream_output_memory_init_allocate(cstream_output_memory_t* memory_stream, uint32_t memory_length)
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

uint32_t cstream_output_memory_write(cstream_output_memory_t* memory_stream, uint32_t desired_byte_count, void* value)
{
   uint32_t bytes_written = 0;

   if(memory_stream && memory_stream->memory)
   {
      uint8_t* memory_bytes = ((uint8_t*)memory_stream->memory) + memory_stream->position;
      uint8_t* value_bytes = (uint8_t*)value;

      if((memory_stream->position + desired_byte_count) >= memory_stream->length)
      {
         desired_byte_count = memory_stream->length - memory_stream->position;
      }

      for(int iter = 0; iter < desired_byte_count; ++iter)
      {
         memory_bytes[iter] = value_bytes[iter];
         memory_stream->position++;
         bytes_written++;
      }
   }

   return(bytes_written);
}

bool cstream_output_memory_write_int8(cstream_output_memory_t* memory_stream, int8_t value)
{
   return(cstream_output_memory_write(memory_stream, 1, &value) == 1);
}

bool cstream_output_memory_write_uint8(cstream_output_memory_t* memory_stream, uint8_t value)
{
   return(cstream_output_memory_write(memory_stream, 1, &value) == 1);
}

bool cstream_output_memory_write_int16(cstream_output_memory_t* memory_stream, int16_t value)
{
   return(cstream_output_memory_write(memory_stream, 2, &value) == 2);
}

bool cstream_output_memory_write_uint16(cstream_output_memory_t* memory_stream, uint16_t value)
{
   return(cstream_output_memory_write(memory_stream, 2, &value) == 2);
}

bool cstream_output_memory_write_int32(cstream_output_memory_t* memory_stream, int32_t value)
{
   return(cstream_output_memory_write(memory_stream, 4, &value) == 4);
}

bool cstream_output_memory_write_uint32(cstream_output_memory_t* memory_stream, uint32_t value)
{
   return(cstream_output_memory_write(memory_stream, 4, &value) == 4);
}

bool cstream_output_memory_write_int64(cstream_output_memory_t* memory_stream, int64_t value)
{
   return(cstream_output_memory_write(memory_stream, 8, &value) == 8);
}

bool cstream_output_memory_write_uint64(cstream_output_memory_t* memory_stream, uint64_t value)
{
   return(cstream_output_memory_write(memory_stream, 8, &value) == 8);
}

