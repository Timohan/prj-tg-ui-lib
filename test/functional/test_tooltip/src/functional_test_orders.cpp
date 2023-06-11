#include "functional_test_orders.h"
#include <fstream>
#include <string>
#include "../../../../lib/src/global/tg_global_log.h"

#ifndef ORDERS_FILE
#define ORDERS_FILE "orders/orders.txt"
#endif

bool FunctionalTestOrders::loadOrders()
{
    std::ifstream ordersFile(ORDERS_FILE);
    size_t i;
    size_t textPos;
    size_t lineIndex = 0;
    bool ignoreLines = false;

    if (ordersFile.is_open()) {
        std::string line;
        while (std::getline(ordersFile, line)) {
            TestOrder orders;
            lineIndex++;
            if (line.compare(0, 2, "/*") == 0) {
                ignoreLines = true;
                continue;
            } else if (line.compare(0, 2, "*/") == 0) {
                ignoreLines = false;
                continue;
            }
            if (ignoreLines) {
                continue;
            }
            orders.m_lineNumber = lineIndex;
            if (line.compare(0, 4, "MMC ") == 0) {
                orders.m_type = TestOrderType::MouseMoveClick;
                textPos = 0;
                for (i=0;i<8;i++) {
                    std::string text = getNextText(line.c_str()+4+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isHover") {
                orders.m_type = TestOrderType::IsCorrectHover;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isButtonDownCount") {
                orders.m_type = TestOrderType::IsButtonDownCount;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isHoverCount") {
                orders.m_type = TestOrderType::isHoverCount;
                textPos = getNextText(line).size()+1;
                for (i=0;i<1;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "setVisible") {
                orders.m_type = TestOrderType::setVisibleItem;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isVisible") {
                orders.m_type = TestOrderType::isVisible;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "msg") {
                orders.m_type = TestOrderType::NormalInfoMessage;
                textPos = getNextText(line).size()+1;
                orders.m_listString.push_back(line.c_str()+textPos);
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isMove") {
                orders.m_type = TestOrderType::isMove;
                textPos = getNextText(line).size()+1;
                for (i=0;i<6;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isMousePressed") {
                orders.m_type = TestOrderType::isMousePressed;
                textPos = getNextText(line).size()+1;
                for (i=0;i<3;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isMouseReleased") {
                orders.m_type = TestOrderType::isMouseReleased;
                textPos = getNextText(line).size()+1;
                for (i=0;i<4;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isMouseClicked") {
                orders.m_type = TestOrderType::isMouseClicked;
                textPos = getNextText(line).size()+1;
                for (i=0;i<3;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "setEnabled") {
                orders.m_type = TestOrderType::setEnabledItem;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isEnabled") {
                orders.m_type = TestOrderType::isEnabled;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "getMouseCursorOnHover") {
                orders.m_type = TestOrderType::getMouseCursorOnHover;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isItemSelected") {
                orders.m_type = TestOrderType::isItemSelected;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "setSelected") {
                orders.m_type = TestOrderType::setSelected;
                textPos = getNextText(line).size()+1;
                for (i=0;i<2;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "isImage") {
                orders.m_type = TestOrderType::isImage;
                textPos = getNextText(line).size()+1;

                std::string text = getNextText(line.c_str()+textPos);
                if (text.size() == 0) {
                    TG_ERROR_LOG("Line is incorrect ", lineIndex );
                    return false;
                }
                orders.m_listString.push_back(text);
                m_listOrder.push_back(orders);
            } else if (getNextText(line) == "MakeStep") {
                orders.m_type = TestOrderType::MakeStep;
                textPos = getNextText(line).size()+1;
                for (i=0;i<1;i++) {
                    std::string text = getNextText(line.c_str()+textPos);
                    if (text.size() == 0) {
                        TG_ERROR_LOG("Line is incorrect ", lineIndex );
                        return false;
                    }
                    orders.m_listNumber.push_back(std::atoi(text.c_str()));
                    textPos += text.size() + 1;
                }
                m_listOrder.push_back(orders);
            }
        }
        ordersFile.close();
    }
    return true;
}

std::string FunctionalTestOrders::getNextText(const std::string &text)
{
    size_t i;
    for (i=0;i<text.size();i++) {
        if (text.at(i) == ' ' || text.at(i) == '\r'  || text.at(i) == '\n'  || text.at(i) == '\t') {
            std::string ret = text;
            ret.resize(i);
            return ret;
        }
    }
    return text;
}

size_t FunctionalTestOrders::getOrdersCount()
{
    return m_listOrder.size();
}

TestOrder *FunctionalTestOrders::getTestOrder(size_t i)
{
    return &m_listOrder.at(i);
}
