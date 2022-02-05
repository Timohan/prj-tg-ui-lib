/*!
 * \file
 * \brief file tg_font_cache.cpp
 *
 * font cache
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_cache.h"
#include <cstring>
#include <algorithm>
#include "../global/tg_global_log.h"
#include "../shader/tg_shader_2d.h"
#include "tg_character_positions.h"

#define FONT_ACCURACY_VALUE 5

TgFontCache::TgFontCache()
{
}

TgFontCache::~TgFontCache()
{
    std::vector<TgFontInfo *>::iterator it;
    std::vector<TgRender *>::iterator itRender;
    for (it=m_listCachedFont.begin();it!=m_listCachedFont.end();it++) {
        prj_ttf_reader_clear_data(&(*it)->m_data);
        for (itRender=(*it)->m_listRender.begin();itRender!=(*it)->m_listRender.end();itRender++) {
            delete (*itRender);
        }
        (*it)->m_listRender.clear();
        delete (*it);
    }
    m_listCachedFont.clear();
}

/*!
 * \brief TgFontCache::isFontCached
 *
 * searches if font is already cached for it can use same vertixes
 * and texture
 *
 * \param text contains all charactes to make vertices and texture
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if not, otherwise cached TgFontInfo
 */
TgFontInfo *TgFontCache::isFontCached(const char *text, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    bool notFound;
    size_t i;
    uint32_t *list_characters = nullptr, list_characters_size = 0;
    std::vector<TgFontInfo *>::iterator it;
    for (it=m_listCachedFont.begin();it!=m_listCachedFont.end();it++) {
        if (memcmp(&(*it)->m_fontSize, &fontSize, sizeof(float)) == 0
            && (*it)->m_fontFile.compare(fontFile) == 0) {
            if (!list_characters) {
                if (prj_ttf_reader_get_characters(text, &list_characters, &list_characters_size)
                    || !list_characters_size) {
                    free(list_characters);
                    return nullptr;
                }
            }
            notFound = false;
            for (i=0;i<list_characters_size;i++) {
                if (std::find((*it)->m_listCharacter.begin(), (*it)->m_listCharacter.end(), list_characters[i]) == (*it)->m_listCharacter.end()) {
                    notFound = true;
                    break;
                }
            }
            if (!notFound) {
                free(list_characters);
                TG_FUNCTION_END();
                return (*it);
            }
        }
    }
    free(list_characters);
    TG_FUNCTION_END();
    return nullptr;
}

/*!
 * \brief TgFontCache::generateCacheForText
 *
 * generates image's vertices and texture for text (but also adds extra latin characters
 * if possible for using these vertices and textures in others
 *
 * \param text contains all charactes to make vertices and texture
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfo *TgFontCache::generateCacheForText(const char *text, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    TgFontInfo *ret = isFontCached(text, fontFile, fontSize);
    if (ret) {
        return ret;
    }
    std::string charactersToGlyph = "ABCQjWERTYUIOPÅSDFGHJKLÖÄZXVNMqwertyuiopasdfghklöäzxcvbnm<>|;:,.-_€'*~^1234567890+'!\"#¤%&/()=?½@£$‰‚{[]} ";
    uint32_t *list_characters = nullptr, list_characters_size;
    if (prj_ttf_reader_get_characters(charactersToGlyph.c_str(), &list_characters, &list_characters_size)
            || !list_characters_size) {
        free(list_characters);
        TG_FUNCTION_END();
        return nullptr;
    }
    char newCharacter[5];
    uint32_t *list_characters2 = nullptr, list_characters_size2, i;
    if (prj_ttf_reader_get_characters(text, &list_characters2, &list_characters_size2)
            || !list_characters_size2) {
        free(list_characters);
        free(list_characters2);
        TG_FUNCTION_END();
        return nullptr;
    }

    for (i=0;i<list_characters_size2;i++) {
        if (!isCharacterInListOfCharacters(list_characters, list_characters_size, list_characters2[i])
            && !isCharacterInListOfCharacters(list_characters2, i, list_characters2[i])) {
            generateCharactedIndexToUtf8(list_characters2[i], newCharacter);
            charactersToGlyph += newCharacter;
        }
    }

    ret = generateCache(charactersToGlyph.c_str(), fontFile, fontSize);
    free(list_characters);
    free(list_characters2);
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgFontCache::generateCache
 *
 * generates image's vertices and texture
 *
 * \param text contains all charactes to make vertices and texture
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfo *TgFontCache::generateCache(const char *text, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    prj_ttf_reader_data_t *data;
    data = prj_ttf_reader_init_data();
    if (!data) {
        TG_ERROR_LOG("prj_ttf_reader_init_data return failed");
        TG_FUNCTION_END();
        return nullptr;
    }
    if (prj_ttf_reader_generate_glyphs_utf8(text, fontFile, fontSize, FONT_ACCURACY_VALUE, data)
        || !data->image.width
        || !data->image.height) {
        prj_ttf_reader_clear_data(&data);
        TG_ERROR_LOG("generating glyphs failed");
        TG_FUNCTION_END();
        return nullptr;
    }

    TgFontInfo *newInfo = new TgFontInfo;
    newInfo->m_data = data;
    if (!addImage(newInfo)) {
        TG_ERROR_LOG("creating the texture failed");
        prj_ttf_reader_clear_data(&data);
        delete newInfo;
        TG_FUNCTION_END();
        return nullptr;
    }
    if (!generateTextVertices(newInfo, text)) {
        TG_ERROR_LOG("creating the text vertices failed");
        prj_ttf_reader_clear_data(&data);
        delete newInfo;
        TG_FUNCTION_END();
        return nullptr;
    }

    newInfo->m_fontFile = fontFile;
    newInfo->m_fontSize = fontSize;
    m_listCachedFont.push_back(newInfo);
    TG_FUNCTION_END();
    return newInfo;
}


/*!
 * \brief TgFontCache::render
 *
 * Renders the text
 * \param info
 * \param charPosition
 * \param vertexTransformIndex vertex tranform index in shader's uniform location
 * \param listMatrix list of matrixes for each character to shader
 */
