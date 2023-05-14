/*!
 * \file
 * \brief file tg_combo_box.cpp
 *
 * ComboBox
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_combo_box.h"
#include "../global/tg_global_log.h"
#include "private/tg_combo_box_private.h"

TgComboBox::TgComboBox(TgItem2d *parent) :
    TgMouseCapture(parent),
    m_private(new TgComboBoxPrivate(this))
{
}

TgComboBox::TgComboBox(TgItem2d *parent, float x, float y, float width, float height) :
    TgMouseCapture(parent, x, y, width, height),
    m_private(new TgComboBoxPrivate(this))
{
}

TgComboBox::~TgComboBox()
{
    if (m_private) {
        delete m_private;
    }
}

/*!
 * \brief TgComboBox::onHoverChanged
 *
 * when hover state is changed, this is called
 *
 * \param hover true if cursor is on button
 */
void TgComboBox::onHoverChanged(bool)
{
    TG_FUNCTION_BEGIN();
    m_private->changeButtonImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBox::onDownChanged
 *
 * set combobox's property to down (pressed) or up (released)
 *
 * \param down true if button is set down
 */
void TgComboBox::onDownChanged(bool down)
{
    TG_FUNCTION_BEGIN();
    m_private->changeDownProperty(down);
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBox::handlePrivateMessage
 *
 * virtual function to handle private function
 *
 * \param message
 */
void TgComboBox::handlePrivateMessage(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    TgMouseCapture::handlePrivateMessage(message);
    if (message->m_type == TgItem2dPrivateMessageType::CurrentItemToDisabled) {
        m_private->changeButtonImageFile();
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBox::addItemText
 *
 * adds text to combobox
 *
 * \param text text to index
 * \param index index to add, if UINT64_MAX, then it adds to last index
 */
void TgComboBox::addItemText(const char *text, size_t index)
{
    m_private->addItemText(text, index);
}

/*!
 * \brief TgComboBox::getItemText
 *
 * \param get text of index
 * \return text by the index
 */
std::string TgComboBox::getItemText(size_t index)
{
    return m_private->getItemText(index);
}

/*!
 * \brief TgComboBox::getItemCount
 *
 * \return number of items of combobox
 */
size_t TgComboBox::getItemCount() const
{
    return m_private->getItemCount(true);
}

/*!
 * \brief TgComboBox::removeText
 *
 * remove text by index
 *
 * \param index to remove
 * \return true if remove was correct, false if index is larger than
 * number of items in the list
 */
bool TgComboBox::removeItemText(size_t index)
{
    return m_private->removeItemText(index);
}

/*!
 * \brief TgComboBox::getCurrentIndex
 *
 * get combobox's index
 *
 * \return index of combobox
 */
size_t TgComboBox::getCurrentIndex() const
{
    return m_private->getCurrentIndex();
}

/*!
 * \brief TgComboBox::setCurrentIndex
 *
 * Set combobox's index
 *
 * \param index
 * \return true if set was correct, false if index is larger than
 * number of items in the list
 */
bool TgComboBox::setCurrentIndex(size_t index)
{
    return m_private->setCurrentIndex(index);
}

/*!
 * \brief TgComboBox::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
TgEventResult TgComboBox::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (m_private->handleEventComboBox(eventData, windowInfo) == TgEventResult::EventResultCompleted) {
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    TG_FUNCTION_END();
    return TgMouseCapture::handleEvent(eventData, windowInfo);
}

/*!
 * \brief TgComboBox::connectOnSelectedIndexChanged
 *
 * connects callback to function for on selected index changed
 * \param selectedIndexChanged
 */
void TgComboBox::connectOnSelectedIndexChanged(std::function<void(size_t)> selectedIndexChanged)
{
    TG_FUNCTION_BEGIN();
    m_private->connectOnSelectedIndexChanged(selectedIndexChanged);
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBox::disconnectOnSelectedIndexChanged
 *
 * disconnects callback to function for on selected index changed
 */
void TgComboBox::disconnectOnSelectedIndexChanged()
{
    TG_FUNCTION_BEGIN();
    m_private->disconnectOnSelectedIndexChanged();
    TG_FUNCTION_END();
}
