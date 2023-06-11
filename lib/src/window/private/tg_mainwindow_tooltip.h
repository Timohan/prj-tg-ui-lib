/*!
 * \file
 * \brief file tg_mainwindow_tooltip.h
 *
 * main window (private) class Tooltip
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MAIN_WINDOW_TOOLTIP_H
#define TG_MAIN_WINDOW_TOOLTIP_H

#include <mutex>
#include "../../item2d/tg_image_part.h"
#include "../../item2d/tg_textfield.h"

class TgMainWindow;

class TgMainWindowTooltip
{
public:
    TgMainWindowTooltip();
    ~TgMainWindowTooltip();

    void checkPositionValuesTooltip(const TgWindowInfo *windowInfo);
    void renderTooltip(const TgWindowInfo *windowInfo);
    void startHandleEvents();
    void startRendering(const TgWindowInfo *windowInfo);

private:
    std::mutex m_mutexTooltip;
    TgImagePart m_background;
    TgTextfield m_textField;
    int m_previousRenderX = -1;
    int m_previousRenderY = -1;
};

#endif // TG_MAIN_WINDOW_TOOLTIP_H
