/*!
 * \file
 * \brief file tg_font_glyph_cache.cpp
 *
 * font glyph cache
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_glyph_cache.h"
#include <cstring>
#include <algorithm>
#include <cmath>
#include "../../global/tg_global_log.h"
#include "../../shader/tg_shader_2d.h"
#include "../tg_character_positions.h"
#include "../tg_font_text.h"
#include "../../global/tg_global_application.h"
#define FONT_ACCURACY_VALUE 5

TgFontGlyphCache::TgFontGlyphCache()
{
}

TgFontGlyphCache::~TgFontGlyphCache()
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
 * \brief TgFontGlyphCache::isFontCached
 *
 * searches if font is already cached for it can use same vertixes
 * and texture
 *
 * \param listCharacters contains all charactes to make vertices and texture
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if not, otherwise cached TgFontInfo
 */
TgFontInfo *TgFontGlyphCache::isFontCached(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize)
{
    TG_FUNCTION_BEGIN();
    bool notFound;
    std::vector<TgFontInfo *>::iterator it;
    std::vector<uint32_t>::const_iterator itListCharacters;
    for (it=m_listCachedFont.begin();it!=m_listCachedFont.end();it++) {
        if (memcmp(&(*it)->m_fontSize, &fontSize, sizeof(float)) == 0
            && (*it)->m_fontFile.compare(fontFile) == 0) {
            notFound = false;
            for (itListCharacters=listCharacters.begin();itListCharacters!=listCharacters.end();itListCharacters++) {
                if (std::find((*it)->m_listCharacter.begin(), (*it)->m_listCharacter.end(), (*itListCharacters)) == (*it)->m_listCharacter.end()) {
                    notFound = true;
                    break;
                }
            }
            if (!notFound) {
                TG_FUNCTION_END();
                return (*it);
            }
        }
    }
    TG_FUNCTION_END();
    return nullptr;
}

/*!
 * \brief TgFontGlyphCache::generateCacheForText
 *
 * generates image's vertices and texture for text (from list of characters)
 * if possible for using these vertices and textures in others
 *
 * \param listCharacters list of characters
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \param onlyForCalculation if true, then this TgFontInfo is not set into cache
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfo *TgFontGlyphCache::generateCacheForText(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize, bool onlyForCalculation)
{
    TG_FUNCTION_BEGIN();
    TgFontInfo *ret = isFontCached(listCharacters, fontFile, fontSize);
    if (ret) {
        return ret;
    }

    std::string additionalCharactersToGlyph = "ABCQWERTYUIOPÅSDFGHJKLÖÄZXVNMqwertyuiopasdfgjhklöäzxcvbnm<>|;:,.-_€'*~^1234567890+'!\"#¤%&/()=?½@£$‰‚{[]}— ";
    uint32_t *list_additonal_characters = nullptr, list_additonal_characters_size;
    if (prj_ttf_reader_get_characters(additionalCharactersToGlyph.c_str(), &list_additonal_characters, &list_additonal_characters_size)
            || !list_additonal_characters_size) {
        TG_FUNCTION_END();
        return nullptr;
    }

    std::vector<uint32_t> newListCharacters = std::move(listCharacters);
    for (uint32_t i=0;i<list_additonal_characters_size;i++) {
        if (std::find(newListCharacters.begin(), newListCharacters.end(), list_additonal_characters[i]) == newListCharacters.end()
            && TgGlobalApplication::getInstance()->getFontCharactersCache()->isCharacterForThisFont(list_additonal_characters[i], fontFile) ) {
            newListCharacters.push_back(list_additonal_characters[i]);
        }
    }
    std::vector<uint32_t>::iterator it;
    it = std::find(newListCharacters.begin(), newListCharacters.end(), 'A');
    if (it != newListCharacters.end()) {
        newListCharacters.erase(it);
        // we set 'A' as a first character, because it's good for font height calculation
        newListCharacters.insert(newListCharacters.begin(), 'A');
    }

    ret = generateCache(newListCharacters, fontFile, fontSize, onlyForCalculation);
    free(list_additonal_characters);
    TG_FUNCTION_END();
    return ret;
}

/*!
 * \brief TgFontGlyphCache::generateCache
 *
 * generates image's vertices and texture
 *
 * \param listCharacters list of characters
 * \param fontFile full file path of the font file
 * \param fontSize font size
 * \return nullptr if fails, generated TgFontInfo otherwise
 */
