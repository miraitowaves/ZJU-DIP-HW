cmake_minimum_required(VERSION 3.25)
project(digital_image_processing_hw03 C)

set(CMAKE_C_STANDARD 11)

add_executable(digital_image_processing_hw03 src/main.c src/BMP_File.h src/BMP_FileOperation.h src/BMP_FileOperation.c)
