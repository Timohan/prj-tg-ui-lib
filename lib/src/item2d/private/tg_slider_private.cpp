/*!
 * \file
 * \brief file tg_mouse_slider_private.cpp
 *
 * Slider private functions
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_slider_private.h"
#include "../../global/tg_global_log.h"
#include "../../global/private/tg_global_defines.h"
#include "../../global/private/tg_global_wait_renderer.h"

TgSliderPrivate::TgSliderPrivate(TgItem2d *parent, TgItem2d *currentItem, TgSliderType type) :
    m_parent(parent),
    m_currentItem(currentItem),
    m_type(type),
    m_background(currentItem,
                 type == TgSliderType::SliderType_Horizontal
                    ? std::string(std::string(IMAGES_PATH) + std::string("/slider/prj-tg-ui-lib-slider-left-right.png")).c_str()
                    : std::string(std::string(IMAGES_PATH) + std::string("/slider/prj-tg-ui-lib-slider-up_down.png")).c_str()),
    m_sliderButton(currentItem, 0, 0, DEFAULT_KNOB_WIDE, DEFAULT_KNOB_WIDE,
                   type == TgSliderType::SliderType_Horizontal
                      ? std::string(std::string(IMAGES_PATH) + std::string("/slider/prj-tg-ui-lib-slider-left-right-button.png")).c_str()
                      : std::string(std::string(IMAGES_PATH) + std::string("/slider/prj-tg-ui-lib-slider-up_down-button.png")).c_str()),
    m_pressPosition(TgSliderPressPosition::PressPositionNA),
    m_sliderCurrentPosition(0),
    m_sliderMaxPosition(100),
    m_pressPositionX(0),
    m_pressPositionY(0),
    m_startButtonPositionX(0),
    m_startButtonPositionY(0),
    f_sliderPositionChanged(nullptr),
    f_sliderMaxPositionChanged(nullptr)
{
    TG_FUNCTION_BEGIN();
    setSliderButtonPositioning();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSliderPrivate::getPressPosition
 *
 * get press position, if the position is on the minus area, plus area or button area
 * \param x cursor position x
 * \param y cursor position y
 * \param windowInfo window info
 * \return press position - minus area, plus area or button area (or not)
 */
TgSliderPressPosition TgSliderPrivate::getPressPosition(double x, double y, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (m_sliderButton.getXminOnVisible() <= x
        && m_sliderButton.getXmaxOnVisible(windowInfo) >= x
        && m_sliderButton.getYminOnVisible() <= y
        && m_sliderButton.getYmaxOnVisible(windowInfo) >= y) {
        TG_FUNCTION_END();
        return TgSliderPressPosition::PressPositionOnButton;
    }

    if (m_type == TgSliderType::SliderType_Horizontal) {
        if (x <= m_sliderButton.getXminOnVisible()) {
            if (m_currentItem->getXminOnVisible() <= x
                && m_currentItem->getXmaxOnVisible(windowInfo) >= x
                && m_currentItem->getYminOnVisible() <= y
                && m_currentItem->getYmaxOnVisible(windowInfo) >= y) {
                TG_FUNCTION_END();
                return TgSliderPressPosition::PressPositionMinus;
            }
        } else {
            if (m_currentItem->getXminOnVisible() <= x
                && m_currentItem->getXmaxOnVisible(windowInfo) >= x
                && m_currentItem->getYminOnVisible() <= y
                && m_currentItem->getYmaxOnVisible(windowInfo) >= y) {
                TG_FUNCTION_END();
                return TgSliderPressPosition::PressPositionPlus;
            }
        }
    } else {
        if (y <= m_sliderButton.getYminOnVisible()) {
            if (m_currentItem->getXminOnVisible() <= x
                && m_currentItem->getXmaxOnVisible(windowInfo) >= x
                && m_currentItem->getYminOnVisible() <= y
                && m_currentItem->getYmaxOnVisible(windowInfo) >= y) {
                TG_FUNCTION_END();
                return TgSliderPressPosition::PressPositionMinus;
            }
        } else {
            if (m_currentItem->getXminOnVisible() <= x
                && m_currentItem->getXmaxOnVisible(windowInfo) >= x
                && m_currentItem->getYminOnVisible() <= y
                && m_currentItem->getYmaxOnVisible(windowInfo) >= y) {
                TG_FUNCTION_END();
                return TgSliderPressPosition::PressPositionPlus;
            }
        }
    }

    TG_FUNCTION_END();
    return TgSliderPressPosition::PressPositionNA;
}

/*!
 * \brief TgSliderPrivate::setCurrentPressPosition
 *
 * \param pressPosition current press position
 * \param pressX current press x position
 * \param pressY current press y position
 */
