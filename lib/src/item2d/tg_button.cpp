/*!
 * \file
 * \brief file tg_button.cpp
 *
 * Inits and draws button
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_button.h"
#include "../global/tg_global_log.h"
#include "private/tg_button_private.h"
#include "private/mouse_capture/tg_mouse_capture_private.h"

/*!
 * \brief TgButton::TgButton
 *
 * constructor to use AnchorFollowParentSize
 *
 * \param parent item's parent
 * \param buttonText button text
 */
TgButton::TgButton(TgItem2d *parent, const char *buttonText) :
    TgMouseCapture(parent),
    m_private(new TgButtonPrivate(this, buttonText))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::TgButton
 *
 * constructor to use AnchorRelativeToParent
 *
 * \param parent item's parent
 * \param x item's relative position x (of parent)
 * \param y item's relative position x (of parent)
 * \param width item's width
 * \param height item's height
 * \param buttonText button text
 */
TgButton::TgButton(TgItem2d *parent, float x, float y, float width, float height, const char *buttonText) :
    TgMouseCapture(parent, x, y, width, height),
    m_private(new TgButtonPrivate(this, buttonText))
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}
TgButton::~TgButton()
{
    TG_FUNCTION_BEGIN();
    if (m_private) {
        delete m_private;
        m_private = nullptr;
    }
    TG_FUNCTION_END();
}


/*!
 * \brief TgButton::setImageCropPosition
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
void TgButton::setImageCropPosition(float imageCropLeft, float imageCropTop, float imageCropRight, float imageCropBottom)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageCropPosition(imageCropLeft, imageCropTop, imageCropRight, imageCropBottom);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::setImageAreaSize
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
void TgButton::setImageAreaSize(float leftAreaSize, float topAreaSize, float rightAreaSize, float bottomAreaSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setImageAreaSize(leftAreaSize, topAreaSize, rightAreaSize, bottomAreaSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::render
 *
 * Renders the image
 * \param windowInfo
 * \param parentOpacity
 * \return true if item was rendered, false if
 * item was not render because it was outside or invisible
 */
bool TgButton::render(const TgWindowInfo *, float)
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        TG_FUNCTION_END();
        return false;
    }
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgButton::checkPositionValues
 *
 * Checks position values before rendering starts
 */
void TgButton::checkPositionValues()
{
    TG_FUNCTION_BEGIN();
    if (!getVisible()) {
        return;
    }
    m_private->checkPositionValues(this);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::onHoverChanged
 *
 * when hover state is changed, this is called
 *
 * \param hover true if cursor is on button
 */
void TgButton::onHoverChanged(bool)
{
    TG_FUNCTION_BEGIN();
    m_private->changeButtonImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::onDownChanged
 *
 * set button's property to down (pressed) or up (released)
 *
 * \param down true if button is set down
 */
void TgButton::onDownChanged(bool down)
{
    TG_FUNCTION_BEGIN();
    m_private->changeDownProperty(down);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::setImage
 *
 * set button image's for different states
 *
 * \param state button state of image
 * \param filename full file path for image
 */
void TgButton::setImage(TgButtonState state, const char *filename)
{
    TG_FUNCTION_BEGIN();
    m_private->setImage(state, filename);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::getButtonDown
 *
 * \return true if button is pressed down
 */
bool TgButton::getButtonDown() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_private->getButtonDown();
}

/*!
 * \brief TgButton::setFontSize
 *
 * set font size for button text
 *
 * \param fontSize new font size
 */
void TgButton::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_private->setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \param windowInfo
 * \return if event result is completed
 */
TgEventResult TgButton::handleEvent(TgEventData *eventData, const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    if (eventData->m_type == TgEventType::EventTypeKeyPress
        && getEnabled() && getSelected()
        && eventData->m_event.m_keyEvent.m_key == ' ') {
        m_private->changeDownProperty(true);
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    if (eventData->m_type == TgEventType::EventTypeKeyRelease
        && getEnabled() && getSelected()
        && eventData->m_event.m_keyEvent.m_key == ' ') {
        if (getButtonDown()) {
            m_private->changeDownProperty(false);
            TgMouseCapture::m_private->sendMouseClickedNoButton();
        }
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }
    TG_FUNCTION_END();
    return TgMouseCapture::handleEvent(eventData, windowInfo);
}

/*!
 * \brief TgButton::setText
 *
 * set button text
 * \param text button text
 */
void TgButton::setText(const char *text)
{
    TG_FUNCTION_BEGIN();
    m_private->m_textfield.setText(text);
    TG_FUNCTION_END();
}

/*!
 * \brief TgButton::setText
 *
 * set button text
 * \param listText button text
 */
void TgButton::setText(const std::vector<TgTextFieldText> &listText)
{
    TG_FUNCTION_BEGIN();
    m_private->m_textfield.setText(listText);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMouseCapture::handlePrivateMessage
 *
 * virtual function to handle private function
 *
 * \param message
 */
void TgButton::handlePrivateMessage(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    TgMouseCapture::handlePrivateMessage(message);
    if (message->m_type == TgItem2dPrivateMessageType::CurrentItemToDisabled) {
        m_private->changeButtonImageFile();
    }
    TG_FUNCTION_END();
}
