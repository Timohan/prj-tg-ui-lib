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
#include <mutex>
#include <prj-ttf-reader.h>
#include <prj_tg_font_draw_helper.h>
#include "../../math/tg_matrix4x4.h"
#include "../../render/tg_render.h"
#include "../../item2d/tg_item2d.h"


struct TgFontText;

struct TgFontInfo
{
    std::vector<TgRender *>m_listRender;
    std::vector<GLuint> m_listTextureImage;
    std::vector<uint32_t>m_listCharacter;
    std::vector<int32_t>m_glyphDrawX;
    std::vector<int32_t>m_glyphDrawY;
    std::vector<int32_t>m_glyphPixelWidth;
    std::vector<size_t> m_lineIndex;
    std::vector<int32_t>m_glyphOffsetDrawX;
    std::vector<int32_t>m_glyphOffsetDrawY;
    float m_fontSize = 0;
    float m_fontHeight = 0;
    bool m_addedToCache = false;
};

class TgFontGlyphCache
{
public:
    explicit TgFontGlyphCache();
    ~TgFontGlyphCache();
    bool loadCharacters(const std::vector<uint32_t> &listCharacters,
                        const std::vector<std::string> &listFontFiles,
                        const float fontSize,
                        const uint32_t maxLineCount, const float maxLineWidth,
                        const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine,
                        std::vector<PrjTgFontDrawHelperData> &listHelperCharacters,
                        float &fontHeight);
    TgFontInfo *generateCacheForText(const std::vector<uint32_t> &listCharacters, const std::vector<std::string> &listFontFiles, float fontSize,
                                     const uint32_t maxLineCount, const float maxLineWidth,
                                     const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine);
    void render(TgFontText *fontText, const int vertexTransformIndex, const int shaderColorIndex, const std::vector<TgMatrix4x4>&listMatrix);
    void getTextPosition(TgFontText *fontText, size_t cursorPosition, float &positionX);
    size_t getTextCharacterIndex(TgFontText *fontText, const float x);
    bool loadCharactersToCache(const char *filename, const std::vector<uint32_t> &listCharacters, const float &fontSize);

private:
    PrjTgFontDrawHelper m_helper;
    std::mutex m_mutex;
};

#endif // TG_FONT_GLYPH_CACHE_H
