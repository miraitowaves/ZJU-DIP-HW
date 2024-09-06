//
// Created by 方星皓 on 2023/11/24.
//

#ifndef DIGITAL_IMAGE_PROCESSING_HW04_GEOTRANSFORM_H
#define DIGITAL_IMAGE_PROCESSING_HW04_GEOTRANSFORM_H

#include "BMP_File.h"

/*
 * 功能：实现图像的平移操作
 *
 * 输入：
 * BMP_File *bmpFilePtr1, 输入原始的BMP图像指针
 * BMP_File *bmpFilePtr2, 输出的新的图像指针
 * LONG toRight, 向右偏移的像素长度（以像素点为单位），正值代表向右，负值代表向左
 * LONG toUp, 向上偏移的像素长度，与toRight同理
 */
void translation(BMP_File *bmpFilePtr1, BMP_File* bmpFilePtr2, LONG toRight, LONG toUp);

/*
 * 功能：实现图像的镜像操作
 *
 * 输入：
 * BMP_File *bmpFilePtr1, 输入的BMP图像指针
 * BMP_File bmpFilePtr2, 接收输出信息的指针
 * char axis: 选择翻转的基准轴（x轴 or y轴），输入的时候注意大写和使用‘’符号！
 */
void mirror(BMP_File *bmpFilePtr1, BMP_File* bmpFilePtr2, char axis);

/*
 * 功能：实现图像的旋转操作
 *
 * 输入：
 * double angle：旋转的角度，逆时针顺序
 */
void rotation(BMP_File *bmpFilePtr1, BMP_File *bmpFilePtr2, double theta);

/*
 * 功能：实现图像的缩放操作
 *
 * 输入：
 * double heightScaling，高度的缩放系数
 * double widthScaling，宽度的缩放系数
 */
void scale(BMP_File *bmpFilePtr1, BMP_File *bmpFilePtr2, double heightScale, double widthScale);

/*
 * 功能：实现图像的错切操作
 *
 * 输入：
 * char axis：选择错切X轴还是错切Y轴
 * double misalignCoefficient，错切系数
 */
void shear(BMP_File* bmpFilePtr1, BMP_File* bmpFilePtr2, char axis, double misalignCoeddicient);

#endif //DIGITAL_IMAGE_PROCESSING_HW04_GEOTRANSFORM_H
