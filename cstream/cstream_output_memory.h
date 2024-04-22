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

#endif//__CSTREAM_OUTPUT_MEMORY__
