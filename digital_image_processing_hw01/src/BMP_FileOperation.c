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
        printf("we can not deal with the BMP file that has the biCount which is not 24 bits");
    }
    // 这个if用于检测真彩色图格式的正确性，是否因为一些不可抗力导致格式出现了错误，偏差值出现了问题
    if (bmpFilePtr->bitmapfileheader.bfOffBits != ftell(file)) {
        printf("Error occurs! The value of bfOffBits isn't the correct value");
    }

    /* 读取实际的位图数据
     * 因为是真彩色图，所以可以直接从bitmapinfoheader后面开始读取了
     * 给data分配好内存数据
     * 注意每行像素数据的字节数通常是4的倍数，所以要对数据进行额外的处理
     * 注意sizeof返回字节数
     */
    // 先给二维数组data分配内存
    bmpFilePtr->data = (RGBQUAD**) malloc(sizeof(RGBQUAD*) * bmpFilePtr->bitmapinfoheader.biHeight);
    for (int i = 0; i < bmpFilePtr->bitmapinfoheader.biWidth; ++i) {
        bmpFilePtr->data[i] = (RGBQUAD*) malloc(sizeof(RGBQUAD) * bmpFilePtr->bitmapinfoheader.biWidth);
    }

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

    // 检测是否正确读到了文件末尾
    if (bmpFilePtr->bitmapfileheader.bfSize == ftell(file)) {
        printf("You are successfully read the data in this BMP file!");
    } else {
        printf("There are some errors in reading the data!");
    }

    fclose(file);
}

/*
 * 功能：实现从RGB颜色空间图像到YUV颜色空间图像的转变
 *
 * 输入
 * BMP_File* bmpFilePtr: 一个文件指针，传入数据
 * BMP_FileInYUV* bmpFileInYuvPtr: 一个文件指针，接收数据
 */
