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
#include "tg_font_text.h"

struct TgTextCharacter
{
    uint32_t m_character;
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
};

class TgFontTextGenerator
{
public:
    static void getCharacters(const std::vector<TgTextFieldText> &listText, std::vector<uint32_t> &listCharacter);
    static void getCharacters(const std::vector<TgTextFieldText> &listText, std::vector<TgTextCharacter> &listCharacter);
    static TgFontText *generateFontTextInfo(const std::vector<TgTextFieldText> &listText, const std::string &mainFontFile);
    static bool changeTextColor(const std::vector<TgTextFieldText> &listText, TgFontText *fontText);
    static std::string generateSingleLineText(const std::vector<TgTextFieldText> &listText);
    static std::vector<TgFontTextCharacterInfo> generateCharacterList(const std::vector<TgTextFieldText> &listText, const std::vector<std::string> &listFontFiles);
;
};

#endif // TG_FONT_TEXT_GENERATOR_H
