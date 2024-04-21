#ifndef __CSTREAM_OUTPUT_MEMORY__
#define __CSTREAM_OUTPUT_MEMORY__

#include <stdbool.h>
#include <stdint.h>

#include "cstream.h"

typedef struct cstream_output_memory_t
{
   uint32_t position;
   uint32_t length;
   void* memory;
} cstream_output_memory_t;

bool cstream_output_memory_init(cstream_output_memory_t* memory_stream, void* memory, uint32_t memory_length);
bool cstream_output_memory_init_allocate(cstream_output_memory_t* memory_stream, uint32_t memory_length);

void cstream_output_memory_quit(cstream_output_memory_t* memory_stream);

bool cstream_output_memory_valid(cstream_output_memory_t memory_stream);

uint32_t cstream_output_memory_write(cstream_output_memory_t* memory_stream, uint32_t desired_byte_count, void* value);

bool cstream_output_memory_write_int8(cstream_output_memory_t* memory_stream, int8_t value);
bool cstream_output_memory_write_uint8(cstream_output_memory_t* memory_stream, uint8_t value);

bool cstream_output_memory_write_int16(cstream_output_memory_t* memory_stream, int16_t value);
bool cstream_output_memory_write_uint16(cstream_output_memory_t* memory_stream, uint16_t value);

bool cstream_output_memory_write_int32(cstream_output_memory_t* memory_stream, int32_t value);
bool cstream_output_memory_write_uint32(cstream_output_memory_t* memory_stream, uint32_t value);

bool cstream_output_memory_write_int64(cstream_output_memory_t* memory_stream, int64_t value);
bool cstream_output_memory_write_uint64(cstream_output_memory_t* memory_stream, uint64_t value);

#endif//__CSTREAM_OUTPUT_MEMORY__
