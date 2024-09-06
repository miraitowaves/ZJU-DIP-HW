//
// Created by 方星皓 on 2023/11/26.
//

#include "Utils.h"
#include "math.h"

/*
 * 对数据范围进行约束
 * */
BYTE adjust(double data) {
    if (data >= 255.0) {
        return 255;
    } else if (data <= 0.0) {
        return 0;
    } else {
        return data;
    }
}


/*
 * 原理：双线性插值
 * 由原图中的邻近的四个像素点来计算新图中的一个点
 */
RGBQUAD bininear(BMP_File* bmpFilePtr, double origin_h, double origin_w) {
    // 获得原始图片的高与宽
    LONG originalHeight = bmpFilePtr->bitmapinfoheader.biHeight;
    LONG originalWidth = bmpFilePtr->bitmapinfoheader.biWidth;

    // 对获得的像素进行调整
    // 对origin_h和origin_w进行范围约束
    if (origin_h > originalHeight - 1) {
        origin_h = originalHeight - 1;
    }
    if (origin_w > originalWidth - 1) {
        origin_w = originalWidth - 1;
    }

    // 分析origin_h和origin_w四周的像素点情况
    int floor_h = floor(origin_h);
    int ceil_h = ceil(origin_h);
    int floor_w = floor(origin_w);
    int ceil_w = ceil(origin_w);

    // origin_h和floor_h的偏移情况, 用于最终的取点
    double shifting_h = origin_h - floor_h;
    double shifting_w = origin_w - floor_w;

    // origin_h origin_w 四周的像素点分布情况
    RGBQUAD left_down = bmpFilePtr->data[floor_h][floor_w];
    RGBQUAD left_up = bmpFilePtr->data[ceil_h][floor_w];
    RGBQUAD right_down = bmpFilePtr->data[floor_h][ceil_w];
    RGBQUAD right_up = bmpFilePtr->data[ceil_h][ceil_w];

    // 给新的画布的像素赋值
    // 思路：调整各个点的RGB的像素值
    // 按照它们与左下角的像素点的偏移值，改变渐进改变RGB的值
    double tmp_rgbRed = (left_up.rgbRed - left_down.rgbRed) * shifting_h +
                        (right_down.rgbRed - left_down.rgbRed) * shifting_w +
                        (left_down.rgbRed + right_up.rgbRed - left_up.rgbRed - right_down.rgbRed)
                        * shifting_w * shifting_h +
                        left_down.rgbRed;
    double tmp_rgbBlue = (left_up.rgbBlue - left_down.rgbBlue) * shifting_h +
                         (right_down.rgbBlue - left_down.rgbBlue) * shifting_w +
                         (left_down.rgbBlue + right_up.rgbBlue - left_up.rgbBlue - right_down.rgbBlue)
                         * shifting_w * shifting_h + left_down.rgbBlue;
    double tmp_rgbGreen = (left_up.rgbGreen - left_down.rgbGreen) * shifting_h +
                          (right_down.rgbGreen - left_down.rgbGreen) * shifting_w +
                          (left_down.rgbGreen + right_up.rgbGreen - left_up.rgbGreen - right_down.rgbGreen)
                          * shifting_w * shifting_h + left_down.rgbGreen;

    // 创建新的data
    RGBQUAD newPixel;

    // 给新画布上色！
    newPixel.rgbRed = adjust(tmp_rgbRed);
    newPixel.rgbBlue = adjust(tmp_rgbBlue);
    newPixel.rgbGreen = adjust(tmp_rgbGreen);

    return newPixel;
}

/*
 * 卷积操作, 均值形式的卷积
 * 对给定的要获得的像素点，考虑它在原始图像中对应的核的大小和位置，进行卷积
 * bmpFilePtr: 要进行卷积的原始图像数据
 * h: 核中心高度
 * w: 核中心宽度
 * size: 核的大小
 * */
RGBQUAD meanConvolution(BMP_File* bmpFilePtr,int h, int w, int size) {
    // 存储处理后的信息
    RGBQUAD newPixel;

    // 分别存储核在均值下的red、green和blue对应的值的总和
    double sumR = 0, sumG = 0, sumB = 0;

    for (int i = h - size/2; i <= h + size/2; ++i) {
        for (int j = w - size/2; j <= w + size/2; ++j) {
            sumR += bmpFilePtr->data[i][j].rgbRed;
            sumG += bmpFilePtr->data[i][j].rgbGreen;
            sumB += bmpFilePtr->data[i][j].rgbBlue;
        }
    }

    // 对总和进行均分
    // 先存到temp里，是因为数据可能会溢出，我们要先对原始数据处理
    double tempR = sumR / (size * size * 1.0);
    double tempG = sumG / (size * size * 1.0);
    double tempB = sumB / (size * size * 1.0);

    // 对数据范围进行约束，得到我们的实际数据
    newPixel.rgbRed = adjust(tempR);
    newPixel.rgbGreen = adjust(tempG);
    newPixel.rgbBlue = adjust(tempB);
    newPixel.rgbReserved = 0;

    return newPixel;
}

/*
 * 卷积操作, 拉普拉斯算子下的卷积
 * 对给定的要获得的像素点，考虑它在原始图像中对应的核的大小和位置，进行卷积
 * bmpFilePtr: 要进行卷积的原始图像数据
 * h: 核中心高度
 * w: 核中心宽度
 * size: 固定为3，所以就不写出了
 * */
RGBQUAD lapConvolution(BMP_File* bmpFilePtr,int h, int w){
    RGBQUAD newPixel;

    // 分别存储核在均值下的red、green和blue对应的值的总和
    double sumR = 0, sumG = 0, sumB = 0;

    for (int i = h - 1; i <= h + 1; ++i) {
        for (int j = w - 1; j <= w + 1; ++j) {
            sumR += bmpFilePtr->data[i][j].rgbRed;
            sumG += bmpFilePtr->data[i][j].rgbGreen;
            sumB += bmpFilePtr->data[i][j].rgbBlue;
        }
    }

    // 对总和进行均分
    // 先存到temp里，是因为数据可能会溢出，我们要先对原始数据处理
    double tempR = 10 * bmpFilePtr->data[h][w].rgbRed - sumR;
    double tempG = 10 * bmpFilePtr->data[h][w].rgbGreen - sumG;
    double tempB = 10 * bmpFilePtr->data[h][w].rgbBlue - sumB;

    // 对数据范围进行约束，得到我们的实际数据
    newPixel.rgbRed = adjust(tempR);
    newPixel.rgbGreen = adjust(tempG);
    newPixel.rgbBlue = adjust(tempB);
    newPixel.rgbReserved = 0;

    return newPixel;
}

