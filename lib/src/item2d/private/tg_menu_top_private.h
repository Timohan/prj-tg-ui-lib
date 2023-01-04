/*!
 * \file
 * \brief file tg_menu_top_private.h
 *
 * Top menu (private)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MENU_TOP_PRIVATE_H
#define TG_MENU_TOP_PRIVATE_H

#include <mutex>
#include "../../global/tg_global_macros.h"
#include "../../event/tg_event_data.h"
#include "../tg_menu_item.h"
#include "../tg_rectangle.h"

/*!
 * \brief TgMenuTopPrivate
 * Top menu for TgItem2d, normally contains File, Edit and such
 */
class TG_MAINWINDOW_EXPORT TgMenuTopPrivate
{
public:
    explicit TgMenuTopPrivate(TgItem2d *currentItem, TgItem2d *parentItem);
    ~TgMenuTopPrivate();

    TgMenuItem *addMenu(const char *text, const TgShortCut *shortCut);

    void checkOnResizeChanged();
    void checkPositionValuesMenu(bool useLock = true);

    size_t getMenuCount();
    bool removeMenu(size_t i);

    void deleteMenu(TgMenuItem *item);
protected:
    TgItem2d *m_currentItem;
    TgItem2d *m_parentItem;
    std::vector<TgMenuItem *>m_listMainMenuItem;
    std::mutex m_mutex;
    TgRectangle m_background;

};

#endif // TG_MENU_TOP_PRIVATE_H
