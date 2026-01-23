#include "tg_font_texture_cache.h"
#include "../../global/tg_global_log.h"


TgFontTextureCache::~TgFontTextureCache()
{
    size_t i, i2;
    for (i=0;i<m_listGlyphs.size();i++) {
        for (i2=0;i2<m_listGlyphs[i].m_listRender.size();i2++) {
            delete m_listGlyphs[i].m_listRender[i2];
        }
        m_listGlyphs[i].m_listRender.clear();
    }
    m_listGlyphs.clear();
}

void TgFontTextureCache::add(const std::vector<PrjTgFontDrawHelperData> &listHelperCharacters, const float fontSize, const std::string &fontFilename)
{
    size_t characterIndex;
    for (size_t i=0;i<listHelperCharacters.size();i++) {
        if (get(listHelperCharacters.at(i).m_character, fontSize, fontFilename, characterIndex)) {
            continue;
        }
        add(&listHelperCharacters[i], fontSize, fontFilename);
    }
}

void TgFontTextureCache::add(const PrjTgFontDrawHelperData *data, const float &fontSize, const std::string &fontFilename)
{
    size_t characterIndex;
    if (get(data->m_character, fontSize, fontFilename, characterIndex)) {
        return;
    }
    addTexture(data, fontSize, fontFilename);
}


TgFontTextureCacheItem *TgFontTextureCache::get(const uint32_t &character, const float &fontSize, const std::string &fontFilename, size_t &characterIndex)
{
    size_t i;
    for (i=0;i<m_listGlyphs.size();i++) {
        if (m_listGlyphs[i].m_fontFilename != fontFilename
            || std::abs(m_listGlyphs[i].m_fontSize - fontSize) > 0) {
            continue;
        }
        if (getCharacterIndexFromItem(&m_listGlyphs[i], character, characterIndex)) {
            return &m_listGlyphs[i];
        }
    }

    return nullptr;
}


bool TgFontTextureCache::getCharacterIndexFromItem(const TgFontTextureCacheItem *item, const uint32_t &character, size_t &characterIndex)
{
    size_t i;
    for (i=0;i<item->m_listCharacter.size();i++) {
        if (item->m_listCharacter[i] == character) {
            characterIndex = i;
            return true;
        }
    }
    return false;
}

void TgFontTextureCache::addTexture(const PrjTgFontDrawHelperData *data, const float &fontSize, const std::string &fontFilename)
{
    Vertice vertices[4];
    m_listGlyphs.push_back(TgFontTextureCacheItem());
    int x, y, i;
    unsigned char* imageData = nullptr;
    GLuint textureImage;
    int verticesIndex = 0;

    glGenTextures(1, &textureImage);

    if (!textureImage) {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureImage);
    imageData = new unsigned char[data->m_glyphOnImageDataWidth*data->m_glyphOnImageDataHeight*4];
    for (y=0;y<data->m_glyphOnImageDataHeight;y++) {
        for (x=0;x<data->m_glyphOnImageDataWidth;x++) {
            for (i=0;i<4;i++) {
                if (data->m_imageData[((y+data->m_glyphOnImageDataY)*data->m_imageWidth+x+data->m_glyphOnImageDataX)]) {
                    if (i == 3) {
                        imageData[(y*data->m_glyphOnImageDataWidth+x)*4+i] = 255;
                    } else {
                        imageData[(y*data->m_glyphOnImageDataWidth+x)*4+i] = data->m_imageData[((y+data->m_glyphOnImageDataY)*data->m_imageWidth+x+data->m_glyphOnImageDataX)];
                    }
                } else {
                    imageData[(y*data->m_glyphOnImageDataWidth+x)*4+i] = 0;
                }
            }
        }
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->m_glyphOnImageDataWidth, data->m_glyphOnImageDataHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    vertices[verticesIndex].x = 0;
    vertices[verticesIndex].y = 0;
    vertices[verticesIndex].s = 0;
    vertices[verticesIndex].t = 0;
    verticesIndex++;
    vertices[verticesIndex].x = static_cast<float>(data->m_glyphOnImageDataWidth);
    vertices[verticesIndex].y = 0;
    vertices[verticesIndex].s = 1;
    vertices[verticesIndex].t = 0;
    verticesIndex++;
    vertices[verticesIndex].x = 0;
    vertices[verticesIndex].y = static_cast<float>(data->m_glyphOnImageDataHeight);
    vertices[verticesIndex].s = 0;
    vertices[verticesIndex].t = 1;
    verticesIndex++;
    vertices[verticesIndex].x = static_cast<float>(data->m_glyphOnImageDataWidth);
    vertices[verticesIndex].y = static_cast<float>(data->m_glyphOnImageDataHeight);
    vertices[verticesIndex].s = 1;
    vertices[verticesIndex].t = 1;
    verticesIndex++;

    m_listGlyphs.back().m_fontFilename = fontFilename;
    m_listGlyphs.back().m_fontSize = fontSize;
    m_listGlyphs.back().m_listCharacter.push_back(data->m_character);
    m_listGlyphs.back().m_texture = textureImage;
    TgRender *render = new TgRender();
    render->init(vertices, verticesIndex);
    m_listGlyphs.back().m_listRender.push_back(render);
    delete[] imageData;
}
