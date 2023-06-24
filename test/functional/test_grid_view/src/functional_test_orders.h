#ifndef FUNCTIONAL_TEST_ORDERS_H
#define FUNCTIONAL_TEST_ORDERS_H

#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>

enum TestOrderType {
    MouseMoveClick = 0,
    IsCorrectHover,         /*< is next event hover */
    IsButtonDownCount,
    isHoverCount,
    setVisibleItem,
    isVisible,
    NormalInfoMessage,
    isMove,
    isMousePressed,
    isMouseReleased,
    isMouseClicked,
    setEnabledItem,
    isEnabled,              /*< is next event enabled */
    getMouseCursorOnHover,  /*< is current item hover */
    isItemSelected,
    setSelected,
    MakeStep,
    isImage,
    MouseMoveRightClick,
    isMenuClicked,
    SendKeyPress,
};

struct TestOrder
{
    TestOrderType m_type;
    std::vector<int>m_listNumber;
    std::vector<std::string>m_listString;
    size_t m_lineNumber;
};

class FunctionalTestOrders
{
public:
    bool loadOrders();
    size_t getOrdersCount();
    TestOrder *getTestOrder(size_t i);

private:
    std::vector<TestOrder>m_listOrder;
    static std::string getNextText(const std::string &text);

};


#endif
