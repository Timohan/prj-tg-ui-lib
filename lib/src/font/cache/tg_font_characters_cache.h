/*!
 * \file
 * \brief file tg_font_characters_cache.h
 *
 * font characters cache
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_CHARACTERS_CACHE_H
#define TG_FONT_CHARACTERS_CACHE_H

#include <vector>
#include <string>
#include <mutex>
#include <prj-ttf-reader.h>

struct TgFontCharacterCache
{
    prj_ttf_reader_supported_characters_t *m_supported_characters;
    std::string m_filename;
};

class TgFontCharactersCache
{
public:
    TgFontCharactersCache();
    ~TgFontCharactersCache();

    bool addFont(const std::string &filename);
    std::vector<int> getSupportedFontsForCharacters(const uint32_t *list_characters, const uint32_t list_characters_size, const std::vector<std::string>&listFontFiles);

    bool isCharacterForThisFont(const uint32_t character, const std::string &fontFileName);
    int getFontIndexForCharacter(const uint32_t character, const std::string &fontFileName, const std::vector<std::string>&listFontFiles);
private:
    std::vector<TgFontCharacterCache *>m_listCharacters;
    std::mutex m_mutex;

    static std::vector<int> getSupportedFontsForCharacters(const uint32_t *list_characters, const uint32_t list_characters_size, const std::vector<TgFontCharacterCache *>&listCharacters);

};

#endif // TG_FONT_CHARACTERS_CACHE_H
