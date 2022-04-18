/*!
 * \file
 * \brief file tg_font_glyph_cache.h
 *
 * font glyph cache
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_GLYPH_CACHE_H
#define TG_FONT_GLYPH_CACHE_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <prj-ttf-reader.h>
#include "../../math/tg_matrix4x4.h"
#include "../../render/tg_render.h"

struct TgFontText;

struct TgFontInfo
{
    prj_ttf_reader_data_t *m_data;
    GLuint m_textureImage;
    std::vector<TgRender *>m_listRender;
    std::vector<uint32_t>m_listCharacter;
    std::vector<float>m_listTopPositionY;
    std::vector<float>m_listBottomPositionY;
    std::string m_fontFile;
    float m_fontSize;
    float m_fontHeight;
};

class TgFontGlyphCache
{
public:
    explicit TgFontGlyphCache();
    ~TgFontGlyphCache();
    TgFontInfo *generateCacheForText(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize);
    void render(TgFontText *fontText, const int vertexTransformIndex, const std::vector<TgMatrix4x4>&listMatrix);

private:
    std::vector<TgFontInfo *>m_listCachedFont;
    TgFontInfo *isFontCached(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize);
    TgFontInfo *generateCache(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize);

    static bool generateTextVertices(TgFontInfo *newInfo, const std::vector<uint32_t> &listCharacters);
    static bool addImage(TgFontInfo *newInfo);

    static void generateCharactedIndexToUtf8(uint32_t character, char text[5]);
};

#endif // TG_FONT_GLYPH_CACHE_H
