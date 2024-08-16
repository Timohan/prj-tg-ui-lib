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
