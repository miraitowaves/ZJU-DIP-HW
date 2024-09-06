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
#include <math.h>

// private function函数的声明，并没有在.h中写出，不要调用它
//-------------------
double* histogram(BMP_File *bmpFilePtr);
//-------------------


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
 * 更改数据为灰度图模式下的数据
 */
void changeToGray(BMP_File* bmpFilePtr) {
    // 设定以下我们可能会用到的量
    LONG height = bmpFilePtr->bitmapinfoheader.biHeight; // 高度
    LONG width = bmpFilePtr->bitmapinfoheader.biWidth; // 宽度

    // 对每个像素点的值进行操作
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // RGB到YUV的数据转变，注意这里一定要是double，不要是BYTE类型，否则会在传递数据的过程中出现错误！
            double Y = 0.299 * bmpFilePtr->data[i][j].rgbRed + 0.587 * bmpFilePtr->data[i][j].rgbGreen + 0.114 * bmpFilePtr->data[i][j].rgbBlue;
            double U = -0.147 * bmpFilePtr->data[i][j].rgbRed - 0.289 * bmpFilePtr->data[i][j].rgbGreen + 0.436 * bmpFilePtr->data[i][j].rgbBlue;
            double V = 0.615 * bmpFilePtr->data[i][j].rgbRed - 0.515 * bmpFilePtr->data[i][j].rgbGreen - 0.100 * bmpFilePtr->data[i][j].rgbBlue;

            // 将YUV下的数据重新生成
            // 虽然没有调色板，但可以通过让r = g = b的方式生成灰度图
            // 首先对Y的值进行范围判断约束
            if (Y >255) {
                Y = 255;
            } else if (Y < 0) {
                Y = 0;
            }
            // 对rgb的值进行更改
            bmpFilePtr->data[i][j].rgbRed = Y;
            bmpFilePtr->data[i][j].rgbGreen = Y;
            bmpFilePtr->data[i][j].rgbBlue = Y;
        }
    }
}

/*
 * 采用对数操作增强图像对比度
 */
void visibilityEnhance(BMP_File *bmpFilePtr) {
    // 设定以下我们可能会用到的量
    LONG height = bmpFilePtr->bitmapinfoheader.biHeight;
    LONG width = bmpFilePtr->bitmapinfoheader.biWidth;
    double maxY = 0.00; // 用来存储我们的最大亮度值

    // 先找到最大亮度值
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // RGB到YUV的数据转变，注意这里一定要是double，不要是BYTE类型，否则会在传递数据的过程中出现错误！
            double Y = 0.299 * bmpFilePtr->data[i][j].rgbRed + 0.587 * bmpFilePtr->data[i][j].rgbGreen + 0.114 * bmpFilePtr->data[i][j].rgbBlue;
            // 如果当前像素的亮度大于maxY
            if (Y > maxY) {
                maxY = Y;
            }
        }
    }

    // 进行图像增强操作，主要使用对数操作思想
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // RGB到YUV的数据转变，注意这里一定要是double，不要是BYTE类型，否则会在传递数据的过程中出现错误！
            double Y = 0.299 * bmpFilePtr->data[i][j].rgbRed + 0.587 * bmpFilePtr->data[i][j].rgbGreen + 0.114 * bmpFilePtr->data[i][j].rgbBlue;
            double U = -0.147 * bmpFilePtr->data[i][j].rgbRed - 0.289 * bmpFilePtr->data[i][j].rgbGreen + 0.436 * bmpFilePtr->data[i][j].rgbBlue;
            double V = 0.615 * bmpFilePtr->data[i][j].rgbRed - 0.515 * bmpFilePtr->data[i][j].rgbGreen - 0.100 * bmpFilePtr->data[i][j].rgbBlue;

            // 使用对数操作重新设定亮度值
            Y = 255.00 * log(Y / 255.00+ 1.00) / log(maxY / 255.00 + 1.00);


            // 再重新变回RGB
            // 将YUV下的数据重新生成

            // 先创建一个值来作为中间量，处理转换后的数据
            // 不要立刻进行赋值，先进行数据处理和范围判断！
            // 2023/11/12，在这个模块卡了好久111
            double rgbRed, rgbGreen, rgbBlue;
            rgbRed = 1.000 * Y - 0.000 * U + 1.140 * V;
            rgbGreen = 1.000 * Y - 0.395 * U - 0.580 * V;
            rgbBlue = 1.000 * Y + 2.036 * U + 0.000 * V;
            if (rgbRed > 255) {
                rgbRed = 255;
            } else if (rgbRed < 0) {
                rgbRed = 0;
            }
            if (rgbGreen > 255) {
                rgbGreen = 255;
            } else if (rgbGreen < 0) {
                rgbGreen = 0;
            }
            if (rgbBlue > 255) {
                rgbBlue = 255;
            } else if (rgbBlue < 0) {
                rgbBlue = 0;
            }
            bmpFilePtr->data[i][j].rgbRed = rgbRed;
            bmpFilePtr->data[i][j].rgbGreen = rgbGreen;
            bmpFilePtr->data[i][j].rgbBlue = rgbBlue;
        }
    }
}

