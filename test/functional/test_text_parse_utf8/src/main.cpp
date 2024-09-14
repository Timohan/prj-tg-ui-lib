#include <iostream>
#include <cstring>
#include <fstream>
#include "../../../../lib/src/font/text/tg_text_parse_utf8.h"

static void removeEndOfLineMarks(std::string &text)
{
    while (1) {
        if (text.empty()) {
            break;
        }
        if (text.back() == '\n' || text.back() == '\r') {
            text.resize(text.size()-1);
            continue;
        }
        break;
    }
}

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc , char *argv[])
{
    std::ifstream ordersFile(ORDERS_FILE);
    if (ordersFile.is_open()) {
        TgTextParseUtf8::TextParseResult result;
        std::string line;
        std::string text0;
        std::string text1;
        int32_t lineIndex = 0;
        bool caseSensitive = true;
        while (std::getline(ordersFile, line)) {
            lineIndex++;
            if (line.front() == '#') {
                continue;
            }
            removeEndOfLineMarks(line);
            if (line.compare(0, strlen("Text0: "), "Text0: ") == 0) {
                text0 = line.substr(strlen("Text0: "));
                continue;
            }
            if (line.compare(0, strlen("Text1: "), "Text1: ") == 0) {
                text1 = line.substr(strlen("Text1: "));
                continue;
            }
            if (line.compare(0, strlen("CaseSensitive: "), "CaseSensitive: ") == 0) {
                if (strstr(line.c_str(), "true")) {
                    caseSensitive = true;
                    continue;
                }
                if (strstr(line.c_str(), "false")) {
                    caseSensitive = false;
                    continue;
                }
                std::cout << "Incorrect line: " << line << "\n";
                return 1;
            }
            if (line.compare(0, strlen("Result: "), "Result: ") == 0) {
                result = TgTextParseUtf8::compareText(text0, text1, caseSensitive);
                if (strstr(line.c_str(), "TextIsEqual")) {
                    if (result == TgTextParseUtf8::TextParseResult::TextIsEqual) {
                        continue;
                    }
                    std::cout << "Incorrect result, should be: TextIsEqual but it is: " << result << " Line: " << lineIndex << std::endl;
                    return 1;
                }
                if (strstr(line.c_str(), "TextIsSameBeginBut0IsSmallerSize")) {
                    if (result == TgTextParseUtf8::TextParseResult::TextIsSameBeginBut0IsSmallerSize) {
                        continue;
                    }
                    std::cout << "Incorrect result, should be: TextIsSameBeginBut0IsSmallerSize but it is: " << result << " Line: " << lineIndex << std::endl;
                    return 1;
                }
                if (strstr(line.c_str(), "TextIsSameBeginBut1IsSmallerSize")) {
                    if (result == TgTextParseUtf8::TextParseResult::TextIsSameBeginBut1IsSmallerSize) {
                        continue;
                    }
                    std::cout << "Incorrect result, should be: TextIsSameBeginBut1IsSmallerSize but it is: " << result << " Line: " << lineIndex << std::endl;
                    return 1;
                }
                if (strstr(line.c_str(), "Text0IsSmaller")) {
                    if (result == TgTextParseUtf8::TextParseResult::Text0IsSmaller) {
                        continue;
                    }
                    std::cout << "Incorrect result, should be: Text0IsSmaller but it is: " << result << " Line: " << lineIndex << std::endl;
                    return 1;
                }
                if (strstr(line.c_str(), "Text1IsSmaller")) {
                    if (result == TgTextParseUtf8::TextParseResult::Text1IsSmaller) {
                        continue;
                    }
                    std::cout << "Incorrect result, should be: Text1IsSmaller but it is: " << result << " Line: " << lineIndex << std::endl;
                    return 1;
                }
                std::cout << "Incorrect result, it is: N/A " << result << " Line: " << lineIndex << std::endl;
                return 1;
            }
        }
    }
    std::cout << "All tests OK\n";
    return 0;
}