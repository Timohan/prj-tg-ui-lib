/*!
 * \file
 * \brief file tg_combo_box_private.h
 *
 * it holds general TgComboBoxPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_COMBO_BOX_PRIVATE_H
#define TG_COMBO_BOX_PRIVATE_H

#include "../tg_textfield.h"
#include "../tg_image_part.h"
#include "item2d/tg_item2d_private.h"
#include "../tg_combo_box.h"
#include "mouse_capture/tg_mouse_capture_private.h"
#include <string>
#include <vector>
#include <mutex>
#include <functional>

class TgItem2d;
struct TgWindowInfo;
class TgMenuItem;

class TgComboBoxPrivate : private TgItem2dInternalCallback, public TgMouseCaptureInternalCallback
{
public:
    explicit TgComboBoxPrivate(TgItem2d *currentItem);
    virtual ~TgComboBoxPrivate();

    void addItemText(const char *text, size_t index = UINT64_MAX);
    std::string getItemText(size_t index, bool useLock = true);
    size_t getItemCount(bool useLock);
    bool removeItemText(size_t index);
    size_t getCurrentIndex() const;
    bool setCurrentIndex(size_t index, bool useLock = true, bool forceSendChangeIndex = false, const TgWindowInfo *windowInfo = nullptr);

    void changeButtonImageFile();
    void changeDownProperty(bool down);
    void getImageFileName(std::string &imageFileNameLeft, std::string &imageFileNameRight);
    void deleteMenu(TgMenuItem *item);
    TgEventResult handleEventComboBox(TgEventData *eventData, const TgWindowInfo *windowInfo);
    void connectOnSelectedIndexChanged(std::function<void(size_t)> selectedIndexChanged);
    void disconnectOnSelectedIndexChanged();

private:
    TgItem2d *m_currentItem;
    std::vector<TgMenuItem *>m_listItems;
    size_t m_selectedIndex = 0;
    bool m_buttonDown = false;
    bool m_selectNameCaseSensitive = false;
    TgImagePart m_backgroundImageLeft;
    TgImagePart m_backgroundImageRight;
    TgTextfield m_textField;
    std::mutex m_mutex;
    std::string m_imageFileNamesLeft[TgComboBoxState::ComboBoxStateCount];
    std::string m_imageFileNamesRight[TgComboBoxState::ComboBoxStateCount];
    std::function<void(size_t)> f_selectedIndexChanged;

    virtual void onSelectedCallback() override;
    virtual void onClickedCallback(TgItem2d *currentItem);

    TgMenuItem *getMenuItem(size_t index);
    void setCurrentIndex(const std::string &itemName, const TgWindowInfo *windowInfo);
    bool isSimilarName(const std::string &itemName, const std::string &itemText);
};

#endif // TG_COMBO_BOX_PRIVATE_H
