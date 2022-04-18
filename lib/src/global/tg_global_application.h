/*!
 * \file
 * \brief file tg_global_application.h
 *
 * global application functionality
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GLOBAL_APPLICATION_H
#define TG_GLOBAL_APPLICATION_H

#include "tg_global_macros.h"
#include "../image/tg_image_assets.h"
#include <vector>
#include <mutex>
#include "../font/cache/tg_font_glyph_cache.h"
#include "../font/cache/tg_font_characters_cache.h"
#include "../font/tg_font_default.h"

class TgMainWindow;
#ifdef USE_GLFW
struct GLFWwindow;
#endif
struct TgEventData;

class TG_MAINWINDOW_EXPORT TgGlobalApplication
{
public:
    TgGlobalApplication();
    ~TgGlobalApplication();
    static TgGlobalApplication *getInstance();
    static void clear();
    void addMainWindow(TgMainWindow *mainWindow);
    int init();
    void render();
    TgImageAssets *getImageAssets();
    TgFontGlyphCache *getFontGlyphCache();
    TgFontCharactersCache *getFontCharactersCache();
    TgFontDefault *getFontDefault();
#ifdef USE_GLFW
    void addEvent(GLFWwindow *window, const TgEventData *eventData);
#endif
    void exit();
    bool getExit();

private:
    static TgGlobalApplication *m_globalApplication;
    bool m_exit;
    TgImageAssets m_imageAssets;
    TgFontGlyphCache m_fontGlyphCache;
    TgFontCharactersCache m_fontCharactersCache;
    TgFontDefault m_fontDefault;
    std::recursive_mutex m_mutex;

    std::vector<TgMainWindow *>m_listMainWindow;
};

#endif // TG_GLOBAL_APPLICATION_H
