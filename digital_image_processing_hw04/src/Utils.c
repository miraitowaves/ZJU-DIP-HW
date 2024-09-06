//
// Created by 方星皓 on 2023/11/26.
//

#include "Utils.h"
#include "math.h"

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

    // 约束数据范围
    if (tmp_rgbRed > 255) {
        tmp_rgbRed = 255;
    } else if (tmp_rgbRed < 0) {
        tmp_rgbRed = 0;
    }
    if (tmp_rgbBlue > 255) {
        tmp_rgbBlue = 255;
    } else if (tmp_rgbBlue < 0) {
        tmp_rgbBlue = 0;
    }
    if (tmp_rgbGreen > 255) {
        tmp_rgbGreen = 255;
    } else if (tmp_rgbGreen < 0) {
        tmp_rgbGreen = 0;
    }

    // 创建新的data
    RGBQUAD newPixel;

    // 给新画布上色！
    newPixel.rgbRed = tmp_rgbRed;
    newPixel.rgbBlue = tmp_rgbBlue;
    newPixel.rgbGreen = tmp_rgbGreen;

    return newPixel;
}