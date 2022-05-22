/*!
 * \file
 * \brief file tg_mouse_capture.cpp
 *
 * Inits and draws rectangle
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mouse_capture.h"
#include "../global/tg_global_log.h"
#include "private/mouse_capture/tg_mouse_capture_private.h"

/*!
 * \brief TgMouseCapture::TgMouseCapture
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 */
TgMouseCapture::TgMouseCapture(TgItem2d *parent) :
    TgItem2d(parent),
    m_private(new TgMouseCapturePrivate(this))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::TgMouseCapture
 *
 * constructor to use AnchorRelativeToParent
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 */
TgMouseCapture::TgMouseCapture(TgItem2d *parent, float x, float y, float width, float height) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgMouseCapturePrivate(this))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgMouseCapture::~TgMouseCapture()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
TgEventResult TgMouseCapture::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (eventData->m_type == TgEventType::EventTypeMousePress
        && getXminOnVisible() <= eventData->m_event.m_mouseEvent.m_x
        && getXmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_x
        && getYminOnVisible() <= eventData->m_event.m_mouseEvent.m_y
        && getYmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_y) {
        if (getEnabled()) {
            onDownChanged(true);
            m_private->setMousePressed(eventData->m_event.m_mouseEvent.m_mouseType, true, true,
                                       static_cast<float>(eventData->m_event.m_mouseEvent.m_x),
                                       static_cast<float>(eventData->m_event.m_mouseEvent.m_y),
                                       eventData->m_event.m_mouseEvent.m_time,
                                       false);
            m_private->swipePressed(eventData->m_event.m_mouseEvent.m_mouseType,
                                    static_cast<float>(eventData->m_event.m_mouseEvent.m_x),
                                    static_cast<float>(eventData->m_event.m_mouseEvent.m_y),
                                    eventData->m_event.m_mouseEvent.m_time);
            eventData->m_event.m_mouseEvent.m_newItem = this;
        }
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }

    if (eventData->m_type == TgEventType::EventTypeMouseRelease
        && eventData->m_event.m_mouseEvent.m_currentMouseDownItem == this
        && m_private->getMousePressed(eventData->m_event.m_mouseEvent.m_mouseType) ) {
        if (getEnabled()) {
            onDownChanged(false);
            m_private->setMousePressed(eventData->m_event.m_mouseEvent.m_mouseType, false,
                                       getXminOnVisible() <= eventData->m_event.m_mouseEvent.m_x
                                       && getXmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_x
                                       && getYminOnVisible() <= eventData->m_event.m_mouseEvent.m_y
                                       && getYmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_y,
                                       static_cast<float>(eventData->m_event.m_mouseEvent.m_x),
                                       static_cast<float>(eventData->m_event.m_mouseEvent.m_y),
                                       eventData->m_event.m_mouseEvent.m_time,
                                       eventData->m_event.m_mouseEvent.m_releaseWoCallback);
        }
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseMove
        && (!eventData->m_event.m_mouseEvent.m_currentMouseDownItem
            || eventData->m_event.m_mouseEvent.m_currentMouseDownItem == this)) {
        if (getXminOnVisible() <= eventData->m_event.m_mouseEvent.m_x
            && getXmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_x
            && getYminOnVisible() <= eventData->m_event.m_mouseEvent.m_y
            && getYmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_y) {
            if (getEnabled()) {
                if (!m_private->getMouseCursorOnHover()) {
                    m_private->setMouseCursorOnHover(true);
                    onHoverChanged(true);
                }
                m_private->setMouseMove(true,
                                        static_cast<float>(eventData->m_event.m_mouseEvent.m_x),
                                        static_cast<float>(eventData->m_event.m_mouseEvent.m_y),
                                        eventData->m_event.m_mouseEvent.m_time);
            }
            TG_FUNCTION_END();
            return TgEventResult::EventResultCompleted;
        } else {
            if (getEnabled()) {
                if (m_private->getMouseCursorOnHover()) {
                    m_private->setMouseCursorOnHover(false);
                    onHoverChanged(false);
                }
                if (eventData->m_event.m_mouseEvent.m_currentMouseDownItem == this
                    || (!eventData->m_event.m_mouseEvent.m_currentMouseDownItem && m_private->getMousePressedAnyButton()) ) {
                    m_private->setMouseMove(false,
                                            static_cast<float>(eventData->m_event.m_mouseEvent.m_x),
                                            static_cast<float>(eventData->m_event.m_mouseEvent.m_y),
                                            eventData->m_event.m_mouseEvent.m_time);
                }
            }
        }
    }

    if (eventData->m_type == TgEventType::EventTypeMouseScrollMove
        && getXminOnVisible() <= eventData->m_event.m_mouseEvent.m_x
        && getXmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_x
        && getYminOnVisible() <= eventData->m_event.m_mouseEvent.m_y
        && getYmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_y) {
        if (getEnabled()) {
            m_private->setMouseScrollMove(eventData->m_event.m_mouseEvent.m_scroll_move_x,
                                          eventData->m_event.m_mouseEvent.m_scroll_move_y);
        }
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    TG_FUNCTION_END();
    return TgItem2d::handleEvent(eventData, windowInfo);
}

