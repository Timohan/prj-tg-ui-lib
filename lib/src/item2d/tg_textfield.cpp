/*!
 * \file
 * \brief file tg_textfield.cpp
 *
 * Inits and draws text
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_textfield.h"
#include "../global/tg_global_log.h"
#include "private/tg_textfield_private.h"
#include "private/item2d/tg_item2d_private.h"

/*!
 * \brief TgTextfield::TgTextfield
 *
 * TgTextfield constructor
 *
 * constructor to use AnchorFollowParentSize (check TgItem2D)
 * position is parent
 *
 * \param parent
 * \param text text in UTF-8
 * \param fontFile full filepath of ttf file, if empty (""), then use default font
 * \param fontSize font size
 * \param r base red color of text
 * \param g base green color of text
 * \param b base blue color of text
 */
TgTextfield::TgTextfield(TgItem2d *parent, const char *text, const char *fontFile, float fontSize, uint8_t r, uint8_t g, uint8_t b) :
    TgItem2d(parent),
    m_private(new TgTextfieldPrivate(this, text, fontFile ? fontFile : "", fontSize, r, g, b))

{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}


/*!
 * \brief TgTextfield::TgTextfield
 *
 * TgTextfield constructor
 * 
 * constructor to use AnchorRelativeToParent (check TgItem2D)
 * position is relative to parent
 *
 * \param parent
 * \param x text field position x (relative to parent)
 * \param y text field position y (relative to parent)
 * \param width text field position width
 * \param height text field position height
 * \param text text in UTF-8
 * \param fontFile full filepath of ttf file, if empty (""), then use default font
 * \param fontSize font size
 * \param r base red color of text
 * \param g base green color of text
 * \param b base blue color of text
 */
TgTextfield::TgTextfield(TgItem2d *parent, float x, float y, float width, float height, const char *text, const char *fontFile, float fontSize,
                         uint8_t r, uint8_t g, uint8_t b) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgTextfieldPrivate(this, text, fontFile, fontSize, r, g, b))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgTextfield::~TgTextfield()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::render
 *
 * Renders the text
 * \param windowInfo
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgTextfield::render(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return false;
    }
    TG_FUNCTION_END();
    return m_private->render(windowInfo, this, reinterpret_cast<TgItem2d *>(this)->m_private);
}

/*!
 * \brief TgTextfield::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgTextfield::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::setHorizontalAlign
 *
 * set text field's horizontal align
 * \param align horizontal align
 */
void TgTextfield::setHorizontalAlign(TgTextfieldHorizontalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_private->setHorizontalAlign(this, align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::setVerticalAlign
 *
 * set text field's vertical align
 * \param align vertical align
 */
void TgTextfield::setVerticalAlign(TgTextfieldVerticalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_private->setVerticalAlign(this, align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::getAlignHorizontal
 *
 * \return get horizontal align
 */
TgTextfieldHorizontalAlign TgTextfield::getAlignHorizontal() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAlignHorizontal();
}

/*!
 * \brief TgTextfield::getAlignVertical
 *
 * \return get vertical align
 */
TgTextfieldVerticalAlign TgTextfield::getAlignVertical() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAlignVertical();
}

/*!
 * \brief TgTextfield::setText
 *
 * sets single color text
 * color of the text is base color
 *
 * \param text text
 */
void TgTextfield::setText(const char *text)
{
    TG_FUNCTION_BEGIN();
    m_private->setText(text, this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::setText
 *
 * sets multi color text
 *
 * \param listText list of text with different colors
 */
void TgTextfield::setText(const std::vector<TgTextFieldText> &listText)
{
    TG_FUNCTION_BEGIN();
    m_private->setText(listText, this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::getText
 *
 * \return text (utf8)
 */
std::string TgTextfield::getText() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getText();
}

/*!
 * \brief TgTextfield::setFontSize
 *
 * \param fontSize fon size for text
 */
void TgTextfield::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::getFontSize
 *
 * \return font size
 */
float TgTextfield::getFontSize()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getFontSize();
}

/*!
 * \brief TgTextfield::getFontFile
 *
 * \return get main font file
 */
std::string TgTextfield::getFontFile()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getFontFile();
}

/*!
 * \brief TgTextfield::getCharacterCount
 *
 * \return get count of characters in this text
 */
size_t TgTextfield::getCharacterCount()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCharacterCount();
}

/*!
 * \brief TgTextfield::getCharacterByIndex
 *
 * \param index of the character
 * \return the uint32 value of the character by the index
 * returns 0, if index larger than character count
 */
uint32_t TgTextfield::getCharacterByIndex(size_t index)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCharacterByIndex(index);
}

/*!
 * \brief TgTextfield::getTextWidth
 *
 * \return text width (pixels)
 */
float TgTextfield::getTextWidth()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getTextWidth();
}

/*!
 * \brief TgTextfield::getTextHeight
 *
 * \return text single line height (pixels)
 */
float TgTextfield::getTextHeight()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getTextHeight();
}

/*!
 * \brief TgTextfield::setMaxLineCount
 *
 * \param maxLineCount max line count
 * default: 1
 * if max line count is 0, then it's unlimited number of lines
 */
void TgTextfield::setMaxLineCount(uint32_t maxLineCount)
{
    TG_FUNCTION_BEGIN();
    m_private->setMaxLineCount(maxLineCount);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::getMaxLineCount
 *
 * \return max line count
 */
uint32_t TgTextfield::getMaxLineCount() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getMaxLineCount();
}

/*!
 * \brief TgTextfield::getAllDrawTextHeight
 *
 * \return height of the all drawed texts (including line breaks)
 */
float TgTextfield::getAllDrawTextHeight()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAllDrawTextHeight();
}

/*!
 * \brief TgTextfield::setWordWrap
 *
 * \param wordWrap word wrap
 */
void TgTextfield::setWordWrap(TgTextFieldWordWrap wordWrap)
{
    TG_FUNCTION_BEGIN();
    m_private->setWordWrap(wordWrap);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::getWordWrap
 *
 * \return current word wrap
 */
TgTextFieldWordWrap TgTextfield::getWordWrap() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getWordWrap();
}

/*!
 * \brief TgTextfield::setAllowBreakLineGoOverMaxLine
 *
 * \param allowBreakLineGoOverMaxLine true, text can go over the
 * max line count with line break '\n', but those lines are not
 * drawed
 * false - line break marks '\n' are ignored on the last line
 * defined by max line count setMaxLineCount()
 *
 * default value: false
 */
void TgTextfield::setAllowBreakLineGoOverMaxLine(bool allowBreakLineGoOverMaxLine)
{
    TG_FUNCTION_BEGIN();
    m_private->setAllowBreakLineGoOverMaxLine(allowBreakLineGoOverMaxLine);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextfield::getAllowBreakLineGoOverMaxLine
 *
 * \return true, text can go over the max line count with
 * line break '\n', but those lines are not drawed
 * false - line break marks '\n' are ignored on the last line
 * defined by max line count setMaxLineCount()
 *
 * default value: false
 */
bool TgTextfield::getAllowBreakLineGoOverMaxLine() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAllowBreakLineGoOverMaxLine();
}

/*!
 * \brief TgTextfield::getColor
 * get default color of the text field
 * \param r red
 * \param g green
 * \param b blue
 */
void TgTextfield::getColor(uint8_t &r, uint8_t &g, uint8_t &b)
{
    TG_FUNCTION_BEGIN();
    m_private->getColor(r, g, b);
    TG_FUNCTION_END();
}
