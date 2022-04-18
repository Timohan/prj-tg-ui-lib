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
#include <prj-ttf-reader.h>
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
 * \return true on success
 */
bool TgCharacterPositions::generateTextCharacterPositioning(TgFontText *fontText)
{
    TG_FUNCTION_BEGIN();
    size_t i, c = fontText->getCharacterCount();
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

    fontText->setTextWidth(0);

    for (i=0;i<c;i++) {
        characterInfo = fontText->getCharacter(i);
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

        characterInfo->positionLeftX = positionLeftX;

        infoCharacterIndex = getCharacterIndex(fontInfo, characterInfo->m_character);
        characterInfo->m_characterInFontInfoIndex = infoCharacterIndex;
        if (!left_glyph) {
            fontText->setVisibleTopY(fontInfo->m_listTopPositionY.at(infoCharacterIndex));
            fontText->setVisibleBottomY(fontInfo->m_listBottomPositionY.at(infoCharacterIndex));
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
        fontText->setTextWidth(positionLeftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x));
    }

    TG_FUNCTION_END();
    return true;
}
