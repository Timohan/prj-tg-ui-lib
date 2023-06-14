/*!
 * \file
 * \brief file tg_mainwindow.h
 *
 * main window class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MAIN_WINDOW_H
#define TG_MAIN_WINDOW_H

#include "../global/tg_global_macros.h"
#include "../item2d/tg_item2d.h"
#include <string>
#ifdef FUNCIONAL_TEST
#include <X11/Xlib.h>
#endif

struct TgMainWindowPrivate;
struct TgItem2dPrivateMessage;
struct GLFWwindow;
class TgEventData;

/*!
 * \brief TgMainWindow
 * Main window, init this after TgApplication init
 */
class TG_MAINWINDOW_EXPORT TgMainWindow : public TgItem2d
{
public:
    explicit TgMainWindow(int width, int height, const char *windowTitle, int minWidth = -1, int minHeight = -1, int maxWidth = -1, int maxHeight = -1);
    ~TgMainWindow();

    virtual bool render(const TgWindowInfo *windowInfo) override;
    void exit();

    void setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount);
    size_t getAllowedNumberMouseButtonCount();
#ifndef USE_GLFW
    void waitForEnd();
#endif

#ifdef FUNCIONAL_TEST
    Display *getDisplay();
    Window *getWindow();
#endif

protected:
    virtual void handlePrivateMessage(const TgItem2dPrivateMessage *message) override;
    virtual int initCustomShader();
    virtual void customBeforeRender();
    virtual void customRender();

private:
    TgMainWindowPrivate *m_mainwindowPrivate;
    std::string m_windowTitle;
    bool m_firstTimeRender;

    int initWindow();
    virtual void addNewEvent(TgEventData *event) override;
#ifdef USE_GLFW
    void addEvent(GLFWwindow *window, const TgEventData *eventData);
#endif
    friend class TgGlobalApplication;
};

#endif // TG_MAIN_WINDOW_H
