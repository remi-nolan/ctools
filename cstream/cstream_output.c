#include "cstream.h"

#include <stdlib.h>

bool cstream_output_init_memory(cstream_output_t* stream_output, uint32_t memory_length, void* memory)
{
   bool result = false;

   stream_output->source_type = CSTREAM_MEMORY;
   result = cstream_output_memory_init(&stream_output->source.memory, memory, memory_length);

   return(result);
}

bool cstream_output_init_memory_allocate(cstream_output_t* stream_output, uint32_t memory_length)
{
   bool result = false;

   stream_output->source_type = CSTREAM_MEMORY;
   result = cstream_output_memory_init_allocate(&stream_output->source.memory, memory_length);

   return(result);
}

void cstream_output_quit(cstream_output_t* stream_output)
{
   if(stream_output)
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_quit(&stream_output->source.memory);
         break;
      }

      stream_output->source_type = CSTREAM_UNKNOWN;
      stream_output->source = (cstream_output_source_t){0};
   }
}

bool cstream_output_valid(cstream_output_t stream_output)
{
   return(stream_output.source_type > CSTREAM_UNKNOWN && stream_output.source_type < CSTREAM_TYPE_UPPER_BOUND);
}

uint32_t cstream_output_write(cstream_output_t* stream_output, uint32_t byte_count, void* source)
{
   uint32_t result = 0;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write(&stream_output->source.memory, byte_count, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_int8(cstream_output_t* stream_output, int8_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_int8(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_uint8(cstream_output_t* stream_output, uint8_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_uint8(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_int16(cstream_output_t* stream_output, int16_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_int16(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_uint16(cstream_output_t* stream_output, uint16_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_uint16(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_int32(cstream_output_t* stream_output, int32_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_int32(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_uint32(cstream_output_t* stream_output, uint32_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_uint32(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_int64(cstream_output_t* stream_output, int64_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_int64(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

bool cstream_output_write_uint64(cstream_output_t* stream_output, uint64_t source)
{
   bool result = false;

   if(stream_output && cstream_output_valid(*stream_output))
   {
      switch(stream_output->source_type)
      {
      case CSTREAM_MEMORY:
         cstream_output_memory_write_uint64(&stream_output->source.memory, source);
         break;
      }
   }

   return(result);
}

