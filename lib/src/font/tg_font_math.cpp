/*!
 * \file
 * \brief file tg_font_math.cpp
 *
 * This calculates font height and width
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_font_math.h"
#include <prj-ttf-reader.h>
#include "../global/tg_global_application.h"
#include "tg_font_text.h"
#include "tg_font_text_generator.h"
#include "../global/tg_global_log.h"
#include <cmath>

/*!
 * \brief TgFontMath::getFontWidthHeight
 *
 * calculates get text width and text height
 * without required to be drawed
 *
 * \param listText [in] text
 * \param fontSize font size
 * \param mainFontFile main font file
 * \param textWidth [out] text's width
 * \param textHeight [out] text's height
 * \param allDrawTextHeight [out] text's draw height (containing all lines)
 * \param maxLineCount [in] max line count, 0 unlimited number of lines
 * \param maxLineWidth [in] max line width
 * \return true on success
 */
bool TgFontMath::getFontWidthHeight(const std::vector<TgTextFieldText> &listText, const float fontSize, const std::string &mainFontFile,
                                    float &textWidth, float &textHeight, float &allDrawTextHeight, const uint32_t maxLineCount, const float maxLineWidth,
                                    const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine)
{
    std::vector<uint32_t> listCharacters;
    TgFontTextGenerator::getCharacters(listText, listCharacters);
    if (listCharacters.empty()) {
        textWidth = 0;
        textHeight = 0;
        allDrawTextHeight = 0;
        return false;
    }
    std::vector<std::string> listFontFiles;
    listFontFiles.push_back(mainFontFile);

    const std::vector<std::string> allListFontFiles = TgGlobalApplication::getInstance()->getFontDefault()->getListFont();
    for (const std::string &s : allListFontFiles) {
        if (std::find(listFontFiles.begin(), listFontFiles.end(), s) == listFontFiles.end()) {
            listFontFiles.push_back(s);
        }
    }
    std::vector<PrjTgFontDrawHelperData> listHelperCharacters;
    TgGlobalApplication::getInstance()->getFontGlyphCache()->loadCharacters(listCharacters,
                                      listFontFiles,
                                      fontSize,
                                      maxLineCount, maxLineWidth,
                                      wordWrap, allowBreakLineGoOverMaxLine,
                                      listHelperCharacters,
                                      textHeight);

    if (!listHelperCharacters.empty()) {
        textWidth = 0;
        size_t lineIndex = listHelperCharacters.at(0).m_lineIndex;
        int endPosition = 0;
        for (size_t i=0;i<listHelperCharacters.size();i++) {
            if (maxLineCount > 0 && maxLineCount <= lineIndex) {
                continue;
            }
            if (listHelperCharacters.at(i).m_lineIndex == lineIndex) {
                if (listHelperCharacters.at(i).m_character != ' '
                    && listHelperCharacters.at(i).m_character != '\n'
                    && listHelperCharacters.at(i).m_character != '\r') {
                    endPosition = listHelperCharacters.at(i).m_glyphDrawX + listHelperCharacters.at(i).m_glyphOnImageDataWidth;
                }
            } else {
                if (textWidth < static_cast<float>(endPosition)) {
                    textWidth = static_cast<float>(endPosition);
                }
                lineIndex = listHelperCharacters.at(i).m_lineIndex;
                endPosition = listHelperCharacters.at(i).m_glyphDrawX + listHelperCharacters.at(i).m_glyphOnImageDataWidth;
            }
        }
        if (textWidth < static_cast<float>(endPosition)) {
            textWidth = static_cast<float>(endPosition);
        }
        if (maxLineCount > lineIndex || maxLineCount == 0) {
            allDrawTextHeight = static_cast<float>(std::ceil(textHeight + static_cast<float>(lineIndex)*static_cast<float>(std::ceil(textHeight*1.5f))));
        } else {
            allDrawTextHeight = static_cast<float>(std::ceil(textHeight + static_cast<float>(maxLineCount-1)*static_cast<float>(std::ceil(textHeight*1.5f))));
        }
    } else {
        textWidth = 0;
        allDrawTextHeight = 0;
    }
    return true;
}
