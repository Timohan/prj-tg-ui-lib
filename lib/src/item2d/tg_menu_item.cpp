/*!
 * \file
 * \brief file tg_menu_item.cpp
 *
 * Menu Item
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_menu_item.h"
#include "private/tg_menu_item_private.h"
#include "../global/tg_global_log.h"
#include "../global/private/tg_global_defines.h"
#include "tg_item2d.h"
#include "private/item2d/tg_item2d_private.h"
#include "private/mouse_capture/tg_mouse_capture_private.h"

TgMenuItem::TgMenuItem(TgMenuItem *parentMenu, TgItem2d *parent, const char *text, const TgShortCut *shortCut) :
    TgMouseCapture(parent, 0, 0, 100.0f, TG_MENU_DEFAULT_HEIGHT),
    m_private(new TgMenuItemPrivate(parentMenu, this, text, shortCut))
{
}

TgMenuItem::~TgMenuItem()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*! \brief TgMenuItem::getMinWidth
 * \return min width of the menu item that is required for item
 * basically: text.width() + left.margin + right.margin
 */
float TgMenuItem::getMinWidth()
{
    return m_private->getMinWidth();
}

/*! \brief TgMenuItem::getMinShortcutWidth
 * \return min width short the short cut text area + left.margin
 */
float TgMenuItem::getMinShortcutWidth()
{
    return m_private->getMinShortcutWidth();
}

/*! \brief TgMenuItem::addMenu
 * \param text
 * \param shortCut
 * \return pointer of created menu item, nullptr if create is failed
 */
TgMenuItem *TgMenuItem::addMenu(const char *text, const TgShortCut *shortCut)
{
    if (m_private->getMenuType() != TgMenuItemPrivate::MenuType::MenuType_TopMenu) {
        return TgItem2d::m_private->addMenu(this, text, shortCut, true);
    }
    return TgItem2d::m_private->addMenu(this, text, shortCut, false);
}

/*!
 * \brief TgMenuItem::onHoverChanged
 *
 * virtual function that can be used to capture message when
 * cursor is on the mouse capture or not
 * \param hover if true, mouse cursor is on mouse capture area
 */
void TgMenuItem::onHoverChanged(bool hover)
{
    TG_FUNCTION_BEGIN();
    m_private->onHoverChanged(hover);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMenuItem::onVisibleChanged
 *
 * virtual function when visible changed
 *
 * \param visible visible true/false
 */
void TgMenuItem::onVisibleChanged(bool visible)
{
    TG_FUNCTION_BEGIN();
    m_private->onVisibleChanged(visible, !TgItem2d::m_private->getTopMenu() && TgItem2d::m_private->getPossibleVisibleMenuCount());
    TG_FUNCTION_END();
}

/*!
 * \brief TgMenuItem::handleEvent
 *
 * handle the single event
 *
 * \param eventData
 * \param windowInfo
 * \return event result
 */
TgEventResult TgMenuItem::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (TgItem2d::m_private->getVisibleState() == TgItem2dVisibilityState::TgItem2dInvisible) {
        return TgEventResult::EventResultNotCompleted;
    }
    if (m_private->handleEvent(eventData) == TgEventResult::EventResultCompleted) {
        return TgEventResult::EventResultCompleted;
    }

    if (eventData->m_type == TgEventType::EventTypeMouseMove && getVisible() && m_private->getMenuType() != TgMenuItemPrivate::MenuType::MenuType_TopMenu && TgItem2d::m_private->getMenuCount() && TgItem2d::m_private->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
        if (!getEnabled()) {
            return TgEventResult::EventResultCompleted;
        }
        TgItem2d::m_private->setMenuVisible(TgItem2d::m_private->getX(), TgItem2d::m_private->getY(), TgItem2d::m_private->getWidth(), windowInfo);
        return TgMouseCapture::handleEvent(eventData, windowInfo);
    }
    TG_FUNCTION_END();
    return TgMouseCapture::handleEvent(eventData, windowInfo);
}

/*!
 * \brief TgMenuItem::handleChildrenEventMenu
 *
 * handles the events for children (menu items only)
 *
 * \param eventData
 * \param windowInfo
 * \return event result
 */
TgEventResult TgMenuItem::handleChildrenEventMenu(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (TgItem2d::m_private->getVisibleState() == TgItem2dVisibilityState::TgItem2dInvisible) {
        return TgEventResult::EventResultNotCompleted;
    }
    TgEventResult ret = TgEventResult::EventResultNotCompleted;
    if (TgItem2d::m_private->handleChildrenEventMenu(eventData, windowInfo) == TgEventResult::EventResultCompleted) {
        ret = TgEventResult::EventResultCompleted;
    }
    if (ret == TgEventResult::EventResultNotCompleted) {
        ret = handleEvent(eventData, windowInfo);
    }
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgMenuItem::renderMenu
 *
 * render's the menu
 *
 * \param windowInfo
 */
void TgMenuItem::renderMenu(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    renderChildren(windowInfo);
    TgItem2d::m_private->renderChildrenMenu(windowInfo);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMenuItem::hideSubMenuList
 *
 * hide sub menu lists
 */
void TgMenuItem::hideSubMenuList()
{
    TG_FUNCTION_BEGIN();
    TgItem2d::m_private->hideSubMenuList();
    m_private->setHighlightOff();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMenuItem::onEnabledChanged
 *
 * virtual function when enabled changed
 *
 * \param enabled enabled true/false
 */
void TgMenuItem::onEnabledChanged(bool enabled)
{
    TG_FUNCTION_BEGIN();
    m_private->onEnabledChanged(enabled);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMenuItem::setVisible
 *
 * sets this item visible/hidden
 *
 * \param visible
 */
void TgMenuItem::setVisible(bool visible)
{
    TgItem2d::m_private->setVisible(visible);
    if (visible && m_private->getMenuType() != TgMenuItemPrivate::MenuType::MenuType_TopMenu) {
        TgItem2d::m_private->parentVisibleChanged(false);
    }
}