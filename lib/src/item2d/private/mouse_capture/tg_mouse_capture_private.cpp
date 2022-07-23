/*!
 * \file
 * \brief file tg_mouse_capture_private.cpp
 *
 * Mouse capture area private functions
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mouse_capture_private.h"
#include <algorithm>
#include "../../../global/tg_global_log.h"
#include "../../tg_mouse_capture.h"
#include "../item2d/tg_item2d_private.h"

TgMouseCapturePrivate::TgMouseCapturePrivate(TgMouseCapture *currentMouseCapture) :
    TgMouseCaptureClick(currentMouseCapture),
    m_mouseCursorOnHover(false),
    f_mousePressed(nullptr),
    f_mouseReleased(nullptr),
    f_mouseMove(nullptr),
    f_mouseScrollMove(nullptr)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::getMouseCursorOnHover
 *
 * \return if true, mouse cursor is on mouse capture area
 */
bool TgMouseCapturePrivate::getMouseCursorOnHover()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_mouseCursorOnHover;
}

/*!
 * \brief TgMouseCapturePrivate::setMouseCursorOnHover
 *
 * \param mouseCursorOnHover if true, mouse cursor is on mouse capture area
 */
void TgMouseCapturePrivate::setMouseCursorOnHover(bool mouseCursorOnHover)
{
    TG_FUNCTION_BEGIN();
    if (m_mouseCursorOnHover == mouseCursorOnHover) {
        TG_FUNCTION_END();
        return;
    }
    if (mouseCursorOnHover) {
        TgItem2dPrivateMessage msg;
        msg.m_fromItem = m_currentMouseCapture;
        msg.m_type = TgItem2dPrivateMessageType::HoverEnabledOnItem;
        m_currentMouseCapture->sendMessageToChildrenFromBegin(&msg);
    }
    if (!m_currentMouseCapture->getEnabled()) {
        if (!mouseCursorOnHover) {
            m_mouseCursorOnHover = mouseCursorOnHover;
        }
        TG_FUNCTION_END();
        return;
    }
    m_mouseCursorOnHover = mouseCursorOnHover;
    m_currentMouseCapture->onHoverChanged(mouseCursorOnHover);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::getMousePressed
 *
 * get if this mouse capture area is pressed down for button type
 *
 * \param type button type
 * \param useLock if true, uses mutex lock, false not (false is used in private)
 * \return true if this mouse capture area is pressed down
 */
bool TgMouseCapturePrivate::getMousePressed(TgMouseType type, bool useLock)
{
    TG_FUNCTION_BEGIN();
    if (useLock) {
        m_mutex.lock();
    }
    bool ret = std::find(m_listButtonDown.begin(), m_listButtonDown.end(), type) != m_listButtonDown.end();
    if (useLock) {
        m_mutex.unlock();
    }
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgMouseCapturePrivate::getMousePressedAnyButton
 *
 * get if this mouse capture area is pressed down for any button type
 *
 * \param useLock if true, uses mutex lock, false not (false is used in private)
 * \return true if this mouse capture area is pressed down
 */
bool TgMouseCapturePrivate::getMousePressedAnyButton(bool useLock)
{
    TG_FUNCTION_BEGIN();
    if (useLock) {
        m_mutex.lock();
    }
    bool ret = !m_listButtonDown.empty();
    if (useLock) {
        m_mutex.unlock();
    }
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgMouseCapturePrivate::setMousePressed
 *
 * when mouse press (or release) happens, this is called
 *
 * \param type left/right/middle button?
 * \param mousePressed true == pressed, false == released
 * \param inArea true if mouse pressed (or released) happens in visible area of mouse capture
 * -> pressed have true inArea always
 * -> released can have true or false inArea
 * \param x
 * \param y
 * \param time
 * \param releaseWoCallback
 */
void TgMouseCapturePrivate::setMousePressed(TgMouseType type, bool mousePressed, bool inArea, float x, float y, double time, bool releaseWoCallback)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (mousePressed) {
        if (!getMousePressed(type, false)) {
            m_listButtonDown.push_back(type);
            if (f_mousePressed) {
                f_mousePressed(type, x, y);
            }
            m_currentMouseCapture->onMousePressed(type, x, y);
            TgMouseCaptureSwipe::swipePressed(type, x, y, time);
        }
    } else if (!mousePressed) {
        std::vector<TgMouseType>::iterator it;
        for (it=m_listButtonDown.begin();it!=m_listButtonDown.end();it++) {
            if (*it == type) {
                m_listButtonDown.erase(it);
                if (f_mouseReleased) {
                    f_mouseReleased(type, inArea, x, y);
                }
                m_currentMouseCapture->onMouseReleased(type, inArea, x, y);
                if (!TgMouseCaptureSwipe::swipeReleased(type, x, y, time, releaseWoCallback)
                    && !releaseWoCallback) {
                    TgMouseCaptureClick::setMouseReleased(type, inArea, x, y, time, releaseWoCallback);
                }
                break;
            }
        }
        if (!inArea && m_listButtonDown.empty() && getMouseCursorOnHover()) {
            setMouseCursorOnHover(false);
        }
    }

    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::clearMousePressed
 *
 * clears mouse pressed list
 * -> this is called for example when button gets disabled
 */
void TgMouseCapturePrivate::clearMousePressed()
{
    m_mutex.lock();
    m_listButtonDown.clear();
    m_mutex.unlock();
}

/*!
 * \brief TgMouseCapturePrivate::setMouseMove
 *
 * when mouse move happens on mouse capture, this is called
 * \param inArea true if mouse pressed (or released) happens in visible area of mouse capture
 * -> pressed have true inArea always
 * -> released can have true or false inArea
 * \return x mouse position (x)
 * \return y mouse position (y)
 */
void TgMouseCapturePrivate::setMouseMove(bool inArea, float x, float y, double time)
{
    TG_FUNCTION_BEGIN();
    if (inArea && f_mouseMove) {
        f_mouseMove(x, y);
    }
    m_currentMouseCapture->onMouseMove(inArea, x, y);
    if (inArea) {
        swipeMove(x, y, time);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::setMouseScrollMove
 *
 * when mouse scroll move happens on mouse capture, this is called
 *
 * \return x mouse scroll position move (x)
 * \return y mouse scroll position move (y)
 */
void TgMouseCapturePrivate::setMouseScrollMove(double x, double y)
{
    TG_FUNCTION_BEGIN();
    if (f_mouseScrollMove) {
        f_mouseScrollMove(x, y);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::connectOnMousePressed
 *
 * connects callback to function for on mouse pressed
 * \param mousePressed
 */
void TgMouseCapturePrivate::connectOnMousePressed(std::function<void(TgMouseType, float, float)> mousePressed)
{
    TG_FUNCTION_BEGIN();
    f_mousePressed = mousePressed;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::disconnectOnMousePressed
 *
 * disconnects callback to function for on mouse pressed
 */
void TgMouseCapturePrivate::disconnectOnMousePressed()
{
    TG_FUNCTION_BEGIN();
    f_mousePressed = nullptr;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::connectOnMouseReleased
 *
 * connects callback to function for on mouse released
 * \param mouseReleased
 */
void TgMouseCapturePrivate::connectOnMouseReleased(std::function<void(TgMouseType, bool, float, float)> mouseReleased)
{
    TG_FUNCTION_BEGIN();
    f_mouseReleased = mouseReleased;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::disconnectOnMouseReleased
 *
 * disconnects callback to function for on mouse released
 */
void TgMouseCapturePrivate::disconnectOnMouseReleased()
{
    TG_FUNCTION_BEGIN();
    f_mouseReleased = nullptr;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::connectOnMouseMove
 *
 * connects callback to function for on mouse move
 *
 * \param mouseMove function of mouse move callback
 */
void TgMouseCapturePrivate::connectOnMouseMove(std::function<void(float, float)> mouseMove)
{
    TG_FUNCTION_BEGIN();
    if (mouseMove) {
        f_mouseMove = mouseMove;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::disconnectOnMouseMove
 *
 * disconnects callback to function for on mouse move
 */
void TgMouseCapturePrivate::disconnectOnMouseMove()
{
    TG_FUNCTION_BEGIN();
    f_mouseMove = nullptr;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::connectOnMouseScrollMove
 *
 * connects callback to function for on mouse move
 *
 * \param mouseScrollMove function of mouse scroll move callback
 */
void TgMouseCapturePrivate::connectOnMouseScrollMove(std::function<void(double, double)> mouseScrollMove)
{
    TG_FUNCTION_BEGIN();
    if (mouseScrollMove) {
        f_mouseScrollMove = mouseScrollMove;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapturePrivate::disconnectOnMouseScrollMove
 *
 * disconnects callback to function for on mouse scroll move
 */
void TgMouseCapturePrivate::disconnectOnMouseScrollMove()
{
    TG_FUNCTION_BEGIN();
    f_mouseScrollMove = nullptr;
    TG_FUNCTION_END();
}

#ifdef FUNCIONAL_TEST
size_t TgMouseCapturePrivate::getMouseDownCount()
{
    return m_listButtonDown.size();
}
#endif
