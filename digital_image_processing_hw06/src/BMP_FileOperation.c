//
// Created by 方星皓 on 2023/10/11.
//

/*
 * 这里是头文件包含模块
 *
 * 对于window.h头文件，它包含了位图文件头，位图信息头，调色板这三个结构的定义，不需要我们再作定义了！
 */
#include "BMP_File.h"
#include "BMP_FileOperation.h"
#include <windows.h>
#include <stdio.h>
#include "stdlib.h"

/*
 * 功能：实现从给定的BMP文件中读取数据
 *
 * 输入
 * char* BMP_Path: 需要读取的文件的路径
 * BMP_File* bmpFilePtr: 指向BMP_File类型变量的指针
 */
void readBMP_File(char *BMP_Path, BMP_File *bmpFilePtr) {
    // 在给定路径上打开一个BMP文件（需要是真彩色图）
    FILE *file = fopen(BMP_Path, "rb");
    // 读取位图文件头的数据
    fread(&bmpFilePtr->bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file);

    // 读取位图信息头的数据
    fread(&bmpFilePtr->bitmapinfoheader, sizeof(BITMAPINFOHEADER), 1, file);

    // 若读取的bmp文件不为真彩色图，那么就需要用到调色板
    // 反之，bitmapinfoheader后面跟的就是data，不需要进行调色板的处理
    // 但本次作业不需要处理真彩色图之外的其他bmp格式的图像文件
    // 下面的第一个if用于检测是否为真彩色图
    if (bmpFilePtr->bitmapinfoheader.biBitCount != 24) {
        printf("we can not deal with the BMP file that has the biCount which is not 24 bits \n");
    }
    // 这个if用于检测真彩色图格式的正确性，是否因为一些不可抗力导致格式出现了错误，偏差值出现了问题
    if (bmpFilePtr->bitmapfileheader.bfOffBits != ftell(file)) {
        printf("Error occurs! The value of bfOffBits isn't the correct value \n");
    }

    /* 读取实际的位图数据
     * 因为是真彩色图，所以可以直接从bitmapinfoheader后面开始读取了
     * 注意每行像素数据的字节数通常是4的倍数，所以要对数据进行额外的处理
     * 注意sizeof返回字节数
     */

    // 读取实际的位图数据
    for (int i = 0; i < bmpFilePtr->bitmapinfoheader.biHeight; ++i) {
        // 存储RGB的颜色数据到data中
        // 注意在文件中数据是以“BGR”的顺序存储
        for (int j = 0; j < bmpFilePtr->bitmapinfoheader.biWidth; ++j) {
            fread(&bmpFilePtr->data[i][j].rgbBlue, 1, 1, file);
            fread(&bmpFilePtr->data[i][j].rgbGreen, 1, 1, file);
            fread(&bmpFilePtr->data[i][j].rgbRed, 1, 1, file);
        }

        //处理非为四的倍数的情况，补充位数
        int tmpRowBytes = bmpFilePtr->bitmapinfoheader.biWidth * 3;
        while (tmpRowBytes % 4 != 0) {
            fgetc(file);
            tmpRowBytes++;
        }
    }

    // 全部数据读取完毕，关闭文件
    fclose(file);
}

BMP_File* initialBmpFilePtr() {
    BMP_File *bmpFilePtr = malloc(sizeof(BMP_File));
    readBMP_File("../resource/Ushio.bmp", bmpFilePtr);

    for (int i = 0; i < maxHeight; ++i) {
        for (int j = 0; j < maxWidth; ++j) {
            bmpFilePtr->data[i][j].rgbGreen = 0;
            bmpFilePtr->data[i][j].rgbBlue = 0;
            bmpFilePtr->data[i][j].rgbRed = 0;
            bmpFilePtr->data[i][j].rgbReserved = 0;
        }
    }

    return bmpFilePtr;
}

/*
 * 功能：生成一个BMP图像，根据不同数据模式下的BMP的数据生成不同的图像
 *
 * 输入
 * BMP_File* bmpFilePtr: 一个文件指针
 * char* givenPath：想要生成的图像的位置
 * 输出
 * 在指定文件路径上生成一个彩色图像
 */
void generateBMP_File(BMP_File* bmpFilePtr, char* givenPath) {
    // 创建一个写入文件
    FILE *file = fopen(givenPath, "wb"); // tm的是“wb”不是“w”啊，我是老坛
    // 写入位图文件头
    fwrite(&bmpFilePtr->bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file);
    // 写入位图信息头
    fwrite(&bmpFilePtr->bitmapinfoheader, sizeof(BITMAPINFOHEADER), 1, file);

    // 写入实际的位图数据
    for (int i = 0; i < bmpFilePtr->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFilePtr->bitmapinfoheader.biWidth; ++j) {
            fwrite(&bmpFilePtr->data[i][j].rgbBlue, 1, 1, file);
            fwrite(&bmpFilePtr->data[i][j].rgbGreen, 1, 1, file);
            fwrite(&bmpFilePtr->data[i][j].rgbRed, 1, 1, file);
        }

        int tmpCurrentRowByteSize = bmpFilePtr->bitmapinfoheader.biWidth * 3;
        while (tmpCurrentRowByteSize % 4 != 0) {
            fputc('0', file);
            tmpCurrentRowByteSize++;
        }
    }

    //全部数据已经写入，关闭文件
    fclose(file);
}












