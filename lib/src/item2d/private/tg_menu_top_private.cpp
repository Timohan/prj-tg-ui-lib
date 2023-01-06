/*!
 * \file
 * \brief file tg_menu_top_private.cpp
 *
 * Top menu (private)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_menu_top_private.h"
#include <cmath>
#include <limits>
#include "../../global/tg_global_log.h"
#include "../tg_menu_item.h"
#include "item2d/tg_item2d_private.h"
#include "../private/tg_menu_item_private.h"
#include "../../global/private/tg_global_deleter.h"
#include "../../global/private/tg_global_wait_renderer.h"

TgMenuTopPrivate::TgMenuTopPrivate(TgItem2d *currentItem, TgItem2d *parentItem) :
    m_currentItem(currentItem),
    m_parentItem(parentItem),
    m_background(currentItem)
{
}

TgMenuTopPrivate::~TgMenuTopPrivate()
{
    m_mutex.lock();
    for (size_t i=0;i<m_listMainMenuItem.size();i++) {
        delete m_listMainMenuItem[i];
    }
    m_listMainMenuItem.clear();
    m_mutex.unlock();
}

TgMenuItem *TgMenuTopPrivate::addMenu(const char *text, const TgShortCut *shortCut)
{
    m_mutex.lock();
    m_listMainMenuItem.push_back(new TgMenuItem(nullptr, m_currentItem, text, shortCut));
    TgMenuItem *ret = m_listMainMenuItem.back();
    TgItem2d *retItem = ret;
    retItem->m_private->setTopMenu(true);
    ret->m_private->setMenuType(TgMenuItemPrivate::MenuType::MenuType_TopMenu);
    if (m_listMainMenuItem.size() >= 2) {
        TgMenuItem *item = m_listMainMenuItem.at(m_listMainMenuItem.size()-2);
        ret->setX(item->getX()+item->getWidth());
    }
    m_mutex.unlock();
    return ret;
}

void TgMenuTopPrivate::checkOnResizeChanged()
{
    m_mutex.lock();
    m_background.checkOnResizeChanged();
     if (std::fabs(m_currentItem->getWidth() - m_parentItem->getWidth()) > std::numeric_limits<double>::epsilon()) {
        m_currentItem->setWidth(m_parentItem->getWidth());
     }
     m_mutex.unlock();
}

void TgMenuTopPrivate::checkPositionValuesMenu(bool useLock)
{
    if (useLock) {
        m_mutex.lock();
    }
    float x = 0;
    for (size_t s=0;s<m_listMainMenuItem.size();s++) {
        if (m_listMainMenuItem.at(s)->getVisible()) {
            m_listMainMenuItem.at(s)->setX(x);
            x += m_listMainMenuItem.at(s)->getWidth();
        }
    }
    if (useLock) {
        m_mutex.unlock();
    }
}

size_t TgMenuTopPrivate::getMenuCount()
{
    m_mutex.lock();
    size_t ret = 0;
    for (size_t i=0;i<m_listMainMenuItem.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listMainMenuItem[i])->m_private->getDeleting()) {
            continue;
        }
        ret++;
    }
    m_mutex.unlock();
    return ret;
}

bool TgMenuTopPrivate::removeMenu(size_t indexToRemove)
{
    m_mutex.lock();
    if (indexToRemove >= m_listMainMenuItem.size()) {
        m_mutex.unlock();
        return false;
    }
    size_t tmpValue = 0;
    bool ret = false;
    for (size_t i=0;i<m_listMainMenuItem.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listMainMenuItem[i])->m_private->getDeleting()) {
            continue;
        }
        if (tmpValue == indexToRemove) {
            TgGlobalDeleter::getInstance()->addTopMenu(this, m_listMainMenuItem[i]);
            reinterpret_cast<TgItem2d *>(m_listMainMenuItem[i])->m_private->setDeleting();
            ret = true;
            break;
        }
        tmpValue++;
    }
    m_mutex.unlock();
    return ret;
}

void TgMenuTopPrivate::deleteMenu(TgMenuItem *item)
{
    m_mutex.lock();
    size_t i;
    for (i=0;i<m_listMainMenuItem.size();i++) {
        if (m_listMainMenuItem[i] == item) {
            delete m_listMainMenuItem[i];
            m_listMainMenuItem.erase(m_listMainMenuItem.begin()+i);
            break;
        }
    }
    m_mutex.unlock();
}
