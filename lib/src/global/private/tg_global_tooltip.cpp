/*!
 * \file
 * \brief file tg_global_tooltip.cpp
 *
 * Tooltip information collected here between for temporary
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_global_tooltip.h"
#include "../tg_global_macros.h"
#include "../tg_global_log.h"

TgGlobalTooltip *TgGlobalTooltip::m_globalTooltip = nullptr;

TgGlobalTooltip *TgGlobalTooltip::getInstance()
{
    TG_FUNCTION_BEGIN();
    if (!TgGlobalTooltip::m_globalTooltip) {
        TgGlobalTooltip::m_globalTooltip = new TgGlobalTooltip();
    }
    TG_FUNCTION_END();
    return TgGlobalTooltip::m_globalTooltip;
}

/*!
 * \brief TgGlobalTooltip::setTooltipOnDisabled
 *
 * called from cursor is moved on disabled item
 */
void TgGlobalTooltip::setTooltipOnDisabled()
{
    m_mutex.lock();
    m_setValidToolTipDone = true;
    m_mutex.unlock();
}

/*!
 * \brief TgGlobalTooltip::setTooltip
 *
 * called from items due handling the events
 * \param text [in]
 * \param x [in]
 * \param y [in]
 * \param currentTooltipParentItem [in]
 */
void TgGlobalTooltip::setTooltip(const std::string &text, int x, int y, TgItem2dTooltip *currentTooltipParentItem)
{
    m_mutex.lock();
    if (m_setValidToolTipDone) {
        m_mutex.unlock();
        return;
    }
    m_newRenderToolTipInfo.m_text = text;
    m_newRenderToolTipInfo.m_x = x;
    m_newRenderToolTipInfo.m_y = y;
    m_newRenderToolTipInfo.m_currentTooltipParentItem = currentTooltipParentItem;
    if (!text.empty()) {
        m_setValidToolTipDone = true;
    }
    m_mutex.unlock();
}

/*!
 * \brief TgGlobalTooltip::startHandleEvents
 *
 * called before handling the events
 */
void TgGlobalTooltip::startHandleEvents()
{
    m_setValidToolTipDone = false;
    m_newRenderToolTipInfo.m_text.clear();
    m_newRenderToolTipInfo.m_currentTooltipParentItem = nullptr;
}

int TgGlobalTooltip::getMsToWaitRendering()
{
    return m_timeMinTimeToWait;
}

/*!
 * \brief TgGlobalTooltip::startRendering
 *
 * called before start rendering
 * to set possible tooltip text for rendering
 * \param x [out]
 * \param y [out]
 * \return get tooltip text
 */
std::string TgGlobalTooltip::startRendering(int &x, int &y)
{
    std::string ret;
    m_mutex.lock();
    m_timeMinTimeToWait = -1;
    if (m_newRenderToolTipInfo.m_currentTooltipParentItem == m_previousRenderToolTipInfo.m_currentTooltipParentItem
        && m_newRenderToolTipInfo.m_text == m_previousRenderToolTipInfo.m_text
        && m_newRenderToolTipInfo.m_x == m_previousRenderToolTipInfo.m_x
        && m_newRenderToolTipInfo.m_y == m_previousRenderToolTipInfo.m_y) {
        if (m_timeDiff.elapsedTimeFromBegin() < 0.5) {
            m_timeMinTimeToWait = 501 - static_cast<int>(m_timeDiff.elapsedTimeFromBegin()*100.0);
            m_mutex.unlock();
            return "";
        }
        ret = m_previousRenderToolTipInfo.m_text;
        x = m_previousRenderToolTipInfo.m_x;
        y = m_previousRenderToolTipInfo.m_y;
        m_mutex.unlock();
        return ret;
    }
    if (m_newRenderToolTipInfo.m_currentTooltipParentItem == nullptr
        && m_previousRenderToolTipInfo.m_currentTooltipParentItem != nullptr
        && m_newRenderToolTipInfo.m_x == m_previousRenderToolTipInfo.m_x
        && m_newRenderToolTipInfo.m_y == m_previousRenderToolTipInfo.m_y
        && m_newRenderToolTipInfo.m_text.empty()
        && !m_previousRenderToolTipInfo.m_text.empty()) {
        if (m_timeDiff.elapsedTimeFromBegin() < 0.5) {
            m_timeMinTimeToWait = 501 - static_cast<int>(m_timeDiff.elapsedTimeFromBegin()*100.0);
            m_mutex.unlock();
            return "";
        }
        ret = m_previousRenderToolTipInfo.m_text;
        x = m_previousRenderToolTipInfo.m_x;
        y = m_previousRenderToolTipInfo.m_y;
        m_mutex.unlock();
        return ret;
    }
    m_previousRenderToolTipInfo.m_text = m_newRenderToolTipInfo.m_text;
    m_previousRenderToolTipInfo.m_x = m_newRenderToolTipInfo.m_x;
    m_previousRenderToolTipInfo.m_y = m_newRenderToolTipInfo.m_y;
    m_previousRenderToolTipInfo.m_currentTooltipParentItem = m_newRenderToolTipInfo.m_currentTooltipParentItem;
    if (!m_previousRenderToolTipInfo.m_text.empty()) {
        m_timeDiff.resetTimer();
    }
    m_mutex.unlock();
    return "";
}

/*!
 * \brief TgGlobalTooltip::clearTooltip
 *
 * clear tooltip information if currentTooltip item is same as
 * from current tooltip
 * - this function is called when item is destroyed, disabled or gone invisible
 * or similar
 * \param currentTooltip [in]
 */
void TgGlobalTooltip::clearTooltip(TgItem2dTooltip *currentTooltip)
{
    m_mutex.lock();
    if (currentTooltip == m_previousRenderToolTipInfo.m_currentTooltipParentItem) {
        m_previousRenderToolTipInfo.m_currentTooltipParentItem = nullptr;
        m_previousRenderToolTipInfo.m_x = -1;
        m_previousRenderToolTipInfo.m_y = -1;
        m_previousRenderToolTipInfo.m_text.clear();
    }
    if (currentTooltip == m_newRenderToolTipInfo.m_currentTooltipParentItem) {
        m_newRenderToolTipInfo.m_currentTooltipParentItem = nullptr;
        m_newRenderToolTipInfo.m_x = -1;
        m_newRenderToolTipInfo.m_y = -1;
        m_newRenderToolTipInfo.m_text.clear();
    }
    m_mutex.unlock();
}
