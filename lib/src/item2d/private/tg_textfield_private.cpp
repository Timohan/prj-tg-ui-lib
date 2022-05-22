/*!
 * \file
 * \brief file tg_textfield_private.cpp
 *
 * it holds general TgTextfieldPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_textfield_private.h"
#include <cmath>
#include <string.h>
#include "../../global/tg_global_application.h"
#include "../../font/cache/tg_font_glyph_cache.h"
#include "../../font/tg_font_default.h"
#include "../../font/tg_font_text.h"
#include "../../global/tg_global_log.h"
#include "../tg_item2d.h"
#include "../../window/tg_mainwindow_private.h"

TgTextfieldPrivate::TgTextfieldPrivate(TgItem2d *currentItem,
                                       const char *text, const char *fontFile, float fontSize,
                                       uint8_t r, uint8_t g, uint8_t b) :
    m_currentItem(currentItem),
    m_fontText(nullptr),
    m_r(r),
    m_g(g),
    m_b(b),
    m_fontFile(fontFile),
    m_fontSize(fontSize),
    m_initDone(false),
    m_alignHorizontal(TgTextfieldHorizontalAlign::AlignLeft),
    m_alignVertical(TgTextfieldVerticalAlign::AlignTop)
{
    if (strlen(text) > 0) {
        TgTextFieldText t;
        t.m_text = text;
        t.m_textColorR = r;
        t.m_textColorG = g;
        t.m_textColorB = b;
        m_listText.push_back(t);
        TgFontTextGenerator::getCharacters(m_listText, m_listCharacter);
    }

    if (!fontFile || m_fontFile.empty()) {
        m_fontFile = TgGlobalApplication::getInstance()->getFontDefault()->getDefaultFont();
    } 
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}
TgTextfieldPrivate::~TgTextfieldPrivate()
{
    TG_FUNCTION_BEGIN();
    m_listTransform.clear();
    if (m_fontText) {
        delete m_fontText;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgRectangle::generateTransform
 *
 * generate transform position
 *
 */
void TgTextfieldPrivate::generateTransform(TgItem2d *currentItem)
{
    size_t i;
    float x = 0, y = 0;
    m_listTransform.resize( m_fontText->getCharacterCount() );
    if (!m_fontText) {
        return;
    }

    switch (m_alignHorizontal) {
        case TgTextfieldHorizontalAlign::AlignLeft:
        default:
            x = currentItem->getXonWindow();
            break;
        case TgTextfieldHorizontalAlign::AlignCenterH:
            x = currentItem->getWidth()/2+currentItem->getXonWindow() - m_fontText->getTextWidth()/2;
            break;
        case TgTextfieldHorizontalAlign::AlignRight:
            x = currentItem->getWidth()+currentItem->getXonWindow() - m_fontText->getTextWidth();
            break;
    }
    switch (m_alignVertical) {
        case TgTextfieldVerticalAlign::AlignTop:
        default:
            y = currentItem->getYonWindow();
            break;
        case TgTextfieldVerticalAlign::AlignCenterV:
            y = currentItem->getHeight()/2+currentItem->getYonWindow() - m_fontText->getFontHeight()/2;
            break;
        case TgTextfieldVerticalAlign::AlignBottom:
            y = currentItem->getHeight()+currentItem->getYonWindow() - m_fontText->getFontHeight();
            break;
    }

    x = std::roundf(x);
    y = std::roundf(y);
    for (i=0;i<m_fontText->getCharacterCount();i++) {
        m_listTransform[i].setTransform(m_fontText->getCharacter(i)->positionLeftX+x, y);
    }
}

/*!
 * \brief TgTextfieldPrivate::setText
 *
 * \param text text
 * \param currentItem
 */
