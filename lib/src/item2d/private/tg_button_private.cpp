/*!
 * \file
 * \brief file tg_button_private.cpp
 *
 * it holds general TgButtonPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_button_private.h"
#include <cmath>
#include "../../global/tg_global_application.h"
#include "../../global/tg_global_log.h"
#include "../tg_item2d.h"
#include "../../shader/tg_shader_2d.h"
#include "../../window/tg_mainwindow_private.h"
#include "../../global/private/tg_global_defines.h"

TgButtonPrivate::TgButtonPrivate(TgItem2d *currentItem, const char *buttonText) :
    m_currentItem(currentItem),
    m_imageFileNames({std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-normal-not-selected.png"),
                      std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-normal-not-selected-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-normal-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-normal-selected-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-down-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-down-not-selected-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-down-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-down-selected-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/button/prj-tg-ui-lib-button-disabled.png")
    }),
    m_buttonDown(false),
    m_backgroundImage(currentItem, getImageFileName().c_str()),
    m_textfield(currentItem, buttonText, "", 21, 0, 0, 0)
{
    TG_FUNCTION_BEGIN();
    currentItem->setCanSelect(true);
    currentItem->m_private->setInternalCallbacks(this);
    m_textfield.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
    m_textfield.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
    TG_FUNCTION_END();
}

TgButtonPrivate::~TgButtonPrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::setImageCropPosition
 *
 * set image's crop position, all values must be under 1
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * imageCropLeft = 0.2 and imageCropTop = 0.2, imageCropRight = 0.2, imageCropBottom = 0.2
 * then setImageAreaSize's leftAreaSize = 10, topAreaSize = 10, rightAreaSize = 10, bottomAreaSize = 10
 * -> top-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * -> left area on UI is 10x180 pixels, and from original image it's 72.4x219.2 pixels (60% of 362)
 * -> bottom-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * center and right size of the area goes similar way.
 *
 * \param imageCropLeft percent the image's crop left area (must be under 1)
 * \param imageCropTop percent the image's crop top area (must be under 1)
 * \param imageCropRight percent the image's crop right area (must be under 1)
 * \param imageCropBottom percent the image's crop bottom area (must be under 1)
 */
void TgButtonPrivate::setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom)
{
    TG_FUNCTION_BEGIN();
    m_backgroundImage.setImageCropPosition(imageCropLeft, imageCropTop, imageCropRight, imageCropBottom);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::setImageAreaSize
 *
 * set image's crop positions, all values are pixels
 *
 * For example, if button's size is 100x200, and button image's orignal size is 362x362
 *
 * setImageCropPosition's imageCropLeft = 0.2 and imageCropTop = 0.2, imageCropRight = 0.2, imageCropBottom = 0.2
 * then leftAreaSize = 10, topAreaSize = 10, rightAreaSize = 10, bottomAreaSize = 10
 * -> top-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * -> left area on UI is 10x180 pixels, and from original image it's 72.4x219.2 pixels (60% of 362)
 * -> bottom-left corner on UI is 10x10 pixels, and from original image it's 72.4x72.4 pixels (20% of 362)
 * center and right size of the area goes similar way.
 *
 * \param leftAreaSize left area's pixel size
 * \param topAreaSize top area's pixel size
 * \param rightAreaSize right area's pixel size
 * \param bottomAreaSize bottom area's pixel size
 */
void TgButtonPrivate::setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize)
{
    TG_FUNCTION_BEGIN();
    m_backgroundImage.setImageAreaSize(leftAreaSize, topAreaSize, rightAreaSize, bottomAreaSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::getImageFileName
 *
 * get button image file name
 *
 * \return get image filename
 */
std::string TgButtonPrivate::getImageFileName()
{
    TgButton *button = reinterpret_cast<TgButton *>(m_currentItem);
    if (!button->getEnabled()) {
        return m_imageFileNames[TgButtonState::ButtonStateDisabled];
    }
    if (m_buttonDown) {
        if (button->getMouseCursorOnHover() && !button->getSelected()) {
            return m_imageFileNames[TgButtonState::ButtonStateDownHover];
        }
        if (button->getMouseCursorOnHover() && button->getSelected()) {
            return m_imageFileNames[TgButtonState::ButtonStateDownSelectedHover];
        }
        if (!button->getMouseCursorOnHover() && button->getSelected()) {
            return m_imageFileNames[TgButtonState::ButtonStateDownSelected];
        }
        return m_imageFileNames[TgButtonState::ButtonStateDown];
    }

    if (button->getMouseCursorOnHover() && !button->getSelected()) {
        return m_imageFileNames[TgButtonState::ButtonStateNormalHover];
    }
    if (button->getMouseCursorOnHover() && button->getSelected()) {
        return m_imageFileNames[TgButtonState::ButtonStateNormalSelectedHover];
    }
    if (!button->getMouseCursorOnHover() && button->getSelected()) {
        return m_imageFileNames[TgButtonState::ButtonStateNormalSelected];
    }
    return m_imageFileNames[TgButtonState::ButtonStateNormal];
}

/*!
 * \brief TgButtonPrivate::render
 *
 * Renders the image
 * \param windowInfo
 * \param currentItem
 */
void TgButtonPrivate::render(const TgWindowInfo *windowInfo, TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    (void)windowInfo;
    (void)currentItem;
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgButtonPrivate::checkPositionValues(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    if (!currentItem->getPositionChanged()) {
        TG_FUNCTION_END();
        return;
    }

    currentItem->setPositionChanged(false);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::changeButtonImageFile
 *
 * Sets new images for button
 */

void TgButtonPrivate::changeButtonImageFile()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_backgroundImage.setImage(getImageFileName().c_str());
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::changeDownProperty
 *
 * set button's property to down (pressed) or up (released)
 *
 * \param down true if button is set down
 */
void TgButtonPrivate::changeDownProperty(bool down)
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
 * \brief TgButtonPrivate::setImage
 *
 * set button image's for different states
 *
 * \param state button state of image
 * \param filename full file path for image
 */
void TgButtonPrivate::setImage(TgButtonState state, const char *filename)
{
    TG_FUNCTION_BEGIN();
    if (TgButtonState::ButtonStateCount == state) {
        TG_ERROR_LOG("Incorrect button state: TgButtonState::ButtonStateCount");
        TG_FUNCTION_END();
        return;
    }
    m_mutex.lock();
    if (m_imageFileNames[state] != filename) {
        m_imageFileNames[state] = filename;
        m_currentItem->setPositionChanged(true);
        m_backgroundImage.setImage(getImageFileName().c_str());
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::getButtonDown
 *
 * \return true if button is pressed down
 */
bool TgButtonPrivate::getButtonDown() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_buttonDown;
}

/*!
 * \brief TgButtonPrivate::setFontSize
 *
 * \return true if button is pressed down
 */
void TgButtonPrivate::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_textfield.setFontSize(fontSize);
    TG_FUNCTION_END();
}


/*!
 * \brief TgButtonPrivate::onSelectedCallback
 *
 * this is private callback from TgItem2dPrivate
 * when selected is changed this is called
 */
void TgButtonPrivate::onSelectedCallback()
{
    TG_FUNCTION_BEGIN();
    if (!m_currentItem->getSelected()) {
        changeDownProperty(false);
    }
    changeButtonImageFile();
    TG_FUNCTION_END();
}


