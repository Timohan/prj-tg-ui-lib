/*!
 * \file
 * \brief file tg_item2d_tooltip.h
 *
 * Tooltip handler
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_ITEM_2D_PRIVATE_TOOLTIP_H
#define TG_ITEM_2D_PRIVATE_TOOLTIP_H

#include <functional>
#include <mutex>
#include "../../../event/tg_event_data.h"
#include "../../../global/private/tg_global_wait_renderer.h"

class TgItem2dPrivate;
struct TgWindowInfo;
struct TgEventData;

class TgItem2dTooltip
{
public:
    TgItem2dTooltip(TgItem2dPrivate *currentItemPrivate);
    ~TgItem2dTooltip();

    void setCanBlockTooltip(bool canBlockTooltip);
    void setTooltip(const char *text);
    void handleEventToolTip(TgEventData *eventData, const TgWindowInfo *windowInfo, bool fromMainWindow = false);
    void itemGoneDisabledOrInvisible();

private:
    TgItem2dPrivate *m_currentItem;
    std::string m_text;
    bool m_canBlockTooltip = false;
};

#endif // TG_ITEM_2D_PRIVATE_TOOLTIP_H
