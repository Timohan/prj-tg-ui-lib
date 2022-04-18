/*!
 * \file
 * \brief file tg_character_positions.h
 *
 * set character positions for text
 * and fills them into TgFontText's TgFontInfo
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
class TgFontText;

class TgCharacterPositions
{
public:
    explicit TgCharacterPositions();
    ~TgCharacterPositions();
    static bool generateTextCharacterPositioning(TgFontText *fontText);

private:
    static size_t getCharacterIndex(const TgFontInfo *newInfo, uint32_t character);

};

#endif // TG_CHARACTER_POSITIONS_H
