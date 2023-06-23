/*!
 * \file
 * \brief file tg_font_text.cpp
 *
 * font text holds the general information about text
 * that is generated by TgFontTextGenerator
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_text.h"
#include <algorithm>
#include <math.h>
#include "cache/tg_font_characters_cache.h"
#include "../global/tg_global_application.h"
#include "../global/tg_global_log.h"


TgFontText::TgFontText() :
    m_textWidth(0),
    m_visibleTopY(0),
    m_visibleBottomY(0),
    m_allLineCount(0)
{

}
/*!
 * \brief TgFontText::setFontFileNames
 *
 * sets and re-generates the font list, sets main font file
 * as first font file on the list
 *
 * \param mainFontFile [in]
 * \param listFontFiles [in] sets mainFontFile as first to this list
 */
void TgFontText::setFontFileNames(const std::string &mainFontFile, const std::vector<std::string> &listFontFileNames)
{
    TgFontDefault::getFontFileNames(mainFontFile, listFontFileNames, m_listFontFileNames);
}

/*!
 * \brief TgFontText::addCharacter
 *
 * add character to list
 * setFontFileNames must be called first before this function
 *
 * \param character [in] character to add into m_listCharacter
 * \param r text red color
 * \param g text green color
 * \param b text blue color
 */
void TgFontText::addCharacter(uint32_t character, uint8_t r, uint8_t g, uint8_t b)
{
    addCharacter(m_listCharacter, character, r, g, b, m_listFontFileNames);
}

/*!
 * \brief TgFontText::addCharacter
 *
 * add character to list
 * setFontFileNames must be called first before this function
 *
 * \param listCharacter [in/out] fill character into list
 * \param character [in] character to add into m_listCharacter
 * \param r text red color
 * \param g text green color
 * \param b text blue color
 */
void TgFontText::addCharacter(std::vector<TgFontTextCharacterInfo>&listCharacter, uint32_t character, uint8_t r, uint8_t g, uint8_t b, const std::vector<std::string> &listFontFileNames)
{
    TgFontTextCharacterInfo charInfo;
    std::string fontFileName;

    if (!listCharacter.empty()
        && listCharacter.back().m_fontFileNameIndex != -1) {
        fontFileName = listFontFileNames.at( listCharacter.back().m_fontFileNameIndex );
    }

    charInfo.m_fontFileNameIndex = TgGlobalApplication::getInstance()->getFontCharactersCache()->getFontIndexForCharacter(character, fontFileName, listFontFileNames);
    charInfo.m_character = character;
    charInfo.m_textColorR = r;
    charInfo.m_textColorG = g;
    charInfo.m_textColorB = b;

    listCharacter.push_back(charInfo);
}

/*!
 * \brief TgFontText::getCharacterCount
 *
 * \return get count of characters in this text
 */
size_t TgFontText::getCharacterCount()
{
    return m_listCharacter.size();
}

/*!
 * \brief TgFontText::getCharacter
 *
 * \param i index of character info from this text
 * \return character info
 */
TgFontTextCharacterInfo *TgFontText::getCharacter(size_t i)
{
    return &m_listCharacter[i];
}

/*!
 * \brief TgFontText::getCharactersByFontFileNameIndex
 *
 * generates list of characters that come from same fontFileNameIndex (same font file)
 * on the text
 *
 * \param fontFileNameIndex
 * \return list of characters that are belonging to this fontFileNameIndex
 * each character is only there for once
 */
std::vector<uint32_t> TgFontText::getCharactersByFontFileNameIndex(int32_t fontFileNameIndex, const std::vector<TgFontTextCharacterInfo>&listCharacter)
{
    std::vector<uint32_t> ret;
    std::vector<TgFontTextCharacterInfo>::const_iterator it;
    for (it=listCharacter.begin();it!=listCharacter.end();it++) {
        if (it->m_fontFileNameIndex == fontFileNameIndex) {
            if (std::find(ret.begin(), ret.end(), it->m_character) == ret.end()) {
                ret.push_back(it->m_character);
            }
        }
    }
    return ret;
}

/*!
 * \brief TgFontText::generateFontTextInfoGlyphs
 *
 * generates m_listFontInfo for this text
 * which font textures contains these glyphs in the text
 *
 * \param fontSize
 * \param onlyForCalculation if true, then this TgFontInfo is not set into cache
 */
