cmake_minimum_required(VERSION 3.8)
project(08_gniazda_programowanie_sterowane_zdarzeniami)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        client.c
        err.c
        err.h)

add_executable(client ${SOURCE_FILES})
add_executable(server  server.c err.c err.h)

target_link_libraries(server event)
target_link_libraries(client event)