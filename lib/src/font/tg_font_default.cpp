/*!
 * \file
 * \brief file tg_font_default.cpp
 *
 * holds the default font information
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_default.h"

#ifndef TG_FONT_DEFAULT_FILENAME
#define TG_FONT_DEFAULT_FILENAME "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
#endif

/*!
 * \brief TgFontDefault::getDefaultFont
 *
 * get font default (full filepath name)
 *
 * \return font default (full filepath)
 */
std::string TgFontDefault::getDefaultFont()
{
    if (m_defaultFont.empty()) {
        return TG_FONT_DEFAULT_FILENAME;
    }
    return m_defaultFont;
}

/*!
 * \brief TgFontDefault::setDefaultFont
 *
 * set font default (full filepath name)
 *
 * \param fullFilePathFont font default (full filepath)
 */
void TgFontDefault::setDefaultFont(std::string fullFilePathFont)
{
    m_defaultFont = fullFilePathFont;
}
