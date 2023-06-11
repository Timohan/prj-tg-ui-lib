#include "functional_test_image.h"
#include <thread>
#include <unistd.h>
#include "../../../../lib/src/global/tg_global_log.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include "mainwindow.h"
#include "tg_image_load.h"

#ifndef IMAGES_TO_COMPARE_DIR
#define IMAGES_TO_COMPARE_DIR "DS"
#endif

bool FunctionalTestImage::isImageToEqual(MainWindow *mainWindow, const char *imageToCompare, int width, int height, bool canBeDifference)
{
    std::string imagePath = IMAGES_TO_COMPARE_DIR;
    imagePath += "/";
    imagePath += imageToCompare;
    int imageWidth = 0;
    int imageHeight = 0;

    unsigned char *pngData = TgImageLoad::loadPng(imagePath.c_str(), imageWidth, imageHeight);
    if (!pngData) {
        TG_ERROR_LOG("Failed to load image: ", imagePath);
        return false;
    }
    if (width != imageWidth
        || height != imageHeight) {
        delete[] pngData;
        TG_ERROR_LOG("Image have a wrong size: " + imagePath + " " + std::to_string(width) + "/" + std::to_string(height) + " vs. " + std::to_string(imageWidth) + "/" + std::to_string(imageHeight) );
        return false;
    }

    XImage *image = XGetImage(mainWindow->getDisplay(),
                              *mainWindow->getWindow(), 0, 0, width, height, AllPlanes, ZPixmap);
    bool ret = true;
    int x, y;
    uint8_t imageColors[3];
    uint8_t pngColors[3];

    for (x=0;x<width && ret;x++) {
        for (y=0;y<height && ret;y++) {
            getRgb(pngData, x, y, width, height, pngColors[0], pngColors[1], pngColors[2]);
            getRgb(image, x, y, width, height, imageColors[0], imageColors[1], imageColors[2]);

            if (!isEqual(pngColors, imageColors)) {
                if (!canBeDifference) {
                    TG_ERROR_LOG("Image have a pixel: " + imagePath + " " + std::to_string(x) + "/" + std::to_string(y) +
                        "(" + std::to_string(pngColors[0]) + "," + std::to_string(pngColors[1]) + "," + std::to_string(pngColors[2]) + ")" +
                        "(" + std::to_string(imageColors[0]) + "," + std::to_string(imageColors[1]) + "," + std::to_string(imageColors[2]) + ")" );
                }
                ret = false;
            }
        }
    }
    XDestroyImage(image);
    delete[] pngData;
    sleep(1);
    return ret;
}

bool FunctionalTestImage::isEqual(uint8_t imageColors0[3], uint8_t imageColors1[3])
{
    for (int i=0;i<3;i++) {
        if (imageColors0[i] == imageColors1[i]) {
            continue;
        }
        if (imageColors0[i] < __UINT8_MAX__-1 && imageColors0[i]+1 == imageColors1[i]) {
            continue;
        }
        if (imageColors0[i] > 0 && imageColors0[i]-1 == imageColors1[i]) {
            continue;
        }
        return false;
    }
    return true;
}

bool FunctionalTestImage::isImagesToEqual(MainWindow *mainWindow, const char *imageToCompare0, const char *imageToCompare1, int width, int height)
{
    bool isEqual[2];
    int equalCount[2];
    int i2;
    memset(equalCount, 0, sizeof(int)*2);
    for (int i=0;i<10;i++) {
        isEqual[0] = FunctionalTestImage::isImageToEqual(mainWindow, imageToCompare0, width, height, true);
        isEqual[1] = FunctionalTestImage::isImageToEqual(mainWindow, imageToCompare1, width, height, true);
        for (i2=0;i2<2;i2++) {
            if (isEqual[i2]) {
                equalCount[i2]++;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!equalCount[0] && !equalCount[1]) {
        TG_ERROR_LOG("Both image comparisions are incorrect: ", imageToCompare0, " ", imageToCompare1);
        return false;
    }
    if (!equalCount[0]) {
        TG_ERROR_LOG("Image was not found during this period: ", imageToCompare0);
        return false;
    }
    if (!equalCount[1]) {
        TG_ERROR_LOG("Image was not found during this period: ", imageToCompare1);
        return false;
    }
    return true;
}

bool FunctionalTestImage::getRgb(const unsigned char *pngData, int x, int y, int width, int height,
                                 unsigned char &r, unsigned char &g, unsigned char &b)
{
    if (x < 0 || x >= width
        || y < 0 || y >= height) {
        return false;
    }
    r =  pngData[ y*width*4+x*4+0 ];
    g =  pngData[ y*width*4+x*4+1 ];
    b =  pngData[ y*width*4+x*4+2 ];
    return true;
}

bool FunctionalTestImage::getRgb(XImage *image, int x, int y, int width, int height,
                                 unsigned char &r, unsigned char &g, unsigned char &b)
{
    if (x < 0 || x >= width
        || y < 0 || y >= height) {
        return false;
    }
    unsigned long pixel = XGetPixel(image,x,y);

    b = static_cast<uint8_t>(pixel & image->blue_mask);
    g = static_cast<uint8_t>((pixel & image->green_mask) >> 8);
    r = static_cast<uint8_t>((pixel & image->red_mask) >> 16);
    return true;
}