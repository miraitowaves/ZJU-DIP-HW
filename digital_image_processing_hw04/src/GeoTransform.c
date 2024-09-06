//
// Created by 方星皓 on 2023/11/24.
//

#include <stdio.h>
#include <math.h>
#include "BMP_File.h"
#include "BMP_FileOperation.h"
#include "GeoTransform.h"
#include "stdlib.h"
#include "Utils.h"

/*
 * 功能：实现图像的平移操作
 *
 * 输入：
 * BMP_File *bmpFilePtr1, 输入原始的BMP图像指针
 * BMP_File *bmpFilePtr2, 输出的新的图像指针
 * LONG toRight, 向右偏移的像素长度（以像素点为单位），正值代表向右，负值代表向左
 * LONG toUp, 向上偏移的像素长度，与toRight同理
 */
void translation(BMP_File *bmpFilePtr1, BMP_File* bmpFilePtr2, LONG toRight, LONG toUp){
    // 初始画布大小
    LONG originalHeight = bmpFilePtr1->bitmapinfoheader.biHeight;
    LONG originalWidth = bmpFilePtr1->bitmapinfoheader.biWidth;

    // 更改画布大小
    bmpFilePtr2->bitmapinfoheader.biHeight = originalHeight + abs(toUp);
    bmpFilePtr2->bitmapinfoheader.biWidth = originalWidth + abs(toRight);


    // 接下来进行平移操作
    for (int i = 0; i < originalHeight; ++i) {
        for (int j = 0; j < originalWidth; ++j) {
            if (toUp >= 0 && toRight >= 0) { // 画布需要向上延申和向右延申
                bmpFilePtr2->data[i + toUp][j + toRight] = bmpFilePtr1->data[i][j];
            } else if (toUp < 0 && toRight >= 0){ // 向下移动，但数组下标最小值为0，因此在纵坐标上不移动
                bmpFilePtr2->data[i][j + toRight] = bmpFilePtr1->data[i][j];
            } else if (toUp >= 0 && toRight < 0) { // 向左移动，但数组下标最小值为0，因此在横坐标上不移动
                bmpFilePtr2->data[i + toUp][j] = bmpFilePtr1->data[i][j];
            }
            // 都小于0，那么就不需要重新赋值，只需要调整画布大小
        }
    }

}

/*
 * 功能：实现图像的镜像操作
 *
 * 输入：
 * BMP_File *bmpFilePtr, 输入的BMP图像指针
 */
void mirror(BMP_File *bmpFilePtr1, BMP_File* bmpFilePtr2, char axis) {
    // 获取画布大小
    LONG height = bmpFilePtr1->bitmapinfoheader.biHeight;
    LONG width = bmpFilePtr1->bitmapinfoheader.biWidth;


    // 进行图像镜像操作
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (axis == 'Y') {
                bmpFilePtr2->data[i][j] = bmpFilePtr1->data[i][width - j - 1];
            } else if (axis == 'X') {
                bmpFilePtr2->data[i][j] = bmpFilePtr1->data[height - i - 1][j];
            }
        }
    }
}

/*
 * 功能：实现图像的旋转操作
 *
 * 输入：
 * double angle：旋转的角度，逆时针顺序
 */
void rotation(BMP_File *bmpFilePtr1, BMP_File *bmpFilePtr2, double theta) {
    // 获得原始画布的大小
    LONG originalHeight = bmpFilePtr1->bitmapinfoheader.biHeight;
    LONG originalWidth = bmpFilePtr1->bitmapinfoheader.biWidth;

    // 获得新画布大小
    LONG height = floor(abs(originalHeight * cos(theta / 180 * pi))
            + abs(originalWidth * sin(theta / 180.0 * pi)));
    LONG width = floor (abs(originalWidth * cos(theta / 180 * pi))
            + abs(originalHeight * sin(theta / 180.0 * pi)));

    // 更改画布大小
    bmpFilePtr2->bitmapinfoheader.biHeight = height;
    bmpFilePtr2->bitmapinfoheader.biWidth = width;

    // 进行图像的旋转操作
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // 现在我们对旋转后所得的画布的每个像素点进行操作
            // 定义originH和originW
            // 为与当前新画布的像素点位置相对应的原始图像的像素点位置
            double origin_h, origin_w;
            origin_h = (i - height / 2.0) * cos(theta / 180 * pi) + (j - width / 2.0)
                    * sin(theta / 180 * pi) + originalHeight / 2.0;
            origin_w = (i - height / 2.0) * sin(-theta / 180 * pi) + (j - width / 2.0)
                    * cos(theta / 180 * pi) +originalWidth / 2.0;
            // 使用双线性插值法获取原图像素信息
            if (origin_h >= 0 && origin_h < originalHeight&& origin_w >= 0 && origin_w < originalWidth) { //
                bmpFilePtr2->data[i][j] = bininear(bmpFilePtr1, origin_h, origin_w); // 获取新的图像信息数据
            }
        }
    }
}