TgFontInfo *TgFontGlyphCache::generateCache(const std::vector<uint32_t> &listCharacters, const char *fontFile, float fontSize, bool onlyForCalculation)
{
    TG_FUNCTION_BEGIN();
    prj_ttf_reader_data_t *data;
    data = prj_ttf_reader_init_data();
    if (!data) {
        TG_ERROR_LOG("prj_ttf_reader_init_data return failed");
        TG_FUNCTION_END();
        return nullptr;
    }
    if (prj_ttf_reader_generate_glyphs_list_characters(listCharacters.data(), static_cast<uint32_t>(listCharacters.size()), fontFile, fontSize, FONT_ACCURACY_VALUE, data)
        || !data->image.width
        || !data->image.height) {
        prj_ttf_reader_clear_data(&data);
        TG_ERROR_LOG("generating glyphs failed");
        TG_FUNCTION_END();
        return nullptr;
    }

    TgFontInfo *newInfo = new TgFontInfo;
    newInfo->m_data = data;
    if (!onlyForCalculation && !addImage(newInfo)) {
        TG_ERROR_LOG("creating the texture failed");
        prj_ttf_reader_clear_data(&data);
        delete newInfo;
        TG_FUNCTION_END();
        return nullptr;
    }

    if (!generateTextVertices(newInfo, listCharacters, onlyForCalculation)) {
        TG_ERROR_LOG("creating the text vertices failed");
        prj_ttf_reader_clear_data(&data);
        delete newInfo;
        TG_FUNCTION_END();
        return nullptr;
    }

    newInfo->m_fontFile = fontFile;
    newInfo->m_fontSize = fontSize;
    if (!onlyForCalculation) {
        newInfo->m_addedToCache = true;
        m_listCachedFont.push_back(newInfo);
    }
    TG_FUNCTION_END();
    return newInfo;
}

/*!
 * \brief TgFontGlyphCache::render
 *
 * Renders the text
 * \param fontText
 * \param vertexTransformIndex vertex tranform index in shader's uniform location
 * \param shaderColorIndex shader color index
 * \param listMatrix list of matrixes for each character to shader
 */
