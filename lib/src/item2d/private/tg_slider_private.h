/*!
 * \file
 * \brief file tg_mouse_slider_private.h
 *
 * Slider private functions
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_SLIDER_PRIVATE_H
#define TG_SLIDER_PRIVATE_H

class TgItem2d;
#include "../tg_slider.h"
#include "../tg_image.h"
#include "../tg_image_part.h"
#include "../tg_mouse_capture.h"

enum TgSliderPressPosition
{
    PressPositionNA = 0,
    PressPositionMinus, /* Press position */
    PressPositionOnButton,
    PressPositionPlus
};

class TgSliderPrivate
{
public:
    explicit TgSliderPrivate(TgItem2d *parent, TgItem2d *currentItem, TgSliderType type);

    TgSliderPressPosition getPressPosition(double x, double y, const TgWindowInfo *windowInfo);

    void setCurrentPressPosition(TgSliderPressPosition pressPosition, double pressX, double pressY);
    TgSliderPressPosition getCurrentPressPosition();

    uint64_t getSliderCurrentPosition() const;
    uint64_t getSliderMaxPosition() const;
    void setSliderCurrentPosition(double cursorX, double cursorY);
    bool setSliderCurrentPosition(uint64_t sliderCurrentPosition);
    void setSliderMaxPosition(uint64_t sliderMaxPosition);
    void setSliderButtonPositioning();

    void connectOnSliderPositionChanged(std::function<void(uint64_t)> slidePositionChanged);
    void disconnectOnSliderPositionChanged();
    void connectOnSliderMaxPositionChanged(std::function<void(uint64_t)> sliderMaxPositionChanged);
    void disconnectOnSliderMaxPositionChanged();
private:
    TgItem2d *m_parent;
    TgItem2d *m_currentItem;
    TgSliderType m_type;

    TgImagePart m_background;
    TgImagePart m_sliderButton;
    TgSliderPressPosition m_pressPosition;
    uint64_t m_sliderCurrentPosition;
    uint64_t m_sliderMaxPosition;
    double m_pressPositionX;
    double m_pressPositionY;
    double m_startButtonPositionX;
    double m_startButtonPositionY;

    std::function<void(uint64_t)> f_sliderPositionChanged;
    std::function<void(uint64_t)> f_sliderMaxPositionChanged;
};

#endif // TG_SLIDER_PRIVATE_H
