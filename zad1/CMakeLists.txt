cmake_minimum_required(VERSION 3.8)
project(zad1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp err.cc tcpConnection.cpp action.cpp menu.cpp state.cpp telnet.cpp)
add_executable(zad1 ${SOURCE_FILES})