#ifndef __CSTREAM_OUTPUT__
#define __CSTREAM_OUTPUT__

#include <stdint.h>
#include <stdbool.h>

#include "cstream_output_memory.h"

typedef union cstream_output_source_t
{
   cstream_output_memory_t memory;
} cstream_output_source_t;

typedef struct cstream_output_t
{
   uint32_t source_type;
   cstream_output_source_t source;
} cstream_output_t;

bool cstream_output_init_memory(cstream_output_t* stream_output, uint32_t memory_length, void* memory);
bool cstream_output_init_memory_allocate(cstream_output_t* stream_output, uint32_t memory_length);

void cstream_output_quit(cstream_output_t* stream_output);

bool cstream_output_valid(cstream_output_t stream_output);

uint32_t cstream_output_write(cstream_output_t* stream_output, uint32_t byte_count, void* source);

bool cstream_output_write_int8(cstream_output_t* stream_output, int8_t source);
bool cstream_output_write_uint8(cstream_output_t* stream_output, uint8_t source);

bool cstream_output_write_int16(cstream_output_t* stream_output, int16_t source);
bool cstream_output_write_uint16(cstream_output_t* stream_output, uint16_t source);

bool cstream_output_write_int32(cstream_output_t* stream_output, int32_t source);
bool cstream_output_write_uint32(cstream_output_t* stream_output, uint32_t source);

bool cstream_output_write_int64(cstream_output_t* stream_output, int64_t source);
bool cstream_output_write_uint64(cstream_output_t* stream_output, uint64_t source);

#endif//__CSTREAM_OUTPUT__
