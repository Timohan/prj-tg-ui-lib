/*!
 * \file
 * \brief file tg_x11_keycode.h
 *
 * main window (x11) keycode to keysym class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_X11_KEYCODE_H
#define TG_X11_KEYCODE_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../../event/tg_event_data.h"

struct TgX11InputModsKeyDown
{
    bool m_shiftLeft  : 1;  /**< left shift down or up - ShiftMask  */
    bool m_shiftRight : 1;  /**< right shift down or up - ShiftMask */
    bool m_ctrlLeft   : 1;  /**< left ctrl down or up - ControlMask */
    bool m_ctrlRight  : 1;  /**< right ctrl down or up - ControlMask */
    bool m_alt        : 1;  /**< alt down or up */
    bool m_altGr      : 1;  /**< alt gr down or up - Mod5Mask */
};

class TgX11KeyCode
{
public:
    TgX11KeyCode();
    bool keyCodeToEventDataPress(Display *display, unsigned int keyCode, TgEventData &data);
    bool keyCodeToEventDataRelease(Display *display, unsigned int keyCode, TgEventData &data);

private:
    TgX11InputModsKeyDown m_modsKeyDown;

    static KeySym keyCodeToKeySym(Display *display, unsigned int keyCode, unsigned int eventMask);
    int generateModsKeyDown();
    unsigned int generateEventMask();
};

#endif // TG_X11_KEYCODE_H