void TgSliderPrivate::setCurrentPressPosition(TgSliderPressPosition pressPosition, double pressX, double pressY)
{
    TG_FUNCTION_BEGIN();
    m_pressPosition = pressPosition;
    m_pressPositionX = pressX;
    m_pressPositionY = pressY;
    m_startButtonPositionX = m_sliderButton.getX();
    m_startButtonPositionY = m_sliderButton.getY();
    TG_FUNCTION_END();
}

/*!
 * \brief TgSliderPrivate::getCurrentPressPosition
 *
 * \return get current press position
 */
TgSliderPressPosition TgSliderPrivate::getCurrentPressPosition()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_pressPosition;
}

/*!
 * \brief TgSliderPrivate::getCurrentPressPosition
 *
 * \return get current slider position
 */
uint64_t TgSliderPrivate::getSliderCurrentPosition() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_sliderCurrentPosition;
}

/*!
 * \brief TgSliderPrivate::getSliderMaxPosition
 *
 * \return get max slider position
 */
uint64_t TgSliderPrivate::getSliderMaxPosition() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_sliderMaxPosition;
}

/*!
 * \brief TgSliderPrivate::setSliderCurrentPosition
 *
 * This function calculates new slider position from previous
 * cursor press position and current cursor position
 *
 * \param cursorX
 * \param cursorY
 */
