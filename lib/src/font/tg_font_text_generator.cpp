/*!
 * \file
 * \brief file tg_font_text_generator.cpp
 *
 * font text generator generates the text glyphs
 * and rendering classes for labels
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_text_generator.h"
#include <prj-ttf-reader.h>
#include "../global/tg_global_log.h"
#include "cache/tg_font_characters_cache.h"
#include "../global/tg_global_application.h"
#include "tg_font_default.h"
#include "tg_font_text.h"

/*!
 * \brief TgFontText::generateFontTextInfo
 *
 * generate TgFontText for the text
 *
 * \param utf8text [in] text
 * \param mainFontFile [in] main font file
 */
TgFontText *TgFontTextGenerator::generateFontTextInfo(const std::string &utf8text, const std::string &mainFontFile)
{
    uint32_t *list_characters = nullptr, list_characters_size;
    if (prj_ttf_reader_get_characters(utf8text.c_str(), &list_characters, &list_characters_size)) {
        TG_ERROR_LOG("Invalid UTF-8 text: ", utf8text);
        return nullptr;
    }

    TgFontText *ret = new TgFontText();
    std::vector<std::string> listFontFiles = TgGlobalApplication::getInstance()->getFontDefault()->getListFont();
    ret->setFontFileNames(mainFontFile, listFontFiles);

    for (uint32_t i=0;i<list_characters_size;i++) {
        ret->addCharacter(list_characters[i]);
    }

    if (list_characters) {
        free(list_characters);
    }
    return ret;
}