void TgFontCache::render(const TgFontInfo *info, const TgCharPosition *charPosition, const int vertexTransformIndex, const std::vector<TgMatrix4x4>&listMatrix)
{
    size_t i;
    for (i=0;i<charPosition->m_listCharacterInFontInfoIndex.size();i++) {
        glUniformMatrix4fv(vertexTransformIndex, 1, 0, listMatrix[i].getMatrixTable()->data);
        info->m_listRender[ charPosition->m_listCharacterInFontInfoIndex.at(i) ]->render( info->m_textureImage, 0, 4, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
}

/*!
 * \brief TgFontCache::generateTextVertices
 *
 * generate text vertices for the text
 *
 * \param newInfo [in/out] info
 * \param text contains the text to render
 * \return true on success
 */
bool TgFontCache::generateTextVertices(TgFontInfo *newInfo, const char *text)
{
    TG_FUNCTION_BEGIN();
    uint32_t i;
    float bottomY;
    bool firstTime = true;
    Vertice *vertices;
    const prj_ttf_reader_glyph_data_t *glyph;
    uint32_t *list_characters = nullptr, list_characters_size;

    if (prj_ttf_reader_get_characters(text, &list_characters, &list_characters_size)
            || !list_characters_size) {
        free(list_characters);
        TG_FUNCTION_END();
        return false;
    }

    vertices = new Vertice[4];
    for (i=0;i<list_characters_size;i++) {
        newInfo->m_listRender.push_back(new TgRender());
        newInfo->m_listCharacter.push_back(list_characters[i]);

        glyph = prj_ttf_reader_get_character_glyph_data(list_characters[i], newInfo->m_data);
        if (!glyph) {
            continue;
        }

        if (firstTime) {
            // set the line y position
            bottomY = static_cast<float>(glyph->image_pixel_bottom_y-glyph->image_pixel_top_y+glyph->image_pixel_offset_line_y);
            firstTime = false;
            newInfo->m_fontHeight = bottomY;
        }

        vertices[0].x = 0;
        vertices[0].y = bottomY - static_cast<float>(glyph->image_pixel_bottom_y-glyph->image_pixel_top_y) - static_cast<float>(glyph->image_pixel_offset_line_y);
        vertices[0].s = static_cast<float>(glyph->image_pixel_left_x)/static_cast<float>(newInfo->m_data->image.width);
        vertices[0].t = static_cast<float>(glyph->image_pixel_top_y)/static_cast<float>(newInfo->m_data->image.height);

        vertices[1].x = static_cast<float>(glyph->image_pixel_right_x - glyph->image_pixel_left_x);
        vertices[1].y = bottomY - static_cast<float>(glyph->image_pixel_bottom_y-glyph->image_pixel_top_y) - static_cast<float>(glyph->image_pixel_offset_line_y);
        vertices[1].s = static_cast<float>(glyph->image_pixel_right_x)/static_cast<float>(newInfo->m_data->image.width);
        vertices[1].t = static_cast<float>(glyph->image_pixel_top_y)/static_cast<float>(newInfo->m_data->image.height);

        vertices[2].x = 0;
        vertices[2].y = bottomY - static_cast<float>(glyph->image_pixel_offset_line_y);
        vertices[2].s = static_cast<float>(glyph->image_pixel_left_x)/static_cast<float>(newInfo->m_data->image.width);
        vertices[2].t = static_cast<float>(glyph->image_pixel_bottom_y)/static_cast<float>(newInfo->m_data->image.height);

        vertices[3].x = static_cast<float>(glyph->image_pixel_right_x - glyph->image_pixel_left_x);
        vertices[3].y = bottomY - static_cast<float>(glyph->image_pixel_offset_line_y);
        vertices[3].s = static_cast<float>(glyph->image_pixel_right_x)/static_cast<float>(newInfo->m_data->image.width);
        vertices[3].t = static_cast<float>(glyph->image_pixel_bottom_y)/static_cast<float>(newInfo->m_data->image.height);

        newInfo->m_listTopPositionY.push_back(vertices[0].y);
        newInfo->m_listBottomPositionY.push_back(static_cast<float>(glyph->image_pixel_offset_line_y*-1));
        newInfo->m_listRender.back()->init(vertices, 4, true);
    }

    free(list_characters);
    delete[]vertices;
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgFontCache::addImage
 *
 * Create image from data to 2D texture image in TgFontInfo
 *
 * \param newInfo [in/out] texture will be set here
 * \return true on success
 */
bool TgFontCache::addImage(TgFontInfo *newInfo)
{
    TG_FUNCTION_BEGIN();
    int x, y, i;
    unsigned char* imageData = nullptr;
    newInfo->m_textureImage = 0;
    glGenTextures(1, &newInfo->m_textureImage);

    if (!newInfo->m_textureImage) {
        TG_ERROR_LOG("generating the texture failed");
        TG_FUNCTION_END();
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, newInfo->m_textureImage);
    imageData = new unsigned char[newInfo->m_data->image.width*newInfo->m_data->image.height*4];

    for (x=0;x<newInfo->m_data->image.width;x++) {
        for (y=0;y<newInfo->m_data->image.height;y++) {
            for (i=0;i<4;i++) {
                imageData[(y*newInfo->m_data->image.width+x)*4+i] = newInfo->m_data->image.data[(y*newInfo->m_data->image.width+x)];
            }
        }
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newInfo->m_data->image.width, newInfo->m_data->image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    delete[]imageData;
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief DrawText::generateCharactedIndexToUtf8
 *
 * \param character [in] fills character to newCharacter as utf8
 * \param newCharacter [out] character will be filled here as in utf8
 */
void TgFontCache::generateCharactedIndexToUtf8(uint32_t character, char newCharacter[5])
{
    TG_FUNCTION_BEGIN();
    memset(newCharacter, '\0', 5);
    if (character >= 1 && character <= 127) {
        newCharacter[0] = static_cast<char>(character);
        TG_FUNCTION_END();
        return;
    }

    // 110xxxxx 10xxxxxx == 2 bytes
    // 0x0080 to 0x07FF
    if (character >= 0x0080 && character <= 0x07FF) {
        newCharacter[0] = static_cast<char>(0xC0 | static_cast<char>(character >> 6));
        newCharacter[1] = static_cast<char>(0x80 | static_cast<char>(character & 0x3F));
        TG_FUNCTION_END();
        return;
    }

    if (character >= 0x0800 && character <= 0xFFFF) {
        newCharacter[0] = static_cast<char>(0xE0 | static_cast<char>(character >> 12));
        newCharacter[1] = static_cast<char>(0x80 | static_cast<char>( (character >> 6) &  0x3F));
        newCharacter[2] = static_cast<char>(0x80 | static_cast<char>(character & 0x3F));
        TG_FUNCTION_END();
        return;
    }

    if (character >= 0x10000 && character <= 0x10FFFF) {
        newCharacter[0] = static_cast<char>(0xF0 | static_cast<char>(character >> 18));
        newCharacter[1] = static_cast<char>(0x80 | static_cast<char>( (character >> 12) &  0x3F));
        newCharacter[2] = static_cast<char>(0x80 | static_cast<char>( (character >> 6) &  0x3F));
        newCharacter[3] = static_cast<char>(0x80 | static_cast<char>(character & 0x3F));
        TG_FUNCTION_END();
        return;
    }
    TG_FUNCTION_END();
}

/*!
 * \brief TgFontCache::isCharacterInListOfCharacters
 *
 * checks if the character is in the list_characters
 *
 * \param list_characters
 * \param list_characters_size
 * \param character
 * \return true if the character is in the list_characters
 */
bool TgFontCache::isCharacterInListOfCharacters(const uint32_t *list_characters, const uint32_t list_characters_size, const uint32_t character)
{
    uint32_t i;
    for (i=0;i<list_characters_size;i++) {
        if (list_characters[i] == character) {
            return true;
        }
    }
    return false;
}
