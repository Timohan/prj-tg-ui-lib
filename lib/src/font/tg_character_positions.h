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
#include "tg_font_text.h"

struct TgFontInfo;
class TgFontText;
struct TgFontInfoData;

class TgCharacterPositions
{
public:
    explicit TgCharacterPositions();
    ~TgCharacterPositions();
    static bool generateTextCharacterPositioning(TgFontText *fontText, const uint32_t maxLineCount, const float maxLineWidth, const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine);
    static bool calculateTextWidthHeight(std::vector<TgFontInfoData *> &listFontInfo,
                                                            std::vector<TgFontTextCharacterInfo> &listCharacter,
                                                            const uint32_t maxLineCount, const float maxLineWidth,
                                                            const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine,
                                                            float &mostTextWidth, float &mostTextHeight, float &allDrawTextHeight);
private:
    static size_t getCharacterIndex(const TgFontInfo *newInfo, uint32_t character);
    static size_t getCharacterIndex(const TgFontInfoData *newInfo, uint32_t character);
    static bool isOverTheLine(const uint32_t lineNumber, const float positionLeftX, const prj_ttf_reader_glyph_data_t *glyph, const uint32_t maxLineCount, const float maxLineWidth);
};

#endif // TG_CHARACTER_POSITIONS_H
