/*!
 * \file
 * \brief file tg_font_text_generator.h
 *
 * font text generator generates the text glyphs
 * and rendering classes for labels
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_TEXT_GENERATOR_H
#define TG_FONT_TEXT_GENERATOR_H

#include <vector>
#include <string>

#include "tg_character_positions.h"
#include "../item2d/tg_textfield.h"

class TgFontTextGenerator
{
public:
    static TgFontText *generateFontTextInfo(const std::vector<TgTextFieldText> &listText, const std::string &mainFontFile);
    static bool changeTextColor(const std::vector<TgTextFieldText> &listText, TgFontText *fontText);

private:
    static std::string generateSingleLineText(const std::vector<TgTextFieldText> &listText);
};

#endif // TG_FONT_TEXT_GENERATOR_H
