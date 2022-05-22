/*!
 * \file
 * \brief file tg_textedit_private.h
 *
 * it holds general TgTexteditPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_TEXTEDIT_PRIVATE_H
#define TG_TEXTEDIT_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../math/tg_matrix4x4.h"
#include "../../font/tg_character_positions.h"
#include <string>
#include <mutex>
#include "../tg_textedit.h"
#include "item2d/tg_item2d_private.h"
#include "../tg_image_part.h"
#include "../../common/time_difference.h"
#include "../tg_rectangle.h"

struct TgFontText;
class TgItem2d;
struct TgWindowInfo;

class TgTexteditPrivate : private TgItem2dInternalCallback
{
public:
    explicit TgTexteditPrivate(TgItem2d *currentItem, const char *text, const char *fontFile, float fontSize, uint8_t r, uint8_t g, uint8_t b);
    virtual ~TgTexteditPrivate();
    void checkPositionValues(TgItem2d *currentItem);
    void setHorizontalAlign(TgTextfieldHorizontalAlign align);
    void setVerticalAlign(TgTextfieldVerticalAlign align);
    TgTextfieldHorizontalAlign getAlignHorizontal() const;
    TgTextfieldVerticalAlign getAlignVertical() const;
    void setText(const char *text);
    void setText(const std::vector<TgTextFieldText> &listText);
    void setFontSize(float fontSize);

    void changeImageFile();
    void changeDownProperty(bool down);
    TgEventResult handleEvent(TgEventData *eventData);

    void onMousePressed(float x);
    void onMouseReleased(float x);
    void onMouseMove(float x);

    float getTextLeftMargin();
    float getTextRightMargin();
    void setTextLeftMargin(float margin);
    void setTextRightMargin(float margin);
    size_t getCursorPosition();
    size_t getCharacterCount();
    uint32_t getCharacterByIndex(size_t index);
    int64_t getSelectedTextSize();

private:
    float m_marginLeft;
    float m_marginRight;
    bool m_cursorPressedDown;
    int64_t m_cursorPosition;
    int64_t m_selectedTextSize;
    uint8_t m_selectedAreaBackgroundColorSelected[3];
    uint8_t m_selectedAreaBackgroundColorNotSelected[3];
    TgItem2d *m_currentItem;
    std::string m_imageFileNames[ TgTexteditState::TexteditStateCount ];
    std::recursive_mutex m_mutex;
    TgImagePart m_backgroundImage;
    TgRectangle m_selectedAreaBackground;
    TgTextfield m_textField;
    TgImagePart m_cursorPositionImage;
    TimeDifference m_cursorPositionTimer;

    std::string getImageFile();
    void clearText();
    void resetTextFieldPosition(const float &positionX);

    virtual void onSelectedCallback() override;
};

#endif // TG_TEXTEDIT_PRIVATE_H
