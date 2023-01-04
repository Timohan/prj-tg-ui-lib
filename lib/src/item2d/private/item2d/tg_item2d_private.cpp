/*!
 * \file
 * \brief file tg_item2d_private.cpp
 *
 * it holds general TgItem2dPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_private.h"
#include <cmath>
#include "../../../global/tg_global_log.h"
#include "../../tg_rectangle.h"
#include "../../../window/tg_mainwindow_private.h"

TgItem2dPrivate::TgItem2dPrivate(TgItem2d *parent, TgItem2d *current) :
    TgItem2dVisible(parent, this),
    TgItem2dEnabled(parent, this),
    TgItem2dPosition(parent, this),
    TgItem2dSelected(parent, current, this),
    TgItem2dMenu(current, parent),
    m_internalCallback(nullptr),
    m_parent(parent),
    m_currentItem(current)
{
    TG_FUNCTION_BEGIN();
    if (parent) {
        parent->addChild(current, false);
    }
    TG_FUNCTION_END();
}

TgItem2dPrivate::TgItem2dPrivate(float x, float y, float width, float height, TgItem2d *parent, TgItem2d *current, bool topMenu) :
    TgItem2dVisible(parent, this),
    TgItem2dEnabled(parent, this),
    TgItem2dPosition(x, y, width, height, parent, this),
    TgItem2dSelected(parent, current, this),
    TgItem2dMenu(current, parent),
    m_internalCallback(nullptr),
    m_parent(parent),
    m_currentItem(current)
{
    TG_FUNCTION_BEGIN();
    if (parent) {
        parent->addChild(current, topMenu);
    }
    TG_FUNCTION_END();
}

TgItem2dPrivate::~TgItem2dPrivate()
{
    TG_FUNCTION_BEGIN();
    TgItem2dPrivateMessage msg;
    msg.m_type = TgItem2dPrivateMessageType::RemovingItem2d;
    msg.m_fromItem = m_currentItem;
    sendMessageToChildrenFromBegin(&msg);
    m_listChildrenItem.clear();
    m_listChildrenTopMenu.clear();
    TG_FUNCTION_END();
}

void TgItem2dPrivate::setInternalCallbacks(TgItem2dInternalCallback *callback)
{
    TG_FUNCTION_BEGIN();
    m_internalCallback = callback;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::addChild
 *
 * add child to list of children
 *
 * \param child
 */
