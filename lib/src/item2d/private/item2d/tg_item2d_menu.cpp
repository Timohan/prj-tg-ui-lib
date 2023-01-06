/*!
 * \file
 * \brief file tg_item2d_menu.cpp
 *
 * It holds the pointer for menu items
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_menu.h"
#include <cmath>
#include "../../../global/tg_global_log.h"
#include "../../../global/private/tg_global_defines.h"
#include "../../../global/private/tg_global_deleter.h"
#include "../../tg_menu_item.h"
#include "../../../item2d/private/tg_menu_item_private.h"
#include "../../../event/tg_event_data.h"
#include "../../../item2d/tg_item2d.h"
#include "../../../item2d/private/item2d/tg_item2d_private.h"
#include "../../../window/tg_mainwindow_private.h"
#include "../../../global/private/tg_global_wait_renderer.h"

TgItem2dMenu::TgItem2dMenu(TgItem2d *currentItem, TgItem2d *parentItem) :
    m_currentItem(currentItem),
    m_parentItem(parentItem)
{
}

TgItem2dMenu::~TgItem2dMenu()
{
    m_mutexMenu.lock();
    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        delete m_listChildrenMenu[i];
    }
    m_listChildrenMenu.clear();
    m_mutexMenu.unlock();
}

TgMenuItem *TgItem2dMenu::addMenu(TgMenuItem *parentMenuItem, const char *text, const TgShortCut *shortCut, bool subMenu)
{
    bool visible = getMenuVisible();
    m_mutexMenu.lock();
    TgMenuItem *ret = new TgMenuItem(parentMenuItem, nullptr, text, shortCut);
    if (m_topMenu) {
        ret->m_private->setMenuType(TgMenuItemPrivate::MenuType::MenuType_TopDropDownMenu);
    } else if (subMenu) {
        ret->m_private->setMenuType(TgMenuItemPrivate::MenuType::MenuType_SubMenu);
    }
    ret->setVisible(true);
    reinterpret_cast<TgItem2d *>(ret)->m_private->parentVisibleChanged(visible);
    m_listChildrenMenu.push_back(ret);
    m_mutexMenu.unlock();
    return m_listChildrenMenu.back();
}

bool TgItem2dMenu::getMenuVisible()
{
    if (m_listChildrenMenu.empty()) {
        return false;
    }
    size_t i;
    for (i=0;i<m_listChildrenMenu.size();i++) {
        if (m_listChildrenMenu.at(i)->getVisible()) {
            return true;
        }
    }
    return false;
}

/*!
 * \brief TgItem2dMenu::getMenuCount
 *
 * \return count of menu items
*/
size_t TgItem2dMenu::getMenuCount()
{
    return m_listChildrenMenu.size();
}

/*!
 * \brief TgItem2dMenu::getPossibleVisibleMenuCount
 *
 * \return count of possible visible menu items,
 * items that are NOT invisible by user action
*/
size_t TgItem2dMenu::getPossibleVisibleMenuCount()
{
    size_t i, ret = 0;
    for (i=0;i<m_listChildrenMenu.size();i++) {
        TgItem2d *tmp = m_listChildrenMenu[i];
        if (tmp->m_private->getVisibleState() != TgItem2dVisibilityState::TgItem2dInvisible) {
            ret++;
        }
    }
    return ret;
}

/*!
 * \brief TgItem2dMenu::getMenu
 *
 * \param index
 * \return pointer of menu item
 * if index >= list of menu items, then return nullptr
 */
TgMenuItem *TgItem2dMenu::getMenu(size_t index)
{
    if (index >= m_listChildrenMenu.size()) {
        return nullptr;
    }
    return m_listChildrenMenu.at(index);
}

void TgItem2dMenu::setTopMenu(bool topMenu)
{
    m_topMenu = topMenu;
}

