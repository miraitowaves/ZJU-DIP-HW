cmake_minimum_required(VERSION 3.25)
project(digital_image_processing_hw05 C)

set(CMAKE_C_STANDARD 11)

add_executable(digital_image_processing_hw05 src/main.c src/BMP_FileOperation.c  src/Utils.c src/Filter.c src/Filter.h src/Filter.h src/Filter.c src/Filter.c src/Laplace.c src/Laplace.h)
