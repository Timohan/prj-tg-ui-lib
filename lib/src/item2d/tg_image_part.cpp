/*!
 * \file
 * \brief file tg_image_part.cpp
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
#include "tg_image_part.h"
#include "../global/tg_global_log.h"
#include "private/tg_image_part_private.h"

/*!
 * \brief TgImagePart::TgImagePart
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 * \param imageFileName image's filename
 * \param imageCropLeft image's crop position left (default 0.2f)
 * \param imageCropTop image's crop position top (default 0.2f)
 * \param imageCropRight image's crop position right (default 0.2f)
 * \param imageCropBottom image's crop position bottom (default 0.2f)
 * \param leftAreaSize left area size in pixels
 * \param topAreaSize top area size in pixels
 * \param rightAreaSize left area size in pixels
 * \param bottomAreaSize bottom area size in pixels
 */
TgImagePart::TgImagePart(TgItem2d *parent, const char *imageFileName, float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom, float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize) :
    TgItem2d(parent),
    m_private(new TgImagePartPrivate(this, imageFileName, TgImagePartType::TgImagePartType_Part9))
{
    TG_FUNCTION_BEGIN();
    setImageCropPosition3LeftToRight(0.2f, 0.2f);
    setImageAreaSize3LeftToRight(10.0f, 10.0f);
    setImageCropPosition3TopToBottom(0.2f, 0.2f);
    setImageAreaSize3TopToBottom(10.0f, 10.0f);
    setImageCropPosition(imageCropLeft, imageCropTop, imageCropRight, imageCropBottom);
    setImageAreaSize(leftAreaSize, topAreaSize, rightAreaSize, bottomAreaSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::TgImagePart
 *
 * constructor to use AnchorRelativeToParent
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param imageFileName image's filename
 * \param imageCropLeft image's crop position left (default 0.2f)
 * \param imageCropTop image's crop position top (default 0.2f)
 * \param imageCropRight image's crop position right (default 0.2f)
 * \param imageCropBottom image's crop position bottom (default 0.2f)
 * \param leftAreaSize left area size in pixels
 * \param topAreaSize top area size in pixels
 * \param rightAreaSize left area size in pixels
 * \param bottomAreaSize bottom area size in pixels
 */
TgImagePart::TgImagePart(TgItem2d *parent, float x, float y, float width, float height, const char *imageFileName,
                         float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom,
                         float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgImagePartPrivate(this, imageFileName, TgImagePartType::TgImagePartType_Part9))
{
    TG_FUNCTION_BEGIN();
    setImageCropPosition3LeftToRight(0.2f, 0.2f);
    setImageAreaSize3LeftToRight(10.0f, 10.0f);
    setImageCropPosition3TopToBottom(0.2f, 0.2f);
    setImageAreaSize3TopToBottom(10.0f, 10.0f);
    setImageCropPosition(imageCropLeft, imageCropTop, imageCropRight, imageCropBottom);
    setImageAreaSize(leftAreaSize, topAreaSize, rightAreaSize, bottomAreaSize);
    TG_FUNCTION_END();
}

TgImagePart::~TgImagePart()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
        m_private = nullptr;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::setImageCropPosition
 *
 * set image's crop position, all values must be under 1
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropLeft = 0.2 and imageCropTop = 0.2, imageCropRight = 0.2, imageCropBottom = 0.2
 * then setImageAreaSize's leftAreaSize = 10, topAreaSize = 10, rightAreaSize = 10, bottomAreaSize = 10
 * -> top-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * -> left area on UI is 10x180 pixels, and from original image it's 72.4x219.2 pixels (60% of 362)
 * -> bottom-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * center and right size of the area goes similar way.
 *
 * \param imageCropLeft percent the image's crop left area (must be under 1)
 * \param imageCropTop percent the image's crop top area (must be under 1)
 * \param imageCropRight percent the image's crop right area (must be under 1)
 * \param imageCropBottom percent the image's crop bottom area (must be under 1)
 */
void TgImagePart::setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageCropPosition(imageCropLeft, imageCropTop, imageCropRight, imageCropBottom);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::setImageAreaSize
 *
 * set image's crop positions, all values are pixels
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * setImageCropPosition's imageCropLeft = 0.2 and imageCropTop = 0.2, imageCropRight = 0.2, imageCropBottom = 0.2
 * then leftAreaSize = 10, topAreaSize = 10, rightAreaSize = 10, bottomAreaSize = 10
 * -> top-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * -> left area on UI is 10x180 pixels, and from original image it's 72.4x219.2 pixels (60% of 362)
 * -> bottom-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * center and right size of the area goes similar way.
 *
 * \param leftAreaSize left area's pixel size
 * \param topAreaSize top area's pixel size
 * \param rightAreaSize right area's pixel size
 * \param bottomAreaSize bottom area's pixel size
 */
void TgImagePart::setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageAreaSize(leftAreaSize, topAreaSize, rightAreaSize, bottomAreaSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::setImageCropPosition3LeftToRight
 *
 * set image's crop position, all values must be under 1
 *
 * These values are only used when image type is TgImagePartType_Part3_LeftToRight
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropLeft = 0.2 imageCropRight = 0.2,
 * then setImageAreaSize's leftAreaSize = 10, rightAreaSize = 10
 *
 * -> left area on UI is 10x200 pixels, and from original image it's 72.4x362 pixels (20% of 362)
 * -> middle area on UI is 80x200 pixels, and from original image it's 219.2x362 pixels (60% of 362)
 * -> right area on UI is 10x200 pixels, and from original image it's 72.4x362 pixels (20% of 362)
 *
 * \param imageCropLeft percent the image's crop left area (must be under 1)
 * \param imageCropRight percent the image's crop right area (must be under 1)
 */
void TgImagePart::setImageCropPosition3LeftToRight(float imageCropLeft, float imageCropRight)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageCropPosition3LeftToRight(imageCropLeft, imageCropRight);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::setImageAreaSize3LeftToRight
 *
 * set image's crop positions, all values are pixels
 *
 * These values are only used when image type is TgImagePartType_Part3_LeftToRight
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropLeft = 0.2 imageCropRight = 0.2,
 * then setImageAreaSize's leftAreaSize = 10, rightAreaSize = 10
 *
 * -> left area on UI is 10x200 pixels, and from original image it's 72.4x362 pixels (20% of 362)
 * -> middle area on UI is 80x200 pixels, and from original image it's 219.2x362 pixels (60% of 362)
 * -> right area on UI is 10x200 pixels, and from original image it's 72.4x362 pixels (20% of 362)
 *
 * \param leftAreaSize left area's pixel size
 * \param rightAreaSize right area's pixel size
 */
void TgImagePart::setImageAreaSize3LeftToRight(float leftAreaSize, float rightAreaSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageAreaSize3LeftToRight(leftAreaSize, rightAreaSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::setImageCropPosition3TopToBottom
 *
 * set image's crop positions, all values are pixels
 *
 * These values are only used when image type is TgImagePartType_Part3_UpToDown
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropTop = 0.2 imageCropBottom = 0.2,
 * then setImageAreaSize's topAreaSize = 10, bottomAreaSize = 10
 *
 * -> left area on UI is 200x10 pixels, and from original image it's 362x72.4 pixels (20% of 362)
 * -> middle area on UI is 200x80 pixels, and from original image it's 362x219.2 pixels (60% of 362)
 * -> right area on UI is 200x10 pixels, and from original image it's 362x72.4 pixels (20% of 362)
 *
 * \param imageCropTop percent the image's crop top area (must be under 1)
 * \param imageCropBottom percent the image's crop bottom area (must be under 1)
 */
void TgImagePart::setImageCropPosition3TopToBottom(float imageCropTop, float imageCropBottom)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageCropPosition3TopToBottom(imageCropTop, imageCropBottom);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::setImageAreaSize3TopToBottom
 *
 * set image's crop positions, all values are pixels
 *
 * These values are only used when image type is TgImagePartType_Part3_UpToDown
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropTop = 0.2 imageCropBottom = 0.2,
 * then setImageAreaSize's topAreaSize = 10, bottomAreaSize = 10
 *
 * -> left area on UI is 200x10 pixels, and from original image it's 362x72.4 pixels (20% of 362)
 * -> middle area on UI is 200x80 pixels, and from original image it's 362x219.2 pixels (60% of 362)
 * -> right area on UI is 200x10 pixels, and from original image it's 362x72.4 pixels (20% of 362)
 *
 * \param topAreaSize left area's pixel size
 * \param bottomAreaSize right area's pixel size
 */
void TgImagePart::setImageAreaSize3TopToBottom(float topAreaSize, float bottomAreaSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageAreaSize3TopToBottom(topAreaSize, bottomAreaSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::render
 *
 * Renders the image
 * \param windowInfo
 */
void TgImagePart::render(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->render(windowInfo, this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param
 */
void TgImagePart::checkPositionValues(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImagePart::getType
 *
 * Get image part's type
 * \return type
 */
TgImagePartType TgImagePart::getType() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getType();
}

/*!
 * \brief TgImagePart::setType
 *
 * Set image part's type
 * \param type
 */
void TgImagePart::setType(TgImagePartType type)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    m_private->setType(type);
}


/*!
 * \brief TgImagePart::setImage
 *
 * set image's filename
 *
 * \param imageFileName full filepath of image file
 */
void TgImagePart::setImage(const char *imageFileName)
{
    TG_FUNCTION_BEGIN();
    m_private->setImage(imageFileName);
    TG_FUNCTION_END();
}