TgEventResult TgItem2dMenu::handleEventItem2dMenu(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (eventData->m_type == TgEventType::EventTypeMousePress
        && eventData->m_event.m_mouseEvent.m_mouseType == TgMouseType::ButtonLeft
        && m_currentItem->getVisible()
        && m_currentItem->getEnabled()
        && m_topMenu
        && !getMenuVisible()
        && m_currentItem->m_private->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
        // open top menu items, that items were not visible before
        setMenuItemsOpen(eventData, windowInfo);
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_event.m_mouseEvent.m_mouseType == TgMouseType::ButtonRight
        && eventData->m_type == TgEventType::EventTypeMousePress
        && m_currentItem->getVisible()
        && m_currentItem->getEnabled()
        && getMenuCount()
        && m_listChildrenMenu.at(0)->m_private->getMenuType() == TgMenuItemPrivate::MenuType::MenuType_NormalMenu
        && !getMenuVisible()
        && m_currentItem->m_private->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
        // open normal item menu with right mouse click
        setMenuItemsOpen(eventData, windowInfo);
        if (m_currentItem) {
            TgItem2dPrivateMessage msg;
            msg.m_fromItem = nullptr;
            msg.m_type = TgItem2dPrivateMessageType::HoverEnabledOnItem;
            m_currentItem->sendMessageToChildrenFromBegin(&msg);
        }
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseMoveForMenuParent
        && eventData->m_event.m_mouseEvent.m_mouseType == TgMouseType::NoButton
        && m_currentItem->getVisible()
        && m_currentItem->getEnabled()
        && !getMenuVisible()
        && m_topMenu
        && m_currentItem->m_private->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
         // open new menu item with mouse move, that it's sibling menu was open
         setMenuItemsOpen(eventData, windowInfo);
        return TgEventResult::EventResultCompleted;
    }

    return TgEventResult::EventResultNotCompleted;
}

TgEventResult TgItem2dMenu::handleChildrenEventMenu(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    if (!m_currentItem->getEnabled()) {
        return TgEventResult::EventResultNotCompleted;
    }
    size_t i, eventCompletedIndex = m_listChildrenMenu.size();
    TgEventResult ret = TgEventResult::EventResultNotCompleted;
    for (i=0;i<m_listChildrenMenu.size();i++) {
        if (m_listChildrenMenu[i]->handleChildrenEventMenu(eventData, windowInfo) == TgEventResult::EventResultCompleted) {
            ret = TgEventResult::EventResultCompleted;
            eventCompletedIndex = i;
        }
    }
    if (eventCompletedIndex < m_listChildrenMenu.size()
        && eventData->m_type == TgEventType::EventTypeMouseMove) {
        for (i=0;i<m_listChildrenMenu.size();i++) {
            if (i != eventCompletedIndex) {
                m_listChildrenMenu[i]->hideSubMenuList();
            }
        }
    }
    return ret;
}

void TgItem2dMenu::getAreaSizeOfMenuItems(std::vector<TgMenuItem *> &listMenu, float &width, float &height, float &shortcutWidth, float &submenuArrowWidth)
{
    size_t i;
    bool menuArrowMargin = false;
    float w;

    width = 0;
    height = 0;
    shortcutWidth = 0;
    submenuArrowWidth = 0;

    for (i=0;i<listMenu.size();i++) {
        TgItem2d *tmp = listMenu[i];
        if (tmp->m_private->getVisibleState() == TgItem2dVisibilityState::TgItem2dInvisible) {
            continue;
        }
        w = listMenu.at(i)->getMinWidth();
        if (tmp->m_private->getPossibleVisibleMenuCount()) {
            menuArrowMargin = true;
        }
        if (width < w) {
            width = w;
        }
        height += listMenu[i]->getHeight();
        w = listMenu.at(i)->getMinShortcutWidth();
        if (shortcutWidth < w) {
            shortcutWidth = w;
        }
    }
    if (menuArrowMargin) {
        submenuArrowWidth = TG_MENU_DEFAULT_SUB_MENU_ARROW_HEIGHT + TG_MENU_DEFAULT_SUB_MENU_ARROW_MARGIN;
    }
}

/*!
 * \brief TgItem2dMenu::setMenuItemsOpen
 *
 * set menu items to open, sets menu items positions and so on
 * \param eventData
 * \param windowInfo
 */
