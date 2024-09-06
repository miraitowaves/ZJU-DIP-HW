//
// Created by 方星皓 on 2023/10/11.
//

#ifndef DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H
#define DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H

#include "windows.h"
#include "BMP_File.h"

/*
 * 下面是和BMP文件相关的操作函数
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
 * 功能：实现从RGB颜色空间图像到YUV颜色空间图像的转变
 *
 * 输入
 * BMP_File* bmpFilePtr: 一个文件指针，传入数据
 * BMP_FileInYUV* bmpFileInYuvPtr: 一个文件指针，接收数据
 */
void changeBMP_FileFromRGB_StandardToYUV_Standard(BMP_File* bmpFilePtr, BMP_FileInYUV* bmpFileInYuvPtr);

/*
 * 功能：实现从YUV颜色空间到RGB颜色空间的转变
 *
 *
 * 输入
 * BMP_FileInYUV* bmpFileInYuvPtr: 一个文件指针，输入数据
 * BMP_File* bmpFilePtr: 一个文件指针，接收数据
 */
void changeBMP_FileFromYUV_StandardToRGB_Standard(BMP_FileInYUV* bmpFileInYuvPtr, BMP_File* bmpFilePtr);

/*
 * 功能：生成一个彩色图像
 *
 * 输入
 * BMP_File* bmpFilePtr: 一个文件指针，数据的来源
 * char* givenPath：想要生成的图像的位置
 */
void generateColorBMP_File(BMP_File* bmpFilePtr, char* givenPath);

/*
 * 功能：生成一个灰色图像
 *
 * 输入
 * BMP_FileInYUV* bmpFileInYuvPtr：YUV空间下的BMP图像的指针，数据的来源
 * char* givenPath：想要生成的图像的位置
 */
void generateGrayBMP_File(BMP_FileInYUV* bmpFileInYuvPtr, char* givenPath);

#endif //DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H
