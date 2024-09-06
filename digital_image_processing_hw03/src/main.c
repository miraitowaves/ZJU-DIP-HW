#include <stdio.h>
#include "BMP_FileOperation.h"

int main() {
    // 创建一个BMP数据文件的指针
    BMP_File* bmpFilePtr1 = (BMP_File*) malloc(sizeof(BMP_File));

    // 调用readBMP_File函数来读取函数中的数据，写入bmpFIlePtr所指示的文件
    readBMP_File("../resource/Ushio.bmp", bmpFilePtr1);


    // 进行图像增强操作
    visibilityEnhance(bmpFilePtr1);

    // 生成增强后的图像
    generateBMP_File(bmpFilePtr1, "../resource/EnhancedUshio.bmp");

    // 创建一个BMP数据文件的指针
    BMP_File* bmpFilePtr2 = (BMP_File*) malloc(sizeof(BMP_File));

    // 调用readBMP_File函数来读取函数中的数据，写入bmpFIlePtr所指示的文件
    readBMP_File("../resource/Ushio.bmp", bmpFilePtr2);

    // 对图像进行灰度化操作
    changeToGray(bmpFilePtr2);

    // 生成灰度化后的图像
    generateBMP_File(bmpFilePtr2, "../resource/GrayUshio.bmp");

    // 创建一个BMP数据文件的指针
    BMP_File* bmpFilePtr3 = (BMP_File*) malloc(sizeof(BMP_File));

    // 调用readBMP_File函数来读取函数中的数据，写入bmpFIlePtr所指示的文件
    readBMP_File("../resource/Ushio.bmp", bmpFilePtr3);

    // 对图像进行直方图均衡化操作
    histogramEqualization(bmpFilePtr3);

    // 生成直方图均衡化后的图像
    generateBMP_File(bmpFilePtr3, "../resource/HistEqUshio.bmp");

    return 0;
}
