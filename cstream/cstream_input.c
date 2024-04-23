#include "cstream.h"

bool cstream_input_init_memory(cstream_input_t* stream_input, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_MEMORY;
      result = cstream_input_memory_init(&stream_input->source.memory, memory_length, memory);
   }

   return(result);
}

bool cstream_input_init_memory_no_copy(cstream_input_t* stream_input, uint64_t memory_length, void* memory)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_MEMORY;
      result = cstream_input_memory_init_no_copy(&stream_input->source.memory, memory_length, memory);
   }

   return(result);
}

bool cstream_input_init_file(cstream_input_t* stream_input, char* file_name)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_FILE;
      result = cstream_input_file_init(&stream_input->source.file, file_name);
   }

   return(result);
}

void cstream_input_quit(cstream_input_t* stream_input)
{
   if(stream_input)
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_input_memory_quit(&stream_input->source.memory);
         break;
      case CSTREAM_FILE:
         cstream_input_file_quit(&stream_input->source.file);
         break;
      }

      stream_input->source_type = CSTREAM_UNKNOWN;
      stream_input->source = (cstream_input_source_t){0};
   }
}

bool cstream_input_valid(cstream_input_t stream_input)
{
   return(stream_input.source_type != CSTREAM_UNKNOWN);
}

uint64_t cstream_input_read(cstream_input_t* stream_input, uint64_t desired_byte_count, void* destination)
{
   uint64_t result = 0;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read(&stream_input->source.memory, desired_byte_count, destination);
         break;
      case CSTREAM_FILE:
         result = cstream_input_file_read(&stream_input->source.file, desired_byte_count, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_int8(cstream_input_t* stream_input, int8_t* destination)
{
   return(cstream_input_read(stream_input, 1, destination) == 1);
}

bool cstream_input_read_uint8(cstream_input_t* stream_input, uint8_t* destination)
{
   return(cstream_input_read(stream_input, 1, destination) == 1);
}

bool cstream_input_read_int16(cstream_input_t* stream_input, int16_t* destination)
{
   return(cstream_input_read(stream_input, 2, destination) == 2);
}

bool cstream_input_read_uint16(cstream_input_t* stream_input, uint16_t* destination)
{
   return(cstream_input_read(stream_input, 2, destination) == 2);
}

bool cstream_input_read_int32(cstream_input_t* stream_input, int32_t* destination)
{
   return(cstream_input_read(stream_input, 4, destination) == 4);
}

bool cstream_input_read_uint32(cstream_input_t* stream_input, uint32_t* destination)
{
   return(cstream_input_read(stream_input, 4, destination) == 4);
}

bool cstream_input_read_int64(cstream_input_t* stream_input, int64_t* destination)
{
   return(cstream_input_read(stream_input, 8, destination) == 8);
}

bool cstream_input_read_uint64(cstream_input_t* stream_input, uint64_t* destination)
{
   return(cstream_input_read(stream_input, 8, destination) == 8);
}

