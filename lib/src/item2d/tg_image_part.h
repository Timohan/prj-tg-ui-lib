/*!
 * \file
 * \brief file tg_image_part.h
 *
 * if type: TgImagePartType_Part9
 * Image from 9 parts (top-left, top-middle, top-right,
 * middle-left, middle-middle, middle-right,
 * bottom-left, bottom-middle, bottom-right).
 *
 * If type: TgImagePartType_Part3_UpToDown
 * Image from 3 parts (top, middle, bottom).
 *
 * If type: TgImagePartType_Part3_LeftToRight
 * Image from 3 parts (left, middle, right).
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_IMAGE_PART_H
#define TG_IMAGE_PART_H

#include "tg_item2d.h"
#include "../global/tg_global_macros.h"

class TgImagePartPrivate;
struct TgWindowInfo;

/*!
 * \brief TgImagePartType
 * defines how this image is parted
 */
enum TgImagePartType
{
    TgImagePartType_Part9,              /*!< image contains 9 images, using functions setImageCropPosition and setImageAreaSize - this is default */
    TgImagePartType_Part3_UpToDown,     /*!< image contains 9 images, using functions setImageCropPosition3TopToBottom and setImageAreaSize3TopToBottom */
    TgImagePartType_Part3_LeftToRight,  /*!< image contains 9 images, using functions setImageCropPosition3LeftToRight and setImageAreaSize3LeftToRight */
};

/*!
 * \brief TgImagePart
 * image that can drawed in multiple parts
 * if type: TgImagePartType_Part9
 * Image from 9 parts (top-left, top-middle, top-right,
 * middle-left, middle-middle, middle-right,
 * bottom-left, bottom-middle, bottom-right).
 *
 * If type: TgImagePartType_Part3_UpToDown
 * Image from 3 parts (top, middle, bottom).
 *
 * If type: TgImagePartType_Part3_LeftToRight
 * Image from 3 parts (left, middle, right).
 */
class TG_MAINWINDOW_EXPORT TgImagePart : public TgItem2d
{
public:
    explicit TgImagePart(TgItem2d *parent, const char *imageFileName, float imageCropLeft = 0.2f, float imageCropTop = 0.2f, float imageCropRight = 0.2f, float imageCropBottom = 0.2f, float leftAreaSize = 10.0f, float topAreaSize = 10.0f, float rightAreaSize = 10.0f, float bottomAreaSize = 10.0f);
    explicit TgImagePart(TgItem2d *parent, float x, float y, float width, float height, const char *imageFileName, float imageCropLeft = 0.2f, float imageCropTop = 0.2f, float imageCropRight = 0.2f, float imageCropBottom = 0.2f, float leftAreaSize = 10.0f, float topAreaSize = 10.0f, float rightAreaSize = 10.0f, float bottomAreaSize = 10.0f);
    virtual ~TgImagePart();

    void setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom);
    void setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize);

    void setImageCropPosition3LeftToRight(float imageCropLeft, float imageCropRight);
    void setImageAreaSize3LeftToRight(float leftAreaSize, float rightAreaSize);

    void setImageCropPosition3TopToBottom(float imageCropTop, float imageCropBottom);
    void setImageAreaSize3TopToBottom(float topAreaSize, float bottomAreaSize);

    TgImagePartType getType() const;
    void setType(TgImagePartType type);

    bool setImage(const char *imageFileName);

private:
    TgImagePartPrivate *m_private;

    virtual bool render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;

    friend class TgMainWindowTooltip;
};

#endif // TG_IMAGE_PART_H
