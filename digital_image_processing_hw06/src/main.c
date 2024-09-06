#include <stdio.h>
#include <stdlib.h>
#include "BMP_File.h"
#include "BMP_FileOperation.h"
#include "BilateralFilter.h"

int main() {
    // 运行的提示信息
    printf("generating, please wait for a moment~");

    // 创建存储原始数据的图像信息指针
    BMP_File *originalBmpFilePtr = initialBmpFilePtr();

    // 读取原始信息数据
    readBMP_File("../resource/Ushio.bmp", originalBmpFilePtr);

    // 创建图像信息指针，用于存储双边滤波后的图像
    BMP_File *bilBmpFilePtr1 = initialBmpFilePtr();

    // 进行双边滤波操作
    // sigmaS = 10000
    // sigmaC = 10000
    // 核的size = 5
    BilateralFilter(originalBmpFilePtr, bilBmpFilePtr1, 10000, 10000, 5);

    // 生成图像
    generateBMP_File(bilBmpFilePtr1, "../resource/Ushio_Bil_10000_10000_5.bmp");

    // 创建图像信息指针，用于存储双边滤波后的图像
    BMP_File *bilBmpFilePtr2 = initialBmpFilePtr();

    //进行双边滤波操作
    // sigmaS = 1
    // sigmaC = 10000
    // 核的size = 5
    BilateralFilter(originalBmpFilePtr, bilBmpFilePtr2, 1, 10000, 5);

    // 生成图像
    generateBMP_File(bilBmpFilePtr2, "../resource/Ushio_Bil_1_10000_5.bmp");

    // 创建图像信息指针，用于存储双边滤波后的图像
    BMP_File *bilBmpFilePtr3 = initialBmpFilePtr();

    //进行双边滤波操作
    // sigmaS = 10000
    // sigmaC = 1
    // 核的size = 5
    BilateralFilter(originalBmpFilePtr, bilBmpFilePtr3, 10000, 1, 5);

    // 生成图像
    generateBMP_File(bilBmpFilePtr3, "../resource/Ushio_Bil_10000_1_5.bmp");

    // 创建图像信息指针，用于存储双边滤波后的图像
    BMP_File *bilBmpFilePtr4 = initialBmpFilePtr();

    //进行双边滤波操作
    // sigmaS = 1
    // sigmaC = 1
    // 核的size = 5
    BilateralFilter(originalBmpFilePtr, bilBmpFilePtr4, 1, 1, 5);

    // 生成图像
    generateBMP_File(bilBmpFilePtr4, "../resource/Ushio_Bil_1_1_5.bmp");
    return 0;
}