void TgItem2dPrivate::addChild(TgItem2d *child, bool topMenu)
{
    TG_FUNCTION_BEGIN();
    if (!topMenu) {
        m_listChildrenItem.push_back(child);
    } else {
        m_listChildrenTopMenu.push_back(child);
    }
    setRequireRecheckVisibleChangeToChildren(true);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::renderChildren
 *
 * renders all children
 */
void TgItem2dPrivate::renderChildren(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        TG_FUNCTION_END();
        return;
    }
    size_t i;
    for (i=0;i<m_listChildrenItem.size();i++) {
        m_listChildrenItem[i]->render(windowInfo);
        m_listChildrenItem[i]->renderChildren(windowInfo);
    }
    for (i=0;i<m_listChildrenTopMenu.size();i++) {
        m_listChildrenTopMenu[i]->render(windowInfo);
        m_listChildrenTopMenu[i]->renderChildren(windowInfo);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::checkPositionValuesChildren
 *
 * check position values to children
 *
 */
void TgItem2dPrivate::checkPositionValuesChildren(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    size_t i;
    for (i=0;i<m_listChildrenItem.size();i++) {
        reCheckChildrenVisibility();
        m_listChildrenItem[i]->checkPositionValues();
        m_listChildrenItem[i]->checkPositionValuesChildren(windowInfo);
    }
    for (i=0;i<m_listChildrenTopMenu.size();i++) {
        reCheckChildrenVisibility();
        m_listChildrenTopMenu[i]->checkPositionValues();
        m_listChildrenTopMenu[i]->checkPositionValuesChildren(windowInfo);
    }
    checkPositionValuesChildrenMenu(windowInfo);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::handleEventsChildren
 *
 * handles the events for children
 *
 * \param eventData
 * \param windowInfo
 * \return event result
 */
TgEventResult TgItem2dPrivate::handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    if (handleEventsChildren(eventData, windowInfo, m_listChildrenTopMenu) == TgEventResult::EventResultCompleted) {
        return TgEventResult::EventResultCompleted;
    }
    if (handleEventsChildren(eventData, windowInfo, m_listChildrenItem) == TgEventResult::EventResultCompleted) {
        return TgEventResult::EventResultCompleted;
    }
    return TgEventResult::EventResultNotCompleted;
}

/*!
 * \brief TgItem2dPrivate::handleEventsChildren
 *
 * handles the events for children
 *
 * \param eventData
 * \param windowInfo
 * \param listChildren
 * \return event result
 */
TgEventResult TgItem2dPrivate::handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo, std::vector<TgItem2d *>&listChildren)
{
    TG_FUNCTION_BEGIN();
    TgEventResult ret = TgEventResult::EventResultNotCompleted;
    for (size_t i=0;i<listChildren.size();i++) {
        if (eventData->m_type == TgEventType::EventTypeCharacterCallback
            || eventData->m_type == TgEventType::EventTypeCharacterCallbackShortCut
            || eventData->m_type == TgEventType::EventTypeSelectLastItem
            || eventData->m_type == TgEventType::EventTypeSelectNextItem
            || eventData->m_type == TgEventType::EventTypeSelectFirstItem) {
            ret = listChildren[i]->handleEvent(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
            ret = listChildren[i]->handleEventsChildren(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
        } else {
            ret = listChildren[listChildren.size()-1-i]->handleEventsChildren(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
            ret = listChildren[listChildren.size()-1-i]->handleEvent(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
        }
    }
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgItem2dPrivate::sendMessageToChildren
 *
 * sends message to all children
 *
 * \param message message
 * \param allowFunctionalityToThisItem
 */
void TgItem2dPrivate::sendMessageToChildren(const TgItem2dPrivateMessage *message, bool allowFunctionalityToThisItem)
{
    TG_FUNCTION_BEGIN();
    if (allowFunctionalityToThisItem) {
        switch (message->m_type) {
            case TgItem2dPrivateMessageType::PositionChanged:
            case TgItem2dPrivateMessageType::SetUnselected:
            case TgItem2dPrivateMessageType::CurrentItemToInvisible:
            case TgItem2dPrivateMessageType::CurrentItemToDisabled:
            case TgItem2dPrivateMessageType::EventClearButtonPressForThisItem:
            case TgItem2dPrivateMessageType::ItemToVisibleChanged:
            case TgItem2dPrivateMessageType::ItemToEnabledChanged:
            case TgItem2dPrivateMessageType::EventChangeButtonPressToThisItem:
            case TgItem2dPrivateMessageType::RemovingItem2d:
            case TgItem2dPrivateMessageType::EventSetMainMenuItems:
            default:
                break;
            case TgItem2dPrivateMessageType::ParentItemToVisible:
                if (!parentVisibleChanged(true)) {
                    return;
                }
                break;
            case TgItem2dPrivateMessageType::ParentItemToEnabled:
                if (!parentEnabledChanged(true)) {
                    return;
                }
                break;
            case TgItem2dPrivateMessageType::ParentItemToInvisible:
                parentVisibleChanged(false);
                break;
            case TgItem2dPrivateMessageType::ParentItemToDisabled:
                parentEnabledChanged(false);
                break;
            case TgItem2dPrivateMessageType::ParentItemToUseRoundedPositionValues:
                setUseRoundedPositionValues(true);
                break;
            case TgItem2dPrivateMessageType::ParentItemToUseNotRoundedPositionValues:
                setUseRoundedPositionValues(false);
                break;
            case TgItem2dPrivateMessageType::HoverEnabledOnItem:
                m_currentItem->handlePrivateMessage(message);
                break;
        }
        handleMessageToChildren(message);
    }

    sendMessageToChildren(message, m_currentItem, m_listChildrenItem);
    sendMessageToChildren(message, m_currentItem, m_listChildrenTopMenu);

    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::sendMessageToChildren
 *
 * sends message to all children
 *
 * \param message message
 * \param currentItem
 * \param listChildren
 */
void TgItem2dPrivate::sendMessageToChildren(const TgItem2dPrivateMessage *message, TgItem2d *currentItem, std::vector<TgItem2d *>&listChildren)
{
    for (size_t i=0;i<listChildren.size();i++) {
        listChildren[i]->m_private->handleMessageToChildren(message);
        switch (message->m_type) {
            case TgItem2dPrivateMessageType::PositionChanged:
                listChildren[i]->setPositionChanged(true);
                break;
            case TgItem2dPrivateMessageType::SetUnselected:
                if (message->m_fromItem != currentItem && currentItem->m_private->getSelected()) {
                    currentItem->m_private->setSelected(false);
                }
                break;
            case TgItem2dPrivateMessageType::RemovingItem2d:
                if (listChildren[i] == message->m_fromItem) {
                    listChildren.erase(listChildren.begin()+i);
                    return;
                }
                break;
            case TgItem2dPrivateMessageType::HoverEnabledOnItem:
                currentItem->handlePrivateMessage(message);
                break;
            case TgItem2dPrivateMessageType::ParentItemToVisible:
            case TgItem2dPrivateMessageType::ParentItemToEnabled:
            case TgItem2dPrivateMessageType::ParentItemToInvisible:
            case TgItem2dPrivateMessageType::EventClearButtonPressForThisItem:
            case TgItem2dPrivateMessageType::ParentItemToDisabled:
            case TgItem2dPrivateMessageType::ParentItemToUseRoundedPositionValues:
            case TgItem2dPrivateMessageType::ParentItemToUseNotRoundedPositionValues:
            case TgItem2dPrivateMessageType::CurrentItemToInvisible:
            case TgItem2dPrivateMessageType::CurrentItemToDisabled:
            case TgItem2dPrivateMessageType::ItemToVisibleChanged:
            case TgItem2dPrivateMessageType::ItemToEnabledChanged:
            case TgItem2dPrivateMessageType::EventChangeButtonPressToThisItem:
            case TgItem2dPrivateMessageType::EventSetMainMenuItems:
            default:
                break;
        }
        listChildren[i]->sendMessageToChildren(message);
    }
}

/*!
 * \brief TgItem2dPrivate::checkOnResizeChangedOnChildren
 *
 * checks if position or size is changed on children
 */
void TgItem2dPrivate::checkOnResizeChangedOnChildren()
{
    TG_FUNCTION_BEGIN();
    for (size_t i=0;i<m_listChildrenItem.size();i++) {
        m_listChildrenItem[i]->checkOnResizeChanged();
        m_listChildrenItem[i]->checkOnResizeChangedOnChildren();
    }
    for (size_t i=0;i<m_listChildrenTopMenu.size();i++) {
        m_listChildrenTopMenu[i]->checkOnResizeChanged();
        m_listChildrenTopMenu[i]->checkOnResizeChangedOnChildren();
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::sendMessageToChildrenFromBegin
 *
 * sends message to all items
 *
 * \param message
 */
void TgItem2dPrivate::sendMessageToChildrenFromBegin(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    if (m_parent) {
        m_parent->sendMessageToChildrenFromBegin(message);
        TG_FUNCTION_END();
        return;
    } else if (message->m_type == TgItem2dPrivateMessageType::ItemToVisibleChanged
                || message->m_type == TgItem2dPrivateMessageType::EventChangeButtonPressToThisItem
                || message->m_type == TgItem2dPrivateMessageType::EventClearButtonPressForThisItem
                || message->m_type == TgItem2dPrivateMessageType::ItemToEnabledChanged) {
        TG_FUNCTION_END();
        return;
    }

    sendMessageToChildren(message);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::setToTop
 *
 * changes this child to top (last on the list of children)
 *
 * \param child
 */
void TgItem2dPrivate::setToTop(TgItem2d *child)
{
    TG_FUNCTION_BEGIN();
    setToTop(child, m_listChildrenItem);
    setToTop(child, m_listChildrenTopMenu);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::setToTop
 *
 * changes this child to top (last on the list of children)
 *
 * \param child
 * \param listChildren
 */
void TgItem2dPrivate::setToTop(TgItem2d *child, std::vector<TgItem2d *>&listChildren)
{
    TG_FUNCTION_BEGIN();
    size_t i;
    for (i=0;i<listChildren.size()-1;i++) {
        if (listChildren[i] == child) {
            listChildren.erase(listChildren.begin()+i);
            listChildren.push_back(child);
            break;
        }
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::isCursorOnItem
 *
 * changes this child to top (last on the list of children)
 *
 * \param x
 * \param y
 * \param windowInfo
 * \return true if cursor on item
 */
bool TgItem2dPrivate::isCursorOnItem(double x, double y, const TgWindowInfo *windowInfo)
{
    return getXminOnVisible() <= x
        && getXmaxOnVisible(windowInfo) > x
        && getYminOnVisible() <= y
        && getYmaxOnVisible(windowInfo) > y;
}

/*!
 * \brief TgItem2dPrivate::getDeleting
 *
 * \return true if item will be deleted
 */
bool TgItem2dPrivate::getDeleting()
{
    return m_deleting;
}

/*!
 * \brief TgItem2dPrivate::setDeleting
 *
 * set this item deleting to true
 * so it will be deleted
 */
void TgItem2dPrivate::setDeleting()
{
    m_deleting = true;
    setDeletingSubMenu();
}
