/*!
 * \file
 * \brief file tg_item2d_position.h
 *
 * it holds positional values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_POSITION_H
#define TG_ITEM_2D_POSITION_H

class TgItem2dPrivate;
class TgGridViewCellPrivate;
#include <functional>
#include "../../tg_item2d.h"

struct TgAnchorMargingValues // using when AnchorFollowParentSize
{
    float m_left;
    float m_top;
    float m_right;
    float m_bottom;
};

enum RenderVisibility
{
    RenderVisibility_NotCalculated = 0,
    RenderVisibility_Visible,
    RenderVisibility_Invisible
};

class TgItem2dPositionInternalResize
{
public:
    virtual void onInternalResize(float x, float y, float width, float height) = 0;
};

class TgItem2dPosition
{
public:
    explicit TgItem2dPosition(TgItem2d *parent, TgItem2dPrivate *currentItemPrivate);
    explicit TgItem2dPosition(float x, float y, float width, float height, TgItem2d *parent, TgItem2dPrivate *currentItemPrivate);

    float getX();
    float getY();
    float getWidth();
    float getHeight();

    void setX(float x);
    void setY(float y);
    bool setWidth(float width, bool useCallback = true);
    bool setHeight(float height, bool useCallback = true);

    void setMarginLeft(float left);
    void setMarginTop(float top);
    void setMarginRight(float right);
    void setMarginBottom(float bottom);

    float getMarginLeft();
    float getMarginTop();
    float getMarginRight();
    float getMarginBottom();

    void connectOnResizeChanged(std::function<void(float x, float y, float width, float height)> resizeChanged);
    void disconnectOnResizeChanged();

    TgItem2dAnchor getAnchorState() const;
    void setAnchorState(TgItem2dAnchor state);

    void checkOnResizeChanged();
    void setPositionChanged(bool positionChanged);
    bool getPositionChanged();

    float getXonWindow();
    float getYonWindow();

    void setAddMinMaxHeightOnVisible(float addMinHeightOnVisible, float addMaxHeightOnVisible);

    float getXminOnVisible();
    float getXmaxOnVisible(const TgWindowInfo *windowInfo);
    float getYminOnVisible();
    float getYmaxOnVisible(const TgWindowInfo *windowInfo);

    bool getUseRoundedPositionValues();
    void setUseRoundedPositionValues(bool useRoundedPositionValues);

    void setCurrentGridViewCell(TgGridViewCellPrivate *currentGridViewCell);

    bool isRenderVisible(const TgWindowInfo *windowInfo);
    void setInternalResize(TgItem2dPositionInternalResize *internalResize);

private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    float m_addMinHeightOnVisible;
    float m_addMaxHeightOnVisible;
    RenderVisibility m_renderVisibility = RenderVisibility_NotCalculated;
    TgItem2dPositionInternalResize *m_internalResize = nullptr;

    TgItem2d *m_parent;
    TgItem2dPrivate *m_currentItemPrivate;
    TgGridViewCellPrivate *m_currentGridViewCell = nullptr;

    TgItem2dAnchor m_anchor;
    TgAnchorMargingValues m_marginValues; // AnchorFollowParentSize
    TgAnchorMargingValues m_previousPosition; // used for f_resizeChanged
    bool m_positionChanged;
    bool m_useRoundedPositionValues;          // if true, then all x/y/width/height are rounded

    std::function<void(float x, float y, float width, float height)> f_resizeChanged;
};

#endif // TG_ITEM_2D_POSITION_H