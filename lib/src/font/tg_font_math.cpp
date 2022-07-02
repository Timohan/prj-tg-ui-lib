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
bool TgFontMath::getFontWidthHeight(const std::vector<TgTextFieldText> &listText, float fontSize, const std::string &mainFontFile,
                                    float &textWidth, float &textHeight, float &allDrawTextHeight, const uint32_t maxLineCount, const float maxLineWidth,
                                    const TgTextFieldWordWrap wordWrap, const bool allowBreakLineGoOverMaxLine)
{
    textWidth = 0;
    textHeight = 0;
    if (listText.empty()) {
        return true;
    }

    TgFontText *fontText = TgFontTextGenerator::generateFontTextInfo(listText, mainFontFile.c_str());
    if (!fontText) {
        return false;
    }
    fontText->generateFontTextInfoGlyphs(fontSize, true);
    if (TgCharacterPositions::generateTextCharacterPositioning(fontText, maxLineCount, maxLineWidth, wordWrap, allowBreakLineGoOverMaxLine)) {
        textWidth = fontText->getTextWidth();
        textHeight = fontText->getFontHeight();
        allDrawTextHeight = fontText->getAllDrawTextHeight();
    }
    delete fontText;
    return true;
}

