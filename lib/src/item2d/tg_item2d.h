/*!
 * \file
 * \brief file tg_item2d.h
 *
 * it holds general TgItem2d class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_H
#define TG_ITEM_2D_H

#include <vector>
#include <functional>
#include "../event/tg_event_data.h"
class TgItem2dPrivate;
struct TgWindowInfo;
struct TgItem2dPrivateMessage;

/*!
 * \brief TgItem2dAnchor
 * anchor type of item
 */
enum TgItem2dAnchor {
    AnchorRelativeToParent = 0, /*!< item have it's own size but it's inside the parent item with relative position (using x/y/width/height) */
    AnchorFollowParentSize,     /*!< item follows the parent size and ignores x/y/width/height, however, size can be
                                 * modified with margin functions, for example setMargin() */
};

/*!
 * \brief TgItem2d
 * single item (widget) that all other items use to make their positioning
 */
class TgItem2d
{
public:
    explicit TgItem2d(TgItem2d *parent);
    explicit TgItem2d(TgItem2d *parent, float x, float y, float width, float height);
    virtual ~TgItem2d();

    bool getVisible();
    void setVisible(bool visible);

    float getX();
    float getY();
    float getXonWindow();
    float getYonWindow();
    float getWidth();
    float getHeight();
    float getXminOnVisible();
    float getXmaxOnVisible(const TgWindowInfo *windowInfo);
    float getYminOnVisible();
    float getYmaxOnVisible(const TgWindowInfo *windowInfo);

    void setX(float x);
    void setY(float y);
    void setWidth(float width);
    void setHeight(float height);

    bool getPositionChanged();
    void setPositionChanged(bool positionChanged);

    TgItem2dAnchor getAnchorState() const;
    void setAnchorState(TgItem2dAnchor state);

    void setMargin(float left, float top, float right, float bottom);
    void setMarginLeft(float left);
    void setMarginTop(float top);
    void setMarginRight(float right);
    void setMarginBottom(float bottom);

    void connectOnResizeChanged(std::function<void(float x, float y, float width, float height)> resizeChanged);
    void disconnectOnResizeChanged();

    void connectOnVisibleChanged(std::function<void(bool visible)> visibleChanged);
    void disconnectOnVisibleChanged();

    void connectOnSelectedChanged(std::function<void(bool selected)> selectedChanged);
    void disconnectOnSelectedChanged();

    bool getSelected();
    bool getCanSelect();
    bool getEnabled();
    void setSelected(bool selected);
    void setCanSelect(bool canSelect);
    void setEnabled(bool enabled);

    void setToTop();

    bool getUseRoundedPositionValues();
    void setUseRoundedPositionValues(bool useRoundedPositionValues);

    void setNextTabItem(TgItem2d *nextTabItem);
    void setPrevTabItem(TgItem2d *prevTabItem);
protected:
    virtual void render(const TgWindowInfo *windowInfo);
    virtual void checkPositionValues(const TgWindowInfo *windowInfo);

    void renderChildren(const TgWindowInfo *windowInfo);
    void checkPositionValuesChildren(const TgWindowInfo *windowInfo);

    virtual void onEnabledChanged(bool enabled);

    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo);
    TgEventResult handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo);

    void checkOnResizeChanged();
    void checkOnResizeChangedOnChildren();
    virtual void handlePrivateMessage(const TgItem2dPrivateMessage *message);

private:
    TgItem2dPrivate *m_private;

    void addChild(TgItem2d *child);
    void sendMessageToChildren(const TgItem2dPrivateMessage *message);
    void sendMessageToChildrenFromBegin(const TgItem2dPrivateMessage *message);
    virtual void addNewEvent(TgEventData *event);

    void setAddMinMaxHeightOnVisible(float addMinHeightOnVisible, float addMaxHeightOnVisible);

    friend class TgItem2dPrivate;
    friend class TgMainWindowPrivate;
    friend class TgTextfieldPrivate;
    friend class TgButtonPrivate;
};

#endif // TG_ITEM_2D_H
