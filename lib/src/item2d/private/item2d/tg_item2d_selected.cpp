/*!
 * \file
 * \brief file tg_item2d_selected.cpp
 *
 * it holds selected values for TgItem2dPrivate
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_item2d_selected.h"
#include <cmath>
#include "../../../global/tg_global_log.h"
#include "../../tg_item2d.h"
#include "tg_item2d_private.h"

TgItem2dSelected::TgItem2dSelected(TgItem2d *parent, TgItem2d *currentItem, TgItem2dPrivate *currentItem2dPrivate) :
    m_parent(parent),
    m_currentItem(currentItem),
    m_currentItem2dPrivate(currentItem2dPrivate),
    m_nextTabItem(nullptr),
    m_prevTabItem(nullptr),
    m_selected(false),
    m_canSelect(false),
    f_selectedChanged(nullptr)
{
}

/*!
 * \brief TgItem2dSelected::getSelected
 *
 * \return is selected
 */
bool TgItem2dSelected::getSelected()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_selected;
}

/*!
 * \brief TgItem2dSelected::getCanSelect
 *
 * \return can item be selected
 */
bool TgItem2dSelected::getCanSelect()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_canSelect && m_currentItem2dPrivate->getEnabled();
}

/*!
 * \brief TgItem2dSelected::setSelected
 *
 * \param selected sets selected true/false
 */
