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

bool cstream_input_memory_read_int8(cstream_input_memory_t* memory_stream, int8_t* destination);
bool cstream_input_memory_read_uint8(cstream_input_memory_t* memory_stream, uint8_t* destination);

bool cstream_input_memory_read_int16(cstream_input_memory_t* memory_stream, int16_t* destination);
bool cstream_input_memory_read_uint16(cstream_input_memory_t* memory_stream, uint16_t* destination);

bool cstream_input_memory_read_int32(cstream_input_memory_t* memory_stream, int32_t* destination);
bool cstream_input_memory_read_uint32(cstream_input_memory_t* memory_stream, uint32_t* destination);

bool cstream_input_memory_read_int64(cstream_input_memory_t* memory_stream, int64_t* destination);
bool cstream_input_memory_read_uint64(cstream_input_memory_t* memory_stream, uint64_t* destination);

#endif//__CSTREAM_INPUT_MEMORY__
