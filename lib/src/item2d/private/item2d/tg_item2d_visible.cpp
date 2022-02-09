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
    m_visibleState(TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible),
    f_visibleChanged(nullptr)
{
    if (m_parent) {
        if (m_parent->getVisible()) {
            m_visibleState = TgItem2dVisibilityState::TgItem2dVisible;
        }
    } else {
        m_visibleState = TgItem2dVisibilityState::TgItem2dInvisible;
    }
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
        msg.m_type = TgItem2dPrivateMessageType::ParentItemToVisible;
        m_currentItem2dPrivate->sendMessageToChildren(&msg, false);

        if (f_visibleChanged) {
            f_visibleChanged(true);
        }
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
        msg.m_type = TgItem2dPrivateMessageType::ParentItemToInvisible;
        m_currentItem2dPrivate->sendMessageToChildren(&msg, false);

        if (f_visibleChanged) {
            f_visibleChanged(false);
        }
        m_currentItem2dPrivate->setSelected(false);
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
 */
void TgItem2dVisible::parentVisibleChanged(bool visible)
{
    TG_FUNCTION_BEGIN();
    if (visible) {
        switch (m_visibleState) {
            case TgItem2dVisibilityState::TgItem2dInvisible:
            case TgItem2dVisibilityState::TgItem2dVisible:
            default:
                TG_FUNCTION_END();
                return;
            case TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dVisible;
                break;
        }
        if (f_visibleChanged) {
            f_visibleChanged(true);
        }
    } else {
        switch (m_visibleState) {
            case TgItem2dVisibilityState::TgItem2dInvisible:
            case TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible:
            default:
                TG_FUNCTION_END();
                return;
            case TgItem2dVisibilityState::TgItem2dVisible:
                m_visibleState = TgItem2dVisibilityState::TgItem2dVisibleButParentInvisible;
                break;
        }
        if (f_visibleChanged) {
            f_visibleChanged(false);
        }
    }

    TG_FUNCTION_END();
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
