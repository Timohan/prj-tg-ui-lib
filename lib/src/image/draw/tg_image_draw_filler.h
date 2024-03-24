/*!
 * \file
 * \brief file tg_image_draw_filler.h
 *
 * Image draw filler, temporary store for the image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#ifndef TG_IMAGE_DRAW_FILLER_H
#define TG_IMAGE_DRAW_FILLER_H

#include <stdint.h>
#include <vector>

class TgImageDrawFiller
{
public:
    ~TgImageDrawFiller();
    void init(const uint32_t &imageWidth, const uint32_t &imageHeight);
    void set(const uint32_t &x, const uint32_t &y, const uint8_t &value);
    uint8_t get(const uint32_t &x, const uint32_t &y);

private:
    uint8_t *m_imageData = nullptr;
    uint32_t m_imageDataSize = 0;
    uint32_t m_imageWidth;
    uint32_t m_imageHeight;
};

#endif // TG_IMAGE_DRAW_FILLER_H
