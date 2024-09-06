//
// Created by 方星皓 on 2023/12/22.
//

#ifndef DIGITAL_IMAGE_PROCESSING_HW06_BILATERALFILTER_H
#define DIGITAL_IMAGE_PROCESSING_HW06_BILATERALFILTER_H
#include "BMP_File.h"

/*
 * 功能：实现双边滤波
 * sigmaS: 空间域的σ大小
 * sigmaC: 色彩空间(RGB)的σ大小
 * size: 卷积核大小
 * */
void BilateralFilter(BMP_File *bmpFilePtr1, BMP_File *bmpFilePtr2, double sigmaS, double sigmaC, int size);

/*
 * 对双边滤波下的每一个像素点进行滤波操作
 * */
RGBQUAD calcPixelByBil(BMP_File * bmpFilePtr, int h, int w, double sigmaS, double sigmaC, int size);
#endif //DIGITAL_IMAGE_PROCESSING_HW06_BILATERALFILTER_H
