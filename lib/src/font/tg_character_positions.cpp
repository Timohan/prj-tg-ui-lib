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
#include "cache/tg_font_glyph_cache_data.h"

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
 * \brief TgCharacterPositions::calculateAdvance
 *
 * calculate advance between two glyphs
 *
 * \param left_glyph [in] left glyph
 * \param right_glyph [in] right glyph
 * \param right_data [in] right glyph data
 * \return advance
 */
float TgCharacterPositions::calculateAdvance(const prj_ttf_reader_glyph_data_t *left_glyph, const prj_ttf_reader_glyph_data_t *right_glyph, const prj_ttf_reader_data_t *right_data)
{
    int32_t left_advance_x, left_bearing, right_bearing;
    left_advance_x = static_cast<int32_t>(left_glyph->image_pixel_advance_x + 0.5f);
    left_bearing = static_cast<int32_t>(left_glyph->image_pixel_bearing);
    if (left_glyph->image_pixel_bearing < 0 && left_glyph->image_pixel_bearing > -1) {
        left_bearing = -1;
    }
    left_advance_x -= left_bearing;
    left_advance_x -= left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x;
    right_bearing = static_cast<int32_t>(right_glyph->image_pixel_bearing);
    if (right_glyph->image_pixel_bearing < 0 && right_glyph->image_pixel_bearing > -1) {
        right_bearing = -1;
    }

    float kerning = prj_ttf_reader_get_kerning(left_glyph->character, right_glyph->character, right_data) + static_cast<float>(right_bearing + left_advance_x);

    float ret = static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
    ret += kerning;
    return ret;
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
    uint32_t currentLine = 1;
    bool firstCharacterAdded = false;
    float textLinesWidth = 0;
    float textWidthToSet = 0;
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
            if (textWidthToSet < positionLeftX) {
                textWidthToSet = positionLeftX;
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
            positionLeftX += calculateAdvance(left_glyph, glyph, fontInfo->m_data);
        }
        if (wordWrap == TgTextFieldWordWrap::WordWrapOn
            && isOverTheLine(currentLine, positionLeftX, glyph, maxLineCount, maxLineWidth)) {
            if (previousSpaceIndex != c && previousSpaceIndex > 0) {
                leftCharacterInfo = fontText->getCharacter(previousSpaceIndex);
                fontText->setListLinesWidth(leftCharacterInfo->m_lineNumber,
                    leftCharacterInfo->positionLeftX);
                if (textWidthToSet < leftCharacterInfo->positionLeftX) {
                    textWidthToSet = leftCharacterInfo->positionLeftX;
                }
                i = previousSpaceIndex;
                positionLeftX = 0;
                currentLine++;
                left_glyph = nullptr;
                continue;
            }
            if (left_glyph) {
                textLinesWidth += positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
                if (textWidthToSet < textLinesWidth) {
                    textWidthToSet = textLinesWidth;
                }
            }
            positionLeftX = 0;
            currentLine++;
        } else if (wordWrap == TgTextFieldWordWrap::WordWrapBounded
                    && isOverTheLine(currentLine, positionLeftX, glyph, maxLineCount, maxLineWidth)) {
            if (left_glyph) {
                textLinesWidth += positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
                if (textWidthToSet < textLinesWidth) {
                    textWidthToSet = textLinesWidth;
                }
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
        if (textWidthToSet < textLinesWidth + positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x)) {
            textWidthToSet = textLinesWidth + positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
        }
        fontText->setTextWidth(textWidthToSet);
    }
    fontText->setAllLineCount(currentLine);

    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgCharacterPositions::calculateTextWidthHeight
 *
 * Calculate text width, height and all draw required height
 * by using glyph cache data if required (non rendering)
 *
 * \param listFontInfo
 * \param listCharacter
 * \param fontText [in] info
 * \param maxLineCount [in] max line count, 0 unlimited number of lines
 * \param maxLineWidth [in] max line width
 * \param wordWrap
 * \param allowBreakLineGoOverMaxLine
 * \param mostTextWidth [out]
 * \param mostTextHeight [out]
 * \param allDrawTextHeight [out]
 * \return true on success
 */
bool TgCharacterPositions::calculateTextWidthHeight(std::vector<TgFontInfoData *> &listFontInfo,
                                                            std::vector<TgFontTextCharacterInfo> &listCharacter,
                                                            const uint32_t maxLineCount, const float maxLineWidth,
                                                            const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine,
                                                            float &mostTextWidth, float &mostTextHeight, float &allDrawTextHeight)
{
    TG_FUNCTION_BEGIN();
    size_t i, i2, c = listCharacter.size();
    TgFontTextCharacterInfo *characterInfo;
    TgFontTextCharacterInfo *leftCharacterInfo = nullptr;
    TgFontInfoData *fontInfo;
    TgFontInfoData *leftFontInfo = nullptr;
    const prj_ttf_reader_glyph_data_t *glyph;
    const prj_ttf_reader_glyph_data_t *left_glyph = nullptr;
    float positionLeftX = 0;
    size_t infoCharacterIndex = 0;
    uint32_t currentLine = 1;
    bool firstCharacterAdded = false;
    float textLinesWidth = 0;
    float textWidthToSet = 0;
    size_t previousSpaceIndex = c;
    mostTextWidth = 0;
    mostTextHeight = 0;
    allDrawTextHeight = 0;
    std::vector<float> listLineWidth;

    for (i=0;i<c;i++) {
        characterInfo = &listCharacter[i];
        characterInfo->m_draw = true;
        if (characterInfo->m_character == '\n') {
            characterInfo->m_draw = false;
            if (allowBreakLineGoOverMaxLine
                && maxLineCount != 0
                && maxLineCount == currentLine) {
                for (i2=i;i2<c;i2++) {
                    listCharacter[i2].m_lineNumber = currentLine - 1;
                }
                break;
            }
            if (textWidthToSet < positionLeftX) {
                textWidthToSet = positionLeftX;
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
        fontInfo = listFontInfo[i];
        if (!fontInfo) {
            continue;
        }

        glyph = prj_ttf_reader_get_character_glyph_data(characterInfo->m_character, fontInfo->m_data);
        if (!glyph) {
            continue;
        }

        if (i && left_glyph) {
            left_glyph = prj_ttf_reader_get_character_glyph_data(leftCharacterInfo->m_character, leftFontInfo->m_data);
            positionLeftX += calculateAdvance(left_glyph, glyph, fontInfo->m_data);
        }
        if (wordWrap == TgTextFieldWordWrap::WordWrapOn
            && isOverTheLine(currentLine, positionLeftX, glyph, maxLineCount, maxLineWidth)) {
            if (previousSpaceIndex != c && previousSpaceIndex > 0) {
                leftCharacterInfo = &listCharacter[previousSpaceIndex];
                TgFontText::setListLinesWidth(listLineWidth, leftCharacterInfo->m_lineNumber,
                    leftCharacterInfo->positionLeftX);
                if (textWidthToSet < leftCharacterInfo->positionLeftX) {
                    textWidthToSet = leftCharacterInfo->positionLeftX;
                }
                i = previousSpaceIndex;
                positionLeftX = 0;
                currentLine++;
                left_glyph = nullptr;
                continue;
            }
            if (left_glyph) {
                textLinesWidth += positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
                if (textWidthToSet < textLinesWidth) {
                    textWidthToSet = textLinesWidth;
                }
            }
            positionLeftX = 0;
            currentLine++;
        } else if (wordWrap == TgTextFieldWordWrap::WordWrapBounded
                    && isOverTheLine(currentLine, positionLeftX, glyph, maxLineCount, maxLineWidth)) {
            if (left_glyph) {
                textLinesWidth += positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
                if (textWidthToSet < textLinesWidth) {
                    textWidthToSet = textLinesWidth;
                }
            }
            positionLeftX = 0;
            currentLine++;
        }

        if (characterInfo->m_character == ' ') {
            previousSpaceIndex = i;
        }

        characterInfo->positionLeftX = positionLeftX;
        characterInfo->m_lineNumber = currentLine - 1;
        TgFontText::setListLinesWidth(listLineWidth, characterInfo->m_lineNumber, positionLeftX + static_cast<float>(glyph->image_pixel_right_x - glyph->image_pixel_left_x));

        infoCharacterIndex = getCharacterIndex(fontInfo, characterInfo->m_character);
        characterInfo->m_characterInFontInfoIndex = infoCharacterIndex;
        if (!firstCharacterAdded) {
            firstCharacterAdded = true;
        }

        left_glyph = glyph;
        leftCharacterInfo = characterInfo;
        leftFontInfo = fontInfo;
    }

    if (left_glyph) {
        if (textWidthToSet < textLinesWidth + positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x)) {
            textWidthToSet = textLinesWidth + positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
        }
        mostTextWidth = textWidthToSet;
    }
    mostTextHeight = TgFontGlyphCacheData::getFontHeight(listFontInfo);
    allDrawTextHeight = TgFontGlyphCacheData::getAllDrawTextHeight(currentLine, listFontInfo);
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgCharacterPositions::calculateTextWidth
 *
 * calculate the text width
 *
 * \param fontText [in] font text
 * \return width
 */
float TgCharacterPositions::calculateTextWidth(TgFontText *fontText)
{
    size_t i, c = fontText->getCharacterCount();
    float advanceX;
    float ret = 0;
    const prj_ttf_reader_glyph_data_t *right_glyph, *left_glyph;
    for (i=0;i<c;i++) {
        TgFontTextCharacterInfo *characterInfo = fontText->getCharacter(i);
        TgFontInfo *fontInfo = fontText->getFontInfo(i);
        right_glyph = prj_ttf_reader_get_character_glyph_data(characterInfo->m_character, fontInfo->m_data);

        if (i > 0) {
            advanceX = TgCharacterPositions::calculateAdvance(left_glyph, right_glyph, fontInfo->m_data);
            ret += advanceX;
        }
        if (i+1 == c) {
            ret += static_cast<float>(right_glyph->image_pixel_right_x - right_glyph->image_pixel_left_x);
        }
        left_glyph = right_glyph;
    }
    return ret;
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
size_t TgCharacterPositions::getCharacterIndex(const TgFontInfoData *newInfo, uint32_t character)
{
    for (size_t i=0;i<newInfo->m_listCharacter.size();i++) {
        if (newInfo->m_listCharacter.at(i) == character) {
            return i;
        }
    }
    return 0;
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