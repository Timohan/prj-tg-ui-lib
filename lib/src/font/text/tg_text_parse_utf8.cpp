/*!
 * \file
 * \brief file tg_text_parse_utf8.cpp
 *
 * Text parse & compare utf-8
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_text_parse_utf8.h"
#include "../../global/tg_global_log.h"
#include <cstring>

/*!
 * \brief TgTextParseUtf8::generateCharactedIndexToUtf8
 *
 * conversts utf32 to utf8
 *
 * \param character [in] fills character to newCharacter as utf8
 * \param newCharacter [out] character will be filled here as in utf8
 */
void TgTextParseUtf8::generateCharactedIndexToUtf8(uint32_t character, char newCharacter[5])
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

/**
 * @brief convert first character from text utf8 to utf32 character
 *
 * @param text first character will be taken from this text
 * @param character [out] parsed character out here
 * @param characterSize [out] size of the character
 * @return true if character and character_size was added
 * @return false if the next character not able to get
 */
bool TgTextParseUtf8::convertNextUtf8CharacterToUtf32(const char *text, uint32_t *character, uint32_t *characterSize)
{
    if (!text) {
        return false;
    }

    // hex 0x0 – 0x7F == 1 byte
    if (text[0] >= 0) {
        *character = static_cast<uint32_t>(text[0]);
        *characterSize = 1;
        return true;
    }

    uint32_t tmp;
    // 110xxxxx 10xxxxxx == 2 bytes
    // 0x0080 to 0x07FF
    if ((text[0] & 0xE0) == 0xC0 && (text[1] & 0xC0) == 0x80) {
        tmp = (static_cast<uint32_t>(text[0] & 0x1F) << 6) | static_cast<uint32_t>(text[1] & 0x3F);
        if (tmp >= 0x80) {
            *character = tmp;
            *characterSize = 2;
            return true;
        }
    }

    // 1110xxxx 10xxxxxx 10xxxxxx
    // 0x0800 to 0xFFFF
    if ((text[0] & 0xF0) == 0xE0 && (text[1] & 0xC0) == 0x80 && (text[2] & 0xC0) == 0x80) {
        tmp = (static_cast<uint32_t>(text[0] & 0x0F) << 12) | (static_cast<uint32_t>(text[1] & 0x3F) << 6) | static_cast<uint32_t>(text[2] & 0x3F);
        if (tmp >= 0x0800) {
            *character = tmp;
            *characterSize = 3;
            return true;
        }
    }

    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    // 0x10000 to 0x10FFFF
    if ((text[0] & 0xF8) == 0xF0 && (text[1] & 0xC0) == 0x80 && (text[2] & 0xC0) == 0x80 && (text[3] & 0xC0) == 0x80) {
        tmp = (static_cast<uint32_t>(text[0] & 0x07) << 18) | (static_cast<uint32_t>(text[1] & 0x3F) << 12) | (static_cast<uint32_t>(text[2] & 0x3F) << 6) | static_cast<uint32_t>(text[3] & 0x3F);
        if (tmp >= 0x10000) {
            *character = tmp;
            *characterSize = 4;
            return true;
        }
    }

    return false;
}

/**
 * @brief compare the text0 and text1
 *
 * @param text0
 * @param text1
 * @param caseSensitive if true then case sensitive, if false then not
 * @return TgTextParseUtf8::TextParseResult result of the text
 */
TgTextParseUtf8::TextParseResult TgTextParseUtf8::compareText(const std::string &text0, const std::string &text1, bool caseSensitive)
{
    if (text0.empty() && text1.empty()) {
        return TgTextParseUtf8::TextParseResult::TextIsEqual;
    }
    if (text0.empty()) {
        return TgTextParseUtf8::TextParseResult::Text0IsSmaller;
    }
    if (text1.empty()) {
        return TgTextParseUtf8::TextParseResult::Text0IsSmaller;
    }
    return compareText(text0.c_str(), text1.c_str(), text0.size(), text1.size(), caseSensitive);
}

/**
 * @brief compare the text0 and text1
 *
 * @param text0
 * @param text1
 * @param textSize0
 * @param textSize1
 * @param caseSensitive if true then case sensitive, if false then not
 * @return TgTextParseUtf8::TextParseResult result of the text
 */
TgTextParseUtf8::TextParseResult TgTextParseUtf8::compareText(const char *text0, const char *text1, size_t textSize0, size_t textSize1, bool caseSensitive)
{
    bool result0;
    bool result1;
    uint32_t character0;
    uint32_t character1;
    uint32_t characterSize0;
    uint32_t characterSize1;
    int lowerCharacter0;
    int lowerCharacter1;
    while (1) {
        result0 = textSize0 != 0 && convertNextUtf8CharacterToUtf32(text0, &character0, &characterSize0);
        result1 = textSize1 != 0 && convertNextUtf8CharacterToUtf32(text1, &character1, &characterSize1);
        if (!result0 && !result1) {
            break;
        }
        if (!result0) {
            return TgTextParseUtf8::TextParseResult::TextIsSameBeginBut0IsSmallerSize;
        }
        if (!result1) {
            return TgTextParseUtf8::TextParseResult::TextIsSameBeginBut1IsSmallerSize;
        }
        if (character0 == character1) {
            text0 = text0+characterSize0;
            text1 = text1+characterSize1;
            textSize0 -= characterSize0;
            textSize1 -= characterSize1;
            continue;
        }
        if (characterSize0 == 1 && characterSize1 == 1) {
            if (!caseSensitive && std::tolower(character0) == std::tolower(character1)) {
                text0 = text0+characterSize0;
                text1 = text1+characterSize1;
                textSize0 -= characterSize0;
                textSize1 -= characterSize1;
                continue;
            }
            lowerCharacter0 = std::tolower(character0);
            lowerCharacter1 = std::tolower(character1);
            if (lowerCharacter0 < lowerCharacter1) {
                return TgTextParseUtf8::TextParseResult::Text0IsSmaller;
            }
            if (lowerCharacter0 > lowerCharacter1) {
                return TgTextParseUtf8::TextParseResult::Text1IsSmaller;
            }
            if (character0 < character1) {
                return TgTextParseUtf8::TextParseResult::Text0IsSmaller;
            }
            return TgTextParseUtf8::TextParseResult::Text1IsSmaller;
        }
        if (character0 < character1) {
            return TgTextParseUtf8::TextParseResult::Text0IsSmaller;
        }
        return TgTextParseUtf8::TextParseResult::Text1IsSmaller;
    }
    return TgTextParseUtf8::TextParseResult::TextIsEqual;
}
