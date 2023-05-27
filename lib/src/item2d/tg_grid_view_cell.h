/*!
 * \file
 * \brief file tg_grid_view_cell.h
 *
 * single cell in grid view, user should not create this by
 * itself, the TgGridView will create this automatically
 * for each cell
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GRID_VIEW_CELL_H
#define TG_GRID_VIEW_CELL_H

#include "../global/tg_global_macros.h"
#include "tg_item2d.h"
#include <cstddef>
#include <string>
#include <vector>
#include "tg_textfield.h"

class TgGridViewCellPrivate;
class TgGridView;

/*!
 * \brief TgGridViewCell
 * gridview cell class
 */
class TG_MAINWINDOW_EXPORT TgGridViewCell : private TgItem2d
{
public:
    ~TgGridViewCell();

    void setText(const char *text);
    void setText(const std::vector<TgTextFieldText> &listText);
    std::string getText() const;
    void setBackground(const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);
    void setWidth(float width);
    void setHeight(float height);
    void setFontSize(float fontSize);

private:
    explicit TgGridViewCell(TgGridView *parent, float x, float y, float width, float height);

    TgGridViewCellPrivate *m_private;

    friend class TgGridViewPrivate;
    friend class TgGridViewCellPrivate;
};

#endif // TG_GRID_VIEW_CELL_H
