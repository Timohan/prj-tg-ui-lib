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
#include <mutex>
#include <prj-ttf-reader.h>

class TgFontDefault
{
public:
    TgFontDefault();
    std::string getDefaultFont();
    void setDefaultFont(std::string fullFilePathFont);

    size_t getFontCount();
    size_t setFont(const std::string &fullFilePathFont, size_t position = UINT64_MAX);
    std::string getFont(size_t i);
    std::vector<std::string> getListFont();

    static std::vector<std::string> getFontFiles(const std::string &mainFontFile);
    static void getFontFileNames(const std::string &mainFontFile, const std::vector<std::string> &listFontFileNames,
                                  std::vector<std::string> &listFontFileNamesOut);
private:
    std::string m_defaultFont;
    std::mutex m_fontMutex;

    std::vector<std::string>m_listFont;

    static void movePosition(std::vector<std::string> &vec, size_t from, size_t to);
};

#endif // TG_FONT_DEFAULT_H
