/*!
 * \file
 * \brief file tg_image.cpp
 *
 * Inits and draws image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_image.h"
#include "../global/tg_global_log.h"
#include "private/tg_image_private.h"

/*!
 * \brief TgImage::TgImage
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 * \param filename full filepath of the filename
 */
TgImage::TgImage(TgItem2d *parent, const char *filename) :
    TgItem2d(parent),
    m_private(new TgImagePrivate(filename))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgImage::TgImage
 *
 * constructor to use AnchorRelativeToParent
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param filename full filepath of the filename
 */
TgImage::TgImage(TgItem2d *parent, float x, float y, float width, float height, const char *filename) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgImagePrivate(filename))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}
TgImage::~TgImage()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
        m_private = nullptr;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgImage::setTextureST
 *
 * set texture ST values
 *
 * All values must be <= 1.0f (or at least should be)
 *
 * \param topLeftS topleft S
 * \param topLeftT topleft T
 * \param topRightS topRight S
 * \param topRightT topRight T
 * \param bottomRightS bottomRight S
 * \param bottomRightT bottomRight T
 * \param bottomLeftS bottomLeft S
 * \param bottomLeftT bottomLeft T
 */
void TgImage::setTextureST(float topLeftS, float topLeftT,
                           float topRightS, float topRightT,
                           float bottomRightS, float bottomRightT,
                           float bottomLeftS, float bottomLeftT)
{
    TG_FUNCTION_BEGIN();
    m_private->setTextureST(topLeftS, topLeftT,
                            topRightS, topRightT,
                            bottomRightS, bottomRightT,
                            bottomLeftS, bottomLeftT);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImage::setImage
 *
 * set image's filename
 *
 * \param filename
 */
void TgImage::setImage(const char *filename)
{
    TG_FUNCTION_BEGIN();
    m_private->setImage(filename);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImage::render
 *
 * Renders the image
 * \param windowInfo
 */
void TgImage::render(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->render(windowInfo, this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgImage::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgImage::checkPositionValues(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}