void TgFontText::generateFontTextInfoGlyphs(float fontSize, bool onlyForCalculation)
{
    TgFontInfo *fontInfo;
    size_t i, indexToUse;
    std::vector<bool> characterUsed;
    characterUsed.resize(getCharacterCount(), false);
    clearCacheValues();
    m_listFontInfo.resize(getCharacterCount(), nullptr);
    bool added;

    while (1) {
        indexToUse = characterUsed.size();
        for (i=0;i<characterUsed.size();i++) {
            if (!characterUsed[i]) {
                indexToUse = i;
                break;
            }
        }
        if (indexToUse == characterUsed.size()) {
            break;
        }

        fontInfo = nullptr;
        if (m_listCharacter[indexToUse].m_fontFileNameIndex != -1) {
            std::vector<uint32_t> listCharacters = getCharactersByFontFileNameIndex(m_listCharacter[indexToUse].m_fontFileNameIndex, m_listCharacter);
            fontInfo = TgGlobalApplication::getInstance()->getFontGlyphCache()->generateCacheForText(listCharacters,
                                        m_listFontFileNames.at( getCharacter(indexToUse)->m_fontFileNameIndex ).c_str(),
                                        fontSize, onlyForCalculation);
        }

        added = false;
        for (i=0;i<characterUsed.size();i++) {
            if (getCharacter(i)->m_fontFileNameIndex == getCharacter(indexToUse)->m_fontFileNameIndex) {
                characterUsed[i] = true;
                m_listFontInfo[i] = fontInfo;
                added = true;
            }
        }
        if (!added) {
            if (fontInfo->m_data) {
                prj_ttf_reader_clear_data(&fontInfo->m_data);
            }
            delete fontInfo;
        }
    }
}

/*!
 * \brief TgFontText::generateFontTextInfoGlyphsData
 *
 * generates m_listFontInfo for this text
 * which font textures contains these glyphs in the text
 * this is for data cache only
 *
 * \param fontSize
 */
void TgFontText::generateFontTextInfoGlyphsData(float fontSize, std::vector<TgFontTextCharacterInfo>&listCharacter,
                                                std::vector<TgFontInfoData *>&listFontInfo,
                                                std::vector<std::string> &listFontFiles)
{
    TgFontInfoData *fontInfo;
    size_t i, indexToUse;
    std::vector<bool> characterUsed;
    characterUsed.resize(listCharacter.size(), false);
    listFontInfo.resize(listCharacter.size(), nullptr);
    bool added;

    while (1) {
        indexToUse = characterUsed.size();
        for (i=0;i<characterUsed.size();i++) {
            if (!characterUsed[i]) {
                indexToUse = i;
                break;
            }
        }
        if (indexToUse == characterUsed.size()) {
            break;
        }

        fontInfo = nullptr;
        if (listCharacter[indexToUse].m_fontFileNameIndex != -1) {
            std::vector<uint32_t> listCharacters = getCharactersByFontFileNameIndex(listCharacter[indexToUse].m_fontFileNameIndex, listCharacter);
            fontInfo = TgGlobalApplication::getInstance()->getFontGlyphCacheData()->generateCacheForText(listCharacters,
                                        listFontFiles.at( listCharacter[indexToUse].m_fontFileNameIndex ).c_str(),
                                        fontSize);
        }

        added = false;
        for (i=0;i<characterUsed.size();i++) {
            if (listCharacter[i].m_fontFileNameIndex == listCharacter[indexToUse].m_fontFileNameIndex) {
                characterUsed[i] = true;
                listFontInfo[i] = fontInfo;
                added = true;
            }
        }
        if (!added) {
            if (fontInfo->m_data) {
                prj_ttf_reader_clear_data(&fontInfo->m_data);
            }
            delete fontInfo;
        }
    }
}

/*!
 * \brief TgFontText::clearCacheValues
 *
 * clearing cache font cache values that were used onlyForCalculation == true on function
 * generateFontTextInfoGlyphs
 */
void TgFontText::clearCacheValues()
{
    size_t i, i2;
    std::vector<size_t>listToDelete;
    for (i=0;i<m_listFontInfo.size();i++) {
        if (!m_listFontInfo[i]
            || m_listFontInfo[i]->m_addedToCache
            || !m_listFontInfo[i]->m_data) {
            continue;
        }
        for (i2=i+1;i2<m_listFontInfo.size();i2++) {
            if (m_listFontInfo[i]->m_data == m_listFontInfo[i2]->m_data) {
                m_listFontInfo[i2]->m_addedToCache = true;
            }
        }
        prj_ttf_reader_clear_data(&m_listFontInfo[i]->m_data);
        listToDelete.push_back(i);
    }
    for (i=0;i<listToDelete.size();i++) {
        delete m_listFontInfo[listToDelete[i]];
    }
    m_listFontInfo.clear();
}

