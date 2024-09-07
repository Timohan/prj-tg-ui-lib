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
#include "private/item2d/tg_item2d_private.h"

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
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgImage::render(const TgWindowInfo *windowInfo, float parentOpacity)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return false;
    }
    TG_FUNCTION_END();
    return m_private->render(windowInfo, this, reinterpret_cast<TgItem2d *>(this)->m_private, parentOpacity*getOpacity());
}

/*!
 * \brief TgImage::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgImage::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}

/**
 * @brief TgImage::setPixel
 *
 * set pixel x/y RGBA
 *
 * @param x
 * @param y
 * @param r red color
 * @param g green color
 * @param b blue color
 * @param a alpha color
 * @return true if pixel was possible to set
 * @return false
 */
bool TgImage::setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->setPixel(x, y, r, g, b, a);
}

/**
 * @brief TgImage::getImageWidth
 *
 * @return uint32_t image pixel width
 */
uint32_t TgImage::getImageWidth()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getImageWidth();
}

/**
 * @brief TgImage::getImageHeight
 *
 * @return uint32_t image pixel height
 */
uint32_t TgImage::getImageHeight()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getImageHeight();
}