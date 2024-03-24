/*!
 * \file
 * \brief file tg_color.cpp
 *
 * Math functions for color, for example, mix two colors
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_color.h"

/*!
 * \brief TgColor::addColor
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
void TgColor::addColor(const uint8_t rBackground, const uint8_t gBackground, const uint8_t bBackground, const uint8_t aBackground,
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