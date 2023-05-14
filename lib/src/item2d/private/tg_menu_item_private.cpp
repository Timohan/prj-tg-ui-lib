/*!
 * \file
 * \brief file tg_menu_item_private.cpp
 *
 * Menu item (private)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_menu_item_private.h"
#include "../../global/tg_global_log.h"
#include "../private/item2d/tg_item2d_private.h"
#include "mouse_capture/tg_mouse_capture_private.h"
#include "../../global/private/tg_global_defines.h"
#include "../../global/private/tg_global_menu_holder.h"
#include "../../global/private/tg_global_wait_renderer.h"

TgMenuItemPrivate::TgMenuItemPrivate(TgMenuItem *parentItem, TgMenuItem *currentItem, const char *text, const TgShortCut *shortCut) :
    m_currentItem(currentItem),
    m_background(currentItem, MENU_ITEM_DEFAULT_BACKGROUND_R, MENU_ITEM_DEFAULT_BACKGROUND_G, MENU_ITEM_DEFAULT_BACKGROUND_B, MENU_ITEM_DEFAULT_BACKGROUND_A),
    m_backgroundEnabled(currentItem, 150, 150, 150),
    m_backgroundHover(currentItem, 226, 226, 226),
    m_itemText(currentItem, text, "", MENU_ITEM_FONT_SIZE, 0, 0, 0),
    m_itemTextShortCut(currentItem, "", "", MENU_ITEM_FONT_SIZE, 0, 0, 0),
    m_subMenuArrowImage(currentItem, getSubMenuArrowX(), getSubMenuArrowY(),
        TG_MENU_DEFAULT_SUB_MENU_ARROW_HEIGHT, TG_MENU_DEFAULT_SUB_MENU_ARROW_HEIGHT,
        std::string(std::string(IMAGES_PATH) + std::string("/menu/prj-tj-ui-lib-menuarrow.png")).c_str())
{
    if (shortCut) {
        m_shortCut = *shortCut;
        m_itemTextShortCut.setText(generateShortCutText().c_str());
    }
    if (!parentItem) {
        m_globalMenuIndex = TgGlobalMenuHolder::getInstance()->addMenuItem(currentItem);
    }
    m_backgroundHover.setVisible(false);
    m_backgroundEnabled.setVisible(false);
    m_itemTextShortCut.setVisible(!m_itemTextShortCut.getText().empty());
    m_subMenuArrowImage.setVisible(false);
    m_itemText.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
    m_itemText.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
    currentItem->setWidth(getMinWidth());
    m_itemText.setMarginLeft(MENU_ITEM_LEFT_TEXT_MARGIN);
    m_itemText.setMarginRight(MENU_ITEM_RIGHT_TEXT_MARGIN);
    m_itemTextShortCut.setMarginLeft( m_itemText.getTextWidth() + MENU_ITEM_LEFT_TEXT_MARGIN*2 );
    m_itemTextShortCut.setMarginRight(MENU_ITEM_RIGHT_TEXT_MARGIN);
    m_itemTextShortCut.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignLeft);
    m_itemTextShortCut.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
}

TgMenuItemPrivate::~TgMenuItemPrivate()
{
    if (m_globalMenuIndex) {
        TgGlobalMenuHolder::getInstance()->removeMenuItem(m_globalMenuIndex);
    }
}

std::string TgMenuItemPrivate::generateShortCutText()
{
    if (!m_shortCut.m_key) {
        return "";
    }
    std::string ret = "[";
    if (m_shortCut.m_alt) {
        ret += "Alt";
    }
    if (m_shortCut.m_ctrl) {
        if (ret.size() > 1) {
            ret += "+";
        }
        ret += "Ctrl";
    }
    if (m_shortCut.m_shift) {
        if (ret.size() > 1) {
            ret += "+";
        }
        ret += "Shift";
    }
    if (ret.size() > 1) {
        ret += "+";
    }
    ret.push_back(static_cast<char>(std::toupper(static_cast<int>(m_shortCut.m_key))));
    ret += "]";
    return ret;
}

float TgMenuItemPrivate::getSubMenuArrowX()
{
    return m_currentItem->getWidth() - TG_MENU_DEFAULT_SUB_MENU_ARROW_HEIGHT - TG_MENU_DEFAULT_SUB_MENU_ARROW_MARGIN;
}

float TgMenuItemPrivate::getSubMenuArrowY()
{
    return (m_currentItem->getHeight() - TG_MENU_DEFAULT_SUB_MENU_ARROW_HEIGHT)/2.0f;
}

void TgMenuItemPrivate::setMenuType(TgMenuItemPrivate::MenuType menuType)
{
    m_menuType = menuType;
}

void TgMenuItemPrivate::setShortCutPosition(float x)
{
    m_itemTextShortCut.setMarginLeft(x);
}

void TgMenuItemPrivate::setSubMenuArrowPosition()
{
    m_subMenuArrowImage.setX(getSubMenuArrowX());
    m_subMenuArrowImage.setY(getSubMenuArrowY());
}

TgMenuItemPrivate::MenuType TgMenuItemPrivate::getMenuType()
{
    return m_menuType;
}

/*! \brief TgMenuItemPrivate::getMinWidth
 * \return min width of the menu item that is required for item
 * basically: text.width() + left.margin + right.margin
 */
