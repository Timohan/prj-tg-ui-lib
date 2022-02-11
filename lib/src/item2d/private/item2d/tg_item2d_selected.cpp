/*!
 * \file
 * \brief file tg_item2d_selected.cpp
 *
 * it holds selected values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_selected.h"
#include <cmath>
#include "../../../global/tg_global_log.h"
#include "../../tg_item2d.h"
#include "tg_item2d_private.h"

TgItem2dSelected::TgItem2dSelected(TgItem2d *parent, TgItem2dPrivate *currentItem2dPrivate) :
    m_parent(parent),
    m_currentItem2dPrivate(currentItem2dPrivate),
    m_selected(false),
    m_canSelect(false)
{
}

/*!
 * \brief TgItem2dSelected::getSelected
 *
 * \return is selected
 */
bool TgItem2dSelected::getSelected()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_selected;
}

/*!
 * \brief TgItem2dSelected::getCanSelect
 *
 * \return can item be selected
 */
bool TgItem2dSelected::getCanSelect()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_canSelect && m_currentItem2dPrivate->getEnabled();
}

/*!
 * \brief TgItem2dSelected::setSelected
 *
 * \param selected sets selected true/false
 */
void TgItem2dSelected::setSelected(bool selected)
{
    TG_FUNCTION_BEGIN();
    if (m_selected == selected) {
        TG_FUNCTION_END();
        return;
    }
    m_selected = selected;
    if (m_currentItem2dPrivate->m_internalCallback) {
        m_currentItem2dPrivate->m_internalCallback->onSelectedCallback();
    }
    if (selected) {
        TgItem2dPrivateMessage msg;
        msg.m_fromItem = m_currentItem2dPrivate->m_currentItem;
        msg.m_type = TgItem2dPrivateMessageType::SetUnselected;
        m_currentItem2dPrivate->sendMessageToChildrenFromBegin(&msg);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dSelected::setCanSelect
 *
 * \param canSelect sets "can be selected" true/false
 */
void TgItem2dSelected::setCanSelect(bool canSelect)
{
    TG_FUNCTION_BEGIN();
    m_canSelect = canSelect;
    TG_FUNCTION_END();
}