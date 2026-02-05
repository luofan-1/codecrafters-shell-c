cmake_minimum_required(VERSION 3.0)

project(codecrafter-shell C)

set(CMAKE_C_STANDARD 99) # Enable the C23 standard

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/../*.c ${CMAKE_SOURCE_DIR}/../*.h)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(myshell ${SOURCE_FILES})

target_link_libraries(myshell PRIVATE readline)
