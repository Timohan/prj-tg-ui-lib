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
 * \brief TgFontText::getCharacters
 *
 * gets text (listText) and converts it to list of characters (listCharacter)
 *
 * \param listText [in] text
 * \param listCharacter [out] list of character from listText
 */
void TgFontTextGenerator::getCharacters(const std::vector<TgTextFieldText> &listText, std::vector<uint32_t> &listCharacter)
{
    listCharacter.clear();
    if (listText.empty()) {
        return;
    }
    uint32_t characterIndex;
    uint32_t *list_characters = nullptr, list_characters_size;

    for (size_t i=0;i<listText.size();i++) {
        if (listText.at(i).m_text.empty()) {
            continue;
        }
        if (prj_ttf_reader_get_characters(listText.at(i).m_text.c_str(), &list_characters, &list_characters_size)) {
            TG_ERROR_LOG("Invalid UTF-8 text: ", generateSingleLineText(listText).c_str());
            return;
        }
        for (characterIndex=0;characterIndex<list_characters_size;characterIndex++) {
            listCharacter.push_back(list_characters[characterIndex]);
        }
        if (list_characters) {
            free(list_characters);
            list_characters = nullptr;
        }
    }
    return;
}

/*!
 * \brief TgFontText::generateFontTextInfo
 *
 * generate TgFontText for the text
 *
 * \param listText [in] text
 * \param mainFontFile [in] main font file
 */
TgFontText *TgFontTextGenerator::generateFontTextInfo(const std::vector<TgTextFieldText> &listText, const std::string &mainFontFile)
{
    TgFontText *ret = new TgFontText();
    std::vector<std::string> listFontFiles = TgGlobalApplication::getInstance()->getFontDefault()->getListFont();
    ret->setFontFileNames(mainFontFile, listFontFiles);
    if (listText.empty()) {
        return ret;
    }
    uint32_t *list_characters = nullptr, list_characters_size;
    uint32_t characterIndex;

    for (size_t i=0;i<listText.size();i++) {
        if (listText.at(i).m_text.empty()) {
            continue;
        }
        if (prj_ttf_reader_get_characters(listText.at(i).m_text.c_str(), &list_characters, &list_characters_size)) {
            TG_ERROR_LOG("Invalid UTF-8 text: ", generateSingleLineText(listText).c_str());
            return nullptr;
        }

        for (characterIndex=0;characterIndex<list_characters_size;characterIndex++) {
            ret->addCharacter(list_characters[characterIndex],
                              listText.at(i).m_textColorR,
                              listText.at(i).m_textColorG,
                              listText.at(i).m_textColorB);
        }

        if (list_characters) {
            free(list_characters);
        }
    }

    return ret;
}

/*!
 * \brief TgFontTextGenerator::generateSingleLineText
 *
 * generates the single line of text
 *
 * \param listText [in] text
 * \param fontText [in/out] main font file
 * \return true if changing color was possible
 * false if changing the color was not possible due different text
 */
std::string TgFontTextGenerator::generateSingleLineText(const std::vector<TgTextFieldText> &listText)
{
    std::string ret = "";
    for (size_t i=0;i<listText.size();i++) {
        ret += listText.at(i).m_text;
    }
    return ret;
}

/*!
 * \brief TgFontTextGenerator::changeTextColor
 *
 * change font text's colors - if it's possible (text must be same to be possible)
 * this function makes changing the text colors little faster
 * because it doesn't re-generate textures and other things
 *
 * \param listText [in] text
 * \param fontText [in/out] main font file
 * \return true if changing color was possible
 * false if changing the color was not possible due different text
 */
bool TgFontTextGenerator::changeTextColor(const std::vector<TgTextFieldText> &listText, TgFontText *fontText)
{
    uint32_t *list_characters = nullptr, list_characters_size;
    uint32_t characterIndex;
    size_t fontTextIndex = 0;
    bool ret = true;

    for (size_t i=0;i<listText.size() && ret;i++) {
        if (prj_ttf_reader_get_characters(listText.at(i).m_text.c_str(), &list_characters, &list_characters_size)) {
            TG_ERROR_LOG("Invalid UTF-8 text: ", generateSingleLineText(listText).c_str());
            return false;
        }

        for (characterIndex=0;characterIndex<list_characters_size;characterIndex++) {
            if (fontText->getCharacter(fontTextIndex)->m_character != list_characters[characterIndex]) {
                ret = false;
                break;
            }
            fontText->getCharacter(fontTextIndex)->m_textColorR = listText.at(i).m_textColorR;
            fontText->getCharacter(fontTextIndex)->m_textColorG = listText.at(i).m_textColorG;
            fontText->getCharacter(fontTextIndex)->m_textColorB = listText.at(i).m_textColorB;
        }

        if (list_characters) {
            free(list_characters);
        }
    }
    return ret;
}