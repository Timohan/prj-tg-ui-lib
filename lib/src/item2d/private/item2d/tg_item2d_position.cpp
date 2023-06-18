/*!
 * \file
 * \brief file tg_item2d_position.cpp
 *
 * it holds general TgItem2dPosition class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_position.h"
#include <cmath>
#include <limits>
#include "../../../global/tg_global_log.h"
#include "tg_item2d_private.h"
#include "../../../window/tg_mainwindow_private.h"
#include "../../../global/private/tg_global_wait_renderer.h"
#include "../grid_view/tg_grid_view_cell_private.h"

TgItem2dPosition::TgItem2dPosition(TgItem2d *parent, TgItem2dPrivate *currentItemPrivate) :
    m_x(0),
    m_y(0),
    m_width(0),
    m_height(0),
    m_addMinHeightOnVisible(0),
    m_addMaxHeightOnVisible(0),
    m_parent(parent),
    m_currentItemPrivate(currentItemPrivate),
    m_anchor(TgItem2dAnchor::AnchorFollowParentSize),
    m_marginValues({0,0,0,0}),
    m_previousPosition({0,0,0,0}),
    m_positionChanged(true),
    m_useRoundedPositionValues(true),
    f_resizeChanged(nullptr)
{
    if (parent) {
        m_width = parent->getWidth();
        m_height = parent->getHeight();
    }
}

TgItem2dPosition::TgItem2dPosition(float x, float y, float width, float height, TgItem2d *parent, TgItem2dPrivate *currentItemPrivate) :
    m_x(x),
    m_y(y),
    m_width(width),
    m_height(height),
    m_addMinHeightOnVisible(0),
    m_addMaxHeightOnVisible(0),
    m_parent(parent),
    m_currentItemPrivate(currentItemPrivate),
    m_anchor(TgItem2dAnchor::AnchorRelativeToParent),
    m_marginValues({0,0,0,0}),
    m_previousPosition({0,0,0,0}),
    m_positionChanged(true),
    m_useRoundedPositionValues(true),
    f_resizeChanged(nullptr)
{    
}

void TgItem2dPosition::setInternalResize(TgItem2dPositionInternalResize *internalResize)
{
    m_internalResize = internalResize;
}

/*!
 * \brief TgItem2dPosition::getX
 *
 * get relative position X on parent
 * if you need to know real X position on window,
 * use getXonWindow()
 * \return relative position X
 */
float TgItem2dPosition::getX()
{
    TG_FUNCTION_BEGIN();
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize
        || m_anchor == TgItem2dAnchor::AnchorRelativeToParentXFollowParentSize) {
        TG_FUNCTION_END();
        return m_marginValues.m_left;
    }
    TG_FUNCTION_END();
    return m_x;
}

/*!
 * \brief TgItem2dPosition::getY
 *
 * get relative position Y on parent
 * if you need to know real Y position on window,
 * use getYonWindow()
 * \return relative position Y
 */
float TgItem2dPosition::getY()
{
    TG_FUNCTION_BEGIN();
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize) {
        TG_FUNCTION_END();
        return m_marginValues.m_top;
    }
    TG_FUNCTION_END();
    return m_y;
}

/*!
 * \brief TgItem2dPosition::getWidth
 *
 * get real width
 *
 * \return relative width
 */
float TgItem2dPosition::getWidth()
{
    TG_FUNCTION_BEGIN();
    if (m_anchor == TgItem2dAnchor::AnchorRelativeToParentXyFollowParentSize) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(m_parent->getWidth());
        }
        TG_FUNCTION_END();
        return m_parent->getWidth();
    }
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize
        || m_anchor == TgItem2dAnchor::AnchorRelativeToParentXFollowParentSize) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(m_parent->getWidth() - m_marginValues.m_left - m_marginValues.m_right);
        }
        TG_FUNCTION_END();
        return m_parent->getWidth() - m_marginValues.m_left - m_marginValues.m_right;
    }
    if (m_width <= -1 && m_parent) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(m_parent->getWidth());
        }
        TG_FUNCTION_END();
        return m_parent->getWidth();
    }
    if (m_useRoundedPositionValues) {
        TG_FUNCTION_END();
        return std::roundf(m_width);
    }
    TG_FUNCTION_END();
    return m_width;
}

/*!
 * \brief TgItem2dPosition::getHeight
 *
 * get real height
 */
