/*!
 * \file
 * \brief file tg_grid_view_cell.cpp
 *
 * single cell in grid view, user should not create this by
 * itself, the TgGridView will create this automatically
 * for each cell
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_grid_view_cell.h"
#include "private/grid_view/tg_grid_view_cell_private.h"
#include "tg_grid_view.h"
#include "../global/tg_global_log.h"
#include "private/item2d/tg_item2d_private.h"

TgGridViewCell::TgGridViewCell(TgGridView *parent, float x, float y, float width, float height) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgGridViewCellPrivate(this, parent))
{
    TgItem2d::m_private->setCurrentGridViewCell(m_private);
}

TgGridViewCell::~TgGridViewCell()
{
    if (m_private) {
        delete m_private;
    }
}

/*! \brief TgGridViewCell::setText
 * \param text
 */
void TgGridViewCell::setText(const char *text)
{
    m_private->setText(text);
}

/*!
 * \brief TgGridViewCell::setText
 *
 * sets multi color text
 *
 * \param listText list of text with different colors
 */
void TgGridViewCell::setText(const std::vector<TgTextFieldText> &listText)
{
    m_private->setText(listText);
}

std::string TgGridViewCell::getText() const
{
    return m_private->getText();
}

/*!
 * \brief TgGridViewCell::setBackground
 *
 * sets background color
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgGridViewCell::setBackground(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    m_private->setBackground(r, g, b, a);
}

/*!
 * \brief TgGridViewCell::setWidth
 *
 * set width type for item
 * \param type
 */
void TgGridViewCell::setWidthType(TgGridViewCellSizeType type)
{
    TG_FUNCTION_BEGIN();
    m_private->setWidthType(type);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCell::setWidth
 *
 * set width for item
 * \param width
 */
void TgGridViewCell::setWidth(float width)
{
    TG_FUNCTION_BEGIN();
    TgItem2d::setWidth(width);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCell::setHeight
 *
 * set height for item
 * \param height
 */
void TgGridViewCell::setHeight(float height)
{
    TG_FUNCTION_BEGIN();
    TgItem2d::setHeight(height);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCell::setFontSize
 *
 * set font height for cell
 * \param fontSize
 */
void TgGridViewCell::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCell::setTextHorizontalAlign
 *
 * set text field's horizontal align
 * \param align horizontal align
 */
void TgGridViewCell::setTextHorizontalAlign(TgTextfieldHorizontalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_private->setHorizontalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCell::setTextVerticalAlign
 *
 * set text field's vertical align
 * \param align vertical align
 */
void TgGridViewCell::setTextVerticalAlign(TgTextfieldVerticalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_private->setVerticalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCell::getTextAlignHorizontal
 *
 * \return get horizontal align
 */
TgTextfieldHorizontalAlign TgGridViewCell::getTextAlignHorizontal() const
{
    return m_private->getAlignHorizontal();
}
/*!
 * \brief TgGridViewCell::getTextAlignVertical
 *
 * \return get vertical align
 */
TgTextfieldVerticalAlign TgGridViewCell::getTextAlignVertical() const
{
    return m_private->getAlignVertical();
}

/*!
 * \brief TgGridViewCell::setMargin
 *
 * set text margin left, top, right and bottom values
 * \param left
 * \param top
 * \param right
 * \param bottom
 */
void TgGridViewCell::setTextMargin(float left, float top, float right, float bottom)
{
    m_private->setTextMargin(left, top, right, bottom);
}

/*!
 * \brief TgGridViewCell::setTextMarginLeft
 *
 * set text margin left
 * \param left
 */
void TgGridViewCell::setTextMarginLeft(float left)
{
    m_private->setTextMarginLeft(left);
}

/*!
 * \brief TgGridViewCell::setTextMarginTop
 *
 * set text margin top
 * \param top
 */
void TgGridViewCell::setTextMarginTop(float top)
{
    m_private->setTextMarginTop(top);
}

/*!
 * \brief TgGridViewCell::setTextMarginRight
 *
 * set text margin right
 * \param right
 */
void TgGridViewCell::setTextMarginRight(float right)
{
    m_private->setTextMarginRight(right);
}

/*!
 * \brief TgGridViewCell::setTextMarginBottom
 *
 * set text margin bottom
 * \param bottom
 */
void TgGridViewCell::setTextMarginBottom(float bottom)
{
    m_private->setTextMarginBottom(bottom);
}

/*!
 * \brief TgGridViewCell::getMarginLeft
 *
 * \return left margin
 */
float TgGridViewCell::getTextMarginLeft()
{
    return m_private->getTextMarginLeft();
}

/*!
 * \brief TgGridViewCell::getTextMarginTop
 *
 * \return top margin
 */
float TgGridViewCell::getTextMarginTop()
{
    return m_private->getTextMarginTop();
}

/*!
 * \brief TgGridViewCell::getTextMarginRight
 *
 * \return right margin
 */
float TgGridViewCell::getTextMarginRight()
{
    return m_private->getTextMarginRight();
}

/*!
 * \brief TgGridViewCell::getTextMarginBottom
 *
 * \return bottom margin
 */
float TgGridViewCell::getTextMarginBottom()
{
    return m_private->getTextMarginBottom();
}

/*!
 * \brief TgGridViewCell::setTooltip
 *
 * set tooltip text
 *
 * \return text
 */
void TgGridViewCell::setTooltip(const char *text)
{
    TgItem2d::setTooltip(text);
}