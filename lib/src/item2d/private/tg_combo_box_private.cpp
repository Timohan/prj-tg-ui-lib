/*!
 * \file
 * \brief file tg_combo_box_private.cpp
 *
 * it holds general TgComboBoxPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_combo_box_private.h"
#include <cmath>
#include <thread>
#include "../../global/tg_global_log.h"
#include "../../global/private/tg_global_defines.h"
#include "../tg_menu_item.h"
#include "../../global/private/tg_global_deleter.h"
#include "../../global/private/tg_global_wait_renderer.h"
#include "tg_menu_item_private.h"

TgComboBoxPrivate::TgComboBoxPrivate(TgItem2d *currentItem) :
    m_currentItem(currentItem),
    m_backgroundImageLeft(currentItem, 0, 0, currentItem->getWidth()-20, currentItem->getHeight(), std::string(std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-normal-not-selected.png")).c_str() ),
    m_backgroundImageRight(currentItem, currentItem->getWidth()-20, 0, 20, currentItem->getHeight(), std::string(std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-normal-not-selected.png")).c_str() ),
    m_textField(currentItem, 10, 0, currentItem->getWidth()-30, currentItem->getHeight(), "", "", 20.0f, 0, 0, 0),
    m_imageFileNamesLeft{std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-normal-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-hover-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-normal-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-hover-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-down-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-down-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-down-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-down-hover-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-left-disabled.png")
    },
    m_imageFileNamesRight{std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-normal-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-hover-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-normal-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-hover-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-down-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-down-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-down-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-down-hover-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/combobox/prj-tg-ui-lib-combobox-disabled.png")
    }
{
    TG_FUNCTION_BEGIN();
    const float areaSize = 6.0f;
    const float cropPosition = 0.2f;
    m_backgroundImageLeft.setImageCropPosition(cropPosition, cropPosition, cropPosition, cropPosition);
    m_backgroundImageLeft.setImageAreaSize(areaSize, areaSize, areaSize, areaSize);

    m_backgroundImageRight.setImageCropPosition(cropPosition, cropPosition, cropPosition, cropPosition);
    m_backgroundImageRight.setImageAreaSize(areaSize, areaSize, areaSize, areaSize);

    m_textField.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);

    currentItem->setCanSelect(true);
    currentItem->m_private->setInternalCallbacks(this);
    changeButtonImageFile();
    TG_FUNCTION_END();
}

TgComboBoxPrivate::~TgComboBoxPrivate()
{
    m_mutex.lock();
    for (size_t i=0;i<m_listItems.size();i++) {
        delete m_listItems[i];
    }
    m_listItems.clear();
    m_mutex.unlock();
}

/*!
 * \brief TgComboBoxPrivate::addItemText
 *
 * adds text to combobox
 *
 * \param text text to index
 * \param index index to add, if UINT64_MAX, then it adds to last index
 */
