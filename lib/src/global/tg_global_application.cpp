/*!
 * \file
 * \brief file tg_global_application.cpp
 *
 * global application functionality
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */
#include "tg_global_application.h"
#include <thread>
#include "tg_global_log.h"
#include "../window/tg_mainwindow.h"
#include "private/tg_global_menu_holder.h"
#include "private/tg_global_wait_renderer.h"

TgGlobalApplication *TgGlobalApplication::m_globalApplication = nullptr;

TgGlobalApplication *TgGlobalApplication::getInstance()
{
    TG_FUNCTION_BEGIN();
    if (!TgGlobalApplication::m_globalApplication) {
        TgGlobalApplication::m_globalApplication = new TgGlobalApplication();
    }
    TG_FUNCTION_END();
    return TgGlobalApplication::m_globalApplication;
}

TgGlobalApplication::TgGlobalApplication() :
    m_exit(false)
{
    TgGlobalMenuHolder::getInstance();
}

/*!
 * \brief TgGlobalApplication::clear
 *
 * clears the global application (and eventually everything else)
 */
void TgGlobalApplication::clear()
{
    TG_FUNCTION_BEGIN();
    if (TgGlobalApplication::m_globalApplication) {
        delete TgGlobalApplication::m_globalApplication;
        TgGlobalApplication::m_globalApplication = nullptr;
    }
    TG_FUNCTION_END();
}

TgGlobalApplication::~TgGlobalApplication()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlobalApplication::getImageAssets
 *
 * get image assets
 *
 * \return global image assets
 */
TgImageAssets *TgGlobalApplication::getImageAssets()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return &m_imageAssets;
}

/*!
 * \brief TgGlobalApplication::getFontGlyphCache
 *
 * get font glyph cache
 *
 * \return global font glyph cache
 */
TgFontGlyphCache *TgGlobalApplication::getFontGlyphCache()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return &m_fontGlyphCache;
}

/*!
 * \brief TgGlobalApplication::getFontGlyphCacheData
 *
 * get font glyph cache (data)
 *
 * \return global font glyph cache (data)
 */
TgFontGlyphCacheData *TgGlobalApplication::getFontGlyphCacheData()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return &m_fontGlyphCacheData;
}


/*!
 * \brief TgGlobalApplication::getFontCharactersCache
 *
 * get font characters cache
 *
 * \return global font characters cache
 */
TgFontCharactersCache *TgGlobalApplication::getFontCharactersCache()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return &m_fontCharactersCache;
}


/*!
 * \brief TgGlobalApplication::getFontDefault
 *
 * get font default information
 *
 * \return global font default information
 */
TgFontDefault *TgGlobalApplication::getFontDefault()
{
    TG_FUNCTION_BEGIN();
    TG_FUNCTION_END();
    return &m_fontDefault;
}

/*!
 * \brief TgGlobalApplication::addMainWindow
 *
 * add main window to list
 * \param mainWindow main window
 */
void TgGlobalApplication::addMainWindow(TgMainWindow *mainWindow)
{
    TG_FUNCTION_BEGIN();
    m_mutex.lock();
    m_listMainWindow.push_back(mainWindow);
    m_mutex.unlock();
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlobalApplication::init
 *
 * init the application windows
 */
int TgGlobalApplication::init()
{
    int ret;
    std::vector<TgMainWindow *>::iterator it;
    m_mutex.lock();
    for (it=m_listMainWindow.begin();it!=m_listMainWindow.end();it++) {
        ret = (*it)->initWindow();
        if (ret != EXIT_SUCCESS) {
            m_mutex.unlock();
            return ret;
        }
    }
    m_mutex.unlock();
    return ret;
}

/*!
 * \brief TgGlobalApplication::render
 *
 * renders the window(s)
 */
void TgGlobalApplication::render()
{
    std::vector<TgMainWindow *>::iterator it;
    m_mutex.lock();
    for (it=m_listMainWindow.begin();it!=m_listMainWindow.end();it++) {
        (*it)->render(nullptr);
    }
    m_mutex.unlock();
}

#ifdef USE_GLFW
/*!
 * \brief TgGlobalApplication::addEvent
 *
 * add event
 * \param window
 * \param eventData
 */
void TgGlobalApplication::addEvent(GLFWwindow *window, const TgEventData *eventData)
{
    std::vector<TgMainWindow *>::iterator it;
    m_mutex.lock();
    for (it=m_listMainWindow.begin();it!=m_listMainWindow.end();it++) {
        (*it)->addEvent(window, eventData);
    }
    m_mutex.unlock();
}
#else
/*!
 * \brief TgGlobalApplication::waitForEnd
 *
 * wait for end the window(s)
 */
void TgGlobalApplication::waitForEnd()
{
    std::vector<TgMainWindow *>::iterator it;
    for (it=m_listMainWindow.begin();it!=m_listMainWindow.end();it++) {
        (*it)->waitForEnd();
    }
}
#endif
/*!
 * \brief TgGlobalApplication::exit
 *
 * quits the application
 */
void TgGlobalApplication::exit()
{
#ifndef USE_GLFW
    std::thread([this]() {
        TgGlobalWaitRenderer::getInstance()->exit();
        TgGlobalWaitRenderer::getInstance()->release(1);
        waitForEnd();
        m_exit = true;
    }).detach();
#else
    m_exit = true;
#endif
}

/*!
 * \brief TgGlobalApplication::getExit
 *
 * gets if app is closing
 */
bool TgGlobalApplication::getExit()
{
    return m_exit;
}
