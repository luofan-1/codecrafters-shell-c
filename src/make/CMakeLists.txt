cmake_minimum_required(VERSION 3.0)

project(codecrafter-shell C)

set(CMAKE_C_STANDARD 99) # Enable the C23 standard

set(SRC_DIR ${CMAKE_SOURCE_DIR}/..)

set(HEADS_DIR ${CMAKE_SOURCE_DIR}/../includes/*.h)

file(GLOB SOURCE_FILES ${SRC_DIR}/*.c ${SRC_DIR}/builtins/*.c ${HEADS_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(myshell ${SOURCE_FILES})

target_link_libraries(myshell PRIVATE readline)