void TgTextfieldPrivate::setText(const char *text, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgTextFieldText> listText;
    TgTextFieldText newText;
    newText.m_text = text;
    newText.m_textColorR = m_r;
    newText.m_textColorG = m_g;
    newText.m_textColorB = m_b;
    listText.push_back(newText);
    setText(listText, currentItem);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::setText
 *
 * sets multi color text
 *
 * \param listText list of text with different colors
 * \param currentItem
 */
void TgTextfieldPrivate::setText(const std::vector<TgTextFieldText> &listText, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (isEqualText(listText)) {
        if (isEqualTextColor(listText)) {
            m_mutex.unlock();
            TG_FUNCTION_END();
            return;
        }
        size_t i;
        for (i=0;i<listText.size();i++) {
            m_listText[i].m_textColorR = listText.at(i).m_textColorR;
            m_listText[i].m_textColorG = listText.at(i).m_textColorG;
            m_listText[i].m_textColorB = listText.at(i).m_textColorB;
        }
        if (TgFontTextGenerator::changeTextColor(listText, m_fontText)) {
            m_mutex.unlock();
            TG_FUNCTION_END();
            return;
        }
    }

    m_listText = std::move(listText);
    TgFontTextGenerator::getCharacters(m_listText, m_listCharacter);
    m_initDone = false;
    currentItem->setPositionChanged(true);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::isEqualText
 *
 * Checks if text is equal as before
 * \param listText
 * \return true if it was equal text
 */
bool TgTextfieldPrivate::isEqualText(const std::vector<TgTextFieldText> &listText)
{
    if (m_listText.size() != listText.size()) {
        return false;
    }
    size_t i;
    for (i=0;i<listText.size();i++) {
        if (m_listText.at(i).m_text != listText.at(i).m_text) {
            return false;
        }
    }
    return true;
}

/*!
 * \brief TgTextfieldPrivate::isEqualTextColor
 *
 * Checks if text color is equal as before
 * \param listText
 * \return true if it was equal text color
 */
bool TgTextfieldPrivate::isEqualTextColor(const std::vector<TgTextFieldText> &listText)
{
    if (m_listText.size() != listText.size()) {
        return false;
    }
    size_t i;
    for (i=0;i<listText.size();i++) {
        if (m_listText.at(i).m_textColorR != listText.at(i).m_textColorR) {
            return false;
        }
        if (m_listText.at(i).m_textColorB != listText.at(i).m_textColorB) {
            return false;
        }
        if (m_listText.at(i).m_textColorG != listText.at(i).m_textColorG) {
            return false;
        }
    }
    return true;
}

/*!
 * \brief TgTextfieldPrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgTextfieldPrivate::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (!m_initDone) {
        if (m_fontText) {
            delete m_fontText;
        }
        m_fontText = TgFontTextGenerator::generateFontTextInfo(m_listText, m_fontFile.c_str());
        m_fontText->generateFontTextInfoGlyphs(m_fontSize);
        TgCharacterPositions::generateTextCharacterPositioning(m_fontText);
        m_initDone = true;
    }
    if (m_currentItem->getPositionChanged()) {
        generateTransform(m_currentItem);
        m_currentItem->setAddMinMaxHeightOnVisible(
            m_fontText->getVisibleTopY(),
            m_fontText->getVisibleBottomY());
        m_currentItem->setPositionChanged(false);
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::render
 *
 * Renders the text
 * \param windowInfo
 * \param currentItem
 */
void TgTextfieldPrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!m_fontText) {
        TG_FUNCTION_END();
        return;
    }

    glUniform1i( windowInfo->m_shaderRenderTypeIndex, 1);
    glUniform4f(windowInfo->m_maxRenderValues,
                currentItem->getXminOnVisible(), currentItem->getYminOnVisible(),
                currentItem->getXmaxOnVisible(windowInfo),
                currentItem->getYmaxOnVisible(windowInfo));

    if (m_fontText) {
        TgGlobalApplication::getInstance()->getFontGlyphCache()->render(m_fontText, windowInfo->m_shaderTransformIndex,
            windowInfo->m_shaderColorIndex, m_listTransform);
    }

    glUniform1i( windowInfo->m_shaderRenderTypeIndex, 0);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::setHorizontalAlign
 *
 * set text field's horizontal align
 * \param currentItem current item
 * \param align horizontal align
 */
void TgTextfieldPrivate::setHorizontalAlign(TgItem2d *currentItem, TgTextfieldHorizontalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_alignHorizontal = align;
    currentItem->setPositionChanged(true);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::setVerticalAlign
 *
 * set text field's vertical align
 * \param currentItem current item
 * \param align vertical align
 */
void TgTextfieldPrivate::setVerticalAlign(TgItem2d *currentItem, TgTextfieldVerticalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_alignVertical = align;
    currentItem->setPositionChanged(true);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::getAlignHorizontal
 *
 * \return get horizontal align
 */
TgTextfieldHorizontalAlign TgTextfieldPrivate::getAlignHorizontal() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_alignHorizontal;
}

/*!
 * \brief TgTextfieldPrivate::getAlignVertical
 *
 * \return get vertical align
 */
TgTextfieldVerticalAlign TgTextfieldPrivate::getAlignVertical() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_alignVertical;
}

/*!
 * \brief TgTextfieldPrivate::setFontSize
 *
 * \param fontSize fon size for text
 */
void TgTextfieldPrivate::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_fontSize = fontSize;
    m_initDone = false;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::getCharacterCount
 *
 * \return get count of characters in this text
 */
size_t TgTextfieldPrivate::getCharacterCount()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    size_t ret = m_listCharacter.size();
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgTextfieldPrivate::getTextCharacterIndex
 *
 * \param x [in]
 * \return character index by the x
 */
size_t TgTextfieldPrivate::getTextCharacterIndex(float x)
{
    return TgGlobalApplication::getInstance()->getFontGlyphCache()->getTextCharacterIndex(m_fontText, x);
}

/*!
 * \brief TgTextfieldPrivate::getCharacterByIndex
 *
 * \param index of the character
 * \return the uint32 value of the character by the index
 * returns 0, if index larger than character count
 */
uint32_t TgTextfieldPrivate::getCharacterByIndex(size_t index)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (m_listCharacter.size() <= index) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return 0;
    }
    uint32_t ret = m_listCharacter.at(index).m_character;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgTextfieldPrivate::getTextPosition
 *
 * \param cursorPosition [in] get position x/Y for this cursor position, 0 == first character
 * \param positionX [out]
 */
