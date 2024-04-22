#ifndef __CSTREAM_INPUT_MEMORY__
#define __CSTREAM_INPUT_MEMORY__

#include <stdint.h>
#include <stdbool.h>

typedef struct cstream_input_memory_t
{
   uint32_t length;
   uint32_t position;
   void* buffer;
} cstream_input_memory_t;

bool cstream_input_memory_init(cstream_input_memory_t* memory_stream, void* memory, uint32_t memory_length);

bool cstream_input_memory_init_no_copy(cstream_input_memory_t* memory_stream, void* memory, uint32_t memory_length);

void cstream_input_memory_quit(cstream_input_memory_t* memory_stream);

bool cstream_input_memory_valid(cstream_input_memory_t memory_stream);

uint32_t cstream_input_memory_read(cstream_input_memory_t* memory_stream, uint32_t desired_byte_count, void* destination);

#endif//__CSTREAM_INPUT_MEMORY__
