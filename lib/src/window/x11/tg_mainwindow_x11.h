/*!
 * \file
 * \brief file tg_mainwindow_x11.h
 *
 * main window (x11) class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MAIN_WINDOW_X11_H
#define TG_MAIN_WINDOW_X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "../../common/time_difference.h"
#include "../../event/tg_events.h"
#include "tg_x11_keycode.h"

struct TgWindowInfo;
class TgMainWindowPrivate;

class TgMainWindowX11 : private TgX11KeyCode
{
public:
    TgMainWindowX11(TgMainWindowPrivate *mainWindowPrivate);
    ~TgMainWindowX11();

    int initWindow(const char *windowTitle, const TgWindowInfo *info);
    bool setupViewForRender();
    bool renderEnd(const TgWindowInfo *info);
#ifdef FUNCIONAL_TEST
    Display *getDisplay();
    Window *getWindow();
#endif

private:
    TgMainWindowPrivate *m_mainWindowPrivate;
    Display *m_display;
    Window m_window;
    GLXContext m_context;
    Colormap m_colorMap;
    TimeDifference m_startTime;

    void inputListener(const TgWindowInfo *info);
    static TgMouseType getButtonType(unsigned int button);
};

#endif // TG_MAIN_WINDOW_X11_H
