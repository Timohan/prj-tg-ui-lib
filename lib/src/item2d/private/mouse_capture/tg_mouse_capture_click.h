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

class TgMouseCapture;

class TgMouseCaptureInternalCallback
{
public:
    virtual void onClickedCallback(TgItem2d *currentItem) = 0;
};

class TgMouseCaptureClick
{
public:
    explicit TgMouseCaptureClick(TgMouseCapture *currentMouseCapture);
    void connectOnMouseClicked(std::function<void(TgMouseType, float, float, const void *)> mouseClicked, const void *id);
    void disconnectOnMouseClicked();
    void sendMouseClickedNoButton();
    void setMouseCaptureInternalCallback(TgMouseCaptureInternalCallback *captureInternalCallback);

protected:
    void setMouseReleased(TgMouseType type, bool inArea, float x, float y, double time, bool releaseWoCallback);

private:
    TgMouseCaptureInternalCallback *m_mouseCaptureInternalCallback = nullptr;
    TgMouseCapture *m_currentMouseCapture;
    std::function<void(TgMouseType, float, float, const void *)> f_mouseClicked;
    const void *m_id = nullptr;
    std::recursive_mutex m_mutex;

    friend class TgMouseCapturePrivate;
};

#endif // TG_MOUSE_CAPTURE_CLICK_H
