/*!
 * \file
 * \brief file tg_image_draw.h
 *
 * image draw functionalities
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#ifndef TG_IMAGE_DRAW_H
#define TG_IMAGE_DRAW_H

#include <stdint.h>

class TgImageDraw
{
public:
    static bool setColor(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t x, const uint32_t y,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
};

#endif // TG_IMAGE_DRAW_H
