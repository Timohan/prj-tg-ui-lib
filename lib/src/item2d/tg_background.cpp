/*!
 * \file
 * \brief file tg_background.cpp
 *
 * Inits and draws background that can be TgRectangle
 * or TgImage
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_background.h"
#include "../global/tg_global_log.h"
#include "private/tg_background_private.h"

/*!
 * \brief TgBackground::TgBackground
 *
 * constructor to use AnchorFollowParentSize
 * and using rectangle as background
 *
 * \param parent item's parent
 * \param r red color for background
 * \param g green color for background
 * \param b blue color for background
 * \param a alpha color for background
 */
TgBackground::TgBackground(TgItem2d *parent, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgItem2d(parent),
    m_private(new TgBackgroundPrivate(this, r, g, b, a))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackground::TgBackground
 *
 * constructor to use AnchorRelativeToParent
 * and using rectangle as background
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param r red color for background
 * \param g green color for background
 * \param b blue color for background
 * \param a alpha color for background
 */
TgBackground::TgBackground(TgItem2d *parent, float x, float y, float width, float height,
                           const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgBackgroundPrivate(this, r, g, b, a))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackground::TgBackground
 *
 * constructor to use AnchorFollowParentSize
 * and using image as background
 *
 * \param parent item's parent
 * \param imageFilename full file name of image
 */
TgBackground::TgBackground(TgItem2d *parent, const char *imageFilename) :
    TgItem2d(parent),
    m_private(new TgBackgroundPrivate(this, imageFilename))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackground::TgBackground
 *
 * constructor to use AnchorRelativeToParent
 * and using image as background
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param imageFilename full file name of image
 */
TgBackground::TgBackground(TgItem2d *parent, float x, float y, float width, float height, const char *imageFilename) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgBackgroundPrivate(this, imageFilename))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgBackground::~TgBackground()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
    }
    TG_FUNCTION_END();
}

void TgBackground::render(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

void TgBackground::checkPositionValues(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackground::setColor
 *
 * set background to rectangle with single color
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgBackground::setColor(const unsigned char r, const unsigned char g , const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_private->setColor(r, g, b, a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackground::setImage
 *
 * set background to image filename
 *
 * \param imageFilename full file path of filename
 */
void TgBackground::setImage(const char *imageFilename)
{
    TG_FUNCTION_BEGIN();
    m_private->setImage(imageFilename);
    TG_FUNCTION_END();
}