void TgFontGlyphCache::render(TgFontText *fontText, const int vertexTransformIndex, const int shaderColorIndex, const std::vector<TgMatrix4x4>&listMatrix)
{
    size_t i, c = fontText->getCharacterCount();
    uint8_t r = 0, g = 0, b = 0;
    bool colorFirstTime = true;
    for (i=0;i<c;i++) {
        if (!fontText->getFontInfo(i)
            || fontText->getCharacter(i)->m_character == '\n'
            || !fontText->getCharacter(i)->m_draw
            || fontText->getCharacter(i)->m_fontFileNameIndex == -1) {
            continue;
        }
        glUniformMatrix4fv(vertexTransformIndex, 1, 0, listMatrix[i].getMatrixTable()->data);
        if (fontText->getCharacter(i)->m_textColorR != r
            || fontText->getCharacter(i)->m_textColorG != g
            || fontText->getCharacter(i)->m_textColorB != b
            || colorFirstTime) {
            glUniform4f(shaderColorIndex,
                static_cast<float>(fontText->getCharacter(i)->m_textColorR )/255.0f,
                static_cast<float>(fontText->getCharacter(i)->m_textColorG )/255.0f,
                static_cast<float>(fontText->getCharacter(i)->m_textColorB )/255.0f, 1);
            r = fontText->getCharacter(i)->m_textColorR;
            g = fontText->getCharacter(i)->m_textColorG;
            b = fontText->getCharacter(i)->m_textColorB;
            colorFirstTime = false;
        }

        fontText->getFontInfo(i)->m_listRender[ fontText->getCharacter(i)->m_characterInFontInfoIndex ]->render( fontText->getFontInfo(i)->m_textureImage, 0, 4, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
}

/*!
 * \brief TgFontGlyphCache::getTextPosition
 *
 * get cursor drawing relative position (x)
 * \param fontText
 * \param cursorPosition 0 == first character, 1 == second character
 * \param positionX [out] cursorPosition's drawing relative position
 */
void TgFontGlyphCache::getTextPosition(TgFontText *fontText, size_t cursorPosition, float &positionX)
{
    if (!fontText) {
        positionX = 0;
        return;
    }
    if (cursorPosition < fontText->getCharacterCount()) {
        positionX = fontText->getCharacter(cursorPosition)->positionLeftX;
    } else if (cursorPosition == fontText->getCharacterCount()) {
        positionX = fontText->getTextWidth();
    }
}

size_t TgFontGlyphCache::getTextCharacterIndex(TgFontText *fontText, const float x)
{
    TG_FUNCTION_BEGIN();
    if (x <= 0) {
        TG_FUNCTION_END();
        return 0;
    }

    if (x >= fontText->getTextWidth()) {
        TG_FUNCTION_END();
        return fontText->getCharacterCount();
    }

    const size_t characterCount = fontText->getCharacterCount();
    size_t ret = static_cast<size_t>(x*static_cast<float>(characterCount)/fontText->getTextWidth());
    float characterPositionX;
    int previousAdd = 0;
    while (1) {
        getTextPosition(fontText, ret, characterPositionX);
        if (characterPositionX < x) {
            if (ret == characterCount) {
                TG_FUNCTION_END();
                return ret;
            }
            if (previousAdd == -1) {
                TG_FUNCTION_END();
                return ret;
            }
            ret++;
            previousAdd = 1;
        } else if (characterPositionX > x) {
            if (ret == 0) {
                TG_FUNCTION_END();
                return ret;
            }
            if (previousAdd == 1) {
                if (ret > 0) {
                    TG_FUNCTION_END();
                    return ret - 1;
                }
                TG_FUNCTION_END();
                return ret;
            }
            ret--;
            previousAdd = -1;
        } else {
            TG_FUNCTION_END();
            return ret;
        }
    }

    TG_FUNCTION_END();
}

/*!
 * \brief TgFontGlyphCache::generateTextVertices
 *
 * generate text vertices for the text
 *
 * \param newInfo [in/out] info
 * \param listCharacters contains the text to render
 * \param onlyForCalculation if true, then this TgFontInfo is not set into cache
 * \return true on success
 */
bool TgFontGlyphCache::generateTextVertices(TgFontInfo *newInfo, const std::vector<uint32_t> &listCharacters, bool onlyForCalculation)
{
    TG_FUNCTION_BEGIN();
    size_t i, c = listCharacters.size();
    float bottomY;
    bool firstTime = true;
    Vertice *vertices;
    const prj_ttf_reader_glyph_data_t *glyph;

    vertices = new Vertice[4];
    for (i=0;i<c;i++) {
        if (!onlyForCalculation) {
            newInfo->m_listRender.push_back(new TgRender());
        }
        newInfo->m_listCharacter.push_back(listCharacters.at(i));

        glyph = prj_ttf_reader_get_character_glyph_data(listCharacters.at(i), newInfo->m_data);
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
        if (!onlyForCalculation) {
            newInfo->m_listRender.back()->init(vertices, 4, true);
        }
    }


    delete[]vertices;
    TG_FUNCTION_END();
    return true;
}

/*!
 * \brief TgFontGlyphCache::addImage
 *
 * Create image from data to 2D texture image in TgFontInfo
 *
 * \param newInfo [in/out] texture will be set here
 * \return true on success
 */
bool TgFontGlyphCache::addImage(TgFontInfo *newInfo)
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
void TgFontGlyphCache::generateCharactedIndexToUtf8(uint32_t character, char newCharacter[5])
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