float TgItem2dPosition::getHeight()
{
    TG_FUNCTION_BEGIN();
    if (m_anchor == TgItem2dAnchor::AnchorRelativeToParentXyFollowParentSize) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(m_parent->getHeight());
        }
        TG_FUNCTION_END();
        return m_parent->getHeight();
    }
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(m_parent->getHeight() - m_marginValues.m_top - m_marginValues.m_bottom);
        }
        TG_FUNCTION_END();
        return m_parent->getHeight() - m_marginValues.m_top - m_marginValues.m_bottom;
    }
    if (m_height < 0 && m_parent) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(m_parent->getHeight());
        }
        TG_FUNCTION_END();
        return m_parent->getHeight();
    }
    if (m_useRoundedPositionValues) {
        TG_FUNCTION_END();
        return std::roundf(m_height);
    }
    TG_FUNCTION_END();
    return m_height;
}

/*!
 * \brief TgItem2dPosition::setWidth
 *
 * set width for item
 * \param width
 * \return true if width is changed
 */
bool TgItem2dPosition::setWidth(float width, bool useCallback)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_width - width) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return false;
    }
    m_width = width;
    setPositionChanged(true);
    if (useCallback && m_currentGridViewCell) {
        m_currentGridViewCell->widthChanged(width);
    }
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgItem2dPosition::setHeight
 *
 * set height for item
 * \param height
 * \return true if height is changed
 */
bool TgItem2dPosition::setHeight(float height, bool useCallback)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_height - height) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return false;
    }
    m_height = height;
    setPositionChanged(true);
    if (useCallback && m_currentGridViewCell) {
        m_currentGridViewCell->heightChanged(height);
    }
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgItem2dPosition::setX
 *
 * set x position
 * \param x
 */
void TgItem2dPosition::setX(float x)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_x - x) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_x = x;
    setPositionChanged(true);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::setY
 *
 * set y position
 * \param y
 */
void TgItem2dPosition::setY(float y)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(m_y - y) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_y = y;
    setPositionChanged(true);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::setMarginLeft
 *
 * set margin left
 * \param left
 */
void TgItem2dPosition::setMarginLeft(float left)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(left - m_marginValues.m_left) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_marginValues.m_left = left;
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize
        || m_anchor == TgItem2dAnchor::AnchorRelativeToParentXFollowParentSize) {
        setPositionChanged(true);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::setMarginTop
 *
 * set margin top
 * \param top
 */
void TgItem2dPosition::setMarginTop(float top)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(top - m_marginValues.m_top) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_marginValues.m_top = top;
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize) {
        setPositionChanged(true);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::setMarginRight
 *
 * set margin right
 * \param right
 */
void TgItem2dPosition::setMarginRight(float right)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(right - m_marginValues.m_right) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_marginValues.m_right = right;
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize
        || m_anchor == TgItem2dAnchor::AnchorRelativeToParentXFollowParentSize) {
        setPositionChanged(true);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::setMarginBottom
 *
 * set margin bottom
 * \param bottom
 */
void TgItem2dPosition::setMarginBottom(float bottom)
{
    TG_FUNCTION_BEGIN();
    if (std::fabs(bottom - m_marginValues.m_bottom) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_marginValues.m_bottom = bottom;
    if (m_anchor == TgItem2dAnchor::AnchorFollowParentSize) {
        setPositionChanged(true);
    }
    TG_FUNCTION_END();
}

float TgItem2dPosition::getMarginLeft()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_marginValues.m_left;
}

float TgItem2dPosition::getMarginTop()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_marginValues.m_top;
}

float TgItem2dPosition::getMarginRight()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_marginValues.m_right;
}

float TgItem2dPosition::getMarginBottom()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_marginValues.m_bottom;
}


/*!
 * \brief TgItem2dPosition::getAnchorState
 *
 * \return item's anchor state
 */
TgItem2dAnchor TgItem2dPosition::getAnchorState() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_anchor;
}

/*!
 * \brief TgItem2dPosition::setAnchorState
 *
 * \param state new item's anchor state
 */