void TgItem2dMenu::setMenuItemsOpen(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    float startX = 0;
    float startY = 0;
    TgItem2dPrivateMessage msg;
    float width = 0;
    float height = 0;
    float shortcutWidth = 0;
    float submenuArrowWidth = 0;
    size_t i;
    msg.m_fromItem = nullptr;
    if (!m_listChildrenMenu.empty()) {
        getAreaSizeOfMenuItems(m_listChildrenMenu, width, height, shortcutWidth, submenuArrowWidth);

        switch (m_listChildrenMenu.at(0)->m_private->getMenuType()) {
            case TgMenuItemPrivate::MenuType::MenuType_NormalMenu:
            default:
                startX = static_cast<float>(eventData->m_event.m_mouseEvent.m_x)+5;
                startY = static_cast<float>(eventData->m_event.m_mouseEvent.m_y)+5;
                if (static_cast<int>(startX+width+shortcutWidth+submenuArrowWidth) > windowInfo->m_windowWidth) {
                    startX = static_cast<float>(eventData->m_event.m_mouseEvent.m_x) - (width + shortcutWidth + submenuArrowWidth) - 5;
                    if (startX < 0) {
                        startX = 0;
                    }
                }
                if (static_cast<int>(startY+height) > windowInfo->m_windowHeight) {
                    startY = static_cast<float>(eventData->m_event.m_mouseEvent.m_y) - height - 5;
                    if (startY < 0) {
                        startY = 0;
                    }
                }
                break;
            case TgMenuItemPrivate::MenuType::MenuType_TopMenu:
            case TgMenuItemPrivate::MenuType::MenuType_TopDropDownMenu:
                startX = m_currentItem->getX();
                startY = m_currentItem->getY() + m_currentItem->getHeight();
                if (static_cast<int>(startX+width+shortcutWidth+submenuArrowWidth) > windowInfo->m_windowWidth) {
                    startX = static_cast<float>(eventData->m_event.m_mouseEvent.m_x) - (width + shortcutWidth + submenuArrowWidth) - 5;
                    if (startX < 0) {
                        startX = 0;
                    }
                }
                break;
            case TgMenuItemPrivate::MenuType::MenuType_SubMenu:
                break;
        }
    }
    if (m_topMenu) {
        msg.m_fromItem = m_parentItem;
        msg.m_toItem = m_currentItem;
    }
    TgItem2dPrivateMessage msgToVisible;
    msgToVisible.m_type = TgItem2dPrivateMessageType::ParentItemToVisible;
    float y = startY;

    for (i=0;i<m_listChildrenMenu.size();i++) {
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->parentVisibleChanged(true);
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->sendMessageToChildren(&msgToVisible, false);
        if (!m_listChildrenMenu[i]->getVisible()) {
            continue;
        }
        m_listChildrenMenu[i]->setX(startX);
        m_listChildrenMenu[i]->setY(y);
        m_listChildrenMenu[i]->setAnchorState(TgItem2dAnchor::AnchorRelativeToParent);
        m_listChildrenMenu[i]->setWidth(width+shortcutWidth+submenuArrowWidth);
        m_listChildrenMenu[i]->setHeight(TG_MENU_DEFAULT_HEIGHT);
        m_listChildrenMenu[i]->m_private->setSubMenuArrowPosition();
        m_listChildrenMenu[i]->m_private->setShortCutPosition(width);
        m_listChildrenMenu[i]->hideSubMenuList();
        msg.m_listAdditionalItems.push_back(m_listChildrenMenu[i]);
        y += m_listChildrenMenu[i]->getHeight();
    }

    if (eventData->m_type == TgEventType::EventTypeMouseMoveForMenuParent) {
        msg.m_primaryValue = 1;
    }
    msg.m_type = TgItem2dPrivateMessageType::EventSetMainMenuItems;
    m_currentItem->sendMessageToChildrenFromBegin(&msg);
    TG_FUNCTION_END();
}

void TgItem2dMenu::setMenuVisible(float x, float y, float w, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    size_t i;
    float width = 0;
    float height = 0;
    float shortcutWidth = 0;
    float submenuArrowWidth = 0;
    getAreaSizeOfMenuItems(m_listChildrenMenu, width, height, shortcutWidth, submenuArrowWidth);

    if (m_listChildrenMenu.empty()) {
        return;
    }

    float startX = x+w;
    float startY = y;

    if (static_cast<int>(startX+width+shortcutWidth+submenuArrowWidth) > windowInfo->m_windowWidth) {
        startX = x - (width + shortcutWidth + submenuArrowWidth);
    }
    if (static_cast<int>(startY+height) > windowInfo->m_windowHeight) {
        startY = y - height + m_listChildrenMenu[0]->getHeight();
    }
    TgItem2dPrivateMessage msgToVisible;
    msgToVisible.m_type = TgItem2dPrivateMessageType::ParentItemToVisible;
    float yPos = startY;

    for (i=0;i<m_listChildrenMenu.size();i++) {
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->parentVisibleChanged(true);
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->sendMessageToChildren(&msgToVisible, false);
        if (!m_listChildrenMenu[i]->getVisible()) {
            continue;
        }
        m_listChildrenMenu[i]->setX(startX);
        m_listChildrenMenu[i]->setY(yPos);
        m_listChildrenMenu[i]->setAnchorState(TgItem2dAnchor::AnchorRelativeToParent);
        m_listChildrenMenu[i]->setWidth(width + shortcutWidth + submenuArrowWidth);
        m_listChildrenMenu[i]->setHeight(TG_MENU_DEFAULT_HEIGHT);
        m_listChildrenMenu[i]->m_private->setSubMenuArrowPosition();
        m_listChildrenMenu[i]->m_private->setShortCutPosition(width);
        m_listChildrenMenu[i]->hideSubMenuList();
        yPos += m_listChildrenMenu[i]->getHeight();
    }

    TG_FUNCTION_END();
}

