/*!
 * \file
 * \brief file tg_mouse_capture_swipe.cpp
 *
 * Mouse capture area's swipe functionality
 * - this is subclass of TgMouseCapturePrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mouse_capture_swipe.h"
#include <algorithm>
#include <string.h>
#include "../../../global/tg_global_log.h"

#define MIN_TOUCH_INDEX_FOR_VALID_SWIPE             5
#define TOUCH_INDEX_COUNT_FOR_SPEED                 15
#define TOUCH_TIME_MAX_SPEED_BETWEEN_TOUCH_POINTS   0.01
#define TOUCH_IGNORE_STEPS_TIME                     0.01

TgMouseCaptureSwipe::TgMouseCaptureSwipe() :
    m_swipeType(SwipeType::SwipeTypeAll),
    m_currentMouseTypeDown(TgMouseType::NoButton),
    m_currentStepIndex(-1),
    f_mouseSwipeCallback(nullptr)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureSwipe::getSwipeType
 *
 * \return switch type (SwipeType)
 */
uint8_t TgMouseCaptureSwipe::getSwipeType()
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    uint8_t ret = m_swipeType;
    m_mutexSwipe.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgMouseCaptureSwipe::setSwipeType
 *
 * \param swipeType switch type (SwipeType)
 */
void TgMouseCaptureSwipe::setSwipeType(uint8_t swipeType)
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    m_swipeType = swipeType;
    m_mutexSwipe.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureSwipe::connectOnMouseSwipe
 *
 * connects callback to function for on mouse swipe
 *
 * \param mouseSwipeCallback function of mouse swipe callback
 */
void TgMouseCaptureSwipe::connectOnMouseSwipe(std::function<void(TgMouseType, SwipeType, double)> mouseSwipeCallback)
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    f_mouseSwipeCallback = mouseSwipeCallback;
    m_mutexSwipe.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureSwipe::disconnectOnMouseSwipe
 *
 * disconnects callback to function for on mouse swipe
 */
void TgMouseCaptureSwipe::disconnectOnMouseSwipe()
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    f_mouseSwipeCallback = nullptr;
    m_mutexSwipe.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureSwipe::swipePressed
 *
 * this is called from mouse event pressed
 *
 * \param x touch x position
 * \param y touch y position
 * \param time touch time
 */
void TgMouseCaptureSwipe::swipePressed(TgMouseType type, float x, float y, double time)
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    if (m_currentMouseTypeDown != TgMouseType::NoButton) {
        m_mutexSwipe.unlock();
        TG_FUNCTION_END();
        return;
    }
    m_currentStepIndex = 0;
    memset(m_swipeStep, 0, sizeof(m_swipeStep));
    m_swipeStep[m_currentStepIndex].m_pressedX = x;
    m_swipeStep[m_currentStepIndex].m_pressedY = y;
    m_swipeStep[m_currentStepIndex].m_time = time;
    m_swipeStep[m_currentStepIndex].m_touchIndex = 1;
    m_currentMouseTypeDown = type;
    m_mutexSwipe.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureSwipe::swipeReleased
 *
 * this is called from mouse event released
 *
 * \param x touch x position
 * \param y touch y position
 * \param time touch time
 * \param releaseWoCallback if true, then not send swipe callback
 * \return true if f_mouseSwipeCallback was called
 */
bool TgMouseCaptureSwipe::swipeReleased(TgMouseType type, float x, float y, double time, bool releaseWoCallback)
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    if (m_currentMouseTypeDown != type || !f_mouseSwipeCallback) {
        m_mutexSwipe.unlock();
        TG_FUNCTION_END();
        return false;
    }
    addSwipeTouchPoint(x, y, time);
    double swipeSpeed;
    SwipeType swipeType = TgMouseCaptureSwipe::getSwipeSpeedAndDirection(swipeSpeed);
    m_currentStepIndex = -1;
    m_currentMouseTypeDown = TgMouseType::NoButton;
    m_mutexSwipe.unlock();
    if (swipeType != SwipeType::SwipeTypeNothing
        && !releaseWoCallback
        && (m_swipeType & swipeType)) {
        f_mouseSwipeCallback(type, swipeType, swipeSpeed);
        TG_FUNCTION_END();
        return true;
    }
    TG_FUNCTION_END();
    return false;
}
/*!
 * \brief TgMouseCaptureSwipe::swipeMove
 *
 * this is called from mouse event move
 *
 * \param x touch x position
 * \param y touch y position
 * \param time touch time
 */
