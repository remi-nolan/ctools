cmake_minimum_required(VERSION 3.28)

# this is for my own personal sake, for Vim/YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ctools)

add_subdirectory(cstream)

add_subdirectory(test)

install(
   TARGETS cstream cstream_test
   RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/out/executable/
   LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/out/shared/
   ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/out/static/
   FRAMEWORK DESTINATION ${CMAKE_SOURCE_DIR}/out/framework/
   PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/out/include/
)

