/*!
 * \file
 * \brief file tg_mouse_capture_click.cpp
 *
 * Mouse capture area private's click functions/handler
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mouse_capture_click.h"
#include <algorithm>
#include <string.h>
#include "../../../global/tg_global_log.h"
#include "../../tg_mouse_capture.h"

TgMouseCaptureClick::TgMouseCaptureClick(TgMouseCapture *currentMouseCapture) :
    m_currentMouseCapture(currentMouseCapture),
    f_mouseClicked(nullptr)
{
}

/*!
 * \brief TgMouseCaptureClick::setMousePressed
 *
 * when mouse press (or release) happens, this is called
 *
 * \param type left/right/middle button?
 * \param inArea true if mouse pressed (or released) happens in visible area of mouse capture
 * -> pressed have true inArea always
 * -> released can have true or false inArea
 * \param x
 * \param y
 * \param time
 * \param releaseWoCallback
 */
void TgMouseCaptureClick::setMouseReleased(TgMouseType type, bool inArea, float x, float y, double time, bool releaseWoCallback)
{
    (void)time;
    m_mutex.lock();
    if (inArea && !releaseWoCallback) {
        if (f_mouseClicked) {
            m_mutex.unlock();
            f_mouseClicked(type, x, y, m_id);
            m_currentMouseCapture->onMouseClicked(type, x, y);
            if (m_mouseCaptureInternalCallback) {
                m_mouseCaptureInternalCallback->onClickedCallback(m_currentMouseCapture);
            }
        } else {
            m_mutex.unlock();
            if (m_mouseCaptureInternalCallback) {
                m_mouseCaptureInternalCallback->onClickedCallback(m_currentMouseCapture);
            }
            m_currentMouseCapture->onMouseClicked(type, x, y);
        }
    } else {
        m_mutex.unlock();
    }
}


/*!
 * \brief TgMouseCaptureClick::connectOnMouseClicked
 *
 * connects callback to function for on mouse clicked
 * \param mouseClicked
 */
void TgMouseCaptureClick::connectOnMouseClicked(std::function<void(TgMouseType, float, float, const void *)> mouseClicked, const void *id)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    f_mouseClicked = mouseClicked;
    m_id = id;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureClick::disconnectOnMouseClicked
 *
 * disconnects callback to function for on mouse clicked
 */
void TgMouseCaptureClick::disconnectOnMouseClicked()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    f_mouseClicked = nullptr;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

void TgMouseCaptureClick::sendMouseClickedNoButton()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (f_mouseClicked) {
        m_mutex.unlock();
        f_mouseClicked(TgMouseType::NoButton, -1, -1, m_id);
        if (m_mouseCaptureInternalCallback) {
            m_mouseCaptureInternalCallback->onClickedCallback(m_currentMouseCapture);
        }
        m_currentMouseCapture->onMouseClicked(TgMouseType::NoButton, -1, -1);
    } else {
        m_mutex.unlock();
        if (m_mouseCaptureInternalCallback) {
            m_mouseCaptureInternalCallback->onClickedCallback(m_currentMouseCapture);
        }
        m_currentMouseCapture->onMouseClicked(TgMouseType::NoButton, -1, -1);
    }
    TG_FUNCTION_END();
}

void TgMouseCaptureClick::setMouseCaptureInternalCallback(TgMouseCaptureInternalCallback *captureInternalCallback)
{
    TG_FUNCTION_BEGIN();
    m_mouseCaptureInternalCallback = captureInternalCallback;
    TG_FUNCTION_END();
}