/*!
 * \file
 * \brief file tg_textfield_private.h
 *
 * it holds general TgTextfieldPrivate class
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_TEXTFIELD_PRIVATE_H
#define TG_TEXTFIELD_PRIVATE_H

#include "../../image/tg_image_assets.h"
#include "../../math/tg_matrix4x4.h"
#include "../../font/tg_character_positions.h"
#include <string>
#include <mutex>
#include "../tg_textfield.h"
#include "../../font/tg_font_text_generator.h"

struct TgFontText;
class TgItem2d;
struct TgWindowInfo;
class TgItem2dPosition;

class TgTextfieldPrivate
{
public:
    explicit TgTextfieldPrivate(TgItem2d *currentItem, const char *text, const char *fontFile, float fontSize, uint8_t r, uint8_t g, uint8_t b);
    ~TgTextfieldPrivate();
    bool render(const TgWindowInfo *windowInfo, TgItem2d *current, TgItem2dPosition *itemPosition, float opacity);
    void checkPositionValues();
    void setHorizontalAlign(TgItem2d *currentItem, TgTextfieldHorizontalAlign align);
    void setVerticalAlign(TgItem2d *currentItem, TgTextfieldVerticalAlign align);
    TgTextfieldHorizontalAlign getAlignHorizontal() const;
    TgTextfieldVerticalAlign getAlignVertical() const;
    std::string getText() const;
    void setText(const char *text, TgItem2d *currentItem);
    void setText(const std::vector<TgTextFieldText> &listText, TgItem2d *currentItem);
    void editText(std::vector<uint32_t>&listAddCharacter, const size_t startCharacterIndex, const size_t characterCountToRemove);
    void setFontSize(float fontSize);
    float getFontSize();
    size_t getCharacterCount();
    uint32_t getCharacterByIndex(size_t index);
    void getTextPosition(const size_t cursorPosition, float &positionX);
    size_t getTextCharacterIndex(float x);
    bool isEqualText(const std::vector<TgTextFieldText> &listText);
    bool isEqualTextColor(const std::vector<TgTextFieldText> &listText);
    std::string getFontFile();
    float getTextWidth();
    float getTextHeight();
    void setMaxLineCount(uint32_t maxLineCount);
    uint32_t getMaxLineCount() const;
    float getAllDrawTextHeight();
    void setWordWrap(TgTextFieldWordWrap wordWrap);
    TgTextFieldWordWrap getWordWrap() const;
    void setAllowBreakLineGoOverMaxLine(bool allowBreakLineGoOverMaxLine);
    bool getAllowBreakLineGoOverMaxLine() const;
    void getColor(uint8_t &r, uint8_t &g, uint8_t &b);

private:
    TgItem2d *m_currentItem;
    TgFontText *m_fontText;
    uint8_t m_r, m_g, m_b;
    std::vector<TgTextFieldText> m_listText;
    std::string m_fontFile;
    float m_fontSize;
    bool m_initDone;
    uint32_t m_maxLineCount;
    TgTextFieldWordWrap m_wordWrap;
    bool m_allowBreakLineGoOverMaxLine;
    float m_previousTextWidthCalc;
    std::vector<TgTextCharacter> m_listCharacter;
    std::vector<TgMatrix4x4>m_listTransform;
    TgTextfieldHorizontalAlign m_alignHorizontal;
    TgTextfieldVerticalAlign m_alignVertical;
    mutable std::recursive_mutex m_mutex;

    void generateTransform(TgItem2d *currentItem);
};

#endif // TG_TEXTFIELD_PRIVATE_H
