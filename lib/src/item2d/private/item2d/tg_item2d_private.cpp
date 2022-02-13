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
    TgItem2dPosition(parent, this),
    TgItem2dSelected(parent, current, this),
    m_internalCallback(nullptr),
    m_parent(parent),
    m_currentItem(current),
    m_enabled(true)
{
    TG_FUNCTION_BEGIN();
    if (parent) {
        parent->addChild(current);
    }
    TG_FUNCTION_END();
}

TgItem2dPrivate::TgItem2dPrivate(float x, float y, float width, float height, TgItem2d *parent, TgItem2d *current) :
    TgItem2dVisible(parent, this),
    TgItem2dPosition(x, y, width, height, parent, this),
    TgItem2dSelected(parent, current, this),
    m_internalCallback(nullptr),
    m_parent(parent),
    m_currentItem(current),
    m_enabled(true)
{
    TG_FUNCTION_BEGIN();
    if (parent) {
        parent->addChild(current);
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
    m_listChildren.clear();
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
void TgItem2dPrivate::addChild(TgItem2d *child)
{
    TG_FUNCTION_BEGIN();
    m_listChildren.push_back(child);
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
    for (size_t i=0;i<m_listChildren.size();i++) {
        m_listChildren[i]->render(windowInfo);
        m_listChildren[i]->renderChildren(windowInfo);
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
    for (size_t i=0;i<m_listChildren.size();i++) {
        reCheckChildrenVisibility();
        m_listChildren[i]->checkPositionValues(windowInfo);
        m_listChildren[i]->checkPositionValuesChildren(windowInfo);
    }
    TG_FUNCTION_END();
}


/*!
 * \brief TgItem2dPrivate::handleEventsChildren
 *
 * handles the events for children
 *
 * \param eventData
 * \return event result
 */
TgEventResult TgItem2dPrivate::handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    TgEventResult ret = TgEventResult::EventResultNotCompleted;
    for (size_t i=0;i<m_listChildren.size();i++) {
        if (eventData->m_type == TgEventType::EventTypeCharacterCallback
            || eventData->m_type == TgEventType::EventTypeSelectLastItem
            || eventData->m_type == TgEventType::EventTypeSelectNextItem
            || eventData->m_type == TgEventType::EventTypeSelectFirstItem) {
            ret = m_listChildren[i]->handleEvent(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
            ret = m_listChildren[i]->handleEventsChildren(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
        } else {
            ret = m_listChildren[m_listChildren.size()-1-i]->handleEventsChildren(eventData, windowInfo);
            if (ret == TgEventResult::EventResultCompleted) {
                TG_FUNCTION_END();
                return ret;
            }
            ret = m_listChildren[m_listChildren.size()-1-i]->handleEvent(eventData, windowInfo);
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
            case RemovingItem2d:
            default:
                break;
            case TgItem2dPrivateMessageType::ParentItemToVisible:
                parentVisibleChanged(true);
                break;
            case TgItem2dPrivateMessageType::ParentItemToInvisible:
                parentVisibleChanged(false);
                break;
            case TgItem2dPrivateMessageType::ParentItemToUseRoundedPositionValues:
                setUseRoundedPositionValues(true);
                break;
            case TgItem2dPrivateMessageType::ParentItemToUseNotRoundedPositionValues:
                setUseRoundedPositionValues(false);
                break;
        }
    }

    for (size_t i=0;i<m_listChildren.size();i++) {
        switch (message->m_type) {
            case TgItem2dPrivateMessageType::PositionChanged:
                m_listChildren[i]->setPositionChanged(true);
                break;
            case TgItem2dPrivateMessageType::SetUnselected:
                if (message->m_fromItem != m_currentItem && getSelected()) {
                    setSelected(false);
                }
                break;
            case TgItem2dPrivateMessageType::RemovingItem2d:
                if (m_listChildren[i] == message->m_fromItem) {
                    m_listChildren.erase(m_listChildren.begin()+i);
                    return;
                }
                break;
            case TgItem2dPrivateMessageType::ParentItemToVisible:
            case TgItem2dPrivateMessageType::ParentItemToInvisible:
            case TgItem2dPrivateMessageType::ParentItemToUseRoundedPositionValues:
            case TgItem2dPrivateMessageType::ParentItemToUseNotRoundedPositionValues:
            default:
                break;
        }
        m_listChildren[i]->sendMessageToChildren(message);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::checkOnResizeChangedOnChildren
 *
 * checks if position or size is changed on children
 */
void TgItem2dPrivate::checkOnResizeChangedOnChildren()
{
    TG_FUNCTION_BEGIN();
    for (size_t i=0;i<m_listChildren.size();i++) {
        m_listChildren[i]->checkOnResizeChanged();
        m_listChildren[i]->checkOnResizeChangedOnChildren();
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPrivate::getEnabled
 *
 * \return is enabled
 */
bool TgItem2dPrivate::getEnabled()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_enabled;
}

/*!
 * \brief TgItem2dPrivate::setEnabled
 *
 * \param enabled sets enabled true/false
 */
void TgItem2dPrivate::setEnabled(bool enabled)
{
    TG_FUNCTION_BEGIN();
    m_enabled = enabled;
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
    for (size_t i=0;i<m_listChildren.size()-1;i++) {
        if (m_listChildren[i] == child) {
            m_listChildren.erase(m_listChildren.begin()+i);
            m_listChildren.push_back(child);
            break;
        }
    }
    TG_FUNCTION_END();
}