void changeBMP_FileFromRGB_StandardToYUV_Standard(BMP_File* bmpFilePtr, BMP_FileInYUV* bmpFileInYuvPtr) {
    // 读取RBG空间下BMP图像的位图文件头
    // 有一部分值并不匹配，有一些之后在生成灰度图的时候修改，如bftype等
    bmpFileInYuvPtr->bitmapfileheader = bmpFilePtr->bitmapfileheader;

    // 读取RBG空间下BMP图像的位图信息头
    bmpFileInYuvPtr->bitmapinfoheader = bmpFilePtr->bitmapinfoheader;

    // 修改位图数据与文件开始位置的偏移值
    bmpFileInYuvPtr->bitmapfileheader.bfOffBits = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
    // 修改bfSize，指定文件大小
    bmpFileInYuvPtr->bitmapfileheader.bfSize = bmpFileInYuvPtr->bitmapfileheader.bfOffBits + (bmpFileInYuvPtr->bitmapinfoheader.biWidth + 3 / 4 * 4) * bmpFileInYuvPtr->bitmapinfoheader.biHeight;
    // 修改表示颜色需要用到的颜色数, biBitCount
    bmpFileInYuvPtr->bitmapinfoheader.biBitCount = 8;

    // 输入调色板信息
    for (int i = 0; i < 256; ++i) { // 256是8位对应的颜色个数
        bmpFileInYuvPtr->rgbquad[i].rgbBlue = i;
        bmpFileInYuvPtr->rgbquad[i].rgbGreen = i;
        bmpFileInYuvPtr->rgbquad[i].rgbRed = i;
        bmpFileInYuvPtr->rgbquad[i].rgbReserved = 0;
    }

    // 输入实际的位图数据
    // 分配内存
    bmpFileInYuvPtr->data = (YUVQUAD**) malloc(sizeof(YUVQUAD*) * bmpFileInYuvPtr->bitmapinfoheader.biHeight);
    for (int i = 0; i < bmpFileInYuvPtr->bitmapinfoheader.biWidth; ++i) {
        bmpFileInYuvPtr->data[i] = (YUVQUAD*) malloc(sizeof(YUVQUAD) * bmpFileInYuvPtr->bitmapinfoheader.biWidth);
    }

    // 输入实际的位图数据
    // RGB空间到YUV空间的转变, 我们采用的是“ITU-R BT.601”的标准
    // maxY用于之后对Y值的处理
    double maxY;
    for (int i = 0; i < bmpFileInYuvPtr->bitmapinfoheader.biHeight; ++i) {
        // 注意在文件中数据是以“BGR”的顺序存储
        for (int j = 0; j < bmpFileInYuvPtr->bitmapinfoheader.biWidth; ++j) {
            double tmpY;
            tmpY = 0.299 * bmpFilePtr->data[i][j].rgbRed + 0.587 * bmpFilePtr->data[i][j].rgbGreen + 0.114 * bmpFilePtr->data[i][j].rgbBlue;
            // 在i = 0且j = 0的时候对minY和maxY进行初始化
            if (i == 0 && j == 0) {
                maxY = tmpY;
            }

            //更新maxY和minY的值
            if (tmpY > maxY) {
                maxY = tmpY;
            }
        }
    }


    for (int i = 0; i < bmpFileInYuvPtr->bitmapinfoheader.biHeight; ++i) {
        // 注意在文件中数据是以“BGR”的顺序存储
        for (int j = 0; j < bmpFileInYuvPtr->bitmapinfoheader.biWidth; ++j) {
            bmpFileInYuvPtr->data[i][j].Y = (255 / maxY) * (0.3 * bmpFilePtr->data[i][j].rgbRed + 0.59 * bmpFilePtr->data[i][j].rgbGreen + 0.11 * bmpFilePtr->data[i][j].rgbBlue);
            bmpFileInYuvPtr->data[i][j].U = -0.147 * bmpFilePtr->data[i][j].rgbRed - 0.289 * bmpFilePtr->data[i][j].rgbGreen + 0.436 * bmpFilePtr->data[i][j].rgbBlue;
            bmpFileInYuvPtr->data[i][j].V = 0.615 * bmpFilePtr->data[i][j].rgbRed - 0.515 * bmpFilePtr->data[i][j].rgbGreen - 0.100 * bmpFilePtr->data[i][j].rgbBlue;
        }
    }

}

/*
 * 功能：实现从YUV颜色空间到RBG颜色空间的转变
 *
 * 输入
 * BMP_FileInYUV* bmpFileInYuvPtr: 一个文件指针，输入数据
 * BMP_File* bmpFilePtr: 一个文件指针，接收数据
 */
void changeBMP_FileFromYUV_StandardToRGB_Standard(BMP_FileInYUV* bmpFileInYuvPtr, BMP_File* bmpFilePtr) {
    // 修改位图文件头
    bmpFilePtr->bitmapfileheader = bmpFileInYuvPtr->bitmapfileheader;
    // 修改文件信息头
    bmpFilePtr->bitmapinfoheader = bmpFileInYuvPtr->bitmapinfoheader;
    // 修改各个信息
    bmpFilePtr->bitmapfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFilePtr->bitmapfileheader.bfSize = bmpFilePtr->bitmapfileheader.bfOffBits + ((bmpFilePtr->bitmapinfoheader.biWidth * 3 + 3) / 4 * 4 * bmpFilePtr->bitmapinfoheader.biHeight);
    bmpFilePtr->bitmapinfoheader.biBitCount = 24; // 彩色图像！！！



    // 输入实际的位图数据
    // 给data分配内存
    bmpFilePtr->data = (RGBQUAD**) malloc(sizeof(RGBQUAD*) * bmpFilePtr->bitmapinfoheader.biHeight);
    for (int i = 0; i < bmpFilePtr->bitmapinfoheader.biWidth; ++i) {
        bmpFilePtr->data[i] = (RGBQUAD*) malloc(sizeof(RGBQUAD) * bmpFilePtr->bitmapinfoheader.biWidth);
    }
    // 写入数据
    for (int i = 0; i < bmpFilePtr->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFilePtr->bitmapinfoheader.biWidth; ++j) {
            bmpFilePtr->data[i][j].rgbRed = 0.100 * bmpFileInYuvPtr->data[i][j].Y + 1.140 * bmpFileInYuvPtr->data[i][j].V;
            bmpFilePtr->data[i][j].rgbGreen = 1.000 * bmpFileInYuvPtr->data[i][j].Y - 0.395 * bmpFileInYuvPtr->data[i][j].U - 0.580 * bmpFileInYuvPtr->data[i][j].V;
            bmpFilePtr->data[i][j].rgbBlue = 1.000 * bmpFileInYuvPtr->data[i][j].Y + 2.036 * bmpFileInYuvPtr->data[i][j].U;
        }
    }
}


