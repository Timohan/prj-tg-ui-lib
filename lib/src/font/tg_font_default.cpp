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
#include <algorithm>

#ifndef TG_FONT_DEFAULT_FILENAME
#define TG_FONT_DEFAULT_FILENAME "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
#endif

TgFontDefault::TgFontDefault()
{
    setDefaultFont(TG_FONT_DEFAULT_FILENAME);
}

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
    setFont(fullFilePathFont, 0);
}

/*!
 * \brief TgFontText::getFontCount
 *
 * \return get font (filename) count
 */
size_t TgFontDefault::getFontCount()
{
    size_t ret;
    m_fontMutex.lock();
    ret = m_listFont.size();
    m_fontMutex.unlock();
    return ret;
}

/*!
 * \brief TgFontDefault::getListFont
 *
 * \return get list of fonts that application support
 */
std::vector<std::string> TgFontDefault::getListFont()
{
    std::vector<std::string> ret;
    m_fontMutex.lock();
    ret = m_listFont;
    m_fontMutex.unlock();
    return ret;
}


/*!
 * \brief TgFontDefault::setFont
 *
 * set/add font (full filepath) to font list into position
 *
 * \param fullFilePathFont add/set this font name
 * \param position font to this position, this position is larger
 * than current index (for example UINT64_MAX), then it adds/set this
 * font to last of the list
 * \return index position that font will be set/added
 */
size_t TgFontDefault::setFont(const std::string &fullFilePathFont, size_t position)
{
    size_t i, ret;
    m_fontMutex.lock();
    for (i=0;i<m_listFont.size();i++) {
        if (m_listFont.at(i) == fullFilePathFont) {
            if (i==position) {
                m_fontMutex.unlock();
                return i;
            }
            if (m_listFont.size() > position) {
                movePosition(m_listFont, i, position);
                if (position == 0) {
                    m_defaultFont = fullFilePathFont;
                }
                m_fontMutex.unlock();
                return position;
            }
            if (m_listFont.size()-1 != i) {
                movePosition(m_listFont, i, m_listFont.size()-1);
            }
            if (m_listFont.size() == 1) {
                m_defaultFont = fullFilePathFont;
            }
            ret = m_listFont.size()-1;
            m_fontMutex.unlock();
            return ret;
        }
    }
    if (m_listFont.size() > position) {
        m_listFont.insert(m_listFont.begin()+position, fullFilePathFont);
        if (position == 0) {
            m_defaultFont = fullFilePathFont;
        }
        m_fontMutex.unlock();
        return position;
    }
    m_listFont.push_back(fullFilePathFont);
    if (m_listFont.size() == 1) {
        m_defaultFont = fullFilePathFont;
    }
    ret = m_listFont.size()-1;
    m_fontMutex.unlock();
    return ret;
}

/*!
 * \brief TgItem2dPosition::movePosition
 *
 * move vector's "from" position to "to" position at "vec"
 * \param vec [in/out]
 * \param from from this index
 * \param to to this index
 */
void TgFontDefault::movePosition(std::vector<std::string> &vec, size_t from, size_t to)
{
    if (from == to) {
        return;
    }
    if (from > to) {
        std::rotate(vec.rend() - from - 1, vec.rend() - from, vec.rend() - to);
        return;
    }
    std::rotate(vec.begin() + from, vec.begin() + from + 1, vec.begin() + to + 1);
}

/*!
 * \brief TgFontText::getFont
 *
 * \param i
 * \return font (filename) for index
 */
std::string TgFontDefault::getFont(size_t i)
{
    m_fontMutex.lock();
    if (i >= m_listFont.size()) {
        m_fontMutex.unlock();
        return "";
    }
    std::string ret = m_listFont.at(i);
    m_fontMutex.unlock();
    return ret;
}

