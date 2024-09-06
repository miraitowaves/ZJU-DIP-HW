//
// Created by 方星皓 on 2023/10/11.
//

#ifndef DIGITAL_PROCESSING_HW01_BMP_FILE_H
#define DIGITAL_PROCESSING_HW01_BMP_FILE_H

#include <windows.h>

/*
 * 定义一个结构体来重新存储读取到的BMP文件的信息
 * 这个BMP_File只能处理真彩色图的数据
 */
typedef struct {
    BITMAPFILEHEADER bitmapfileheader; // 位图文件头
    BITMAPINFOHEADER bitmapinfoheader; // 文件信息头
    RGBQUAD **data;// 实际的位图数据
} BMP_File;

/*
 * 这个结构对应YUV的颜色空间
 *
 * 变量介绍
*/
typedef struct tagYUVQUAD {
    BYTE Y;
    BYTE U;
    BYTE V;
    BYTE yuvRerserved;
} YUVQUAD;

/*
 * 定义一个结构体来重新存储读取到的BMP文件的信息
 * 这个结构特指在YUV颜色空间下的BMP的文件数据
 */
typedef struct {
    BITMAPFILEHEADER bitmapfileheader; // 位图文件头
    BITMAPINFOHEADER bitmapinfoheader; // 文件信息头
    RGBQUAD rgbquad[256]; //  调色板，生成灰度图
    YUVQUAD **data;// 实际的位图数据
} BMP_FileInYUV;

typedef BMP_FileInYUV BinaryBMP_File;

#endif //DIGITAL_PROCESSING_HW01_BMP_FILE_H
