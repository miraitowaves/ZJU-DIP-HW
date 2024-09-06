//
// Created by 方星皓 on 2023/12/8.
//

#ifndef DIGITAL_IMAGE_PROCESSING_HW05_LAPLACE_H
#define DIGITAL_IMAGE_PROCESSING_HW05_LAPLACE_H

#include "BMP_File.h"

/*
 * 对一个图像进行拉普拉斯图像增强操作
 * bmpFilePtr1：要处理的图像
 * bmpFilePtr2： 处理完后的图像
 * */
void lapEnhance(BMP_File* bmpFilePtr1, BMP_File* bmpFilePtr2);
#endif //DIGITAL_IMAGE_PROCESSING_HW05_LAPLACE_H
