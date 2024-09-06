//
// Created by 方星皓 on 2023/12/22.
//

#include <math.h>
#include "BilateralFilter.h"
#include "Utils.h"

/*
 * 功能：实现双边滤波
 * sigmaS: 空间域的σ大小
 * sigmaC: 色彩空间(RGB)的σ大小
 * */
void BilateralFilter(BMP_File *bmpFilePtr1, BMP_File *bmpFilePtr2, double sigmaS, double sigmaC, int size) {
    // 获取新生成图像的大小
    bmpFilePtr2->bitmapinfoheader.biHeight = bmpFilePtr1->bitmapinfoheader.biHeight;
    bmpFilePtr2->bitmapinfoheader.biWidth = bmpFilePtr1->bitmapinfoheader.biWidth;

    // 对每个像素进行处理
    for (int i = 0; i < bmpFilePtr2->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFilePtr2->bitmapinfoheader.biWidth; ++j) {
            bmpFilePtr2->data[i][j] = calcPixelByBil(bmpFilePtr1, i, j, sigmaS, sigmaC, size);
        }
    }
}

/*
 * 对双边滤波下的每一个像素点进行滤波操作
 * */
RGBQUAD calcPixelByBil(BMP_File * bmpFilePtr, int h, int w, double sigmaS, double sigmaC, int size) {
    // 创建要返回的像素点
    RGBQUAD newPixel;
    // 确定该像素点的滤波器大小
    int hmin = h - (size - 1) / 2;
    int hmax = h + (size - 1) / 2;
    int wmin = w - (size - 1) / 2;
    int wmax = w + (size - 1) / 2;

    double Wp = 0; // 归一化因子
    double sumR = 0, sumG = 0, sumB = 0;

    // 对滤波器大小进行约束
    if (hmin < 0) hmin = 0;
    if (hmax > bmpFilePtr->bitmapinfoheader.biHeight - 1) hmax = bmpFilePtr->bitmapinfoheader.biHeight - 1;
    if (wmin < 0) wmin = 0;
    if (wmax > bmpFilePtr->bitmapinfoheader.biWidth - 1) wmax = bmpFilePtr->bitmapinfoheader.biWidth - 1;

    // 进行滤波处理
    for (int i = hmin; i <= hmax; ++i) {
        for (int j = wmin; j <= wmax; ++j) {
            // 用高斯函数计算权重
            double weightS = 1 / (sqrt(2 * pi) * sigmaS)
                    * exp(- calcSpaceDistance(i, j, h, w) / (2 * sigmaS * sigmaS));
            double weightC = 1 / (sqrt(2 * pi) * sigmaC)
                    * exp(- calcColorDistance(bmpFilePtr, i, j, h, w) / (2 * sigmaC * sigmaC));

            // 真正的权重
            double weight = weightC * weightS;
            // 计算归一化因子
            Wp += weight;

            sumR += weight * bmpFilePtr->data[i][j].rgbRed;
            sumG += weight * bmpFilePtr->data[i][j].rgbGreen;
            sumB += weight * bmpFilePtr->data[i][j].rgbBlue;
        }
    }

    // 调整赋值
    newPixel.rgbRed = adjust(sumR / Wp);
    newPixel.rgbBlue = adjust(sumB / Wp);
    newPixel.rgbGreen = adjust(sumG / Wp);
    newPixel.rgbReserved = 0;

    return newPixel;
}
