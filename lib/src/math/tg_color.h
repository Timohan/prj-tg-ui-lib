/*!
 * \file
 * \brief file tg_color.h
 *
 * Math functions for color, for example, mix two colors
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_COLOR_H
#define TG_COLOR_H

#include <stdint.h>

class TgColor
{
public:
    static void addColor(const uint8_t rBackground, const uint8_t gBackground, const uint8_t bBackground, const uint8_t aBackground,
                           const uint8_t rAdd, const uint8_t gAdd, const uint8_t bAdd, const uint8_t aAdd,
                           uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);


};

#endif // TG_COLOR_H
