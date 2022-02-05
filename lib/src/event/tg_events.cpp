/*!
 * \file
 * \brief file tg_events.cpp
 *
 * Handles the event "caching" before handled during rendering
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_events.h"
#include <cmath>
#include <cstdlib>
#include "../global/tg_global_log.h"

TgEvents::TgEvents()
{
}

/*!
 * \brief TgEvents::addEvent
 *
 * add event to handler list
 * \param eventData event data to add
 */
void TgEvents::addEvent(const TgEventData *eventData)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    /*
    if (eventData->m_type == TgEventType::EventTypeMouseMove && !m_eventData.empty()
        && m_eventData.back().m_type == TgEventType::EventTypeMouseMove) {
        m_eventData[m_eventData.size()-1] = *eventData;
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }*/
    if (eventData->m_type == TgEventType::EventTypeMouseScrollMove && !m_eventData.empty()
        && m_eventData.back().m_type == TgEventType::EventTypeMouseScrollMove
        && m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y > 0
        && eventData->m_event.m_mouseEvent.m_scroll_move_y > 0
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x-eventData->m_event.m_mouseEvent.m_scroll_move_x) <= std::numeric_limits<double>::epsilon()
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y-eventData->m_event.m_mouseEvent.m_scroll_move_y) <= std::numeric_limits<double>::epsilon()) {
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x += eventData->m_event.m_mouseEvent.m_scroll_move_x;
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y += eventData->m_event.m_mouseEvent.m_scroll_move_y;
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseScrollMove && !m_eventData.empty()
        && m_eventData.back().m_type == TgEventType::EventTypeMouseScrollMove
        && m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y < 0
        && eventData->m_event.m_mouseEvent.m_scroll_move_y < 0
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x-eventData->m_event.m_mouseEvent.m_scroll_move_x) <= std::numeric_limits<double>::epsilon()
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y-eventData->m_event.m_mouseEvent.m_scroll_move_y) <= std::numeric_limits<double>::epsilon()) {
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x += eventData->m_event.m_mouseEvent.m_scroll_move_x;
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y += eventData->m_event.m_mouseEvent.m_scroll_move_y;
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseScrollMove && !m_eventData.empty()
        && m_eventData.back().m_type == TgEventType::EventTypeMouseScrollMove
        && m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x > 0
        && eventData->m_event.m_mouseEvent.m_scroll_move_x > 0
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x-eventData->m_event.m_mouseEvent.m_scroll_move_x) <= std::numeric_limits<double>::epsilon()
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y-eventData->m_event.m_mouseEvent.m_scroll_move_y) <= std::numeric_limits<double>::epsilon()) {
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x += eventData->m_event.m_mouseEvent.m_scroll_move_x;
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y += eventData->m_event.m_mouseEvent.m_scroll_move_y;
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseScrollMove && !m_eventData.empty()
        && m_eventData.back().m_type == TgEventType::EventTypeMouseScrollMove
        && m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x < 0
        && eventData->m_event.m_mouseEvent.m_scroll_move_x < 0
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x-eventData->m_event.m_mouseEvent.m_scroll_move_x) <= std::numeric_limits<double>::epsilon()
        && std::fabs(m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y-eventData->m_event.m_mouseEvent.m_scroll_move_y) <= std::numeric_limits<double>::epsilon()) {
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_x += eventData->m_event.m_mouseEvent.m_scroll_move_x;
        m_eventData[m_eventData.size()-1].m_event.m_mouseEvent.m_scroll_move_y += eventData->m_event.m_mouseEvent.m_scroll_move_y;
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_eventData.push_back(*eventData);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgEvents::lock
 *
 * lock the mutex
 */
void TgEvents::lock()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgEvents::unlock
 *
 * unlock the mutex
 */
void TgEvents::unlock()
{
    TG_FUNCTION_BEGIN();
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgEvents::getFirstEventData
 *
 * get pointer of first event data
 * \return pointer of first event data
 */
TgEventData *TgEvents::getFirstEventData()
{
    TG_FUNCTION_BEGIN();
    if (m_eventData.empty()) {
        return nullptr;
    }
    TG_FUNCTION_END();
    return &m_eventData[0];
}

/*!
 * \brief TgEvents::clearFirstEventData
 *
 * clear the first event data
 */
void TgEvents::clearFirstEventData()
{
    m_eventData.erase(m_eventData.begin());
}