/*!
 * \brief TgMouseCapture::connectOnMousePressed
 *
 * connects callback to function for on mouse pressed
 * \param mousePressed
 */
void TgMouseCapture::connectOnMousePressed(std::function<void(TgMouseType, float, float)> mousePressed)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnMousePressed(mousePressed);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::disconnectOnMousePressed
 *
 * disconnects callback to function for on mouse pressed
 */
void TgMouseCapture::disconnectOnMousePressed()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnMousePressed();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::connectOnMouseReleased
 *
 * connects callback to function for on mouse released
 * \param mouseReleased
 */
void TgMouseCapture::connectOnMouseReleased(std::function<void(TgMouseType, bool, float, float)> mouseReleased)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnMouseReleased(mouseReleased);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::disconnectOnMouseReleased
 *
 * disconnects callback to function for on mouse released
 */
void TgMouseCapture::disconnectOnMouseReleased()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnMouseReleased();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::connectOnMouseClicked
 *
 * connects callback to function for on mouse clicked
 * \param mouseClicked
 */
void TgMouseCapture::connectOnMouseClicked(std::function<void(TgMouseType, float, float)> mouseClicked)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnMouseClicked(mouseClicked);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::disconnectOnMouseClicked
 *
 * disconnects callback to function for on mouse clicked
 */
void TgMouseCapture::disconnectOnMouseClicked()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnMouseClicked();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::connectOnMouseMove
 *
 * connects callback to function for on mouse move
 *
 * \param mouseMove function of mouse mouse callback
 */
void TgMouseCapture::connectOnMouseMove(std::function<void(float, float)> mouseMove)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnMouseMove(mouseMove);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::disconnectOnMouseMove
 *
 * disconnects callback to function for on mouse move
 */
void TgMouseCapture::disconnectOnMouseMove()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnMouseMove();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::connectOnMouseScrollMove
 *
 * connects callback to function for on mouse move
 *
 * \param mouseScrollMove function of mouse scroll move callback
 */
void TgMouseCapture::connectOnMouseScrollMove(std::function<void(double, double)> mouseScrollMove)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnMouseScrollMove(mouseScrollMove);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::disconnectOnMouseScrollMove
 *
 * disconnects callback to function for on mouse scroll move
 */
void TgMouseCapture::disconnectOnMouseScrollMove()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnMouseScrollMove();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::onHoverChanged
 *
 * virtual function that can be used to capture message when
 * cursor is on the mouse capture or not
 * \param hover if true, mouse cursor is on mouse capture area
 */
