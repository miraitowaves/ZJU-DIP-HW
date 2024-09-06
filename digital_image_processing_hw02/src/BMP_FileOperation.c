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
        printf("we can not deal with the BMP file that has the biCount which is not 24 bits\n");
    }
    // 这个if用于检测真彩色图格式的正确性，是否因为一些不可抗力导致格式出现了错误，偏差值出现了问题
    if (bmpFilePtr->bitmapfileheader.bfOffBits != ftell(file)) {
        printf("Error occurs! The value of bfOffBits isn't the correct value\n");
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

    // 全部数据读取完毕，关闭文件
    fclose(file);
}

/*
 *  功能：实现从二值图像中读取数据
 *
 *  输入
 *  char *binaryBMP_Path
 *  BinaryBMP_File *binaryBmpFilePtr
 *
 */
void readBinaryBMP_File(char *binaryBMP_Path, BinaryBMP_File *binaryBmpFilePtr) {
    // 在给定路径上打开一个BMP文件, 这里输入的图片应当是二值图像
    FILE *file = fopen(binaryBMP_Path, "rb");
    // 读取位图文件头的数据
    fread(&binaryBmpFilePtr->bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file);

    // 读取位图信息头的数据
    fread(&binaryBmpFilePtr->bitmapinfoheader, sizeof(BITMAPINFOHEADER), 1, file);

    // 若读取的bmp文件不为真彩色图，那么就需要用到调色板
    // 反之，bitmapinfoheader后面跟的就是data，不需要进行调色板的处理
    // 下面的第一个if用于检测是否为灰度图（二值图像）
    if (binaryBmpFilePtr->bitmapinfoheader.biBitCount != 8) {
        printf("we can not deal with the BMP file that has the biCount which is not 8 bits\n");
    }

    // 读取调色板的数据
    for (int i = 0; i < 256; ++i) {
        fread(&binaryBmpFilePtr->rgbquad[i], sizeof(YUVQUAD), 1, file);
    }

    /*
     * 读取实际的位图数据
     * 给data分配好内存数据
     * 注意每行像素数据的字节数通常是4的倍数，所以要对数据进行额外的处理
     * 注意sizeof返回字节数
     */
    // 先给二维数组data分配内存
    binaryBmpFilePtr->data = (YUVQUAD**) malloc(sizeof(YUVQUAD*) * binaryBmpFilePtr->bitmapinfoheader.biHeight);
    for (int i = 0; i < binaryBmpFilePtr->bitmapinfoheader.biWidth; ++i) {
        binaryBmpFilePtr->data[i] = (YUVQUAD*) malloc(sizeof(YUVQUAD) * binaryBmpFilePtr->bitmapinfoheader.biWidth);
    }

    // 读取实际的位图数据
    for (int i = 0; i < binaryBmpFilePtr->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < binaryBmpFilePtr->bitmapinfoheader.biWidth; ++j) {
            fread(&binaryBmpFilePtr->data[i][j].Y, 1, 1, file);
        }

        //处理非为四的倍数的情况，补充位数
        int tmpRowBytes = binaryBmpFilePtr->bitmapinfoheader.biWidth;
        while (tmpRowBytes % 4 != 0) {
            fgetc(file);
            tmpRowBytes++;
        }
    }

    // 全部文件读取完毕，关闭文件
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
 * 功能：将灰度图转换为二值图像，存到新输入的指针当中
 *
 * 输入
 * bmpFileInYuvPtr: 输入的待转换的灰度图的指针
 * binaryBmpFilePtr：要转换成的二值图像的指针
 * threshold: 转换阈值
 */
