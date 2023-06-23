/*!
 * \file
 * \brief file tg_font_glyph_cache_data.h
 *
 * font glyph cache, but without OpenGL Rendering cache
 * (Rendering required cache is in TgFontGlyphCache)
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_GLYPH_CACHE_DATA_H
#define TG_FONT_GLYPH_CACHE_DATA_H

#include <vector>
#include <string>
#include <prj-ttf-reader.h>
#include "../../math/tg_matrix4x4.h"

struct TgFontInfoData
{
    prj_ttf_reader_data_t *m_data = nullptr;
    std::string m_fontFile;
    float m_fontSize = 0;
    float m_fontHeight = 0;
    std::vector<uint32_t>m_listCharacter;
    std::vector<float>m_listTopPositionY;
    std::vector<float>m_listBottomPositionY;
    bool m_cached = false;
};

class TgFontGlyphCacheData
{
public:
    explicit TgFontGlyphCacheData();
    ~TgFontGlyphCacheData();

    TgFontInfoData *generateCacheForText(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize);
    TgFontInfoData *generateCache(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize);

    bool generateTextVertices(TgFontInfoData *newInfo, const std::vector<uint32_t> &listCharacters);
    void addCache(TgFontInfoData *data);
    static float getFontHeight(std::vector<TgFontInfoData *> &listFontInfo);
    static float getLineHeight(std::vector<TgFontInfoData *> &listFontInfo);
    static float getAllDrawTextHeight(uint32_t allLineCount, std::vector<TgFontInfoData *> &listFontInfo);
private:
    std::vector<TgFontInfoData *>m_listCachedData;

    TgFontInfoData *isFontCached(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize);

};

#endif // TG_FONT_GLYPH_CACHE_DATA_H