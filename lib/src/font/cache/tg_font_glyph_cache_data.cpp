/*!
 * \file
 * \brief file tg_font_glyph_cache_data.cpp
 *
 * font glyph cache, but without OpenGL Rendering cache
 * (Rendering required cache is in TgFontGlyphCache)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_glyph_cache_data.h"
#include <cstring>
#include <algorithm>
#include <cmath>
#include "../../global/tg_global_log.h"
#include "../../global/tg_global_application.h"
#ifndef FONT_ACCURACY_VALUE
#define FONT_ACCURACY_VALUE 5
#endif

TgFontGlyphCacheData::TgFontGlyphCacheData()
{

}

TgFontGlyphCacheData::~TgFontGlyphCacheData()
{
    for (size_t i=0;i<m_listCachedData.size();i++) {
        prj_ttf_reader_clear_data(&m_listCachedData[i]->m_data);
        delete m_listCachedData[i];
    }
    m_listCachedData.clear();
}

/*!
 * \brief TgFontGlyphCache::generateCacheForText
 *
 * generates image's vertices and texture for text (from list of characters)
 * if possible for using these vertices and textures in others
 *
 * \param listCharacters list of characters
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfoData *TgFontGlyphCacheData::generateCacheForText(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    TgFontInfoData *ret = isFontCached(listCharacters, fontFile, fontSize);
    if (ret) {
        return ret;
    }
    std::string additionalCharactersToGlyph = "ABCQWERTYUIOPÅSDFGHJKLÖÄZXVNMqwertyuiopasdfgjhklöäzxcvbnm<>|;:,.-_€'*~^1234567890+'!\"#¤%&/()=?½@£$‰‚{[]}— ";
    uint32_t *list_additonal_characters = nullptr, list_additonal_characters_size;
    if (prj_ttf_reader_get_characters(additionalCharactersToGlyph.c_str(), &list_additonal_characters, &list_additonal_characters_size)
            || !list_additonal_characters_size) {
        TG_FUNCTION_END();
        return nullptr;
    }

    std::vector<uint32_t> newListCharacters = std::move(listCharacters);
    for (uint32_t i=0;i<list_additonal_characters_size;i++) {
        if (std::find(newListCharacters.begin(), newListCharacters.end(), list_additonal_characters[i]) == newListCharacters.end()
            && TgGlobalApplication::getInstance()->getFontCharactersCache()->isCharacterForThisFont(list_additonal_characters[i], fontFile) ) {
            newListCharacters.push_back(list_additonal_characters[i]);
        }
    }
    std::vector<uint32_t>::iterator it;
    it = std::find(newListCharacters.begin(), newListCharacters.end(), 'A');
    if (it != newListCharacters.end()) {
        newListCharacters.erase(it);
        // we set 'A' as a first character, because it's good for font height calculation
        newListCharacters.insert(newListCharacters.begin(), 'A');
    }

    ret = generateCache(newListCharacters, fontFile, fontSize);
    free(list_additonal_characters);
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgFontGlyphCacheData::isFontCached
 *
 * searches if font is already cached for it can use same vertixes
 * and texture
 *
 * \param listCharacters contains all charactes to make vertices and texture
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if not, otherwise cached TgFontInfo
 */
TgFontInfoData *TgFontGlyphCacheData::isFontCached(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    bool notFound;
    std::vector<TgFontInfoData *>::iterator it;
    std::vector<uint32_t>::const_iterator itListCharacters;
    for (it=m_listCachedData.begin();it!=m_listCachedData.end();it++) {
        if (memcmp(&(*it)->m_fontSize, &fontSize, sizeof(float)) == 0
            && (*it)->m_fontFile.compare(fontFile) == 0) {
            notFound = false;
            for (itListCharacters=listCharacters.begin();itListCharacters!=listCharacters.end();itListCharacters++) {
                if (std::find((*it)->m_listCharacter.begin(), (*it)->m_listCharacter.end(), (*itListCharacters)) == (*it)->m_listCharacter.end()) {
                    notFound = true;
                    break;
                }
            }
            if (!notFound) {
                TG_FUNCTION_END();
                return (*it);
            }
        }
    }
    TG_FUNCTION_END();
    return nullptr;
}