void TgComboBoxPrivate::addItemText(const char *text, size_t index)
{
    TG_FUNCTION_BEGIN();
    TgShortCut shortCut;
    TgMenuItem *menuItem = nullptr;
    m_mutex.lock();
    size_t cmp_index = 0;
    bool indexAddIsBeforeCurrentIndex = true;
    bool needHideTheList = false;
    for (size_t i=0;i<m_listItems.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listItems[i])->m_private->getDeleting()) {
            continue;
        }
        if (m_listItems[i]->m_private->getMenuRendering()) {
            needHideTheList = true;
        }
        if (cmp_index == index) {
            menuItem = new TgMenuItem(nullptr, nullptr, text, &shortCut);
            m_listItems.insert(m_listItems.begin()+i, menuItem);
            break;
        }
        if (cmp_index == m_selectedIndex) {
            indexAddIsBeforeCurrentIndex = false;
        }
        cmp_index++;
    }
    if (!menuItem) {
        menuItem = new TgMenuItem(nullptr, nullptr, text, &shortCut);
        m_listItems.push_back(menuItem);
    }
    menuItem->m_private->setMenuType(TgMenuItemPrivate::MenuType::MenuType_ComboBoxMenu);
    menuItem->setVisible(true);
    reinterpret_cast<TgItem2d *>(menuItem)->m_private->parentVisibleChanged(true);
    reinterpret_cast<TgMouseCapture *>(menuItem)->m_private->setMouseCaptureInternalCallback(this);

    if (m_listItems.size() == 1) {
        setCurrentIndex(0, false);
        m_listItems[0]->setBackgroundColor(200, 200, 200, MENU_ITEM_DEFAULT_BACKGROUND_A);
    } else if (indexAddIsBeforeCurrentIndex) {
        setCurrentIndex(m_selectedIndex+1, false, true);
    }
    if (needHideTheList) {
        TgItem2dPrivateMessage msgHideTheList;
        msgHideTheList.m_type = TgItem2dPrivateMessageType::EventHideTheList;
        m_currentItem->sendMessageToChildrenFromBegin(&msgHideTheList);
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBoxPrivate::onClickedCallback
 *
 * \param currentItem get text of index
 */
void TgComboBoxPrivate::onClickedCallback(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    size_t tmpIndex = 0;
    m_mutex.lock();
    for (size_t i=0;i<m_listItems.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listItems[i]) == currentItem) {
            setCurrentIndex(tmpIndex, false);
            break;
        }
        if (!reinterpret_cast<TgItem2d *>(m_listItems[i])->m_private->getDeleting()) {
            tmpIndex++;
        }
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBoxPrivate::getItemText
 *
 * \param get text of index
 * \return text by the index
 */
std::string TgComboBoxPrivate::getItemText(size_t index, bool useLock)
{
    TG_FUNCTION_BEGIN();
    if (useLock) {
        m_mutex.lock();
    }
    if (index >= m_listItems.size()) {
        if (useLock) {
            m_mutex.unlock();
        }
        TG_FUNCTION_END();
        return "";
    }
    TgMenuItem *menuItem = getMenuItem(index);
    if (!menuItem) {
        if (useLock) {
            m_mutex.unlock();
        }
        TG_FUNCTION_END();
        return "";
    }
    std::string ret = menuItem->getText();
    if (useLock) {
        m_mutex.unlock();
    }
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgComboBoxPrivate::getItemCount
 *
 * \return number of items of combobox
 */
size_t TgComboBoxPrivate::getItemCount(bool useLock)
{
    size_t ret = 0;
    if (useLock) {
        m_mutex.lock();
    }
    for (size_t i=0;i<m_listItems.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listItems[i])->m_private->getDeleting()) {
            continue;
        }
        ret++;
    }
    if (useLock) {
        m_mutex.unlock();
    }
    return ret;
}

/*!
 * \brief TgComboBoxPrivate::removeItemText
 *
 * remove text by index
 *
 * \param index to remove
 * \return true if remove was correct, false if index is larger than
 * number of items in the list
 */
bool TgComboBoxPrivate::removeItemText(size_t index)
{
    m_mutex.lock();
    if (index >= m_listItems.size()) {
        m_mutex.unlock();
        return false;
    }
    TgMenuItem *menuItem = TgComboBoxPrivate::getMenuItem(index);
    if (!menuItem) {
        m_mutex.unlock();
        return false;
    }
    TG_DEBUG_LOG("Removing combobox item index: ", index);
    if (menuItem->m_private->getMenuRendering()) {
        TgItem2dPrivateMessage msgHideTheList;
        msgHideTheList.m_type = TgItem2dPrivateMessageType::EventHideTheList;
        m_currentItem->sendMessageToChildrenFromBegin(&msgHideTheList);
    }
    TgGlobalDeleter::getInstance()->addComboBoxMenu(this, menuItem);
    reinterpret_cast<TgItem2d *>(menuItem)->m_private->setDeleting();

    if (index <= getCurrentIndex()) {
        if (getCurrentIndex() > 0 && getItemCount(false) > 0) {
            setCurrentIndex(getCurrentIndex()-1, false, true);
        } else {
            setCurrentIndex(0, false, true);
        }
    }
    m_mutex.unlock();
    return true;
}

/*!
 * \brief TgComboBoxPrivate::deleteMenu
 *
 * When Combobox item is going to be really deleted, this function is called
 *
 * \param item
 */
void TgComboBoxPrivate::deleteMenu(TgMenuItem *item)
{
    m_mutex.lock();
    size_t i;
    for (i=0;i<m_listItems.size();i++) {
        if (m_listItems[i] == item) {
            delete m_listItems[i];
            m_listItems.erase(m_listItems.begin()+i);
            break;
        }
    }
    m_mutex.unlock();
}

/*!
 * \brief TgComboBoxPrivate::getCurrentIndex
 *
 * get combobox's index
 *
 * \return index of combobox
 */