/*!
 * \brief TgFontText::getFontInfo
 *
 * get font info of index
 *
 * \param i index of font info
 * \return
 */
TgFontInfo *TgFontText::getFontInfo(size_t i)
{
    return m_listFontInfo[i];
}

/*!
 * \brief TgFontText::getTextWidth
 *
 * \return get text width
 */
float TgFontText::getTextWidth()
{
    return m_textWidth;
}

/*!
 * \brief TgFontText::getVisibleTopY
 *
 * \return get visible top y
 */
float TgFontText::getVisibleTopY()
{
    return m_visibleTopY;
}

/*!
 * \brief TgFontText::getVisibleBottomY
 *
 * \return get visible bottom y
 */
float TgFontText::getVisibleBottomY()
{
    return m_visibleBottomY;
}

/*!
 * \brief TgFontText::getFontHeight
 *
 * \return font height
 */
float TgFontText::getFontHeight()
{
    float ret = 0;
    size_t i, c = m_listFontInfo.size();
    for (i=0;i<c;i++) {
        if (m_listFontInfo[i]) {
            if (ret < m_listFontInfo[i]->m_fontHeight) {
                ret = m_listFontInfo[i]->m_fontHeight;
            }
        }
    }

    return static_cast<float>(ret);
}

/*!
 * \brief TgFontText::getLineHeight
 *
 * \return line heght of single line
 */
float TgFontText::getLineHeight()
{
    return static_cast<float>(std::ceil(getFontHeight()*1.5f));
}

/*!
 * \brief TgFontText::getAllDrawTextHeight
 *
 * \return height of the all drawed texts (including line breaks)
 */
float TgFontText::getAllDrawTextHeight()
{
    uint32_t allLineCount = getAllLineCount();
    if (allLineCount == 0) {
        return 0;
    }
    float fontHeight = getFontHeight();
    if (allLineCount == 1) {
        return fontHeight;
    }
    return static_cast<float>(std::ceil(fontHeight + static_cast<float>(allLineCount-1)*getLineHeight()));
}

void TgFontText::setTextWidth(float textWidth)
{
    m_textWidth = textWidth;
}

void TgFontText::setVisibleTopY(float visibleTopY)
{
    m_visibleTopY = visibleTopY;
}

void TgFontText::setVisibleBottomY(float visibleBottomY)
{
    m_visibleBottomY = visibleBottomY;
}

void TgFontText::setAllLineCount(uint32_t lineCount)
{
    m_allLineCount = lineCount;
}

uint32_t TgFontText::getAllLineCount()
{
    return m_allLineCount;
}

/*!
 * \brief TgFontText::clearListLinesWidth
 *
 * clears all widths of lines
 */
void TgFontText::clearListLinesWidth()
{
    m_listLineWidth.clear();
}

/*!
 * \brief TgFontText::setListLinesWidth
 *
 * sets width (pixels) of the line
 * \param lineNumber [in] line number, 0 == first line
 * \param lineWidth [in] width of line
 */
void TgFontText::setListLinesWidth(size_t lineNumber, float lineWidth)
{
    setListLinesWidth(m_listLineWidth, lineNumber, lineWidth);
}

/*!
 * \brief TgFontText::setListLinesWidth
 *
 * sets width (pixels) of the line
 * \param listLineWidth [in/out]
 * \param lineNumber [in] line number, 0 == first line
 * \param lineWidth [in] width of line
 */
void TgFontText::setListLinesWidth(std::vector<float> &listLineWidth, size_t lineNumber, float lineWidth)
{
    if (listLineWidth.size() > lineNumber) {
        listLineWidth[lineNumber] = lineWidth;
    } else if (listLineWidth.size() == lineNumber) {
        listLineWidth.push_back(lineWidth);
    } else {
        while (1) {
            listLineWidth.push_back(0);
            if (listLineWidth.size() == lineNumber) {
                listLineWidth.push_back(lineWidth);
                break;
            }
        }
    }
}

/*!
 * \brief TgFontText::getTextLineWidth
 *
 * \param lineNumber [0] first line == 0
 * \return text width of lineNumber
 */
float TgFontText::getTextLineWidth(size_t lineNumber)
{
    if (m_listLineWidth.size() > lineNumber) {
        return m_listLineWidth.at(lineNumber);
    }
    return 0;
}
