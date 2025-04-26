cmake_minimum_required(VERSION 3.30)
project(untitled3 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled3 main.c
        bmp8.h
        bmp8.c
        bmp24.c
        bmp24.h)
