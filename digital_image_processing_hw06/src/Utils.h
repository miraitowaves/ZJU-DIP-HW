//
// Created by 方星皓 on 2023/11/26.
//
// 用于存放我自己创建的常用的函数库
//

#ifndef DIGITAL_IMAGE_PROCESSING_HW04_UTILS_H
#define DIGITAL_IMAGE_PROCESSING_HW04_UTILS_H

#include "BMP_File.h"
/*
 * 对数据范围进行约束
 * */
BYTE adjust(double data);

/*
 * 计算空间域下的欧式距离
 * */
double calcSpaceDistance(int h1, int w1, int h2, int w2);

/*
 * 计算RGB（三维向量域）下的欧式距离
 * */
double calcColorDistance(BMP_File* bmpFilePtr, int h1, int w1, int h2, int w2);


/*
 * 原理：双线性插值
 * 由原图中的邻近的四个像素点来计算新图中的一个点
 */
RGBQUAD bininear(BMP_File* bmpFilePtr, double origin_h, double origin_w);

/*
 * 卷积操作, 均值形式的卷积
 * 对给定的要获得的像素点，考虑它在原始图像中对应的核的大小和位置，进行卷积
 * bmpFilePtr: 要进行卷积的原始图像数据
 * h: 核中心高度
 * w: 核中心宽度
 * size: 核的大小
 * */
RGBQUAD meanConvolution(BMP_File* bmpFilePtr, int h, int w, int size);

/*
 * 卷积操作, 拉普拉斯算子下的卷积
 * 对给定的要获得的像素点，考虑它在原始图像中对应的核的大小和位置，进行卷积
 * bmpFilePtr: 要进行卷积的原始图像数据
 * h: 核中心高度
 * w: 核中心宽度
 * size: 固定为3，所以就不写出了
 * */
RGBQUAD lapConvolution(BMP_File* bmpFilePtr,int h, int w);
#endif //DIGITAL_IMAGE_PROCESSING_HW04_UTILS_H
