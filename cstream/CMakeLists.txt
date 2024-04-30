# ANTI-CAPITALIST SOFTWARE LICENSE (v 1.4)
# 
# Copyright © 2024 Remi Eidahl
# 
# This is anti-capitalist software, released for free use by individuals and organizations that do not operate by capitalist principles.
# 
# Permission is hereby granted, free of charge, to any person or organization (the "User") obtaining a copy of this software and associated documentation files (the "Software"), to use, copy, modify, merge, distribute, and/or sell copies of the Software, subject to the following conditions:
# 
# 1. The above copyright notice and this permission notice shall be included in all copies or modified versions of the Software.
# 
# 2. The User is one of the following:
# a. An individual person, laboring for themselves
# b. A non-profit organization
# c. An educational institution
# d. An organization that seeks shared profit for all of its members, and allows non-members to set the cost of their labor
# 
# 3. If the User is an organization with owners, then all owners are workers and all workers are owners with equal equity and/or equal vote.
# 
# 4. If the User is an organization, then the User is not law enforcement or military, or working for or under either.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.28)

set(CSTREAM_HEADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream.h

   ${CMAKE_CURRENT_SOURCE_DIR}/stb_sprintf.h

   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_input_memory.h
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_input_file.h
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_input.h

   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_output_memory.h
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_output_file.h
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_output.h
)

set(CSTREAM_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/stb_sprintf.c

   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_input_memory.c
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_input_file.c
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_input.c

   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_output_memory.c
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_output_file.c
   ${CMAKE_CURRENT_SOURCE_DIR}/cstream_output.c
)

add_library(cstream
   ${CSTREAM_HEADERS}
   ${CSTREAM_SOURCES}
)

set_target_properties(cstream PROPERTIES
   FRAMEWORK TRUE
   FRAMEWORK_VERSION C
   MACOSX_FRAMEWORK_IDENTIFIER com.duphus.cstream
   # MACOSX_FRAMEWORK_INFO_PLIST Info.plist
   # "current version" in semantic format in Mach-0 binary file
   VERSION 1.0.0
   # "compatibilty version" in semantic format in Mach-0 binary file
   SOVERSION 1.0.0
   PUBLIC_HEADER "${CSTREAM_HEADERS}"
   # XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "duphus"
)

target_compile_features(cstream PUBLIC c_std_99)


# compile with all warnings & treat them as errors
# this way compiling cstream into your project doesn't cause a bunch of nuance errors

if(MSVC)
  target_compile_options(cstream PRIVATE /W4 /WX)
else()
  target_compile_options(cstream PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set_property(TARGET cstream PROPERTY COMPILE_WARNING_AS_ERROR ON)