size_t TgComboBoxPrivate::getCurrentIndex() const
{
    return m_selectedIndex;
}

/*!
 * \brief TgComboBoxPrivate::getMenuItem
 *
 * \return menu item pointer
 * nullptr if not found
 */
TgMenuItem *TgComboBoxPrivate::getMenuItem(size_t index)
{
    TG_FUNCTION_BEGIN();
    size_t cmp_index = 0;
    for (size_t i=0;i<m_listItems.size();i++) {
        if (reinterpret_cast<TgItem2d *>(m_listItems[i])->m_private->getDeleting()) {
            continue;
        }
        if (cmp_index == index) {
            return m_listItems[i];
        }
        cmp_index++;
    }
    TG_FUNCTION_END();
    return nullptr;
}

/*!
 * \brief TgComboBoxPrivate::setCurrentIndex
 *
 * Set combobox's index
 *
 * \param index
 * \return true if set was correct, false if index is larger than
 * number of items in the list
 */
bool TgComboBoxPrivate::setCurrentIndex(size_t index, bool useLock, bool forceSendChangeIndex)
{
    TG_FUNCTION_BEGIN();
    if (useLock) {
        m_mutex.lock();
    }

    if (index >= getItemCount(false)) {
        if (getItemCount(false) == 0) {
            m_textField.setText("");
        }
        if (useLock) {
            m_mutex.unlock();
        }
        return false;
    }
    bool ignoreSendIndex = m_selectedIndex == index;
    m_selectedIndex = index;
    m_textField.setText(getItemText(index, false).c_str());
    if (!ignoreSendIndex || forceSendChangeIndex == true) {
        std::thread([this, index]() {
            if (f_selectedIndexChanged) {
                f_selectedIndexChanged(index);
            }
        }).detach();
        TgMenuItem *menuItem = getMenuItem(index);
        if (menuItem) {
            std::vector<TgMenuItem *>::iterator it;
            for (it=m_listItems.begin();it!=m_listItems.end();it++) {
                if (menuItem == (*it)) {
                    (*it)->setBackgroundColor(200,
                                              200,
                                              200,
                                              MENU_ITEM_DEFAULT_BACKGROUND_A);
                } else {
                    (*it)->setBackgroundColor(MENU_ITEM_DEFAULT_BACKGROUND_R,
                                              MENU_ITEM_DEFAULT_BACKGROUND_G,
                                              MENU_ITEM_DEFAULT_BACKGROUND_B,
                                              MENU_ITEM_DEFAULT_BACKGROUND_A);
                }
            }
        }
    }
    if (useLock) {
        m_mutex.unlock();
    }
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgComboBoxPrivate::changeDownProperty
 *
 * set button's property to down (pressed) or up (released)
 *
 * \param down true if button is set down
 */
void TgComboBoxPrivate::changeDownProperty(bool down)
{
    TG_FUNCTION_BEGIN();
    if (m_buttonDown == down) {
        TG_FUNCTION_END();
        return;
    }
    m_buttonDown = down;
    if (down && m_currentItem->getCanSelect()) {
        m_currentItem->setSelected(true);
    }
    changeButtonImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBoxPrivate::onSelectedCallback
 *
 * this is private callback from TgItem2dPrivate
 * when selected is changed this is called
 */
void TgComboBoxPrivate::onSelectedCallback()
{
    TG_FUNCTION_BEGIN();
    if (!m_currentItem->getSelected()) {
        changeDownProperty(false);
    }
    changeButtonImageFile();
    TG_FUNCTION_END();
}

void TgComboBoxPrivate::changeButtonImageFile()
{
    TG_FUNCTION_BEGIN();
    std::string imageFileNameLeft;
    std::string imageFileNameRight;
    m_mutex.lock();
    getImageFileName(imageFileNameLeft, imageFileNameRight);
    bool imageLeftChanged = m_backgroundImageLeft.setImage(imageFileNameLeft.c_str());
    bool imageRightChanged = m_backgroundImageRight.setImage(imageFileNameRight.c_str());
    if (imageLeftChanged || imageRightChanged) {
        TgGlobalWaitRenderer::getInstance()->release(DEFAULT_RENDER_WAIT_MAX_TIMEOUT);
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBoxPrivate::getImageFileName
 *
 * get button image file name
 *
 * \param imageFileNameLeft image filename for left (text area)
 * \param imageFileNameRight image filename for right (button area)
 */
void TgComboBoxPrivate::getImageFileName(std::string &imageFileNameLeft, std::string &imageFileNameRight)
{
    TgComboBox *comboBox = reinterpret_cast<TgComboBox *>(m_currentItem);
    if (!comboBox->getEnabled()) {
        imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateDisabled];
        imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateDisabled];
        return;
    }
    if (m_buttonDown) {
        if (comboBox->getMouseCursorOnHover() && !comboBox->getSelected()) {
            imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateDownHover];
            imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateDownHover];
            return;
        }
        if (comboBox->getMouseCursorOnHover() && comboBox->getSelected()) {
            imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateDownSelectedHover];
            imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateDownSelectedHover];
            return;
        }
        if (!comboBox->getMouseCursorOnHover() && comboBox->getSelected()) {
            imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateDownSelected];
            imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateDownSelected];
            return;
        }
        imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateDown];
        imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateDown];
        return;
    }

    if (comboBox->getMouseCursorOnHover() && !comboBox->getSelected()) {
        imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateNormalHover];
        imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateNormalHover];
        return;
    }
    if (comboBox->getMouseCursorOnHover() && comboBox->getSelected()) {
        imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateNormalSelectedHover];
        imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateNormalSelectedHover];
        return;
    }
    if (!comboBox->getMouseCursorOnHover() && comboBox->getSelected()) {
        imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateNormalSelected];
        imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateNormalSelected];
        return;
    }
    imageFileNameLeft = m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateNormal];
    imageFileNameRight = m_imageFileNamesRight[TgComboBoxState::ComboBoxStateNormal];
}

