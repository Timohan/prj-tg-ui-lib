/*!
 * \file
 * \brief file tg_textfield_private.cpp
 *
 * it holds general TgTexteditPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_textedit_private.h"
#include  <cmath>
#include "../../global/tg_global_application.h"
#include "../../font/cache/tg_font_glyph_cache.h"
#include "../../font/tg_font_default.h"
#include "../../font/tg_font_text_generator.h"
#include "../../font/tg_font_text.h"
#include "../../global/tg_global_log.h"
#include "../tg_item2d.h"
#include "../../window/tg_mainwindow_private.h"
#include "tg_textfield_private.h"

#define TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_R  51
#define TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_G  100
#define TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_B  145

#define TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_R_NOT_SELECTED  35
#define TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_G_NOT_SELECTED  38
#define TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_B_NOT_SELECTED  42

TgTexteditPrivate::TgTexteditPrivate(TgItem2d *currentItem, const char *text, const char *fontFile, float fontSize,
                                     uint8_t r, uint8_t g, uint8_t b) :
    m_marginLeft(3),
    m_marginRight(3),
    m_cursorPressedDown(false),
    m_cursorPosition(0),
    m_selectedTextSize(0),
    m_selectedAreaBackgroundColorSelected{TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_R, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_G, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_B},
    m_selectedAreaBackgroundColorNotSelected{TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_R_NOT_SELECTED, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_G_NOT_SELECTED, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_B_NOT_SELECTED},
    m_currentItem(currentItem),
    m_imageFileNames{std::string(IMAGES_PATH) + std::string("/textedit/prj-tj-ui-lib-textedit-normal-not-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/textedit/prj-tg-ui-lib-textedit-normal-not-selected-hover.png"),
                     std::string(IMAGES_PATH) + std::string("/textedit/prj-tj-gui-lib-textedit-selected.png"),
                     std::string(IMAGES_PATH) + std::string("/textedit/prj-tj-gui-lib-textedit-disabled.png")},
    m_backgroundImage(currentItem, m_imageFileNames[ TgTexteditState::TexteditStateNormal ].c_str()),
    m_selectedAreaBackground(currentItem, 3.0f, 3.0f, 3.0f, currentItem->getHeight()-6, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_R_NOT_SELECTED, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_G_NOT_SELECTED, TG_TEXTEDIT_SELECTED_AREA_BACKGROUND_B_NOT_SELECTED),
    m_textField(currentItem, text, fontFile, fontSize, r, g, b),
    m_cursorPositionImage(currentItem, 3, 3, 4, currentItem->getHeight()-6, std::string(std::string(IMAGES_PATH) + std::string("/textedit/prj-tj-gui-lib-textedit-cursor-position.png")).c_str() )
{
    TG_FUNCTION_BEGIN();
    currentItem->setCanSelect(true);
    currentItem->m_private->setInternalCallbacks(this);
    changeImageFile();
    m_textField.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
    m_textField.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
    m_textField.setMargin(getTextLeftMargin(), 3, getTextRightMargin(), 3);

    m_cursorPositionImage.setType(TgImagePartType::TgImagePartType_Part3_UpToDown);
    m_cursorPositionImage.setImageCropPosition3TopToBottom(4.0f/32.0f, 4.0f/32.0f);
    m_cursorPositionImage.setImageAreaSize3TopToBottom(4.0f, 4.0f);

    TG_FUNCTION_END();
}

TgTexteditPrivate::~TgTexteditPrivate()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::setText
 *
 * \param text text
 */
