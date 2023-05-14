/*!
 * \file
 * \brief file tg_combo_box.h
 *
 * ComboBox
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_COMBO_BOX_H
#define TG_COMBO_BOX_H

#include "tg_mouse_capture.h"
#include "../global/tg_global_macros.h"
#include "tg_textfield.h"
#include <cstddef>
#include <string>
#include <stdint.h>

class TgComboBoxPrivate;

/*!
 * \brief TgComboboxState
 * combobox state
 */
enum TgComboBoxState
{
    ComboBoxStateNormal = 0,          /*!< normal state */
    ComboBoxStateNormalHover,         /*!< mouse cursor is on combobox */
    ComboBoxStateNormalSelected,      /*!< combobox is selected */
    ComboBoxStateNormalSelectedHover, /*!< combobox is selected & mouse cursor is hovering */
    ComboBoxStateDown,                /*!< combobox is pressed down */
    ComboBoxStateDownHover,           /*!< combobox is pressed down & mouse cursor is on combobox */
    ComboBoxStateDownSelected,        /*!< combobox is pressed down & combobox is selected */
    ComboBoxStateDownSelectedHover,   /*!< combobox is pressed down & combobox is selected & mouse cursor is hovering */
    ComboBoxStateDisabled,            /*!< combobox is disabled */
    ComboBoxStateCount,               /*!< combobox state count (not used) */
};

/*!
 * \brief TgComboBox
 * combobox class
 */
class TG_MAINWINDOW_EXPORT TgComboBox : public TgMouseCapture
{
public:
    explicit TgComboBox(TgItem2d *parent);
    explicit TgComboBox(TgItem2d *parent, float x, float y, float width, float height);
    ~TgComboBox();

    void addItemText(const char *text, size_t index = UINT64_MAX);
    std::string getItemText(size_t index);
    size_t getItemCount() const;
    bool removeItemText(size_t index);
    size_t getCurrentIndex() const;
    bool setCurrentIndex(size_t index);
    void connectOnSelectedIndexChanged(std::function<void(size_t)> selectedIndexChanged);
    void disconnectOnSelectedIndexChanged();

protected:
    virtual void onHoverChanged(bool hover) override;
    virtual void onDownChanged(bool down) override;
    virtual void handlePrivateMessage(const TgItem2dPrivateMessage *message) override;
    virtual TgEventResult handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo) override;

private:
    TgComboBoxPrivate *m_private;
};

#endif // TG_COMBO_BOX_H
