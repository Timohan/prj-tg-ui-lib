/*!
 * \file
 * \brief file tg_item2d_enabled.cpp
 *
 * it holds enabled values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_enabled.h"
#include <cmath>
#include "../../../global/tg_global_log.h"
#include "../../tg_item2d.h"
#include "tg_item2d_private.h"
#include "../../../global/private/tg_global_wait_renderer.h"

TgItem2dEnabled::TgItem2dEnabled(TgItem2d *parent, TgItem2dPrivate *currentItem2dPrivate) :
    m_parent(parent),
    m_currentItem2dPrivate(currentItem2dPrivate),
    m_enabledState(
        parent
            ? parent->getEnabled() ? TgItem2dEnabledState::TgItem2dEnabled : TgItem2dEnabledState::TgItem2dEnabledButParentDisabled
            : TgItem2dEnabledState::TgItem2dEnabled),
    m_requireRecheckEnabledChangeToChildren(false),
    f_enabledChanged(nullptr)
{
}

/*!
 * \brief TgItem2dEnabled::getEnabled
 *
 * get enabled
 *
 * \return enabled
 */
bool TgItem2dEnabled::getEnabled()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_enabledState == TgItem2dEnabledState::TgItem2dEnabled;
}

/*!
 * \brief TgItem2dEnabled::setEnabled
 *
 * sets this item enabled/disabled
 *
 * \param enabled
 */
void TgItem2dEnabled::setEnabled(bool enabled)
{
    TG_FUNCTION_BEGIN();
    TgItem2dPrivateMessage msg;
    if (enabled) {
        switch (m_enabledState) {
            case TgItem2dEnabledState::TgItem2dEnabled:
            case TgItem2dEnabledState::TgItem2dEnabledButParentDisabled:
            default:
                TG_FUNCTION_END();
                return;
            case TgItem2dEnabledState::TgItem2dDisabled:
                m_enabledState = TgItem2dEnabledState::TgItem2dEnabled;
                break;
        }

        if (f_enabledChanged) {
            f_enabledChanged(true);
        }
        m_currentItem2dPrivate->m_currentItem->onEnabledChanged(true);

        msg.m_type = TgItem2dPrivateMessageType::ParentItemToEnabled;
        m_currentItem2dPrivate->sendMessageToChildren(&msg, false);

        TgItem2dPrivateMessage msgEnabled;
        msgEnabled.m_type = TgItem2dPrivateMessageType::ItemToEnabledChanged;
        msgEnabled.m_fromItem = nullptr;
        m_currentItem2dPrivate->sendMessageToChildrenFromBegin(&msgEnabled);
    } else {
        switch (m_enabledState) {
            case TgItem2dEnabledState::TgItem2dDisabled:
            default:
                TG_FUNCTION_END();
                return;
            case TgItem2dEnabledState::TgItem2dEnabledButParentDisabled:
                m_enabledState = TgItem2dEnabledState::TgItem2dDisabled;
                TG_FUNCTION_END();
                return;
            case TgItem2dEnabledState::TgItem2dEnabled:
                m_enabledState = TgItem2dEnabledState::TgItem2dDisabled;
                break;
        }
        if (f_enabledChanged) {
            f_enabledChanged(false);
        }
        m_currentItem2dPrivate->m_currentItem->onEnabledChanged(false);
        m_currentItem2dPrivate->setSelected(false);

        msg.m_type = TgItem2dPrivateMessageType::ParentItemToDisabled;
        m_currentItem2dPrivate->sendMessageToChildren(&msg, false);

        TgItem2dPrivateMessage msgDisabled;
        msgDisabled.m_fromItem = nullptr;
        msgDisabled.m_type = TgItem2dPrivateMessageType::CurrentItemToDisabled;
        m_currentItem2dPrivate->m_currentItem->handlePrivateMessage(&msgDisabled);

        TgItem2dPrivateMessage msgEnabled;
        msgEnabled.m_type = TgItem2dPrivateMessageType::ItemToEnabledChanged;
        msgEnabled.m_fromItem = nullptr;
        m_currentItem2dPrivate->sendMessageToChildrenFromBegin(&msgEnabled);
    }
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dEnabled::parentEnabledChanged
 *
 * sets this item enabled/disabled by parent enabled changed
 * This function is called from sendMessageToChildren
 *
 * \param enabled
 * \return true if can continue sending same message to parentEnabledChanged()
 * to this item's children
 */
bool TgItem2dEnabled::parentEnabledChanged(bool enabled)
{
    TG_FUNCTION_BEGIN();
    if (enabled) {
        switch (m_enabledState) {
            case TgItem2dEnabledState::TgItem2dDisabled:
                TG_FUNCTION_END();
                return false;
            case TgItem2dEnabledState::TgItem2dEnabled:
            default:
                TG_FUNCTION_END();
                return true;
            case TgItem2dEnabledState::TgItem2dEnabledButParentDisabled:
                m_enabledState = TgItem2dEnabledState::TgItem2dEnabled;
                break;
        }
        if (f_enabledChanged) {
            f_enabledChanged(true);
        }
        m_currentItem2dPrivate->m_currentItem->onEnabledChanged(true);
    } else {
        switch (m_enabledState) {
            case TgItem2dEnabledState::TgItem2dDisabled:
            case TgItem2dEnabledState::TgItem2dEnabledButParentDisabled:
            default:
                TG_FUNCTION_END();
                return true;
            case TgItem2dEnabledState::TgItem2dEnabled:
                m_enabledState = TgItem2dEnabledState::TgItem2dEnabledButParentDisabled;
                break;
        }
        if (f_enabledChanged) {
            f_enabledChanged(false);
        }
        m_currentItem2dPrivate->m_currentItem->onEnabledChanged(false);
        m_currentItem2dPrivate->setSelected(false);

        TgItem2dPrivateMessage msg;
        msg.m_fromItem = nullptr;
        msg.m_type = TgItem2dPrivateMessageType::CurrentItemToDisabled;
        m_currentItem2dPrivate->m_currentItem->handlePrivateMessage(&msg);
    }

    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgItem2dEnabled::connectOnEnabledChanged
 *
 * set connect enabled changed callback
 * \param enabledChanged callback of enabled changed
 */
void TgItem2dEnabled::connectOnEnabledChanged(std::function<void(bool)> enabledChanged)
{
    TG_FUNCTION_BEGIN();
    if (enabledChanged) {
        f_enabledChanged = enabledChanged;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dEnabled::disconnectOnEnabledChanged
 *
 * disconnect enabled changed callback
 */
void TgItem2dEnabled::disconnectOnEnabledChanged()
{
    TG_FUNCTION_BEGIN();
    f_enabledChanged = nullptr;
    TG_FUNCTION_END();
}

void TgItem2dEnabled::setRequireRecheckEnabledChangeToChildren(bool require)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_requireRecheckEnabledChangeToChildren = require;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

void TgItem2dEnabled::reCheckChildrenEnabled()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (!m_requireRecheckEnabledChangeToChildren) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_requireRecheckEnabledChangeToChildren = false;
    m_mutex.unlock();
    TgItem2dPrivateMessage msg;
    if (getEnabled()) {
        msg.m_type = TgItem2dPrivateMessageType::ParentItemToEnabled;
    } else {
        msg.m_type = TgItem2dPrivateMessageType::ParentItemToDisabled;
    }
    m_currentItem2dPrivate->sendMessageToChildren(&msg, false);
    TG_FUNCTION_END();
}