void TgSliderPrivate::setSliderCurrentPosition(double cursorX, double cursorY)
{
    TG_FUNCTION_BEGIN();
    if (m_type == TgSliderType::SliderType_Horizontal) {
        double newButtonPosition = cursorX - (m_pressPositionX-m_startButtonPositionX);
        if (newButtonPosition <= 0) {
            setSliderCurrentPosition(0);
        } else {
            uint64_t newPos = static_cast<uint64_t>(static_cast<double>(m_sliderMaxPosition)*static_cast<double>(newButtonPosition)/static_cast<double>(m_background.getWidth() - m_sliderButton.getWidth()));
            if (newPos > m_sliderMaxPosition) {
                newPos = m_sliderMaxPosition;
            }
            setSliderCurrentPosition(newPos);
        }
    } else {
        double newButtonPosition = cursorY - (m_pressPositionY-m_startButtonPositionY);
        if (newButtonPosition <= 0) {
            setSliderCurrentPosition(0);
        } else {
            uint64_t newPos = static_cast<uint64_t>(static_cast<double>(m_sliderMaxPosition)*static_cast<double>(newButtonPosition)/static_cast<double>(m_background.getHeight() - m_sliderButton.getHeight()));
            if (newPos > m_sliderMaxPosition) {
                newPos = m_sliderMaxPosition;
            }
            setSliderCurrentPosition(newPos);
        }
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgSliderPrivate::setSliderCurrentPosition
 *
 * \param sliderCurrentPosition slider's new current position
 * \return true if slider current position was changed
 * false if slider current position was not changed, for example if
 * sliderCurrentPosition was same as before, or sliderCurrentPosition
 * is larger than slider's max position
 */
bool TgSliderPrivate::setSliderCurrentPosition(uint64_t sliderCurrentPosition)
{
    TG_FUNCTION_BEGIN();
    if (m_sliderCurrentPosition == sliderCurrentPosition
        || sliderCurrentPosition > m_sliderMaxPosition) {
        return false;
    }
    m_sliderCurrentPosition = sliderCurrentPosition;
    setSliderButtonPositioning();
    if (f_sliderPositionChanged) {
        f_sliderPositionChanged(sliderCurrentPosition);
    }
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgSliderPrivate::setSliderMaxPosition
 *
 * \param sliderMaxPosition slider's new max position
 * if current position is larger than new max position,
 * current position will be set to max position
 */
void TgSliderPrivate::setSliderMaxPosition(uint64_t sliderMaxPosition)
{
    TG_FUNCTION_BEGIN();
    if (m_sliderMaxPosition == sliderMaxPosition) {
        TG_FUNCTION_END();
        return;
    }
    m_sliderMaxPosition = sliderMaxPosition;
    if (f_sliderMaxPositionChanged) {
        f_sliderMaxPositionChanged(sliderMaxPosition);
    }
    if (m_sliderCurrentPosition > m_sliderMaxPosition) {
        setSliderCurrentPosition(m_sliderMaxPosition);
    } else {
        setSliderButtonPositioning();
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgSliderPrivate::calculateSliderKnobWidth
 *
 * calculates knob's width horizontal (or height on vertical)
 * \param backgroundWidth background width on horizontal, height on vertical
 * \return knob's width or height
 */
float TgSliderPrivate::calculateSliderKnobWidth(float backgroundWidth)
{
    if (m_sliderMaxPosition == 0) {
        return backgroundWidth;
    }

    float ret = backgroundWidth/static_cast<float>(m_sliderMaxPosition+1);
    if (ret <= DEFAULT_KNOB_WIDE) {
        return DEFAULT_KNOB_WIDE;
    }
    return ret;
}

/*!
 * \brief TgSliderPrivate::setSliderButtonPositioning
 *
 * set slider's new button positioning
 */
void TgSliderPrivate::setSliderButtonPositioning()
{
    TG_FUNCTION_BEGIN();
    if (m_type == TgSliderType::SliderType_Horizontal) {
        m_background.setType(TgImagePartType::TgImagePartType_Part3_LeftToRight);
        m_background.setImageCropPosition3LeftToRight(0.25f, 0.25f);
        m_background.setImageAreaSize3LeftToRight(10, 10);
        m_sliderButton.setType(TgImagePartType::TgImagePartType_Part3_LeftToRight);
        if (m_sliderMaxPosition == 0) {
            m_sliderButton.setImageCropPosition3LeftToRight(0.25f, 0.25f);
            m_sliderButton.setImageAreaSize3LeftToRight(10, 10);
            m_sliderButton.setWidth(m_background.getWidth());
            m_sliderButton.setHeight(m_background.getHeight());
            TG_FUNCTION_END();
            return;
        }


        float w = calculateSliderKnobWidth(m_background.getWidth());
        float x = (m_background.getWidth()-w)*(static_cast<float>(m_sliderCurrentPosition)/static_cast<float>(m_sliderMaxPosition));
        m_sliderButton.setImageCropPosition3LeftToRight(0.25f, 0.25f);
        m_sliderButton.setImageAreaSize3LeftToRight(10, 10);
        m_sliderButton.setX(x);
        m_sliderButton.setWidth(w);
        m_sliderButton.setHeight(m_currentItem->getHeight());
    } else {
        m_background.setType(TgImagePartType::TgImagePartType_Part3_UpToDown);
        m_background.setImageCropPosition3TopToBottom(0.25f, 0.25f);
        m_background.setImageAreaSize3TopToBottom(10, 10);
        if (m_sliderMaxPosition == 0) {
            m_sliderButton.setImageCropPosition3TopToBottom(0.25f, 0.25f);
            m_sliderButton.setImageAreaSize3TopToBottom(10, 10);
            m_sliderButton.setWidth(m_background.getWidth());
            m_sliderButton.setHeight(m_background.getHeight());
            TG_FUNCTION_END();
            return;
        }
        float h = calculateSliderKnobWidth(m_background.getHeight());
        float y = (m_background.getHeight()-h)*(static_cast<float>(m_sliderCurrentPosition)/static_cast<float>(m_sliderMaxPosition));
        m_sliderButton.setType(TgImagePartType::TgImagePartType_Part3_UpToDown);
        m_sliderButton.setImageCropPosition3TopToBottom(0.25f, 0.25f);
        m_sliderButton.setImageAreaSize3TopToBottom(10, 10);
        m_sliderButton.setY(y);
        m_sliderButton.setHeight(h);
        m_sliderButton.setWidth(m_currentItem->getWidth());
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::connectOnSliderPositionChanged
 *
 * connects callback to function for on slider position changed
 *
 * \param slidePositionChanged callback function
 */
void TgSliderPrivate::connectOnSliderPositionChanged(std::function<void(uint64_t)> slidePositionChanged)
{
    TG_FUNCTION_BEGIN();
    f_sliderPositionChanged = slidePositionChanged;
    TG_FUNCTION_END();
}

/*!
 * \brief TgSlider::disconnectOnSliderPositionChanged
 *
 * disconnects callback to function for on slider position changed
 */
void TgSliderPrivate::disconnectOnSliderPositionChanged()
{
    TG_FUNCTION_BEGIN();
    f_sliderPositionChanged = nullptr;
    TG_FUNCTION_END();
}

/*!
 * \brief TgSliderPrivate::connectOnSliderMaxPositionChanged
 *
 * connects callback to function for on slider max position changed
 *
 * \param sliderMaxPositionChanged callback function
 */
void TgSliderPrivate::connectOnSliderMaxPositionChanged(std::function<void(uint64_t)> sliderMaxPositionChanged)
{
    TG_FUNCTION_BEGIN();
    f_sliderMaxPositionChanged = sliderMaxPositionChanged;
    TG_FUNCTION_END();
}

/*!
 * \brief TgSliderPrivate::disconnectOnSliderMaxPositionChanged
 *
 * disconnects callback to function for on slider max position changed
 */
void TgSliderPrivate::disconnectOnSliderMaxPositionChanged()
{
    TG_FUNCTION_BEGIN();
    f_sliderMaxPositionChanged = nullptr;
    TG_FUNCTION_END();
}