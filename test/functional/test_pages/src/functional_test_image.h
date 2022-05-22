#ifndef FUNCTIONAL_TEST_IMAGE_H
#define FUNCTIONAL_TEST_IMAGE_H

#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
class MainWindow;

class FunctionalTestImage
{
public:
    static bool isImageToEqual(MainWindow *mainWindow, const char *imageToCompare, int width, int height, bool canBeDifference = false);
    static bool isImagesToEqual(MainWindow *mainWindow, const char *imageToCompare0, const char *imageToCompare1, int width, int height);
private:
    static bool getRgb(const unsigned char *pngData, int x, int y, int width, int height, unsigned char &r, unsigned char &g, unsigned char &b);
    static bool getRgb(XImage *image, int x, int y, int width, int height,
                                 unsigned char &r, unsigned char &g, unsigned char &b);
};

#endif