/*
 * 功能：实现图像的缩放操作
 *
 * 输入：
 * double heightScaling
 * double widthScaling
 */
void scale(BMP_File *bmpFilePtr1, BMP_File *bmpFilePtr2, double heightScale, double widthScale) {
    // 获取原始画布大小
    LONG originalHeight = bmpFilePtr1->bitmapinfoheader.biHeight;
    LONG originalWidth = bmpFilePtr1->bitmapinfoheader.biWidth;

    // 获取新的画布大小
    LONG scaledHeight = floor(originalHeight * heightScale);
    LONG scaledWidth = floor(originalWidth * widthScale);

    // 将新的画布赋给bmpFilePtr2
    bmpFilePtr2->bitmapinfoheader.biHeight = scaledHeight;
    bmpFilePtr2->bitmapinfoheader.biWidth = scaledWidth;

    // 定义原始像素点变量
    double origin_h, origin_w;

    for (int i = 0; i < scaledHeight; ++i) {
        for (int j = 0; j < scaledWidth; ++j) {
            // 对于新的像素点data[i][j]
            // 获取它所对应的原图中的像素点
            // 并把数据赋值给origin_h和origin_w
            origin_h = i / heightScale;
            origin_w = j / widthScale;
            // 使用双线性插值法获取原图像素信息
            if (origin_h >= 0 && origin_h < originalHeight&& origin_w >= 0 && origin_w < originalWidth) { //
                bmpFilePtr2->data[i][j] = bininear(bmpFilePtr1, origin_h, origin_w); // 获取新的图像信息数据
            }
        }
    }
}

/*
 * 功能：实现图像的错切操作
 *
 * 输入：
 * char axis：选择错切X轴还是错切Y轴
 * double misalignCoefficient
 */
void shear(BMP_File* bmpFilePtr1, BMP_File* bmpFilePtr2, char axis, double misalignCoeddicient) {
    // 获取原始画布大小
    LONG originalHeight = bmpFilePtr1->bitmapinfoheader.biHeight;
    LONG originalWidth = bmpFilePtr1->bitmapinfoheader.biWidth;

    // 获取新的画布大小
    LONG height, width;
    if (axis == 'X') {
        width = originalWidth;
        height = originalHeight + misalignCoeddicient * originalWidth;
    } else if (axis == 'Y') {
        width = originalWidth + originalHeight * misalignCoeddicient;
        height = originalHeight;
    }

    // 将新画布大小赋值给bmpFilePtr2
    bmpFilePtr2->bitmapinfoheader.biHeight = height;
    bmpFilePtr2->bitmapinfoheader.biWidth = width;

    // 定义原始像素点变量
    double origin_h, origin_w;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // 根据变换情况，获取原图信息
            if (axis == 'X') {
                origin_w = j;
                origin_h = i - misalignCoeddicient * j;
            } else if (axis == 'Y') {
                origin_w = j - misalignCoeddicient * i;
                origin_h = i;
            }
            // 使用双线性插值法获取原图像素信息
            if (origin_h >= 0 && origin_h < originalHeight&& origin_w >= 0 && origin_w < originalWidth) { //
                bmpFilePtr2->data[i][j] = bininear(bmpFilePtr1, origin_h, origin_w); // 获取新的图像信息数据
            }
        }
    }
}
