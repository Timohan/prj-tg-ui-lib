/*!
 * \file
 * \brief file tg_item2d_selected.h
 *
 * it holds selected values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_PRIVATE_SELECTED_H
#define TG_ITEM_2D_PRIVATE_SELECTED_H

#include <functional>
#include <mutex>
#include "../../../event/tg_event_data.h"

class TgItem2d;
class TgItem2dPrivate;
class TgItem2dPrivateMessage;

class TgItem2dSelected
{
public:
    TgItem2dSelected(TgItem2d *parent, TgItem2d *currentItem, TgItem2dPrivate *currentItem2dPrivate);

    bool getSelected();
    bool getCanSelect();

    void setSelected(bool selected);
    void setCanSelect(bool canSelect);

    void connectOnSelectedChanged(std::function<void(bool)> selectedChanged);
    void disconnectOnSelectedChanged();

    void setNextTabItem(TgItem2d *nextTabItem);
    void setPrevTabItem(TgItem2d *prevTabItem);
protected:
    bool handleEventSelected(TgEventData *eventData, TgEventResult &result);
    void handleMessageToChildren(const TgItem2dPrivateMessage *message);

private:
    TgItem2d *m_parent;
    TgItem2d *m_currentItem;
    TgItem2dPrivate *m_currentItem2dPrivate;
    TgItem2d *m_nextTabItem;
    TgItem2d *m_prevTabItem;
    std::mutex m_mutex;

    bool m_selected;
    bool m_canSelect;

    std::function<void(bool selected)> f_selectedChanged;
};

#endif // TG_ITEM_2D_PRIVATE_SELECTED_H
