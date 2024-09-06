//
// Created by 方星皓 on 2023/10/11.
//

#ifndef DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H
#define DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H

#include "windows.h"
#include "BMP_File.h"

/*
 * 下面是和BMP文件相关的操作函数
 *
 * 2023/11/10 忽然发现自己之前的思路错误了（加入了调色板的机制和YUV空间的图像信息，实际上不需要这么复杂）
 * 这下好了，之前做的要重新做了呜呜呜
 */

/*
 * 功能：实现从给定的BMP文件中读取数据
 *
 * 输入
 * char* BMP_Path: 需要读取的文件的路径
 * BMP_File* bmpFilePtr: 指向BMP_File类型变量的指针
 */
void readBMP_File(char *BMP_Path, BMP_File *bmpFilePtr);

/*
 * 更改彩色图数据为灰度图模式下的数据
 */
void changeToGray(BMP_File* bmpFilePtr);

/*
 * 更改灰度图模式下的数据为彩色图模式下的数据
 */
void changeToColor(BMP_File* bmpFilePtr);

/*
 * 采用对数操作增强图像对比度
 */
void visibilityEnhance(BMP_File *bmpFilePtr);

/*
 * 对一个BMP进行直方图均衡化
 */
void histogramEqualization(BMP_File *bmpFilePtr);


/*
 * 功能：生成一个BMP图像，根据不同数据模式下的BMP生成不同的图像
 *
 * 输入
 * BMP_File* bmpFilePtr: 一个文件指针，数据的来源
 * char* givenPath：想要生成的图像的位置
 */
void generateBMP_File(BMP_File* bmpFilePtr, char* givenPath);













#endif //DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H
