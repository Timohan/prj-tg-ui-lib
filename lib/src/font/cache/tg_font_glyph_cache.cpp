/*!
 * \file
 * \brief file tg_font_glyph_cache.cpp
 *
 * font glyph cache
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_glyph_cache.h"
#include <cstring>
#include <algorithm>
#include <cmath>
#include "../../global/tg_global_log.h"
#include "../../shader/tg_shader_2d.h"
#include "../tg_font_text.h"
#include "../tg_font_math.h"
#include "../../global/tg_global_application.h"
#define FONT_ACCURACY_VALUE 5

TgFontGlyphCache::TgFontGlyphCache()
{
}

TgFontGlyphCache::~TgFontGlyphCache()
{
}

bool TgFontGlyphCache::loadCharacters(const std::vector<uint32_t> &listCharacters,
                                      const std::vector<std::string> &listFontFiles,
                                      const float fontSize,
                                      const uint32_t maxLineCount, const float maxLineWidth,
                                      const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine,
                                      std::vector<PrjTgFontDrawHelperData> &listHelperCharacters,
                                      float &fontHeight)
{
    m_mutex.lock();
    bool ret;
    if (allowBreakLineGoOverMaxLine) {
        ret = m_helper.load(listHelperCharacters, listCharacters, listFontFiles, fontSize, 0, FONT_ACCURACY_VALUE, wordWrap, maxLineWidth, 0, allowBreakLineGoOverMaxLine);
    } else {
        ret = m_helper.load(listHelperCharacters, listCharacters, listFontFiles, fontSize, 0, FONT_ACCURACY_VALUE, wordWrap, maxLineWidth, maxLineCount, allowBreakLineGoOverMaxLine);
    }
    fontHeight = m_helper.getFontHeight(listFontFiles.at(0), fontSize, 0, FONT_ACCURACY_VALUE);
    m_mutex.unlock();
    return ret;
}

bool TgFontGlyphCache::loadCharactersToCache(const char *filename, const std::vector<uint32_t> &listCharacters, const float &fontSize)
{
    std::vector<PrjTgFontDrawHelperData> listCharactersData;
    std::vector<std::string> fontFile;
    fontFile.push_back(std::string(filename));
    m_mutex.lock();
    bool ret = m_helper.load(listCharactersData, listCharacters, fontFile, fontSize, 0, FONT_ACCURACY_VALUE);
    m_mutex.unlock();
    return ret;
}

/*!
 * \brief TgFontGlyphCache::generateCacheForText
 *
 * generates image's vertices and texture for text (from list of characters)
 * if possible for using these vertices and textures in others
 *
 * \param listCharacters list of characters
 * \param listFontFiles full file path of the font files
 * \param fontSize font size
 * \param onlyForCalculation if true, then this TgFontInfo is not set into cache
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfo *TgFontGlyphCache::generateCacheForText(const std::vector<uint32_t> &listCharacters,
                                                   const std::vector<std::string> &listFontFiles,
                                                   float fontSize,
                                                   const uint32_t maxLineCount, const float maxLineWidth,
                                                   const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine)
{
    TG_FUNCTION_BEGIN();
    size_t i;
    std::vector<PrjTgFontDrawHelperData> listHelperCharacters;
    m_mutex.lock();
    if (allowBreakLineGoOverMaxLine) {
        if (!m_helper.load(listHelperCharacters, listCharacters, listFontFiles, fontSize, 0, FONT_ACCURACY_VALUE, wordWrap, maxLineWidth, 0, allowBreakLineGoOverMaxLine)) {
            m_mutex.unlock();
            return nullptr;
        }
    } else {
        if (!m_helper.load(listHelperCharacters, listCharacters, listFontFiles, fontSize, 0, FONT_ACCURACY_VALUE, wordWrap, maxLineWidth, maxLineCount, allowBreakLineGoOverMaxLine)) {
            m_mutex.unlock();
            return nullptr;
        }
    }
    m_mutex.unlock();
    TgFontInfo *ret = new TgFontInfo;
    for (i=0;i<listHelperCharacters.size();i++) {
        if (listHelperCharacters.at(i).m_character != '\n'
            && listHelperCharacters.at(i).m_character != '\r') {
            TgGlobalApplication::getInstance()->getFontTextureCache()->add(&listHelperCharacters.at(i),
                fontSize,
                listFontFiles.at(listHelperCharacters.at(i).m_fontSizeIndex));
        }
    }
    size_t characterIndex;

    m_mutex.lock();
    ret->m_fontHeight = m_helper.getFontHeight(listFontFiles.at(0), fontSize, 0.0f,FONT_ACCURACY_VALUE);
    m_mutex.unlock();
    ret->m_fontSize = fontSize;

    for (i=0;i<listHelperCharacters.size();i++) {
        if (listHelperCharacters.at(i).m_character == '\n'
            || listHelperCharacters.at(i).m_character == '\r') {
            ret->m_listRender.push_back(nullptr);
            ret->m_listTextureImage.push_back(0);
        } else {
            TgFontTextureCacheItem *item = TgGlobalApplication::getInstance()->getFontTextureCache()->get(
                listHelperCharacters.at(i).m_character, fontSize,
                listFontFiles.at(listHelperCharacters.at(i).m_fontSizeIndex),
                characterIndex);
            ret->m_listRender.push_back(item->m_listRender.at(0));
            ret->m_listTextureImage.push_back(item->m_texture);
        }
        ret->m_listCharacter.push_back(listHelperCharacters.at(i).m_character);
        ret->m_glyphDrawX.push_back(listHelperCharacters.at(i).m_glyphDrawX);
        ret->m_glyphDrawY.push_back(listHelperCharacters.at(i).m_glyphDrawY);
        ret->m_glyphPixelWidth.push_back(listHelperCharacters.at(i).m_glyphOnImageDataWidth);
        ret->m_lineIndex.push_back(listHelperCharacters.at(i).m_lineIndex);
        ret->m_glyphOffsetDrawX.push_back(listHelperCharacters.at(i).m_glyphOffsetPositionX);
        ret->m_glyphOffsetDrawY.push_back(listHelperCharacters.at(i).m_glyphOffsetPositionY);
    }

    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgFontGlyphCache::render
 *
 * Renders the text
 * \param fontText
 * \param vertexTransformIndex vertex tranform index in shader's uniform location
 * \param shaderColorIndex shader color index
 * \param listMatrix list of matrixes for each character to shader
 */
