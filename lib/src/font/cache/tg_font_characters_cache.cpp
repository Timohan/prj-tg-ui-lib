/*!
 * \file
 * \brief file tg_font_characters_cache.cpp
 *
 * font characters cache
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_characters_cache.h"
#include "../../global/tg_global_log.h"

TgFontCharactersCache::TgFontCharactersCache()
{

}

TgFontCharactersCache::~TgFontCharactersCache()
{
    size_t i;
    m_mutex.lock();
    for (i=0;i<m_listCharacters.size();i++) {
        if (m_listCharacters[i]->m_supported_characters) {
            prj_ttf_reader_clear_supported_character(&m_listCharacters[i]->m_supported_characters);
        }
        delete m_listCharacters[i];
    }
    m_listCharacters.clear();
    m_mutex.unlock();
}

/*!
 * \brief TgFontCharactersCache::addFont
 *
 * add new font to m_listCharacters list
 * if font is already in the list, it's not going to be added
 *
 * \param filename [in]
 * \return true, if the font was added (or it's already in the list)
 */

bool TgFontCharactersCache::addFont(const std::string &filename)
{
    m_mutex.lock();
    std::vector<TgFontCharacterCache *>::iterator it;
    for (it=m_listCharacters.begin();it!=m_listCharacters.end();it++) {
        if ((*it)->m_filename == filename) {
            m_mutex.unlock();
            return true;
        }
    }

    TgFontCharacterCache *cache = new TgFontCharacterCache;
    cache->m_filename = filename;
    cache->m_supported_characters = prj_ttf_reader_init_supported_character();
    if (!cache->m_supported_characters) {
        TG_ERROR_LOG("Failed to init supported characters");
        delete cache;
        m_mutex.unlock();
        return false;
    }
    if (prj_ttf_reader_get_supported_characters(filename.c_str(), cache->m_supported_characters)) {
        TG_ERROR_LOG("Could not get supported characters from font: ", filename);
        prj_ttf_reader_clear_supported_character(&cache->m_supported_characters);
        delete cache;
        m_mutex.unlock();
        return false;
    }
    m_listCharacters.push_back(cache);
    m_mutex.unlock();
    return true;
}

/*!
 * \brief TgFontCharactersCache::getSupportedFontsForCharacters
 *
 * \param character [in]
 * \param fontFileName [in]
 * \param listCharacters [in]
 * \return generated list of, which character belongs to which listCharacters (as in index),
 * for example, if first value is 0, that character belongs to first font from listCharacters
 * for example, if second value is 1, that character belongs to second font from listCharacters
 * -1 is ignored (not found)
 */
std::vector<int> TgFontCharactersCache::getSupportedFontsForCharacters(const uint32_t *list_characters, const uint32_t list_characters_size,
                                                                       const std::vector<TgFontCharacterCache *>&listCharacters)
{
    std::vector<int>ret;
    uint32_t i, i3;
    size_t i2;
    bool found;
    int previousFontForCharacter = -1;
    for (i=0;i<list_characters_size;i++) {
        found = false;
        if (previousFontForCharacter >= 0) {
            for (i3=0;i3<listCharacters[previousFontForCharacter]->m_supported_characters->character_list_count;i3++) {
                if (listCharacters[previousFontForCharacter]->m_supported_characters->list_character[i3] == list_characters[i]) {
                    ret.push_back(previousFontForCharacter);
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            continue;
        }
        for (i2=0;i2<listCharacters.size();i2++) {
            for (i3=0;i3<listCharacters[i2]->m_supported_characters->character_list_count;i3++) {
                if (listCharacters[i2]->m_supported_characters->list_character[i3] == list_characters[i]) {
                    previousFontForCharacter = static_cast<int>(i2);
                    ret.push_back(static_cast<int>(i2));
                    found = true;
                    i2 = listCharacters.size();
                    break;
                }
            }
        }
        if (!found) {
            ret.push_back(-1);
        }
    }
    return ret;
}

/*!
 * \brief TgFontCharactersCache::isCharacterForThisFont
 *
 * \param character [in]
 * \param fontFileName [in]
 * \return true if character exists in the font
 */
bool TgFontCharactersCache::isCharacterForThisFont(const uint32_t character, const std::string &fontFileName)
{
    uint32_t i;
    std::vector<TgFontCharacterCache *>::const_iterator it;
    if (fontFileName.empty()) {
        return false;
    }
    for (it=m_listCharacters.begin();it!=m_listCharacters.end();it++) {
        if ((*it)->m_filename == fontFileName) {
            for (i=0;i<(*it)->m_supported_characters->character_list_count;i++) {
                if (character == (*it)->m_supported_characters->list_character[i]) {
                    return true;
                }
            }
        }
    }
    return false;
}

/*!
 * \brief TgFontCharactersCache::getFontIndexForCharacter
 *
 * \param character [in]
 * \param fontFileName [in] first (main) font to use search character, fontFileName must be in listFontFiles
 * \param listFontFiles [in] list of fonts in files
 * \return index of font
 */
int TgFontCharactersCache::getFontIndexForCharacter(const uint32_t character, const std::string &fontFileName, const std::vector<std::string>&listFontFiles)
{
    size_t i;
    if (isCharacterForThisFont(character, fontFileName)) {
        for (i=0;i<listFontFiles.size();i++) {
            if (listFontFiles.at(i) == fontFileName) {
                return static_cast<int>(i);
            }
        }
    }

    for (i=0;i<listFontFiles.size();i++) {
        if (listFontFiles.at(i) != fontFileName) {
            if (isCharacterForThisFont(character, listFontFiles.at(i))) {
                return static_cast<int>(i);
            }
        }
    }

    return -1;
}

/*!
 * \brief TgFontCharactersCache::getSupportedFontsForCharacters
 *
 * get supported fonts for (list of) characters
 *
 * \param list_characters [in] list of characters
 * \param list_characters_size [in] list of characters index
 * \param listFontFiles [in]
 * \return index position of font for each character
 */
std::vector<int> TgFontCharactersCache::getSupportedFontsForCharacters(const uint32_t *list_characters, const uint32_t list_characters_size,
                                                                       const std::vector<std::string> &listFontFiles)
{
    std::vector<int>ret;
    bool found = false;
    size_t i, listCharactersIndex;
    std::vector<TgFontCharacterCache *> listCharacters;

    for (i=0;i<listFontFiles.size();i++) {
        for (listCharactersIndex=0;listCharactersIndex<m_listCharacters.size();listCharactersIndex++) {
            if (listFontFiles.at(i) == m_listCharacters.at(listCharactersIndex)->m_filename) {
                listCharacters.push_back(m_listCharacters.at(listCharactersIndex));
                found = true;
                break;
            }
        }
        if (!found && addFont(listFontFiles.at(i))) {
            listCharacters.push_back(m_listCharacters.back());
        }
    }

    return getSupportedFontsForCharacters(list_characters, list_characters_size, listCharacters);
}

