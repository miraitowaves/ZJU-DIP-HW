#include <stdio.h>
#include <stdlib.h>
#include "BMP_File.h"
#include "BMP_FileOperation.h"
#include "Filter.h"
#include "Laplace.h"

int main() {
    // 运行的提示信息
    printf("generating, please wait for a moment~");

    // 创建存储原始数据的图像信息指针
    BMP_File *originalBmpFilePtr = initialBmpFilePtr();

    // 读取原始信息数据
    readBMP_File("../resource/Ushio.bmp", originalBmpFilePtr);

    // 创建图像信息指针，用于存储均值滤波后的图像
    BMP_File *filBmpFilePtr1 = initialBmpFilePtr();

    // 进行均值滤波操作
    meanFiltering(originalBmpFilePtr, filBmpFilePtr1, 3);

    // 生成图像
    generateBMP_File(filBmpFilePtr1, "../resource/Ushio_Fil_size3.bmp");

    // 创建图像信息指针，用于存储均值滤波后的图像
    BMP_File *filBmpFilePtr2 = initialBmpFilePtr();

    // 进行均值滤波操作
    meanFiltering(originalBmpFilePtr, filBmpFilePtr2, 7);

    // 生成图像
    generateBMP_File(filBmpFilePtr2, "../resource/Ushio_Fil_size7.bmp");

    // 创建图像信息指针，用于存储拉普拉斯增强后的图像
    BMP_File *lapBmpFilePtr = initialBmpFilePtr();

    // 进行拉普拉斯增强操作
    lapEnhance(originalBmpFilePtr, lapBmpFilePtr);

    // 生成图像
    generateBMP_File(lapBmpFilePtr, "../resource/Ushio_lap.bmp");
    return 0;
}