void changeFromGrayToBinary(BMP_FileInYUV* bmpFileInYuvPtr, BinaryBMP_File* binaryBmpFilePtr, BYTE threshold) {
    // 修改位图文件头
    binaryBmpFilePtr->bitmapfileheader = bmpFileInYuvPtr->bitmapfileheader;
    // 修改文件信息头
    binaryBmpFilePtr->bitmapinfoheader = bmpFileInYuvPtr->bitmapinfoheader;

    // 存入调色板数据
    for (int i = 0; i < 256; ++i) {
        binaryBmpFilePtr->rgbquad[i] = bmpFileInYuvPtr->rgbquad[i];
    }

    // 输入实际的位图数据
    // 给data分配内存
    binaryBmpFilePtr->data = (YUVQUAD**) malloc(sizeof(YUVQUAD*) * bmpFileInYuvPtr->bitmapinfoheader.biHeight);
    for (int i = 0; i < binaryBmpFilePtr->bitmapinfoheader.biWidth; ++i) {
        binaryBmpFilePtr->data[i] = (YUVQUAD*) malloc(sizeof(YUVQUAD) * bmpFileInYuvPtr->bitmapinfoheader.biWidth);
    }
    // 写入数据
    for (int i = 0; i < bmpFileInYuvPtr->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFileInYuvPtr->bitmapinfoheader.biWidth; ++j) {
            binaryBmpFilePtr->data[i][j] = bmpFileInYuvPtr->data[i][j];

            // 根据阈值调整Y值
            if (binaryBmpFilePtr->data[i][j].Y < threshold) {
                binaryBmpFilePtr->data[i][j].Y = 0;
            } else {
                binaryBmpFilePtr->data[i][j].Y = 255;
            }
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
    // 创建文件头指针
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

/*
 * 功能：生成一个二值图像
 *
 * 二值图像是由灰度图转变而来的
 *
 * 输入
 * BMP_FileInYUV* bmpFileInYuvPtr
 * char* givenPath
 */
void generateBinaryBMP_File(BinaryBMP_File* binaryBmpFilePtr, char* givenPath) {
    generateGrayBMP_File(binaryBmpFilePtr, givenPath);
}

/*
 * 功能：对一个二值图像进行水平腐蚀操作
 *
 * 一个简单的水平腐蚀：一个中心点的左右两边均为0，那么就保留这个中心点为0
 * 否则变成0
 *
 * 输入
 * BinaryBMP_File* binaryBmpFilePtr：一个二值图像的指针
 */
void levelCorrodeBinaryBmpFile(BinaryBMP_File* binaryBmpFilePtr) {
    // 创建一个二级指针，用于记录对应的中心点是否需要被腐蚀
    int** ifCorrode;

    // 创建几个变量，方便之后使用
    LONG height = binaryBmpFilePtr->bitmapinfoheader.biHeight;
    LONG width = binaryBmpFilePtr->bitmapinfoheader.biWidth;

    // 给二级指针分配内存，最后再释放
    ifCorrode = (int**) malloc(sizeof(int*) * height);
    for (int i = 0; i < binaryBmpFilePtr->bitmapinfoheader.biWidth; ++i) {
        ifCorrode[i] = (int*) malloc(sizeof(int) * width);
    }

    // 给ifCorrode赋上初值
    // 为0表示不需要腐蚀
    // 为1表示需要腐蚀
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            ifCorrode[i][j] = 0;
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            BYTE tmpY = binaryBmpFilePtr->data[i][j].Y;

            if (tmpY == 0) { // 如果值为0(黑色)，就进行接下来的判断
                if (j - 1 >= 0) { // 这一条件表明这一中心点左边也存在点
                     BYTE tmpLeftY = binaryBmpFilePtr->data[i][j - 1].Y;

                     if (tmpLeftY == 255) {
                         ifCorrode[i][j] = 1; // 如果左边的值为0，就把这个中心点腐蚀
                     }
                }
                // 和上面同理，讨论右边的点的情况
                if (j + 1 <= width - 1) {
                    BYTE tmpRightY = binaryBmpFilePtr->data[i][j + 1].Y;

                    if (tmpRightY == 255) {
                        ifCorrode[i][j] = 1; // 如果右边的值为0，就把这个中心点腐蚀
                    }
                }
            }
        }
    }

    // 开始腐蚀！
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (ifCorrode[i][j] == 1) {
                binaryBmpFilePtr->data[i][j].Y = 255;
            }
        }
    }

    // 最后释放指针
    for (int i = 0; i < height; ++i) {
        free(ifCorrode[i]);
    }
    free(ifCorrode);
}

