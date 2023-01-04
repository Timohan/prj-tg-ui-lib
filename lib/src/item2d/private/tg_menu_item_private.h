/*!
 * \file
 * \brief file tg_menu_item_private.cpp
 *
 * Menu item (private)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MENU_ITEM_PRIVATE_H
#define TG_MENU_ITEM_PRIVATE_H

struct TgShortCut;
class TgMenuItem;
#include "../tg_textfield.h"
#include "../tg_rectangle.h"
#include "../tg_image.h"
#include "../tg_menu_item.h"

#define MENU_ITEM_LEFT_TEXT_MARGIN  10.0f
#define MENU_ITEM_RIGHT_TEXT_MARGIN 10.0f
#define MENU_ITEM_FONT_SIZE         17.0f

/*!
 * \brief TgMenuItemPrivate
 * Menu item
 */
class TgMenuItemPrivate
{
public:
    enum MenuType {
        MenuType_NormalMenu = 0,
        MenuType_TopMenu,
        MenuType_TopDropDownMenu,
        MenuType_SubMenu,
    };

    explicit TgMenuItemPrivate(TgMenuItem *parentItem, TgMenuItem *currentItem, const char *text, const TgShortCut *shortCut);
    ~TgMenuItemPrivate();

    float getMinWidth();
    float getMinShortcutWidth();
    void onHoverChanged(bool hover);
    void onEnabledChanged(bool enabled);
    void onVisibleChanged(bool visible, bool haveSubMenu);

    void setMenuType(MenuType menuType);
    MenuType getMenuType();

    void setHighlightOff();
    void setSubMenuArrowPosition();
    void setShortCutPosition(float x);

    std::string getName();

    TgEventResult handleEvent(TgEventData *eventData);
private:
    TgMenuItem *m_currentItem;

    MenuType m_menuType = MenuType::MenuType_NormalMenu;
    TgRectangle m_background;
    TgRectangle m_backgroundEnabled;
    TgRectangle m_backgroundHover;
    TgTextfield m_itemText;
    TgTextfield m_itemTextShortCut;
    TgImage m_subMenuArrowImage;
    TgShortCut m_shortCut;
    uint64_t m_globalMenuIndex = 0;

    float getSubMenuArrowX();
    float getSubMenuArrowY();
    std::string generateShortCutText();
};

#endif // TG_MENU_ITEM_PRIVATE_H