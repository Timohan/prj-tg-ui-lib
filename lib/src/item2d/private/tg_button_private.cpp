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
    m_imageTopLeft(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageTopMiddle(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageTopRight(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageMiddleLeft(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageMiddleMiddle(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageMiddleRight(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageBottomLeft(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageBottomMiddle(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_imageBottomRight(currentItem, 0, 0, 100, 100, getImageFileName().c_str()),
    m_textfield(currentItem, buttonText, "", 21, 0, 0, 0),
    m_imageCropLeft(0.0f),
    m_imageCropTop(0.0f),
    m_imageCropRight(0.0f),
    m_imageCropBottom(0.0f),
    m_leftAreaSize(0.0f),
    m_topAreaSize(0.0f),
    m_rightAreaSize(0.0f),
    m_bottomAreaSize(0.0f)
{
    TG_FUNCTION_BEGIN();
    currentItem->setCanSelect(true);
    currentItem->m_private->setInternalCallbacks(this);
    setImageCropPosition(0.2f, 0.2f, 0.2f, 0.2f);
    setImageAreaSize(10.0f, 10.0f, 10.0f, 10.0f);
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
    if (std::fabs(m_imageCropLeft - imageCropLeft) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCropTop - imageCropTop) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCropRight - imageCropRight) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_imageCropBottom - imageCropBottom) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_imageCropLeft = imageCropLeft;
    m_imageCropTop = imageCropTop;
    m_imageCropRight = imageCropRight;
    m_imageCropBottom = imageCropBottom;
    m_currentItem->setPositionChanged(true);
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
    if (std::fabs(m_leftAreaSize - leftAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_topAreaSize - topAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_rightAreaSize - rightAreaSize) <= std::numeric_limits<double>::epsilon()
        || std::fabs(m_bottomAreaSize - bottomAreaSize) <= std::numeric_limits<double>::epsilon()) {
        TG_FUNCTION_END();
        return;
    }
    m_leftAreaSize = leftAreaSize;
    m_topAreaSize = topAreaSize;
    m_rightAreaSize = rightAreaSize;
    m_bottomAreaSize = bottomAreaSize;
    m_currentItem->setPositionChanged(true);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButtonPrivate::setImagePositions
 *
 * set image positions for button images based by crop and image area size
 */
void TgButtonPrivate::setImagePositions()
{
    TG_FUNCTION_BEGIN();
    // top
    m_imageTopLeft.setTextureST(0, 0,
                                m_imageCropLeft, 0,
                                m_imageCropLeft, m_imageCropTop,
                                0, m_imageCropTop);
    m_imageTopLeft.setWidth(m_leftAreaSize);
    m_imageTopLeft.setHeight(m_topAreaSize);

    m_imageTopMiddle.setTextureST(m_imageCropLeft, 0,
                                  1.0f-m_imageCropRight, 0,
                                  1.0f-m_imageCropRight, m_imageCropTop,
                                  m_imageCropLeft, m_imageCropTop);
    m_imageTopMiddle.setX(m_leftAreaSize);
    m_imageTopMiddle.setY(0);
    m_imageTopMiddle.setWidth(m_currentItem->getWidth()-m_leftAreaSize-m_rightAreaSize);
    m_imageTopMiddle.setHeight(m_topAreaSize);

    m_imageTopRight.setTextureST(1.0f-m_imageCropRight, 0,
                                 1.0f, 0,
                                 1.0f, m_imageCropTop,
                                 1.0f-m_imageCropRight, m_imageCropTop);
    m_imageTopRight.setX(m_currentItem->getWidth()-m_rightAreaSize);
    m_imageTopRight.setY(0);
    m_imageTopRight.setWidth(m_rightAreaSize);
    m_imageTopRight.setHeight(m_topAreaSize);

    // middle
    m_imageMiddleLeft.setTextureST(0, m_imageCropTop,
                                   m_imageCropLeft, m_imageCropTop,
                                   m_imageCropLeft, 1.0f-m_imageCropBottom,
                                   0, 1.0f-m_imageCropBottom);
    m_imageMiddleLeft.setX(0);
    m_imageMiddleLeft.setY(m_topAreaSize);
    m_imageMiddleLeft.setWidth(m_leftAreaSize);
    m_imageMiddleLeft.setHeight(m_currentItem->getHeight()-m_topAreaSize-m_bottomAreaSize);

    m_imageMiddleMiddle.setTextureST(m_imageCropLeft, m_imageCropTop,
                                  1.0f-m_imageCropRight, m_imageCropTop,
                                  1.0f-m_imageCropRight, 1.0f-m_imageCropBottom,
                                  m_imageCropLeft, 1.0f-m_imageCropBottom);
    m_imageMiddleMiddle.setX(m_leftAreaSize);
    m_imageMiddleMiddle.setY(m_topAreaSize);
    m_imageMiddleMiddle.setWidth(m_currentItem->getWidth()-m_leftAreaSize-m_rightAreaSize);
    m_imageMiddleMiddle.setHeight(m_currentItem->getHeight()-m_topAreaSize-m_bottomAreaSize);

    m_imageMiddleRight.setTextureST(1.0f-m_imageCropRight, m_imageCropTop,
                                 1.0f, m_imageCropTop,
                                 1.0f, 1.0f-m_imageCropBottom,
                                 1.0f-m_imageCropRight, 1.0f-m_imageCropBottom);
    m_imageMiddleRight.setX(m_currentItem->getWidth()-m_rightAreaSize);
    m_imageMiddleRight.setY(m_topAreaSize);
    m_imageMiddleRight.setWidth(m_rightAreaSize);
    m_imageMiddleRight.setHeight(m_currentItem->getHeight()-m_topAreaSize-m_bottomAreaSize);

    //bottom
    m_imageBottomLeft.setTextureST(0, 1.0f-m_imageCropBottom,
                                   m_imageCropLeft, 1.0f-m_imageCropBottom,
                                   m_imageCropLeft, 1,
                                   0, 1);
    m_imageBottomLeft.setX(0);
    m_imageBottomLeft.setY(m_currentItem->getHeight()-m_bottomAreaSize);
    m_imageBottomLeft.setWidth(m_leftAreaSize);
    m_imageBottomLeft.setHeight(m_bottomAreaSize);

    m_imageBottomMiddle.setTextureST(m_imageCropLeft, 1.0f-m_imageCropBottom,
                                    1.0f-m_imageCropRight, 1.0f-m_imageCropBottom,
                                    1.0f-m_imageCropRight, 1.0f,
                                    m_imageCropLeft, 1.0f);
    m_imageBottomMiddle.setX(m_leftAreaSize);
    m_imageBottomMiddle.setY(m_currentItem->getHeight()-m_bottomAreaSize);
    m_imageBottomMiddle.setWidth(m_currentItem->getWidth()-m_leftAreaSize-m_rightAreaSize);
    m_imageBottomMiddle.setHeight(m_bottomAreaSize);

    m_imageBottomRight.setTextureST(1.0f-m_imageCropRight, 1.0f-m_imageCropBottom,
                                 1.0f, 1.0f-m_imageCropBottom,
                                 1.0f, 1.0f,
                                 1.0f-m_imageCropRight, 1.0f);
    m_imageBottomRight.setX(m_currentItem->getWidth()-m_rightAreaSize);
    m_imageBottomRight.setY(m_currentItem->getHeight()-m_bottomAreaSize);
    m_imageBottomRight.setWidth(m_rightAreaSize);
    m_imageBottomRight.setHeight(m_bottomAreaSize);
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

    setImagePositions();
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
    m_imageTopLeft.setImage(getImageFileName().c_str());
    m_imageTopMiddle.setImage(getImageFileName().c_str());
    m_imageTopRight.setImage(getImageFileName().c_str());
    m_imageMiddleLeft.setImage(getImageFileName().c_str());
    m_imageMiddleMiddle.setImage(getImageFileName().c_str());
    m_imageMiddleRight.setImage(getImageFileName().c_str());
    m_imageBottomLeft.setImage(getImageFileName().c_str());
    m_imageBottomMiddle.setImage(getImageFileName().c_str());
    m_imageBottomRight.setImage(getImageFileName().c_str());
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
        m_imageTopLeft.setImage(getImageFileName().c_str());
        m_imageTopMiddle.setImage(getImageFileName().c_str());
        m_imageTopRight.setImage(getImageFileName().c_str());
        m_imageMiddleLeft.setImage(getImageFileName().c_str());
        m_imageMiddleMiddle.setImage(getImageFileName().c_str());
        m_imageMiddleRight.setImage(getImageFileName().c_str());
        m_imageBottomLeft.setImage(getImageFileName().c_str());
        m_imageBottomMiddle.setImage(getImageFileName().c_str());
        m_imageBottomRight.setImage(getImageFileName().c_str());
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

/*!
 * \brief TgButtonPrivate::setText
 *
 * set button text
 * \param text button text
 */
void TgButtonPrivate::setText(const char *text)
{
    TG_FUNCTION_BEGIN();
    m_textfield.setText(text);
    TG_FUNCTION_END();
}


