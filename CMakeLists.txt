cmake_minimum_required(VERSION 3.16)
project(hashcons C)

set(CMAKE_C_STANDARD 99)

add_executable(hashcons main.c hashcons.h hashcons.c prime.h prime.c common.h test.c test.h)

target_link_libraries(hashcons m)