/*!
 * \file
 * \brief file tg_grid_view_cell_private.h
 *
 * it holds general TgGridViewPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GRID_VIEW_CELL_PRIVATE_H
#define TG_GRID_VIEW_CELL_PRIVATE_H

class TgGridViewCell;
class TgGridView;
#include "../../tg_textfield.h"
#include "../../tg_rectangle.h"
#include <string>

class TgGridViewCellPrivate
{
public:
    TgGridViewCellPrivate(TgGridViewCell *currentItem, TgGridView *parentItem);

    void setText(const char *text);
    void setText(const std::vector<TgTextFieldText> &listText);
    std::string getText() const;
    void widthChanged(float width);
    void heightChanged(float height);
    void setBackground(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
    void setFontSize(float fontSize);

private:
    TgGridViewCell *m_currentItem;
    TgGridView *m_parentItem;
    TgRectangle m_background;
    TgTextfield m_text;
};

#endif // TG_GRID_VIEW_CELL_PRIVATE_H
