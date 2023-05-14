/*!
 * \file
 * \brief file tg_mainwindow_menu.h
 *
 * main window (private) class collecting visible menu pointers
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MAIN_WINDOW_MENU_H
#define TG_MAIN_WINDOW_MENU_H

#include <vector>
#include <mutex>
#include "../../event/tg_events.h"
#include "../../item2d/tg_rectangle.h"

class TgMenuItem;
struct TgItem2dPrivateMessage;
struct TgWindowInfo;

class TgMainWindowMenu
{
public:
    TgMainWindowMenu();
    ~TgMainWindowMenu();
    void handlePrivateMessageWindowMenu(const TgItem2dPrivateMessage *message);
    void renderChildrenMenu(const TgWindowInfo *windowInfo);
    TgEventResult handleEventsMenu(TgEventData *eventData, const TgWindowInfo *windowInfo);
    size_t menuItemCount();
    void checkPositionValuesChildrenWindowMenu(const TgWindowInfo *windowInfo);
    bool isMenuEnabled();
protected:
    void hideList();

private:
    std::mutex m_mutexMenu;
    TgItem2d *m_parentMenuItem = nullptr;    /**< just pointer to parent of visible menu items - this is only pointer to TgMenuTop */
    TgMenuItem *m_currentMenuItem = nullptr;    /**< just pointer to current visible menu items - this is only pointer to TgMenuTop */
    std::vector<TgMenuItem *>m_listMenuItem; /**< just pointer to visible menu items */
};

#endif // TG_MAIN_WINDOW_MENU_H
