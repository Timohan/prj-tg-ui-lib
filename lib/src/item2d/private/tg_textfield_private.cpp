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
#include  <cmath>
#include "../../global/tg_global_application.h"
#include "../../font/tg_font_cache.h"
#include "../../font/tg_font_default.h"
#include "../../global/tg_global_log.h"
#include "../tg_item2d.h"
#include "../../window/tg_mainwindow_private.h"

TgTextfieldPrivate::TgTextfieldPrivate(const char *text, const char *fontFile, float fontSize,
                                       uint8_t r, uint8_t g, uint8_t b) :
    m_fontInfo(nullptr),
    m_r(static_cast<float>(r)/255.0f),
    m_g(static_cast<float>(g)/255.0f),
    m_b(static_cast<float>(b)/255.0f),
    m_text(text),
    m_fontFile(fontFile),
    m_fontSize(fontSize),
    m_initDone(false),
    m_alignHorizontal(TgTextfieldHorizontalAlign::AlignLeft),
    m_alignVertical(TgTextfieldVerticalAlign::AlignTop)
{
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
    float x, y;
    m_listTransform.resize( m_characterPositions.m_listCharacterInFontInfoIndex.size() );
    if (!m_fontInfo) {
        return;
    }

    switch (m_alignHorizontal) {
        case TgTextfieldHorizontalAlign::AlignLeft:
        default:
            x = currentItem->getXonWindow();
            break;
        case TgTextfieldHorizontalAlign::AlignCenterH:
            x = currentItem->getWidth()/2+currentItem->getXonWindow() - m_characterPositions.m_textWidth/2;
            break;
        case TgTextfieldHorizontalAlign::AlignRight:
            x = currentItem->getWidth()+currentItem->getXonWindow() - m_characterPositions.m_textWidth;
            break;
    }
    switch (m_alignVertical) {
        case TgTextfieldVerticalAlign::AlignTop:
        default:
            y = currentItem->getYonWindow();
            break;
        case TgTextfieldVerticalAlign::AlignCenterV:
            y = currentItem->getHeight()/2+currentItem->getYonWindow() - m_fontInfo->m_fontHeight/2;
            break;
        case TgTextfieldVerticalAlign::AlignBottom:
            y = currentItem->getHeight()+currentItem->getYonWindow() - m_fontInfo->m_fontHeight;
            break;
    }

    x = std::roundf(x);
    y = std::roundf(y);
    for (i=0;i<m_characterPositions.m_listCharacterInFontInfoIndex.size();i++) {
        m_listTransform[i].setTransform(m_characterPositions.m_listTransformX[i]+x, y);
    }
}

/*!
 * \brief TgTextfieldPrivate::setText
 *
 * \param text text
 */
void TgTextfieldPrivate::setText(const char *text, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (m_text.compare(text) == 0) {
        TG_FUNCTION_END();
        return;
    }
    m_text = text;
    m_initDone = false;
    currentItem->setPositionChanged(true);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgTextfieldPrivate::checkPositionValues(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (!m_initDone) {
        m_fontInfo = TgGlobalApplication::getInstance()->getFontCache()->generateCacheForText(m_text.c_str(), m_fontFile.c_str(), m_fontSize);
        TgCharacterPositions::generateTextCharacterPositioning(m_fontInfo, m_text.c_str(), m_characterPositions);
        m_initDone = true;
    }
    if (currentItem->getPositionChanged()) {
        generateTransform(currentItem);
        currentItem->setAddMinMaxHeightOnVisible(m_characterPositions.m_visibleTopY,  m_characterPositions.m_visibleBottomY);
        currentItem->setPositionChanged(false);
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfieldPrivate::render
 *
 * Renders the text
 * \param windowInfo
 */
void TgTextfieldPrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!m_fontInfo) {
        TG_FUNCTION_END();
        return;
    }

    glUniform1i( windowInfo->m_shaderRenderTypeIndex, 1);
    glUniform4f( windowInfo->m_shaderColorIndex, m_r, m_g, m_b, 1);
    glUniform4f(windowInfo->m_maxRenderValues,
                currentItem->getXminOnVisible(), currentItem->getYminOnVisible(),
                currentItem->getXmaxOnVisible(windowInfo),
                currentItem->getYmaxOnVisible(windowInfo));
    TgGlobalApplication::getInstance()->getFontCache()->render(m_fontInfo, &m_characterPositions, windowInfo->m_shaderTransformIndex, m_listTransform);
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
