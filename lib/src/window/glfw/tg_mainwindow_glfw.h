/*!
 * \file
 * \brief file tg_mainwindow_glfw.h
 *
 * main window (glfw) class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MAIN_WINDOW_GLFW_H
#define TG_MAIN_WINDOW_GLFW_H

struct GLFWwindow;
struct TgWindowInfo;

class TgMainWindowGlfw
{
public:
    TgMainWindowGlfw();
    ~TgMainWindowGlfw();

    int initWindow(const char *windowTitle, const TgWindowInfo *info);
    bool setupViewForRender();
    bool renderEnd();
    GLFWwindow *getWindow();

private:
    GLFWwindow *m_window;

};

#endif // TG_MAIN_WINDOW_GLFW_H
