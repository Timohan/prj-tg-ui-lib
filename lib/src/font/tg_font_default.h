/*!
 * \file
 * \brief file tg_font_default.h
 *
 * holds the default font information
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_DEFAULT_H
#define TG_FONT_DEFAULT_H

#include <vector>
#include <string>
#include <prj-ttf-reader.h>

class TgFontDefault
{
public:
    std::string getDefaultFont();
    void setDefaultFont(std::string fullFilePathFont);

private:
    std::string m_defaultFont;
};

#endif // TG_FONT_DEFAULT_H
