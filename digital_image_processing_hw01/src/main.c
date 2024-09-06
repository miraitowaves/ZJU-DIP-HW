#include <stdio.h>
#include "BMP_File.h"
#include "BMP_FileOperation.h"


int main() {

    // 创建一个BMP数据文件的指针
    BMP_File* bmpFilePtr = (BMP_File*) malloc(sizeof(BMP_File));

    // 调用readBMP_File函数来读取函数中的数据，写入bmpFIlePtr所指示的文件
    readBMP_File("../resource/initialImage.bmp", bmpFilePtr);

    // 创建一个在YUV空间下的BMP数据文件的指针
    BMP_FileInYUV* bmpFileInYuvPtr = (BMP_FileInYUV*) malloc(sizeof(BMP_FileInYUV));

    // 将RGB空间下的bmpFilePtr的数据写入bmpFileInYuvPtr
    changeBMP_FileFromRGB_StandardToYUV_Standard(bmpFilePtr,bmpFileInYuvPtr);

    // 生成一个灰色图像
    generateGrayBMP_File(bmpFileInYuvPtr, "../resource/GGBondInGray.bmp");

    // 改变Y值
    // 在这里我直接翻转了Y值
    // 比方说，原本是255，现在改成0
    for (int i = 0; i < bmpFileInYuvPtr->bitmapinfoheader.biHeight; ++i) {
        for (int j = 0; j < bmpFileInYuvPtr->bitmapinfoheader.biWidth; ++j) {
            bmpFileInYuvPtr->data[i][j].Y = 255 - bmpFileInYuvPtr->data[i][j].Y;
        }
    }

    // 创建新的BMP_File文件bmpFilePtr1
    BMP_File* bmpFilePtr1 = (BMP_File*) malloc(sizeof(BMP_File));

    // 将YUV空间下的bmpFileInYuvPtr的数据写入新创建的bmpFilePtr1
    changeBMP_FileFromYUV_StandardToRGB_Standard(bmpFileInYuvPtr, bmpFilePtr1);

    //输出一个新的彩色BMP图像文件
    generateColorBMP_File(bmpFilePtr1, "../resource/GGBond.bmp");
    return 0;
}