void TgItem2dMenu::hideSubMenuList()
{
    TG_FUNCTION_BEGIN();
    m_mutexMenu.lock();
    size_t i;
    TgItem2dPrivateMessage msgToInvisible;
    msgToInvisible.m_type = TgItem2dPrivateMessageType::ParentItemToInvisible;

    for (i=0;i<m_listChildrenMenu.size();i++) {
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->parentVisibleChanged(false);
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->sendMessageToChildren(&msgToInvisible, false);
        m_listChildrenMenu[i]->hideSubMenuList();
    }
    m_mutexMenu.unlock();
    TG_FUNCTION_END();
}

void TgItem2dMenu::renderMenu(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexMenu.lock();
    if (!m_currentItem->getVisible()) {
        TG_FUNCTION_END();
        return;
    }

    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        if (!m_listChildrenMenu[i]->getVisible()) {
            break;
        }
        m_listChildrenMenu[i]->renderChildren(windowInfo);
    }
    m_mutexMenu.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dMenu::checkPositionValuesChildrenMenu
 *
 * check position values to children
 * \param windowInfo
 */
void TgItem2dMenu::checkPositionValuesChildrenMenu(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexMenu.lock();
    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        m_listChildrenMenu[i]->checkPositionValues();
        m_listChildrenMenu[i]->checkPositionValuesChildren(windowInfo);
    }
    m_mutexMenu.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dMenu::getTopMenu
 *
 * \return true if this menu item is top menu's first menu item
 */
bool TgItem2dMenu::getTopMenu()
{
    return m_topMenu;
}

void TgItem2dMenu::renderChildrenMenu(const TgWindowInfo *windowInfo)
{
    m_mutexMenu.lock();
    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        m_listChildrenMenu[i]->renderMenu(windowInfo);
    }
    m_mutexMenu.unlock();
}

/*!
 * \brief TgItem2dMenu::getSubMenuCount
 *
 * \return sub menu count
 */
size_t TgItem2dMenu::getSubMenuCount()
{
    m_mutexMenu.lock();
    size_t ret = 0;
    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->getDeleting()) {
            continue;
        }
        ret++;
    }
    m_mutexMenu.unlock();
    return ret;
}

/*!
 * \brief TgItem2dMenu::removeSubMenu
 *
 * \param indexToRemove index of sub menu
 * \return true if sub menu was removed
 */
bool TgItem2dMenu::removeSubMenu(size_t indexToRemove)
{
    m_mutexMenu.lock();
    if (indexToRemove >= m_listChildrenMenu.size()) {
        m_mutexMenu.unlock();
        return false;
    }
    size_t tmpValue = 0;
    bool ret = false;
    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->getDeleting()) {
            continue;
        }
        if (tmpValue == indexToRemove) {
            TgGlobalDeleter::getInstance()->addSubMenu(this, m_listChildrenMenu[i]);
            reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->setDeleting();
            ret = true;
            break;
        }
        tmpValue++;
    }
    m_mutexMenu.unlock();
    return ret;
}

/*!
 * \brief TgItem2dMenu::deleteMenu
 *
 * \param item item to delete
 */
void TgItem2dMenu::deleteMenu(TgMenuItem *item)
{
    m_mutexMenu.lock();
    size_t i;
    for (i=0;i<m_listChildrenMenu.size();i++) {
        if (m_listChildrenMenu[i] == item) {
            delete m_listChildrenMenu[i];
            m_listChildrenMenu.erase(m_listChildrenMenu.begin()+i);
            break;
        }
    }
    m_mutexMenu.unlock();
}

/*!
 * \brief TgItem2dMenu::setDeletingSubMenu
 *
 * set deleting to submenus
 */
void TgItem2dMenu::setDeletingSubMenu()
{
    m_mutexMenu.lock();
    for (size_t i=0;i<m_listChildrenMenu.size();i++) {
        reinterpret_cast<TgItem2d *>(m_listChildrenMenu[i])->m_private->setDeleting();
    }
    m_mutexMenu.unlock();
}