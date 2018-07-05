cmake_minimum_required(VERSION 3.8)
project(09_gniazda_UDP_z_wieloma_odbiorcami)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        err.c
        err.h
        multi-recv.c
        multi-recv6.c
        multi-send.c
        multi-send6.c)

add_executable(multi-recv err.c err.h multi-recv.c)
add_executable(multi-send err.c err.h multi-send.c)