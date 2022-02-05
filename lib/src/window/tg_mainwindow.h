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
    explicit TgMainWindow(int width, int height, const char *windowTitle);
    ~TgMainWindow();

    virtual void render(const TgWindowInfo *windowInfo) override;
    void exit();

    void setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount);
    size_t getAllowedNumberMouseButtonCount();
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
    void addEvent(GLFWwindow *window, const TgEventData *eventData);

    friend class TgGlobalApplication;
};

#endif // TG_MAIN_WINDOW_H
