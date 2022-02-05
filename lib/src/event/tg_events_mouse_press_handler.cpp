/*!
 * \file
 * \brief file tg_events_mouse_press_handler.cpp
 *
 * Additional class for TgEvents
 * handlers caching mouse down events for TgEvents
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include <cmath>
#include <cstdlib>
#include "../global/tg_global_log.h"
#include "tg_events_mouse_press_handler.h"

TgEventsMousePressHandler::TgEventsMousePressHandler() :
    m_allowedNumberMouseButtonDownCount(1)
{

}

/*!
 * \brief TgEvents::setMouseDownItem
 *
 * \param type mouse button type
 * \param mouseDownItem set item that was mouse down type
 */
void TgEventsMousePressHandler::setMouseDownItem(TgMouseType type, TgItem2d *mouseDownItem)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgEventsMousePressType>::iterator it;
    m_mutex.lock();
    for (it=m_listMouseType.begin();it!=m_listMouseType.end();it++) {
        if (it->m_mouseType == type) {
            it->m_mouseDownItem = mouseDownItem;
            it->m_releaseWoCallback = false;
            m_mutex.unlock();
            TG_FUNCTION_END();
            return;
        }
    }
    TgEventsMousePressType event;
    event.m_mouseDownItem = mouseDownItem;
    event.m_mouseType = type;
    event.m_releaseWoCallback = false;
    m_listMouseType.push_back(event);
    if (m_allowedNumberMouseButtonDownCount < m_listMouseType.size()) {
        size_t allowedCount = m_allowedNumberMouseButtonDownCount;
        int i, c = static_cast<int>(m_listMouseType.size());
        for (i=c-1;i>=0;i--) {
            if (m_listMouseType[i].m_releaseWoCallback) {
                continue;
            }
            if (allowedCount) {
                allowedCount--;
                continue;
            }
            m_listMouseType[i].m_releaseWoCallback = true;
        }
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgEvents::getMouseDownItem
 *
 * \param type mouse button type
 * \param releaseWoCallback [out] if true, the item must be released without click callback
 * \return item that was mouse down item for mouse down type
 * returns nullptr if not found such item
 */
TgItem2d *TgEventsMousePressHandler::getMouseDownItem(TgMouseType type, bool &releaseWoCallback)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgEventsMousePressType>::iterator it;
    for (it=m_listMouseType.begin();it!=m_listMouseType.end();it++) {
        if (it->m_mouseType == type) {
            releaseWoCallback = it->m_releaseWoCallback;
            TG_FUNCTION_END();
            return it->m_mouseDownItem;
        }
    }
    releaseWoCallback = false;
    TG_FUNCTION_END();
    return nullptr;
}

/*!
 * \brief TgEventsMousePressHandler::removingItem
 *
 * when item is to remove, then this is called so it can possible
 * remove current m_mouseDownItem
 *
 * \param itemToRemove
 */
void TgEventsMousePressHandler::removingItem(TgItem2d *itemToRemove)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgEventsMousePressType>::iterator it;
    m_mutex.lock();
    for (it=m_listMouseType.begin();it!=m_listMouseType.end();it++) {
        if (it->m_mouseDownItem == itemToRemove) {
            m_listMouseType.erase(it);
            break;
        }
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgEventsMousePressHandler::removingItem
 *
 * when item is to remove, then this is called so it can possible
 * remove current m_mouseDownItem
 *
 * \param type
 */
void TgEventsMousePressHandler::removingItem(TgMouseType type)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgEventsMousePressType>::iterator it;
    m_mutex.lock();
    for (it=m_listMouseType.begin();it!=m_listMouseType.end();it++) {
        if (it->m_mouseType == type) {
            m_listMouseType.erase(it);
            break;
        }
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgEventsMousePressHandler::setAllowedNumberMouseButtonCount
 *
 * set allowed number of mouse button down at once
 *
 * \param allowedNumberMouseButtonDownCount
 */
void TgEventsMousePressHandler::setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount)
{
    m_mutex.lock();
    if (allowedNumberMouseButtonDownCount < m_listMouseType.size()) {
        while (1) {
            m_listMouseType.erase(m_listMouseType.begin());
            if (allowedNumberMouseButtonDownCount >= m_listMouseType.size()) {
                break;
            }
        }
    }
    m_allowedNumberMouseButtonDownCount = allowedNumberMouseButtonDownCount;
    m_mutex.unlock();
}

/*!
 * \brief TgEventsMousePressHandler::getAllowedNumberMouseButtonCount
 *
 * \return allowed number of mouse button down at once
 */
size_t TgEventsMousePressHandler::getAllowedNumberMouseButtonCount()
{
    m_mutex.lock();
    size_t ret = m_allowedNumberMouseButtonDownCount;
    m_mutex.unlock();
    return ret;
}