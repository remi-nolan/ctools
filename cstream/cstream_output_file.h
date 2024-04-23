#ifndef __CSTREAM_OUTPUT_FILE__
#define __CSTREAM_OUTPUT_FILE__

#include <stdbool.h>
#include <stdint.h>

typedef struct cstream_output_file_t
{
   uint64_t bytes_written;
   void* handle;
} cstream_output_file_t;

bool cstream_output_file_init(cstream_output_file_t* file_stream, char* file_name);

void cstream_output_file_quit(cstream_output_file_t* file_stream);

bool cstream_output_file_valid(cstream_output_file_t file_stream);

uint64_t cstream_output_file_write(cstream_output_file_t* file_stream, uint64_t desired_byte_count, void* value);

#endif//__CSTREAM_OUTPUT_FILE__
