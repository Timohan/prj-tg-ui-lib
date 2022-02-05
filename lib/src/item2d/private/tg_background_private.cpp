/*!
 * \file
 * \brief file tg_background_private.cpp
 *
 * Inits and draws background that can be TgRectangle
 * or TgImage (private)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_background_private.h"
#include "../../global/tg_global_log.h"
#include "../tg_rectangle.h"
#include "../tg_image.h"

TgBackgroundPrivate::TgBackgroundPrivate(TgItem2d *currentItem, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    m_currentItem(currentItem),
    m_rectangle(new TgRectangle(currentItem, r, g, b, a)),
    m_image(nullptr)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgBackgroundPrivate::TgBackgroundPrivate(TgItem2d *currentItem, const char *imageFilename) :
    m_currentItem(currentItem),
    m_rectangle(nullptr),
    m_image(new TgImage(currentItem, imageFilename))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}


TgBackgroundPrivate::~TgBackgroundPrivate()
{
    TG_FUNCTION_BEGIN();
    if (m_image) {
        delete m_image;
    }
    if (m_rectangle) {
        delete m_rectangle;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackgroundPrivate::setColor
 *
 * set background to rectangle with single color
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgBackgroundPrivate::setColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    if (m_image) {
        m_image->setVisible(false);
    }
    if (!m_rectangle) {
        m_rectangle = new TgRectangle(m_currentItem, r, g, b, a);
        TG_FUNCTION_END();
        return;
    }
    m_rectangle->setColor(r, g, b, a);
    m_rectangle->setVisible(a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgBackgroundPrivate::setImage
 *
 * set background to image filename
 *
 * \param imageFilename full file path of filename
 */
void TgBackgroundPrivate::setImage(const char *imageFilename)
{
    TG_FUNCTION_BEGIN();
    if (m_rectangle) {
        m_rectangle->setVisible(false);
    }
    if (!m_image) {
        m_image = new TgImage(m_currentItem, imageFilename);
        TG_FUNCTION_END();
        return;
    }
    m_image->setImage(imageFilename);
    m_image->setVisible(true);
    TG_FUNCTION_END();
}

