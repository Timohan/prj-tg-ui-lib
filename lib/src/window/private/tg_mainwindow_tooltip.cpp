/*!
 * \file
 * \brief file tg_mainwindow_tooltip.cpp
 *
 * main window (private) class Tooltip
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#include "tg_mainwindow_tooltip.h"
#include "../tg_mainwindow.h"
#include "../../global/tg_global_log.h"
#include "../../global/private/tg_global_defines.h"
#include "../../global/private/tg_global_tooltip.h"
#include "../../font/tg_font_math.h"
#include "../tg_mainwindow_private.h"

TgMainWindowTooltip::TgMainWindowTooltip()
    : m_background(nullptr, 0, 0, 10, 10, std::string(std::string(IMAGES_PATH) + std::string("/tooltip/prj-tg-ui-lib-tooltip-background.png")).c_str())
    , m_textField(&m_background, "", "", 21, 255, 255, 255)
{
    m_background.setVisible(false);
    m_textField.setHorizontalAlign(TgTextfieldHorizontalAlign::AlignCenterH);
    m_textField.setVerticalAlign(TgTextfieldVerticalAlign::AlignCenterV);
    m_textField.setMaxLineCount(0);
    m_textField.setAllowBreakLineGoOverMaxLine(false);
}

TgMainWindowTooltip::~TgMainWindowTooltip()
{
}

/*!
 * \brief TgMainWindowTooltip::startHandleEvents
 *
 * called before handling the events
 */
void TgMainWindowTooltip::startHandleEvents()
{
    TgGlobalTooltip::getInstance()->startHandleEvents();
}

/*!
 * \brief TgMainWindowTooltip::startRendering
 *
 * called before start rendering
 * to set possible tooltip text for rendering
 * \param windowInfo
 */
void TgMainWindowTooltip::startRendering(const TgWindowInfo *windowInfo)
{
    int x = 0, y = 0;
    std::string tooltipText = TgGlobalTooltip::getInstance()->startRendering(x, y);
    if (tooltipText.empty()) {
        m_previousRenderX = -1;
        m_previousRenderY = -1;
        m_background.setVisible(false);
        return;
    }
    if (m_background.getVisible() && tooltipText == m_textField.getText() && m_previousRenderX == x && m_previousRenderY == y) {
        return;
    }

    m_previousRenderX = x;
    m_previousRenderY = y;
    m_background.setVisible(true);
    m_textField.setText(tooltipText.c_str());

    float textWidth = 0;
    float textHeight;
    std::vector<TgTextFieldText> listText;
    TgTextFieldText line;
    line.m_text = m_textField.getText();
    listText.push_back(line);
    float h;
    TgFontMath::getFontWidthHeight(listText, m_textField.getFontSize(), m_textField.getFontFile(), textWidth, textHeight, h, m_textField.getMaxLineCount(),
                                   static_cast<float>(windowInfo->m_windowWidth-20), m_textField.getWordWrap(), m_textField.getAllowBreakLineGoOverMaxLine() );

    if (windowInfo->m_windowHeight > y+static_cast<int>(h)+15) {
        m_background.setY(static_cast<float>(y+10));
        m_background.setHeight(h + 10);
    } else {
        m_background.setY(static_cast<float>(y-15)-h);
        m_background.setHeight(h + 10);
    }
    if (windowInfo->m_windowWidth > x+static_cast<int>(textWidth)+15) {
        m_background.setX(static_cast<float>(x+10));
        m_background.setWidth(textWidth + 10);
    } else {
        m_background.setX(static_cast<float>(x-15)-textWidth);
        m_background.setWidth(textWidth + 10);
    }
}

/*!
 * \brief TgMainWindowTooltip::checkPositionValuesTooltip
 *
 * check position values
 * \param windowInfo
 */
void TgMainWindowTooltip::checkPositionValuesTooltip(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexTooltip.lock();
    if (m_background.getVisible()) {
        m_background.checkPositionValues();
        m_background.checkPositionValuesChildren(windowInfo);
    }
    m_mutexTooltip.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindowTooltip::renderTooltip
 *
 * renders children items
 * \param windowInfo
 */
void TgMainWindowTooltip::renderTooltip(const TgWindowInfo *windowInfo)
{
    TG_FUNCTION_BEGIN();
    m_mutexTooltip.lock();
    if (!m_background.getVisible()) {
        m_mutexTooltip.unlock();
        return;
    }
    m_background.render(windowInfo);
    m_background.renderChildren(windowInfo);
    m_mutexTooltip.unlock();
    TG_FUNCTION_END();
}