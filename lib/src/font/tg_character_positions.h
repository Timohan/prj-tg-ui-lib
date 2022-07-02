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
#include <prj-ttf-reader.h>
#include "../item2d/tg_textfield.h"

struct TgFontInfo;
class TgFontText;

class TgCharacterPositions
{
public:
    explicit TgCharacterPositions();
    ~TgCharacterPositions();
    static bool generateTextCharacterPositioning(TgFontText *fontText, const uint32_t maxLineCount, const float maxLineWidth, const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine);

private:
    static size_t getCharacterIndex(const TgFontInfo *newInfo, uint32_t character);
    static bool isOverTheLine(const uint32_t lineNumber, const float positionLeftX, const prj_ttf_reader_glyph_data_t *glyph, const uint32_t maxLineCount, const float maxLineWidth);
};

#endif // TG_CHARACTER_POSITIONS_H