void TgFontGlyphCache::render(TgFontText *fontText, const int vertexTransformIndex, const int shaderColorIndex, const std::vector<TgMatrix4x4>&listMatrix)
{
    if (!fontText->getFontInfo()) {
        return;
    }
    size_t i, c = fontText->getFontInfo()->m_listTextureImage.size();
    uint8_t r = 0, g = 0, b = 0;
    bool colorFirstTime = true;
    for (i=0;i<c;i++) {
        if (fontText->getCharacter(i)->m_character == '\n'
            || !fontText->getCharacter(i)->m_draw
            || fontText->getCharacter(i)->m_fontFileNameIndex == -1) {
            continue;
        }
        glUniformMatrix4fv(vertexTransformIndex, 1, 0, listMatrix[i].getMatrixTable()->data);
        if (fontText->getCharacter(i)->m_textColorR != r
            || fontText->getCharacter(i)->m_textColorG != g
            || fontText->getCharacter(i)->m_textColorB != b
            || colorFirstTime) {
            glUniform4f(shaderColorIndex,
                static_cast<float>(fontText->getCharacter(i)->m_textColorR )/255.0f,
                static_cast<float>(fontText->getCharacter(i)->m_textColorG )/255.0f,
                static_cast<float>(fontText->getCharacter(i)->m_textColorB )/255.0f, 1);
            r = fontText->getCharacter(i)->m_textColorR;
            g = fontText->getCharacter(i)->m_textColorG;
            b = fontText->getCharacter(i)->m_textColorB;
            colorFirstTime = false;
        }
        fontText->getFontInfo()->m_listRender[ i ]->render(
            fontText->getFontInfo()->m_listTextureImage.at( i ),
            0, 4, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
}

/*!
 * \brief TgFontGlyphCache::getTextPosition
 *
 * get cursor drawing relative position (x)
 * \param fontText
 * \param cursorPosition 0 == first character, 1 == second character
 * \param positionX [out] cursorPosition's drawing relative position
 */
void TgFontGlyphCache::getTextPosition(TgFontText *fontText, size_t cursorPosition, float &positionX)
{
    if (!fontText) {
        positionX = 0;
        return;
    }
    if (cursorPosition < fontText->getCharacterCount()) {
        positionX = fontText->getCharacter(cursorPosition)->m_positionX;
    } else if (cursorPosition == fontText->getCharacterCount()) {
        positionX = fontText->getTextWidth();
    }
}

size_t TgFontGlyphCache::getTextCharacterIndex(TgFontText *fontText, const float x)
{
    TG_FUNCTION_BEGIN();
    if (x <= 0) {
        TG_FUNCTION_END();
        return 0;
    }

    if (x >= fontText->getTextWidth()) {
        TG_FUNCTION_END();
        return fontText->getCharacterCount();
    }

    const size_t characterCount = fontText->getCharacterCount();
    size_t ret = static_cast<size_t>(x*static_cast<float>(characterCount)/fontText->getTextWidth());
    float characterPositionX;
    int previousAdd = 0;
    while (1) {
        getTextPosition(fontText, ret, characterPositionX);
        if (characterPositionX < x) {
            if (ret == characterCount) {
                TG_FUNCTION_END();
                return ret;
            }
            if (previousAdd == -1) {
                TG_FUNCTION_END();
                return ret;
            }
            ret++;
            previousAdd = 1;
        } else if (characterPositionX > x) {
            if (ret == 0) {
                TG_FUNCTION_END();
                return ret;
            }
            if (previousAdd == 1) {
                if (ret > 0) {
                    TG_FUNCTION_END();
                    return ret - 1;
                }
                TG_FUNCTION_END();
                return ret;
            }
            ret--;
            previousAdd = -1;
        } else {
            TG_FUNCTION_END();
            return ret;
        }
    }

    TG_FUNCTION_END();
}
