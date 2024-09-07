/*!
 * \file
 * \brief file tg_rectangle.cpp
 *
 * Inits and draws rectangle
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_rectangle.h"
#include "../global/tg_global_log.h"
#include "private/tg_rectangle_private.h"
#include "private/item2d/tg_item2d_private.h"

/*!
 * \brief TgRectangle::TgRectangle
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 * \param r color red
 * \param g color green
 * \param b color blue
 * \param a color alpha
 */
TgRectangle::TgRectangle(TgItem2d *parent, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgItem2d(parent),
    m_private(new TgRectanglePrivate(r, g, b, a))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgRectangle::TgRectangle
 *
 * constructor to use AnchorRelativeToParent
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param r color red
 * \param g color green
 * \param b color blue
 * \param a color alpha
 */
TgRectangle::TgRectangle(TgItem2d *parent, float x, float y, float width, float height,
                         const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgRectanglePrivate(r, g, b, a))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgRectangle::~TgRectangle()
{
    if (m_private) {
        delete m_private;
    }
}

/*!
 * \brief TgRectangle::setColor
 *
 * set color for rectangle
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgRectangle::setColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_private->setColor(r, g, b, a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgRectangle::render
 *
 * Renders the rectangle
 * \param windowInfo
 * \param parentOpacity
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgRectangle::render(const TgWindowInfo *windowInfo, float parentOpacity)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return false;
    }
    TG_FUNCTION_END();
    return m_private->render(windowInfo, this, reinterpret_cast<TgItem2d *>(this)->m_private, parentOpacity*getOpacity());
}

/*!
 * \brief TgRectangle::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgRectangle::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}
