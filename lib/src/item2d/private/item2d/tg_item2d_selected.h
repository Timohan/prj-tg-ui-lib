/*!
 * \file
 * \brief file tg_item2d_selected.h
 *
 * it holds selected values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_PRIVATE_SELECTED_H
#define TG_ITEM_2D_PRIVATE_SELECTED_H

#include <functional>

class TgItem2d;
class TgItem2dPrivate;

class TgItem2dSelected
{
public:
    TgItem2dSelected(TgItem2d *parent, TgItem2dPrivate *currentItem2dPrivate);

    bool getSelected();
    bool getCanSelect();

    void setSelected(bool selected);
    void setCanSelect(bool canSelect);
private:
    TgItem2d *m_parent;
    TgItem2dPrivate *m_currentItem2dPrivate;

    bool m_selected;
    bool m_canSelect;
};

#endif // TG_ITEM_2D_PRIVATE_SELECTED_H
