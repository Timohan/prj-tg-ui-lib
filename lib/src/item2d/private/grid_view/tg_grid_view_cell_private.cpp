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