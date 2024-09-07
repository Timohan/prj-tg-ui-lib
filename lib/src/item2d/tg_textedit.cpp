/*!
 * \file
 * \brief file tg_textedit.cpp
 *
 * Inits and draws text
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_textedit.h"
#include "../global/tg_global_log.h"
#include "private/tg_textedit_private.h"

/*!
 * \brief TgTextedit::TgTextedit
 *
 * TgTextedit constructor
 *
 * constructor to use AnchorFollowParentSize (check TgItem2D)
 * position is parent
 *
 * \param parent
 * \param text text in UTF-8
 * \param fontFile full filepath of ttf file, if empty (""), then use default font
 * \param fontSize font size
 * \param r red color of text
 * \param g green color of text
 * \param b blue color of text
 */
TgTextedit::TgTextedit(TgItem2d *parent, const char *text, const char *fontFile, float fontSize, uint8_t r, uint8_t g, uint8_t b) :
    TgMouseCapture(parent),
    m_private(new TgTexteditPrivate(this, text, fontFile, fontSize, r, g, b))

{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::TgTextedit
 *
 * TgTextedit constructor
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
 * \param r red color of text
 * \param g green color of text
 * \param b blue color of text
 */
TgTextedit::TgTextedit(TgItem2d *parent, float x, float y, float width, float height, const char *text, const char *fontFile, float fontSize,
                         uint8_t r, uint8_t g, uint8_t b) :
    TgMouseCapture(parent, x, y, width, height),
    m_private(new TgTexteditPrivate(this, text, fontFile, fontSize, r, g, b))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgTextedit::~TgTextedit()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgTextedit::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::setHorizontalAlign
 *
 * set text field's horizontal align
 * \param align horizontal align
 */
void TgTextedit::setHorizontalAlign(TgTextfieldHorizontalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_private->setHorizontalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::setVerticalAlign
 *
 * set text field's vertical align
 * \param align vertical align
 */
void TgTextedit::setVerticalAlign(TgTextfieldVerticalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_private->setVerticalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::getAlignHorizontal
 *
 * \return get horizontal align
 */
TgTextfieldHorizontalAlign TgTextedit::getAlignHorizontal() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAlignHorizontal();
}

/*!
 * \brief TgTextedit::getAlignVertical
 *
 * \return get vertical align
 */
TgTextfieldVerticalAlign TgTextedit::getAlignVertical() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getAlignVertical();
}

/*!
 * \brief TgTextedit::setText
 *
 * \param text text
 */
void TgTextedit::setText(const char *text)
{
    TG_FUNCTION_BEGIN();
    m_private->setText(text);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::setText
 *
 * sets multi color text
 *
 * \param listText list of text with different colors
 */
void TgTextedit::setText(const std::vector<TgTextFieldText> &listText)
{
    TG_FUNCTION_BEGIN();
    m_private->setText(listText);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::getText
 *
 * \return text (utf8)
 */
std::string TgTextedit::getText() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getText();
}

/*!
 * \brief TgTextedit::setFontSize
 *
 * \param fontSize fon size for text
 */
void TgTextedit::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::onHoverChanged
 *
 * when hover state is changed, this is called
 *
 * \param hover true if cursor is on button
 */
void TgTextedit::onHoverChanged(bool hover)
{
    TG_FUNCTION_BEGIN();
    (void)hover;
    m_private->changeImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::onHoverDownChanged
 *
 * virtual function from TgMouseCapture when down is changed
 *
 * \param down if true, mouse cursor press down
 */
void TgTextedit::onDownChanged(bool down)
{
    TG_FUNCTION_BEGIN();
    m_private->changeDownProperty(down);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::onEnabledChanged
 *
 * virtual function from TgItem2d when enabled changed
 *
 * \param enabled enabled true/false
 */
void TgTextedit::onEnabledChanged(bool)
{
    TG_FUNCTION_BEGIN();
    m_private->changeImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
TgEventResult TgTextedit::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (m_private->handleEvent(eventData) == TgEventResult::EventResultCompleted) {
        return TgEventResult::EventResultCompleted;
    }
    TG_FUNCTION_END();
    return TgMouseCapture::handleEvent(eventData, windowInfo);
}

/*!
 * \brief TgTextedit::onMousePressed
 *
 * virtual function from TgMouseCapture when mouse is pressed down
 *
 * \param button button
 * \param x x position
 * \param y y position
 */
void TgTextedit::onMousePressed(TgMouseType button, float x, float)
{
    TG_FUNCTION_BEGIN();
    if (button == TgMouseType::ButtonLeft) {
        m_private->onMousePressed(x);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::onMouseReleased
 *
 * virtual function when mouse is released (up)
 *
 * \param button button
 * \param inArea if true, mouse was released on this TgMouseCapture area,
 * if false, then mouse was released outside the TgMouseCapture area
 * \param x x position
 * \param y y position
 */
void TgTextedit::onMouseReleased(TgMouseType button, bool, float x, float)
{
    TG_FUNCTION_BEGIN();
    if (button == TgMouseType::ButtonLeft) {
        m_private->onMouseReleased( x);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::onMouseMove
 *
 * virtual function when mouse is moving on the TgMouseCapture area
 *
 * \param inArea if true, mouse was moving in this area
 * this can be false, only if it was pressed down in the this
 * TgMouseCapture
 * \param x x position
 * \param y y position
 */
void TgTextedit::onMouseMove(bool, float x, float)
{
    TG_FUNCTION_BEGIN();
    m_private->onMouseMove(x);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTextedit::getTextLeftMargin
 *
 * the default margin between background area of textfield
 * and left text start position in default position
 *
 * \return left text margin
 */
float TgTextedit::getTextLeftMargin()
{
    return m_private->getTextLeftMargin();
}

/*!
 * \brief TgTextedit::getTextRightMargin
 *
 * the default margin between background area of textfield
 * and right text start position in default position
 *
 * \return right text margin
 */
float TgTextedit::getTextRightMargin()
{
    return m_private->getTextRightMargin();
}

/*!
 * \brief TgTextedit::setTextLeftMargin
 *
 * the default margin between background area of textfield
 * and left text start position in default position
 *
 * \param margin left text margin
 */
void TgTextedit::setTextLeftMargin(float margin)
{
    m_private->setTextLeftMargin(margin);
}

/*!
 * \brief TgTextedit::setTextRightMargin
 *
 * the default margin between background area of textfield
 * and right text start position in default position
 *
 * \param margin right text margin
 */
void TgTextedit::setTextRightMargin(float margin)
{
    m_private->setTextRightMargin(margin);
}

/*!
 * \brief TgTextedit::getCursorPosition
 *
 * get cursor position
 *
 * \return cursor position
 */
size_t TgTextedit::getCursorPosition()
{
    return m_private->getCursorPosition();
}

/*!
 * \brief TgTextedit::getCharacterCount
 *
 * \return get count of characters in this text
 */
size_t TgTextedit::getCharacterCount()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCharacterCount();
}

/*!
 * \brief TgTextedit::getCharacterByIndex
 *
 * \param index of the character
 * \return the uint32 value of the character by the index
 * returns 0, if index larger than character count
 */
uint32_t TgTextedit::getCharacterByIndex(size_t index)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCharacterByIndex(index);
}

/*!
 * \brief TgTextedit::getSelectedTextSize
 *
 * \return get number of characters selected
 * if it's negative, then selection area from getCursorPosition goes backward on text
 * if it's position, then selection area from getCursorPosition goes forward on text
 */
int64_t TgTextedit::getSelectedTextSize()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getSelectedTextSize();
}

/*!
 * \brief TgTextedit::getTextWidth
 *
 * \return get text width
 */
float TgTextedit::getTextWidth()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getTextWidth();
}

/*!
 * \brief TgTextedit::getTextHeight
 *
 * \return text single line height (pixels)
 */
float TgTextedit::getTextHeight()
{
    return m_private->getTextHeight();
}