/*!
 * \brief TgComboBox::handleEventComboBox
 *
 * handles the event
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
TgEventResult TgComboBoxPrivate::handleEventComboBox(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (eventData->m_type == TgEventType::EventTypeMousePress
        && eventData->m_event.m_mouseEvent.m_mouseType == TgMouseType::ButtonLeft
        && m_currentItem->getVisible()
        && m_currentItem->getEnabled()
        && getItemCount(false)
        && m_currentItem->m_private->isCursorOnItem(eventData->m_event.m_mouseEvent.m_x, eventData->m_event.m_mouseEvent.m_y, windowInfo)) {
        // open normal item menu with right mouse click
        TgItem2dMenu::setMenuItemsOpen(false, m_currentItem, nullptr, m_listItems, eventData, windowInfo, getCurrentIndex());
        m_mutex.unlock();
        m_currentItem->setSelected(true);
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeKeyPress
        && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_Key_Down
        && m_currentItem->getVisible()
        && m_currentItem->getSelected()
        && m_currentItem->getEnabled()) {
        setCurrentIndex(getCurrentIndex()+1, false, false);
        m_mutex.unlock();
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeKeyPress
        && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_Key_Up
        && m_currentItem->getVisible()
        && m_currentItem->getSelected()
        && m_currentItem->getEnabled()) {
        if (getCurrentIndex() > 0) {
            setCurrentIndex(getCurrentIndex()-1, false, false);
        }
        m_mutex.unlock();
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeCharacterCallback
        && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_NormalKey
        && eventData->m_event.m_keyEvent.m_key == 32
        && m_currentItem->getVisible()
        && m_currentItem->getSelected()
        && m_currentItem->getEnabled()) {
        TgItem2dMenu::setMenuItemsOpen(false, m_currentItem, nullptr, m_listItems, eventData, windowInfo);
        m_mutex.unlock();
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
    return TgEventResult::EventResultNotCompleted;
}

/*!
 * \brief TgComboBoxPrivate::connectOnSelectedIndexChanged
 *
 * connects callback on combobox selected index change
 * \param selectedIndexChanged
 */
void TgComboBoxPrivate::connectOnSelectedIndexChanged(std::function<void(size_t)> selectedIndexChanged)
{
    TG_FUNCTION_BEGIN();
    f_selectedIndexChanged = selectedIndexChanged;
    TG_FUNCTION_END();
}

/*!
 * \brief TgComboBoxPrivate::disconnectOnSelectedIndexChanged
 *
 * disconnects callback on combobox selected index change
 */
void TgComboBoxPrivate::disconnectOnSelectedIndexChanged()
{
    TG_FUNCTION_BEGIN();
    f_selectedIndexChanged = nullptr;
    TG_FUNCTION_END();
}