/*!
 * \brief TgFontGlyphCacheData::generateCache
 *
 * generates image's vertices and texture
 *
 * \param listCharacters list of characters
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfoData *TgFontGlyphCacheData::generateCache(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    prj_ttf_reader_data_t *data;
    data = prj_ttf_reader_init_data();
    if (!data) {
        TG_ERROR_LOG("prj_ttf_reader_init_data return failed");
        TG_FUNCTION_END();
        return nullptr;
    }
    if (prj_ttf_reader_generate_glyphs_list_characters(listCharacters.data(), static_cast<uint32_t>(listCharacters.size()), fontFile, fontSize, FONT_ACCURACY_VALUE, data)
        || !data->image.width
        || !data->image.height) {
        prj_ttf_reader_clear_data(&data);
        TG_ERROR_LOG("generating glyphs failed");
        TG_FUNCTION_END();
        return nullptr;
    }

    TgFontInfoData *newInfo = new TgFontInfoData;
    newInfo->m_data = data;

    if (!calculateFontHeight(newInfo, listCharacters)) {
        TG_ERROR_LOG("creating the text vertices failed");
        prj_ttf_reader_clear_data(&data);
        delete newInfo;
        TG_FUNCTION_END();
        return nullptr;
    }

    newInfo->m_fontFile = fontFile;
    newInfo->m_fontSize = fontSize;
    TG_FUNCTION_END();
    return newInfo;
}

/*!
 * \brief TgFontGlyphCacheData::calculateFontHeight
 *
 * calculates the font height of rendering
 *
 * \param newInfo [in/out] info
 * \param listCharacters contains the text
 * \return true on success
 */
bool TgFontGlyphCacheData::calculateFontHeight(TgFontInfoData *newInfo, const std::vector<uint32_t> &listCharacters)
{
    TG_FUNCTION_BEGIN();
    size_t i, c = listCharacters.size();
    bool firstTime = false;
    const prj_ttf_reader_glyph_data_t *glyph;

    for (i=0;i<c;i++) {
        newInfo->m_listCharacter.push_back(listCharacters.at(i));

        glyph = prj_ttf_reader_get_character_glyph_data(listCharacters.at(i), newInfo->m_data);
        if (!glyph) {
            continue;
        }

        if (!firstTime) {
            // set the line y position
            newInfo->m_fontHeight = static_cast<float>(glyph->image_pixel_bottom_y-glyph->image_pixel_top_y+glyph->image_pixel_offset_line_y);;
            firstTime = true;
        }
    }

    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgFontGlyphCacheData::addCache
 *
 * add data to cache, as long it's not cached already
 *
 * \param data
 */
void TgFontGlyphCacheData::addCache(TgFontInfoData *data)
{
    if (!data || data->m_cached) {
        return;
    }
    data->m_cached = true;
    m_listCachedData.push_back(data);
}

/*!
 * \brief TgFontGlyphCacheData::getFontHeight
 *
 * \param listFontInfo [in] list of font info
 * \return font height
 */
float TgFontGlyphCacheData::getFontHeight(std::vector<TgFontInfoData *> &listFontInfo)
{
    float ret = 0;
    size_t i, c = listFontInfo.size();
    for (i=0;i<c;i++) {
        if (listFontInfo[i]) {
            if (ret < listFontInfo[i]->m_fontHeight) {
                ret = listFontInfo[i]->m_fontHeight;
            }
        }
    }

    return static_cast<float>(ret);
}

/*!
 * \brief TgFontGlyphCacheData::getLineHeight
 *
 * \param listFontInfo [in] list of font info
 * \return line heght of single line
 */
float TgFontGlyphCacheData::getLineHeight(std::vector<TgFontInfoData *> &listFontInfo)
{
    return static_cast<float>(std::ceil(getFontHeight(listFontInfo)*1.5f));
}

/*!
 * \brief TgFontGlyphCacheData::getAllDrawTextHeight
 *
 * \param allLineCount [in] count of lines
 * \param listFontInfo [in] list of font info
 * \return height of the all drawed texts (including line breaks)
 */
float TgFontGlyphCacheData::getAllDrawTextHeight(uint32_t allLineCount, std::vector<TgFontInfoData *> &listFontInfo)
{
    if (allLineCount == 0) {
        return 0;
    }
    float fontHeight = getFontHeight(listFontInfo);
    if (allLineCount == 1) {
        return fontHeight;
    }
    return static_cast<float>(std::ceil(fontHeight + static_cast<float>(allLineCount-1)*getLineHeight(listFontInfo)));
}