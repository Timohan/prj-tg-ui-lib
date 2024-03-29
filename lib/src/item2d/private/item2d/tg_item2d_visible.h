/*!
 * \file
 * \brief file tg_item2d_visible.h
 *
 * it holds visible values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_PRIVATE_VISIBLE_H
#define TG_ITEM_2D_PRIVATE_VISIBLE_H

#include <functional>
#include <mutex>

enum TgItem2dVisibilityState
{
    TgItem2dVisible = 0,
    TgItem2dInvisible,
    TgItem2dVisibleButParentInvisible
};

class TgItem2d;
class TgItem2dPrivate;

class TgItem2dVisible
{
public:
    TgItem2dVisible(TgItem2d *parent, TgItem2dPrivate *currentItem2dPrivate);

    bool getVisible();
    void setVisible(bool visible);
    bool parentVisibleChanged(bool visible);

    void connectOnVisibleChanged(std::function<void(bool)> visibleChanged);
    void disconnectOnVisibleChanged();
    void setRequireRecheckVisibleChangeToChildren(bool require);
    TgItem2dVisibilityState getVisibleState();

protected:
    void reCheckChildrenVisibility();

private:
    TgItem2d *m_parent;
    TgItem2dPrivate *m_currentItem2dPrivate;
    TgItem2dVisibilityState m_visibleState;
    bool m_requireRecheckVisibleChangeToChildren;
    std::mutex m_mutex;
    bool m_isMenuItem = false;

    std::function<void(bool visible)> f_visibleChanged;
};

#endif // TG_ITEM_2D_PRIVATE_VISIBLE_H
