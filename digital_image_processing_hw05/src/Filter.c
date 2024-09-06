//
// Created by 方星皓 on 2023/12/8.
//

#include "Filter.h"
#include "Utils.h"

/*
 * 实现均值滤波的函数
 *
 * bmpFilePtr1: 顾名思义，是我们要处理的图像的指针
 * bmpFilePtr2: 处理完后，存储数据的新的指针
 * size: 我们处理的核的大小(size * size),一般为奇数（不然就不对称了111）
 * */
void meanFiltering(BMP_File* bmpFilePtr1,BMP_File* bmpFilePtr2, int size) {
    // 更改滤波后的新的图像的实际大小数据
    bmpFilePtr2->bitmapinfoheader.biWidth = bmpFilePtr1->bitmapinfoheader.biWidth - size + 1;
    bmpFilePtr2->bitmapinfoheader.biHeight = bmpFilePtr1->bitmapinfoheader.biHeight- size + 1;

    // 开始进行滤波操作
    for (int i = 0; i < bmpFilePtr2->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFilePtr2->bitmapinfoheader.biWidth; ++j) {
            bmpFilePtr2->data[i][j] = meanConvolution(bmpFilePtr1, i + size/2, j + size/2, size);
        }
    }

}
