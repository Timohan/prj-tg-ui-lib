/*!
 * \file
 * \brief file tg_font_to_image.cpp
 *
 * Draws font (text) into image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_to_image.h"
#include <limits>
#include <cmath>
#include "../../global/tg_global_log.h"
#include "../cache/tg_font_glyph_cache.h"
#include "../../image/draw/tg_image_draw.h"

TgFontToImage::~TgFontToImage()
{
    if (m_fontText) {
        delete m_fontText;
    }
    m_listCharacter.clear();
    m_listText.clear();
}

/*!
 * \brief TgCharacterPositions::setListText
 *
 * set list text and generates m_fontText
 *
 * \param fontFile [in] font file of text
 * \param fontSize [in] font size
 * \param text [in] text
 */
void TgFontToImage::setListText(const char *fontFile, float fontSize, const char *text)
{
    if (m_fontText
        && m_listText.size() == 1 && m_listText.at(0).m_text == text
        && m_fontFile == fontFile
        && std::fabs(m_fontSize - fontSize) <= std::numeric_limits<double>::epsilon()) {
        return;
    }

    if (m_fontText) {
        delete m_fontText;
    }

    TgTextFieldText t;
    t.m_text = text;
    m_listText.clear();
    m_listText.push_back(t);
    m_fontFile = fontFile;
    m_fontSize = fontSize;

    m_listCharacter.clear();
    TgFontTextGenerator::getCharacters(m_listText, m_listCharacter);

    m_fontText = TgFontTextGenerator::generateFontTextInfo(m_listText, fontFile);
    m_fontText->generateFontTextInfoGlyphs(fontSize, false);
}

/*!
 * \brief TgCharacterPositions::fillIntoImage
 *
 * fill glyph into image
 *
 * \param image [in/out] text will be filled here
 * \param x [in] start X
 * \param y [in] start y
 * \param r [in] color r
 * \param g [in] color g
 * \param b [in] color b
 * \param a [in] color a
 * \param alignH [in] text align (horizontal)
 * \param alignV [in] text align (vertical)
 */
void TgFontToImage::fillIntoImage(TgImageAsset &image, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, TgTextfieldHorizontalAlign alignH, TgTextfieldVerticalAlign alignV)
{
    float sx, sy;
    size_t i;
    const prj_ttf_reader_glyph_data_t *left_glyph;
    float advanceX, leftPositionX = 0;

    switch (alignH) {
        case TgTextfieldHorizontalAlign::AlignLeft:
        default:
            sx = x;
            break;
        case TgTextfieldHorizontalAlign::AlignCenterH:
            sx = x - TgCharacterPositions::calculateTextWidth(m_fontText)/2;
            break;
        case TgTextfieldHorizontalAlign::AlignRight:
            sx = x - TgCharacterPositions::calculateTextWidth(m_fontText);
            break;
    }

    switch (alignV) {
        case TgTextfieldVerticalAlign::AlignTop:
        default:
            sy = y+m_fontText->getFontHeight();
            break;
        case TgTextfieldVerticalAlign::AlignCenterV:
            sy = y+m_fontText->getFontHeight()/2;
            break;
        case TgTextfieldVerticalAlign::AlignBottom:
            sy = y;
            break;
    }

    for (i=0;i<m_fontText->getCharacterCount();i++) {
        TgFontTextCharacterInfo *characterInfo = m_fontText->getCharacter(i);
        TgFontInfo *fontInfo = m_fontText->getFontInfo(i);
        const prj_ttf_reader_glyph_data_t *right_glyph = prj_ttf_reader_get_character_glyph_data(characterInfo->m_character, fontInfo->m_data);

        if (i > 0) {
            advanceX = TgCharacterPositions::calculateAdvance(left_glyph, right_glyph, fontInfo->m_data);
            leftPositionX += advanceX;
        }
        fillIntoImage(image, leftPositionX+sx, sy, r, g, b, a,
                                  right_glyph,
                                  &fontInfo->m_data->image);
        left_glyph = right_glyph;
    }
}

/*!
 * \brief TgCharacterPositions::fillIntoImage
 *
 * fill glyph into image
 *
 * \param image [in/out] glyph will be filled here
 * \param x [in] start X
 * \param y [in] start y
 * \param r [in] color r
 * \param g [in] color g
 * \param b [in] color b
 * \param a [in] color a
 * \param glyph [in] this glyph will be drawed
 * \param ttf_image [in] image that contains glyph
 */
void TgFontToImage::fillIntoImage(TgImageAsset &image, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
                                  const prj_ttf_reader_glyph_data_t *glyph,
                                  const prj_ttf_reader_image *ttf_image)
{
    int32_t ix, iy, dataIndex;
    for (ix=glyph->image_pixel_left_x;ix<=glyph->image_pixel_right_x;ix++) {
        if (static_cast<int32_t>(x)+ix-glyph->image_pixel_left_x < 0) {
            continue;
        }
        if (static_cast<int32_t>(static_cast<int32_t>(x)+ix-glyph->image_pixel_left_x) >= image.m_imageData.m_generatedImage.m_width) {
            return;
        }
        for (iy=glyph->image_pixel_top_y;iy<=glyph->image_pixel_bottom_y;iy++) {
            dataIndex = iy*ttf_image->width+ix;
            if (ttf_image->data[dataIndex]) {
                if (static_cast<int32_t>(y)+iy+glyph->image_pixel_bottom_y-glyph->image_pixel_offset_line_y < 0) {
                    continue;
                }
                if (static_cast<int32_t>(static_cast<int32_t>(y)+iy-glyph->image_pixel_bottom_y-glyph->image_pixel_offset_line_y) >= image.m_imageData.m_generatedImage.m_height) {
                    break;
                }
                TgImageDraw::addColor(image.m_imageData.m_generatedImage.m_imageData,
                           image.m_imageData.m_generatedImage.m_width,
                           image.m_imageData.m_generatedImage.m_height,
                           static_cast<uint32_t>(static_cast<int32_t>(x)+ix-glyph->image_pixel_left_x),
                           static_cast<uint32_t>(static_cast<int32_t>(y)+iy-glyph->image_pixel_bottom_y-glyph->image_pixel_offset_line_y),
                           static_cast<uint8_t>(static_cast<uint32_t>(ttf_image->data[dataIndex])*static_cast<uint32_t>(r)/255),
                           static_cast<uint8_t>(static_cast<uint32_t>(ttf_image->data[dataIndex])*static_cast<uint32_t>(g)/255),
                           static_cast<uint8_t>(static_cast<uint32_t>(ttf_image->data[dataIndex])*static_cast<uint32_t>(b)/255),
                           static_cast<uint8_t>(static_cast<uint32_t>(ttf_image->data[dataIndex])*static_cast<uint32_t>(a)/255));
            }
        }
    }
}