float TgMenuItemPrivate::getMinWidth()
{
    return m_itemText.getTextWidth() + MENU_ITEM_LEFT_TEXT_MARGIN + MENU_ITEM_RIGHT_TEXT_MARGIN;
}

/*! \brief TgMenuItemPrivate::getMinShortcutWidth
 * \return min width short the short cut text area
 */
float TgMenuItemPrivate::getMinShortcutWidth()
{
    return m_itemTextShortCut.getTextWidth() + MENU_ITEM_LEFT_TEXT_MARGIN;
}

void TgMenuItemPrivate::onHoverChanged(bool hover)
{
    TG_FUNCTION_BEGIN();
    if (!m_currentItem->getEnabled()) {
        return;
    }
    if (hover
        || (!hover && !reinterpret_cast<TgItem2d *>(m_currentItem)->m_private->getMenuVisible())
        || (m_menuType == MenuType::MenuType_TopMenu && !reinterpret_cast<TgItem2d *>(m_currentItem)->m_private->getMenuVisible())) {
        m_background.setVisible(!hover);
        m_backgroundHover.setVisible(hover);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMenuItemPrivate::onEnabledChanged
 *
 * virtual function when enabled changed
 *
 * \param enabled enabled true/false
 */
void TgMenuItemPrivate::onEnabledChanged(bool enabled)
{
    TG_FUNCTION_BEGIN();
    if (!enabled) {
        m_background.setVisible(false);
        m_backgroundHover.setVisible(false);
        m_backgroundEnabled.setVisible(true);
    } else {
        m_backgroundEnabled.setVisible(false);
        onHoverChanged(m_currentItem->getEnabled());
    }
    TG_FUNCTION_END();
}

void TgMenuItemPrivate::onVisibleChanged(bool visible, bool haveSubMenu)
{
    TG_FUNCTION_BEGIN();
    m_subMenuArrowImage.setVisible(visible && haveSubMenu);
    TG_FUNCTION_END();
}

void TgMenuItemPrivate::setHighlightOff()
{
    TG_FUNCTION_BEGIN();
    if (!m_currentItem->getEnabled()) {
        return;
    }
    m_background.setVisible(true);
    m_backgroundHover.setVisible(false);
    TG_FUNCTION_END();
}

std::string TgMenuItemPrivate::getName()
{
    return m_itemText.getText();
}

/*!
 * \brief TgMenuItemPrivate::handleEvent
 *
 * handles the event
 *
 * \param eventData
 * \return if event result is completed
 */
TgEventResult TgMenuItemPrivate::handleEvent(TgEventData *eventData)
{
    TG_FUNCTION_BEGIN();
    if (eventData->m_type == TgEventType::EventTypeCharacterCallbackShortCut
        && m_shortCut.m_key
        && m_currentItem->getEnabled()
        && static_cast<uint32_t>(toupper(m_shortCut.m_key)) == static_cast<uint32_t>(toupper(eventData->m_event.m_keyEvent.m_key))
        && m_shortCut.m_shift == static_cast<bool>(eventData->m_event.m_keyEvent.m_pressModsKeyDown & PressModsKeyDown_Shift)
        && m_shortCut.m_alt == static_cast<bool>(eventData->m_event.m_keyEvent.m_pressModsKeyDown & PressModsKeyDown_Alt)
        && m_shortCut.m_ctrl == static_cast<bool>(eventData->m_event.m_keyEvent.m_pressModsKeyDown & PressModsKeyDown_Ctrl)) {
        reinterpret_cast<TgMouseCapture *>(m_currentItem)->m_private->sendMouseClickedNoButton();
        return TgEventResult::EventResultCompleted;
    }
    TG_FUNCTION_END();
    return TgEventResult::EventResultNotCompleted;
}

/*!
 * \brief TgMenuItemPrivate::setBackgroundColor
 *
 * set menu item's basic background color
 *
 * \param r
 * \param g
 * \param b
 * \param a
 */
void TgMenuItemPrivate::setBackgroundColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    m_background.setColor(r, g, b, a);
}

/*!
 * \brief TgMenuItemPrivate::getMenuRendering
 *
 * get if menu is rendering
 *
 * \return menuRendering
 */
bool TgMenuItemPrivate::getMenuRendering()
{
    return m_menuRendering;
}

/*!
 * \brief TgMenuItemPrivate::setMenuRendering
 *
 * set if menu is rendering
 *
 * \param menuRendering
 */
void TgMenuItemPrivate::setMenuRendering(bool menuRendering)
{
    m_menuRendering = menuRendering;
}