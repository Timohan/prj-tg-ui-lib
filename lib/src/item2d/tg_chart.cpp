/*!
 * \file
 * \brief file tg_chart.cpp
 *
 * Inits and draws chart
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_chart.h"
#include "../global/tg_global_log.h"
#include "private/tg_chart_private.h"
#include "private/item2d/tg_item2d_private.h"

/*!
 * \brief TgChart::TgChart
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 * \param r color red
 * \param g color green
 * \param b color blue
 * \param a color alpha
 */
TgChart::TgChart(TgItem2d *parent, const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgItem2d(parent),
    m_private(new TgChartPrivate(this, r, g, b, a))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::TgChart
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
TgChart::TgChart(TgItem2d *parent, float x, float y, float width, float height,
                 const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
    TgItem2d(parent, x, y, width, height),
    m_private(new TgChartPrivate(this, r, g, b, a))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgChart::~TgChart()
{
    if (m_private) {
        delete m_private;
    }
}

/*!
 * \brief TgChart::setBackgroundColor
 *
 * set background color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChart::setBackgroundColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_private->setBackgroundColor(r, g, b, a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setGridLinesColor
 *
 * set gridline color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChart::setGridLinesColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_private->setGridLinesColor(r, g, b, a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setLineColor
 *
 * set line color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChart::setLineColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_private->setLineColor(r, g, b, a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setTextColor
 *
 * set text color for chart
 *
 * \param r red
 * \param g green
 * \param b blue
 * \param a alpha
 */
void TgChart::setTextColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    TG_FUNCTION_BEGIN();
    m_private->setTextColor(r, g, b, a);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::render
 *
 * Renders the chart
 * \param windowInfo
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgChart::render(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return false;
    }
    TG_FUNCTION_END();
    return m_private->render(windowInfo, this, reinterpret_cast<TgItem2d *>(this)->m_private);
}

/*!
 * \brief TgChart::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgChart::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::addChartPosition
 *
 * add new chart position
 * \param position
 */
void TgChart::addChartPosition(const TgChartPosition position)
{
    TG_FUNCTION_BEGIN();
    m_private->addChartPosition(position);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setXyDrawIsMaxedToSize
 *
 * X/Y draw is maxed to size
 * = true, x/y positions are maxed
 * = false, x/y positions is linear
 *
 * \param xyDrawIsMaxedToSize
 */
void TgChart::setXyDrawIsMaxedToSize(bool xyDrawIsMaxedToSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setXyDrawIsMaxedToSize(xyDrawIsMaxedToSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::getXyDrawIsMaxedToSize
 *
 * get current if X/Y draw is maxed to size
 * = true, x/y positions are maxed
 * = false, x/y positions is linear
 *
 * \return xy draw is maxed to fize
 */
bool TgChart::getXyDrawIsMaxedToSize()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getXyDrawIsMaxedToSize();
}

/*!
 * \brief TgChart::setFontSize
 *
 * \param fontSize fon size for text
 */
void TgChart::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::getFontSize
 *
 * \return font size
 */
float TgChart::getFontSize()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getFontSize();
}

/*!
 * \brief TgChart::clear
 *
 * clear all chart positions
 */
void TgChart::clear()
{
    TG_FUNCTION_BEGIN();
    m_private->clear();
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setChartPosition
 *
 * set new chart list position
 * \param listPosition
 */
void TgChart::setChartPosition(const std::vector<TgChartPosition> &listPosition)
{
    TG_FUNCTION_BEGIN();
    m_private->setChartPosition(listPosition);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::getBottomGridMargin
 * \return bottom grid margin
 */
uint32_t TgChart::getBottomGridMargin() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getBottomGridMargin();
}

/*!
 * \brief TgChart::getTopGridMargin
 * \return top grid margin
 */
uint32_t TgChart::getTopGridMargin() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getTopGridMargin();
}

/*!
 * \brief TgChart::getLeftGridMargin
 * \return left grid margin
 */
uint32_t TgChart::getLeftGridMargin() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getLeftGridMargin();
}

/*!
 * \brief TgChart::getRightGridMargin
 * \return right grid margin
 */
uint32_t TgChart::getRightGridMargin() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getRightGridMargin();
}

/*!
 * \brief TgChart::setBottomGridMargin
 * \param gridBottomMargin bottom grid margin
 */
void TgChart::setBottomGridMargin(uint32_t gridBottomMargin)
{
    TG_FUNCTION_BEGIN();
    m_private->setBottomGridMargin(gridBottomMargin);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setTopGridMargin
 * \param gridTopMargin top grid margin
 */
void TgChart::setTopGridMargin(uint32_t gridTopMargin)
{
    TG_FUNCTION_BEGIN();
    m_private->setTopGridMargin(gridTopMargin);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setLeftGridMargin
 * \param gridLeftMargin left grid margin
 */
void TgChart::setLeftGridMargin(uint32_t gridLeftMargin)
{
    TG_FUNCTION_BEGIN();
    m_private->setLeftGridMargin(gridLeftMargin);
    TG_FUNCTION_END();
}

/*!
 * \brief TgChart::setRightGridMargin
 * \param gridRightMargin right grid margin
 */
void TgChart::setRightGridMargin(uint32_t gridRightMargin)
{
    TG_FUNCTION_BEGIN();
    m_private->setRightGridMargin(gridRightMargin);
    TG_FUNCTION_END();
}