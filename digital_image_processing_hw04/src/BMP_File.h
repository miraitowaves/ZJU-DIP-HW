//
// Created by 方星皓 on 2023/10/11.
//

#ifndef DIGITAL_PROCESSING_HW01_BMP_FILE_H
#define DIGITAL_PROCESSING_HW01_BMP_FILE_H

#include <windows.h>

#define maxHeight  5000
#define maxWidth 5000
#define pi 3.14159

/*
 * 定义一个结构体来重新存储读取到的BMP文件的信息
 * 这个BMP_File只能处理真彩色图的数据
 */
typedef struct {
    BITMAPFILEHEADER bitmapfileheader; // 位图文件头
    BITMAPINFOHEADER bitmapinfoheader; // 文件信息头
    RGBQUAD data[maxHeight][maxWidth];// 实际的位图数据

    //以下是自己设置的字段，方便自己进行数据处理
} BMP_File;



#endif //DIGITAL_PROCESSING_HW01_BMP_FILE_H
