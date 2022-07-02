/*!
 * \file
 * \brief file tg_character_positions.cpp
 *
 * set character positions for text
 * and fills them into TgFontText's TgFontInfo
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_character_positions.h"
#include "../global/tg_global_log.h"
#include "cache/tg_font_glyph_cache.h"
#include "tg_font_text_generator.h"
#include <cstring>
#include "tg_font_text.h"

TgCharacterPositions::TgCharacterPositions()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgCharacterPositions::~TgCharacterPositions()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgCharacterPositions::getCharacterIndex
 *
 * get chraracter index from newInfo's m_listCharacter
 *
 * \param newInfo [in] info
 * \param character character
 * \return character index to find from newInfo->m_listCharacter
 */
size_t TgCharacterPositions::getCharacterIndex(const TgFontInfo *newInfo, uint32_t character)
{
    for (size_t i=0;i<newInfo->m_listCharacter.size();i++) {
        if (newInfo->m_listCharacter.at(i) == character) {
            return i;
        }
    }
    return 0;
}

/*!
 * \brief TgCharacterPositions::generateTextCharacterPositioning
 *
 * generate text vertices for the text
 *
 * \param fontText [in] info
 * \param maxLineCount [in] max line count, 0 unlimited number of lines
 * \param maxLineWidth [in] max line width
 * \param wordWrap
 * \param allowBreakLineGoOverMaxLine
 * \return true on success
 */
