/*!
 * \file
 * \brief file tg_global_deleter.cpp
 *
 * global deleter collector
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_global_deleter.h"
#include "../tg_global_macros.h"
#include "../tg_global_log.h"
#include "../../item2d/tg_menu_top.h"
#include "../../item2d/private/tg_menu_top_private.h"
#include "../../item2d/private/item2d/tg_item2d_menu.h"

TgGlobalDeleter *TgGlobalDeleter::m_globalDeleter = nullptr;

TgGlobalDeleter *TgGlobalDeleter::getInstance()
{
    TG_FUNCTION_BEGIN();
    if (!TgGlobalDeleter::m_globalDeleter) {
        TgGlobalDeleter::m_globalDeleter = new TgGlobalDeleter();
    }
    TG_FUNCTION_END();
    return TgGlobalDeleter::m_globalDeleter;
}

void TgGlobalDeleter::addTopMenu(TgMenuTopPrivate *topItem, TgMenuItem *menuItem)
{
    m_mutex.lock();
    TgDeleterInfo item;
    item.m_type = TgDeleterInfoType::TopMenuItem;
    item.m_itemType.m_topMenuItem.m_topItem = topItem;
    item.m_itemType.m_topMenuItem.m_item = menuItem;
    m_listItemsToDelete.push_back(item);
    m_mutex.unlock();
}

void TgGlobalDeleter::addSubMenu(TgItem2dMenu *parentMenu, TgMenuItem *menuItem)
{
    m_mutex.lock();
    TgDeleterInfo item;
    item.m_type = TgDeleterInfoType::SubMenuItem;
    item.m_itemType.m_subMenuItem.m_subItem = parentMenu;
    item.m_itemType.m_subMenuItem.m_item = menuItem;
    m_listItemsToDelete.push_back(item);
    m_mutex.unlock();
}

bool TgGlobalDeleter::removeItems()
{
    m_mutex.lock();
    bool ret = !m_listItemsToDelete.empty();
    for (size_t i=0;i<m_listItemsToDelete.size();i++) {
        switch (m_listItemsToDelete[i].m_type) {
            case TgDeleterInfoType::NormalItem:
                break;
            case TgDeleterInfoType::TopMenuItem:
                m_listItemsToDelete[i].m_itemType.m_topMenuItem.m_topItem->deleteMenu( m_listItemsToDelete[i].m_itemType.m_topMenuItem.m_item );
                break;
            case TgDeleterInfoType::SubMenuItem:
                m_listItemsToDelete[i].m_itemType.m_subMenuItem.m_subItem->deleteMenu( m_listItemsToDelete[i].m_itemType.m_subMenuItem.m_item );
                break;
            default:
                break;
        }
    }
    m_listItemsToDelete.clear();
    m_mutex.unlock();
    return ret;
}
