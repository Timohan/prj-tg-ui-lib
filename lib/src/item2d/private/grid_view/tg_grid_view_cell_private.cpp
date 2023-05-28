/*!
 * \file
 * \brief file tg_grid_view_private.cpp
 *
 * it holds general TgGridViewCellPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_grid_view_cell_private.h"
#include "../../../global/tg_global_log.h"
#include "../../tg_grid_view_cell.h"
#include "../../tg_grid_view.h"
#include "tg_grid_view_private.h"

TgGridViewCellPrivate::TgGridViewCellPrivate(TgGridViewCell *currentItem, TgGridView *parentItem) : m_currentItem(currentItem),
                                                                                                    m_parentItem(parentItem),
                                                                                                    m_background(currentItem, 255, 255, 255),
                                                                                                    m_text(currentItem, "", "", 25, 0, 0, 0)
{
    TG_FUNCTION_BEGIN();
    m_text.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
    m_text.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
    TG_FUNCTION_END();
}

/*! \brief TgGridViewCellPrivate::setText
 * \param text
 */
void TgGridViewCellPrivate::setText(const char *text)
{
    m_text.setText(text);
}

/*!
 * \brief TgGridViewCellPrivate::setText
 *
 * sets multi color text
 *
 * \param listText list of text with different colors
 */
void TgGridViewCellPrivate::setText(const std::vector<TgTextFieldText> &listText)
{
    m_text.setText(listText);
}

/*! \brief TgGridViewCellPrivate::getText
 * \return current text of the cell
 */
std::string TgGridViewCellPrivate::getText() const
{
    return m_text.getText();
}

void TgGridViewCellPrivate::widthChanged(float width)
{
    m_parentItem->m_private->widthCellChanged(width, m_currentItem);
}

void TgGridViewCellPrivate::heightChanged(float height)
{
    m_parentItem->m_private->heightCellChanged(height, m_currentItem);
}

/*!
 * \brief TgGridViewCellPrivate::setBackground
 *
 * sets background color
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgGridViewCellPrivate::setBackground(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    m_background.setColor(r, g, b, a);
}

/*!
 * \brief TgGridViewCellPrivate::setFontSize
 *
 * set font height for cell
 * \param fontSize
 */
void TgGridViewCellPrivate::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_text.setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCellPrivate::setHorizontalAlign
 *
 * set text field's horizontal align
 * \param align horizontal align
 */
void TgGridViewCellPrivate::setHorizontalAlign(TgTextfieldHorizontalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_text.setHorizontalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCellPrivate::setVerticalAlign
 *
 * set text field's vertical align
 * \param align vertical align
 */
void TgGridViewCellPrivate::setVerticalAlign(TgTextfieldVerticalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_text.setVerticalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgGridViewCellPrivate::getAlignHorizontal
 *
 * \return get horizontal align
 */
TgTextfieldHorizontalAlign TgGridViewCellPrivate::getAlignHorizontal() const
{
    return m_text.getAlignHorizontal();
}
/*!
 * \brief TgGridViewCellPrivate::getAlignVertical
 *
 * \return get vertical align
 */
TgTextfieldVerticalAlign TgGridViewCellPrivate::getAlignVertical() const
{
    return m_text.getAlignVertical();
}

/*!
 * \brief TgGridViewCellPrivate::setMargin
 *
 * set text margin left, top, right and bottom values
 * \param left
 * \param top
 * \param right
 * \param bottom
 */
void TgGridViewCellPrivate::setTextMargin(float left, float top, float right, float bottom)
{
    m_text.setMargin(left, top, right, bottom);
}

/*!
 * \brief TgGridViewCellPrivate::setTextMarginLeft
 *
 * set text margin left
 * \param left
 */
void TgGridViewCellPrivate::setTextMarginLeft(float left)
{
    m_text.setMarginLeft(left);
}

/*!
 * \brief TgGridViewCellPrivate::setTextMarginTop
 *
 * set text margin top
 * \param top
 */
void TgGridViewCellPrivate::setTextMarginTop(float top)
{
    m_text.setMarginTop(top);
}

/*!
 * \brief TgGridViewCellPrivate::setTextMarginRight
 *
 * set text margin right
 * \param right
 */
void TgGridViewCellPrivate::setTextMarginRight(float right)
{
    m_text.setMarginRight(right);
}

/*!
 * \brief TgGridViewCellPrivate::setTextMarginBottom
 *
 * set text margin bottom
 * \param bottom
 */
void TgGridViewCellPrivate::setTextMarginBottom(float bottom)
{
    m_text.setMarginBottom(bottom);
}

/*!
 * \brief TgGridViewCellPrivate::getMarginLeft
 *
 * \return left margin
 */
float TgGridViewCellPrivate::getTextMarginLeft()
{
    return m_text.getMarginLeft();
}

/*!
 * \brief TgGridViewCellPrivate::getTextMarginTop
 *
 * \return top margin
 */
float TgGridViewCellPrivate::getTextMarginTop()
{
    return m_text.getMarginTop();
}

/*!
 * \brief TgGridViewCellPrivate::getTextMarginRight
 *
 * \return right margin
 */
float TgGridViewCellPrivate::getTextMarginRight()
{
    return m_text.getMarginRight();
}

/*!
 * \brief TgGridViewCellPrivate::getTextMarginBottom
 *
 * \return bottom margin
 */
float TgGridViewCellPrivate::getTextMarginBottom()
{
    return m_text.getMarginBottom();
}