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

class TgFontText;

/*struct TgFontTextInfo
{
    std::vector<TgFontInfo *>m_listFontInfo;
    float m_fontHeight;
    std::vector<TgCharPosition>m_listCharPosition;
    std::vector<TgFontText *>m_listTextForFont;
};
*/
class TgFontTextGenerator
{
public:
    static TgFontText *generateFontTextInfo(const std::string &utf8text, const std::string &mainFontFile);
};

#endif // TG_FONT_TEXT_GENERATOR_H
