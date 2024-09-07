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
#include "private/tg_mainwindow_menu.h"
#include "private/tg_mainwindow_tooltip.h"
#ifdef USE_GLFW
#include "glfw/tg_mainwindow_glfw.h"
#else
#include "x11/tg_mainwindow_x11.h"
#endif

class TgItem2d;
class TgMenuItem;
struct TgItem2dPrivateMessage;

struct TgWindowInfo
{
    TgWindowInfo(int width, int height, int minWidth, int minHeight, int maxWidth, int maxHeight);
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
     * shader's opacity of values index (uniform)
     */
    int m_shaderOpacityIndex;
    /**
     * shader's max render of values index (uniform)
     */
    int m_maxRenderValues;
    /*!
     * min window width
     */
    int m_minWidth;
    /*!
     * min window height
     */
    int m_minHeight;
    /*!
     * max window width
     */
    int m_maxWidth;
    /*!
     * max window height
     */
    int m_maxHeight;
};

class TgMainWindowPrivate
#ifdef USE_GLFW
: public TgMainWindowGlfw
#else
: public TgMainWindowX11
#endif
, public TgMainWindowMenu
, public TgMainWindowTooltip
{
public:
    explicit TgMainWindowPrivate(int width, int height, TgItem2d *item, int minWidth, int minHeight, int maxWidth, int maxHeight);
    ~TgMainWindowPrivate();
    int initWindow(const char *windowTitle);
    void handleEvents();
    bool setupViewForRender();
    bool renderEnd();
    void addEvent(const TgEventData *eventData);
    bool setup2DShaderToUniforms();
    const TgWindowInfo *getWindowInfo() const;

    void setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount);
    size_t getAllowedNumberMouseButtonCount();

    void handlePrivateMessage(const TgItem2dPrivateMessage *message);
private:
    TgItem2d *m_currentItem;
    TgWindowInfo m_windowInfo;
    TgShader2d m_shader2d;
    TgEvents m_events;

    TgMatrix4x4 m_model;
    TgMatrix4x4 m_view;
    TgMatrix4x4 m_projection;

    double m_currentMousePositionX;
    double m_currentMousePositionY;

    TgEventResult startHandleEventsChildren(TgEventData *eventData);
    void reShapeWindow(int width, int height);
    static void errorCallback(int error, const char* description);

    friend class TgMainWindow;
};

#endif // TG_MAIN_WINDOW_PRIVATE_H
