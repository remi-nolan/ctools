/*
 * cbug - cbug.h
 * a minimalist c99, header-only cross-platform debug toolkit (assert & debugbreak)
 *
 * License:
 * ANTI-CAPITALIST SOFTWARE LICENSE(v 1.4)
 *
 * Copyright © 2023 Remi Nolan Eidahl
 *
 * This is anti-capitalist software, released for free use by individuals and organizations that do not operate by capitalist principles.
 *
 * Permission is hereby granted, free of charge, to any person or organization(the "User") obtaining a copy of this software and associated documentation files(the "Software"), to use, copy, modify, merge, distribute, and/or sell copies of the Software, subject to the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be included in all copies or modified versions of the Software.
 *
 * 2. The User is one of the following:
 * a. An individual person, laboring for themselves
 * b. A non-profit organization
 * c. An educational institution
 * d. An organization that seeks shared profit for all of its members, and allows non-members to set the cost of their labor
 *
 * 3. If the User is an organization with owners, then all owners are workers and all workers are owners with equal equity and/or equal vote.
 *
 * 4. If the User is an organization, then the User is not law enforcement or military, or working for or under either.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef CTOOLS_STDINT
#define CTOOLS_STDINT

#ifndef CTOOLS_NO_STANDARD_INCLUDE
#include <stdint.h>
#include <stdbool.h>
#else

#define int8_t    signed char
#define int16_t   signed short
#define int32_t   signed long
#define int64_t   signed long long
#define uint8_t   unsigned char
#define uint16_t  unsigned short
#define uint32_t  unsigned long
#define uint64_t  unsigned long long

#ifndef bool
#define bool _Bool
#endif//bool
   
#ifndef true
#define true (1)
#endif//true

#ifndef false
#define false (0)
#endif//false
#endif//CTOOLS_NO_STANDARD_INCLUDE

#endif//CTOOLS_STDINT

#ifndef CBUG_INCLUDED
#define CBUG_INCLUDED

#ifndef CBUG_STUB
   #ifdef _MSC_VER
      #ifdef _M_X64
         #define CBug_Breakpoint(...) __asm int3
      #elif defined(_M_ARM) || defined(_M_ARMT)
         #define CBug_Breakpoint(...) __asm BKPT
      #else//not x86_64 or arm
         #error CBug doesn't support the current architecture
      #endif
   #elif defined(__GNUC__) || defined(__llvm__)
      #if defined(__x86_64)
         #define CBug_Breakpoint(...) asm("int $3")
      #elif defined(__arm__)
         #define CBug_Breakpoint(...) asm("BKPT")
      #else//not x86_64 or arm
         #error CBug doesn't support the current architecture
      #endif
   #else
      #error CBUG doesn't support the current compiler
   #endif

   typedef bool (*CBug_Assert_Fail)(const char* condition, const char* filename, int64_t line, const char* function);
   extern CBug_Assert_Fail cbug_assert_fail;

   #ifdef CBug_Assert
      #undef CBug_Assert
   #endif//CBUG_ASSERT
   #define CBug_Assert(cond) if(!(cond) && ((cbug_assert_fail != 0)?cbug_assert_fail(#cond, __FILE__, __LINE__, __func__):true)) CBug_Breakpoint()
#else
   #define CBug_Breakpoint(...)
   #define CBug_Assert(...)
#endif//CBUG_STUB

#endif//CBUG_INCLUDED

#if defined(CTOOLS_IMPLEMENTATION) || defined(CBUG_IMPLEMENTATION)
CBug_Assert_Fail cbug_assert_fail = 0;
#endif//defined(CTOOLS_IMPLEMENTATION) || defined(CBUG_IMPLEMENTATION)

