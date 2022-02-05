/*!
 * \file
 * \brief file tg_events.h
 *
 * Handles the event "caching" before handled during rendering
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_EVENTS_H
#define TG_EVENTS_H

#include <mutex>
#include <vector>
#include "tg_event_data.h"
#include "tg_events_mouse_press_handler.h"

struct TgEventMouse
{
    TgItem2d *m_mouseDownItem;
    TgMouseType m_mouseType;
};

class TgEvents : public TgEventsMousePressHandler
{
public:
    explicit TgEvents();
    void addEvent(const TgEventData *eventData);
    void lock();
    void unlock();
    TgEventData *getFirstEventData();
    void clearFirstEventData();
private:
    std::vector<TgEventData>m_eventData;
    std::recursive_mutex m_mutex;
};

#endif //TG_EVENTS_H
