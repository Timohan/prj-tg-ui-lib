/*!
 * \file
 * \brief file tg_mainwindow_private.cpp
 *
 * main window (private) class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mainwindow_private.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../global/tg_global_log.h"
#include "../global/tg_global_application.h"
#include "../input/tg_glfw_input.h"
#include "../item2d/tg_item2d.h"

/*!
 * \brief TgWindowInfo::TgWindowInfo
 * constructor of window info, this class stores
 * current window 2D shader index of uniforms and
 * window resolution and so on.
 * \param width window width
 * \param height window height
 */
TgWindowInfo::TgWindowInfo(int width, int height) :
    m_windowWidth(width),
    m_windowHeight(height),
    m_shaderTransformIndex(0),
    m_shaderRenderTypeIndex(0),
    m_shaderColorIndex(0),
    m_maxRenderValues(0)
{
}

TgMainWindowPrivate::TgMainWindowPrivate(int width, int height, TgItem2d *item) :
    m_currentItem(item),
    m_window(nullptr),
    m_windowInfo(width, height)
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

TgMainWindowPrivate::~TgMainWindowPrivate()
{
    TG_FUNCTION_BEGIN();
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindowPrivate::reShapeWindow
 *
 * sets viewport and projection for window
 * \param width width of window
 * \param height height of window
 */
void TgMainWindowPrivate::reShapeWindow(int width, int height)
{
    TG_FUNCTION_BEGIN();
    glViewport(0, 0, width, height);
    m_projection.setOrth(0, static_cast<float>(width), 0, static_cast<float>(height), -1.0f, 1.0f);
    TG_FUNCTION_END();
}
void TgMainWindowPrivate::errorCallback(int error, const char* description)
{
    TG_FUNCTION_BEGIN();
    (void)error;
    (void)description;
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindowPrivate::initWindow
 *
 * inits the window
 *
 * \param windowTitle title of window
 * \return 0 on success
 */
int TgMainWindowPrivate::initWindow(const char *windowTitle)
{
    TG_FUNCTION_BEGIN();
    if (!glfwInit()) {
        TG_ERROR_LOG("glfwInit failed");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(m_windowInfo.m_windowWidth, m_windowInfo.m_windowHeight, windowTitle, nullptr, nullptr);
    if (!m_window) {
        TG_ERROR_LOG("glfwCreateWindow failed");
        glfwTerminate();
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    TgGlfwInput::instance()->setup(m_window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        TG_ERROR_LOG("glewInit failed");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    glfwShowWindow(m_window);
    if (m_shader2d.init() != 0) {
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    m_windowInfo.m_shaderTransformIndex = glGetUniformLocation(m_shader2d.generalShader(), "vertex_transform" );
    m_windowInfo.m_shaderRenderTypeIndex = glGetUniformLocation(m_shader2d.generalShader(), "render_type" );
    m_windowInfo.m_shaderColorIndex = glGetUniformLocation(m_shader2d.generalShader(), "color" );
    m_windowInfo.m_maxRenderValues = glGetUniformLocation(m_shader2d.generalShader(), "maxRenderValues" );
    TG_FUNCTION_END();
    return EXIT_SUCCESS;
}

/*!
 * \brief TgMainWindowPrivate::handleEvents
 *
 * functionality to handle events
 */
void TgMainWindowPrivate::handleEvents()
{
    TG_FUNCTION_BEGIN();
    TgEventData *eventData;
    TgEventResult ret;
    TgEventData tmpEventData;
    m_events.lock();
    while (1) {
        eventData = m_events.getFirstEventData();
        if (!eventData) {
            break;
        }
        if (eventData->m_type == TgEventType::EventTypeWindowResize) {
             m_windowInfo.m_windowWidth = eventData->m_event.m_windowResize.m_width;
             m_windowInfo.m_windowHeight = eventData->m_event.m_windowResize.m_height;
             m_currentItem->setWidth(static_cast<float>(m_windowInfo.m_windowWidth));
             m_currentItem->setHeight(static_cast<float>(m_windowInfo.m_windowHeight));
        } else {
            if (eventData->m_type == TgEventType::EventTypeMousePress) {
                eventData->m_event.m_mouseEvent.m_newItem = nullptr;
                eventData->m_event.m_mouseEvent.m_currentMouseDownItem = nullptr;
            } else if (eventData->m_type == TgEventType::EventTypeMouseRelease
                      || eventData->m_type == TgEventType::EventTypeMouseMove) {
                eventData->m_event.m_mouseEvent.m_newItem = nullptr;
                eventData->m_event.m_mouseEvent.m_currentMouseDownItem = m_events.getMouseDownItem(eventData->m_event.m_mouseEvent.m_mouseType, eventData->m_event.m_mouseEvent.m_releaseWoCallback);
            }
            ret = m_currentItem->handleEventsChildren(eventData, &m_windowInfo);
            if (ret == TgEventResult::EventResultNotCompleted
                && eventData->m_type == TgEventType::EventTypeCharacterCallback
                && eventData->m_event.m_keyEvent.m_pressReleaseKey == TgPressReleaseKey::PressReleaseKey_NormalKey
                && eventData->m_event.m_keyEvent.m_key == '\t') {
                tmpEventData.m_type = TgEventType::EventTypeSelectFirstItem;
                tmpEventData.m_event.m_keyEvent.m_previousItem2d = eventData->m_event.m_keyEvent.m_previousItem2d;
                m_currentItem->handleEventsChildren(&tmpEventData, &m_windowInfo);
            }
            if (eventData->m_type == TgEventType::EventTypeMousePress) {
                m_events.setMouseDownItem(
                    eventData->m_event.m_mouseEvent.m_mouseType,
                    eventData->m_event.m_mouseEvent.m_newItem);
            }
        }
        if (eventData->m_type == TgEventType::EventTypeMouseRelease) {
            m_events.removingItem(eventData->m_event.m_mouseEvent.m_mouseType);
        }

        m_events.clearFirstEventData();
    }
    m_events.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindowPrivate::setupViewForRender
 *
 * \return true if success
 */
bool TgMainWindowPrivate::setupViewForRender()
{
    TG_FUNCTION_BEGIN();
    if (glfwWindowShouldClose(m_window)) {
        TG_FUNCTION_END();
        return false;
    }

    reShapeWindow(m_windowInfo.m_windowWidth, m_windowInfo.m_windowHeight);
    glClearColor(0, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    TG_FUNCTION_END();
    return true;
}

bool TgMainWindowPrivate::setup2DShaderToUniforms()
{
    TG_FUNCTION_BEGIN();
    glUseProgram(m_shader2d.generalShader() );
    glUniform1i(glGetUniformLocation(m_shader2d.generalShader(), "texture" ), 0);
    glUniformMatrix4fv( glGetUniformLocation(m_shader2d.generalShader(), "model" ), 1, 0, m_model.getMatrixTable()->data);
    glUniformMatrix4fv( glGetUniformLocation(m_shader2d.generalShader(), "view" ), 1, 0, m_view.getMatrixTable()->data);
    glUniformMatrix4fv( glGetUniformLocation(m_shader2d.generalShader(), "projection" ), 1, 0, m_projection.getMatrixTable()->data);
    glUniformMatrix4fv( m_windowInfo.m_shaderTransformIndex, 1, 0, m_view.getMatrixTable()->data);
    glUniform1i( m_windowInfo.m_shaderRenderTypeIndex, 0);
    glUniform4f( m_windowInfo.m_shaderColorIndex, 0, 0, 0, 0);
    glUniform4f( m_windowInfo.m_maxRenderValues, 0, 0,
                 static_cast<float>(m_windowInfo.m_windowWidth),
                 static_cast<float>(m_windowInfo.m_windowHeight));
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgMainWindowPrivate::renderEnd
 *
 * opengl render end functionalities
 *
 * \return true always
 */
bool TgMainWindowPrivate::renderEnd()
{
    TG_FUNCTION_BEGIN();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgMainWindowPrivate::getWindowInfo
 *
 * \return window info
 */
const TgWindowInfo *TgMainWindowPrivate::getWindowInfo() const
{
    return &m_windowInfo;
}

/*!
 * \brief TgMainWindowPrivate::getWindow
 *
 * \return get GLFWwindow window of this window
 */
GLFWwindow *TgMainWindowPrivate::getWindow()
{
    return m_window;
}

/*!
 * \brief TgMainWindowPrivate::addEvent
 *
 * \param eventData add event
 */
void TgMainWindowPrivate::addEvent(const TgEventData *eventData)
{
    m_events.addEvent(eventData);
}

/*!
 * \brief TgMainWindowPrivate::setAllowedNumberMouseButtonCount
 *
 * set allowed number of mouse button down at once
 *
 * \param allowedNumberMouseButtonDownCount
 */
void TgMainWindowPrivate::setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount)
{
    m_events.setAllowedNumberMouseButtonCount(allowedNumberMouseButtonDownCount);
}

/*!
 * \brief TgMainWindowPrivate::getAllowedNumberMouseButtonCount
 *
 * \return allowed number of mouse button down at once
 */
size_t TgMainWindowPrivate::getAllowedNumberMouseButtonCount()
{
    return m_events.getAllowedNumberMouseButtonCount();
}