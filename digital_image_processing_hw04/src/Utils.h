//
// Created by 方星皓 on 2023/11/26.
//
// 用于存放我自己创建的常用的函数库
//

#ifndef DIGITAL_IMAGE_PROCESSING_HW04_UTILS_H
#define DIGITAL_IMAGE_PROCESSING_HW04_UTILS_H

#include "BMP_File.h"

/*
 * 原理：双线性插值
 * 由原图中的邻近的四个像素点来计算新图中的一个点
 */
RGBQUAD bininear(BMP_File* bmpFilePtr, double origin_h, double origin_w);

#endif //DIGITAL_IMAGE_PROCESSING_HW04_UTILS_H
