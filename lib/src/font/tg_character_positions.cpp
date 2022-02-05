/*!
 * \file
 * \brief file tg_character_positions.cpp
 *
 * font words
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_character_positions.h"
#include "../global/tg_global_log.h"
#include "tg_font_cache.h"
#include <cstring>
#include <prj-ttf-reader.h>

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
 * \brief TgCharacterPositions::clear
 *
 * clears the positions
 *
 * \param positions [in/out] position that needs to be cleared
 */
void TgCharacterPositions::clear(TgCharPosition &positions)
{
    positions.m_utf8text = "";
    positions.m_listCharacterIndex.clear();
    positions.m_listTransformX.clear();
    positions.m_listCharacterInFontInfoIndex.clear();
    positions.m_textWidth = 0;
    positions.m_visibleTopY = 0;
    positions.m_visibleBottomY = 0;
}

/*!
 * \brief TgCharacterPositions::generateTextCharacterPositioning
 *
 * generate text vertices for the text
 *
 * \param newInfo [in] info
 * \param text contains the text to render
 * \param positions [in/out] fills the character positions here
 * \return true on success
 */
bool TgCharacterPositions::generateTextCharacterPositioning(TgFontInfo *newInfo, const char *text, TgCharPosition &positions)
{
    TG_FUNCTION_BEGIN();
    clear(positions);

    uint32_t *list_characters = nullptr, list_characters_size = 0, i;
    if (!text || strlen(text) == 0) {
        TG_FUNCTION_END();
        return false;
    }

    if (prj_ttf_reader_get_characters(text, &list_characters, &list_characters_size)
            || !list_characters_size) {
        free(list_characters);
        TG_ERROR_LOG("prj_ttf_reader_get_characters failed");
        TG_FUNCTION_END();
        return false;
    }
    size_t infoCharacterIndex = 0;
    const prj_ttf_reader_glyph_data_t *glyph;
    const prj_ttf_reader_glyph_data_t *left_glyph = nullptr;
    float leftX = 0;
    int32_t left_advance_x, left_bearing;
    int32_t right_bearing;
    float kerning = 0;
    for (i=0;i<list_characters_size;i++) {
        glyph = prj_ttf_reader_get_character_glyph_data(list_characters[i], newInfo->m_data);
        if (!glyph) {
            continue;
        }

        if (i && left_glyph) {
            left_glyph = prj_ttf_reader_get_character_glyph_data(list_characters[i-1], newInfo->m_data);
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
            kerning = prj_ttf_reader_get_kerning(list_characters[i-1], list_characters[i], newInfo->m_data) + static_cast<float>(right_bearing + left_advance_x);

            leftX += static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
            leftX += kerning;
        }

        positions.m_listTransformX.push_back(leftX);
        positions.m_listCharacterIndex.push_back(list_characters[i]);
        infoCharacterIndex = getCharacterIndex(newInfo, list_characters[i]);
        positions.m_listCharacterInFontInfoIndex.push_back(infoCharacterIndex);
        if (!left_glyph) {
            positions.m_visibleTopY = newInfo->m_listTopPositionY.at(infoCharacterIndex);
            positions.m_visibleBottomY = newInfo->m_listBottomPositionY.at(infoCharacterIndex);
        } else {
            if (positions.m_visibleTopY > newInfo->m_listTopPositionY.at(infoCharacterIndex)) {
                positions.m_visibleTopY = newInfo->m_listTopPositionY.at(infoCharacterIndex);
            }
            if (positions.m_visibleBottomY < newInfo->m_listBottomPositionY.at(infoCharacterIndex)) {
                positions.m_visibleBottomY = newInfo->m_listBottomPositionY.at(infoCharacterIndex);
            }
        }
        left_glyph = glyph;
    }
    if (left_glyph) {
        positions.m_textWidth = leftX + static_cast<float>(left_glyph->image_pixel_right_x - left_glyph->image_pixel_left_x);
    }
    positions.m_utf8text = text;
    free(list_characters);
    TG_FUNCTION_END();
    return true;
}
