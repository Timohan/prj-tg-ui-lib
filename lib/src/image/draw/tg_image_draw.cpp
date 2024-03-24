/*!
 * \file
 * \brief file tg_image_draw.cpp
 *
 * image draw functionalities
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#include "tg_image_draw.h"
#include <cstddef>
#include "../../global/tg_global_log.h"
#include "../../math/tg_color.h"
#include "../../global/tg_global_log.h"

bool TgImageDraw::drawLine(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t fromX, const uint32_t fromY,
                           const uint32_t toX, const uint32_t toY,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a,
                           const uint32_t lineWidth)
{
    if (fromX > toX) {
         return drawLine(imageData, imageWidth, imageHeight, toX, toY, fromX, fromY, r, g, b, a, lineWidth);
    }

    if (fromX == toX && fromY == toY) {
        return addColor(imageData, imageWidth, imageHeight, fromX, fromY, r, g, b, a);
    }

    if (fromX == toX) {
        if (fromY <= toY) {
            if (lineWidth == 1) {
                for (uint32_t y=fromY;y<=toY;y++) {
                    if (!addColor(imageData, imageWidth, imageHeight, fromX, y, r, g, b, a)) {
                        return false;
                    }
                }
            } else if ((lineWidth % 2) == 1) {
                int xTmpMin = static_cast<int>(fromX) - static_cast<int>((lineWidth-1)/2);
                if (xTmpMin + static_cast<int>(lineWidth) < 0) {
                    return false;
                }
                uint32_t xMin = xTmpMin < 0 ? 0 : static_cast<uint32_t>(xTmpMin);
                uint32_t xMax = static_cast<uint32_t>(xTmpMin + static_cast<int>(lineWidth));
                uint32_t x;
                if (xMin >= imageWidth) {
                    return false;
                }
                if (xMax >= imageWidth) {
                    xMax = imageWidth - 1;
                }
                for (uint32_t y=fromY;y<=toY;y++) {
                    for (x=xMin;x<xMax;x++) {
                        if (!addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a)) {
                            return false;
                        }
                    }
                }
            } else {
                int xTmpMin = static_cast<int>(fromX) - static_cast<int>((lineWidth-1)/2);
                if (xTmpMin + static_cast<int>(lineWidth+1) < 0) {
                    return false;
                }
                uint32_t xMin = xTmpMin < 0 ? 0 : static_cast<uint32_t>(xTmpMin);
                uint32_t xMax = static_cast<uint32_t>(xTmpMin + static_cast<int>(lineWidth)-1);
                uint32_t x;
                if (xMin >= imageWidth) {
                    return false;
                }
                bool ignoreLargerAlpha = false;
                if (xMax >= imageWidth) {
                    xMax = imageWidth - 1;
                    ignoreLargerAlpha = true;
                }
                for (uint32_t y=fromY;y<=toY;y++) {
                    for (x=xMin;x<xMax;x++) {
                        if (!addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a)) {
                            return false;
                        }
                    }
                    if (xTmpMin - 1 >= 0) {
                        if (!addColor(imageData, imageWidth, imageHeight, xTmpMin-1, y, r, g, b, a/2)) {
                            return false;
                        }
                    }
                    if (!ignoreLargerAlpha) {
                        if (!addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/2)) {
                            return false;
                        }
                    }
                }
            }
        } else {
             return drawLine(imageData, imageWidth, imageHeight, toX, toY, fromX, fromY, r, g, b, a, lineWidth);
        }
        return true;
    }
    if (fromY == toY) {
        if (lineWidth == 1) {
            for (uint32_t x=fromX;x<=toX;x++) {
                if (!setColor(imageData, imageWidth, imageHeight, x, fromY, r, g, b, a)) {
                    return false;
                }
            }
        } else if ((lineWidth % 2) == 1) {
            int yTmpMin = static_cast<int>(fromY) - static_cast<int>((lineWidth-1)/2);
            if (yTmpMin + static_cast<int>(lineWidth) < 0) {
                return false;
            }
            uint32_t yMin = yTmpMin < 0 ? 0 : static_cast<uint32_t>(yTmpMin);
            uint32_t yMax = static_cast<uint32_t>(yTmpMin + static_cast<int>(lineWidth));
            uint32_t y;
            if (yMin >= imageWidth) {
                return false;
            }
            if (yMax >= imageWidth) {
                yMax = imageWidth - 1;
            }
            for (uint32_t x=fromX;x<=toX;x++) {
                for (y=yMin;y<yMax;y++) {
                    if (!addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a)) {
                        return false;
                    }
                }
            }
        } else {
            int yTmpMin = static_cast<int>(fromY) - static_cast<int>((lineWidth-1)/2);
            if (yTmpMin + static_cast<int>(lineWidth) < 0) {
                return false;
            }
            uint32_t yMin = yTmpMin < 0 ? 0 : static_cast<uint32_t>(yTmpMin);
            uint32_t yMax = static_cast<uint32_t>(yTmpMin + static_cast<int>(lineWidth)-1);
            uint32_t y;
            if (yMin >= imageWidth) {
                return false;
            }
            bool ignoreLargerAlpha = false;
            if (yMax >= imageWidth) {
                yMax = imageWidth - 1;
                ignoreLargerAlpha = true;
            }
            for (uint32_t x=fromX;x<=toX;x++) {
                for (y=yMin;y<yMax;y++) {
                    if (!addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a)) {
                        return false;
                    }
                }
                if (yTmpMin - 1 >= 0) {
                    if (!addColor(imageData, imageWidth, imageHeight, x, yTmpMin-1, r, g, b, a/2)) {
                        return false;
                    }
                }
                if (!ignoreLargerAlpha) {
                    if (!addColor(imageData, imageWidth, imageHeight, x, y, r, g, b, a/2)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    if (toY > fromY && toY-fromY == toX-fromX) {
        // PI/2 angle
        if (lineWidth == 1) {
            for (uint32_t x=fromX;x<=toX;x++) {
                if (!setColor(imageData, imageWidth, imageHeight, x, fromY+(x-fromX), r, g, b, a)) {
                    return false;
                }
                if (x==fromX) {
                    continue;
                }
                if (!setColor(imageData, imageWidth, imageHeight, x-1, fromY+(x-fromX), r, g, b, a/2)) {
                    return false;
                }
                if (!setColor(imageData, imageWidth, imageHeight, x, fromY+(x-fromX)+1, r, g, b, a/2)) {
                    return false;
                }

            }
        }
        return true;
    }

    if (fromY < toY) {
        if (toY-fromY < toX-fromX) {
            for (uint32_t x=fromX;x<=toX;x++) {
                if (!setColor(imageData, imageWidth, imageHeight, x, fromY+(x-fromX)*(toY-fromY)/(toX-fromX), r, g, b, a)) {
                    return false;
                }
            }
            return true;
        }

        for (uint32_t y=fromY;y<=toY;y++) {
            if (!setColor(imageData, imageWidth, imageHeight, fromX+(y-fromY)*(toX-fromX)/(toY-fromY), y, r, g, b, a)) {
                return false;
            }
        }
    } else {
        if (fromY-toY < toX-fromX) {
            for (uint32_t x=fromX;x<=toX;x++) {
                if (!setColor(imageData, imageWidth, imageHeight, x, static_cast<uint32_t>(static_cast<int>(fromY)+static_cast<int>(x-fromX)*(static_cast<int>(toY)-static_cast<int>(fromY))/static_cast<int>(toX-fromX)), r, g, b, a)) {
                    return false;
                }
            }
            return true;
        }

        for (uint32_t y=toY;y<=fromY;y++) {
            if (!setColor(imageData, imageWidth, imageHeight, static_cast<uint32_t>(static_cast<int>(fromX)+static_cast<int>(y-fromY)*(static_cast<int>(toX)-static_cast<int>(fromX))/static_cast<int>(static_cast<int>(toY)-static_cast<int>(fromY))), y, r, g, b, a)) {
                return false;
            }
        }
    }
    return true;
}

bool TgImageDraw::setColor(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t x, const uint32_t y,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    if (x >= imageWidth) {
        return false;
    }
    if (y >= imageHeight) {
        return false;
    }
    size_t pos = (y*imageWidth+x)*4;
    imageData[pos] = r;
    imageData[pos+1] = g;
    imageData[pos+2] = b;
    imageData[pos+3] = a;
    return true;
}

bool TgImageDraw::addColor(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t x, const uint32_t y,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    if (x >= imageWidth) {
        return false;
    }
    if (y >= imageHeight) {
        return false;
    }
    size_t pos = (y*imageWidth+x)*4;
    uint8_t rNew, gNew, bNew, aNew;

    TgColor::addColor(imageData[pos], imageData[pos+1], imageData[pos+2], imageData[pos+3],
             r, g, b, a,
             rNew, gNew, bNew, aNew);

    imageData[pos] = rNew;
    imageData[pos+1] = gNew;
    imageData[pos+2] = bNew;
    imageData[pos+3] = aNew;
    return true;
}
