/*!
 * \file
 * \brief file tg_grid_view.cpp
 *
 * Gridview
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_grid_view.h"
#include "../global/tg_global_log.h"
#include "private/grid_view/tg_grid_view_private.h"
#include "private/item2d/tg_item2d_private.h"

TgGridView::TgGridView(TgItem2d *parent, size_t columnCount, size_t rowCount) :
    TgItem2d(parent),
    m_private(new TgGridViewPrivate(this, columnCount, rowCount))
{

}

TgGridView::TgGridView(TgItem2d *parent, float x, float y, float width, float height, size_t columnCount, size_t rowCount) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgGridViewPrivate(this, columnCount, rowCount))
{
}

/*! \brief TgGridView::getColumCount
 * \return column count
 */
size_t TgGridView::getColumCount()
{
    return m_private->getColumCount();
}

/*! \brief TgGridView::getRowCount
 * \return row count
 */
size_t TgGridView::getRowCount()
{
    return m_private->getRowCount();
}

/*! \brief TgGridView::setRowCount
 * \param row
 */
void TgGridView::setRowCount(size_t row)
{
    m_private->setRowCount(row);
}

/*! \brief TgGridView::setColumCount
 * \param column count
 */
void TgGridView::setColumCount(size_t column)
{
    m_private->setColumCount(column);
}

/*! \brief TgGridView::getCell
 *
 * get pointer of cell item
 *
 * \param column
 * \param row
 * \return nullptr if item doesn't exists (row or column is too large)
 * otherwise returns pointer of item
 */
TgGridViewCell *TgGridView::getCell(size_t column, size_t row)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getCell(column, row);
}

/*!
 * \brief TgGridView::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgGridView::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (TgItem2d::m_private->getPositionChanged()) {
        m_private->setSliderVisibilityAndPosition();
        TgItem2d::m_private->setPositionChanged(false);
    }
    TgItem2d::checkPositionValues();
    TG_FUNCTION_END();
}