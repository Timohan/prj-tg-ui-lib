/*!
 * \file
 * \brief file tg_global_tooltip.h
 *
 * Tooltip information collected here between for temporary
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GLOBAL_TOOLTIP_H
#define TG_GLOBAL_TOOLTIP_H

#include <mutex>
#include "../../common/time_difference.h"
#include <stdint.h>

class TgItem2dTooltip;

struct TgToolTipInfo
{
    TgItem2dTooltip *m_currentTooltipParentItem = nullptr;
    std::string m_text;
    int m_x;
    int m_y;
};

class TgGlobalTooltip
{
public:
    static TgGlobalTooltip *getInstance();
    void setTooltip(const std::string &text, int x, int y, TgItem2dTooltip *currentTooltipParentItem);
    void clearTooltip(TgItem2dTooltip *currentTooltip);
    void setTooltipOnDisabled();

    void startHandleEvents();
    std::string startRendering(int &x, int &y);
    int getMsToWaitRendering();

private:
    static TgGlobalTooltip *m_globalTooltip;
    std::mutex m_mutex;
    TgToolTipInfo m_previousRenderToolTipInfo;
    TgToolTipInfo m_newRenderToolTipInfo;
    TimeDifference m_timeDiff;
    int m_timeMinTimeToWait = -1;
    bool m_setValidToolTipDone = false;
};

#endif // TG_GLOBAL_TOOLTIP_H