/*
 * 对一个BMP进行直方图均衡化
 */
void histogramEqualization(BMP_File *bmpFilePtr) {
    // 获取直方图数据
    double* hist;
    hist = histogram(bmpFilePtr);

    // 对直方图数据进行处理，获得均衡化后的数据
    // 对于离散的灰度值，均衡化后的数据等于均衡前的数据的部分和
    // 了解相关的参考材料
    double* histEqualization;
    histEqualization = malloc(sizeof(double) * 256);
    histEqualization[0] = hist[0];
    for (int i = 1; i < 256; ++i) {
        histEqualization[i] = hist[i] + histEqualization[i - 1];
    }

    // 对histEqualization进行处理，获得我们新的灰度值对应数据
    BYTE equalization[256]; // 对于之前的每一个灰度值i，equalization[i]就是新的对应的灰度值
    for (int i = 0; i < 256; ++i) {
        // 先进行数据处理和范围约束
        double tmp = histEqualization[i] * 255.00;

        if (tmp > 255) {
            tmp = 255;
        } else if (tmp < 0) {
            tmp = 0;
        }

        equalization[i] = tmp;
    }

    // 开始对BMP的数据进行修改！
    // 总体代码根visibilityEnhance是一致的！

    // 设定以下我们可能会用到的量
    LONG height = bmpFilePtr->bitmapinfoheader.biHeight;
    LONG width = bmpFilePtr->bitmapinfoheader.biWidth;

    // 对每个像素进行直方图均衡化操作
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // RGB到YUV的数据转变，注意这里一定要是double，不要是BYTE类型，否则会在传递数据的过程中出现错误！
            double tmpY = 0.299 * bmpFilePtr->data[i][j].rgbRed + 0.587 * bmpFilePtr->data[i][j].rgbGreen + 0.114 * bmpFilePtr->data[i][j].rgbBlue;

            // 对Y进行范围约束，并根据直方图的数据，修改Y值并存到tmpY中
            if (tmpY > 255) {
                tmpY = 255;
            } else if (tmpY < 0) {
                tmpY = 0;
            }
            BYTE Y = tmpY;
            Y = equalization[Y];

            // 重新设定rgb的值
            bmpFilePtr->data[i][j].rgbRed = Y;
            bmpFilePtr->data[i][j].rgbGreen = Y;
            bmpFilePtr->data[i][j].rgbBlue = Y;
        }
    }

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
 * 以下是private function，不留接口给外部
 *
 * 可以看到代码中重复的模块并不少，如果日后有时间，整合打包一下，进行优化
 */
//-----------------------------------------------------------------------------------------------

/*
 * 输入一个BMP的数据，返回它的灰度图模式下的直方图数据
 *
 * 实际上，为了统一输入的指标，我们在实现代码上做了一些牺牲，也就是实现代码中重复的模块变多的
 * 但留出的接口更加简单易理解，我觉得可以这是可以接受的
 */
double* histogram(BMP_File *bmpFilePtr) {
    LONG count[256] = {0}; // 存储每个灰度级的像素的个数
    double* histogram = malloc(sizeof(double) * 256); // 存储每个灰度级出现的相对频率

    // 设定以下我们可能会用到的量
    LONG height = bmpFilePtr->bitmapinfoheader.biHeight; // 高度
    LONG width = bmpFilePtr->bitmapinfoheader.biWidth; // 宽度

    // 先将彩色图数据处理为灰度图数据
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // RGB到YUV的数据转变，注意这里一定要是double，不要是BYTE类型，否则会在传递数据的过程中出现错误！
            double tmpY = 0.299 * bmpFilePtr->data[i][j].rgbRed + 0.587 * bmpFilePtr->data[i][j].rgbGreen + 0.114 * bmpFilePtr->data[i][j].rgbBlue;

            // 首先对tmpY的值进行范围判断约束
            if (tmpY > 255) {
                tmpY = 255;
            } else if (tmpY < 0) {
                tmpY = 0;
            }

            // 下面对count进行操作
            BYTE Y = tmpY; // 用于存储灰度级的值
            count[Y]++; // 对灰度级的值为Y的，count++
        }
    }

    // 接下来转换count的数据到histogram中
    for (int i = 0; i < 256; ++i) {
        histogram[i] = ((double) count[i]) / (width * height);
    }

    // 返回直方图数据
    return histogram;
}

//--------------------------------------------------------------------------------------------------









