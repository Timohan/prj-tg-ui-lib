/*!
 * \file
 * \brief file tg_mouse_capture_private.h
 *
 * Mouse capture area private functions
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MOUSE_CAPTURE_PRIVATE_H
#define TG_MOUSE_CAPTURE_PRIVATE_H

#include <functional>
#include <mutex>
#include <vector>
#include "../../../event/tg_event_data.h"
#include "tg_mouse_capture_swipe.h"
#include "tg_mouse_capture_click.h"

class TgMouseCapturePrivate : public TgMouseCaptureClick, public TgMouseCaptureSwipe
{
public:
    explicit TgMouseCapturePrivate();
    bool getMousePressed(TgMouseType type, bool useLock = true);
    void setMousePressed(TgMouseType type, bool mousePressed, bool inArea, float x, float y, double time, bool releaseWoCallback);

    void setMouseMove(float x, float y, double time);
    void setMouseScrollMove(double x, double y);

    void connectOnMousePressed(std::function<void(TgMouseType, float, float)> mousePressed);
    void disconnectOnMousePressed();
    void connectOnMouseReleased(std::function<void(TgMouseType, bool, float, float)> mouseReleased);
    void disconnectOnMouseReleased();

    void connectOnMouseMove(std::function<void(float, float)> leftMouseMove);
    void disconnectOnMouseMove();

    void connectOnMouseScrollMove(std::function<void(double, double)> mouseScrollMove);
    void disconnectOnMouseScrollMove();

    bool getMouseCursorOnHover();
    void setMouseCursorOnHover(bool mouseCursorOnHover);

    void clearMousePressed();
private:
    bool m_mouseCursorOnHover;
    std::vector<TgMouseType>m_listButtonDown;

    std::function<void(TgMouseType, float, float)> f_mousePressed;
    std::function<void(TgMouseType, bool, float, float)> f_mouseReleased;

    std::function<void(float, float)> f_mouseMove;
    std::function<void(double, double)> f_mouseScrollMove;
    std::recursive_mutex m_mutex;
};

#endif // TG_MOUSE_CAPTURE_PRIVATE_H
