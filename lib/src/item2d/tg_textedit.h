/*!
 * \file
 * \brief file tg_textedit.h
 *
 * Inits and draws text
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_TEXTEDIT_H
#define TG_TEXTEDIT_H

#include "tg_textfield.h"
#include "tg_mouse_capture.h"

class TgTexteditPrivate;
struct TgWindowInfo;
class TgItem2d;

/*!
 * \brief TgTextEditState
 * text edit state
 */
enum TgTexteditState
{
    TexteditStateNormal = 0,          /*!< normal state */
    TexteditStateNormalHover,         /*!< mouse cursor is on text edit */
    TexteditStateSelected,            /*!< text edit is selected */
    TexteditStateDisabled,            /*!< text edit is disabled */
    TexteditStateCount                /*!< text edit state count (not used) */
};

/*!
 * \brief TgTextedit
 * allows text field to be edited
 */
class TG_MAINWINDOW_EXPORT TgTextedit : public TgMouseCapture
{
public:
    explicit TgTextedit(TgItem2d *parent, const char *text, const char *fontFile, float fontSize, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    explicit TgTextedit(TgItem2d *parent, float x, float y, float width, float height, const char *text, const char *fontFile, float fontSize, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    virtual ~TgTextedit();
    void setHorizontalAlign(TgTextfieldHorizontalAlign align);
    void setVerticalAlign(TgTextfieldVerticalAlign align);
    TgTextfieldHorizontalAlign getAlignHorizontal() const;
    TgTextfieldVerticalAlign getAlignVertical() const;
    void setText(const char *text);
    void setText(const std::vector<TgTextFieldText> &listText);
    void setFontSize(float fontSize);
    float getTextLeftMargin();
    float getTextRightMargin();
    void setTextLeftMargin(float margin);
    void setTextRightMargin(float margin);
    size_t getCursorPosition();
    size_t getCharacterCount();
    uint32_t getCharacterByIndex(size_t index);
    int64_t getSelectedTextSize();
protected:
    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo);
    virtual void render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;
    virtual void onHoverChanged(bool);
    virtual void onDownChanged(bool down) override;
    virtual void onEnabledChanged(bool enabled) override;
    virtual void onMousePressed(TgMouseType button, float x, float y) override;
    virtual void onMouseReleased(TgMouseType button, bool inArea, float x, float y) override;
    virtual void onMouseMove(bool inArea, float x, float y) override;

private:
    TgTexteditPrivate *m_private;
};

#endif // TG_TEXTEDIT_H