void TgMouseCaptureSwipe::swipeMove(float x, float y, double time)
{
    TG_FUNCTION_BEGIN();
    m_mutexSwipe.lock();
    if (m_currentStepIndex == -1) {
        m_mutexSwipe.unlock();
        TG_FUNCTION_END();
        return;
    }
    addSwipeTouchPoint(x, y, time);
    m_mutexSwipe.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCaptureSwipe::addSwipeTouchPoint
 *
 * add new point to touch list
 * \param x touch x position
 * \param y touch y position
 * \param time touch time
 */
void TgMouseCaptureSwipe::addSwipeTouchPoint(float x, float y, double time)
{
    int previousCurrentIndex = m_currentStepIndex;
    m_currentStepIndex++;
    if (m_currentStepIndex == TG_MOUSE_CAPTURE_SWIPE_COUNT) {
        m_currentStepIndex = 0;
    }
    m_swipeStep[m_currentStepIndex].m_pressedX = x;
    m_swipeStep[m_currentStepIndex].m_pressedY = y;
    m_swipeStep[m_currentStepIndex].m_time = time;
    m_swipeStep[m_currentStepIndex].m_touchIndex = m_swipeStep[previousCurrentIndex].m_touchIndex + 1;
}

/*!
 * \brief TgMouseCaptureSwipe::setupNewCurrentIndex
 *
 * setups the new current index after the release of mouse
 * button
 *
 * All indexes that happened in last TOUCH_IGNORE_STEPS_TIME
 * are ignored
 */
void TgMouseCaptureSwipe::setupNewCurrentIndex()
{
    int i;
    int newCurrent = m_currentStepIndex;
    double endTime = m_swipeStep[m_currentStepIndex].m_time;
    for (i=0;i<TG_MOUSE_CAPTURE_SWIPE_COUNT;i++) {
        newCurrent--;
        if (newCurrent <= 0) {
            if (newCurrent == -1) {
                newCurrent = TG_MOUSE_CAPTURE_SWIPE_COUNT - 1;
            }
        }

        if (TOUCH_IGNORE_STEPS_TIME < endTime - m_swipeStep[newCurrent].m_time
            || m_swipeStep[newCurrent].m_touchIndex == 0) {
            m_currentStepIndex = newCurrent + 1;
            if (m_currentStepIndex == TG_MOUSE_CAPTURE_SWIPE_COUNT) {
                m_currentStepIndex = 0;
            }
            break;
        }
    }
}

/*!
 * \brief TgMouseCaptureSwipe::getSwipeSpeedAndDirection
 *
 * calculates swipe type and speed for all indexes
 *
 * \param speed [out] speed of swipe
 * \return SwipeTypeNothing if not valid speed, otherwise returns
 * single dirction of swipe
 */
SwipeType TgMouseCaptureSwipe::getSwipeSpeedAndDirection(double &speed)
{
    setupNewCurrentIndex();
    if (m_swipeStep[m_currentStepIndex].m_touchIndex <= MIN_TOUCH_INDEX_FOR_VALID_SWIPE + TOUCH_INDEX_COUNT_FOR_SPEED) {
        return SwipeType::SwipeTypeNothing;
    }

    int i;
    int startCurrentStepIndex = m_currentStepIndex;
    double tmpDiffTime;
    double tmpDiffPixels;
    SwipeType defaultType;
    defaultType = getSwipeSpeedAndDirection(startCurrentStepIndex, tmpDiffTime, tmpDiffPixels);
    if (defaultType == SwipeType::SwipeTypeAll) {
        startCurrentStepIndex--;
        if (startCurrentStepIndex == -1) {
            startCurrentStepIndex = TG_MOUSE_CAPTURE_SWIPE_COUNT - 1;
        }
        defaultType = getSwipeSpeedAndDirection(startCurrentStepIndex, tmpDiffTime, tmpDiffPixels);
    }

    if (defaultType == SwipeType::SwipeTypeNothing
        || defaultType == SwipeType::SwipeTypeAll
        || (m_swipeType & defaultType) == 0) {
        return SwipeType::SwipeTypeNothing;
    }

    double diffTime = tmpDiffTime;
    double diffPixels = tmpDiffPixels;

    for (i=1;i<TOUCH_INDEX_COUNT_FOR_SPEED;i++) {
        startCurrentStepIndex--;
        if (startCurrentStepIndex == -1) {
            startCurrentStepIndex = TG_MOUSE_CAPTURE_SWIPE_COUNT - 1;
        }
        if (getSwipeSpeedAndDirection(startCurrentStepIndex, tmpDiffTime, tmpDiffPixels) != defaultType) {
            return SwipeType::SwipeTypeNothing;
        }
        diffTime += tmpDiffTime;
        diffPixels += tmpDiffPixels;
    }

    speed = diffPixels/(diffTime*static_cast<double>(TOUCH_INDEX_COUNT_FOR_SPEED));
    return defaultType;
}


/*!
 * \brief TgMouseCaptureSwipe::getSwipeSpeedAndDirection
 *
 * get index's swipe type and speed
 *
 * \param currentIndex [in] index of speed and swipe type
 * \param diffTime [out] difference of time between steps (for calc speed)
 * \param diffPixels [out] difference of pixels between steps (for calc speed)
 * \return SwipeTypeNothing if not valid speed, otherwise returns
 * single dirction of swipe
 */
SwipeType TgMouseCaptureSwipe::getSwipeSpeedAndDirection(int currentIndex, double &diffTime, double &diffPixels)
{
    int i;
    int previousCurrentIndex = currentIndex - 1;
    for (i=0;i<MIN_TOUCH_INDEX_FOR_VALID_SWIPE;i++) {
        previousCurrentIndex--;
        if (previousCurrentIndex == -1) {
            previousCurrentIndex = TG_MOUSE_CAPTURE_SWIPE_COUNT - 1;
        }
    }
    double speed;
    diffTime = m_swipeStep[currentIndex].m_time - m_swipeStep[previousCurrentIndex].m_time;
    double diffX = static_cast<double>(m_swipeStep[currentIndex].m_pressedX - m_swipeStep[previousCurrentIndex].m_pressedX);
    double diffY = static_cast<double>(m_swipeStep[currentIndex].m_pressedY - m_swipeStep[previousCurrentIndex].m_pressedY);
    if (diffTime < 0
        || (diffTime > TOUCH_TIME_MAX_SPEED_BETWEEN_TOUCH_POINTS && std::abs(diffY) <= 0 && std::abs(diffX) <= 0)
        || m_swipeStep[previousCurrentIndex].m_touchIndex == 1 ) {
        return SwipeType::SwipeTypeNothing;
    }
    if (diffTime <= 0) {
        diffTime = 0.00001;
    }

    if (std::abs(diffY) <= 0 && std::abs(diffX) <= 0) {
        return SwipeType::SwipeTypeAll;
    }

    if (std::abs(diffY) > std::abs(diffX)) {
        // up or down swipe
        speed = std::abs(diffY/diffTime);
        if (speed <= TOUCH_TIME_MAX_SPEED_BETWEEN_TOUCH_POINTS) {
            return SwipeType::SwipeTypeNothing;
        }
        diffPixels = std::abs(diffY);
        if (diffY > 0) {
            return SwipeType::SwipeTypeDown;
        }
        return SwipeType::SwipeTypeUp;
    } else {
        // left or right swipe
        speed = std::abs(diffX/diffTime);
        if (speed <= TOUCH_TIME_MAX_SPEED_BETWEEN_TOUCH_POINTS) {
           return SwipeType::SwipeTypeNothing;
        }
        diffPixels = std::abs(diffX);
        if (diffX > 0) {
            return SwipeType::SwipeTypeRight;
        }
        return SwipeType::SwipeTypeLeft;
    }
}