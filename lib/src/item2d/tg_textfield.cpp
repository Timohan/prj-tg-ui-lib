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
    m_private(new TgTextfieldPrivate(text, fontFile, fontSize, r, g, b))

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
    m_private(new TgTextfieldPrivate(text, fontFile, fontSize, r, g, b))
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
 */
void TgTextfield::render(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->render(windowInfo, this);
    TG_FUNCTION_END();
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
    m_private->checkPositionValues(this);
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

