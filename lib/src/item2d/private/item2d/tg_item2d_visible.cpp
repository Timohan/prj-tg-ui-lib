/*!
 * \file
 * \brief file tg_item2d_visible.cpp
 *
 * it holds visible values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_visible.h"
#include <cmath>
#include "../../../global/tg_global_log.h"
#include "../../tg_item2d.h"
#include "tg_item2d_private.h"

TgItem2dVisible::TgItem2dVisible(TgItem2d *parent, TgItem2dPrivate *currentItem2dPrivate) :
    m_parent(parent),
    m_currentItem2dPrivate(currentItem2dPrivate),
    m_visibleState(parent
                   ? parent->getVisible()
                    ? TgItem2dVisibilityState::TgItem2dVisible
                    : TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible
                   : TgItem2dVisibilityState::TgItem2dVisible),
    m_requireRecheckVisibleChangeToChildren(false),
    f_visibleChanged(nullptr)
{
}

/*!
 * \brief TgItem2dVisible::getVisible
 *
 * get visible
 *
 * \return visible
 */
bool TgItem2dVisible::getVisible()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_visibleState == TgItem2dVisibilityState::TgItem2dVisible;
}

TgItem2dVisibilityState TgItem2dVisible::getVisibleState()
{
    return m_visibleState;
}

/*!
 * \brief TgItem2dVisible::setVisible
 *
 * sets this item visible/hidden
 *
 * \param visible
 */
void TgItem2dVisible::setVisible(bool visible)
{
    TG_FUNCTION_BEGIN();
    TgItem2dPrivateMessage msg;
    if (visible) {
        switch (m_visibleState) {
            case TgItem2dVisibilityState::TgItem2dVisible:
            case TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible:
            default:
                TG_FUNCTION_END();
                return;
            case TgItem2dVisibilityState::TgItem2dInvisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dVisible;
                break;
        }

        if (f_visibleChanged) {
            f_visibleChanged(true);
        }

        m_currentItem2dPrivate->m_currentItem->onVisibleChanged(true);

        msg.m_type = TgItem2dPrivateMessageType::ParentItemToVisible;
        m_currentItem2dPrivate->sendMessageToChildren(&msg, false);

        TgItem2dPrivateMessage msgVisible;
        msgVisible.m_type = TgItem2dPrivateMessageType::ItemToVisibleChanged;
        msgVisible.m_fromItem = nullptr;
        m_currentItem2dPrivate->sendMessageToChildrenFromBegin(&msgVisible);
    } else {
        switch (m_visibleState) {
            case TgItem2dVisibilityState::TgItem2dInvisible:
            default:
                TG_FUNCTION_END();
                return;
            case TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dInvisible;
                TG_FUNCTION_END();
                return;
            case TgItem2dVisibilityState::TgItem2dVisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dInvisible;
                break;
        }
        if (f_visibleChanged) {
            f_visibleChanged(false);
        }
        m_currentItem2dPrivate->m_currentItem->onVisibleChanged(false);
        m_currentItem2dPrivate->setSelected(false);

        msg.m_type = TgItem2dPrivateMessageType::ParentItemToInvisible;
        m_currentItem2dPrivate->sendMessageToChildren(&msg, false);

        TgItem2dPrivateMessage msgInvisible;
        msgInvisible.m_fromItem = nullptr;
        msgInvisible.m_type = TgItem2dPrivateMessageType::CurrentItemToInvisible;
        m_currentItem2dPrivate->m_currentItem->handlePrivateMessage(&msgInvisible);

        TgItem2dPrivateMessage msgVisible;
        msgVisible.m_type = TgItem2dPrivateMessageType::ItemToVisibleChanged;
        msgVisible.m_fromItem = nullptr;
        m_currentItem2dPrivate->sendMessageToChildrenFromBegin(&msgVisible);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dVisible::parentVisibleChanged
 *
 * sets this item visible/hidden by parent visible changed
 * This function is called from sendMessageToChildren
 *
 * \param visible
 * \return true if can continue sending same message to parentVisibleChanged()
 * to this item's children
 */
bool TgItem2dVisible::parentVisibleChanged(bool visible)
{
    TG_FUNCTION_BEGIN();
    if (visible) {
        switch (m_visibleState) {
            case TgItem2dVisibilityState::TgItem2dInvisible:
                TG_FUNCTION_END();
                return false;
            case TgItem2dVisibilityState::TgItem2dVisible:
            default:
                TG_FUNCTION_END();
                return true;
            case TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dVisible;
                break;
        }
        if (f_visibleChanged) {
            f_visibleChanged(true);
        }
        m_currentItem2dPrivate->m_currentItem->onVisibleChanged(true);
    } else {
        switch (m_visibleState) {
            case TgItem2dVisibilityState::TgItem2dInvisible:
            case TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible:
            default:
                TG_FUNCTION_END();
                return true;
            case TgItem2dVisibilityState::TgItem2dVisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible;
                break;
        }
        if (f_visibleChanged) {
            f_visibleChanged(false);
        }
        m_currentItem2dPrivate->m_currentItem->onVisibleChanged(false);
        m_currentItem2dPrivate->setSelected(false);

        TgItem2dPrivateMessage msg;
        msg.m_fromItem = nullptr;
        msg.m_type = TgItem2dPrivateMessageType::CurrentItemToInvisible;
        m_currentItem2dPrivate->m_currentItem->handlePrivateMessage(&msg);
    }

    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgItem2dVisible::connectOnVisibleChanged
 *
 * set connect visible changed callback
 * \param visibleChanged callback of visible changed
 */
void TgItem2dVisible::connectOnVisibleChanged(std::function<void(bool)> visibleChanged)
{
    TG_FUNCTION_BEGIN();
    if (visibleChanged) {
        f_visibleChanged = visibleChanged;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dVisible::disconnectOnVisibleChanged
 *
 * disconnect visible changed callback
 */
void TgItem2dVisible::disconnectOnVisibleChanged()
{
    TG_FUNCTION_BEGIN();
    f_visibleChanged = nullptr;
    TG_FUNCTION_END();
}

void TgItem2dVisible::setRequireRecheckVisibleChangeToChildren(bool require)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_requireRecheckVisibleChangeToChildren = require;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

void TgItem2dVisible::reCheckChildrenVisibility()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (!m_requireRecheckVisibleChangeToChildren) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_requireRecheckVisibleChangeToChildren = false;
    m_mutex.unlock();
    TgItem2dPrivateMessage msg;
    if (getVisible()) {
        msg.m_type = TgItem2dPrivateMessageType::ParentItemToVisible;
    } else {
        msg.m_type = TgItem2dPrivateMessageType::ParentItemToInvisible;
    }
    m_currentItem2dPrivate->sendMessageToChildren(&msg, false);
    TG_FUNCTION_END();
}
