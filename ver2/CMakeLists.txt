cmake_minimum_required(VERSION 3.8)
project(ver2)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cc tcpConnection.cc)
add_executable(ver2 ${SOURCE_FILES})