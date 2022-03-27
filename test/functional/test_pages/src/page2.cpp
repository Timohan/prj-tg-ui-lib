#include "page2.h"
#include <iostream>
#include <item2d/tg_pages.h>

Page2::Page2(TgPages *parent) :
    TgPage(parent, 0, 0, 255),
    m_pages(parent),
    m_buttonChange(this, 20, 20, 250, 50, "Change page to page 0"),
    m_buttonChangeSwitchTypeDirect(this, 20, 90, 250, 50, "Change Switch to Direct"),
    m_buttonChangeSwitchTypeFromTop(this, 20, 160, 300, 50, "Change Switch to From up"),
    m_buttonChangeSwitchTypeFromBottom(this, 20, 250, 300, 50, "Change Switch to From bottom"),
    m_buttonChangeSwitchTypeFromLeft(this, 20, 320, 300, 50, "Change Switch to From left"),
    m_buttonChangeSwitchTypeFromRight(this, 20, 390, 300, 50, "Change Switch to From right")
{
    m_buttonChange.connectOnMouseClicked( std::bind(&Page2::onButtonClickPage0, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeSwitchTypeDirect.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromDirect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeSwitchTypeFromTop.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromTop, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeSwitchTypeFromBottom.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromBottom, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeSwitchTypeFromLeft.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromLeft, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    m_buttonChangeSwitchTypeFromRight.connectOnMouseClicked( std::bind(&Page2::onButtonClickFromRight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) );
    this->connectOnVisibleChanged( std::bind(&Page2::onPageVisibleChanged, this, std::placeholders::_1) );

    m_buttonChange.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSelectedChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeDirect.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeDirectSelectedChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromTop.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromTopChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromBottom.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromBottomChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromLeft.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromLeftChanged, this, std::placeholders::_1) );
    m_buttonChangeSwitchTypeFromRight.connectOnSelectedChanged( std::bind(&Page2::onButtonChangeSwitchTypeFromRightChanged, this, std::placeholders::_1) );
}

Page2::~Page2()
{
}

void Page2::onButtonClickPage0(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page 2 button to page 0\n";
    m_pages->setPage(0);
}

void Page2::onButtonClickFromDirect(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch to direct\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_Direct);
}

void Page2::onButtonClickFromTop(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from top\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromTop);
}

void Page2::onButtonClickFromBottom(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from bottom\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromBottom);
}

void Page2::onButtonClickFromLeft(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from left\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromLeft);
}

void Page2::onButtonClickFromRight(TgMouseType type, float x, float y)
{
    (void)type;
    (void)x;
    (void)y;
    std::cout << "Clicked page switch from right\n";
    m_pages->setPageSwitchType(TgPagesPageSwitchType::PageSwitchType_SlideFromRight);
}

void Page2::onPageVisibleChanged(bool visible)
{
    std::cout << "Page2 visible changed: " << visible << "\n";
}

void Page2::onButtonChangeSelectedChanged(bool selected)
{
    std::cout << "Page2::Change page from page 2 Button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeDirectSelectedChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeDirect Button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromTopChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromTop button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromBottomChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromBottom button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromLeftChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromLeft button selected changed: " << selected << "\n";
}

void Page2::onButtonChangeSwitchTypeFromRightChanged(bool selected)
{
    std::cout << "Page2::ChangeSwitchTypeFromRight button selected changed: " << selected << "\n";
}

TgButton *Page2::getButton(int index)
{
    switch (index) {
        case 0: return &m_buttonChange;
        case 1: return &m_buttonChangeSwitchTypeDirect;
        case 2: return &m_buttonChangeSwitchTypeFromTop;
        case 3: return &m_buttonChangeSwitchTypeFromBottom;
        case 4: return &m_buttonChangeSwitchTypeFromLeft;
        case 5: return &m_buttonChangeSwitchTypeFromRight;
        default:
            break;
    }
    return nullptr;
}
