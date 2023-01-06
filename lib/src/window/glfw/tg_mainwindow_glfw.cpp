/*!
 * \file
 * \brief file tg_mainwindow_glfw.cpp
 *
 * main window (glfw) class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_mainwindow_glfw.h"
#include "../tg_mainwindow_private.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "../../global/tg_global_log.h"
#include "tg_glfw_input.h"
#include "../../global/private/tg_global_wait_renderer.h"

TgMainWindowGlfw::TgMainWindowGlfw() :
    m_window(nullptr)
{
}

TgMainWindowGlfw::~TgMainWindowGlfw()
{
    TG_FUNCTION_BEGIN();
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
    TG_FUNCTION_END();
}


/*!
 * \brief TgMainWindowGlfw::initWindow
 *
 * inits the window
 *
 * \param windowTitle title of window
 * \return 0 on success
 */
int TgMainWindowGlfw::initWindow(const char *windowTitle, const TgWindowInfo *info)
{
    TG_FUNCTION_BEGIN();
    if (!glfwInit()) {
        TG_ERROR_LOG("glfwInit failed");
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, info->m_minWidth == info->m_windowWidth
                                   && info->m_maxWidth == info->m_windowWidth
                                   && info->m_minHeight == info->m_windowHeight
                                   && info->m_maxHeight == info->m_windowHeight
                                    ? GL_FALSE : GL_TRUE);

    m_window = glfwCreateWindow(info->m_windowWidth, info->m_windowHeight, windowTitle, nullptr, nullptr);
    if (!m_window) {
        TG_ERROR_LOG("glfwCreateWindow failed");
        glfwTerminate();
        TG_FUNCTION_END();
        return EXIT_FAILURE;
    }
    glfwSetWindowSizeLimits(m_window, info->m_minWidth, info->m_minHeight, info->m_maxWidth, info->m_maxHeight);
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
    return EXIT_SUCCESS;
}

/*!
 * \brief TgMainWindowGlfw::setupViewForRender
 *
 * \return true if success
 */
bool TgMainWindowGlfw::setupViewForRender()
{
    TG_FUNCTION_BEGIN();
    if (glfwWindowShouldClose(m_window)) {
        TG_FUNCTION_END();
        return false;
    }
    TgGlobalWaitRenderer::getInstance()->waitForRender();
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgMainWindowGlfw::renderEnd
 *
 * opengl render end functionalities
 *
 * \return true always
 */
bool TgMainWindowGlfw::renderEnd()
{
    TG_FUNCTION_BEGIN();
    glfwSwapBuffers(m_window);
    glfwPollEvents();
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgMainWindowGlfw::getWindow
 *
 * \return get GLFWwindow window of this window
 */
GLFWwindow *TgMainWindowGlfw::getWindow()
{
    return m_window;
}
