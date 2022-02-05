/*!
 * \file
 * \brief file tg_character_positions.h
 *
 * font words
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_CHARACTER_POSITIONS_H
#define TG_CHARACTER_POSITIONS_H

#include <vector>
#include <string>

struct TgFontInfo;

struct TgCharPosition
{
    std::string m_utf8text;
    std::vector<uint32_t>m_listCharacterIndex;
    std::vector<float>m_listTransformX;
    std::vector<size_t>m_listCharacterInFontInfoIndex;
    float m_textWidth;
    float m_visibleTopY;
    float m_visibleBottomY;
};

class TgCharacterPositions
{
public:
    explicit TgCharacterPositions();
    ~TgCharacterPositions();
    static bool generateTextCharacterPositioning(TgFontInfo *newInfo, const char *text, TgCharPosition &positions);

private:
    static size_t getCharacterIndex(const TgFontInfo *newInfo, uint32_t character);
    static void clear(TgCharPosition &positions);

};

#endif // TG_CHARACTER_POSITIONS_H
