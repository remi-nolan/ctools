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

