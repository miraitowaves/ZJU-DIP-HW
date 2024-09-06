cmake_minimum_required(VERSION 3.25)
project(digital_processing_hw01 C)

set(CMAKE_C_STANDARD 11)

add_executable(digital_processing_hw01 src/main.c src/BMP_FileOperation.c src/BMP_FileOperation.h src/BMP_File.h)
