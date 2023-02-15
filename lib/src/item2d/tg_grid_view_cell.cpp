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