void TgTexteditPrivate::setText(const char *text)
{
    TG_FUNCTION_BEGIN();
    std::vector<TgTextFieldText> listText;
    TgTextFieldText newText;
    newText.m_text = text;
    newText.m_textColorR = 0;
    newText.m_textColorG = 0;
    newText.m_textColorB = 0;
    listText.push_back(newText);
    setText(listText);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::setText
 *
 * \param listText list of text with different colors
 */
void TgTexteditPrivate::setText(const std::vector<TgTextFieldText> &listText)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    if (!m_textField.m_private->isEqualText(listText)
        || !m_textField.m_private->isEqualTextColor(listText)) {
        m_textField.setText(listText);
        m_selectedTextSize = 0;
        if (static_cast<size_t>(m_cursorPosition) > m_textField.getCharacterCount()) {
            m_cursorPosition = static_cast<int64_t>(m_textField.getCharacterCount());
        }
        m_currentItem->setPositionChanged(true);
    }
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::getText
 *
 * \return text (utf8)
 */
std::string TgTexteditPrivate::getText() const
{
    TG_FUNCTION_BEGIN();
    std::string ret;
    m_mutex.lock();
    ret = m_textField.getText();
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgTexteditPrivate::getTextLeftMargin
 *
 * the default margin between background area of textfield
 * and left text start position in default position
 *
 * \return left text margin
 */
float TgTexteditPrivate::getTextLeftMargin()
{
    return m_marginLeft;
}

/*!
 * \brief TgTexteditPrivate::getTextRightMargin
 *
 * the default margin between background area of textfield
 * and right text start position in default position
 *
 * \return right text margin
 */
float TgTexteditPrivate::getTextRightMargin()
{
    return m_marginRight;
}

/*!
 * \brief TgTexteditPrivate::setTextLeftMargin
 *
 * the default margin between background area of textfield
 * and left text start position in default position
 *
 * \param margin left text margin
 */
void TgTexteditPrivate::setTextLeftMargin(float margin)
{
    m_marginLeft = margin;
}

/*!
 * \brief TgTexteditPrivate::setTextRightMargin
 *
 * the default margin between background area of textfield
 * and right text start position in default position
 *
 * \param margin right text margin
 */
void TgTexteditPrivate::setTextRightMargin(float margin)
{
    m_marginRight = margin;
}

/*!
 * \brief TgTexteditPrivate::checkPositionValues
 *
 * Checks position values before rendering starts
 * \param currentItem
 */
void TgTexteditPrivate::checkPositionValues(TgItem2d *currentItem)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    bool selected = m_currentItem->getSelected();
    if (selected) {
        uint64_t cursorPositionTimer = static_cast<uint64_t>(m_cursorPositionTimer.elapsedTimeFromBegin()*10);
        m_cursorPositionImage.setVisible(cursorPositionTimer % 10 <= 5);
    } else {
        m_cursorPositionImage.setVisible(false);
    }
    if (!currentItem->getPositionChanged() && !m_textField.getPositionChanged()) {
        m_mutex.unlock();
        TG_FUNCTION_END();
        return;
    }
    if (m_selectedTextSize != 0) {
        m_selectedAreaBackground.setVisible(true);
        if (selected) {
            m_selectedAreaBackground.setColor(m_selectedAreaBackgroundColorSelected[0], m_selectedAreaBackgroundColorSelected[1], m_selectedAreaBackgroundColorSelected[2], 255);
        } else {
            m_selectedAreaBackground.setColor(m_selectedAreaBackgroundColorNotSelected[0], m_selectedAreaBackgroundColorNotSelected[1], m_selectedAreaBackgroundColorNotSelected[2], 255);
        }
    } else {
        m_selectedAreaBackground.setVisible(false);
    }
    m_textField.checkPositionValues();

    float positionX = 0;
    float positionEndX = 0;
    if (m_selectedTextSize == 0) {
        m_textField.m_private->getTextPosition(static_cast<size_t>(m_cursorPosition), positionX);
        resetTextFieldPosition(positionX);
        m_cursorPositionImage.setX(m_textField.getX()+positionX-m_cursorPositionImage.getWidth()/2.0f);
    } else if (m_selectedTextSize < 0) {
        m_textField.m_private->getTextPosition(static_cast<size_t>(m_cursorPosition+m_selectedTextSize), positionX);
        m_textField.m_private->getTextPosition(static_cast<size_t>(m_cursorPosition), positionEndX);
        resetTextFieldPosition(positionX);

        m_cursorPositionImage.setX(m_textField.getX()+positionX-m_cursorPositionImage.getWidth()/2.0f);
        m_selectedAreaBackground.setX(m_textField.getX()+positionX);

        m_selectedAreaBackground.setWidth(positionEndX-positionX);
    } else {
        m_textField.m_private->getTextPosition(static_cast<size_t>(m_cursorPosition+m_selectedTextSize), positionEndX);
        m_textField.m_private->getTextPosition(static_cast<size_t>(m_cursorPosition), positionX);

        resetTextFieldPosition(positionEndX);
        m_cursorPositionImage.setX(m_textField.getX()+positionEndX-m_cursorPositionImage.getWidth()/2.0f);
        m_selectedAreaBackground.setX(m_textField.getX()+positionX);

        m_selectedAreaBackground.setWidth(m_cursorPositionImage.getX()-(m_textField.getX()+positionX)+m_cursorPositionImage.getWidth()/2.0f);
    }

    currentItem->setPositionChanged(false);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::setHorizontalAlign
 *
 * set text field's horizontal align
 * \param align horizontal align
 */
void TgTexteditPrivate::setHorizontalAlign(TgTextfieldHorizontalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_textField.setHorizontalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::setVerticalAlign
 *
 * set text field's vertical align
 * \param align vertical align
 */
void TgTexteditPrivate::setVerticalAlign(TgTextfieldVerticalAlign align)
{
    TG_FUNCTION_BEGIN();
    m_textField.setVerticalAlign(align);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::getAlignHorizontal
 *
 * \return get horizontal align
 */
TgTextfieldHorizontalAlign TgTexteditPrivate::getAlignHorizontal() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_textField.getAlignHorizontal();
}

/*!
 * \brief TgTexteditPrivate::getAlignVertical
 *
 * \return get vertical align
 */
TgTextfieldVerticalAlign TgTexteditPrivate::getAlignVertical() const
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return m_textField.getAlignVertical();
}

/*!
 * \brief TgTexteditPrivate::setFontSize
 *
 * \param fontSize fon size for text
 */
void TgTexteditPrivate::setFontSize(float fontSize)
{
    TG_FUNCTION_BEGIN();
    m_textField.setFontSize(fontSize);
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::onSelectedCallback
 *
 * when selected state is changed, this is called
 */
void TgTexteditPrivate::onSelectedCallback()
{
    TG_FUNCTION_BEGIN();
    changeImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::getImageFile
 *
 * get background image file
 * \return background image file
 */
std::string TgTexteditPrivate::getImageFile()
{
    if (!m_currentItem->getEnabled()) {
        return m_imageFileNames[TgTexteditState::TexteditStateDisabled];
    }
    if (m_currentItem->getSelected()) {
        return m_imageFileNames[TgTexteditState::TexteditStateSelected];
    }

    TgTextedit *textedit = reinterpret_cast<TgTextedit *>(m_currentItem);
    if (textedit->getMouseCursorOnHover()) {
        return m_imageFileNames[TgTexteditState::TexteditStateNormalHover];
    }
    return m_imageFileNames[TgTexteditState::TexteditStateNormal];
}

/*!
 * \brief TgTexteditPrivate::changeImageFile
 *
 * Sets new images for button
 */
void TgTexteditPrivate::changeImageFile()
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_backgroundImage.setImage( getImageFile().c_str() );
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::changeDownProperty
 *
 * set button's property to down (pressed) or up (released)
 *
 * \param down true if button is set down
 */
void TgTexteditPrivate::changeDownProperty(bool down)
{
    TG_FUNCTION_BEGIN();
    if (down && m_currentItem->getCanSelect()) {
        m_currentItem->setSelected(true);
    }
    changeImageFile();
    TG_FUNCTION_END();
}

/*!
 * \brief TgTexteditPrivate::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \return if event result is completed
 */
TgEventResult TgTexteditPrivate::handleEvent(TgEventData *eventData)
{
    TG_FUNCTION_BEGIN();
    std::vector<uint32_t> listAddCharacter;
    m_mutex.lock();
    if ((eventData->m_type == TgEventType::EventTypeKeyPress || eventData->m_type == TgEventType::EventTypeKeyRepeat)
        && m_currentItem->getEnabled() && m_currentItem->getSelected()) {
        if (eventData->m_event.m_keyEvent.m_key == 0
            && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_Key_Left) {
            if (eventData->m_event.m_keyEvent.m_pressModsKeyDown & PressModsKeyDown_Shift) {
                if (m_selectedTextSize + m_cursorPosition > 0) {
                    m_selectedTextSize--;
                }
            } else {
                if (m_selectedTextSize) {
                    m_cursorPosition = static_cast<size_t>(m_selectedTextSize+m_cursorPosition);
                } else if (m_cursorPosition > 0) {
                    m_cursorPosition--;
                }
                m_selectedTextSize = 0;
            }
            m_currentItem->setPositionChanged(true);
        } else if (eventData->m_event.m_keyEvent.m_key == 0
                   && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_Key_Right) {
            if (eventData->m_event.m_keyEvent.m_pressModsKeyDown & PressModsKeyDown_Shift) {
                if (m_selectedTextSize + m_cursorPosition < static_cast<int64_t>(m_textField.getCharacterCount())) {
                    m_selectedTextSize++;
                }
            } else {
                if (m_selectedTextSize) {
                    m_cursorPosition = static_cast<size_t>(m_selectedTextSize+m_cursorPosition);
                } else if (static_cast<size_t>(m_cursorPosition) < m_textField.getCharacterCount()) {
                    m_cursorPosition++;
                }
                m_selectedTextSize = 0;
            }
            m_currentItem->setPositionChanged(true);
        } else if (eventData->m_event.m_keyEvent.m_key == 0
                   && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_Key_Backspace) {
            if (m_selectedTextSize) {
                clearText();
            } else if (m_cursorPosition > 0) {
                m_cursorPosition--;
                m_textField.m_private->editText(listAddCharacter, static_cast<size_t>(m_cursorPosition), 1);
            }
            m_currentItem->setPositionChanged(true);
        } else if (eventData->m_event.m_keyEvent.m_key == 0
                   && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_Key_Delete) {
            if (m_selectedTextSize) {
                clearText();
            } else if (static_cast<size_t>(m_cursorPosition) < m_textField.getCharacterCount()) {
                m_textField.m_private->editText(listAddCharacter, static_cast<size_t>(m_cursorPosition), 1);
            }
            m_currentItem->setPositionChanged(true);
        }
        m_mutex.unlock();
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    } else if (eventData->m_type == TgEventType::EventTypeCharacterCallback
               && m_currentItem->getEnabled() && m_currentItem->getSelected()) {
        if (eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_NormalKey
            && eventData->m_event.m_keyEvent.m_key == '\t') {
            TG_FUNCTION_END();
            m_currentItem->setPositionChanged(true);
            m_mutex.unlock();
            return TgEventResult::EventResultNotCompleted;
        }
        if (eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_NormalKey
            && eventData->m_event.m_keyEvent.m_key != 0) {
            std::vector<std::string> listFontFileNames;
            std::string fontFile = m_textField.m_private->getFontFile();
            TgFontText::getFontFileNames(fontFile,
                                         TgGlobalApplication::getInstance()->getFontDefault()->getListFont(),
                                         listFontFileNames);
            if (TgGlobalApplication::getInstance()->getFontCharactersCache()->getFontIndexForCharacter(eventData->m_event.m_keyEvent.m_key,
                fontFile, listFontFileNames) != -1) {
                listAddCharacter.push_back(eventData->m_event.m_keyEvent.m_key);
                if (m_selectedTextSize >= 0) {
                    m_textField.m_private->editText(listAddCharacter, static_cast<size_t>(m_cursorPosition), static_cast<size_t>(m_selectedTextSize));
                    m_cursorPosition++;
                } else {
                    m_textField.m_private->editText(listAddCharacter, static_cast<size_t>(m_cursorPosition+m_selectedTextSize), static_cast<size_t>(std::abs(m_selectedTextSize)));
                    m_cursorPosition = m_cursorPosition+m_selectedTextSize+1;
                }
                m_selectedTextSize = 0;
                m_currentItem->setPositionChanged(true);
            }
        }
        m_mutex.unlock();
        TG_FUNCTION_END();
        return TgEventResult::EventResultCompleted;
    }

    m_mutex.unlock();
    TG_FUNCTION_END();
    return TgEventResult::EventResultNotCompleted;
}

/*!
 * \brief TgTexteditPrivate::resetTextFieldPosition
 *
 * \param positionX text's drawing cursor position
 */
void TgTexteditPrivate::resetTextFieldPosition(const float &positionX)
{
    if (m_currentItem->getWidth() > getTextLeftMargin() + getTextRightMargin() + m_textField.m_private->getTextWidth() ) {
        // text is smaller than edit area
        m_textField.setMarginLeft( getTextLeftMargin() );
    } else {
        if (m_currentItem->getWidth() < m_textField.getMarginLeft() + positionX + m_cursorPositionImage.getWidth() + getTextRightMargin() ) {
            m_textField.setMarginLeft( m_currentItem->getWidth() - positionX - m_cursorPositionImage.getWidth() - getTextRightMargin() );
        } else if (m_textField.getMarginLeft() + positionX < getTextLeftMargin() ) {
            m_textField.setMarginLeft( getTextLeftMargin() - positionX );
        } else if (m_currentItem->getWidth() - getTextLeftMargin() - getTextRightMargin() > m_textField.m_private->getTextWidth() + m_textField.getMarginLeft() + m_cursorPositionImage.getWidth() ) {
            // There is too much gap on the right side (text and "side of textedit field" )
            m_textField.setMarginLeft( m_currentItem->getWidth() - getTextLeftMargin() - getTextRightMargin() - m_cursorPositionImage.getWidth() - m_textField.m_private->getTextWidth()  );
        }
    }
}

void TgTexteditPrivate::clearText()
{
    std::vector<uint32_t> listAddCharacter;
    if (m_selectedTextSize >= 0) {
        m_textField.m_private->editText(listAddCharacter, static_cast<size_t>(m_cursorPosition), static_cast<size_t>(m_selectedTextSize));
    } else {
        m_textField.m_private->editText(listAddCharacter, static_cast<size_t>(m_cursorPosition+m_selectedTextSize), static_cast<size_t>(std::abs(m_selectedTextSize)));
        m_cursorPosition = m_cursorPosition+m_selectedTextSize;
    }
    m_selectedTextSize = 0;
    m_currentItem->setPositionChanged(true);
}

void TgTexteditPrivate::onMousePressed(float x)
{
    m_cursorPosition = static_cast<int64_t>(m_textField.m_private->getTextCharacterIndex(x-m_textField.getXonWindow()));
    m_selectedTextSize = 0;
    m_currentItem->setPositionChanged(true);
    m_cursorPressedDown = true;
}

void TgTexteditPrivate::onMouseReleased(float x)
{
    if (m_cursorPressedDown) {
        int64_t tmpCurrentPositionIndex = static_cast<int64_t>(m_textField.m_private->getTextCharacterIndex(x-m_textField.getXonWindow()));
        m_selectedTextSize = tmpCurrentPositionIndex - m_cursorPosition;
    }
    m_currentItem->setPositionChanged(true);
    m_cursorPressedDown = false;
}

/*!
 * \brief TgTexteditPrivate::onMouseMove
 *
 * called when mouse is moving on the TgTextedit's TgMouseCapture area
 *
 * \param x x position
 */
void TgTexteditPrivate::onMouseMove(float x)
{
    TG_FUNCTION_BEGIN();
    if (m_cursorPressedDown) {
        int64_t tmpCurrentPositionIndex = static_cast<int64_t>(m_textField.m_private->getTextCharacterIndex(x-m_textField.getXonWindow()));
        m_selectedTextSize = tmpCurrentPositionIndex - m_cursorPosition;
        m_currentItem->setPositionChanged(true);
    }
    TG_FUNCTION_END();
}

size_t TgTexteditPrivate::getCursorPosition()
{
    size_t ret;
    m_mutex.lock();
    if (!m_selectedTextSize)  {
        ret = static_cast<size_t>(m_cursorPosition);
    } else if (m_selectedTextSize > 0) {
        ret = static_cast<size_t>(m_cursorPosition+m_selectedTextSize);
    } else {
        ret = static_cast<size_t>(m_cursorPosition+m_selectedTextSize);
    }
    m_mutex.unlock();
    return ret;
}

size_t TgTexteditPrivate::getCharacterCount()
{
    size_t ret;
    m_mutex.lock();
    ret = m_textField.getCharacterCount();
    m_mutex.unlock();
    return ret;
}

uint32_t TgTexteditPrivate::getCharacterByIndex(size_t index)
{
    uint32_t ret;
    m_mutex.lock();
    ret = m_textField.getCharacterByIndex(index);
    m_mutex.unlock();
    return ret;
}

/*!
 * \brief TgTexteditPrivate::getSelectedTextSize
 *
 * \return get number of characters selected
 */
int64_t TgTexteditPrivate::getSelectedTextSize()
{
    int64_t ret;
    m_mutex.lock();
    ret = m_selectedTextSize*-1;
    m_mutex.unlock();
    return ret;
}

/*!
 * \brief TgTexteditPrivate::getTextWidth
 *
 * \return get text width
 */
float TgTexteditPrivate::getTextWidth()
{
    TG_FUNCTION_BEGIN();
    float ret;
    m_mutex.lock();
    ret = m_textField.m_private->getTextWidth();
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgTexteditPrivate::getTextHeight
 *
 * \return text single line height (pixels)
 */
float TgTexteditPrivate::getTextHeight()
{
    TG_FUNCTION_BEGIN();
    float ret;
    m_mutex.lock();
    ret = m_textField.m_private->getTextHeight();
    m_mutex.unlock();
    TG_FUNCTION_END();
    return ret;
}