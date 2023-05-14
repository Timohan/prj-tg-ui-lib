/*!
 * \file
 * \brief file tg_global_deleter.h
 *
 * global deleter collector
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GLOBAL_DELETER_H
#define TG_GLOBAL_DELETER_H

#include <mutex>
#include <vector>
#include "../../event/tg_event_data.h"
class TgItem2d;
class TgMenuTopPrivate;
class TgMenuItem;
class TgItem2dMenu;
class TgComboBoxPrivate;

enum TgDeleterInfoType
{
    NormalItem = 0,
    TopMenuItem,
    SubMenuItem,
    ComboBoxMenuItem,
};

struct TgDeleterInfo
{
    TgDeleterInfoType m_type;
    union {
        struct {
            TgItem2d *m_item;
        } m_normalItem;
        struct {
            TgMenuTopPrivate *m_topItem;
            TgMenuItem *m_item;
        } m_topMenuItem;
        struct {
            TgItem2dMenu *m_subItem;
            TgMenuItem *m_item;
        } m_subMenuItem;
        struct {
            TgComboBoxPrivate *m_comboBoxItem;
            TgMenuItem *m_item;
        } m_comboBoxMenuItem;
    } m_itemType;
};

class TgGlobalDeleter
{
public:
    static TgGlobalDeleter *getInstance();
    void addTopMenu(TgMenuTopPrivate *topItem, TgMenuItem *menuItem);
    void addSubMenu(TgItem2dMenu *parentMenu, TgMenuItem *menuItem);
    void addComboBoxMenu(TgComboBoxPrivate *comboBoxMenu, TgMenuItem *menuItem);
    void add(TgItem2d *itemToDelete);
    bool removeItems();

private:
    static TgGlobalDeleter *m_globalDeleter;
    std::recursive_mutex m_mutex;
    std::vector<TgDeleterInfo>m_listItemsToDelete;
};

#endif // TG_GLOBAL_DELETER_H