void TgTextfieldPrivate::getTextPosition(const size_t cursorPosition, float &positionX)
{
    TgGlobalApplication::getInstance()->getFontGlyphCache()->getTextPosition(m_fontText, cursorPosition, positionX);
}

/*!
 * \brief TgTextfieldPrivate::getFontFile
 *
 * \return get font file
 */
std::string TgTextfieldPrivate::getFontFile()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    std::string ret = m_fontFile;
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgTextfieldPrivate::getTextWidth
 *
 * \return text width (pixels)
 */
float TgTextfieldPrivate::getTextWidth()
{
    TG_FUNCTION_BEGIN();
    float ret = 0;
    m_mutex.lock();
    if (m_fontText) {
        ret = m_fontText->getTextWidth();
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgTextfieldPrivate::editText
 *
 * modifies the text
 *
 * \param listAddCharacter characters to add to startCharacterIndex
 * \param startCharacterIndex starts adding the characters to this position
 * \param characterCountToRemove number of characters to remove from startCharacterIndex
 */
void TgTextfieldPrivate::editText(std::vector<uint32_t>&listAddCharacter, const size_t startCharacterIndex, const size_t characterCountToRemove)
{
    m_mutex.lock();
    if (characterCountToRemove) {
        m_listCharacter.erase(m_listCharacter.begin()+startCharacterIndex, m_listCharacter.begin()+(startCharacterIndex+characterCountToRemove));
    }

    size_t i;
    uint8_t r = m_listCharacter.empty()
        ? m_r
        : m_listCharacter.size() == startCharacterIndex
         ? m_listCharacter.at(startCharacterIndex-1).m_r
         : m_listCharacter.at(startCharacterIndex).m_r;
    uint8_t g = m_listCharacter.empty()
        ? m_g
        : m_listCharacter.size() == startCharacterIndex
         ? m_listCharacter.at(startCharacterIndex-1).m_g
         : m_listCharacter.at(startCharacterIndex).m_g;
    uint8_t b = m_listCharacter.empty()
        ? m_b
        : m_listCharacter.size() == startCharacterIndex
         ? m_listCharacter.at(startCharacterIndex-1).m_b
         : m_listCharacter.at(startCharacterIndex).m_b;

    TgTextCharacter c;
    for (i=0;i<listAddCharacter.size();i++) {
        c.m_character = listAddCharacter.at(i);
        c.m_r = r;
        c.m_g = g;
        c.m_b = b;
        if (m_listCharacter.size() <= startCharacterIndex+i) {
            m_listCharacter.push_back(c);
        } else {
            m_listCharacter.insert(m_listCharacter.begin()+(startCharacterIndex+i), c);
        }
    }

    m_listText.clear();
    if (!m_listCharacter.empty()) {
        TgTextFieldText t;
        char utf8Character[5];
        t.m_textColorR = m_listCharacter.at(0).m_r;
        t.m_textColorG = m_listCharacter.at(0).m_g;
        t.m_textColorB = m_listCharacter.at(0).m_b;
        TgFontGlyphCache::generateCharactedIndexToUtf8(m_listCharacter.at(0).m_character, utf8Character);
        t.m_text = utf8Character;

        for (i=1;i<m_listCharacter.size();i++) {
            if (t.m_textColorR == m_listCharacter.at(i).m_r
                && t.m_textColorG == m_listCharacter.at(i).m_g
                && t.m_textColorB == m_listCharacter.at(i).m_b) {
                TgFontGlyphCache::generateCharactedIndexToUtf8(m_listCharacter.at(i).m_character, utf8Character);
                t.m_text += utf8Character;
                continue;
            }
            m_listText.push_back(t);
            t.m_textColorR = m_listCharacter.at(i).m_r;
            t.m_textColorG = m_listCharacter.at(i).m_g;
            t.m_textColorB = m_listCharacter.at(i).m_b;
            TgFontGlyphCache::generateCharactedIndexToUtf8(m_listCharacter.at(i).m_character, utf8Character);
            t.m_text = utf8Character;
        }
        m_listText.push_back(t);
        TgFontTextGenerator::getCharacters(m_listText, m_listCharacter);
    }

    m_initDone = false;
    m_currentItem->setPositionChanged(true);
    m_mutex.unlock();
}