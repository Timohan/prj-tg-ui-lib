/*!
 * \file
 * \brief file tg_menu_item.h
 *
 * Menu Item
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MENU_ITEM_H
#define TG_MENU_ITEM_H

#include <functional>
#include <string>
#include "../global/tg_global_macros.h"
#include "tg_mouse_capture.h"

struct TgShortCut
{
    bool m_alt { false };
    bool m_ctrl { false };
    bool m_shift { false };
    uint32_t m_key { 0 };
};

class TgMenuItemPrivate;
class TgItem2d;

/*!
 * \brief TgMenuItem
 * Menu item
 */
class TG_MAINWINDOW_EXPORT TgMenuItem : public TgMouseCapture
{
public:
    ~TgMenuItem();

    virtual TgMenuItem *addMenu(const char *text, const TgShortCut *shortCut);
    float getMinWidth();
    float getMinShortcutWidth();

    virtual void setVisible(bool visible);
    std::string getText() const;

    void setBackgroundColor(const unsigned char r = 255, const unsigned char g = 255, const unsigned char b = 255, const unsigned char a = 255);

protected:
    virtual void onHoverChanged(bool hover) override;
    virtual void onEnabledChanged(bool enabled) override;
    virtual void onVisibleChanged(bool visible) override;
    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo) override;

private:
    TgMenuItemPrivate *m_private;

    explicit TgMenuItem(TgMenuItem *parentMenu, TgItem2d *parent, const char *text, const TgShortCut *shortCut);

    void renderMenu(const TgWindowInfo *windowInfo, float parentOpacity);
    void hideSubMenuList();
    TgEventResult handleChildrenEventMenu(TgEventData *eventData, const TgWindowInfo *windowInfo);

    friend class TgItem2dMenu;
    friend class TgMenuTopPrivate;
    friend class TgMainWindowMenu;
    friend class TgGlobalMenuHolder;
    friend class TgComboBoxPrivate;
};

#endif // TG_MENU_ITEM_H
