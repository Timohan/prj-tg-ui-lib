/*!
 * \file
 * \brief file tg_textfield.h
 *
 * Inits and draws text
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_TEXTFIELD_H
#define TG_TEXTFIELD_H

#include "tg_item2d.h"
#include <string>
#include "../global/tg_global_macros.h"

class TgTextfieldPrivate;
struct TgWindowInfo;

/*!
 * \brief TgTextfieldHorizontalAlign
 * text horizontal align
 */
enum TgTextfieldHorizontalAlign {
    AlignLeft = 0,  /*!< text to set horizontal left */
    AlignCenterH,   /*!< text to set horizontal center */
    AlignRight      /*!< text to set horizontal right */
};

/*!
 * \brief TgTextfieldVerticalAlign
 * text vertical align
 */
enum TgTextfieldVerticalAlign {
    AlignTop = 0,  /*!< text to set vertical top */
    AlignCenterV,  /*!< text to set vertical center */
    AlignBottom    /*!< text to set vertical bottom */
};

/*!
 * \brief TgTextFieldText
 * this is used with function setText(const std::vector<TgTextFieldText> &listText)
 * to set multicolor text
 */
struct TgTextFieldText
{
    std::string m_text;     /*!< utf8 text */
    uint8_t m_textColorR = 0;   /*!< m_text's red color */
    uint8_t m_textColorG = 0;   /*!< m_text's green color */
    uint8_t m_textColorB = 0;   /*!< m_text's blue color */
};

/*!
 * \brief TgTextFieldWordWrap
 *
 */
enum TgTextFieldWordWrap
{
    WordWrapBounded = 0, /*!< default, words will be wrapped to minimum of area */
    WordWrapOff,         /*!< only the line break '\n' will wrap the line */
    WordWrapOn,          /*!< lines are wrapped to ' ' (empty space) or '\n' if it's possible, if there is no space or '\n' at the line, then it's same wrapping as WordWrapBounded */
};

/*!
 * \brief TgTextfield
 * handles text field label functionality (just drawing the text)
 */
class TG_MAINWINDOW_EXPORT TgTextfield : public TgItem2d
{
public:
    explicit TgTextfield(TgItem2d *parent, const char *text, const char *fontFile, float fontSize, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
    explicit TgTextfield(TgItem2d *parent, float x, float y, float width, float height, const char *text, const char *fontFile, float fontSize, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
    ~TgTextfield();
    void setHorizontalAlign(TgTextfieldHorizontalAlign align);
    void setVerticalAlign(TgTextfieldVerticalAlign align);
    TgTextfieldHorizontalAlign getAlignHorizontal() const;
    TgTextfieldVerticalAlign getAlignVertical() const;
    void setText(const char *text);
    void setText(const std::vector<TgTextFieldText> &listText);
    std::string getText() const;
    size_t getCharacterCount();
    uint32_t getCharacterByIndex(size_t index);
    void setFontSize(float fontSize);
    float getFontSize();
    float getTextWidth();
    float getTextHeight();
    std::string getFontFile();
    void setMaxLineCount(uint32_t maxLineCount);
    uint32_t getMaxLineCount() const;
    float getAllDrawTextHeight();
    void setWordWrap(TgTextFieldWordWrap wordWrap);
    TgTextFieldWordWrap getWordWrap() const;
    void setAllowBreakLineGoOverMaxLine(bool allowBreakLineGoOverMaxLine);
    bool getAllowBreakLineGoOverMaxLine() const;
    void getColor(uint8_t &r, uint8_t &g, uint8_t &b);

protected:
    virtual bool render(const TgWindowInfo *windowInfo) override;
    virtual void checkPositionValues() override;

private:
    TgTextfieldPrivate *m_private;

    friend class TgTextEdit;
    friend class TgTexteditPrivate;
};

#endif // TG_TEXTFIELD_H
