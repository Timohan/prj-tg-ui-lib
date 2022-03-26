/*!
 * \file
 * \brief file tg_button.h
 *
 * Inits and draws button
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_BUTTON_H
#define TG_BUTTON_H

#include "tg_mouse_capture.h"
#include "../global/tg_global_macros.h"

class TgButtonPrivate;
struct TgWindowInfo;

/*!
 * \brief TgButtonState
 * button state
 */
enum TgButtonState
{
    ButtonStateNormal = 0,          /*!< normal state */
    ButtonStateNormalHover,         /*!< mouse cursor is on button */
    ButtonStateNormalSelected,      /*!< button is selected */
    ButtonStateNormalSelectedHover, /*!< button is selected & mouse cursor is hovering */
    ButtonStateDown,                /*!< button is pressed down */
    ButtonStateDownHover,           /*!< button is pressed down & mouse cursor is on button */
    ButtonStateDownSelected,        /*!< button is pressed down & button is selected */
    ButtonStateDownSelectedHover,   /*!< button is pressed down & button is selected & mouse cursor is hovering */
    ButtonStateDisabled,            /*!< button is disabled */
    ButtonStateCount,               /*!< button state count (not used) */
};

/*!
 * \brief TgButton
 * normal button class
 */
class TG_MAINWINDOW_EXPORT TgButton : public TgMouseCapture
{
public:
    explicit TgButton(TgItem2d *parent, const char *buttonText);
    explicit TgButton(TgItem2d *parent, float x, float y, float width, float height, const char *buttonText);
    virtual ~TgButton();

    void setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom);
    void setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize);
    void setImage(TgButtonState state, const char *filename);
    bool getButtonDown() const;
    void setFontSize(float fontSize);

protected:
    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo) override;
    virtual void render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;
    virtual void onHoverChanged(bool hover) override;
    virtual void onDownChanged(bool down) override;
    virtual void onEnabledChanged(bool enabled) override;

private:
    TgButtonPrivate *m_private;
};

#endif // TG_BUTTON_H