bool TgCharacterPositions::generateTextCharacterPositioning(TgFontText *fontText, const uint32_t maxLineCount, const float maxLineWidth,
                                                            const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine)
{
    TG_FUNCTION_BEGIN();
    size_t i, i2, c = fontText->getCharacterCount();
    TgFontTextCharacterInfo *characterInfo;
    TgFontTextCharacterInfo *leftCharacterInfo = nullptr;
    TgFontInfo *fontInfo;
    TgFontInfo *leftFontInfo = nullptr;
    const prj_ttf_reader_glyph_data_t *glyph;
    const prj_ttf_reader_glyph_data_t *left_glyph = nullptr;
    float positionLeftX = 0;
    size_t infoCharacterIndex = 0;
    float kerning = 0;
    int32_t left_advance_x, left_bearing, right_bearing;
    uint32_t currentLine = 1;
    bool firstCharacterAdded = false;
    float textLinesWidth = 0;
    size_t previousSpaceIndex = c;
    fontText->setTextWidth(0);
    fontText->clearListLinesWidth();

    for (i=0;i<c;i++) {
        characterInfo = fontText->getCharacter(i);
        characterInfo->m_draw = true;
        if (characterInfo->m_character == '\n') {
            characterInfo->m_draw = false;
            if (allowBreakLineGoOverMaxLine
                && maxLineCount != 0
                && maxLineCount == currentLine) {
                for (i2=i;i2<c;i2++) {
                    fontText->getCharacter(i2)->m_draw = false;
                    fontText->getCharacter(i2)->m_lineNumber = currentLine - 1;
                }
                break;
            }
            if (maxLineCount == 0 || maxLineCount > currentLine) {
                previousSpaceIndex = c;
                currentLine++;
                positionLeftX = 0;
                left_glyph = nullptr;
            }
            characterInfo->positionLeftX = 0;
            characterInfo->m_lineNumber = currentLine - 1;
            continue;
        }

        if (characterInfo->m_fontFileNameIndex == -1) {
            continue;
        }
        fontInfo = fontText->getFontInfo(i);
        if (!fontInfo) {
            continue;
        }

        glyph = prj_ttf_reader_get_character_glyph_data(characterInfo->m_character, fontInfo->m_data);
        if (!glyph) {
            continue;
        }

        if (i && left_glyph) {
            left_glyph = prj_ttf_reader_get_character_glyph_data(leftCharacterInfo->m_character, leftFontInfo->m_data);
            left_advance_x = static_cast<int32_t>(left_glyph->image_pixel_advance_x + 0.5f);
            left_bearing = static_cast<int32_t>(left_glyph->image_pixel_bearing);
            if (left_glyph->image_pixel_bearing < 0 && left_glyph->image_pixel_bearing > -1) {
                left_bearing = -1;
            }
            left_advance_x -= left_bearing;
            left_advance_x -= left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x;
            right_bearing = static_cast<int32_t>(glyph->image_pixel_bearing);
            if (glyph->image_pixel_bearing < 0 && glyph->image_pixel_bearing > -1) {
                right_bearing = -1;
            }

            kerning = prj_ttf_reader_get_kerning(leftCharacterInfo->m_character, characterInfo->m_character, fontInfo->m_data) + static_cast<float>(right_bearing + left_advance_x);

            positionLeftX += static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
            positionLeftX += kerning;
        }
        if (wordWrap == TgTextFieldWordWrap::WordWrapOn
            && isOverTheLine(currentLine, positionLeftX, glyph, maxLineCount, maxLineWidth)) {
            if (previousSpaceIndex != c && previousSpaceIndex > 0) {
                leftCharacterInfo = fontText->getCharacter(previousSpaceIndex);
                fontText->setListLinesWidth(leftCharacterInfo->m_lineNumber,
                    leftCharacterInfo->positionLeftX);
                i = previousSpaceIndex;
                positionLeftX = 0;
                currentLine++;
                left_glyph = nullptr;
                continue;
            }
            if (left_glyph) {
                textLinesWidth += positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
            }
            positionLeftX = 0;
            currentLine++;
        } else if (wordWrap == TgTextFieldWordWrap::WordWrapBounded
                    && isOverTheLine(currentLine, positionLeftX, glyph, maxLineCount, maxLineWidth)) {
            if (left_glyph) {
                textLinesWidth += positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
            }
            positionLeftX = 0;
            currentLine++;
        }

        if (characterInfo->m_character == ' ') {
            previousSpaceIndex = i;
        }

        characterInfo->positionLeftX = positionLeftX;
        characterInfo->m_lineNumber = currentLine - 1;
        fontText->setListLinesWidth(characterInfo->m_lineNumber, positionLeftX + static_cast<float>(glyph->image_pixel_right_x - glyph->image_pixel_left_x));

        infoCharacterIndex = getCharacterIndex(fontInfo, characterInfo->m_character);
        characterInfo->m_characterInFontInfoIndex = infoCharacterIndex;
        if (!firstCharacterAdded) {
            fontText->setVisibleTopY(fontInfo->m_listTopPositionY.at(infoCharacterIndex));
            fontText->setVisibleBottomY(fontInfo->m_listBottomPositionY.at(infoCharacterIndex));
            firstCharacterAdded = true;
        } else {
            if (fontText->getVisibleTopY() > fontInfo->m_listTopPositionY.at(infoCharacterIndex)) {
                fontText->setVisibleTopY(fontInfo->m_listTopPositionY.at(infoCharacterIndex));
            }
            if (fontText->getVisibleBottomY() < fontInfo->m_listBottomPositionY.at(infoCharacterIndex)) {
                fontText->setVisibleBottomY(fontInfo->m_listBottomPositionY.at(infoCharacterIndex));
            }
        }

        left_glyph = glyph;
        leftCharacterInfo = characterInfo;
        leftFontInfo = fontInfo;
    }

    if (left_glyph) {
        fontText->setTextWidth(textLinesWidth + positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x));
    }
    fontText->setAllLineCount(currentLine);

    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgCharacterPositions::isOverTheLine
 *
 * checks if the current glyph position is over the line
 *
 * \param lineNumber [in] current line number, 1 == first line
 * \param positionLeftX [in] current glyph's left x
 * \param glyph [in] current glyph
 * \param maxLineCount max line count
 * \param maxLineWidth max line width
 * \return true - if glyph position goes over the line
 */
bool TgCharacterPositions::isOverTheLine(const uint32_t lineNumber, const float positionLeftX, const prj_ttf_reader_glyph_data_t *glyph,
                                         const uint32_t maxLineCount, const float maxLineWidth)
{
    if (positionLeftX + static_cast<float>(glyph->image_pixel_right_x - glyph->image_pixel_left_x) < maxLineWidth) {
        return false;
    }

    if (maxLineCount == lineNumber || lineNumber == UINT32_MAX) {
        return false;
    }

    return true;
}