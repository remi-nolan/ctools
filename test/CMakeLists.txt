cmake_minimum_required(VERSION 3.28)

add_executable(cstream_test cstream_test.c)
target_link_libraries(cstream_test PUBLIC cstream)
target_include_directories(cstream_test PUBLIC ${PROJECT_SOURCE_DIR}/cstream/)

