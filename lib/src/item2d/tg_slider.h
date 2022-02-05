/*!
 * \file
 * \brief file tg_slider.h
 *
 * Slider
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_SLIDER_H
#define TG_SLIDER_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

class TgSliderPrivate;
struct TgWindowInfo;

/*!
 * \brief TgSliderType
 * slider type for horizontal or vertical
 */
enum TgSliderType {
    SliderType_Horizontal = 0,      /*!< slider to horizontal */
    SliderType_Vertical             /*!< slider to vertical */
};

/*!
 * \brief TgSlider
 * handles slider functionality
 */
class TG_MAINWINDOW_EXPORT TgSlider : public TgItem2d
{
public:
    explicit TgSlider(TgItem2d *parent, TgSliderType type);
    explicit TgSlider(TgItem2d *parent, float x, float y, float width, float height, TgSliderType type);
    virtual ~TgSlider();


    uint64_t getSliderCurrentPosition() const;
    uint64_t getSliderMaxPosition() const;
    bool setSliderCurrentPosition(uint64_t sliderCurrentPosition);
    void setSliderMaxPosition(uint64_t sliderMaxPosition);

    void connectOnSliderPositionChanged(std::function<void(uint64_t position)> slidePositionChanged);
    void disconnectOnSliderPositionChanged();
    void connectOnSliderMaxPositionChanged(std::function<void(uint64_t maxPosition)> sliderMaxPositionChanged);
    void disconnectOnSliderMaxPositionChanged();
protected:
    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo) override;

    virtual void render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues(const TgWindowInfo *windowInfo) override;

private:
    TgSliderPrivate *m_private;
};

#endif // TG_SLIDER_H
