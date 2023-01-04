/*!
 * \file
 * \brief file tg_global_menu_holder.h
 *
 * global menu holder, holds the pointer of menu items
 * but parent menu's only
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GLOBAL_MENU_HOLDER_H
#define TG_GLOBAL_MENU_HOLDER_H

#include <map>
#include <mutex>
#include <cstdint>
#include "../../event/tg_event_data.h"

struct TgEventData;
class TgMenuItem;
struct TgWindowInfo;

class TgGlobalMenuHolder
{
public:
    static TgGlobalMenuHolder *getInstance();

    uint64_t addMenuItem(TgMenuItem *menuItem);
    void removeMenuItem(uint64_t index);

    TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo);

private:
    static TgGlobalMenuHolder *m_globalMenuHolder;

    std::recursive_mutex m_mutex;
    uint64_t m_itemIndex = 0;
    std::map<uint64_t, TgMenuItem *>m_mapMenuItems;

};

#endif // TG_GLOBAL_MENU_HOLDER_H
