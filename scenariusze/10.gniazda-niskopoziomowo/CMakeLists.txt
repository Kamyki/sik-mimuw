cmake_minimum_required(VERSION 3.8)
project(10_gniazda_niskopoziomowo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        dropnobody.c
        err.c
        err.h
        in_cksum.c
        rawping.c
        )

add_executable(rawping ${SOURCE_FILES})

add_executable(spoofping spoofping.c dropnobody.c
        err.c
        err.h
        in_cksum.c)