void TgItem2dPosition::setAnchorState(TgItem2dAnchor state)
{
    TG_FUNCTION_BEGIN();
    if (m_anchor == state) {
        TG_FUNCTION_END();
        return;
    }
    m_anchor = state;
    setPositionChanged(true);
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::connectOnResizeChanged
 *
 * set connect resize changed callback
 * \param resizeChanged callback of resize changed
 */
void TgItem2dPosition::connectOnResizeChanged(std::function<void(float x, float y, float width, float height)> resizeChanged)
{
    TG_FUNCTION_BEGIN();
    if (resizeChanged) {
        f_resizeChanged = resizeChanged;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::disconnectOnResizeChanged
 *
 * disconnect resize changed callback
 */
void TgItem2dPosition::disconnectOnResizeChanged()
{
    TG_FUNCTION_BEGIN();
    f_resizeChanged = nullptr;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::checkOnResizeChanged
 *
 * checks if position or size is changed, and sends
 * possible resizeChanged callback
 */
void TgItem2dPosition::checkOnResizeChanged()
{
    TG_FUNCTION_BEGIN();
    float x = getXonWindow();
    float y = getYonWindow();
    float w = getWidth();
    float h = getHeight();
    if ((f_resizeChanged || m_internalResize)
        && (std::fabs(x - m_previousPosition.m_left) > std::numeric_limits<double>::epsilon()
            || std::fabs(y - m_previousPosition.m_top) > std::numeric_limits<double>::epsilon()
            || std::fabs(w - m_previousPosition.m_right) > std::numeric_limits<double>::epsilon()
            || std::fabs(h - m_previousPosition.m_bottom) > std::numeric_limits<double>::epsilon())) {
        if (m_internalResize) {
            m_internalResize->onInternalResize(getX(), getY(), w, h);
        }
        if (f_resizeChanged) {
            f_resizeChanged(getX(), getY(), w, h);
        }
    }
    m_previousPosition.m_left = x;
    m_previousPosition.m_top = y;
    m_previousPosition.m_right = w;
    m_previousPosition.m_bottom = h;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::getPositionChanged
 *
 * \return get position change and transform (or vertices) re-do is required
 */
bool TgItem2dPosition::getPositionChanged()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_positionChanged;
}

/*!
 * \brief TgItem2dPosition::setPositionChanged
 *
 * \param positionChanged true - set position change and transform (or vertices) re-do is required
 */
void TgItem2dPosition::setPositionChanged(bool positionChanged)
{
    TG_FUNCTION_BEGIN();
    m_positionChanged = positionChanged;
    if (positionChanged) {
        m_renderVisibility = RenderVisibility_NotCalculated;
        TgItem2dPrivateMessage msg;
        msg.m_type = TgItem2dPrivateMessageType::PositionChanged;
        m_currentItemPrivate->sendMessageToChildren(&msg);
    }
    TgGlobalWaitRenderer::getInstance()->release();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dPosition::getXminOnVisible
 *
 * get min X position on visible area
 *
 * \return min x position
 */
float TgItem2dPosition::getXminOnVisible()
{
    TG_FUNCTION_BEGIN();
    float minX = 0;
    if (m_parent) {
        minX = m_parent->getXminOnVisible();
    }
    if (getXonWindow() > minX) {
        minX = getXonWindow();
    }
    TG_FUNCTION_END();
    return minX;
}

/*!
 * \brief TgItem2dPosition::getXmaxOnVisible
 *
 * get max X position on visible area
 *
 * \param windowInfo
 * \return max x position
 */
float TgItem2dPosition::getXmaxOnVisible(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    float maxX = static_cast<float>(windowInfo->m_windowWidth);
    if (m_parent) {
        maxX = m_parent->getXmaxOnVisible(windowInfo);
    }
    if (getXonWindow() + getWidth() < maxX) {
        maxX = getXonWindow() + getWidth();
    }
    TG_FUNCTION_END();
    return maxX;
}

/*!
 * \brief TgItem2dPosition::getXonWindow
 *
 * get real position X (on window)
 */
float TgItem2dPosition::getXonWindow()
{
    TG_FUNCTION_BEGIN();
    if (!m_parent) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(getX());
        }
        TG_FUNCTION_END();
        return getX();
    }
    if (m_useRoundedPositionValues) {
        TG_FUNCTION_END();
        return std::roundf(getX() + m_parent->getXonWindow());
    }
    TG_FUNCTION_END();
    return getX() + m_parent->getXonWindow();
}

/*!
 * \brief TgItem2dPosition::getYonWindow
 *
 * get real position Y (on window)
 */
float TgItem2dPosition::getYonWindow()
{
    TG_FUNCTION_BEGIN();
    if (!m_parent) {
        if (m_useRoundedPositionValues) {
            TG_FUNCTION_END();
            return std::roundf(getY());
        }
        TG_FUNCTION_END();
        return getY();
    }
    if (m_useRoundedPositionValues) {
        TG_FUNCTION_END();
        return std::roundf(getY() + m_parent->getYonWindow());
    }
    TG_FUNCTION_END();
    return getY() + m_parent->getYonWindow();
}

/*!
 * \brief TgItem2dPosition::setAddMinMaxHeightOnVisible
 *
 * set addon values to calculate min/max height for visible
 * area, this is used with font drawing, so bottom and top
 * areas will be fully visible for example for character j
 *
 * \param addMinHeightOnVisible
 * \param addMaxHeightOnVisible
 */
void TgItem2dPosition::setAddMinMaxHeightOnVisible(
    float addMinHeightOnVisible,
    float addMaxHeightOnVisible)
{
    m_addMinHeightOnVisible = addMinHeightOnVisible;
    m_addMaxHeightOnVisible = addMaxHeightOnVisible;
}

/*!
 * \brief TgItem2dPosition::getYminOnVisible
 *
 * get min Y position on visible area
 *
 * \return min y position
 */
float TgItem2dPosition::getYminOnVisible()
{
    TG_FUNCTION_BEGIN();
    float minY = 0;
    if (m_parent) {
        minY = m_parent->getYminOnVisible();
    }
    if (getYonWindow() + m_addMinHeightOnVisible > minY) {
        minY = getYonWindow() + m_addMinHeightOnVisible;
    }
    TG_FUNCTION_END();
    return minY;
}

/*!
 * \brief TgItem2dPosition::getYmaxOnVisible
 *
 * get max Y position on visible area
 *
 * \param windowInfo
 * \return max y position
 */
float TgItem2dPosition::getYmaxOnVisible(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    float maxY = static_cast<float>(windowInfo->m_windowHeight);
    if (m_parent) {
        maxY = m_parent->getYmaxOnVisible(windowInfo);
    }
    if (getYonWindow() + getHeight() + m_addMaxHeightOnVisible < maxY) {
        maxY = getYonWindow() + getHeight() + m_addMaxHeightOnVisible;
    }
    TG_FUNCTION_END();
    return maxY;
}

/*!
 * \brief TgItem2dPosition::isRenderVisible
 *
 * checks if render is visible
 *
 * \param windowInfo
 * \return true if render is visible and it can be done
 * false if no rendering is visible, so rendering can be skipped
 */
bool TgItem2dPosition::isRenderVisible(const TgWindowInfo *windowInfo)
{
    if (m_renderVisibility != RenderVisibility_NotCalculated) {
        return m_renderVisibility == RenderVisibility_Visible;
    }
    float xMinOnVisible = getXminOnVisible();
    if (windowInfo->m_windowWidth < static_cast<int>(xMinOnVisible)) {
        m_renderVisibility = RenderVisibility_Invisible;
        return false;
    }
    float yMinOnVisible = getYminOnVisible();
    if (windowInfo->m_windowHeight < static_cast<int>(yMinOnVisible)) {
        m_renderVisibility = RenderVisibility_Invisible;
        return false;
    }

    float xMaxOnVisible = getXmaxOnVisible(windowInfo);
    if (xMaxOnVisible < 0) {
        m_renderVisibility = RenderVisibility_Invisible;
        return false;
    }

    float yMaxOnVisible = getYmaxOnVisible(windowInfo);
    if (yMaxOnVisible < 0) {
        m_renderVisibility = RenderVisibility_Invisible;
        return false;
    }

    m_renderVisibility = RenderVisibility_Visible;
    return true;
}

/*!
 * \brief TgItem2dPosition::getUseRoundedPositionValues
 *
 * if true, this item is using rounded values for positional
 * values (x, y, width, height)
 *
 * if true, and x is set to be 64.2, it will be set to 64
 * if false, and x is set to be 64.2, it will be set to 64.2
 *
 * \return true if this item is using rounded values
 */
bool TgItem2dPosition::getUseRoundedPositionValues()
{
    return m_useRoundedPositionValues;
}

/*!
 * \brief TgItem2dPosition::setUseRoundedPositionValues
 *
 * if true, this item is using rounded values for positional
 * values (x, y, width, height)
 *
 * if true, and x is set to be 64.2, it will be set to 64
 * if false, and x is set to be 64.2, it will be set to 64.2
 *
 * \param useRoundedPositionValues to set if this item is using rounded values
 * or not
 */
void TgItem2dPosition::setUseRoundedPositionValues(bool useRoundedPositionValues)
{
    if (m_useRoundedPositionValues == useRoundedPositionValues) {
        return;
    }
    m_useRoundedPositionValues = useRoundedPositionValues;
    setPositionChanged(true);
    TgItem2dPrivateMessage msg;
    msg.m_type = useRoundedPositionValues
                 ? TgItem2dPrivateMessageType::ParentItemToUseRoundedPositionValues
                 : TgItem2dPrivateMessageType::ParentItemToUseNotRoundedPositionValues;
    m_currentItemPrivate->sendMessageToChildren(&msg, false);
}

/*!
 * \brief TgItem2dPosition::setCurrentGridViewCell
 *
 * \param currentGridViewCell
 */
void TgItem2dPosition::setCurrentGridViewCell(TgGridViewCellPrivate *currentGridViewCell)
{
    m_currentGridViewCell = currentGridViewCell;
}
