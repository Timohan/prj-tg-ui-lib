/*!
 * \file
 * \brief file tg_item2d_menu.h
 *
 * It holds the pointer for menu items
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_MENU_H
#define TG_ITEM_2D_MENU_H

#include <vector>
#include <cstddef>
#include <mutex>
#include "../../../event/tg_event_data.h"
class TgMenuItem;
struct TgShortCut;
class TgItem2d;
struct TgEventData;
struct TgWindowInfo;

class TgItem2dMenu
{
public:
    explicit TgItem2dMenu(TgItem2d *currentItem, TgItem2d *parentItem);
    virtual ~TgItem2dMenu();
    TgMenuItem *addMenu(TgMenuItem *parentMenuItem, const char *text, const TgShortCut *shortCut, bool subMenu = false);
    size_t getMenuCount();
    TgMenuItem *getMenu(size_t index);

    bool getMenuVisible();
    void renderMenu(const TgWindowInfo *windowInfo, float parentOpacity);

    TgEventResult handleEventItem2dMenu(TgEventData *eventData, const TgWindowInfo *windowInfo);
    void setTopMenu(bool topMenu);
    bool getTopMenu();

    void setMenuVisible(float x, float y, float w, const TgWindowInfo *windowInfo);
    void renderChildrenMenu(const TgWindowInfo *windowInfo, float parentOpacity);

    void hideSubMenuList();
    TgEventResult handleChildrenEventMenu(TgEventData *eventData, const TgWindowInfo *windowInfo);

    size_t getSubMenuCount();
    bool removeSubMenu(size_t i);
    void deleteMenu(TgMenuItem *item);
    size_t getPossibleVisibleMenuCount();
    void setDeletingSubMenu();
    static void setMenuItemsOpen(bool topMenu, TgItem2d *currentItem, TgItem2d *parentItem, std::vector<TgMenuItem *>&listChildrenMenu, TgEventData *eventData, const TgWindowInfo *windowInfo, size_t currentIndex = 0);
protected:
    void checkPositionValuesChildrenMenu(const TgWindowInfo *windowInfo);

private:
    bool m_topMenu = { false };
    TgItem2d *m_currentItem;
    TgItem2d *m_parentItem;
    std::vector<TgMenuItem *>m_listChildrenMenu;
    std::mutex m_mutexMenu;

    static void getAreaSizeOfMenuItems(std::vector<TgMenuItem *> &listMenu, float &width, float &height, float &shortcutWidth, float &submenuArrowWidth);

    void setMenuItemsOpen(TgEventData *eventData, const TgWindowInfo *windowInfo);
};

#endif // TG_ITEM_2D_MENU_H
