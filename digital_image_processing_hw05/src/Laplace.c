//
// Created by 方星皓 on 2023/12/8.
//

#include "Laplace.h"
#include "Utils.h"

/*
 * 对一个图像进行拉普拉斯图像增强操作
 * bmpFilePtr1：要处理的图像
 * bmpFilePtr2： 处理完后的图像
 * */
void lapEnhance(BMP_File* bmpFilePtr1, BMP_File* bmpFilePtr2) {

    // 拉普拉斯图像增强的核的size = 3
    bmpFilePtr2->bitmapinfoheader.biWidth = bmpFilePtr1->bitmapinfoheader.biWidth - 2;
    bmpFilePtr2->bitmapinfoheader.biHeight = bmpFilePtr1->bitmapinfoheader.biHeight - 2;

    for (int i = 0; i < bmpFilePtr2->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFilePtr2->bitmapinfoheader.biWidth; ++j) {
            bmpFilePtr2->data[i][j] = lapConvolution(bmpFilePtr1, i + 1, j + 1);
        }
    }
}