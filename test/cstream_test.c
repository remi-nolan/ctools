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

bool test_output(cstream_output_t* output)
{
   if(output && cstream_output_valid(*output))
   {
      if(cstream_output_write(output, 2, test_string) == 2)
         printf("%x %x ", test_string[0], test_string[1]);
      else
         return(false);

      if(cstream_output_write_uint8(output, test_string[2]))
         printf("%d ", test_string[2]);
      else
         return(false);

      if(cstream_output_write_uint16(output, *((int16_t*)(&test_string[3]))))
         printf("%x ", *((uint16_t*)(&test_string[3])));
      else
         return(false);

      if(cstream_output_write_uint32(output, *((int32_t*)(&test_string[5]))))
         printf("%x ", *((uint32_t*)(&test_string[5])));
      else
         return(false);

      if(cstream_output_write_uint64(output, *((int64_t*)(&test_string[9]))))
         printf("%llx\n", *((uint64_t*)(&test_string[9])));
      else
         return(false);
   }

   return(true);
}

bool test_input(cstream_input_t* input)
{
   if(input && cstream_input_valid(*input))
   {
      uint8_t read_array[2];
      if((cstream_input_read(input, 2, &read_array[0]) == 2) && read_array[0] == 0xBE && read_array[1] == 0xEF)
         printf("%x %x ", read_array[0], read_array[1]);
      else
         return(false);

      int8_t read8;
      if(cstream_input_read_int8(input, &read8) && read8 == 32)
         printf("%d ", read8);
      else
         return(false);

      uint16_t read16;
      if(cstream_input_read_uint16(input, &read16) && read16 == 0xFECA)
         printf("%x ", read16);
      else
         return(false);

      int32_t read32;
      if(cstream_input_read_int32(input, &read32) && read32 == 0xEFBEADDE)
         printf("%x ", read32);
      else
         return(false);

      uint64_t read64;
      if(cstream_input_read_uint64(input, &read64) && read64 == 0xDEDAEFBEFECAADDE)
         printf("%llx\n", read64);
      else
         return(false);
   }

   return(true);
}

int main(int arg_count, char** args)
{
   //write to memory
   {
      cstream_output_t memory_output;
      if(cstream_output_init_memory(&memory_output, sizeof(buffer), &buffer[0]))
      {
         printf("memory_output:\n   ");
         if(!test_output(&memory_output))
            return(-1);
         //we don't have to quit because we didn't allocate the memory
      }
   }

   //read from memory
   {
      cstream_input_t memory_input;
      if(cstream_input_init_memory(&memory_input, sizeof(buffer), &buffer[0]))
      {
         printf("memory_input:\n   ");
         if(!test_input(&memory_input))
            return(-2);
         cstream_input_quit(&memory_input);
      }
   }

   //write to file
   {
      cstream_output_t file_output;
      if(cstream_output_init_file(&file_output, "cstream_test.binary"))
      {
         printf("file_output:\n   ");
         if(!test_output(&file_output))
            return(-3);
         cstream_output_quit(&file_output);
      }
   }

   //read from file
   {
      cstream_input_t file_input;
      if(cstream_input_init_file(&file_input, "cstream_test.binary"))
      {
         printf("file_input:\n   ");
         if(!test_input(&file_input))
            return(-4);
         cstream_input_quit(&file_input);
      }
   }

   return(0);
}

