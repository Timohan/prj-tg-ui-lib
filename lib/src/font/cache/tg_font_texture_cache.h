/*!
 * \file
 * \brief file tg_font_texture_cache.h
 *
 * font texture cache
 * when glyph(s) is loaded from TgFontGlyphCache::generateCacheForText()
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_TEXTURE_CACHE_H
#define TG_FONT_TEXTURE_CACHE_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <prj_tg_font_draw_helper.h>
#include "../../math/tg_matrix4x4.h"
#include "../../render/tg_render.h"

struct TgFontTextureCacheItem
{
    std::vector<uint32_t> m_listCharacter;
    std::vector<TgRender *> m_listRender;
    GLuint m_texture;
    float m_fontSize;
    std::string m_fontFilename;
};

class TgFontTextureCache
{
public:
    ~TgFontTextureCache();
    void add(const std::vector<PrjTgFontDrawHelperData> &listHelperCharacters, const float fontSize, const std::string &fontFilename);
    void add(const PrjTgFontDrawHelperData *data, const float &fontSize, const std::string &fontFilename);
    TgFontTextureCacheItem *get(const uint32_t &character, const float &fontSize, const std::string &fontFilename, size_t &characterIndex);

private:
    std::vector<TgFontTextureCacheItem> m_listGlyphs;

    static bool getCharacterIndexFromItem(const TgFontTextureCacheItem *item, const uint32_t &character, size_t &characterIndex);

    void addTexture(const PrjTgFontDrawHelperData *data, const float &fontSize, const std::string &fontFilename);
};

#endif // TG_FONT_TEXTURE_CACHE_H
