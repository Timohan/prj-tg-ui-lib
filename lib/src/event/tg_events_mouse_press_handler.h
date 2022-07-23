/*!
 * \file
 * \brief file tg_events_mouse_press_handler.h
 *
 * Additional class for TgEvents
 * handlers caching mouse down events for TgEvents
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_EVENTS_MOUSE_PRESS_HANDLER_H
#define TG_EVENTS_MOUSE_PRESS_HANDLER_H

#include <mutex>
#include <vector>
#include "tg_event_data.h"

struct TgEventsMousePressType
{
    TgItem2d *m_mouseDownItem;
    TgMouseType m_mouseType;
    bool m_releaseWoCallback;
};

class TgEventsMousePressHandler
{
public:
    TgEventsMousePressHandler();
    void setMouseDownItem(TgMouseType type, TgItem2d *mouseDownItem);
    TgItem2d *getMouseDownItem(TgMouseType type, bool &releaseWoCallback);
    TgItem2d *getMouseDownItem(bool &releaseWoCallback);
    void setMouseDownItemToNull(TgItem2d *mouseDownItem);
    size_t getMouseDownItemCount();
    void removingItem(TgMouseType type);
    void removingItem(TgItem2d *itemToRemove);

    void setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount);
    size_t getAllowedNumberMouseButtonCount();

private:
    std::vector<TgEventsMousePressType>m_listMouseType;
    size_t m_allowedNumberMouseButtonDownCount;
    std::mutex m_mutex;
};

#endif //TG_EVENTS_MOUSE_PRESS_HANDLER_H
