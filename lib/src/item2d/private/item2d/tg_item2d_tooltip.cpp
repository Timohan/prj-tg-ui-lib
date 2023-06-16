/*!
 * \file
 * \brief file tg_item2d_tooltip.cpp
 *
 * Tooltip handler
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_tooltip.h"
#include "../../../global/tg_global_log.h"
#include "../../tg_item2d.h"
#include "../../../global/private/tg_global_tooltip.h"
#include "tg_item2d_private.h"

TgItem2dTooltip::TgItem2dTooltip(TgItem2dPrivate *currentItemPrivate) :
    m_currentItem(currentItemPrivate)
{
}

TgItem2dTooltip::~TgItem2dTooltip()
{
    TgGlobalTooltip::getInstance()->clearTooltip(this);
}

/*!
 * \brief TgItem2dTooltip::setTooltip
 *
 * set tooltip text
 *
 * \return text
 */
void TgItem2dTooltip::setTooltip(const char *text)
{
    if (!text) {
        m_text.clear();
        return;
    }
    m_text = std::string(text);
}

/*!
 * \brief TgItem2dTooltip::handleEventToolTip
 *
 * handles the event for tooltip
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
void TgItem2dTooltip::handleEventToolTip(TgEventData *eventData, const TgWindowInfo *windowInfo, bool fromMainWindow)
{
    TG_FUNCTION_BEGIN();
    if (!m_currentItem->getVisible()) {
        TG_FUNCTION_END();
        return;
    }
    if (!m_currentItem->getEnabled()) {
        if ((eventData->m_type == TgEventType::EventTypeMouseMove
            || eventData->m_type == TgEventType::EventTypeMouseMoveForMenuParent)
            && m_currentItem->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
            TgGlobalTooltip::getInstance()->setTooltipOnDisabled();
        }
        TG_FUNCTION_END();
        return;
    }
    if ((eventData->m_type == TgEventType::EventTypeMouseMove
            || eventData->m_type == TgEventType::EventTypeMouseMoveForMenuParent)
        && m_currentItem->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
        if (m_canBlockTooltip && m_text.empty()) {
            TgGlobalTooltip::getInstance()->setTooltipOnDisabled();
            TG_FUNCTION_END();
            return;
        }
        TgGlobalTooltip::getInstance()->setTooltip(m_text,
                                                   static_cast<int>(eventData->m_event.m_mouseEvent.m_x),
                                                   static_cast<int>(eventData->m_event.m_mouseEvent.m_y),
                                                   this);
        TG_FUNCTION_END();
        return;
    }

    if (fromMainWindow
        && (eventData->m_type == TgEventType::EventTypeMousePress
            || eventData->m_type == TgEventType::EventTypeMouseScrollMove
            || eventData->m_type == TgEventType::EventTypeKeyPress
            || eventData->m_type == TgEventType::EventTypeMouseRelease
            || eventData->m_type == TgEventType::EventTypeKeyRelease
            || eventData->m_type == TgEventType::EventTypeKeyRepeat)) {
        TgGlobalTooltip::getInstance()->setTooltip("",
                                                   static_cast<int>(eventData->m_event.m_mouseEvent.m_x),
                                                   static_cast<int>(eventData->m_event.m_mouseEvent.m_y),
                                                   this);
    }

    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dTooltip::itemGoneDisabledOrInvisible
 *
 * called when this item is gone to disabled or Invisible
 */
void TgItem2dTooltip::itemGoneDisabledOrInvisible()
{
    TG_FUNCTION_BEGIN();
    TgGlobalTooltip::getInstance()->clearTooltip(this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dTooltip::setCanBlockTooltip
 *
 * set can block tooltip
 * \param if true, this item can block tooltip, just
 * as it is disabled
 */
void TgItem2dTooltip::setCanBlockTooltip(bool canBlockTooltip)
{
    m_canBlockTooltip = canBlockTooltip;
}
