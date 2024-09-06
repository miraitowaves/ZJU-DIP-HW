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
 *  功能：实现从二值图像中读取数据
 *
 *  输入
 *  char *binaryBMP_Path
 *  BinaryBMP_File *binaryBmpFilePtr
 *
 */
void readBinaryBMP_File(char *binaryBMP_Path, BinaryBMP_File *binaryBmpFilePtr);

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
 * 功能：将灰度图转换为二值图像，存到新输入的指针当中
 *
 * 输入
 * bmpFileInYuvPtr: 输入的待转换的灰度图的指针
 * binaryBmpFilePtr：要转换成的二值图像的指针
 * threshold：转换阈值
 */
void changeFromGrayToBinary(BMP_FileInYUV* bmpFileInYuvPtr, BinaryBMP_File* binaryBmpFilePtr, BYTE threshold);

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

/*
 * 功能：生成一个二值图像，由YUV空间下的灰度图转换而来
 *
 * 二值图像是由灰度图转变而来的
 *
 * 输入
 * BMP_FileInYUV* bmpFileInYuvPtr
 * char* givenPath
 */
void generateBinaryBMP_File(BMP_FileInYUV* bmpFileInYuvPtr, char* givenPath);

/*
 * 功能：对一个二值图像进行水平腐蚀操作
 *
 * 一个简单的水平腐蚀：一个中心点的左右两边均为0，那么就保留这个中心点为0
 * 否则变成0
 *
 * 输入
 * BinaryBMP_File* binaryBmpFilePtr：一个二值图像的指针
 */
void levelCorrodeBinaryBmpFile(BinaryBMP_File* binaryBmpFilePtr);

/*
 * 功能：对一个二值图像进行垂直膨胀操作
 *
 * 一个简单的垂直膨胀，如果一个中心点的上下两边有一边为0，那么这个中心点就设置为0
 * 否则设置为1
 *
 * 0：黑色 1：白色
 */
void verticalDilateBinaryBmpFile(BinaryBMP_File* binaryBmpFilePtr);

/*
 * 功能：对一个二值图像进行开运算
 *
 * 开运算：先腐蚀后膨胀
 * 调用上面的函数
 *
 * 输入
 * BinaryBMP_File *binaryBmpFilePtr: 要操作的二值图像的指针
 * int corrosionStrength:腐蚀次数，也就是调用水平腐蚀函数的次数，简单理解为腐蚀力度
 * int DilationStrength：膨胀力度
 */
void openingBinaryBmpFile(BinaryBMP_File *binaryBmpFilePtr, int corrosionStrength, int dilationStrength);

/*
 * 功能：对一个二值图像进行闭运算
 *
 * 开运算：先膨胀后腐蚀
 * 调用上面的函数
 *
 * 输入
 * （注意跟上面的函数，虽然是对偶的，但参数的顺序换了一下，不要输入错了，导致得不到想要的结果111）
 * BinaryBMP_File *binaryBmpFilePtr
 * int DilationStrength：膨胀力度
 * int corrosionStrength:腐蚀次数，也就是调用水平腐蚀函数的次数，简单理解为腐蚀力度
 *
 */
void closingBinaryBmpFile(BinaryBMP_File *binaryBmpFilePtr, int dilationStrength, int corrosionStrength);
#endif //DIGITAL_PROCESSING_HW01_BMP_FILEOPERATION_H
