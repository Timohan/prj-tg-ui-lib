/*!
 * \file
 * \brief file tg_item2d_private.h
 *
 * it holds general TgItem2dPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_PRIVATE_H
#define TG_ITEM_2D_PRIVATE_H

#include <vector>
#include <functional>
struct TgWindowInfo;
#include "../../event/tg_event_data.h"
#include "../tg_item2d.h"
#include "tg_item2d_position.h"

enum TgItem2dPrivateMessageType
{
    PositionChanged = 0,
    SetUnselected,
    RemovingItem2d,
    ParentItemToVisible,
    ParentItemToInvisible,
    ParentItemToUseRoundedPositionValues,
    ParentItemToUseNotRoundedPositionValues,
};

struct TgItem2dPrivateMessage
{
    TgItem2dPrivateMessageType m_type;
    TgItem2d *m_fromItem;
};

enum TgItem2dVisibilityState
{
    TgItem2dVisible = 0,
    TgItem2dInvisible,
    TgItem2dVisibleButParentInvisible
};

class TgItem2dInternalCallback
{
public:
    virtual void onSelectedCallback() = 0;
};

class TgItem2dPrivate : public TgItem2dPosition
{
public:
    explicit TgItem2dPrivate(TgItem2d *parent, TgItem2d *current);
    explicit TgItem2dPrivate(float x, float y, float width, float height, TgItem2d *parent, TgItem2d *current);
    ~TgItem2dPrivate();

    bool getVisible();
    void setVisible(bool visible);
    void renderChildren(const TgWindowInfo *windowInfo);
    void checkPositionValuesChildren(const TgWindowInfo *windowInfo);
    TgEventResult handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo);
    TgEventResult handleEventsChildren(TgEventData *eventData);

    void connectOnVisibleChanged(std::function<void(bool)> visibleChanged);
    void disconnectOnVisibleChanged();

    void checkOnResizeChangedOnChildren();

    bool getSelected();
    bool getCanSelect();
    bool getEnabled();
    void setSelected(bool selected);
    void setCanSelect(bool canSelect);
    void setEnabled(bool enabled);

    void setInternalCallbacks(TgItem2dInternalCallback *callback);
    void setToTop(TgItem2d *child);
protected:
    std::vector<TgItem2d *>m_listChildren;

private:
    TgItem2dInternalCallback *m_internalCallback;
    TgItem2dVisibilityState m_visibleState;
    TgItem2d *m_parent;
    TgItem2d *m_currentItem;
    bool m_selected;
    bool m_canSelect;
    bool m_enabled;

    std::function<void(bool)> f_visibleChanged;

    void setDefaultValues();
    void addChild(TgItem2d *child);
    void sendMessageToChildren(const TgItem2dPrivateMessage *message, bool allowFunctionalityToThisItem = true);
    void sendMessageToChildrenFromBegin(const TgItem2dPrivateMessage *message);
    void parentVisibleChanged(bool visible);

    friend class TgItem2d;
    friend class TgItem2dPosition;
};

#endif // TG_ITEM_2D_PRIVATE_H
