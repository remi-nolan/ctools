#ifndef __CSTREAM_INPUT_FILE__
#define __CSTREAM_INPUT_FILE__

#include <stdint.h>
#include <stdbool.h>

typedef struct cstream_input_file_t
{
   uint64_t position;
   uint64_t length;
   void* handle;
} cstream_input_file_t;

bool cstream_input_file_init(cstream_input_file_t* file_stream, char* file_name);

void cstream_input_file_quit(cstream_input_file_t* file_stream);

bool cstream_input_file_valid(cstream_input_file_t file_stream);

uint64_t cstream_input_file_read(cstream_input_file_t* file_stream, uint64_t desired_byte_count, void* destination);

#endif//__CSTREAM_INPUT_FILE__
