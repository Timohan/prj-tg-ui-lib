/*!
 * \file
 * \brief file tg_grid_view.h
 *
 * Gridview
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GRID_VIEW_H
#define TG_GRID_VIEW_H

#include "../global/tg_global_macros.h"
#include "tg_item2d.h"
#include <cstddef>

class TgGridViewPrivate;
class TgGridViewCell;

/*!
 * \brief TgGridView
 * gridview class
 */
class TG_MAINWINDOW_EXPORT TgGridView : public TgItem2d
{
public:
    explicit TgGridView(TgItem2d *parent, size_t columnCount, size_t rowCount);
    explicit TgGridView(TgItem2d *parent, float x, float y, float width, float height, size_t columnCount, size_t rowCount);

    size_t getColumCount();
    size_t getRowCount();
    void setRowCount(size_t row);
    void setColumCount(size_t column);
    TgGridViewCell *getCell(size_t column, size_t row);

protected:
    virtual void checkPositionValues() override;
private:
    TgGridViewPrivate *m_private;

    friend class TgGridViewCellPrivate;
};

#endif // TG_GRID_VIEW_H
