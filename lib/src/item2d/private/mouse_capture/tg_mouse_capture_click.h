/*!
 * \file
 * \brief file tg_mouse_capture_click.h
 *
 * Mouse capture area private's click functions/handler
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MOUSE_CAPTURE_CLICK_H
#define TG_MOUSE_CAPTURE_CLICK_H

#include <functional>
#include <mutex>
#include <vector>
#include "../../../event/tg_event_data.h"

class TgMouseCaptureClick
{
public:
    explicit TgMouseCaptureClick();
    void connectOnMouseClicked(std::function<void(TgMouseType, float, float)> mouseClicked);
    void disconnectOnMouseClicked();
    void sendMouseClickedNoButton();

protected:
    void setMouseReleased(TgMouseType type, bool inArea, float x, float y, double time, bool releaseWoCallback);

private:
    std::function<void(TgMouseType, float, float)> f_mouseClicked;
    std::recursive_mutex m_mutex;
};

#endif // TG_MOUSE_CAPTURE_CLICK_H
