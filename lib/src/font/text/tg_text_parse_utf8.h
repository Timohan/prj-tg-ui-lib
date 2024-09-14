/*!
 * \file
 * \brief file tg_text_parse_utf8.h
 *
 * Text parse & compare utf-8
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_TEXT_PARSE_UTF8_H
#define TG_TEXT_PARSE_UTF8_H

#include <vector>
#include <string>


class TgTextParseUtf8
{
public:
    enum TextParseResult {
        TextIsEqual = 0,                            /*!< both text are same, "ab" <-> "ab" */
        TextIsSameBeginBut0IsSmallerSize,           /*!< text0 is same as text1 beginning, but text1 is longer text "ab" <-> "abc" */
        TextIsSameBeginBut1IsSmallerSize,           /*!< text1 is same as text0 beginning, but text0 is longer text "abc" <-> "ab" */
        Text0IsSmaller,                             /*!< text0 is "smaller", "aa" <-> "ab", or "aaaaaaaaa" <-> "bb", (a < b) */
        Text1IsSmaller                              /*!< text0 is "smaller", "ab" <-> "aa", or "bb" <-> "aaaaaaaa", (a < b)*/
    };

    static TextParseResult compareText(const std::string &text0, const std::string &text1, bool caseSensitive);

    static void generateCharactedIndexToUtf8(uint32_t character, char newCharacter[5]);
    static bool convertNextUtf8CharacterToUtf32(const char *text, uint32_t *character, uint32_t *characterSize);

private:
    static TextParseResult compareText(const char *text0, const char *text1, size_t textSize0, size_t textSize1, bool caseSensitive);
};

#endif // TG_TEXT_PARSE_UTF8_H
