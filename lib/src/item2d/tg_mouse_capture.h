/*!
 * \file
 * \brief file tg_mouse_capture.h
 *
 * Mouse capture area
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MOUSE_CAPTURE_H
#define TG_MOUSE_CAPTURE_H

#include <functional>
#include "tg_item2d.h"
#include "../global/tg_global_macros.h"
#include "../event/tg_event_data.h"

struct TgWindowInfo;
class TgMouseCapturePrivate;

/*!
 * \brief SwipeType
 * allowed swipe direction for callback
 * swipe direction can be set with setSwipeType()
 * and it can use combination as SwipeTypeUp | SwipeTypeDown
 * to allow only up and down direction swipes on callback
 */
enum SwipeType
{
    SwipeTypeNothing    = 0x00, /*!< callback allows no swipe direction */
    SwipeTypeUp         = 0x01, /*!< callback allows swipe to up direction */
    SwipeTypeDown       = 0x02, /*!< callback allows swipe to down  direction */
    SwipeTypeLeft       = 0x04, /*!< callback allows swipe to left  direction */
    SwipeTypeRight      = 0x08, /*!< callback allows swipe to right  direction */
    SwipeTypeAll        = 0x0F, /*!< callback allows all swipe  directions (up, down, left, right)  */
};

/*!
 * \brief TgMouseCapture
 * mouse capture area, can capture clicks, hover, swipe, button press/released
 */
class TG_MAINWINDOW_EXPORT TgMouseCapture : public TgItem2d
{
public:
    explicit TgMouseCapture(TgItem2d *parent);
    explicit TgMouseCapture(TgItem2d *parent, float x, float y, float width, float height);
    ~TgMouseCapture();

    void connectOnMousePressed(std::function<void(TgMouseType button, float x, float y)> mousePressed);
    void disconnectOnMousePressed();
    void connectOnMouseReleased(std::function<void(TgMouseType button, bool inArea, float x, float y)> mouseReleased);
    void disconnectOnMouseReleased();
    void connectOnMouseClicked(std::function<void(TgMouseType button, float x, float y)> mouseClicked);
    void disconnectOnMouseClicked();

    void connectOnMouseMove(std::function<void(float x, float y)> mouseMove);
    void disconnectOnMouseMove();

    void connectOnMouseScrollMove(std::function<void(double x, double y)> mouseScrollMove);
    void disconnectOnMouseScrollMove();

    bool getMouseCursorOnHover();

    uint8_t getSwipeType();
    void setSwipeType(uint8_t swipeType);

    void connectOnMouseSwipe(std::function<void(TgMouseType mouseType, SwipeType swipeType, double speed)> mouseSwipeCallback);
    void disconnectOnMouseSwipe();
protected:
    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo) override;
    virtual void onHoverChanged(bool hover);
    virtual void onDownChanged(bool down);
    virtual void onEnabledChanged(bool enabled) override;

private:
    TgMouseCapturePrivate *m_private;

    friend class TgButton;
};

#endif // TG_MOUSE_CAPTURE_H