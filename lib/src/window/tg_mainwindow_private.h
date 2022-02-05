/*!
 * \file
 * \brief file tg_mainwindow_private.h
 *
 * main window (private) class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_MAIN_WINDOW_PRIVATE_H
#define TG_MAIN_WINDOW_PRIVATE_H

#include "../global/tg_global_macros.h"
#include "../shader/tg_shader_2d.h"
#include "../math/tg_matrix4x4.h"
#include "../event/tg_events.h"

struct GLFWwindow;
class TgItem2d;

struct TgWindowInfo
{
    TgWindowInfo(int width, int height);
    /*!
     * current window width
     */
    int m_windowWidth;
    /*!
     * current window height
     */
    int m_windowHeight;
    /**
     * shader's transform index's index (uniform)
     */
    int m_shaderTransformIndex;
    /**
     * shader's render type's index (uniform)
     */
    int m_shaderRenderTypeIndex;
    /**
     * shader's shader color index's index (uniform)
     */
    int m_shaderColorIndex;
    /**
     * shader's max render of values index (uniform)
     */
    int m_maxRenderValues;
};

class TgMainWindowPrivate
{
public:
    explicit TgMainWindowPrivate(int width, int height, TgItem2d *item);
    ~TgMainWindowPrivate();
    int initWindow(const char *windowTitle);
    void handleEvents();
    bool setupViewForRender();
    bool renderEnd();
    const TgWindowInfo *getWindowInfo() const;
    GLFWwindow *getWindow();
    void addEvent(const TgEventData *eventData);
    bool setup2DShaderToUniforms();

    void setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount);
    size_t getAllowedNumberMouseButtonCount();

private:
    TgItem2d *m_currentItem;
    GLFWwindow *m_window;
    TgWindowInfo m_windowInfo;
    TgShader2d m_shader2d;
    TgEvents m_events;

    TgMatrix4x4 m_model;
    TgMatrix4x4 m_view;
    TgMatrix4x4 m_projection;

    void reShapeWindow(int width, int height);
    static void errorCallback(int error, const char* description);

    friend class TgMainWindow;
};

#endif // TG_MAIN_WINDOW_PRIVATE_H
