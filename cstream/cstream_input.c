#include "cstream.h"

bool cstream_input_init_memory(cstream_input_t* stream_input, uint32_t memory_length, void* memory)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_MEMORY;
      result = cstream_input_memory_init(&stream_input->source.memory, memory, memory_length);
   }

   return(result);
}

bool cstream_input_init_memory_no_copy(cstream_input_t* stream_input, void* memory, uint32_t memory_length)
{
   bool result = false;

   if(stream_input)
   {
      stream_input->source_type = CSTREAM_MEMORY;
      result = cstream_input_memory_init_no_copy(&stream_input->source.memory, memory, memory_length);
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
      }

      stream_input->source_type = CSTREAM_UNKNOWN;
      stream_input->source = (cstream_input_source_t){0};
   }
}

bool cstream_input_valid(cstream_input_t stream_input)
{
   return(stream_input.source_type > CSTREAM_UNKNOWN && stream_input.source_type < CSTREAM_TYPE_UPPER_BOUND);
}

uint32_t cstream_input_read(cstream_input_t* stream_input, uint32_t desired_byte_count, void* destination)
{
   uint32_t result = 0;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read(&stream_input->source.memory, desired_byte_count, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_int8(cstream_input_t* stream_input, int8_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_int8(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_uint8(cstream_input_t* stream_input, uint8_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_uint8(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_int16(cstream_input_t* stream_input, int16_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_int16(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}
bool cstream_input_read_uint16(cstream_input_t* stream_input, uint16_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_uint16(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_int32(cstream_input_t* stream_input, int32_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_int32(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}
bool cstream_input_read_uint32(cstream_input_t* stream_input, uint32_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_uint32(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_int64(cstream_input_t* stream_input, int64_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_int64(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}

bool cstream_input_read_uint64(cstream_input_t* stream_input, uint64_t* destination)
{
   bool result = false;

   if(stream_input && cstream_input_valid(*stream_input))
   {
      switch(stream_input->source_type)
      {
      case CSTREAM_MEMORY:
         result = cstream_input_memory_read_uint64(&stream_input->source.memory, destination);
         break;
      }
   }

   return(result);
}


