cmake_minimum_required(VERSION 3.10)
project(big3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall )
set(ODBIORNIK odbiornik.cpp menu.cpp buffor.cpp)
set(NADAJNIK fifo.cpp set.cpp nadajnik.cpp)
set(SOURCE_FILES argument_parser.cpp err.cpp)

add_executable(odbiornik ${ODBIORNIK} ${SOURCE_FILES})
add_executable(nadajnik ${NADAJNIK} ${SOURCE_FILES})