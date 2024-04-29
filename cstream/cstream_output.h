#ifndef __CSTREAM_OUTPUT__
#define __CSTREAM_OUTPUT__

#include <stdint.h>
#include <stdbool.h>

#include "cstream_output_memory.h"
#include "cstream_output_file.h"

typedef union cstream_output_source_t
{
   cstream_output_memory_t memory;
   cstream_output_file_t file;
} cstream_output_source_t;

typedef struct cstream_output_t
{
   uint32_t source_type;
   cstream_output_source_t source;
} cstream_output_t;

bool cstream_output_init_memory(cstream_output_t* output_stream, uint64_t memory_length, void* memory);
bool cstream_output_init_memory_allocate(cstream_output_t* output_stream, uint64_t memory_length);

bool cstream_output_init_file(cstream_output_t* output_stream, char* file_name);

void cstream_output_quit(cstream_output_t* output_stream);

bool cstream_output_valid(cstream_output_t output_stream);

uint64_t cstream_output_write(cstream_output_t* output_stream, uint64_t byte_count, void* source);

bool cstream_output_write_int8(cstream_output_t* output_stream, int8_t source);
bool cstream_output_write_uint8(cstream_output_t* output_stream, uint8_t source);

bool cstream_output_write_int16(cstream_output_t* output_stream, int16_t source);
bool cstream_output_write_uint16(cstream_output_t* output_stream, uint16_t source);

bool cstream_output_write_int32(cstream_output_t* output_stream, int32_t source);
bool cstream_output_write_uint32(cstream_output_t* output_stream, uint32_t source);

bool cstream_output_write_int64(cstream_output_t* output_stream, int64_t source);
bool cstream_output_write_uint64(cstream_output_t* output_stream, uint64_t source);

uint64_t cstream_output_write_string(cstream_output_t* output_stream, const char* format_string, ...);

#endif//__CSTREAM_OUTPUT__
