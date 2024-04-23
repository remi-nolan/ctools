#include "cstream_input_memory.h"

#include <stdlib.h>

bool cstream_input_memory_init(cstream_input_memory_t* memory_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(memory_stream && memory)
   {
      uint8_t* memory_bytes = (uint8_t*)memory;

      if(memory_length == 0)
      {
         while(*memory_bytes++ != 0)
         {
            memory_length++;
         }
         memory_bytes = memory;
      }

      memory_stream->length = memory_length;
      memory_stream->position = 0;

      memory_stream->buffer = malloc(memory_length);

      if(memory_stream->buffer)
      {
         uint8_t* buffer_bytes = (uint8_t*)memory_stream->buffer;
         for(int iter = 0; iter < memory_length; ++iter)
         {
            *buffer_bytes++ = *memory_bytes++;
         }

         result = true;
      }
   }

   return(result);
}

bool cstream_input_memory_init_no_copy(cstream_input_memory_t* memory_stream, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(memory_stream && memory)
   {
      if(memory_length == 0)
      {
         uint8_t* memory_bytes = (uint8_t*)memory;
         while(*memory_bytes++ != 0)
         {
            memory_length++;
         }
         memory_bytes = memory;
      }

      memory_stream->length = memory_length;
      memory_stream->position = 0;
      memory_stream->buffer = memory;

      result = true;
   }

   return(result);
}

void cstream_input_memory_quit(cstream_input_memory_t* memory_stream)
{
   if(memory_stream && cstream_input_memory_valid(*memory_stream))
   {
      free(memory_stream->buffer);
      *memory_stream = (cstream_input_memory_t){0};
   }
}

bool cstream_input_memory_valid(cstream_input_memory_t memory_stream)
{
   return(memory_stream.length > 0 && memory_stream.buffer != 0);
}

uint64_t cstream_input_memory_read(cstream_input_memory_t* memory_stream, uint64_t desired_byte_count, void* destination)
{
   uint64_t result = 0;

   if(memory_stream && cstream_input_memory_valid(*memory_stream) && destination)
   {
      if((memory_stream->position + desired_byte_count) > memory_stream->length)
      {
         desired_byte_count = memory_stream->length - memory_stream->position;
      }

      if(desired_byte_count > 0)
      {
         uint8_t* destination_bytes = (uint8_t*)destination;
         uint8_t* buffer_bytes = ((uint8_t*)memory_stream->buffer) + memory_stream->position;

         for(int iter = 0; iter < desired_byte_count; ++iter)
         {
            *destination_bytes++ = *buffer_bytes++;

            result++;
            memory_stream->position++;
         }
      }
   }

   return(result);
}

