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
    static bool drawLine(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                         const uint32_t fromX, const uint32_t fromY,
                         const uint32_t toX, const uint32_t toY,
                         const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
    static bool setColor(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t x, const uint32_t y,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
    static bool addColor(uint8_t *imageData, const uint32_t imageWidth, const uint32_t imageHeight,
                           const uint32_t x, const uint32_t y,
                           const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

private:
    static void addColor(const uint8_t rBackground, const uint8_t gBackground, const uint8_t bBackground, const uint8_t aBackground,
                         const uint8_t rAdd, const uint8_t gAdd, const uint8_t bAdd, const uint8_t aAdd,
                         uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);
};

#endif // TG_IMAGE_DRAW_H