/*
 * 功能：生成一个彩色图像
 *
 * 输入
 * BMP_File* bmpFilePtr: 一个文件指针
 * char* givenPath：想要生成的图像的位置
 * 输出
 * 在指定文件路径上生成一个彩色图像
 */
void generateColorBMP_File(BMP_File* bmpFilePtr, char* givenPath) {
    // 创建一个写入文件
    FILE *file = fopen(givenPath, "wb"); // tm的是“wb”不是“w”啊，我是老坛
    // 写入位图文件头
    fwrite(&bmpFilePtr->bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file);
    // 写入位图信息头
    fwrite(&bmpFilePtr->bitmapinfoheader, sizeof(BITMAPINFOHEADER), 1, file);
    if (bmpFilePtr->bitmapinfoheader.biBitCount != 24) {
        printf("we can not deal with the BMP file that has the biCount which is not 24 bits");
    }
    // 这个if用于检测真彩色图格式的正确性，是否因为一些不可抗力导致格式出现了错误，偏差值出现了问题
    if (bmpFilePtr->bitmapfileheader.bfOffBits != ftell(file)) {
        printf("Error occurs! The value of bfOffBits isn't correct value");
    }


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

    // 检测是否正确读到了文件末尾
    if (bmpFilePtr->bitmapfileheader.bfSize == ftell(file)) {
        printf("You are successfully read the data in this BMP file!");
    } else {
        printf("There are some errors in reading the data!");
    }

    //全部数据已经写入，关闭文件
    fclose(file);
}

/*
 * 功能：生成一个灰色图像
 *
 * 输入
 * BMP_FileInYUV* bmpFileInYuvPtr：YUV空间下的BMP图像的指针，数据的来源
 * char* givenPath：想要生成的图像的位置
 */
void generateGrayBMP_File(BMP_FileInYUV* bmpFileInYuvPtr, char* givenPath) {
    // 创建文件职指针
    FILE* file = fopen(givenPath, "wb");
    // 写入位图文件头
    fwrite(&bmpFileInYuvPtr->bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file);
    // 写入文件信息头
    fwrite(&bmpFileInYuvPtr->bitmapinfoheader, sizeof(BITMAPINFOHEADER), 1, file);
    // 写入调色板, 对于灰度图是256色
    for (int i = 0; i < 256; ++i) {
        fwrite(&bmpFileInYuvPtr->rgbquad[i], sizeof(RGBQUAD), 1, file);
    }
    // 写入实际的位图数据
    for (int i = 0; i < bmpFileInYuvPtr->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFileInYuvPtr->bitmapinfoheader.biWidth; ++j) {
            fwrite(&bmpFileInYuvPtr->data[i][j].Y, sizeof(BYTE), 1, file);
        }

        // 处理非4倍数的情况
        int tmpRowByteSize = bmpFileInYuvPtr->bitmapinfoheader.biWidth;
        while (tmpRowByteSize % 4 != 0) {
            fputc('0', file);
            tmpRowByteSize++;
        }
    }

    // 写入完毕，关闭文件
    fclose(file);
}