/*
 * 功能：对一个二值图像进行垂直膨胀操作
 *
 * 一个简单的垂直膨胀，如果一个中心点的上下两边有一边为0，那么这个中心点就设置为0
 * 否则设置为1
 *
 * 0：黑色 1：白色
 */
void verticalDilateBinaryBmpFile(BinaryBMP_File* binaryBmpFilePtr) {
    // 创建一个二级指针，用于记录对应的中心点是否需要被膨胀
    int** ifDilate;

    // 创建几个变量，方便之后使用
    LONG height = binaryBmpFilePtr->bitmapinfoheader.biHeight;
    LONG width = binaryBmpFilePtr->bitmapinfoheader.biWidth;

    // 给二级指针分配内存，最后再释放
    ifDilate = (int**) malloc(sizeof(int*) * height);
    for (int i = 0; i < binaryBmpFilePtr->bitmapinfoheader.biWidth; ++i) {
        ifDilate[i] = (int*) malloc(sizeof(int) * width);
    }

    // 给ifCorrode赋上初值
    // 为0表示不需要膨胀
    // 为1表示需要膨胀
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            ifDilate[i][j] = 0;
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            BYTE tmpY = binaryBmpFilePtr->data[i][j].Y;

            if (tmpY == 255) { // 如果值为255(白色)，就进行接下来的判断
                if (i - 1 >= 0) { // 这一条件表明这一中心点上边也存在点
                    BYTE tmpUpY = binaryBmpFilePtr->data[i - 1][j].Y;

                    if (tmpUpY == 0) {
                        ifDilate[i][j] = 1; // 如果上边的值为0，就把这个中心点膨胀
                    }
                }
                // 和上面同理，讨论右边的点的情况
                if (i + 1 <= height - 1) {
                    BYTE tmpDownY = binaryBmpFilePtr->data[i + 1][j].Y;

                    if (tmpDownY == 0) {
                        ifDilate[i][j] = 1; // 如果下边的值为0，就把这个中心点腐蚀
                    }
                }
            }
        }
    }

    // 开始膨胀！
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (ifDilate[i][j] == 1) {
                binaryBmpFilePtr->data[i][j].Y = 0;
            }
        }
    }

    // 最后释放指针
    for (int i = 0; i < height; ++i) {
        free(ifDilate[i]);
    }
    free(ifDilate);
}

/*
 * 功能：对一个二值图像进行开运算
 *
 * 开运算：先腐蚀后膨胀
 * 调用上面的函数
 *
 * 输入
 * int corrosionStrength:腐蚀次数，也就是调用水平腐蚀函数的次数，简单理解为腐蚀力度
 * int DilationStrength：膨胀力度
 */
void openingBinaryBmpFile(BinaryBMP_File *binaryBmpFilePtr, int corrosionStrength, int dilationStrength) {
    // 先腐蚀
    for (int i = 0; i < corrosionStrength; ++i) {
        levelCorrodeBinaryBmpFile(binaryBmpFilePtr);
    }

    //后水平
    for (int i = 0; i < dilationStrength; ++i) {
        verticalDilateBinaryBmpFile(binaryBmpFilePtr);
    }
}

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
void closingBinaryBmpFile(BinaryBMP_File *binaryBmpFilePtr, int dilationStrength, int corrosionStrength) {
    // 先膨胀
    for (int i = 0; i < dilationStrength; ++i) {
        verticalDilateBinaryBmpFile(binaryBmpFilePtr);
    }

    // 再腐蚀
    for (int i = 0; i < corrosionStrength; ++i) {
        levelCorrodeBinaryBmpFile(binaryBmpFilePtr);
    }
}

/*
 * 以下是private function，不留接口给外部
 *
 * 可以看到代码中重复的模块并不少，如果日后有时间，整合打包一下，进行优化
 */










