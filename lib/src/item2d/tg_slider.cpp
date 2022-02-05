/*!
 * \file
 * \brief file tg_slider.cpp
 *
 * Slider
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_slider.h"
#include "../global/tg_global_log.h"
#include "private/tg_slider_private.h"
#include <iostream>

/*!
 * \brief TgSlider::TgSlider
 * 
 * constructor to use AnchorFollowParentSize (check TgItem2D)
 *
 * \param parent
 * \param type type of slider (horizontal or vertical)
 */
TgSlider::TgSlider(TgItem2d *parent, TgSliderType type) :
    TgItem2d(parent),
    m_private(new TgSliderPrivate(parent, this, type))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::TgSlider
 * 
 * constructor to use AnchorRelativeToParent (check TgItem2D)
 * position is relative to parent
 *
 * \param parent
 * \param x text field position x (relative to parent)
 * \param y text field position y (relative to parent)
 * \param width text field position width
 * \param height text field position height
 * \param type type of slider (horizontal or vertical)
 */
TgSlider::TgSlider(TgItem2d *parent, float x, float y, float width, float height, TgSliderType type) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgSliderPrivate(parent, this, type))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::~TgSlider
 * 
 */
TgSlider::~TgSlider()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::render
 *
 * Renders the slider
 * \param windowInfo
 */
void TgSlider::render(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param windowInfo
 */
void TgSlider::checkPositionValues(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
TgEventResult TgSlider::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (eventData->m_type == TgEventType::EventTypeMousePress
        && eventData->m_event.m_mouseEvent.m_mouseType == TgMouseType::ButtonLeft
        && getXminOnVisible() <= eventData->m_event.m_mouseEvent.m_x
        && getXmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_x
        && getYminOnVisible() <= eventData->m_event.m_mouseEvent.m_y
        && getYmaxOnVisible(windowInfo) >= eventData->m_event.m_mouseEvent.m_y) {
        if (getEnabled()) {
            TgSliderPressPosition r = m_private->getPressPosition(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo);
            if (r != TgSliderPressPosition::PressPositionNA) {
                eventData->m_event.m_mouseEvent.m_newItem = this;
                m_private->setCurrentPressPosition(r,
                    eventData->m_event.m_mouseEvent.m_x,
                    eventData->m_event.m_mouseEvent.m_y);
            }
        }
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseRelease
        && eventData->m_event.m_mouseEvent.m_mouseType == TgMouseType::ButtonLeft
        && eventData->m_event.m_mouseEvent.m_currentMouseDownItem == this
        && m_private->getCurrentPressPosition() != TgSliderPressPosition::PressPositionNA) {
        if (getEnabled()) {
            TgSliderPressPosition r = m_private->getPressPosition(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo);
            if (m_private->getCurrentPressPosition() == r) {
                switch (r) {
                    case TgSliderPressPosition::PressPositionMinus:
                        if (getSliderCurrentPosition()) {
                            setSliderCurrentPosition(getSliderCurrentPosition()-1);
                        }
                        break;
                    case TgSliderPressPosition::PressPositionPlus:
                        if (getSliderCurrentPosition() != getSliderMaxPosition()) {
                            setSliderCurrentPosition(getSliderCurrentPosition()+1);
                        }
                        break;
                    case TgSliderPressPosition::PressPositionNA:
                    case TgSliderPressPosition::PressPositionOnButton:
                    default:
                        break;
                }
            }
            m_private->setCurrentPressPosition(TgSliderPressPosition::PressPositionNA,
                                               eventData->m_event.m_mouseEvent.m_x,
                                               eventData->m_event.m_mouseEvent.m_y);
        }
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeMouseMove
        && (!eventData->m_event.m_mouseEvent.m_currentMouseDownItem
            || eventData->m_event.m_mouseEvent.m_currentMouseDownItem == this)) {
        if (getEnabled()
            && m_private->getCurrentPressPosition() == TgSliderPressPosition::PressPositionOnButton) {
            m_private->setSliderCurrentPosition(eventData->m_event.m_mouseEvent.m_x,
                                                eventData->m_event.m_mouseEvent.m_y);
        }
    }
    TG_FUNCTION_END();
    return TgItem2d::handleEvent(eventData, windowInfo);
}

/*!
 * \brief TgSliderPressPosition::getCurrentPressPosition
 *
 * \return get current slider position
 */
uint64_t TgSlider::getSliderCurrentPosition() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getSliderCurrentPosition();
}

/*!
 * \brief TgSlider::getSliderMaxPosition
 *
 * \return get max slider position
 */
uint64_t TgSlider::getSliderMaxPosition() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getSliderMaxPosition();
}

/*!
 * \brief TgSlider::setSliderCurrentPosition
 *
 * \param sliderCurrentPosition slider's new current position
 * \return true if slider current position was changed
 * false if slider current position was not changed, for example if
 * sliderCurrentPosition was same as before, or sliderCurrentPosition
 * is larger than slider's max position
 */
bool TgSlider::setSliderCurrentPosition(uint64_t sliderCurrentPosition)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->setSliderCurrentPosition(sliderCurrentPosition);
}

/*!
 * \brief TgSlider::setSliderMaxPosition
 *
 * \param sliderMaxPosition slider's new max position
 * if current position is larger than new max position,
 * current position will be set to max position
 */
void TgSlider::setSliderMaxPosition(uint64_t sliderMaxPosition)
{
    TG_FUNCTION_BEGIN();
    m_private->setSliderMaxPosition(sliderMaxPosition);
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::connectOnSliderPositionChanged
 *
 * connects callback to function for on slider position changed
 *
 * \param slidePositionChanged callback function
 */
void TgSlider::connectOnSliderPositionChanged(std::function<void(uint64_t)> slidePositionChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnSliderPositionChanged(slidePositionChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::disconnectOnSliderPositionChanged
 *
 * disconnects callback to function for on slider position changed
 */
void TgSlider::disconnectOnSliderPositionChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnSliderPositionChanged();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::connectOnSliderMaxPositionChanged
 *
 * connects callback to function for on slider max position changed
 *
 * \param sliderMaxPositionChanged callback function
 */
void TgSlider::connectOnSliderMaxPositionChanged(std::function<void(uint64_t)> sliderMaxPositionChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnSliderMaxPositionChanged(sliderMaxPositionChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::disconnectOnSliderMaxPositionChanged
 *
 * disconnects callback to function for on slider max position changed
 */
void TgSlider::disconnectOnSliderMaxPositionChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnSliderMaxPositionChanged();
    TG_FUNCTION_END();
}