void TgItem2dSelected::setSelected(bool selected)
{
    TG_FUNCTION_BEGIN();
    if (m_selected == selected) {
        TG_FUNCTION_END();
        return;
    }
    m_selected = selected;
    if (m_currentItem2dPrivate->m_internalCallback) {
        m_currentItem2dPrivate->m_internalCallback->onSelectedCallback();
    }
    if (selected) {
        TgItem2dPrivateMessage msg;
        msg.m_fromItem = m_currentItem2dPrivate->m_currentItem;
        msg.m_type = TgItem2dPrivateMessageType::SetUnselected;
        m_currentItem2dPrivate->sendMessageToChildrenFromBegin(&msg);
    }
    if (f_selectedChanged) {
        f_selectedChanged(selected);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dSelected::setCanSelect
 *
 * \param canSelect sets "can be selected" true/false
 */
void TgItem2dSelected::setCanSelect(bool canSelect)
{
    TG_FUNCTION_BEGIN();
    m_canSelect = canSelect;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dSelected::connectOnSelectedChanged
 *
 * set connect selected changed callback
 * \param selectedChanged callback of selected changed
 */
void TgItem2dSelected::connectOnSelectedChanged(std::function<void(bool)> selectedChanged)
{
    TG_FUNCTION_BEGIN();
    if (selectedChanged) {
        f_selectedChanged = selectedChanged;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dSelected::disconnectOnSelectedChanged
 *
 * disconnect selected changed callback
 */
void TgItem2dSelected::disconnectOnSelectedChanged()
{
    TG_FUNCTION_BEGIN();
    f_selectedChanged = nullptr;
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dSelected::handleEventSelected
 *
 * Handle events that are result to selelect
 * keys: shift + '\t' and '\t'
 *
 * \param eventData
 * \param result [out] filled if this is true
 * \return true if result was filled this item
 * was using handleEvent in this function
 * and Item2d's own handle event can be follow
 * the result
 */
bool TgItem2dSelected::handleEventSelected(TgEventData *eventData, TgEventResult &result)
{
    TG_FUNCTION_BEGIN();
    if (eventData->m_type == TgEventType::EventTypeSelectNextItem) {
        if (getCanSelect()) {
            eventData->m_event.m_selectLastItem.m_previousItem2d->setSelected(false);
            setSelected(true);
            result = TgEventResult::EventResultCompleted;
            return true;
        }
        result = TgEventResult::EventResultNotCompleted;
        TG_FUNCTION_END();
        return true;
    }

    if (eventData->m_type == TgEventType::EventTypeSelectFirstItem) {
        if (eventData->m_event.m_selectLastItem.m_previousItem2d == m_currentItem) {
            result = TgEventResult::EventResultCompleted;
            return true;
        }
        if (getCanSelect()) {
            if (eventData->m_event.m_selectLastItem.m_previousItem2d) {
                eventData->m_event.m_selectLastItem.m_previousItem2d->setSelected(false);
            }
            setSelected(true);
            result = TgEventResult::EventResultCompleted;
            return true;
        }
        result = TgEventResult::EventResultNotCompleted;
        TG_FUNCTION_END();
        return true;
    }

    if (eventData->m_type == TgEventType::EventTypeSelectLastItem) {
        if (getSelected()) {
            result = TgEventResult::EventResultCompleted;
            TG_FUNCTION_END();
            return true;
        }
        if (getCanSelect()) {
             eventData->m_event.m_selectLastItem.m_nextItem2d = m_currentItem;
        }
        result = TgEventResult::EventResultNotCompleted;
        TG_FUNCTION_END();
        return true;
    }

    if (eventData->m_type != TgEventType::EventTypeCharacterCallback
        || eventData->m_event.m_keyEvent.m_pressReleaseKey != TgPressReleaseKey::PressReleaseKey_NormalKey
        || eventData->m_event.m_keyEvent.m_key != '\t') {
        TG_FUNCTION_END();
        return false;
    }

    // after this it's just tab select functionality
    if ((eventData->m_event.m_keyEvent.m_pressModsKeyDown & static_cast<int>(TgPressModsKeyDown::PressModsKeyDown_Shift)) == 0) {
        // it is '\t' without shift
        if (getSelected()) {
            m_mutex.lock();
            if (m_nextTabItem && m_nextTabItem->getCanSelect() && m_nextTabItem->getVisible()) {
                setSelected(false);
                m_nextTabItem->setSelected(true);
                m_mutex.unlock();
                result = TgEventResult::EventResultCompleted;
                TG_FUNCTION_END();
                return true;
            }
            m_mutex.unlock();

            eventData->m_type = TgEventType::EventTypeSelectNextItem;
            eventData->m_event.m_selectLastItem.m_previousItem2d = m_currentItem;
            eventData->m_event.m_selectLastItem.m_nextItem2d = nullptr;
        }
        result = TgEventResult::EventResultNotCompleted;
        TG_FUNCTION_END();
        return true;
    } else if ((eventData->m_event.m_keyEvent.m_pressModsKeyDown & static_cast<int>(TgPressModsKeyDown::PressModsKeyDown_Shift))) {
        // it is '\t' with shift
        if (getSelected()) {
            m_mutex.lock();
            if (m_prevTabItem && m_prevTabItem->getCanSelect() && m_prevTabItem->getVisible()) {
                setSelected(false);
                m_prevTabItem->setSelected(true);
                m_mutex.unlock();
                result = TgEventResult::EventResultCompleted;
                TG_FUNCTION_END();
                return true;
            }
            m_mutex.unlock();
            if (eventData->m_event.m_keyEvent.m_previousItem2d) {
                // it's selected item, and previous item was already found
                setSelected(false);
                eventData->m_event.m_keyEvent.m_previousItem2d->setSelected(true);
                result = TgEventResult::EventResultCompleted;
                TG_FUNCTION_END();
                return true;
            } else {
                // it's selected item, and previous item was NOT found
                eventData->m_type = TgEventType::EventTypeSelectLastItem;
                eventData->m_event.m_selectLastItem.m_previousItem2d = m_currentItem;
                eventData->m_event.m_selectLastItem.m_nextItem2d = nullptr;
                result = TgEventResult::EventResultNotCompleted;
                TG_FUNCTION_END();
                return true;
            }
        } else if (getCanSelect()) {
            eventData->m_event.m_keyEvent.m_previousItem2d = m_currentItem;
        }
        result = TgEventResult::EventResultNotCompleted;
        TG_FUNCTION_END();
        return true;
    }
    result = TgEventResult::EventResultNotCompleted;
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgItem2dSelected::setNextTabItem
 *
 * set item to be selected with tab
 *
 * \param nextTabItem item to be selected with tab
 * NOTE: this item must be selectable
 */
void TgItem2dSelected::setNextTabItem(TgItem2d *nextTabItem)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_nextTabItem = nextTabItem;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgItem2dSelected::setPrevTabItem
 *
 * set item to be selected with shift+tab
 *
 * \param prevTabItem item to be selected with shift+tab
 * NOTE: this item must be selectable
 */
void TgItem2dSelected::setPrevTabItem(TgItem2d *prevTabItem)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_prevTabItem = prevTabItem;
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*! /brief TgItem2dSelected::handleMessageToChildren
 * handles the messages that are sent from sendMessageToChildren
 * (or sendMessageToChildrenBegin)
 * \param message
 */
void TgItem2dSelected::handleMessageToChildren(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    if (message->m_type == TgItem2dPrivateMessageType::RemovingItem2d) {
        m_mutex.lock();
        if (message->m_fromItem == m_nextTabItem) {
            m_nextTabItem = nullptr;
        }
        if (message->m_fromItem == m_prevTabItem) {
            m_prevTabItem = nullptr;
        }
        m_mutex.unlock();
    }
    TG_FUNCTION_END();
}