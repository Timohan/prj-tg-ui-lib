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

#include "tg_mainwindow_menu.h"
#include "../../global/tg_global_log.h"
#include "../../item2d/private/item2d/tg_item2d_private.h"
#include "../../item2d/tg_menu_item.h"
#include "../../item2d/private/tg_menu_item_private.h"
#include "../tg_mainwindow_private.h"

TgMainWindowMenu::TgMainWindowMenu()
{
}

TgMainWindowMenu::~TgMainWindowMenu()
{
    m_listMenuItem.clear();
}

/*!
 * \brief TgMainWindowMenu::handlePrivateMessageWindowMenu
 *
 * \param message
 */
void TgMainWindowMenu::handlePrivateMessageWindowMenu(const TgItem2dPrivateMessage *message)
{
    if (message->m_type == TgItem2dPrivateMessageType::EventSetMainMenuItems) {
        if (message->m_primaryValue == 0) {
            m_mutexMenu.lock();
        }
        hideList();
        m_parentMenuItem = message->m_fromItem;
        if (message->m_toItem) {
            m_currentMenuItem = reinterpret_cast<TgMenuItem *>(message->m_toItem);
        }
        for (size_t i=0;i<message->m_listAdditionalItems.size();i++) {
            TgMenuItem *item = reinterpret_cast<TgMenuItem *>(message->m_listAdditionalItems[i]);
            m_listMenuItem.push_back(item);
            item->m_private->setMenuRendering(true);
        }
        if (message->m_primaryValue == 0) {
            m_mutexMenu.unlock();
        }
        TgGlobalWaitRenderer::getInstance()->release(DEFAULT_RENDER_WAIT_MAX_TIMEOUT);
    }
    if (message->m_type == TgItem2dPrivateMessageType::EventHideTheList) {
        m_mutexMenu.lock();
        hideList();
        m_mutexMenu.unlock();
        TgGlobalWaitRenderer::getInstance()->release(DEFAULT_RENDER_WAIT_MAX_TIMEOUT);
    }
}

/*!
 * \brief TgMainWindowMenu::handlePrivateMessageWindowMenu
 *
 * renders children items
 * \param windowInfo
 */
