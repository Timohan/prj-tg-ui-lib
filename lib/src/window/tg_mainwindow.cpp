/*!
 * \file
 * \brief file tg_mainwindow.cpp
 *
 * main window class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#include "tg_mainwindow.h"
#include "../global/tg_global_log.h"
#include "../global/tg_global_application.h"
#include "tg_mainwindow_private.h"
#include "../event/tg_events.h"
#include "../item2d/private/item2d/tg_item2d_private.h"

/*!
 * \brief TgMainWindow::TgMainWindow
 *
 * constructor
 * \param width width of window
 * \param height height of window
 * \param windowTitle window title
 */
TgMainWindow::TgMainWindow(int width, int height, const char *windowTitle) :
    TgItem2d(nullptr, 0, 0, static_cast<float>(width), static_cast<float>(height)),
    m_mainwindowPrivate(new TgMainWindowPrivate(width, height, this)),
    m_windowTitle(windowTitle),
    m_firstTimeRender(true)
{
    TG_FUNCTION_BEGIN();
    TgGlobalApplication::getInstance()->addMainWindow(this);
    TG_FUNCTION_END();
}

TgMainWindow::~TgMainWindow()
{
    TG_FUNCTION_BEGIN();
    if (m_mainwindowPrivate) {
        delete m_mainwindowPrivate;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::initWindow
 *
 * inits the window
 *
 * \return 0 if success
 */
int TgMainWindow::initWindow()
{
    TG_FUNCTION_BEGIN();
    int ret = m_mainwindowPrivate->initWindow(m_windowTitle.c_str());
    if (ret) {
        return ret;
    }
    TG_FUNCTION_END();
    return initCustomShader();
}

/*!
 * \brief TgMainWindow::render
 *
 * main window's render
 *
 */
void TgMainWindow::render(const TgWindowInfo *)
{
    TG_FUNCTION_BEGIN();
    if (m_firstTimeRender) {
        setVisible(true);
        m_firstTimeRender = false;
    }

    m_mainwindowPrivate->handleEvents();
    customBeforeRender();
    checkPositionValuesChildren(m_mainwindowPrivate->getWindowInfo());
    m_mainwindowPrivate->setupViewForRender();
    customRender();
    m_mainwindowPrivate->setup2DShaderToUniforms();
    renderChildren(m_mainwindowPrivate->getWindowInfo());
    m_mainwindowPrivate->renderEnd();
    checkOnResizeChangedOnChildren();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::addEvent
 *
 * add event data to main window
 *
 * \param window event will go to this window
 * \param eventData event data
 */
void TgMainWindow::addEvent(GLFWwindow *window, const TgEventData *eventData)
{
    TG_FUNCTION_BEGIN();
    if (m_mainwindowPrivate && m_mainwindowPrivate->getWindow() == window) {
        m_mainwindowPrivate->addEvent(eventData);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::exit
 *
 * quits the application
 */
void TgMainWindow::exit()
{
    TG_FUNCTION_BEGIN();
    TgGlobalApplication::getInstance()->exit();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::addNewEvent
 *
 * add new event to window list
 *
 * \param event
 */
void TgMainWindow::addNewEvent(TgEventData *event)
{
    TG_FUNCTION_BEGIN();
    m_mainwindowPrivate->addEvent(event);
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::handlePrivateMessage
 *
 * virtual function to handle private function
 *
 */
void TgMainWindow::handlePrivateMessage(const TgItem2dPrivateMessage *message)
{
    TG_FUNCTION_BEGIN();
    if (message->m_type == TgItem2dPrivateMessageType::RemovingItem2d) {
        m_mainwindowPrivate->m_events.removingItem(message->m_fromItem);
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::initCustomShader
 *
 * virtual function if you like to init custom shader
 * which can be rendered on custom rendering
 */
int TgMainWindow::initCustomShader()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return 0;
}

/*!
 * \brief TgMainWindow::customBeforeRender
 *
 * virtual function if you like to use a function that happens
 * all before rendering
 */
void TgMainWindow::customBeforeRender()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::customRender
 *
 * virtual function if to make custom render
 */
void TgMainWindow::customRender()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgMainWindow::setAllowedNumberMouseButtonCount
 *
 * set allowed number of mouse button down at once
 *
 * \param allowedNumberMouseButtonDownCount
 */
void TgMainWindow::setAllowedNumberMouseButtonCount(size_t allowedNumberMouseButtonDownCount)
{
    m_mainwindowPrivate->setAllowedNumberMouseButtonCount(allowedNumberMouseButtonDownCount);
}

/*!
 * \brief TgMainWindow::getAllowedNumberMouseButtonCount
 *
 * \return allowed number of mouse button down at once
 */
size_t TgMainWindow::getAllowedNumberMouseButtonCount()
{
    return m_mainwindowPrivate->getAllowedNumberMouseButtonCount();
}
