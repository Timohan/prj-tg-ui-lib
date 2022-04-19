/*!
 * \file
 * \brief file tg_button_private.h
 *
 * it holds general TgButtonPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_BUTTON_PRIVATE_H
#define TG_BUTTON_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../math/tg_matrix4x4.h"
#include "../tg_textfield.h"
#include "../tg_image.h"
#include "../tg_button.h"
#include "item2d/tg_item2d_private.h"
#include <string>
#include <mutex>

class TgItem2d;
struct TgWindowInfo;

class TgButtonPrivate : private TgItem2dInternalCallback
{
public:
    explicit TgButtonPrivate(TgItem2d *currentItem, const char *buttonText);
    virtual ~TgButtonPrivate();
    void render(const TgWindowInfo *windowInfo, TgItem2d *currentItem);
    void checkPositionValues(TgItem2d *currentItem);

    void setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom);
    void setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize);

    void changeButtonImageFile();
    void changeDownProperty(bool down);
    void setImage(TgButtonState state, const char *filename);

    bool getButtonDown() const;
    void setFontSize(float fontSize);
    void setText(const char *text);

private:
    TgItem2d *m_currentItem;
    std::string m_imageFileNames[TgButtonState::ButtonStateCount];
    bool m_buttonDown;
    std::recursive_mutex m_mutex;

    TgImage m_imageTopLeft;
    TgImage m_imageTopMiddle;
    TgImage m_imageTopRight;

    TgImage m_imageMiddleLeft;
    TgImage m_imageMiddleMiddle;
    TgImage m_imageMiddleRight;

    TgImage m_imageBottomLeft;
    TgImage m_imageBottomMiddle;
    TgImage m_imageBottomRight;

    TgTextfield m_textfield;
    float m_imageCropLeft, m_imageCropTop, m_imageCropRight, m_imageCropBottom;
    float m_leftAreaSize, m_topAreaSize, m_rightAreaSize, m_bottomAreaSize;

    std::string getImageFileName();
    void setImagePositions();
    virtual void onSelectedCallback() override;
};

#endif // TG_BUTTON_PRIVATE_H