void TgMouseCapture::onHoverChanged(bool hover)
{
    TG_FUNCTION_BEGIN();
    (void)hover;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::onHoverDownChanged
 *
 * virtual function when down is changed
 *
 * \param down if true, mouse cursor press down
 */
void TgMouseCapture::onDownChanged(bool down)
{
    TG_FUNCTION_BEGIN();
    (void)down;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::onMouseClicked
 *
 * virtual function when mouse is clicked
 *
 * \param button button
 * \param x x position
 * \param y y position
 */
void TgMouseCapture::onMouseClicked(TgMouseType button, float x, float y)
{
    TG_FUNCTION_BEGIN();
    (void)button;
    (void)x;
    (void)y;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::onMousePressed
 *
 * virtual function when mouse is pressed down
 *
 * \param button button
 * \param x x position
 * \param y y position
 */
void TgMouseCapture::onMousePressed(TgMouseType button, float x, float y)
{
    TG_FUNCTION_BEGIN();
    (void)button;
    (void)x;
    (void)y;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::onMouseReleased
 *
 * virtual function when mouse is released (up)
 *
 * \param button button
 * \param inArea if true, mouse was released on this TgMouseCapture area,
 * if false, then mouse was released outside the TgMouseCapture area
 * \param x x position
 * \param y y position
 */
void TgMouseCapture::onMouseReleased(TgMouseType button, bool inArea, float x, float y)
{
    TG_FUNCTION_BEGIN();
    (void)button;
    (void)inArea;
    (void)x;
    (void)y;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::onMouseMove
 *
 * virtual function when mouse is moving on the TgMouseCapture area
 *
 * \param inArea if true, mouse was moving in this area
 * this can be false, only if it was pressed down in the this
 * TgMouseCapture
 * \param x x position
 * \param y y position
 */
void TgMouseCapture::onMouseMove(bool inArea, float x, float y)
{
    TG_FUNCTION_BEGIN();
    (void)inArea;
    (void)x;
    (void)y;
    TG_FUNCTION_END();
}


/*!
 * \brief TgMouseCapture::getMouseCursorOnHover
 *
 * \return if true, mouse cursor is on mouse capture area
 */
bool TgMouseCapture::getMouseCursorOnHover()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getMouseCursorOnHover();
}

/*!
 * \brief TgMouseCapture::onEnabledChanged
 *
 * virtual function when enabled changed
 *
 * \param enabled enabled true/false
 */
void TgMouseCapture::onEnabledChanged(bool enabled)
{
    TG_FUNCTION_BEGIN();
    if (!enabled) {
        m_private->setMouseCursorOnHover(false);
        m_private->clearMousePressed();
    }
    TG_FUNCTION_END();
}


/*!
 * \brief TgMouseCapture::getSwipeType
 *
 * \return current allowed switch types (SwipeType) for
 * swipe types, it can contain multiple directions for swipe
 * for example: SwipeTypeUp | SwipeTypeDown if you want callback
 * for up and down directions
 */
uint8_t TgMouseCapture::getSwipeType()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getSwipeType();
}

/*!
 * \brief TgMouseCapture::setSwipeType
 *
 * \param swipeType allowed switch types (SwipeType) for
 * swipe types, it can contain multiple directions for swipe
 * for example: SwipeTypeUp | SwipeTypeDown if you want callback
 * for up and down directions
 */
void TgMouseCapture::setSwipeType(uint8_t swipeType)
{
    TG_FUNCTION_BEGIN();
    return m_private->setSwipeType(swipeType);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::connectOnMouseSwipe
 *
 * connects callback to function for on mouse swipe
 *
 * \param mouseSwipeCallback function of mouse swipe callback
 */
void TgMouseCapture::connectOnMouseSwipe(std::function<void(TgMouseType, SwipeType, double)> mouseSwipeCallback)
{
    TG_FUNCTION_BEGIN();
    return m_private->connectOnMouseSwipe(mouseSwipeCallback);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::disconnectOnMouseSwipe
 *
 * disconnects callback to function for on mouse swipe
 */
void TgMouseCapture::disconnectOnMouseSwipe()
{
    TG_FUNCTION_BEGIN();
    return m_private->disconnectOnMouseSwipe();
    TG_FUNCTION_END();
}
