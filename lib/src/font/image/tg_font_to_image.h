/*!
 * \file
 * \brief file tg_font_to_image.h
 *
 * Draws font (text) into image
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_TO_IMAGE_H
#define TG_FONT_TO_IMAGE_H

#include <vector>
#include <string>
#include <prj-ttf-reader.h>

#include "../../image/tg_image_assets.h"
#include "../../item2d/tg_textfield.h"
#include "../tg_font_text_generator.h"

class TgFontToImage
{
public:
    ~TgFontToImage();
    void fillIntoImage(TgImageAsset &image, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, TgTextfieldHorizontalAlign alignH, TgTextfieldVerticalAlign alignV);
    void setListText(const char *fontFile, float fontSize, const char *text);

private:
    TgFontText *m_fontText = nullptr;
    float m_fontSize = -1;
    std::string m_fontFile;
    std::vector<TgTextFieldText> m_listText;
    std::vector<TgTextCharacter> m_listCharacter;

    static void fillIntoImage(TgImageAsset &image, float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a,
        const prj_ttf_reader_glyph_data_t *glyph, const prj_ttf_reader_image *ttf_image);

};

#endif // TG_FONT_TO_IMAGE_H