void TgMainWindowMenu::renderChildrenMenu(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexMenu.lock();
    if (!m_listMenuItem.empty()) {
        for (size_t i=0;i<m_listMenuItem.size();i++) {
            m_listMenuItem[i]->renderMenu(windowInfo, 1.0f);
        }
    }
    m_mutexMenu.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindowMenu::checkPositionValuesChildrenWindowMenu
 *
 * check position values
 * \param windowInfo
 */
void TgMainWindowMenu::checkPositionValuesChildrenWindowMenu(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexMenu.lock();
    if (!m_listMenuItem.empty()) {
        for (size_t i=0;i<m_listMenuItem.size();i++) {
            m_listMenuItem[i]->checkPositionValuesChildren(windowInfo);
        }
    }
    m_mutexMenu.unlock();
    TG_FUNCTION_END();
}

void TgMainWindowMenu::hideList()
{
    size_t i;
    TgItem2dPrivateMessage msgToInvisible;
    msgToInvisible.m_type = TgItem2dPrivateMessageType::ParentItemToInvisible;

    for (i=0;i<m_listMenuItem.size();i++) {
        reinterpret_cast<TgItem2d *>(m_listMenuItem[i])->m_private->parentVisibleChanged(false);
        reinterpret_cast<TgItem2d *>(m_listMenuItem[i])->m_private->sendMessageToChildren(&msgToInvisible, false);
        m_listMenuItem[i]->hideSubMenuList();
        m_listMenuItem[i]->m_private->setMenuRendering(false);
    }
    m_listMenuItem.clear();
    if (m_currentMenuItem) {
        m_currentMenuItem->hideSubMenuList();
    }
    m_parentMenuItem = nullptr;
    m_currentMenuItem = nullptr;
}

/*!
 * \brief TgMainWindowMenu::handleEventsMenu
 *
 * handles the events for menu
 *
 * \param eventData
 * \param windowInfo
 * \return event result
 */
TgEventResult TgMainWindowMenu::handleEventsMenu(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexMenu.lock();
    TgEventResult ret = TgEventResult::EventResultNotCompleted;
    if (m_listMenuItem.empty()) {
        m_mutexMenu.unlock();
        return TgEventResult::EventResultNotCompleted;
    }
    size_t i, completedMenuIndex = m_listMenuItem.size();
    for (i=0;i<m_listMenuItem.size();i++) {
        if (m_listMenuItem[i]->handleChildrenEventMenu(eventData, windowInfo) == TgEventResult::EventResultCompleted) {
            ret = TgEventResult::EventResultCompleted;
            completedMenuIndex = i;
        }
    }
    if (ret == TgEventResult::EventResultCompleted
        && eventData->m_type == TgEventType::EventTypeMouseMove
        && completedMenuIndex < m_listMenuItem.size()) {
        for (i=0;i<m_listMenuItem.size();i++) {
            if (completedMenuIndex != i) {
                m_listMenuItem[i]->hideSubMenuList();
            }
        }
    }
    if (eventData->m_type == TgEventType::EventTypeMouseScrollMove
        && ret == TgEventResult::EventResultCompleted
        && !m_listMenuItem.empty()
        && m_listMenuItem.front()->m_private->getMenuType() == TgMenuItemPrivate::MenuType::MenuType_ComboBoxMenu
        && completedMenuIndex < m_listMenuItem.size()) {
        float menuHeight = m_listMenuItem.back()->getY() + m_listMenuItem.back()->getHeight() - m_listMenuItem.front()->getY();
        if (menuHeight > static_cast<float>(windowInfo->m_windowHeight)) {
            if (eventData->m_event.m_mouseEvent.m_scroll_move_y < 0) {
                if (m_listMenuItem.back()->getY() + m_listMenuItem.back()->getHeight() > static_cast<float>(windowInfo->m_windowHeight)) {
                    for (i=0;i<m_listMenuItem.size();i++) {
                        m_listMenuItem[i]->setY( m_listMenuItem[i]->getY() - m_listMenuItem[i]->getHeight());
                    }

                    // need to send mouse move to item to change hover
                    TgEventData moveEventData;
                    moveEventData.m_type = TgEventType::EventTypeMouseMove;
                    moveEventData.m_event.m_mouseEvent.m_x = eventData->m_event.m_mouseEvent.m_x;
                    moveEventData.m_event.m_mouseEvent.m_y = eventData->m_event.m_mouseEvent.m_y;
                    moveEventData.m_event.m_mouseEvent.m_currentMouseDownItem = nullptr;
                    for (i=0;i<m_listMenuItem.size();i++) {
                        m_listMenuItem[i]->handleEvent(&moveEventData, windowInfo);
                    }
                }
            } else if (eventData->m_event.m_mouseEvent.m_scroll_move_y > 0) {
                if (m_listMenuItem.front()->getY() < 0) {
                    for (i=0;i<m_listMenuItem.size();i++) {
                        m_listMenuItem[i]->setY( m_listMenuItem[i]->getY() + m_listMenuItem[i]->getHeight());
                    }
                    // need to send mouse move to item to change hover
                    TgEventData moveEventData;
                    moveEventData.m_type = TgEventType::EventTypeMouseMove;
                    moveEventData.m_event.m_mouseEvent.m_x = eventData->m_event.m_mouseEvent.m_x;
                    moveEventData.m_event.m_mouseEvent.m_y = eventData->m_event.m_mouseEvent.m_y;
                    moveEventData.m_event.m_mouseEvent.m_currentMouseDownItem = nullptr;
                    for (i=0;i<m_listMenuItem.size();i++) {
                        m_listMenuItem[i]->handleEvent(&moveEventData, windowInfo);
                    }
                }
            }
        }
    }
    if (ret == TgEventResult::EventResultNotCompleted
        && !m_listMenuItem.empty()
        && eventData->m_type == TgEventType::EventTypeCharacterCallback
        && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_NormalKey
        && (eventData->m_event.m_keyEvent.m_key == 32 || eventData->m_event.m_keyEvent.m_key == '\t')
        && m_listMenuItem[0]->m_private->getMenuType() == TgMenuItemPrivate::MenuType::MenuType_ComboBoxMenu) {
        hideList();
        ret = TgEventResult::EventResultCompleted;
    }
    if (ret == TgEventResult::EventResultNotCompleted
        && eventData->m_type == TgEventType::EventTypeMousePress
        && (!m_listMenuItem.empty() || m_parentMenuItem)) {
        hideList();
        ret = TgEventResult::EventResultCompleted;
    }
    if (!m_listMenuItem.empty() && ret == TgEventResult::EventResultCompleted
        && eventData->m_type == TgEventType::EventTypeMouseRelease) {
        hideList();
    }
    if (ret == TgEventResult::EventResultNotCompleted
        && eventData->m_type == TgEventType::EventTypeMouseMove
        && m_parentMenuItem) {
        eventData->m_type = TgEventType::EventTypeMouseMoveForMenuParent;
        ret = m_parentMenuItem->handleEventsChildren(eventData, windowInfo);
        eventData->m_type = TgEventType::EventTypeMouseMove;
    }
    TG_FUNCTION_END();
    m_mutexMenu.unlock();
    return ret;
}

/*!
 * \brief TgMainWindowMenu::menuItemCount
 *
 * \return menu item count
 */
size_t TgMainWindowMenu::menuItemCount()
{
    m_mutexMenu.lock();
    size_t ret = m_listMenuItem.size();
    m_mutexMenu.unlock();
    return ret;
}

/*!
 * \brief TgMainWindowMenu::isMenuEnabled
 *
 * \return true if menu is enabled, on one way or another
 */
bool TgMainWindowMenu::isMenuEnabled()
{
    m_mutexMenu.lock();
    bool ret = !m_listMenuItem.empty() || m_parentMenuItem;
    m_mutexMenu.unlock();
    return ret;
}
