cmake_minimum_required(VERSION 3.25)
project(digital_image_processing_hw04 C)

set(CMAKE_C_STANDARD 11)

add_executable(digital_image_processing_hw04 src/main.c src/GeoTransform.c src/BMP_FileOperation.c src/Utils.c src/Utils.h)
