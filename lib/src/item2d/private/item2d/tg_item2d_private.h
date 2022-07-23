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
#include "../../../event/tg_event_data.h"
#include "../../tg_item2d.h"
#include "tg_item2d_position.h"
#include "tg_item2d_visible.h"
#include "tg_item2d_selected.h"
#include "tg_item2d_enabled.h"

enum TgItem2dPrivateMessageType
{
    PositionChanged = 0,
    SetUnselected,
    RemovingItem2d,
    ParentItemToVisible,
    ParentItemToInvisible,
    ParentItemToUseRoundedPositionValues,
    ParentItemToUseNotRoundedPositionValues,
    HoverEnabledOnItem,
    ItemToVisibleChanged,
    ParentItemToEnabled,
    ItemToEnabledChanged,
    ParentItemToDisabled,
    EventClearButtonPressForThisItem,

    CurrentItemToInvisible,  /*!< This is only used on virtual void handlePrivateMessage(const TgItem2dPrivateMessage *message); */
    CurrentItemToDisabled,
};

struct TgItem2dPrivateMessage
{
    TgItem2dPrivateMessageType m_type;
    TgItem2d *m_fromItem;
};

class TgItem2dInternalCallback
{
public:
    virtual void onSelectedCallback() = 0;
};

class TgItem2dPrivate : public TgItem2dVisible, public TgItem2dEnabled, public TgItem2dPosition, public TgItem2dSelected
{
public:
    explicit TgItem2dPrivate(TgItem2d *parent, TgItem2d *current);
    explicit TgItem2dPrivate(float x, float y, float width, float height, TgItem2d *parent, TgItem2d *current);
    ~TgItem2dPrivate();

    void renderChildren(const TgWindowInfo *windowInfo);
    void checkPositionValuesChildren(const TgWindowInfo *windowInfo);
    TgEventResult handleEventsChildren(TgEventData *eventData, const TgWindowInfo *windowInfo);
    TgEventResult handleEventsChildren(TgEventData *eventData);

    void checkOnResizeChangedOnChildren();

    void setInternalCallbacks(TgItem2dInternalCallback *callback);
    void setToTop(TgItem2d *child);

    bool isCursorOnItem(double x, double y, const TgWindowInfo *windowInfo);

protected:
    std::vector<TgItem2d *>m_listChildren;

private:
    TgItem2dInternalCallback *m_internalCallback;
    TgItem2d *m_parent;
    TgItem2d *m_currentItem;

    void addChild(TgItem2d *child);
    void sendMessageToChildren(const TgItem2dPrivateMessage *message, bool allowFunctionalityToThisItem = true);
    void sendMessageToChildrenFromBegin(const TgItem2dPrivateMessage *message);

    friend class TgItem2d;
    friend class TgItem2dPosition;
    friend class TgItem2dVisible;
    friend class TgItem2dEnabled;
    friend class TgItem2dSelected;
};

#endif // TG_ITEM_2D_PRIVATE_H
