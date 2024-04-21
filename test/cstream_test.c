#include <cstream.h>
#include <stdbool.h>
#include <stdio.h>

uint8_t  buffer[] = {
   0xBE, 0xEF,
   32,
   0xCA, 0xFE,
   0xDE, 0xAD, 0xBE, 0xEF,
   0xDE, 0xAD, 0xCA, 0xFE, 0xBE, 0xEF, 0xDA, 0xDE,
};

bool test_input(cstream_input_t* input)
{
   if(input && cstream_input_valid(*input))
   {
      uint8_t read_array[2];
      if(!(cstream_input_read(input, 2, &read_array[0]) && read_array[0] == 0xBE && read_array[1] == 0xEF))
         return(false);

      int8_t read8;
      if(!(cstream_input_read_int8(input, &read8) && read8 == 32))
         return(false);

      uint16_t read16;
      if(!(cstream_input_read_uint16(input, &read16) && read16 == 0xFECA))
         return(false);

      int32_t read32;
      if(!(cstream_input_read_int32(input, &read32) && read32 == 0xEFBEADDE))
         return(false);

      uint64_t read64;
      if(!(cstream_input_read_uint64(input, &read64) && read64 == 0xDEDAEFBEFECAADDE))
         return(false);
   }

   return(true);
}

int main(int arg_count, char** args)
{
   cstream_input_t memory_input;
   if(cstream_input_init_memory(&memory_input, sizeof(buffer), &buffer[0]))
   {
      if(!test_input(&memory_input))
         return(1);
   }

   return(0);
}

