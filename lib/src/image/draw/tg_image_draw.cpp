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

bool TgImageDraw::drawLine(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t fromX, const uint32_t fromY,
                           const uint32_t toX, const uint32_t toY,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    if (fromX > toX) {
         return drawLine(imageData, imageWidth, imageHeight, toX, toY, fromX, fromY, r, g, b, a);
    }

    if (fromX == toX && fromY == toY) {
        return setColor(imageData, imageWidth, imageHeight, fromX, fromY, r, g, b, a);
    }

    if (fromX == toX) {
        if (fromY <= toY) {
            for (uint32_t y=fromY;y<=toY;y++) {
                if (!setColor(imageData, imageWidth, imageHeight, fromX, y, r, g, b, a)) {
                    return false;
                }
            }
        } else {
            for (uint32_t y=toY;y<=fromY;y++) {
                if (!setColor(imageData, imageWidth, imageHeight, fromX, y, r, g, b, a)) {
                    return false;
                }
            }
        }
        return true;
    }
    if (fromY == toY) {
        for (uint32_t x=fromX;x<=toX;x++) {
            if (!setColor(imageData, imageWidth, imageHeight, x, fromY, r, g, b, a)) {
                return false;
            }
        }
        return true;
    }

    if (toY > fromY && toY-fromY == toX-fromX) {
        for (uint32_t x=fromX;x<=toX;x++) {
            if (!setColor(imageData, imageWidth, imageHeight, x, fromY+(x-fromX), r, g, b, a)) {
                return false;
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

    addColor(imageData[pos], imageData[pos+1], imageData[pos+2], imageData[pos+3],
             r, g, b, a,
             rNew, gNew, bNew, aNew);

    imageData[pos] = rNew;
    imageData[pos+1] = gNew;
    imageData[pos+2] = bNew;
    imageData[pos+3] = aNew;
    return true;
}

/*!
 * \brief TgImageDraw::addColor
 *
 * math to add new color on background
 *
 * \param rBackground current color r
 * \param gBackground current color g
 * \param bBackground current color b
 * \param aBackground current color a
 * \param rAdd r color to be on
 * \param gAdd r color to be on
 * \param bAdd g color to be on
 * \param aAdd b color to be on
 * \param r [out] new mixed color r
 * \param g [out] new mixed color g
 * \param b [out] new mixed color b
 * \param a [out] new mixed color a
 */
void TgImageDraw::addColor(const uint8_t rBackground, const uint8_t gBackground, const uint8_t bBackground, const uint8_t aBackground,
                           const uint8_t rAdd, const uint8_t gAdd, const uint8_t bAdd, const uint8_t aAdd,
                           uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a)
{
    double ra = static_cast<double>(rAdd)/255;
    double ga = static_cast<double>(gAdd)/255;
    double ba = static_cast<double>(bAdd)/255;
    double aa = static_cast<double>(aAdd)/255;

    double rb = static_cast<double>(rBackground)/255;
    double gb = static_cast<double>(gBackground)/255;
    double bb = static_cast<double>(bBackground)/255;
    double ab = static_cast<double>(aBackground)/255;

    double aValue = (1 - aa)*ab + aa;
    r = static_cast<uint8_t>(255.0*(((1 - aa)*ab*rb + aa*ra) / aValue));
    g = static_cast<uint8_t>(255.0*(((1 - aa)*ab*gb + aa*ga) / aValue));
    b = static_cast<uint8_t>(255.0*(((1 - aa)*ab*bb + aa*ba) / aValue));
    a = static_cast<uint8_t>(255.0*aValue);
}