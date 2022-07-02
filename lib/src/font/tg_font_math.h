/*!
 * \file
 * \brief file tg_font_math.h
 *
 * This calculates font height and width
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_MATH_H
#define TG_FONT_MATH_H

#include <vector>
#include <string>
#include "../item2d/tg_textfield.h"

class TgFontMath
{
public:
    TgFontMath();
    static bool getFontWidthHeight(const std::vector<TgTextFieldText> &listText, float fontSize, const std::string &mainFontFile,
                                  float &textWidth, float &textHeight, float &allDrawTextHeight, const uint32_t maxLineCount, const float maxLineWidth,
                                  const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine);
private:
};

#endif // TG_FONT_MATH_H
