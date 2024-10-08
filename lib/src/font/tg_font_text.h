/*!
 * \file
 * \brief file tg_font_text.h
 *
 * font text holds the general information about text
 * that is generated by TgFontTextGenerator
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_FONT_TEXT_H
#define TG_FONT_TEXT_H

#include <vector>
#include <string>
#include <mutex>

struct TgFontInfo;
struct TgFontInfoData;

struct TgFontTextCharacterInfo
{
    uint32_t m_character;           /*!< character index */
    int32_t m_fontFileNameIndex;    /*!< character is drawed from this TgFontText's index, from m_listFontFileNames, -1 == ignored */

    float positionLeftX;            /*!< glyph X position, generated in TgCharacterPositions::generateTextCharacterPositioning */
    size_t m_characterInFontInfoIndex = 0; /*!< glyph index in TgFontInfo, generated in TgCharacterPositions::generateTextCharacterPositioning */
    uint32_t m_lineNumber;          /*!< line number, required to calculate glyph Y position, generated in TgCharacterPositions::generateTextCharacterPositioning */

    uint8_t m_textColorR;
    uint8_t m_textColorG;
    uint8_t m_textColorB;

    bool m_draw { true };           /*!< false == not draw */
};

class TgFontText
{
public:
    TgFontText();

    void setFontFileNames(const std::string &mainFontFile, const std::vector<std::string> &listFontFileNames);
    void addCharacter(uint32_t character, uint8_t r, uint8_t g, uint8_t b);
    static void addCharacter(std::vector<TgFontTextCharacterInfo>&listCharacter, uint32_t character, uint8_t r, uint8_t g, uint8_t b, const std::vector<std::string> &listFontFileNames);
    void generateFontTextInfoGlyphs(float fontSize, bool onlyForCalculation);
    static void generateFontTextInfoGlyphsData(float fontSize, std::vector<TgFontTextCharacterInfo>&listCharacter, std::vector<TgFontInfoData *>&listFontInfo, std::vector<std::string> &listFontFiles);

    size_t getCharacterCount();
    TgFontTextCharacterInfo *getCharacter(size_t i);
    TgFontInfo *getFontInfo(size_t i);

    static std::vector<uint32_t> getCharactersByFontFileNameIndex(int32_t fontFileNameIndex, const std::vector<TgFontTextCharacterInfo>&listCharacter);

    float getTextWidth();
    float getVisibleTopY();
    float getVisibleBottomY();
    uint32_t getAllLineCount();

    float getFontHeight();
    float getLineHeight();
    float getAllDrawTextHeight();

    void setTextWidth(float textWidth);
    void setVisibleTopY(float visibleTopY);
    void setVisibleBottomY(float visibleBottomY);
    void setAllLineCount(uint32_t lineCount);
    void clearListLinesWidth();
    void setListLinesWidth(size_t lineNumber, float lineWidth);
    static void setListLinesWidth(std::vector<float> &listLineWidth, size_t lineNumber, float lineWidth);
    float getTextLineWidth(size_t lineNumber);

    void clearCacheValues(bool useLock);

private:
    std::mutex m_mutex;
    float m_textWidth;
    float m_visibleTopY;
    float m_visibleBottomY;
    uint32_t m_allLineCount;
    std::vector<float>m_listLineWidth;   // [0] line width of the first line

    std::vector<std::string>m_listFontFileNames;
    std::vector<TgFontTextCharacterInfo>m_listCharacter;
    std::vector<TgFontInfo *>m_listFontInfo;
};

#endif // TG_FONT_TEXT_H
