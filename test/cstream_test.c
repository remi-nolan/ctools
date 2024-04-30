#include <cstream.h>
#include <stdbool.h>
#include <stdio.h>

static uint8_t test_string[] = {
   0xBE, 0xEF,
   32,
   0xCA, 0xFE,
   0xDE, 0xAD, 0xBE, 0xEF,
   0xDE, 0xAD, 0xCA, 0xFE, 0xBE, 0xEF, 0xDA, 0xDE,
};
static uint8_t buffer[sizeof(test_string)];

void test_output(cstream_output_t* output)
{
   if(output && cstream_output_valid(*output))
   {
      cstream_output_write_string(&cstream_standard_out, "   cstream_output_write: ");
      if(cstream_output_write(output, 2, test_string) == 2)
         cstream_output_write_string(&cstream_standard_out, "%x %x\n", test_string[0], test_string[1]);
      else
         cstream_output_write_string(&cstream_standard_out, "fail\n", test_string[0], test_string[1]);

      cstream_output_write_string(&cstream_standard_out, "   cstream_output_write_uint8: ");
      if(cstream_output_write_uint8(output, test_string[2]))
         cstream_output_write_string(&cstream_standard_out, "%d\n", test_string[2]);
      else
         cstream_output_write_string(&cstream_standard_out, "fail\n", test_string[0], test_string[1]);

      cstream_output_write_string(&cstream_standard_out, "   cstream_output_write_uint16: ");
      if(cstream_output_write_uint16(output, *((int16_t*)(&test_string[3]))))
         cstream_output_write_string(&cstream_standard_out, "%x\n", *((uint16_t*)(&test_string[3])));
      else
         cstream_output_write_string(&cstream_standard_out, "fail\n", test_string[0], test_string[1]);

      cstream_output_write_string(&cstream_standard_out, "   cstream_output_write_uint32: ");
      if(cstream_output_write_uint32(output, *((int32_t*)(&test_string[5]))))
         cstream_output_write_string(&cstream_standard_out, "%x\n", *((uint32_t*)(&test_string[5])));
      else
         cstream_output_write_string(&cstream_standard_out, "fail\n", test_string[0], test_string[1]);

      cstream_output_write_string(&cstream_standard_out, "   cstream_output_write_uint64: ");
      if(cstream_output_write_uint64(output, *((int64_t*)(&test_string[9]))))
         cstream_output_write_string(&cstream_standard_out, "%llx\n", *((uint64_t*)(&test_string[9])));
      else
         cstream_output_write_string(&cstream_standard_out, "fail\n", test_string[0], test_string[1]);
   }
}

void test_input(cstream_input_t* input)
{
   if(input && cstream_input_valid(*input))
   {
      uint8_t read_array[2];
      cstream_output_write_string(&cstream_standard_out, "   cstream_input_read: ");
      if((cstream_input_read(input, 2, &read_array[0]) == 2) && read_array[0] == 0xBE && read_array[1] == 0xEF)
         cstream_output_write_string(&cstream_standard_out, "%x %x\n", read_array[0], read_array[1]);
      else
         cstream_output_write_string(&cstream_standard_error, "fail\n");

      int8_t read8;
      cstream_output_write_string(&cstream_standard_out, "   cstream_input_read_int8: ");
      if(cstream_input_read_int8(input, &read8) && read8 == 32)
         cstream_output_write_string(&cstream_standard_out, "%d\n", read8);
      else
         cstream_output_write_string(&cstream_standard_error, "fail\n");

      uint16_t read16;
      cstream_output_write_string(&cstream_standard_out, "   cstream_input_read_int16: ");
      if(cstream_input_read_uint16(input, &read16) && read16 == 0xFECA)
         cstream_output_write_string(&cstream_standard_out, "%x\n", read16);
      else
         cstream_output_write_string(&cstream_standard_error, "fail\n");

      int32_t read32;
      cstream_output_write_string(&cstream_standard_out, "   cstream_input_read_int32: ");
      if(cstream_input_read_int32(input, &read32) && read32 == 0xEFBEADDE)
         cstream_output_write_string(&cstream_standard_out, "%x\n", read32);
      else
         cstream_output_write_string(&cstream_standard_error, "fail\n");

      uint64_t read64;
      cstream_output_write_string(&cstream_standard_out, "   cstream_input_read_int64: ");
      if(cstream_input_read_uint64(input, &read64) && read64 == 0xDEDAEFBEFECAADDE)
         cstream_output_write_string(&cstream_standard_out, "%llx\n", read64);
      else
         cstream_output_write_string(&cstream_standard_error, "fail\n");
   }
}

int main(int arg_count, char** args)
{
   //write to memory
   {
      cstream_output_t memory_output;
      if(cstream_output_init_memory(&memory_output, sizeof(buffer), &buffer[0]))
      {
         printf("memory_output:\n");
         test_output(&memory_output);
         //we don't have to quit because we didn't allocate the memory
      }
   }

   //read from memory
   {
      cstream_input_t memory_input;
      if(cstream_input_init_memory(&memory_input, sizeof(buffer), &buffer[0]))
      {
         printf("memory_input:\n");
         test_input(&memory_input);
         cstream_input_quit(&memory_input);
      }
   }

   //write to file
   {
      cstream_output_t file_output;
      if(cstream_output_init_file(&file_output, "cstream_test.binary"))
      {
         printf("file_output:\n");
         test_output(&file_output);
         cstream_output_quit(&file_output);
      }
   }

   //read from file
   {
      cstream_input_t file_input;
      if(cstream_input_init_file(&file_input, "cstream_test.binary"))
      {
         printf("file_input:\n");
         test_input(&file_input);
         cstream_input_quit(&file_input);
      }
   }

   //read from stdin
   {
      uint8_t stdin_value;
      int read_char_count;
      for(read_char_count = 0; read_char_count < sizeof(buffer); ++read_char_count)
      {
         cstream_input_read_uint8(&cstream_standard_in, &stdin_value);

         if(stdin_value == '\r' || stdin_value == '\n')
            break;

         buffer[read_char_count] = stdin_value;
      }

      buffer[read_char_count] = 0;
   }

   //write to stderr
   {
      for(int iter = 0; iter < sizeof(buffer); ++iter)
      {
         if(buffer[iter] == 0)
         {
            cstream_output_write_uint8(&cstream_standard_error, '\n');
            break;
         }
         else
         {
            cstream_output_write_uint8(&cstream_standard_error, buffer[iter]);
         }
      }
   }

   return(0);
}

