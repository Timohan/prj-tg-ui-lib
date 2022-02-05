/*!
 * \file
 * \brief file tg_mouse_capture_swipe.h
 *
 * Mouse capture area's swipe functionality
 * - this is subclass of TgMouseCapturePrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MOUSE_CAPTURE_SWIPE_H
#define TG_MOUSE_CAPTURE_SWIPE_H

#include <functional>
#include <mutex>
#include <vector>
#include "../../../event/tg_event_data.h"
#include "../../../common/time_difference.h"
#include "../../tg_mouse_capture.h"

struct TgMouseCaptureSwipeStep
{
    double m_time;
    float m_pressedX;
    float m_pressedY;
    int m_touchIndex;
};

#define TG_MOUSE_CAPTURE_SWIPE_COUNT    1000

class TgMouseCaptureSwipe
{
public:
    explicit TgMouseCaptureSwipe();

    uint8_t getSwipeType();
    void setSwipeType(uint8_t swipeType);

    void connectOnMouseSwipe(std::function<void(TgMouseType, SwipeType, double)> mouseSwipeCallback);
    void disconnectOnMouseSwipe();

    void swipePressed(TgMouseType type, float x, float y, double time);
    bool swipeReleased(TgMouseType type, float x, float y, double time, bool releaseWoCallback);
    void swipeMove(float x, float y, double time);

private:
    uint8_t m_swipeType;
    TgMouseType m_currentMouseTypeDown;
    int m_currentStepIndex;
    TgMouseCaptureSwipeStep m_swipeStep[TG_MOUSE_CAPTURE_SWIPE_COUNT];

    std::function<void(TgMouseType, SwipeType, double)> f_mouseSwipeCallback;
    std::mutex m_mutexSwipe;

    void addSwipeTouchPoint(float x, float y, double time);
    SwipeType getSwipeSpeedAndDirection(double &speed);
    SwipeType getSwipeSpeedAndDirection(int currentIndex, double &diffTime, double &diffPixels);
    void setupNewCurrentIndex();
};

#endif // TG_MOUSE_CAPTURE_SWIPE_H
