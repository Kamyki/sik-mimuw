cmake_minimum_required(VERSION 3.8)
project(big)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(ODBIORNIK odbiornik.cpp menu.c buffor.cpp)
set(NADAJNIK nadajnik.c fifo.c set.c)

set(SOURCE_FILES argument_parser.c err.c)

add_executable(odbiornik ${ODBIORNIK} ${SOURCE_FILES})
add_executable(nadajnik ${NADAJNIK} ${SOURCE_FILES})
add_executable(tescik test.c)