/*!
 * \file
 * \brief file tg_global_menu_holder.cpp
 *
 * global menu holder, holds the pointer of menu items
 * but parent menu's only
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_global_menu_holder.h"
#include "../tg_global_macros.h"
#include "../tg_global_log.h"
#include "../../item2d/tg_menu_item.h"

TgGlobalMenuHolder *TgGlobalMenuHolder::m_globalMenuHolder = nullptr;

TgGlobalMenuHolder *TgGlobalMenuHolder::getInstance()
{
    TG_FUNCTION_BEGIN();
    if (!TgGlobalMenuHolder::m_globalMenuHolder) {
        TgGlobalMenuHolder::m_globalMenuHolder = new TgGlobalMenuHolder();
    }
    TG_FUNCTION_END();
    return TgGlobalMenuHolder::m_globalMenuHolder;
}

uint64_t TgGlobalMenuHolder::addMenuItem(TgMenuItem *menuItem)
{
    TG_FUNCTION_BEGIN();
    std::lock_guard<std::recursive_mutex> m(m_mutex);
    m_itemIndex++;
    m_mapMenuItems[m_itemIndex] = menuItem;
    TG_FUNCTION_END();
    return m_itemIndex;
}

void TgGlobalMenuHolder::removeMenuItem(uint64_t index)
{
    TG_FUNCTION_BEGIN();
    std::lock_guard<std::recursive_mutex> m(m_mutex);
    m_mapMenuItems.erase(index);
    TG_FUNCTION_END();
}

TgEventResult TgGlobalMenuHolder::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    std::lock_guard<std::recursive_mutex> m(m_mutex);
    for (std::map<uint64_t, TgMenuItem *>::iterator it=m_mapMenuItems.begin();it!=m_mapMenuItems.end();it++) {
        if (!it->second->getEnabled()) {
            continue;
        }
        if (it->second->handleChildrenEventMenu(eventData, windowInfo) == TgEventResult::EventResultCompleted) {
            TG_FUNCTION_END();
            return TgEventResult::EventResultCompleted;
        }
    }
    TG_FUNCTION_END();
    return TgEventResult::EventResultNotCompleted;
}
