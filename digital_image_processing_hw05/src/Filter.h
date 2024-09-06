//
// Created by 方星皓 on 2023/12/8.
//

#ifndef DIGITAL_IMAGE_PROCESSING_HW05_FILTER_H
#define DIGITAL_IMAGE_PROCESSING_HW05_FILTER_H

#include "BMP_File.h"

/*
 * 实现均值滤波的函数
 *
 * bmpFilePtr1: 顾名思义，是我们要处理的图像的指针
 * bmpFilePtr2: 处理完后，存储数据的新的指针
 * size: 我们处理的核的大小(size * size)
 * */
void meanFiltering(BMP_File* bmpFilePtr1,BMP_File* bmpFilePtr2, int size);

#endif //DIGITAL_IMAGE_PROCESSING_HW05_FILTER_H

