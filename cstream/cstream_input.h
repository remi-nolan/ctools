#ifndef __CSTREAM_INPUT__
#define __CSTREAM_INPUT__

#include "cstream_input_memory.h"
#include "cstream_input_file.h"

#include <stdbool.h>

typedef union cstream_input_source_t
{
   cstream_input_memory_t memory;
   cstream_input_file_t file;
} cstream_input_source_t;

typedef struct cstream_input_t
{
   uint8_t source_type;
   cstream_input_source_t source;
} cstream_input_t;

extern cstream_input_t cstream_standard_in;

bool cstream_input_init_memory(cstream_input_t* stream_input, uint64_t memory_length, void* memory);
bool cstream_input_init_memory_no_copy(cstream_input_t* stream_input, uint64_t memory_length, void* memory);

bool cstream_input_init_file(cstream_input_t* input_stream, char* filename);
bool cstream_input_init_stdin(cstream_input_t* input_stream);

void cstream_input_quit(cstream_input_t* stream_input);

bool cstream_input_valid(cstream_input_t stream_input);

uint64_t cstream_input_read(cstream_input_t* stream_input, uint64_t desired_byte_count, void* destination);

bool cstream_input_read_int8(cstream_input_t* stream_input, int8_t* destination);
bool cstream_input_read_uint8(cstream_input_t* stream_input, uint8_t* destination);

bool cstream_input_read_int16(cstream_input_t* stream_input, int16_t* destination);
bool cstream_input_read_uint16(cstream_input_t* stream_input, uint16_t* destination);

bool cstream_input_read_int32(cstream_input_t* stream_input, int32_t* destination);
bool cstream_input_read_uint32(cstream_input_t* stream_input, uint32_t* destination);

bool cstream_input_read_int64(cstream_input_t* stream_input, int64_t* destination);
bool cstream_input_read_uint64(cstream_input_t* stream_input, uint64_t* destination);

#endif//__CSTREAM_INPUT__
