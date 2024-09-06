#include <stdio.h>
#include "BMP_FileOperation.h"

int main() {
    // 创建一个BMP数据文件的指针
    BMP_File* bmpFilePtr = (BMP_File*) malloc(sizeof(BMP_File));

    // 调用readBMP_File函数来读取函数中的数据，写入bmpFIlePtr所指示的文件
    readBMP_File("../resource/GGBond.bmp", bmpFilePtr);

    // 创建一个在YUV空间下的BMP数据文件的指针(灰度图)
    BMP_FileInYUV* bmpFileInYuvPtr = (BMP_FileInYUV*) malloc(sizeof(BMP_FileInYUV));

    // 将RGB空间下的bmpFilePtr的数据写入bmpFileInYuvPtr
    changeBMP_FileFromRGB_StandardToYUV_Standard(bmpFilePtr,bmpFileInYuvPtr);

    // 生成一个灰度图，用于后续图像的效果对比
    generateGrayBMP_File(bmpFileInYuvPtr, "../resource/GrayGGBond.bmp");

    // 创建一个二值图像的指针
    BinaryBMP_File* binaryBmpFilePtr1 = (BinaryBMP_File*) malloc(sizeof(BinaryBMP_File));

    // 将灰度图转换为二值图像
    changeFromGrayToBinary(bmpFileInYuvPtr, binaryBmpFilePtr1, 125);

    // 生成一个二值图像，在给定的路径中
    generateBinaryBMP_File(binaryBmpFilePtr1, "../resource/BinaryGGBond.bmp");

    // 调用readBinaryBMP_File函数来读取数据，写入binaryBmpFilePtr1所指示的文件
    readBinaryBMP_File("../resource/BinaryGGBond.bmp", binaryBmpFilePtr1);

    // 腐蚀二值图像
    // 因为设置的腐蚀条件较为简单，水平腐蚀
    // 所以多调用了几次腐蚀
    for(int i = 0; i < 4; i++) {
        levelCorrodeBinaryBmpFile(binaryBmpFilePtr1);
    }

    // 生成二值图像，经腐蚀过的
    generateBinaryBMP_File(binaryBmpFilePtr1, "../resource/CorrosionTest.bmp");

    // 创建一个新的二值图像的指针，用于膨胀测试
    BinaryBMP_File *binaryBmpFilePtr2 = (BinaryBMP_File*) malloc(sizeof(BinaryBMP_File));

    // 调用readBinaryBMP_File函数来读取数据，写入binaryBmpFilePtr2所指示的文件
    readBinaryBMP_File("../resource/BinaryGGBond.bmp", binaryBmpFilePtr2);

    // 进行膨胀操作
    // 做一个区分，膨胀选择的是简单的垂直膨胀
    // 同样多膨胀几次，做一个区分
    for (int i = 0; i < 3; ++i) {
        verticalDilateBinaryBmpFile(binaryBmpFilePtr2);
    }

    // 生成膨胀后的二值图像
    generateBinaryBMP_File(binaryBmpFilePtr2, "../resource/DilationTest.bmp");

    // 创建一个新的二值图像的指针，用于开运算测试
    BinaryBMP_File *binaryBmpFilePtr3 = (BinaryBMP_File*) malloc(sizeof(BinaryBMP_File));

    // 调用readBinaryBMP_File函数来读取数据，写入binaryBmpFilePtr3所指示的文件
    readBinaryBMP_File("../resource/BinaryGGBond.bmp", binaryBmpFilePtr3);

    // 开运算
    openingBinaryBmpFile(binaryBmpFilePtr3, 3, 3);

    // 输出开运算后的图像
    generateBinaryBMP_File(binaryBmpFilePtr3, "../resource/OpeningTest.bmp");

    // 创建一个新的二值图像的指针，用于闭运算测试
    BinaryBMP_File *binaryBmpFilePtr4 = (BinaryBMP_File*) malloc(sizeof(BinaryBMP_File));

    // 调用readBinaryBMP_File函数来读取数据，写入binaryBmpFilePtr4所指示的文件
    readBinaryBMP_File("../resource/BinaryGGBond.bmp", binaryBmpFilePtr4);

    // 闭运算
    closingBinaryBmpFile(binaryBmpFilePtr4, 3, 3);

    // 输出闭运算后的图像
    generateBinaryBMP_File(binaryBmpFilePtr4, "../resource/ClosingTest.bmp");

    return 0;
}
