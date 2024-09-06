#include <stdio.h>
#include <stdlib.h>
#include "BMP_File.h"
#include "BMP_FileOperation.h"
#include "GeoTransform.h"

int main() {
    // 运行的提示信息
    printf("正在生成图像，请耐心等待");

    // 创建存储原始数据的图像信息指针
    BMP_File *originalBmpFilePtr = initialBmpFilePtr();

    // 读取原始信息数据
    readBMP_File("../resource/Ushio.bmp", originalBmpFilePtr);

    // 创建一个图像信息指针，用于存储平移变换后的图像信息
    BMP_File *tranBmpFilePtr = initialBmpFilePtr();

    // 进行平移操作
    translation(originalBmpFilePtr, tranBmpFilePtr, 300, 300);

    // 生成平移后的图像
    generateBMP_File(tranBmpFilePtr, "../resource/Ushio_tran.bmp");

    // 创建一个图像信息指针，用于存储镜像变换后的图像信息
    BMP_File *mirrorBmpFilePtr = initialBmpFilePtr();

    // 进行镜像操作
    mirror(originalBmpFilePtr, mirrorBmpFilePtr, 'Y');

    // 生成镜像后的图像
    generateBMP_File(mirrorBmpFilePtr, "../resource/Ushio_mirror.bmp");

    // 创建一个图像信息指针，用于存储旋转后的图像信息
    BMP_File *rotationBmpFilePtr = initialBmpFilePtr();

    // 进行旋转操作
    rotation(originalBmpFilePtr, rotationBmpFilePtr, 30);

    // 生成旋转后的图像
    generateBMP_File(rotationBmpFilePtr, "../resource/Ushio_rotation.bmp");

    // 创建一个图像信息指针，用于存储缩放后的图像信息
    BMP_File *scaleBmpFilePtr = initialBmpFilePtr();

    // 进行缩放操作
    scale(originalBmpFilePtr, scaleBmpFilePtr, 1.2, 0.6);

    // 生成缩放后的图像
    generateBMP_File(scaleBmpFilePtr, "../resource/Ushio_scale.bmp");

    // 创建一个图像信息指针，用于存储错切后的图像信息
    BMP_File *shearBmpFilePtr = initialBmpFilePtr();

    // 进行错切操作
    shear(originalBmpFilePtr, shearBmpFilePtr, 'Y', 0.6);

    // 生成错切后的图像
    generateBMP_File(shearBmpFilePtr, "../resource/Ushio_shear.bmp");



    return 0;
}
